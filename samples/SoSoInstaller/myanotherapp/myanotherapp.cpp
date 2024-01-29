#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	if (HMODULE mydll = LoadLibrary(L"mydll.dll"))
	{
		getchar();
	}
	return 0;
}

