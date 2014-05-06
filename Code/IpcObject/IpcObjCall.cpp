/** @file

@brief Criação de chamadas (envio ou recebimento de mensagem) ao objeto.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 08/2006
*/
#include "ipcobjcommon.h"
#include "ipcobjcall.h"
#include "ipcobjectpriv.h"
#include "ipcobjsync.h"


/** @brief Estrutura que armazena recursos de controle para 
chamar o objeto. */
struct IPC_OBJECT_CALL
{
	PIPC_OBJECT object;   ///< Objeto destino.
	HANDLE lock;          ///< Recurso de acesso exclusivo.
	HANDLE clientEvt;     ///< Evento do lado cliente.
	HANDLE serverEvt;     ///< Evento do lado servidor.
};


//
// Protótipos
//

/** @brief Lê os bytes de retorno da mensagem que foi enviada para o objeto 
ipc ao chamar WriteIpcObjectMessage. */
DWORD ReadIpcObjectMessage(PIPC_OBJECT pObject, 
									 PVOID msgBuf, DWORD msgSize);


/** @brief Escreve os bytes da mensagem a ser enviada na memória global 
do objeto. */
DWORD WriteIpcObjectMessage(PIPC_OBJECT pObject, 
									  PVOID msgBuf, DWORD msgSize);



/** Cria recursos para que um objeto possa ser chamado.

@param object Handle do objeto ipc obtido com OpenIpcObject ou CreateIpcObject.

@return Se a estrutura de chamada for criada com sucesso, o ponteiro para os
recursos alocados para a chamada (deve ser desalocado chamando a função 
CloseIpcObjectCall). Caso contrário, é retornado NULL. Para obter mais 
detalhes sobre o erro, chamar a função GetLastError.

@see CloseIpcObjectCall.
*/
PIPC_OBJECT_CALL CreateIpcObjectCall(HANDLE object)
{
	ASSERT(object != NULL);

	PIPC_OBJECT_CALL ret = NULL;
	DWORD err = ERROR_UNHANDLED_ERROR;
	PSECURITY_ATTRIBUTES objSec = GetIpcObjectSecurity(object);

	ASSERT((GetVersion() & 0x80000000) || objSec != NULL);

	ret = (PIPC_OBJECT_CALL) HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY, sizeof(IPC_OBJECT_CALL));

	if( ret )
	{
		// obs: aqui NÃO chamamos UnViewIpcObject, pois ele será usado
		// @see CloseIpcObjectCall.
		if( ret->object = ViewIpcObject(object) )
		{
			PCSTR objName = ret->object->name;
			ret->clientEvt = CreateIpcObjectClientEvent(objSec, objName);

			if( ret->clientEvt )
			{
				ret->serverEvt = CreateIpcObjectServerEvent(objSec, objName);

				if( ret->serverEvt )
				{
					ret->lock = CreateIpcObjectLock(objSec, objName);

					if( ret->lock )
					{
						err = ERROR_SUCCESS;
					}
				}
			}
		}
	}

	if( err != ERROR_SUCCESS )
	{
		err = GetLastError();

		if( ret )
			CloseIpcObjectCall(ret), ret = NULL;

		if( objSec )
			FreeIpcObjectSecurity(objSec);
	}

	SetLastError(err);
	return ret;
}


/** Abre recursos de um objeto para chamá-lo.

@param object Handle do objeto ipc obtido com OpenIpcObject ou CreateIpcObject.

@return Ponteiro para estrutura que mantém os recursos para chamar o objeto.
Pode se usado em chamadas a CallIpcObject. Para liberar os recursos, a função
CloseIpcObjectCall deve ser chamada.

@see CallIpcObject, CloseIpcObjectCall.
*/
PIPC_OBJECT_CALL OpenIpcObjectCall(HANDLE object)
{
	ASSERT(object != NULL);

	PIPC_OBJECT_CALL ret;
	DWORD err = ERROR_UNHANDLED_ERROR;

	ret = (PIPC_OBJECT_CALL) HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY, sizeof(IPC_OBJECT_CALL));

	if( ret )
	{
		// obs: aqui NÃO chamamos UnViewIpcObject, pois ele será usado
		// @see CloseIpcObjectCall.
		if( ret->object = ViewIpcObject(object) )
		{
			if( ret->clientEvt = OpenIpcObjectClientEvent(ret->object->name) )
			{
				if( ret->serverEvt = OpenIpcObjectServerEvent(ret->object->name) )
				{
					ret->lock = OpenIpcObjectLock(ret->object->name);

					if( ret->lock )
					{
						err = ERROR_SUCCESS;
					}
				}
			}
		}
	}

	if( err != ERROR_SUCCESS )
	{
		err = GetLastError();

		if( ret )
			CloseIpcObjectCall(ret), ret = NULL;
	}

	SetLastError(err);
	return ret;
}


/** Chama objeto e aguarda por resposta.

@param objCall Ponteiro para recursos de chamada ao objeto ipc.

@param timeout Tempo máximo de espera para enviar a mensagem.

@return Se a mensagem foi enviada com sucesso, TRUE. Caso contrário, FALSE. 
Para obter código de erro, use GetLastError.
*/
BOOL CallIpcObjectAndWait(PIPC_OBJECT_CALL objCall, DWORD timeout)
{
	BOOL ret = FALSE;

	if( ResetIpcObjectEvent(objCall->clientEvt) )
		if( SetIpcObjectEvent(objCall->serverEvt) )
			if( WaitIpcObjectEvent(objCall->clientEvt, timeout) )
				ret = TRUE;

	return ret;
}


/** Escreve os bytes da mensagem a ser enviada na memória global do objeto.

@param pObject Ponteiro para estrutura interna do objeto.

@param msgBuf Ponteiro para os dados da mensagem.

@param msgSize Tamanho em bytes do buffer apontado por msgBuf.

@return Quantidade de bytes copiados para a memória.

@remark É possível ler de volta os dados utilizando a função 
ReadIpcObjectMessage.

@see ReadIpcObjectMessage.
*/
DWORD WriteIpcObjectMessage(PIPC_OBJECT pObject, 
									 PVOID msgBuf, DWORD msgSize)
{
	DWORD maxMsgSize = GetIpcObjectSize(pObject) - sizeof(IPC_OBJECT);
	DWORD ret = min(msgSize, maxMsgSize);

	ASSERT(maxMsgSize >= msgSize);

	RealCopyMemory(pObject->message.buffer, msgBuf, ret);
	pObject->message.size = ret;

	return ret;
}


/** Envia mensagem para objeto ipc.

@param objCall Ponteiro para recursos de chamada ao objeto ipc.

@param msgBuffer Dados da mensagem a ser transmitida.

@param msgSize Número de bytes da mensagem apontada por msgBuffer.

@param retBuffer Dados de retorno da mensagem que foi transmitida. Se for
NULL, nada é copiado.

@param retSize Número de bytes copiados para retBuffer após o tratamento
da mensagem. Se retBuffer for NULL, esse parâmetro é ignorado.

@param timeout Tempo máximo de espera para enviar a mensagem.

@return Se a mensagem foi enviada com sucesso, TRUE. Caso contrário, FALSE. 
Para obter código de erro, use GetLastError.
*/
BOOL CallIpcObject(PIPC_OBJECT_CALL objCall, 
						 PVOID msgBuffer, DWORD msgSize, 
						 PVOID retBuffer, DWORD retSize, 
						 DWORD timeout)
{
	BOOL ret = FALSE;

	if( GetIpcObjectLock(objCall->lock, timeout) )
	{
		DWORD err = ERROR_SUCCESS;

		WriteIpcObjectMessage(objCall->object, msgBuffer, msgSize);

		if( CallIpcObjectAndWait(objCall, timeout) )
		{
			if( retBuffer )
				ReadIpcObjectMessage(objCall->object, retBuffer, retSize);

			ret = TRUE;
		}
		else
			err = GetLastError();

		ReleaseIpcObjectLock(objCall->lock);
		SetLastError(err);
	}

	return ret;
}



/** Espera por chamadas para o objeto ipc.

@param objCall Ponteiro para recursos de chamada ao objeto ipc.

@param timeout Tempo máximo de espera por uma mensagem.

@return Se a espera foi bem sucedida, TRUE. Caso contrário, FALSE. Chame
GetLastError para obter o código do erro.
*/
BOOL WaitIpcObjectCall(PIPC_OBJECT_CALL objCall, DWORD timeout)
{
	objCall->object->status = IPC_OBJECT_STATUS_LISTENING;
	return WaitIpcObjectEvent(objCall->serverEvt, timeout);
}


/** Chama função de callback passando os parâmetros recebidos.

@param objCall Ponteiro para recursos de chamada ao objeto ipc.

@param callback Função de callback responsável por receber as mensagens
direcionadas ao objeto icp.

@param context Ponteiro de contexto enviado à função de callback junto
com os dados da mensagem.

@return Retorno da chamada à função de callback.
*/
BOOL CallIpcObjectCallback(PIPC_OBJECT_CALL objCall, 
									IPC_OBJECT_CALLBACK callback, 
									PVOID context)
{
	objCall->object->status = IPC_OBJECT_STATUS_WORKING;

	BOOL ret = callback(context, 
		objCall->object->message.buffer,
		&objCall->object->message.size,
		objCall->object->message.bufSize);

	return ret;
}


/** Retorna de uma chamada feita ao objeto.

@param objCall Ponteiro para recursos de chamada ao objeto ipc.

@return Se o retorno foi bem sucedido, TRUE. Caso contrário, FALSE. Chame
GetLastError para obter o código de erro.
*/
BOOL ReturnFromIpcObjectCall(PIPC_OBJECT_CALL objCall)
{
	BOOL ret = FALSE;

	if( ResetIpcObjectEvent(objCall->serverEvt) )
	{
		if( SetIpcObjectEvent(objCall->clientEvt) )
		{
			objCall->object->status = IPC_OBJECT_STATUS_RESTARTING;
			ret = TRUE;
		}
	}

	return ret;
}


/** Fecha recursos abertos para chamar um objeto ou tratar chamadas.

@param objCall Ponteiro para estrutura que mantém recursos para chamadas
ao objeto obtido ao chamar as funções OpenIpcObjectCall ou 
CreateIpcObjectCall.

@return Se a liberação dos recursos ocorrer com sucesso, TRUE. Caso contrário,
FALSE. Para obter mais detalhes, use a função GetLastError.

@see OpenIpcObjectCall, CreateIpcObjectCall.
*/
BOOL CloseIpcObjectCall(PIPC_OBJECT_CALL objCall)
{
	ASSERT(objCall != NULL);

	if( objCall->lock )
		CloseHandle(objCall->lock);

	if( objCall->serverEvt )
		CloseHandle(objCall->serverEvt);

	if( objCall->clientEvt )
		CloseHandle(objCall->clientEvt);

	if( objCall->object )
		UnviewIpcObject(objCall->object);

	return HeapFree(GetProcessHeap(), 0, objCall);
}


/** Lê os bytes de retorno da mensagem que foi enviada para o objeto ipc
ao chamar WriteIpcObjectMessage.

@param pObject Ponteiro para estrutura interna do objeto.

@param msgBuf Ponteiro de destino dos dados da mensagem.

@param msgSize Tamanho em bytes do buffer apontado por msgBuf.

@return Quantidade de bytes copiados para a memória.

@see WriteIpcObjectMessage.
*/
DWORD ReadIpcObjectMessage(PIPC_OBJECT pObject, 
									PVOID msgBuf, DWORD msgSize)
{
	DWORD maxMsgSize = GetIpcObjectSize(pObject) - sizeof(IPC_OBJECT);
	DWORD ret = min(msgSize, maxMsgSize);

	ASSERT(maxMsgSize >= msgSize);

	RealCopyMemory(msgBuf, pObject->message.buffer, ret);

	return ret;
}
