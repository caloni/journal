/*
Funções e estruturas internas da biblioteca.
*/
#pragma once
#include "ipcobject.h"
#include "ipcobjcall.h"


/** @brief Estrutura que armazena todos os dados relativos 
a um objeto criado. */
struct IPC_OBJECT
{
	/** @brief Tamanho da estrutura ( sizeof(IPC_OBJECT) + message.size ).
	*/
	DWORD size;

	/** @brief Versão da estrutura (HIWORD == major, LOWORD == minor).
	*/
	DWORD version;

	/** @brief Nome do objeto usado na interface do usuário.
	*/
	CHAR name[IPC_OBJECT_NAME_SIZE_MAX];

	/** @brief Offset do primeiro byte dos dados da mensagem.
	*/
	DWORD msgOffset;

	/** @brief Estado do objeto no momento.
	*/
	BYTE status;

	/** @brief Dados da última mensagem enviada.
	*/
	struct MESSAGE
	{
		/** @brief Número de bytes do buffer alocado.
		*/
		DWORD bufSize;

		/** @brief Número de bytes usados.
		*/
		DWORD size;

		/** @brief Início dos bytes da mensagem.
		*/
		BYTE buffer[1];

	} message;
};


/** Tipo de ponteiro para um objeto (muito usado ao acessar a estrutura). */
typedef IPC_OBJECT* PIPC_OBJECT;


/** @brief Mapeia visão do objeto para o processo atual. */
PIPC_OBJECT ViewIpcObject(HANDLE object);

/** @brief Desmapeia a visão de um objeto previamente mapeado. */
BOOL UnviewIpcObject(PIPC_OBJECT pObject);

/** @brief Aloca o espaço de memória global necessário para armazenar os dados sobre
o objeto ipc. */
HANDLE CreateIpcObjectSpace(PSECURITY_ATTRIBUTES objSec,
									 PCSTR objName, DWORD msgSize);

/** @brief Obtém tamanho total do objeto na memória global do sistema. */
DWORD GetIpcObjectSize(PIPC_OBJECT pObject);

/** @brief Inicializa os dados do objeto ipc baseado nas informações passadas. */
DWORD InitializeIpcObject(PIPC_OBJECT pObject, PCSTR objName, 
								  DWORD objVersion);

/** @brief Define estado do objeto ipc. */
BOOL SetIpcObjectStatus(HANDLE object, IPC_OBJECT_STATUS objStatus);

/** @brief Inicia recebimento de mensagems dirigidas ao objeto ipc. */
DWORD StartIpcObjectLoop(PIPC_OBJECT_CALL objCall, IPC_OBJECT_CALLBACK msgFunc, 
								 PVOID context, DWORD timeout);

/** @brief Verifica se objeto suporta mensagem do tamanho especificado. */
DWORD CheckMessageSize(HANDLE object, DWORD msgSize);
