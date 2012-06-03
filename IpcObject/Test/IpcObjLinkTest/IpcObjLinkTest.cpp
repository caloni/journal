/** Teste de linkedição para a biblioteca IpcObject.

Inicialmente o objetivo desse projeto é usar a biblioteca e não depender
das funções da LIBC e derivadas; apenas kernel32, user32, etc.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 08/2006
*/
#include "../../ipcobject.h"


BOOL WINAPI ServerCallback(PVOID context, PVOID msgBuffer, 
									PDWORD msgSize, DWORD bufferSize)
{
	static int counter = 0;
	
	MessageBoxA(NULL, (PSTR) msgBuffer, "IpcObjLinkTest", MB_OK);

	if( ++counter >= 3 )
		return FALSE;
	return TRUE;
}


DWORD WINAPI ServerThread(PVOID pvParam)
{
	HANDLE ipcObjSrv = *(PHANDLE) pvParam;
	return ReceiveIpcObjectMessage(ipcObjSrv, ServerCallback, NULL, 10000);
}


int main()
{
	DWORD ret = ERROR_SUCCESS;
	HANDLE ipcObjSrv = CreateIpcObject(NULL, "IpcObjLinkTest", 2, 0, 0);

	if( ipcObjSrv )
	{
		DWORD tid = 0;
		HANDLE srvThr = CreateThread(NULL, 0, ServerThread, &ipcObjSrv, 0, &tid);

		if( srvThr )
		{
			HANDLE ipcObjClient = OpenIpcObject("IpcObjLinkTest");

			if( ipcObjClient )
			{
				for( int i = 0; i < 10; ++i )
				{
					if( GetIpcObjectStatus(ipcObjClient) == 
						IPC_OBJECT_STATUS_LISTENING )
					{
						ret = SendIpcObjectMessage(ipcObjClient, "1", 2, 0, 0, INFINITE);
						ret = SendIpcObjectMessage(ipcObjClient, "2", 2, "1", 2, INFINITE);
						ret = SendIpcObjectMessage(ipcObjClient, "3", 2, 0, 0, INFINITE);
						break;
					}
					Sleep(1000);
				}

				CloseHandle(ipcObjClient);
			}

			WaitForSingleObject(srvThr, INFINITE);
			CloseHandle(srvThr);
		}

		CloseHandle(ipcObjSrv);
	}

	return (int) ret;
}
