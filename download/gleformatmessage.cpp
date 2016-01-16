#define _CRT_SECURE_NO_WARNINGS // quanta frescura...
#include <tchar.h>
#include <windows.h>
#include <string>

using namespace std;


wstring GetErrorDescription(DWORD errNumber)
{
	wstring ret;
	bool msgOk = false;
	LPVOID lpMsgBuf = NULL;

	if( FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER // aloque pra mim (n�o sei o tamanho)
		| FORMAT_MESSAGE_FROM_SYSTEM // descri��o do erro est� no sistema
		| FORMAT_MESSAGE_IGNORE_INSERTS, // ignora os inserts pra n�o sofrer com hackerzinhos
		NULL, // sem fonte:
		errNumber, // a fonte � o c�digo de erro
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // idioma padr�o
		(LPTSTR)&lpMsgBuf,// isso � um ponteiro para um ponteiro para um buffer que ser� alocado
		0, // nada disso
		NULL // e nem disso
		) > 0 ) // maior que zero quer dizer "beleza!"
	{
		if( lpMsgBuf ) // s� pra...
		{
			ret = (PCWSTR) lpMsgBuf; // ok, vamos usar essa string
			msgOk = true;
			LocalFree(lpMsgBuf); // n�o precisamos mais da mem�ria alocada
		}
	}

	if( ! msgOk ) // alguma coisa n�o deu certo
	{
		wchar_t msgBuf[100]; // o suficiente
		_snwprintf(msgBuf, 100, L"Unknown error (code %d)", errNumber);
		ret = msgBuf;
	}

	return ret;
}


int CALLBACK wWinMain(HINSTANCE, HINSTANCE, PWSTR errNumberStr, int)
{
	int errNumber = _wtoi(errNumberStr);
	wstring errDesc = GetErrorDescription(errNumber);
	MessageBox(NULL, errDesc.c_str(), L"GetLastError", MB_OK | MB_ICONINFORMATION);
	return errNumber;
}
