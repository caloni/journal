// LeakDetectionDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <vld.h>


int * g_crtLeak;
HANDLE g_vmLeak;
HANDLE g_fsLeak;
HMODULE g_dllLeak;

void Leak()
{
	g_crtLeak = new int[100];
	g_vmLeak = VirtualAlloc(NULL, 100 * sizeof(int), MEM_COMMIT, PAGE_READWRITE);
	g_fsLeak = CreateFile(_T("C:\\LeakDetectionDll.tmp"), GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
	g_dllLeak = LoadLibrary(_T("urlmon"));
}
