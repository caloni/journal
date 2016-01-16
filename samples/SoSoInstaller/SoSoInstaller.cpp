#include "stdafx.h"
#include <Windows.h>
#include <iostream>


int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Happy installing...\n";
	CreateDirectory(L"C:\\soso", NULL);
	CopyFile(L"myapp.exe", L"c:\\soso\\myapp.exe", FALSE);
	CopyFile(L"mydll.dll", L"c:\\soso\\mydll.dll", FALSE);
	CopyFile(L"myanotherapp.exe", L"c:\\soso\\myanotherapp.exe", FALSE);
	std::cout << "Everything just got fine =)\n";

	return 0;
}
