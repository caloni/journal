#include "Resource.h"
#include <windows.h>


BOOL WINAPI DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}


int WINAPI WinMain(HINSTANCE inst, HINSTANCE, PSTR, int)
{
	INT_PTR ret = DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DLG1), 
		NULL, DialogProc, 0);

	return ERROR_SUCCESS;
}
