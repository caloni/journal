#include <windows.h>
#include <tchar.h>

int main()
{
	MessageBox(NULL, _T("Isto é um MessageBox"), 
		_T("Afirmação estúpida"), MB_OK | MB_ICONINFORMATION);
}
