// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include ".\maindlg.h"

#include <ks.h> //SIZEOF_ARRAY



CMainDlg::CMainDlg()
: m_StrMgr(&m_ComHeap)
{
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	TimerProc(m_hWnd, WM_TIMER, (UINT_PTR)this, 0);
	m_Timer = SetTimer((UINT_PTR)this, 1000, TimerProc);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	RECT rect = { };
	GetWindowRect(&rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	
	width = m_cDayToDay.GetOption(_T("MainWindow"), _T("Width"), width);
	height = m_cDayToDay.GetOption(_T("MainWindow"), _T("Height"), height);

	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	DlgResize_Init(FALSE);
	SetWindowPos(NULL, &rect, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	UIAddChildWindowContainer(m_hWnd);

	return TRUE;
}


LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	RECT rect = { };
	
	GetWindowRect(&rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	m_cDayToDay.SaveOption(_T("MainWindow"), _T("Width"), width);
	m_cDayToDay.SaveOption(_T("MainWindow"), _T("Height"), height);

	KillTimer(m_Timer);
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleString sMsg(&m_StrMgr);

#if defined(_UNICODE) || defined(UNICODE)
#error UNICODE!!
#endif

    if( GetDlgItemText(IDC_EDIT_EVENT, sMsg) )
		m_cDayToDay.SaveMessage(sMsg.GetString());

	CloseDialog(wID);
	return 0;
}



VOID CALLBACK CMainDlg::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	CDayToDay::String sCaption = _T("daytoday - ");
	CMainDlg *pThis = (CMainDlg*) idEvent;

	sCaption += pThis->m_cDayToDay.GetCurrentDT("%Y.%m.%d %H.%M.%S");

	pThis->SetWindowText(sCaption.c_str());
}

LRESULT CMainDlg::OnBnClickedAppAbout2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	TCHAR sFilePath[MAX_PATH] = _T(".");

	GetCurrentDirectory(SIZEOF_ARRAY(sFilePath), sFilePath);
	ShellExecute(m_hWnd, _T("open"), _T("daytoday.log"), 
		NULL, sFilePath, SW_SHOWDEFAULT);
	CloseDialog(0);

	return 0;
}
