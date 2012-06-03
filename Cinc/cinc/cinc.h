/** CINC - Comunicação entre processos.
@author Wanderley Caloni Jr <wanderley@caloni.com.br>
@date jan.2006
*/
#pragma once
#include <tchar.h>

#define CINC_SHARED_MEMORY_SUFFIX "SharedMemory"
#define CINC_CLIENT_CALL_SUFFIX "ClientCall"
#define CINC_CLIENT_SERVER_SUFFIX "ClientServer"
#define CINC_SERVER_CLIENT_SUFFIX "ServerClient"


namespace Cinc
{

class Server;
class Client;

}
