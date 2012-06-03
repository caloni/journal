/** CINC - Módulo de teste do lado cliente.
@author Wanderley Caloni Jr <wanderley@caloni.com.br>
@date jan.2006
*/
#include "client.h"
#include "../cinc/cinc.h"
#include "../cinc/client.h"
#include "../server/server.h"

#include <windows.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


int main()
{
	HRESULT ret;
	Cinc::Client client(SERVER_BASE_NAME);

	fflush(stdin);

	while( ! kbhit() )
	{
		ret = client.Call();
		Sleep(100);
	}

	if( ret == S_OK )
	{
		cout << "Client-server call made successfully!\n";
	}
	else
	{
		cout << "Error 0x" << hex << ret << " trying to call Server\n";
	}

	system("pause");

	return (int) ret;
}
