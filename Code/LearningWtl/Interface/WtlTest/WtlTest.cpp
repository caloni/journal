#include "MyWindow.h"
#include <atlbase.h>
#include <atlwin.h>


// Alocação de memória no ATL 7.1.
#pragma comment(lib, "atlthunk.lib")

// Não se esqueça disso se você for usar controles comuns!
#pragma comment(lib, "comctl32.lib")


/*
* No princípio, tio Bill disse: 'WinMain!'
*/
int WINAPI WinMain(HINSTANCE inst, HINSTANCE, PSTR, int show)

{
	MyWindow myWin; // nossa janela principal
	MSG msg = { }; // onde guardamos as mensagens

	// Não se esqueça disso se você for usar controles comuns!
	InitCommonControls();

	// Criamos a janela com o estilo padrão e um título bacana.
	if( myWin.Create(NULL, CWindow::rcDefault, _T("This is ATL!! Uhuu!")) )
	{
		myWin.ShowWindow(show); // mostra janela
		myWin.UpdateWindow(); // atualiza janela

		// Loop de mensagens.
		while( GetMessage(&msg, NULL, 0, 0) > 0 )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// Código de erro da última mensagem.
	return msg.wParam;
}
