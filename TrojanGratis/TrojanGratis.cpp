#include <windows.h>
#include <shlwapi.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
	CHAR wndTxt[MAX_PATH];

	while( true )
	{
		HWND fgWin = GetForegroundWindow();
		wndTxt[0] = 0;

		if( GetWindowText(fgWin, wndTxt, sizeof(wndTxt)) )
		{
			if( StrStrI(wndTxt, "Banco Itaú") )
			{
				MessageBox(fgWin, "Olá! Gostaria de ser atacado?",
					"Trojan de grátis", MB_OK | MB_ICONINFORMATION);
				break;
			}
		}
	}

	ExitProcess(ERROR_SUCCESS);
}
