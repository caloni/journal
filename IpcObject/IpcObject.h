/** @mainpage IpcObject

@section Introduction Introdu��o

IpcObject � uma biblioteca de comunica��o simples e efetiva entre processos. 
Seu objetivo � disponibilizar uma interface simples, baseada em objetos, de 
enviar e receber dados entre processos. As fun��es de manipula��o de objetos 
segue o mesmo fucionamento da API Win32.

@section HowToUse Como usar

Sendo uma biblioteca fundamentalmente unidirecional e n�o mantenedora de 
estado, ou seja, quem recebe os dados n�o sabe nada de quem envia e pode 
apenas retornar dados na chamada atual, existem duas divis�es no uso da 
biblioteca: parte servidora e parte cliente.

@subsection ServerPart Parte servidora

Basicamente o que um servidor de IpcObject deve fazer �:
 - Criar um objeto ipc (@ref CreateIpcObject).
 - Iniciar recebimento de chamadas (@ref ReceiveIpcObjectMessage).
 - Tratar chamadas recebidas (@ref IpcObjectCallback).
 
Para dar uma olhada nas fun��es relativas � essa parte, v� em @ref 
ServerFunctions.

@subsection ClientPart Parte cliente

Um cliente pode possuir apenas o nome do objeto ao qual deseja chamar. Tendo 
isso, o procedimento b�sico de funcionamento �:
 - Abrir o objeto (@ref OpenIpcObject).
 - Enviar mensagem para o objeto (@ref SendIpcObjectMessage).

Para dar uma olhada nas fun��es relativas � essa parte, v� em @ref 
ClientFunctions.
*/
#pragma once
#include <windows.h>


/** Constantes utilizadas na biblioteca. */
enum IPC_OBJECT_CONSTANTS
{
	/** N�mero m�ximo de caracteres que o nome de um objeto de ipc pode ter. */
	IPC_OBJECT_NAME_SIZE_MAX = 64,
};


/** Estado de um objeto ipc em um dado momento. */
enum IPC_OBJECT_STATUS
{
	IPC_OBJECT_STATUS_IDLE           = 0, ///< Criando objeto (n�o totalmente pronto).
	IPC_OBJECT_STATUS_STARTING       = 1, ///< Iniciando objeto (ainda n�o recebe msgs).
	IPC_OBJECT_STATUS_LISTENING      = 2, ///< Objeto pronto para receber mensagens.
	IPC_OBJECT_STATUS_WORKING        = 3, ///< Respondendo � uma mensagem recebida.
	IPC_OBJECT_STATUS_RESTARTING     = 4, ///< Objeto voltando a receber mensagens.
	IPC_OBJECT_STATUS_STOPPING       = 5, ///< Objeto est� parando.
	IPC_OBJECT_STATUS_DESTROYING     = 6, ///< Objeto em processo de destrui��o.
	IPC_OBJECT_STATUS_INVALID        = 7, ///< Objeto em estado inv�lido.
};


/* Fun��o de callback que recebe as mensagens de um objeto. */
typedef BOOL (WINAPI *IPC_OBJECT_CALLBACK)(PVOID, PVOID, PDWORD, DWORD);


/** @defgroup ServerFunctions Fun��es para o servidor */
/*@{*/

/** @brief Cria um objeto ipc. */
HANDLE CreateIpcObject(PSECURITY_ATTRIBUTES objSec, PCSTR objName,
							  DWORD msgSize, DWORD objVersion, DWORD flags);

/** @brief Inicia o recebimento de mensagens de um objeto j� criado. */
DWORD ReceiveIpcObjectMessage(HANDLE object, IPC_OBJECT_CALLBACK msgFunc, 
										PVOID context, DWORD timeout);

/**
Essa fun��o � chamada cada vez que uma mensagem enviada pelo cliente � 
recebida  do lado servidor.

@param context Um ponteiro de contexto que � passado na chamada da fun��o 
ReceiveIpcObjectMessage para que a fun��o de callback saiba identificar qual dos
objetos recebeu uma mensagem.

@param[in,out] msgBuffer Buffer que cont�m os dados da mensagem recebida.

@param [in, out] msgSize Tamanho em bytes de msgBuffer. Na entrada, possui o 
tamanho da mensagem recebida. Na sa�da, deve ser preenchido com o n�mero de 
bytes da resposta, que n�o pode exceder bufferSize.

@param bufferSize Tamanho m�ximo da mensagem para o objeto atual. Serve como
limitador para a resposta.

@return Se o objeto deve continuar recebendo mensagens, deve ser retornado TRUE. 
Caso o objeto deva parar de receber mensagens, deve ser retornado FALSE.

@remark Tome cuidado ao preencher a resposta para que esta n�o passe em bytes
de bufferSize.

@see ReceiveIpcObjectMessage.
*/
BOOL WINAPI IpcObjectCallback(PVOID context, PVOID msgBuffer, 
										PDWORD msgSize, DWORD bufferSize);

/*@} ServerFunctions*/


/** @defgroup ClientFunctions Fun��es para o cliente */
/*@{*/

/** Abre um objeto j� existente. */
HANDLE OpenIpcObject(PCSTR objName);

/** Envia mensagem para objeto j� aberto. */
DWORD SendIpcObjectMessage(HANDLE object, PVOID msgBuffer, DWORD msgSize, 
									PVOID retBuffer, DWORD retSize, DWORD timeout);

/** Obt�m vers�o atual do objeto ipc. */
BOOL GetIpcObjectVersion(HANDLE object, PDWORD version);

/** Obt�m estado atual do objeto ipc. */
IPC_OBJECT_STATUS GetIpcObjectStatus(HANDLE object);

/*@} ClientFunctions*/
