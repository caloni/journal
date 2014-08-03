#include "stdafx.h"
#include "TaServido.h"

#include <iostream>

using namespace std;


TaServido::TaServido() : Service("TaServido", "TaServido TDC2014")
{
}

void TaServido::Main()
{
	while (_status.dwCurrentState == SERVICE_RUNNING)
	{
		Sleep(1000);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool ok = true;
	TaServido serv;

	if (argc < 2)
	{
		ok = serv.Run();
	}
	else if (strcmp(argv[1], "-install") == 0)
	{
		ok = serv.Install();
	}
	else if (strcmp(argv[1], "-uninstall") == 0)
	{
		ok = serv.Uninstall();
	}
	else if (strcmp(argv[1], "-debug") == 0)
	{
		serv.Debug();
	}

	if ( ! ok )
		cout << "TaServido Service\nOptions:\n-install\n-uninstall\n-debug\n";

	return ok ? 0 : -1;
}
