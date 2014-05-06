/* @file ConfigUpd.cpp

@brief Serviço de transferência de configurações de 
inventário cliente => servidor.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 2009-09
*/
#include "ConfigUpd.h"
#include "ConfigUpdLib\ConfigUpdLib.h"
#include <RpcSvc\Libraries\RsLib\RsLib.h>
#include <RpcSvc\Libraries\RsLib\Plugin.h>
#include <RpcSvc\Libraries\RsLib\Log.h>
#include <tchar.h>
#include <windows.h>

#pragma comment(lib, "RsLib.lib")

using namespace RpcSvc;

FP_AllocateMemory g_allocMem = NULL;
FP_FreeMemory g_freeMem = NULL;


// Chamada ao iniciar serviço.
long RsStartService(long service, 
						FP_AllocateMemory allocMem, 
						FP_FreeMemory freeMem)
{
	SetLog("RsSample");
	Log("Starting RsSampleService. %s %s", __DATE__, __TIME__);

	g_allocMem = allocMem;
	g_freeMem = freeMem;
	Log("Service number %d", service);

	return 0;
}


// Chamada ao terminar serviço.
long RsStopService(long service)
{
	Log("Stopping RcpSvcSampleService");
	Log("Service number %d", service);
	return 0;
}


DWORD ReceiveFile(ConfigUpdFile* updFile)
{
	DWORD ret = ERROR_SUCCESS;
	TCHAR filePath[MAX_PATH];

	if( GetTempFileName(_T("C:\\Hsi\\ConfigUpd"), updFile->fileName, 0, filePath) )
	{
		HANDLE hFile = CreateFile(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

		if( hFile != INVALID_HANDLE_VALUE )
		{
			DWORD written = 0;

			if( WriteFile(hFile, updFile->fileData, updFile->fileSize, &written, NULL) )
			{
				if( written != updFile->fileSize )
					ret = ERROR_FILE_CORRUPT;
			}
			else ret = GetLastError();

			CloseHandle(hFile);
		}
	}
	else ret = GetLastError();

	return ret;
}


long RsSendMsg(long service, long function,
					  long msgLen, PBYTE msgBuf,
					  long* retMsgLen, PBYTE* retMsgBuf)
{
	long ret = 0;
	Log("SendMsg!");

	switch( function )
	{
	case CONFIG_UPD_FUN_SENDFILE:
		{
			ConfigUpdFile* updFile = (ConfigUpdFile*) msgBuf;
			Log("File sent: %s", updFile->fileName);
			ret = ReceiveFile(updFile);
		}
		break;

	default:
		Log("Warning: Invalid function");
		break;
	}

	return ret;
}


DWORD InvUpdFile_SendFile(PCTSTR hostName, PCTSTR filePath, DWORD fileOptions)
{
	DWORD ret = ConfigUpd::SendFile(hostName, filePath, fileOptions);
	return ret;
}
