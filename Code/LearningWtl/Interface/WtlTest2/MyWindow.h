#pragma once
#include "Resource.h"

#define _WTL_USE_CSTRING
#include <atlbase.h> // classes base da ATL
#include <atlapp.h> // classes base da WTL

extern CAppModule _Module; // versão WTL do CComModule
#include <atlwin.h> // classes GUI da ATL
#include <atlframe.h> // classes de janelas frame da WTL
#include <atlmisc.h> // classes utilitárias da WTL (e.g. CString)
#include <atlcrack.h> // macros de mensagens melhoradas da WTL


// Alocação de memória no ATL 7.1.
#pragma comment(lib, "atlthunk.lib")


// Uma janela em forma de frame do WTL
class MyWindow : public CFrameWindowImpl<MyWindow>
{
public:
	// Usando no lugar de DECLARE_WND_CLASS.
	DECLARE_FRAME_WND_CLASS(_T("MyWindow frame class"), IDR_MAINFRAME);

	BEGIN_MSG_MAP(MyWindow)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		CHAIN_MSG_MAP(CFrameWindowImpl<MyWindow>)
	END_MSG_MAP()

	LRESULT OnCreate(LPCREATESTRUCT createStruct)
	{
		SetTimer(1, 1000);
		SetMsgHandled(false);
		return 0;
	}

	void OnDestroy()
	{
		KillTimer(1);
		SetMsgHandled(false);
	}

	void OnTimer(UINT_PTR nIDEvent)
	{
		if( nIDEvent != 1 )
			SetMsgHandled(false);
		else
			RedrawWindow();
	}

	BOOL OnEraseBkgnd(CDCHandle dc)
	{
		CRect rc;
		SYSTEMTIME st;
		CString sTime;

		GetClientRect(&rc);
		GetLocalTime(&st);

		sTime.Format(_T("The time is %d:%02d:%02d"),
			st.wHour, st.wMinute, st.wSecond);

		dc.SaveDC();
		dc.SetBkColor( RGB(255, 153, 0) );
		dc.SetTextColor( RGB(0, 0, 0) );
		dc.ExtTextOut(0, 0, ETO_OPAQUE, rc, sTime, sTime.GetLength(), NULL);
		dc.RestoreDC(-1);

		return TRUE;
	}
};
