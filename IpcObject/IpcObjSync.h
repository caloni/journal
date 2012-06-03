/*
Controle de sincronismo envolvido no envio e recebimento de mensagens.
*/
#pragma once
#include <windows.h>


//
// Eventos
//

/** @brief Cria evento cliente para objeto ipc. */
HANDLE CreateIpcObjectClientEvent(PSECURITY_ATTRIBUTES objSec, PCSTR objName);

/** @brief Abre evento cliente de objeto ipc já criado. */
HANDLE OpenIpcObjectClientEvent(PCSTR objName);


/** @brief Cria evento servidor para objeto ipc. */
HANDLE CreateIpcObjectServerEvent(PSECURITY_ATTRIBUTES objSec, PCSTR objName);

/** @brief Abre evento servidor de objeto ipc já criado. */
HANDLE OpenIpcObjectServerEvent(PCSTR objName);


/** @brief Marca evento de objeto ipc. */
BOOL SetIpcObjectEvent(HANDLE objEvt);

/** @brief Desmarca evento de objeto ipc. */
BOOL ResetIpcObjectEvent(HANDLE objEvt);

/** @brief Aguarda por evento de objeto ipc ser marcado. */
BOOL WaitIpcObjectEvent(HANDLE objEvt, DWORD timeout);


//
// Acesso exclusivo
//

/** @brief Cria acesso exclusivo para objeto ipc. */
HANDLE CreateIpcObjectLock(PSECURITY_ATTRIBUTES objSec, PCSTR objName);

/** @brief Abre acesso exclusivo a objeto ipc. */
HANDLE OpenIpcObjectLock(PCSTR objName);

/** @brief Obtém acesso exclusivo a objeto ipc. */
BOOL GetIpcObjectLock(HANDLE objLock, DWORD timeout);

/** @brief Libera acesso exclusivo para o objeto ipc. */
BOOL ReleaseIpcObjectLock(HANDLE objLock);
