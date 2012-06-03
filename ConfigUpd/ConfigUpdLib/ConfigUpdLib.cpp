#define _CRT_SECURE_NO_WARNINGS
#include "ConfigUpdLib.h"
#include "..\ConfigUpd.h"
#include <RpcSvc\Libraries\RsClientLib\RsClientLib.h>
#include <stdio.h>

#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "RsLib.lib")
#pragma comment(lib, "RsClientLib.lib")

using namespace std;


ConfigUpdFile* MountUpdFile(PCTSTR filePath, DWORD fileOptions, PDWORD updFileSize)
{
	ConfigUpdFile* ret = NULL;
	DWORD err = ERROR_SUCCESS;
	HANDLE hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if( hFile != INVALID_HANDLE_VALUE )
	{
		DWORD fileSize = GetFileSize(hFile, NULL);

		if( fileSize != INVALID_FILE_SIZE )
		{
			SIZE_T retSize = sizeof(ConfigUpdFile) + fileSize;

			if( ret = (ConfigUpdFile*) LocalAlloc(LPTR, retSize) )
			{
				if( ReadFile(hFile, ret->fileData, fileSize, &ret->fileSize, NULL) )
				{
					if( fileSize == ret->fileSize )
					{
						if( PCTSTR p = _tcsrchr(filePath, _T('\\')) )
							_tcscpy(ret->fileName, p + 1 );
						else
							_tcscpy(ret->fileName, filePath);
						
						ret->fileOptions = fileOptions;
						*updFileSize = retSize;
					}
					else err = ERROR_FILE_CORRUPT;
				}
				else err = GetLastError();

				if( err != ERROR_SUCCESS )
				{
					LocalFree(ret);
					ret = NULL;
				}
			}
			else err = GetLastError();
		}
		else err = GetLastError();

		CloseHandle(hFile);
	}
	else err = GetLastError();

	SetLastError(err);
	return ret;
}


void UnmountUpdFile(ConfigUpdFile* updFile)
{
	LocalFree(updFile);
}


DWORD ConfigUpd::SendFile(PCTSTR hostName, PCTSTR filePath, DWORD fileOptions)
{
	DWORD ret = ERROR_SUCCESS;
	RpcSvc::Client client(hostName);
	ConfigUpdFile* updFile = NULL;
	DWORD updFileSize = 0;
	
	printf("Sending file %s, options=%08X\n", filePath, fileOptions, &updFileSize);

	updFile = MountUpdFile(filePath, fileOptions, &updFileSize);

	if( updFile )
	{
		long err = 0;
		long outLen = 0;
		PBYTE out = NULL;
		RPC_STATUS rpcErr = client.SendMsg(CONFIG_UPD_SERVICENUMBER, CONFIG_UPD_FUN_SENDFILE, 
			updFileSize, (PBYTE) updFile, &outLen, &out, &err);

		if( rpcErr == RPC_S_OK )
			ret = err;
		else
			ret = rpcErr;

		UnmountUpdFile(updFile);
	}
	else ret = GetLastError();

	if( ret != ERROR_SUCCESS )
		printf("Error %d\n", ret);

	return ret;
}
