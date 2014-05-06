#pragma once
#include <windows.h>

VOID GpaHookInitialize();
FARPROC WINAPI OnGetProcAddress(HMODULE hModule, LPCSTR lpProcName);
