#include "../gpahook/gpahook.h"
#include <windows.h>


typedef int (WINAPI FMessageBoxA)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);


int main(int argc, TCHAR* argv[])
{
	GpaHookInitialize();

	HMODULE dll = LoadLibraryA("user32.dll");

	if( dll )
	{
		FMessageBoxA* msgBox = (FMessageBoxA*) GetProcAddress(dll, "MessageBoxA");
		if( msgBox )
		{
			msgBox(NULL, "teste", "teste", 0);
		}
	}
}
