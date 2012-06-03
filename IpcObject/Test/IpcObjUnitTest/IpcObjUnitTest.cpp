/** @file

Funções comuns para a execução dos testes da biblioteca IpcObject.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 08/2006
*/
#include "ipcobjunittest.h"
#include <shlwapi.h>
#include <tchar.h>
#include <iostream>


using namespace std;


/** Um objeto padrão que recebe strings em C, imprime na saída e muda a string
para "pong!".

@remark Não possui condição de saída, ou seja, deve ser controlado pelo 
timeout na hora da criação do loop de mensagens.
*/
BOOL WINAPI ServerCallbackNormal(PVOID context, PVOID msgBuffer, PDWORD msgSize, DWORD bufferSize)
{
	if( msgSize )
	{
		PSTR msg = (PSTR) msgBuffer;

		cout << msg << endl;
		lstrcpy(msg, "pong!");
		cout << msg << endl;
	}

	return TRUE;
}


/** Um objeto padrão que não faz nada. Bom, retorna TRUE.
*/
BOOL WINAPI ServerCallbackEmpty(PVOID context, PVOID msgBuffer, 
										  PDWORD msgSize, DWORD bufferSize)
{
	return TRUE;
}


/** Função usada para ser criada uma thread que vai ficar recebendo mensagens
de um objeto já criado.
*/
DWORD WINAPI ServerThread(PVOID pvParam)
{
	DWORD ret;
	ServerThreadParams* pParams = (ServerThreadParams*) pvParam;

	ret = ReceiveIpcObjectMessage(pParams->obj, 
		pParams->callback, 
		pParams->context, 
		pParams->timeout);

	return ret;
}
