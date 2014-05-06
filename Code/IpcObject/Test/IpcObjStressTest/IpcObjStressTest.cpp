/** Teste de estresse para verificar o quanto agüenta o nosso sisteminha.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 08/2006
*/
#include "../../ipcobject.h"
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <string.h>
#include <stdarg.h>

using namespace std;


HANDLE g_startEvent;
HANDLE g_printLock;

int IpcObjStressTestMain(int argc, TCHAR* argv[]);


void Print(PCTSTR fmt, ...)
{
	va_list vaList;
	TCHAR msg[0x500];

	va_start(vaList, fmt);
	_vstprintf(msg, fmt, vaList);

	DWORD wait = WaitForSingleObject(g_printLock, 200);

	if( wait == WAIT_OBJECT_0 || wait == WAIT_ABANDONED_0 )
	{
		cout << msg << endl;
		ReleaseMutex(g_printLock);
	}

	OutputDebugString(msg);

	va_end(vaList);
}


DWORD WINAPI IpcObjStressTestThr(PVOID pvArgv)
{
	PTSTR* argv = (PTSTR*) pvArgv;
	DWORD ret = (DWORD) IpcObjStressTestMain(2, argv);
	delete [] argv[1];
	delete [] argv;
	return ret;
}


BOOL CreateBrother(PTSTR procPath, int instance)
{
	BOOL ret = FALSE;
	BOOL createProc = TRUE;

	if( instance == 0 )
	{
		DWORD tid = GetCurrentThreadId();
		CHAR msg[100];
		sprintf(msg, "Server. Tid: %d.", tid);
		MessageBoxA(NULL, msg, "IpcObjStressTestThr", MB_OK);

		g_startEvent = CreateEvent(NULL, TRUE, FALSE, _T("IpcObjStressTestEvt"));
		if( g_startEvent )
			SetEvent(g_startEvent);
		ret = TRUE;
	}
	else
	{
		TCHAR szInst[MAX_PATH] = { };
		_itot(instance - 1, szInst, 10);

		g_startEvent = CreateEvent(NULL, TRUE, FALSE, _T("IpcObjStressTestEvt"));

		if( createProc )
		{
			TCHAR cmdLine[MAX_PATH] = { };

			lstrcpy(cmdLine, procPath);
			lstrcat(cmdLine, _T(" "));
			lstrcat(cmdLine, szInst);

			STARTUPINFO si = { sizeof(si) };
			PROCESS_INFORMATION pi = { };

			if( CreateProcess(NULL, cmdLine, NULL, NULL, 
				FALSE, 0, NULL, NULL, &si, &pi) )
			{
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				ret = TRUE;
			}
		}
		else // cria thread
		{
			PTSTR argv1 = new TCHAR[MAX_PATH];
			lstrcpy(argv1, szInst);
			PTSTR* argv = new PTSTR[3];
			argv[0] = procPath;
			argv[1] = argv1;
			argv[2] = 0;

			DWORD tid = 0;
			HANDLE thr = CreateThread(NULL, 0, IpcObjStressTestThr, argv, 0, &tid);

			if( thr )
			{
				CloseHandle(thr);
				ret = TRUE;
			}
		}

		WaitForSingleObject(g_startEvent, INFINITE);
		Sleep(1000);
	}

	return ret;
}


BOOL WINAPI ServerCallback(PVOID context, PVOID msgBuffer, 
									PDWORD msgSize, DWORD bufferSize)
{
	PTSTR psz = (PTSTR) msgBuffer;
	Print("%s", psz);
	return TRUE;
}


int IpcObjStressTestMain(int argc, TCHAR* argv[])
{
	int ret = -1;

	if( argc == 2 )
	{
		g_printLock = CreateMutex(NULL, FALSE, _T("IpcObjStressTestPrintLock"));
		ret = _tstoi(argv[1]);
		CreateBrother(argv[0], ret); // cria o próximo

		if( ret ) // sou o cliente
		{
			HANDLE ipcObj = OpenIpcObject("IpcObjStressTest");

			if( ! ipcObj )
			{
				Print("Client %d. Error 0x%08X creating object.", ret, GetLastError());
			}
			else
			{
				Print("Client %d. Starting to send messages.", ret);

				while( true )
				{
					DWORD msgSize = (DWORD) _tcslen(argv[1]) * sizeof(TCHAR) + sizeof(TCHAR);
					DWORD err = SendIpcObjectMessage(ipcObj, 
						argv[1], msgSize,
						NULL, 0, 5000);

					if( err != ERROR_SUCCESS )
					{
						Print("Client %d. Error 0x%08X. Exiting.", ret, err);
						break;
					}
				}
			}
		}
		else // sou o servidor
		{
			HANDLE ipcObj = CreateIpcObject(NULL, "IpcObjStressTest", MAX_PATH, 0, 0);

			if( ! ipcObj )
			{
				Print("Server. Error 0x%08X creating object.", GetLastError());
			}
			else
			{
				Print("Server. Starting to receive messages.");
				ReceiveIpcObjectMessage(ipcObj, ServerCallback, NULL, 10000);
			}
		}
	}

	return ret;
}


int _tmain(int argc, TCHAR* argv[])
{
	return IpcObjStressTestMain(argc, argv);
}
