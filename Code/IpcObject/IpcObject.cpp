/** @file

@brief Parte principal da biblioteca, contém a interface pública da biblioteca.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 08/2006
*/
#include "ipcobjcommon.h"
#include "ipcobjectpriv.h"
#include "ipcobjcall.h"
#include <stddef.h>


/**
Após o final desse processo o objeto estará no modo 
IPC_OBJECT_STATUS_STARTING. Para iniciar o recebimento de mensagens,
deve ser usada a função ReceiveIpcObjectMessage.

@param objSec Atributo de segurança para restringir o acesso ao
objeto. Pode ser passado NULL para utilizar a segurança default.

@param objName String ANSIZ que identifica o objeto que será criado. Não pode 
ser um nome de objeto existente e deve ter no máximo 
IPC_OBJECT_CONSTANTS bytes de tamanho, incluindo o zero terminador.

@param msgSize Tamanho máximo de bytes que uma mensagem recebida por esse
objeto pode conter.

@param objVersion Versão do objeto que será criado. A versão pode ser usada
pelos clientes para saber as funcionalidades de um objeto. É recomendável
utilizar o padrão LOWORD(objVersion) para mudanças menores e 
HIWORD(objVersion) para grandes mudanças e/ou quebra de compatibilidade.

@param flags Reservado para uso futuro. O valor desse parâmetro deve ser zero.

@return Caso tudo dê certo, um valor não-nulo que pode ser usado nas funções
de objeto ipc. Após o uso, deve ser liberado com CloseHandle. Caso algum erro 
ocorra, o valor retornado será NULL e deve ser chamado GetLastError para 
obter o código do erro que ocorreu.

@see ReceiveIpcObjectMessage.
*/
HANDLE CreateIpcObject(PSECURITY_ATTRIBUTES objSec, PCSTR objName,
							  DWORD msgSize, DWORD objVersion, DWORD flags)
{
	ASSERT(objName != NULL);
	ASSERT(IsBadReadPtr(objName, 1) == 0);
	ASSERT(flags == 0);

	HANDLE ret = NULL;
	DWORD err = ERROR_SUCCESS;

	// aloca espaço no sistema para esse objeto
	ret = CreateIpcObjectSpace(objSec, objName, msgSize);

	if( ret != NULL )
	{
		if( PIPC_OBJECT obj = ViewIpcObject(ret) )
		{
			// inicializa a estrutura do objeto que foi criado
			err = InitializeIpcObject(obj, objName, objVersion);
			UnviewIpcObject(obj);
		}
		else
			err = GetLastError();
	}

	if( err != ERROR_SUCCESS )
	{
		CloseHandle(ret);
		ret = NULL;
		SetLastError(err);
	}

	return ret;
}


/** Inicia o recebimento de mensagens de um objeto já criado.

Quando chamada, muda o modo do objeto para IPC_OBJECT_STATUS_LISTENING.

@param object Handle para um objeto criado por CreateIpcObject ou aberto por 
OpenIpcObject.

@param msgFunc Função que será chamada para cada mensagem recebida pelo objeto.

@param context Parâmetro que será passado para msgFunc ao ser recebida uma
mensagem desse objeto.

@param timeout Tempo de espera para a próxima mensagem. Caso o tempo expire,
a função retorna.

@return Se o loop de mensagens saiu por vontade da função de callback, 
ERROR_SUCCESS. Caso contrário, ou seja, algum erro ocorreu ou o tempo de
espera por mensagens expirou, o código do erro ocorrido.

@see CreateIpcObject, OpenIpcObject, IpcObjectCallback.
*/
DWORD ReceiveIpcObjectMessage(HANDLE object, IPC_OBJECT_CALLBACK msgFunc, 
										PVOID context, DWORD timeout)
{
	ASSERT(object != NULL);
	ASSERT(IsBadCodePtr((FARPROC)msgFunc) == 0);

	DWORD ret = ERROR_SUCCESS;

	// para ele ser chamado, ele precisa de uma pilha de chamadas
	if( PIPC_OBJECT_CALL objCall = CreateIpcObjectCall(object) )
	{
		// objeto iniciando trabalho...
		BOOL success = SetIpcObjectStatus(object, IPC_OBJECT_STATUS_STARTING);

		if( success )
			ret = StartIpcObjectLoop(objCall, msgFunc, context, timeout);

		// objeto está parando...
		SetIpcObjectStatus(object, IPC_OBJECT_STATUS_STOPPING);
		CloseIpcObjectCall(objCall);

		// objeto está de bobeira de novo
		SetIpcObjectStatus(object, IPC_OBJECT_STATUS_IDLE);
	}
	else
	{
		ret = GetLastError();
		LOG(("%08X: error %d creating call.\n", object, ret));
	}

	return ret;
}


/**  @brief Protótipo da função de callback que recebe as mensagens de um objeto.
*/
BOOL WINAPI IpcObjectCallback(PVOID context, PVOID msgBuffer, DWORD msgSize);


/** Abre um objeto já existente.

A partir do HANDLE retornado é possível enviar mensagens para esse objeto.

@param objName Nome do objeto. Não deve ser maior que @b IPC_OBJECT_NAME_SIZE_MAX
bytes, incluindo o zero terminador. Deve ser o nome de um objeto já criado pela
função OpenIpcObject.

@return Se o objeto existir e o chamador ter direitos de acesso ao objeto, um
valor não-nulo que pode se usado para enviar mensagens. Caso contrário, NULL.
Chame GetLastError para obter detalhes do que ocorreu de errado.

@see SendIpcObjectMessage.
*/
HANDLE OpenIpcObject(PCSTR objName)
{
	ASSERT(IsBadReadPtr(objName, 0) == 0);

	HANDLE ret = OpenFileMappingA(FILE_MAP_ALL_ACCESS, TRUE, objName);
	return ret;
}


/** Envia mensagem para objeto já aberto.

Para abrir um objeto de forma a lhe enviar mensagens, use a função 
OpenIpcObject.

@param object Handle para um objeto aberto por OpenIpcObject ou CreateIpcObject.

@param msgBuffer Ponteiro para os dados da mensagem a ser enviada.

@param msgSize Número de bytes que msgBuffer contém.

@param[out] retBuffer Ponteiro para copiar dados de retorno da mensagem. Pode 
ser NULL caso não se deseje um retorno, ou msgBuffer, desde que a memória 
tenha direitos de escrita.

@param[out] retSize Número máximo de bytes que podem ser copiados para 
retBuffer, no caso em que retBuffer não é igual a NULL.

@param timeout Tempo máximo de espera para que o objeto trate a mensagem.

@return Será retornado ERROR_SUCCESS caso a mensagem tenha sido enviada com
sucesso. Caso contrário, o código de erro do que ocorreu.

@see OpenIpcObject.
*/
DWORD SendIpcObjectMessage(HANDLE object, 
									PVOID msgBuffer, DWORD msgSize,
									PVOID retBuffer, DWORD retSize,
									DWORD timeout)
{
	ASSERT(object != NULL);
	ASSERT(IsBadReadPtr(msgBuffer, msgSize) == 0);
	ASSERT(retBuffer == NULL || IsBadWritePtr(retBuffer, retSize) == 0);

	// verifica se o tamanho das mensagens não ultrapassa tamanho máximo
	DWORD ret = CheckMessageSize(object, max(msgSize, retSize));

	if( ret == ERROR_SUCCESS )
	{
		if( PIPC_OBJECT_CALL objCall = OpenIpcObjectCall(object) )
		{
			if( ! CallIpcObject(objCall, 
				msgBuffer, msgSize, 
				retBuffer, retSize, 
				timeout) )
			{
				ret = GetLastError();
			}

			CloseIpcObjectCall(objCall), objCall = NULL;
		}
		else
			ret = GetLastError();
	}

	return ret;
}


/** Mapeia visão do objeto para o processo atual.

@param object Handle para objeto ipc aberto com OpenIpcObject ou criado
com CreateIpcObject.

@return Um ponteiro para a estrutura interna do objeto.

@see UnviewIpcObject.
*/
PIPC_OBJECT ViewIpcObject(HANDLE object)
{
	PVOID ret = MapViewOfFile(object, FILE_MAP_WRITE, 0, 0, 0);
	return static_cast<PIPC_OBJECT>(ret);
}


/** Desmapeia a visão de um objeto previamente mapeado.

@param pObject Ponteiro para estrutura interna do objeto recebida ao chamar
a função ViewIpcObject.

@return Se o desmapeamento ocorreu com sucesso, TRUE. Caso contrário, FALSE.
Chama GetLastError para obter maiores informações do erro.

@see ViewIpcObject.
*/
BOOL UnviewIpcObject(PIPC_OBJECT pObject)
{
	BOOL ret = UnmapViewOfFile(pObject);
	return ret;
}


/** Aloca o espaço de memória global necessário para armazenar os dados sobre
o objeto ipc.

@param objSec Controle de acesso à memória do objeto.

@param objName Nome do objeto ipc. O tamanho do nome (incluindo o zero 
terminador) não pode ultrapassar IPC_OBJECT_NAME_SIZE_MAX bytes.

@param msgSize Tamanho máximo de uma mensagem que poderá ser enviada para 
esse objeto.
*/
HANDLE CreateIpcObjectSpace(PSECURITY_ATTRIBUTES objSec, PCSTR objName, DWORD msgSize)
{
	HANDLE ret = NULL;
	DWORD err = ERROR_SUCCESS;

	ASSERT( lstrlenA(objName) + 1 <= IPC_OBJECT_NAME_SIZE_MAX );

	if( lstrlenA(objName) + 1 <= IPC_OBJECT_NAME_SIZE_MAX )
	{
		DWORD objSize = sizeof(IPC_OBJECT) + msgSize;

		ret = CreateFileMappingA(INVALID_HANDLE_VALUE, objSec, 
			PAGE_READWRITE, 0, objSize, objName);
	}
	else
		err = ERROR_BUFFER_OVERFLOW;

	if( ret != NULL && (err = GetLastError()) == ERROR_ALREADY_EXISTS )
	{
		err = ERROR_ALREADY_EXISTS;
		CloseHandle(ret);
		ret = NULL;
	}

	SetLastError(err);
	return ret;
}


/** Obtém tamanho total do objeto na memória global do sistema.

@param pObject Ponteiro para estrutura interna do objeto.

@return O tamanho total do objeto em bytes. Caso ocorra algum erro, zero.
Para obter mais informações sobre o erro, use a função GetLastError.
*/
DWORD GetIpcObjectSize(PIPC_OBJECT pObject)
{
	SIZE_T ret = 0;
	MEMORY_BASIC_INFORMATION memInfo = { };

	if( VirtualQuery(pObject, &memInfo, sizeof(memInfo)) )
		ret = memInfo.RegionSize;

	return DWORD(ret);
}


/** Inicializa os dados do objeto ipc baseado nas informações passadas.

@param pObject Ponteiro para estrutura interna do objeto.

@param objName Nome do objeto. Note que esse nome será usado para a criação
de todos os recursos necessários para o objeto.

@param objVersion Versão do objeto. LOWORD(objVersion) é a versão menor e
HIWORD(objVersion) é a versão maior.
*/
DWORD InitializeIpcObject(PIPC_OBJECT pObject, PCSTR objName, DWORD objVersion)
{
	DWORD ret = ERROR_SUCCESS;
	DWORD objSize;

	objSize = GetIpcObjectSize(pObject);

	if( objSize )
	{
		RealZeroMemory(pObject, objSize);

		pObject->size = objSize;
		pObject->version = objVersion;
		lstrcpynA(pObject->name, objName, sizeof(pObject->name));
		pObject->msgOffset = offsetof(IPC_OBJECT, message);
		pObject->status = IPC_OBJECT_STATUS_STARTING;
		pObject->message.bufSize = objSize - sizeof(IPC_OBJECT);
		pObject->message.buffer[pObject->message.bufSize] = '$'; // marcador de final
	}
	else
		ret = GetLastError();

	return ret;
}


/** Obtém versão atual do objeto ipc.

@param object Handle do objeto ipc obtido com @ref OpenIpcObject ou 
@ref CreateIpcObject.

@param[out] version Onde será escrita o número da versão do objeto
atualmente no ar.

@return Se foi obtida a versão com sucesso, TRUE. Caso contrário, FALSE.
Chame GetLastError para obter detalhes do erro.

@remark Por padrão, LOWORD(version) é alterado em mudanças de versão pequenas
e HIWORD(version) é uma mudança grande e/ou quebradora de compatibilidade.

@see CreateIpcObject, OpenIpcObject.
*/
BOOL GetIpcObjectVersion(HANDLE object, PDWORD version)
{
	ASSERT(object != NULL);
	ASSERT(IsBadWritePtr(version, sizeof(*version)) == 0);

	BOOL ret = FALSE;

	if( PIPC_OBJECT pObject = ViewIpcObject(object) )
	{
		*version = (IPC_OBJECT_STATUS) pObject->version;
		ret = TRUE;
		UnviewIpcObject(pObject);
	}

	return ret;
}


/** Obtém estado atual do objeto ipc.

@param object Handle do objeto ipc obtido com OpenIpcObject ou CreateIpcObject.

@return Se for obtido com sucesso, o estado atual do objeto. Se ocorrer algum 
erro, IPC_OBJECT_STATUS_INVALID. Para diferenciar entre um erro ao obter o
estado do objeto e o estado IPC_OBJECT_STATUS_INVALID utilize a função 
GetLastError, que deve retornar ERROR_SUCCESS se o estado do objeto for
realmente IPC_OBJECT_STATUS_INVALID.
*/
IPC_OBJECT_STATUS GetIpcObjectStatus(HANDLE object)
{
	ASSERT(object != NULL);

	IPC_OBJECT_STATUS ret = IPC_OBJECT_STATUS_INVALID;

	if( PIPC_OBJECT pObject = ViewIpcObject(object) )
	{
		ret = (IPC_OBJECT_STATUS) pObject->status;
		UnviewIpcObject(pObject);
		SetLastError(ERROR_SUCCESS);
	}

	return ret;
}


/** Define estado do objeto ipc.

@param object Handle do objeto ipc obtido com OpenIpcObject ou CreateIpcObject.

@param objStatus Novo estado do objeto.

@return Se for definido com sucesso, TRUE. Caso contrário, FALSE. Para obter o
código de erro chame GetLastError.
*/
BOOL SetIpcObjectStatus(HANDLE object, IPC_OBJECT_STATUS objStatus)
{
	BOOL ret = FALSE;

	if( PIPC_OBJECT pObject = ViewIpcObject(object) )
	{
		pObject->status = objStatus;
		UnviewIpcObject(pObject);
		ret = TRUE;
	}

	return ret;
}


/** Inicia recebimento de mensagems dirigidas ao objeto ipc.

@param objCall Recursos de chamada alocados para o objeto por meio de
CreateIpcObjectCall ou OpenIpcObjectCall.

@param msgFunc Função de callback do servidor.

@param context Ponteiro de contexto passado para a função de callback.

@param timeout Tempo máximo de espera da próxima mensagem. Após esgotar-se 
esse tempo, a função retorna.

@return Se a função sair por intermédio da função de callback - que deve 
retornar FALSE para sair do loop de mensagens - será retornado ERROR_SUCCESS.
Se ocorrer algum erro durante o processo, será retornado o código de erro
correspondente à falha.

@remark A expiração do tempo de timeout também pode gerar erro durante a 
espera. Possivelmente ERROR_TIMEOUT.
*/
DWORD StartIpcObjectLoop(PIPC_OBJECT_CALL objCall, 
								 IPC_OBJECT_CALLBACK msgFunc, 
								 PVOID context,
								 DWORD timeout)
{
	DWORD ret = ERROR_SUCCESS;
	BOOL active = TRUE;
	IPC_OBJECT_STATUS objStat = IPC_OBJECT_STATUS_LISTENING;

	while( active )
	{
		if( WaitIpcObjectCall(objCall, timeout) )
		{
			active = CallIpcObjectCallback(objCall, msgFunc, context);
			ReturnFromIpcObjectCall(objCall);
		}
		else
		{
			ret = GetLastError();
			active = FALSE;
		}
	}

	return ret;
}


/** Verifica se o tamanho máximo de uma mensagem em um determinado objeto é 
menor ou igual que um determinado tamanho.

@param object O objeto determinado.

@param msgSize O tamanho determinado.

@return Caso msgSize seja menor ou igual ao tamanho máximo de uma mensagem
para object, ERROR_SUCCESS. Caso seja maior, ERROR_MESSAGE_EXCEEDS_MAX_SIZE.
Caso algum erro ocorra na verificação, o código do erro.
*/
DWORD CheckMessageSize(HANDLE object, DWORD msgSize)
{
	DWORD ret;

	if( PIPC_OBJECT pObject = ViewIpcObject(object) )
	{
		DWORD objMsgSize = pObject->message.bufSize;

		if( msgSize <= objMsgSize )
			ret = ERROR_SUCCESS;
		else
			ret = ERROR_MESSAGE_EXCEEDS_MAX_SIZE;

		UnviewIpcObject(pObject);
	}
	else
		ret = GetLastError();

	return ret;
}
