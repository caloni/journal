/** @file

Fun��es comuns para a execu��o dos testes da biblioteca IpcObject.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 08/2006
*/
#include "ipcobjunittest.h"
#include <shlwapi.h>
#include <tchar.h>
#include <iostream>


using namespace std;


/** Um objeto padr�o que recebe strings em C, imprime na sa�da e muda a string
para "pong!".

@remark N�o possui condi��o de sa�da, ou seja, deve ser controlado pelo 
timeout na hora da cria��o do loop de mensagens.
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


/** Um objeto padr�o que n�o faz nada. Bom, retorna TRUE.
*/
BOOL WINAPI ServerCallbackEmpty(PVOID context, PVOID msgBuffer, 
										  PDWORD msgSize, DWORD bufferSize)
{
	return TRUE;
}


/** Fun��o usada para ser criada uma thread que vai ficar recebendo mensagens
de um objeto j� criado.
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
