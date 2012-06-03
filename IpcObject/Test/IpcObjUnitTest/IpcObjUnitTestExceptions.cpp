#include "ipcobjunittest.h"


testSuite(IpcObjExceptions);


/** Quando existir j� o EVENTO CLIENTE criado, a cria��o deve falhar.
*/
testCase(ClientEventAlreadyCreated, IpcObjExceptions)
{
	HANDLE evt = CreateIpcObjectClientEvent(NULL, IPCOBJ_UNIT_TEST_NAME);
	assertTrue(evt != NULL);
	
	if( evt )
	{
		HANDLE obj = CreateIpcObject(NULL, IPCOBJ_UNIT_TEST_NAME, 0, 0, 0);
		assertTrue(obj != NULL);

		if( obj )
		{
			DWORD err = ReceiveIpcObjectMessage(obj, ServerCallbackEmpty, 0, 100);
			assertTrue(err != ERROR_SUCCESS);
			assertTrue(err != ERROR_TIMEOUT);

			CloseHandle(obj);
		}

		CloseHandle(evt);
	}
}


/** Quando existir j� o EVENTO SERVIDOR criado, a cria��o deve falhar.
*/
testCase(ServerEventAlreadyCreated, IpcObjExceptions)
{
	HANDLE evt = CreateIpcObjectServerEvent(NULL, IPCOBJ_UNIT_TEST_NAME);
	assertTrue(evt != NULL);
	
	if( evt )
	{
		HANDLE obj = CreateIpcObject(NULL, IPCOBJ_UNIT_TEST_NAME, 0, 0, 0);
		assertTrue(obj != NULL);

		if( obj )
		{
			DWORD err = ReceiveIpcObjectMessage(obj, ServerCallbackEmpty, 0, 100);
			assertTrue(err != ERROR_SUCCESS);
			assertTrue(err != ERROR_TIMEOUT);

			CloseHandle(obj);
		}

		CloseHandle(evt);
	}
}


/** Quando existir j� o ACESSO EXCLUSIVO criado, a cria��o deve falhar.
*/
testCase(LockAlreadyCreated, IpcObjExceptions)
{
	HANDLE lck = CreateIpcObjectLock(NULL, IPCOBJ_UNIT_TEST_NAME);
	assertTrue(lck != NULL);

	if( lck )
	{
		HANDLE obj = CreateIpcObject(NULL, IPCOBJ_UNIT_TEST_NAME, 0, 0, 0);
		assertTrue(obj != NULL);

		if( obj )
		{
			DWORD err = ReceiveIpcObjectMessage(obj, ServerCallbackEmpty, 0, 100);
			assertTrue(err != ERROR_SUCCESS);
			assertTrue(err != ERROR_TIMEOUT);

			CloseHandle(obj);
		}

		CloseHandle(lck);
	}
}
