#include <atlbase.h>
#include <atlwin.h>


/* Pinta o fundo da tela de uma janela. */
template <class T, COLORREF t_crBrushColor>
class PaintBkgnd
{
public:
	PaintBkgnd() { m_hbrBkgnd = CreateSolidBrush(t_crBrushColor); }
	~PaintBkgnd() { DeleteObject ( m_hbrBkgnd ); }

	BEGIN_MSG_MAP(CPaintBkgnd)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()

	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T*   pT = static_cast<T*>(this);
		HDC  dc = (HDC) wParam;
		RECT rcClient;

		pT->GetClientRect ( &rcClient );
		FillRect ( dc, &rcClient, m_hbrBkgnd );
		return 1;    // we painted the background

	}

protected:
	HBRUSH m_hbrBkgnd;
};
