#pragma once
#include "unit--.h"
#include "../../ipcobject.h"
#include "../../ipcobjectpriv.h"
#include "../../ipcobjsync.h"

#define IPCOBJ_UNIT_TEST_NAME "IpcObjUnitTest"


struct ServerThreadParams
{
	DWORD tid;
	HANDLE obj;
	IPC_OBJECT_CALLBACK callback;
	PVOID context;
	DWORD timeout;
};

DWORD WINAPI ServerThread(PVOID pvParam);

BOOL WINAPI ServerCallbackNormal(PVOID context, PVOID msgBuffer, 
											PDWORD msgSize, DWORD bufferSize);

BOOL WINAPI ServerCallbackEmpty(PVOID context, PVOID msgBuffer, 
										  PDWORD msgSize, DWORD bufferSize);
