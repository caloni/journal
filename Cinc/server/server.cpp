/** CINC - Módulo de teste do lado servidor.
@author Wanderley Caloni Jr <wanderley@caloni.com.br>
@date jan.2006
*/
#include "server.h"
#include "../cinc/server.h"

#include <iostream>
#include <stdlib.h>

using namespace std;


int main()
{
	HRESULT ret;
	Cinc::Server server(SERVER_BASE_NAME);

	ret = server.Start(false);

	if( ret == S_OK )
	{
		flush(
	}
	else
		cout << "Error 0x" << hex << ret << " trying to start Server\n";

	return (int) ret;
}
