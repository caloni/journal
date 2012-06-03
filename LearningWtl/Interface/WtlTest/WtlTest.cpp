#include "MyWindow.h"
#include <atlbase.h>
#include <atlwin.h>


// Aloca��o de mem�ria no ATL 7.1.
#pragma comment(lib, "atlthunk.lib")

// N�o se esque�a disso se voc� for usar controles comuns!
#pragma comment(lib, "comctl32.lib")


/*
* No princ�pio, tio Bill disse: 'WinMain!'
*/
int WINAPI WinMain(HINSTANCE inst, HINSTANCE, PSTR, int show)

{
	MyWindow myWin; // nossa janela principal
	MSG msg = { }; // onde guardamos as mensagens

	// N�o se esque�a disso se voc� for usar controles comuns!
	InitCommonControls();

	// Criamos a janela com o estilo padr�o e um t�tulo bacana.
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

	// C�digo de erro da �ltima mensagem.
	return msg.wParam;
}
