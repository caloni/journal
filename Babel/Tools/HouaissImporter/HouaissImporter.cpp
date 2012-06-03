#define _CRT_SECURE_NO_WARNINGS

#include "..\..\Libraries\BabelBaseLib\BabelBaseLib.h"
#include <windows.h>
#include <shlwapi.h>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "BabelBase.lib")

#define HOUAISS_UNINSTALL_SUBKEY "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Houaiss"
#define HOUAISS_UNINSTALL_VALUE "UninstallString"


using namespace std;


static CHAR g_houaissPath[MAX_PATH];
static CHAR g_decryptFilePath[MAX_PATH];


int GetHouaissPath()
{
	DWORD ret;
	HKEY houaissKey;

	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, HOUAISS_UNINSTALL_SUBKEY, 0, 
		KEY_QUERY_VALUE, &houaissKey);

	// abrimos a chave que diz onde está o desinstalador
	if( ret == ERROR_SUCCESS )
	{
		DWORD houaissPathSz = sizeof(g_houaissPath);
		ret = RegQueryValueEx(houaissKey, HOUAISS_UNINSTALL_VALUE, NULL, NULL, 
			(PBYTE) g_houaissPath, &houaissPathSz);
		RegCloseKey(houaissKey);
	}

	// lemos o valor de onde está o desinstalador
	if( ret == ERROR_SUCCESS )
	{
		// tiramos o nome, pegamos o path
		if( char* slash = strrchr(g_houaissPath, '\\') )
			slash[0] = 0;

		// esse path existe mesmo?
		if( ! PathFileExists(g_houaissPath) )
			ret = GetLastError();
	}

	// não conseguimos, perguntamos para o usuário
	if( ret != ERROR_SUCCESS )
	{
		ret = ERROR_SUCCESS;

		// em que PASTA está o houaiss?
		printf("Onde o Houaiss esta instalado?\n");
		fgets(g_houaissPath, sizeof(g_houaissPath), stdin);
		if( char* newLine = strrchr(g_houaissPath, '\n') )
			*newLine = 0;

		// esse path existe mesmo?
		if( ! PathFileExists(g_houaissPath) )
			ret = GetLastError();
	}

	return ret;
}


int DecryptHouaiss()
{
	DWORD ret = ERROR_SUCCESS;
	CHAR houaissFileName[MAX_PATH];
	DWORD houaissFileSize = 0;
	PVOID houaissView = NULL;
	PVOID decryptView = NULL;

	// esse é o arquivo do Houaiss que precisamos
	sprintf(houaissFileName, "%s\\Dicionario\\deah001.dhx", g_houaissPath);

	// esse é o arquivo temporário que será decriptografado
	CHAR tempPath[MAX_PATH] = ".";
	GetTempPath(MAX_PATH, tempPath);
	GetTempFileName(tempPath,  "HoI", 0, g_decryptFilePath);


	// vamos abrir e visualizar ambos os arquivos
	HANDLE houaissFile = CreateFile(houaissFileName, GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if( houaissFile == INVALID_HANDLE_VALUE )
		ret = GetLastError();

	HANDLE decryptFile = CreateFile(g_decryptFilePath, 
		GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, 0, NULL);
	if( decryptFile == INVALID_HANDLE_VALUE )
		ret = GetLastError();

	if( ret == ERROR_SUCCESS )
	{
		houaissFileSize = GetFileSize(houaissFile, NULL);

		if( houaissFileSize != INVALID_FILE_SIZE )
		{
			ret = SetFilePointer(decryptFile, houaissFileSize, 
				NULL, FILE_BEGIN);

			ret = ret == INVALID_SET_FILE_POINTER || ! SetEndOfFile(decryptFile) ?
				GetLastError() : ERROR_SUCCESS;

			if( ret == ERROR_SUCCESS )
			{
				HANDLE houaissMap = CreateFileMapping(houaissFile, NULL, 
					PAGE_READONLY, 0, 0, NULL);
				if( ! houaissMap )
					ret = GetLastError();

				HANDLE decryptMap = CreateFileMapping(decryptFile, NULL,
					PAGE_READWRITE, 0, 0, NULL);
				if( ! decryptMap )
					ret = GetLastError();

				if( ret == ERROR_SUCCESS )
				{
					houaissView = MapViewOfFile(houaissMap, FILE_MAP_READ, 
						0, 0, 0);
					if( ! houaissView )
						ret = GetLastError();

					decryptView = MapViewOfFile(decryptMap, FILE_MAP_ALL_ACCESS, 
						0, 0, 0);
					if( ! decryptView )
						ret = GetLastError();
				}

				if( houaissMap )
					CloseHandle(houaissMap);
				if( decryptMap )
					CloseHandle(decryptMap);
			}
		}
		else ret = GetLastError();
	}


	// tudo certo para a desencriptação
	if( ret == ERROR_SUCCESS )
	{
		PBYTE inputView = (PBYTE) houaissView;
		PBYTE outputView = (PBYTE) decryptView;

		for( DWORD inputCount = 0; inputCount < houaissFileSize; ++inputCount )
		{
			outputView[inputCount] = inputView[inputCount] + 0x0B;
		}
	}


	// fechamos as views
	if( houaissView )
		UnmapViewOfFile(houaissView);
	if( decryptView )
		UnmapViewOfFile(decryptView);

	// fechamos os arquivos
	if( houaissFile != INVALID_HANDLE_VALUE )
		CloseHandle(houaissFile);
	if( decryptFile != INVALID_HANDLE_VALUE )
		CloseHandle(decryptFile);

	return ret;
}


int ConvertHouaiss()
{
	DWORD ret = ERROR_SUCCESS;
	Babel::IImporter* importer = Babel::GetImporter();

	if( importer )
	{
		ifstream ifs(g_decryptFilePath);

		if( ifs )
		{
			string line;
			string word;

			while( ret == ERROR_SUCCESS && getline(ifs, line) )
			{
				char cmd = line[0];
				
				line.erase(0, 1);

				switch( cmd )
				{
				case '*': // verbete
					word = line;
					break;

				case ':': // definição
					if( ! importer->ImportDefinition(word.c_str(), line.c_str(), 
						"HouaissImporter") )
					{
						ret = GetLastError();
					}
					break;
				}
			}

			ifs.close();
			DeleteFile(g_decryptFilePath);
		}
		else ret = ERROR_FILE_NOT_FOUND;

		Babel::FreeImporter(importer);
	}

	return ret;
}


int main()
{
	int ret = 0;
	
	ret = GetHouaissPath();

	if( ret == 0 )
	{
		ret = DecryptHouaiss();

		if( ret == 0 )
			ConvertHouaiss();
	}

	return ret;
}
