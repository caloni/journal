#include "LibMod.h"
#include <windows.h>
#include <Tlhelp32.h>
#include <stdio.h>

// Essa fun��o � usada pelo nosso App
int UsingOldApis()
{
	DWORD ver = GetVersion(); // API paleozoica, OK.
	return int( (DWORD)(LOBYTE(LOWORD(ver))) );
}

 
