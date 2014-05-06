#include "Common.h"
#include "MyDialog.h"
#include "Utils.h"
#include <atlbase.h>
#include <atlwin.h>


// Formatação para criaçao de janela.
typedef CWinTraits<WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 
WS_EX_APPWINDOW> MyWindowTraits;


// Como criar uma janela padrão que pinta seu fundo.
class MyWindow : public CWindowImpl<MyWindow, CWindow, MyWindowTraits>,
	public PaintBkgnd<MyWindow, RGB(50,50,50)>
{
public:
	DECLARE_WND_CLASS(_T("MyWindow Class"));

	typedef PaintBkgnd<MyWindow, RGB(50,50,50)> PaintBase; // veja no mapa de msgs

	BEGIN_MSG_MAP(MyWindow)
		MESSAGE_HANDLER(WM_CLOSE, OnClose) // mensagens padrão
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		COMMAND_ID_HANDLER(IDM_ABOUT, OnAbout) // lidando com controles
		CHAIN_MSG_MAP(PaintBase) // repassando mensagens
	END_MSG_MAP()

	LRESULT OnCreate(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled)
	{
		HMENU hmenu = LoadMenu ( _AtlBaseModule.GetResourceInstance(),
			MAKEINTRESOURCE(IDR_MENU1));

		SetMenu ( hmenu );
		return 0;
	}

	LRESULT OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// criação de uma caixa de diálogo
		MyDialog dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnClose(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled)
	{
		DestroyWindow();
		return 0;
	}

	LRESULT OnDestroy(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled)
	{
		PostQuitMessage(0);
		return 0;
	}
};
