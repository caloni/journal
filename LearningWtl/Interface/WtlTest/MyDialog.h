#include "Common.h"
#include <atlbase.h>
#include <atlwin.h>


// Cria��o de uma caixa de di�logo padr�o.
class MyDialog : public CDialogImpl<MyDialog>
{
public:
	enum { IDD = IDD_DLG1 };

	BEGIN_MSG_MAP(MyDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog) // mensagens padr�o
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDC_BTN1, OnOKCancel) // lidando com controles
		COMMAND_ID_HANDLER(IDC_BTN2, OnOKCancel)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CenterWindow();
		return TRUE;    // deixando o sistema cuidar do foco
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		EndDialog(IDCANCEL);
		return 0;
	}

	LRESULT OnOKCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
};
