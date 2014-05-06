#include "ipcobjunittest.h"

testSuite(IpcObjUnitTest);


/** Caso normal de uso da biblioteca. A idéia é chamar todas as funções 
públicas (IpcObject.h) em um contexto normal de execução, ou seja, primeiro
criar um servidor, depois o cliente enviar uma mensagem, o servidor tratar
a mensagem, etc.
*/
testCase(NormalUse, IpcObjUnitTest)
{
	DWORD dwErr;
	BOOL bErr;

	HANDLE ipcObjSrv = CreateIpcObject(NULL,    // segurança padrão
		IPCOBJ_UNIT_TEST_NAME,                   // nome do objeto usado para testes
		0x1000 - sizeof(IPC_OBJECT),             // 4KB de objeto
		0x55AA55AA,                              // para teste de leitura
		0);

	assertTrue(ipcObjSrv != NULL);

	if( ipcObjSrv )
	{
		// verifica versão
		{
			DWORD version = 0x00000000;
			bErr = GetIpcObjectVersion(ipcObjSrv, &version);

			assertTrue(bErr == TRUE);
			assertTrue(version == 0x55AA55AA);
		}


		// cria thread servidora
		ServerThreadParams srvParams =
		{
			0,
			ipcObjSrv, 
			ServerCallbackNormal, 
			NULL, 
			1000
		};

		HANDLE srvThr = CreateThread(NULL, 0, ServerThread, 
			&srvParams, 0, &srvParams.tid);
		assertTrue(srvThr != NULL);


		if( srvThr )
		{
			HANDLE ipcObjClient = OpenIpcObject(IPCOBJ_UNIT_TEST_NAME);
			assertTrue(ipcObjClient != NULL);

			if( ipcObjClient )
			{
				bErr = FALSE;

				for( int i = 0; i < 10; ++i )
				{
					IPC_OBJECT_STATUS objStatus;
					objStatus = GetIpcObjectStatus(ipcObjClient);

					if( objStatus == IPC_OBJECT_STATUS_LISTENING )
					{
						CHAR msgBuffer[0x100];
						DWORD msgSize = sizeof(msgBuffer);

						// manda buffer, recebe no mesmo modificado para "pong!"
						lstrcpyA(msgBuffer, "ping?");
						dwErr = SendIpcObjectMessage(ipcObjClient, 
							msgBuffer, msgSize,
							msgBuffer, msgSize,
							500);
						assertTrue(dwErr == ERROR_SUCCESS);

						// manda buffer maior que a encomenda: deve dar erro
						// obs: existe um assert interno para isso em debug
#ifndef _DEBUG
						lstrcpyA(msgBuffer, "buffer overflow?");
 						dwErr = SendIpcObjectMessage(ipcObjClient, 
							msgBuffer, 0x1000, // MAIOR que a maior mensagem
							msgBuffer, msgSize,
							500);
						assertTrue(dwErr == ERROR_MESSAGE_EXCEEDS_MAX_SIZE);
#endif

						// mensagem com nada. envio ok, mas sem resposta
						dwErr = SendIpcObjectMessage(ipcObjClient, NULL, 0, NULL, 0, 500);
						assertTrue(dwErr == ERROR_SUCCESS);

						// aguarda timeout do servidor
						dwErr = WaitForSingleObject(srvThr, 
							1500); // MAIOR que o timeout do objeto
						assertTrue(dwErr == WAIT_OBJECT_0);

						// agora sem loop, tem que dar erro
						dwErr = SendIpcObjectMessage(ipcObjClient, NULL, 0, NULL, 0, 500);
						assertTrue(dwErr != ERROR_SUCCESS);

						bErr = TRUE;
						break;
					}

					Sleep(100);
				}

				assertTrue(bErr == TRUE); // verifica se conseguiu entrar no loop

				CloseHandle(ipcObjClient);
			}

			// verifica retorno da thread servidora
			DWORD exitCode = 0;
			bErr = GetExitCodeThread(srvThr, &exitCode);
			assertTrue(bErr == TRUE);
			// tem que dar timeout, porque estamos saindo sem condição
			assertTrue(exitCode == ERROR_TIMEOUT);

			CloseHandle(srvThr);
		}

		CloseHandle(ipcObjSrv);
	}
}
