#include "MyWindow.h"

CAppModule _Module;

int WINAPI WinMain(HINSTANCE inst, HINSTANCE, PSTR cmdLine, int show)
{
	_Module.Init(NULL, inst);

	MyWindow myWin;
	MSG msg;
	msg.wParam = ERROR_INVALID_PARAMETER;

	if( myWin.CreateEx() )
	{
		myWin.ShowWindow(show);
		myWin.UpdateWindow();

		while ( GetMessage ( &msg, NULL, 0, 0 ) > 0 )
		{
			TranslateMessage ( &msg );
			DispatchMessage ( &msg );
		}
	}

	_Module.Term();

	return msg.wParam;
}
