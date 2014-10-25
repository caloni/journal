#include "LibMod.h"
#include <windows.h>
#include <Tlhelp32.h>
#include <stdio.h>

// Essa fun��o N�O � usada pelo nosso App
void UsingNewApis()
{
	// Opa: fun��o moderninha!!
	if( HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL) )
	{
		PROCESSENTRY32 procEntry;

		// Tudo bem, nosso App n�o vai usar essa fun��o.
		if( Process32First(snapshot, &procEntry) )
		{
			printf("Process list:\n");

			do
			{
				printf("%s\n", procEntry.szExeFile);
			}
			while( Process32Next(snapshot, &procEntry) );
		}

		CloseHandle(snapshot);
	}
}
 
