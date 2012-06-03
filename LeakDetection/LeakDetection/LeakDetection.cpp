// LeakDetection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SecondCpp.h"
#include "..\LeakDetectionDll\LeakDetectionDll.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>


int _tmain(int argc, _TCHAR* argv[])
{
	new int;
	malloc(1000);

	if( HMODULE dll = LoadLibrary(_T("LeakDetectionDll")) )
	{
		SecondCppLeak();

		if( FP_Leak leak = (FP_Leak) GetProcAddress(dll, "Leak") )
		{
			leak();
		}

		FreeLibrary(dll);
	}

	return 0;
}
