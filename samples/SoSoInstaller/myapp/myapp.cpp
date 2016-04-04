#include "stdafx.h"
#include "..\mydll\mydll.h"
#include <Windows.h>
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	if (HMODULE mydll = LoadLibrary(L"mydll.dll"))
	{
		void(*func)() = (void(*)()) GetProcAddress(mydll, "Version1");
		if (func)
			func();
		else
			std::cout << "Error undefinable and indescritable\n";
	}

	return 0;
}

