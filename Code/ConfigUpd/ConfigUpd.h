#pragma once
#include <tchar.h>
#include <windows.h>

#define CONFIG_UPD_SERVICENUMBER 36


enum ConfigUpdFunction
{
	CONFIG_UPD_FUN_SENDFILE = 1,
};


struct ConfigUpdFile
{
	TCHAR fileName[MAX_PATH];
	DWORD fileSize;
	DWORD fileOptions;
	BYTE fileData[1];
};


DWORD InvUpdFile_SendFile(PCTSTR hostName, PCTSTR filePath, DWORD fileOptions);
