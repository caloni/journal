#include <windows.h>
#include <tchar.h>

int main()
{
	MessageBox(NULL, _T("Isto � um MessageBox"), 
		_T("Afirma��o est�pida"), MB_OK | MB_ICONINFORMATION);
}
