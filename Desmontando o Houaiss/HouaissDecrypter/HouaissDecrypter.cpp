/** @file HouaissDecrypter.cpp

@brief Desencripta (desofusca) arquivos do Dicionário Eletrônico Houaiss 2.0.

@author Wanderley Caloni (wanderley@caloni.com.br)
@date 2011-07
*/
#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <stdio.h>
 
 
int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR cmdLine, int)
{
	CHAR houPath[MAX_PATH] = { }; // onde está instalado o dicionário
 
	// vamos capturar da linha de comando
	sscanf(cmdLine, "-p \"%[^\"]s", houPath);
 	if( houPath[0] == 0 )
	{
		// modo de usar
		MessageBox(NULL, 
			"How to use:\r\nHouaissDecrypter.exe -p \"C:\\HouaissPath\\\"", 
			"Houaiss Decrypter v. alpha",
			0);
		return 1;
	}
 
	// para cada arquivo do dicionário...
	for( int fileIdx = 1; fileIdx <= 65; ++fileIdx )
	{
		CHAR dicPath[MAX_PATH];

		// primeiro tentamos abrir o arquivo original...
		sprintf(dicPath, "%s\\deah%03d.dhx", houPath, fileIdx);
		HANDLE dicFile = CreateFile(dicPath, GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING, 0, NULL);
 
		if( dicFile != INVALID_HANDLE_VALUE )
		{
			CHAR outPath[MAX_PATH];

			// ... depois abrimos o arquivo de saída
			sprintf(outPath, "deah%03d.txt", fileIdx);
			HANDLE outFile = CreateFile(outPath, GENERIC_READ | GENERIC_WRITE, 0,
				NULL, CREATE_ALWAYS, 0, NULL);

			if( outFile != INVALID_HANDLE_VALUE )
			{
				DWORD fileSize = GetFileSize(dicFile, NULL);
 
				if( SetFilePointer(outFile, fileSize, NULL, FILE_BEGIN) )
				{
					SetEndOfFile(outFile); // mesmo tamanho que arquivo original
 
					// vamos trabalhar com mapeamento, que nos dá ponteiros transparentes
					HANDLE map1 = CreateFileMapping(dicFile, NULL, PAGE_READONLY, 0, 0, NULL);
					HANDLE map2 = CreateFileMapping(outFile, NULL, PAGE_READWRITE, 0, 0, NULL);
 
					if( map1 && map2 )
					{
						PBYTE view1 = (PBYTE) MapViewOfFile(map1, FILE_MAP_READ, 0, 0, 0);
						PBYTE view2 = (PBYTE) MapViewOfFile(map2, FILE_MAP_WRITE, 0, 0, 0);
 
						if( view1 && view2 )
						{
							for( DWORD i = 0; i < fileSize; ++i )
							{
								// todo esse código apenas para isso
								view2[i] = view1[i] + 0x0B;
							}
						}
 
						if( view1 )
							UnmapViewOfFile(view1);
						if( view2 )
							UnmapViewOfFile(view2);
					}
 
					if( map1 )
						CloseHandle(map1);
					if( map2 )
						CloseHandle(map2);
				}

				CloseHandle(outFile);
			}

			CloseHandle(dicFile);
		}
	}
 
	return 0;
}
