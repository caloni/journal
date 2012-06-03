/*
Criação de chamadas (envio ou recebimento de mensagem) ao objeto.
*/
#pragma once
#include "ipcobject.h"
#include <windows.h>


/** Dados relacionados ao envio de uma mensagem a um objeto. */
struct IPC_OBJECT_CALL;


/** Tipo de ponteiro para uma chamada a um objeto. */
typedef IPC_OBJECT_CALL* PIPC_OBJECT_CALL;


/** @brief Cria recursos para que um objeto possa ser chamado. */
PIPC_OBJECT_CALL CreateIpcObjectCall(HANDLE object);


/** @brief Abre recursos de um objeto para chamá-lo. */
PIPC_OBJECT_CALL OpenIpcObjectCall(HANDLE object);


/** @brief Envia mensagem para objeto ipc. */
BOOL CallIpcObject(PIPC_OBJECT_CALL objCall, PVOID msgBuffer, DWORD msgSize,
						 PVOID retBuffer, DWORD retSize, DWORD timeout);


/** @brief Espera por chamadas para o objeto ipc. */
BOOL WaitIpcObjectCall(PIPC_OBJECT_CALL objCall, DWORD timeout);


/** @brief Chama função de callback passando os parâmetros recebidos. */
BOOL CallIpcObjectCallback(PIPC_OBJECT_CALL objCall, 
									IPC_OBJECT_CALLBACK callback, PVOID context);


/** @brief Retorna de uma chamada feita ao objeto. */
BOOL ReturnFromIpcObjectCall(PIPC_OBJECT_CALL objCall);


/** @brief Fecha recursos abertos para chamar um objeto ou tratar chamadas. */
BOOL CloseIpcObjectCall(PIPC_OBJECT_CALL objCall);


/** @brief Chama objeto e aguarda por resposta. */
BOOL CallIpcObjectAndWait(PIPC_OBJECT_CALL objCall, DWORD timeout);
