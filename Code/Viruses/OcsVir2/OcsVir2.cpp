/** @file Exemplo de programa que infecta outros programas.

@author Wanderley Caloni
@date dec.2006
*/
#include <windows.h>


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if( fdwReason == DLL_PROCESS_ATTACH )
		MessageBox(NULL, "Bem-vindo, caro usuário, ao maravilhoso mundo do "
		"Bloco de Notas, mundialmente conhecido como NOTEPAD!! "
		"Palmas para ele!!", "Microsoft Notepad 2020 Enterprise Edition", 0);
	return TRUE;
}
