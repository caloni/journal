/** @file

@brief Controle de sincronismo envolvido no envio e recebimento de mensagens.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 08/2006
*/
#include "ipcobjcommon.h"
#include "ipcobjsync.h"
#include "ipcobject.h"
#include <stdio.h>

/** Sufixo para eventos. */
#define IPC_OBJECT_EVENT_SUFFIX "Evt"
/** Sufixo para acesso exclusivo. */
#define IPC_OBJECT_LOCK_SUFFIX "Lck"

/** Tag usada na criação de recursos cliente. */
#define IPC_OBJECT_CLIENT_TAG 'C'
/** Tag usada na criação de recursos servidor. */
#define IPC_OBJECT_SERVER_TAG 'S'



/** Tamanho máximo do nome do evento de um objeto. */
static const DWORD IPC_OBJECT_EVENT_NAME_SIZE_MAX = 
( IPC_OBJECT_NAME_SIZE_MAX + sizeof(IPC_OBJECT_EVENT_SUFFIX) );


/** Tamanho máximo do nome do acesso exclusivo de um objeto. */
static const DWORD IPC_OBJECT_LOCK_NAME_SIZE_MAX =
( IPC_OBJECT_NAME_SIZE_MAX + sizeof(IPC_OBJECT_LOCK_SUFFIX) );



//
// Eventos
//


/** Cria nome de evento para objeto ipc. O nome será formado pela concatenação
dos elementos objName + tag + IPC_OBJECT_EVENT_SUFFIX.
*/
VOID CreateIpcObjectEventName(PSTR objEvtName, PCSTR objName, CHAR tag)
{
	CHAR szTag[] = { tag, 0 };

	lstrcpyA(objEvtName, objName);
	lstrcatA(objEvtName, szTag);
	lstrcatA(objEvtName, IPC_OBJECT_EVENT_SUFFIX);
}


/** Cria evento para objeto ipc.

@param objSec Controle de acesso para o recurso sendo criado.

@param objName Nome do objeto para qual o evento está sendo criado.

@param tag Identificador do evento. Possíveis valores são 
IPC_OBJECT_CLIENT_TAG e IPC_OBJECT_SERVER_TAG.

@return Handle para evento com permissão de aguardar, marcar e desmarcar.
*/
HANDLE CreateIpcObjectEvent(PSECURITY_ATTRIBUTES objSec, 
									 PCSTR objName, CHAR tag)
{
	HANDLE ret;
	DWORD err;
	CHAR objEvtName[IPC_OBJECT_EVENT_NAME_SIZE_MAX];

	CreateIpcObjectEventName(objEvtName, objName, tag);
	ret = CreateEventA(objSec, TRUE, FALSE, objEvtName);
	err = GetLastError();

	if( err == ERROR_ALREADY_EXISTS )
	{
		CloseHandle(ret), ret = NULL;
		SetLastError(err);
	}

	return ret;
}


/** Cria evento cliente para objeto ipc.
*/
HANDLE CreateIpcObjectClientEvent(PSECURITY_ATTRIBUTES objSec, PCSTR objName)
{
	return CreateIpcObjectEvent(objSec, objName, IPC_OBJECT_CLIENT_TAG);
}


/** Abre evento servidor de objeto ipc já criado.
*/
HANDLE CreateIpcObjectServerEvent(PSECURITY_ATTRIBUTES objSec, PCSTR objName)
{
	return CreateIpcObjectEvent(objSec, objName, IPC_OBJECT_SERVER_TAG);
}


/** Abre evento para objeto ipc.

@param objName Nome do objeto para qual o evento está sendo criado.

@param tag Identificador do evento. Possíveis valores são 
IPC_OBJECT_CLIENT_TAG e IPC_OBJECT_SERVER_TAG.

@return Handle para evento com permissão de aguardar, marcar e desmarcar.
*/
HANDLE OpenIpcObjectEvent(PCSTR objName, CHAR tag)
{
	HANDLE ret;
	CHAR objEvtName[IPC_OBJECT_EVENT_NAME_SIZE_MAX];

	CreateIpcObjectEventName(objEvtName, objName, tag);
	ret = OpenEventA(EVENT_ALL_ACCESS, TRUE, objEvtName);

	return ret;
}


/** Abre evento cliente de objeto ipc já criado.
*/
HANDLE OpenIpcObjectClientEvent(PCSTR objName)
{
	return OpenIpcObjectEvent(objName, IPC_OBJECT_CLIENT_TAG);
}


/** Abre evento servidor de objeto ipc já criado.
*/
HANDLE OpenIpcObjectServerEvent(PCSTR objName)
{
	return OpenIpcObjectEvent(objName, IPC_OBJECT_SERVER_TAG);
}


/** Marca evento de objeto ipc.

@param objEvt Handle para evento de um objeto ipc.

@return Se o evento foi marcado com sucesso, TRUE. Caso contrário, FALSE. Para
obter mais detalhes, chame a função GetLastError.
*/
BOOL SetIpcObjectEvent(HANDLE objEvt)
{
	return SetEvent(objEvt);
}


/** Desmarca evento de objeto ipc.

@param objEvt Handle para evento de um objeto ipc.

@return Se o evento foi desmarcado com sucesso, TRUE. Caso contrário, 
FALSE. Para obter mais detalhes, chame a função GetLastError.
*/
BOOL ResetIpcObjectEvent(HANDLE objEvt)
{
	return ResetEvent(objEvt);
}


/** Aguarda por evento de objeto ipc ser marcado.

@param objEvt Handle para evento de um objeto ipc.

@param timeout Tempo máximo em milisegundos de espera pelo evento.

@return Se a espera do evento foi bem sucedida, TRUE. Caso contrário, FALSE. 
Para obter mais detalhes, chame a função GetLastError.
*/
BOOL WaitIpcObjectEvent(HANDLE objEvt, DWORD timeout)
{
	DWORD err = ERROR_SUCCESS;
	DWORD wait = WaitForSingleObject(objEvt, timeout);
	
	switch( wait )
	{
	case WAIT_OBJECT_0:
		break;

	case WAIT_TIMEOUT:
		err = ERROR_TIMEOUT;
		break;

	case WAIT_FAILED:
	default:
		err = GetLastError();
		break;
	}

	SetLastError(err);
	return err == ERROR_SUCCESS ? TRUE : FALSE;
}



//
// Acesso exclusivo
//


/** Cria nome de acesso exclusivo para objeto ipc. O nome será formado pela 
concatenação dos elementos objName + IPC_OBJECT_LOCK_SUFFIX.
*/
VOID CreateIpcObjectLockName(PSTR objLockName, PCSTR objName)
{
	lstrcpyA(objLockName, objName);
	lstrcatA(objLockName, IPC_OBJECT_LOCK_SUFFIX);
}


/** Cria acesso exclusivo para objeto ipc.

@param objSec Controle de acesso para o recurso sendo criado.

@param objName Nome do objeto para qual o evento está sendo criado.

@return Handle para evento com permissão de obter e liberar o acesso.
*/
HANDLE CreateIpcObjectLock(PSECURITY_ATTRIBUTES objSec, PCSTR objName)
{
	HANDLE ret;
	DWORD err;
	CHAR objLockName[IPC_OBJECT_LOCK_NAME_SIZE_MAX];

	CreateIpcObjectLockName(objLockName, objName);
	ret = CreateMutexA(objSec, FALSE, objLockName);
	err = GetLastError();

	if( err == ERROR_ALREADY_EXISTS )
	{
		CloseHandle(ret), ret = NULL;
		SetLastError(err);
	}

	return ret;
}


/** Cria acesso exclusivo para objeto ipc.

@param objName Nome do objeto para qual o evento está sendo criado.

@return Handle para evento com permissão de obter e liberar o acesso.
*/
HANDLE OpenIpcObjectLock(PCSTR objName)
{
	HANDLE ret;
	CHAR objLockName[IPC_OBJECT_LOCK_NAME_SIZE_MAX];

	CreateIpcObjectLockName(objLockName, objName);
	ret = OpenMutexA(MUTEX_ALL_ACCESS, TRUE, objLockName);

	return ret;
}


/** Obtém acesso exclusivo a objeto ipc.

@param objLock Handle para acesso exclusivo do objeto ipc.

@param timeout Tempo máximo de espera para obter acesso exclusivo. Após 
expirado o tempo a função retorna erro.

@return Se foi obtido o acesso exclusivo com sucesso, TRUE. Caso contrário, 
FALSE. Para obter mais detalhes use a função GetLastError.
*/
BOOL GetIpcObjectLock(HANDLE objLock, DWORD timeout)
{
	BOOL ret = FALSE;
	DWORD err = ERROR_SUCCESS;

	switch( DWORD wait = WaitForSingleObject(objLock, timeout) )
	{
	case WAIT_OBJECT_0:
	case WAIT_ABANDONED:
		ret = TRUE;
		break;

	case WAIT_TIMEOUT:
		err = ERROR_TIMEOUT;
		break;

	default:
		err = GetLastError();
		break;
	}

	SetLastError(err);
	return ret;
}


/** Libera acesso exclusivo para o objeto ipc.

@param objLock Handle para acesso exclusivo do objeto ipc.

@return Se foi liberado o acesso exclusivo com sucesso, TRUE. Caso 
contrário, FALSE. Para obter mais detalhes use a função GetLastError.
*/
BOOL ReleaseIpcObjectLock(HANDLE objLock)
{
	BOOL ret = ReleaseMutex(objLock);

	// workaround para evitar que no Windows 9x uma thread 
	// sempre consiga ser a próxima a ser executada, evitando
	// assim das outras threads conseguirem rodar.
	Sleep(0);

	return ret;
}
