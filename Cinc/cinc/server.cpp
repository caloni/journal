/** Módulo de chamada do lado servidor.
@author Wanderley Caloni Jr <wanderley@caloni.com.br>
@date jan.2006
*/
#include "server.h"
#include "cinc.h"
#include <iostream>

#define SIZEOF_ARRAY(ar) (sizeof(ar)/sizeof((ar)[0])) // ks.h

using namespace std;


Cinc::Server::Server(PCTSTR baseName)
{
	if( ! lstrcpyn(m_baseName, baseName, SIZEOF_ARRAY(m_baseName)) )
		m_baseName[0] = 0;

	m_sharedMemory = NULL;
	m_clientCall = NULL;
	m_clientServer = NULL;
	m_serverClient = NULL;
	m_serverView = NULL;
	m_opened = false;

	m_workerThread = NULL;
	m_timeout = DefaultTimeout;
	m_started = false;
}


Cinc::Server::~Server()
{
	if( m_opened )
		Close();

	if( m_started )
		Stop();
}


HRESULT Cinc::Server::Create()
{
	HRESULT hr = S_OK;

	if( m_opened )
		Close();

	if( ! AlreadyRunning() )
	{
		TCHAR objectName[sizeof(m_baseName)];

		lstrcpyn(objectName, m_baseName, SIZEOF_ARRAY(objectName));
		lstrcat(objectName, CINC_CLIENT_CALL_SUFFIX);
		if( m_clientCall = CreateMutex(NULL, FALSE, objectName) )
		{
			lstrcpyn(objectName, m_baseName, SIZEOF_ARRAY(objectName));
			lstrcat(objectName, CINC_CLIENT_SERVER_SUFFIX);
			if( m_clientServer = CreateEvent(NULL, TRUE, FALSE, objectName) )
			{
				lstrcpyn(objectName, m_baseName, SIZEOF_ARRAY(objectName));
				lstrcat(objectName, CINC_SERVER_CLIENT_SUFFIX);
				if( m_serverClient = CreateEvent(NULL, TRUE, FALSE, objectName) )
				{
					lstrcpyn(objectName, m_baseName, SIZEOF_ARRAY(objectName));
					lstrcat(objectName, CINC_SHARED_MEMORY_SUFFIX);
					if( m_sharedMemory = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, 
						PAGE_READWRITE, 0, BufferSize, objectName) )
					{
						if( m_serverView = MapViewOfFile(m_sharedMemory, 
							FILE_MAP_ALL_ACCESS, 0, 0, 0) )
						{
							m_opened = true;
						}
						else
							hr = HRESULT_FROM_WIN32( GetLastError() );
					}
					else
						hr = HRESULT_FROM_WIN32( GetLastError() );
				}
				else
					hr = HRESULT_FROM_WIN32( GetLastError() );
			}
			else
				hr = HRESULT_FROM_WIN32( GetLastError() );
		}
		else
			hr = HRESULT_FROM_WIN32( GetLastError() );

		if( hr != S_OK )
			Close();
	}
	else
		hr = HRESULT_FROM_WIN32( ERROR_ALREADY_EXISTS );

	return hr;
}


bool Cinc::Server::AlreadyRunning() const
{
	bool ret;
	TCHAR objectName[MAX_PATH];
	HANDLE clientCall;
	HANDLE clientServer;
	HANDLE serverClient;
	HANDLE sharedMemory;

	lstrcpy(objectName, m_baseName);
	lstrcat(objectName, CINC_CLIENT_CALL_SUFFIX);
	clientCall = OpenMutex(MUTEX_ALL_ACCESS, FALSE, objectName);

	lstrcpy(objectName, m_baseName);
	lstrcat(objectName, CINC_CLIENT_SERVER_SUFFIX);
	clientServer = OpenEvent(EVENT_ALL_ACCESS, FALSE, objectName);

	lstrcpy(objectName, m_baseName);
	lstrcat(objectName, CINC_SERVER_CLIENT_SUFFIX);
	serverClient = OpenEvent(EVENT_ALL_ACCESS, FALSE, objectName);

	lstrcpy(objectName, m_baseName);
	lstrcat(objectName, CINC_SHARED_MEMORY_SUFFIX);
	sharedMemory = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, objectName);

	ret = clientCall || clientServer || serverClient || sharedMemory;

	if( clientCall )
		CloseHandle(clientCall), clientCall = NULL;

	if( clientServer )
		CloseHandle(clientServer), clientServer = NULL;

	if( serverClient )
		CloseHandle(serverClient), serverClient = NULL;

	if( sharedMemory )
		CloseHandle(sharedMemory), sharedMemory = NULL;

	return ret;
}


Cinc::Server::operator bool () const
{
	return m_started;
}


HRESULT Cinc::Server::Close()
{
	if( m_clientCall )
	{
		CloseHandle(m_clientCall);
		m_clientCall = NULL;
	}

	if( m_serverView )
	{
		UnmapViewOfFile(m_serverView);
		m_serverView = NULL;
	}

	if( m_sharedMemory )
	{
		CloseHandle(m_sharedMemory);
		m_sharedMemory = NULL;
	}

	if( m_serverClient )
	{
		CloseHandle(m_serverClient);
		m_serverClient = NULL;
	}

	if( m_clientServer )
	{
		CloseHandle(m_clientServer);
		m_clientServer = NULL;
	}

	m_opened = false;

	return S_OK;
}


DWORD WINAPI Cinc::Server::WorkerThread(PVOID pvThis)
{
	DWORD ret = ERROR_SUCCESS;
	Server* pThis = static_cast<Server*>( pvThis );

	cout << "Cinc::Server::WorkerThread " << pThis << " begin...\n";

	while( WaitForSingleObject(pThis->m_clientServer, pThis->m_timeout)
		== WAIT_OBJECT_0 )
	{
		/// @todo Aguardar chamadas do cliente e repassar para OnCall().
		cout << (char*) pThis->m_serverView << endl;
		lstrcat((char*) pThis->m_serverView, " OK");
		ResetEvent(pThis->m_clientServer);
		SetEvent(pThis->m_serverClient);
	}

	DWORD tid = 0;
	CloseHandle( CreateThread(NULL, 0, StopThread, pThis, 0, &tid) );

	cout << "Cinc::Server::WorkerThread " << pThis << " end...\n";
	return ret;
}


HRESULT Cinc::Server::Start(bool wait)
{
	HRESULT hr = m_started ? S_FALSE : S_OK;

	if( hr == S_OK )
	{
		hr = Create(); // criamos os recursos para responder à chamadas remotas...

		if( hr == S_OK )
		{
			DWORD tid = 0;

			// ... e a thread de monitoramento de chamadas
			m_workerThread = CreateThread(NULL, 0, WorkerThread, this, 0, &tid);

			if( m_workerThread )
			{
				m_started = true;

				if( wait )
					WaitForSingleObject(m_workerThread, INFINITE);
			}
			else
			{
				hr = HRESULT_FROM_WIN32( GetLastError() );
				Close(); // se não criar a thread, fecha todos os recursos
			}
		}
	}

	return  hr;
}


HRESULT Cinc::Server::Stop()
{
	HRESULT hr = m_started ? S_OK : S_FALSE;

	if( hr == S_OK )
	{
		Close();
		DWORD wait = WaitForSingleObject(m_workerThread, Timeout());

		switch( wait )
		{
		case WAIT_OBJECT_0:
			{
				DWORD exitCode = 0;
				GetExitCodeThread(m_workerThread, &exitCode);
				hr = HRESULT_FROM_WIN32(exitCode);
				break;
			}

		case WAIT_TIMEOUT:
			{
				hr = HRESULT_FROM_WIN32( ERROR_TIMEOUT );
				break;
			}
		}

		CloseHandle(m_workerThread), m_workerThread = NULL;
		m_started = false;
	}

	return  hr;
}


DWORD WINAPI Cinc::Server::StopThread(PVOID pvThis)
{
	DWORD ret = ERROR_SUCCESS;
	Server* pThis = static_cast<Server*>( pvThis );

	ret = pThis->Stop();

	return ret;
}


DWORD Cinc::Server::Timeout()
{
	return m_timeout;
}


void Cinc::Server::Timeout(DWORD timeout)
{
	m_timeout = timeout;
}
