#include "ipcobjunittest.h"


testSuite(IpcObjExceptions);


/** Quando existir já o EVENTO CLIENTE criado, a criação deve falhar.
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


/** Quando existir já o EVENTO SERVIDOR criado, a criação deve falhar.
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


/** Quando existir já o ACESSO EXCLUSIVO criado, a criação deve falhar.
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
