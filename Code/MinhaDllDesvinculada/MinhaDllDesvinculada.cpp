// MinhaDllDesvinculada.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <iostream>

__declspec(dllexport) void FuncaoDificilDeLinkar()
{
	std::cout << "Achou!\n";
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

