/** Módulo de chamada do lado cliente.
@author Wanderley Caloni Jr <wanderley@caloni.com.br>
@date jan.2006
*/
#include "client.h"
#include "cinc.h"

#include <iostream>

#define SIZEOF_ARRAY(ar) (sizeof(ar)/sizeof((ar)[0])) // ks.h


using namespace std;


Cinc::Client::Client(PCTSTR baseName)
{
	if( ! lstrcpyn(m_baseName, baseName, SIZEOF_ARRAY(m_baseName)) )
		m_baseName[0] = 0;

	m_sharedMemory = NULL;
	m_clientCall = NULL;
	m_clientServer = NULL;
	m_serverClient = NULL;
	m_serverView = NULL;
	m_opened = false;

	m_timeout = DefaultTimeout;
}


Cinc::Client::~Client()
{
	if( m_opened )
		Close();
}


HRESULT Cinc::Client::Open()
{
	HRESULT hr = S_OK;

	if( m_opened )
		Close();

	TCHAR objectName[sizeof(m_baseName)];

	lstrcpy(objectName, m_baseName);
	lstrcat(objectName, CINC_CLIENT_CALL_SUFFIX);
	if( m_clientCall = OpenMutex(MUTEX_ALL_ACCESS, FALSE, objectName) )
	{
		lstrcpy(objectName, m_baseName);
		lstrcat(objectName, CINC_CLIENT_SERVER_SUFFIX);
		if( m_clientServer = OpenEvent(EVENT_ALL_ACCESS, FALSE, objectName) )
		{
			lstrcpy(objectName, m_baseName);
			lstrcat(objectName, CINC_SERVER_CLIENT_SUFFIX);
			if( m_serverClient = OpenEvent(EVENT_ALL_ACCESS, FALSE, objectName) )
			{
				lstrcpy(objectName, m_baseName);
				lstrcat(objectName, CINC_SHARED_MEMORY_SUFFIX);
				if( m_sharedMemory = OpenFileMapping(FILE_MAP_ALL_ACCESS, 
					FALSE, objectName) )
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

	return hr;
}


HRESULT Cinc::Client::Close()
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


HRESULT Cinc::Client::Call()
{
	HRESULT hr = Open();

	if( hr == S_OK )
	{
		DWORD wait;

		wait = WaitForSingleObject(m_clientCall, m_timeout);

		if( wait == WAIT_OBJECT_0 )
		{
			lstrcpy((char*)m_serverView, "chamando, chamando");
			SetEvent(m_clientServer);
			wait = WaitForSingleObject(m_serverClient, m_timeout);

			if( wait == WAIT_OBJECT_0 )
				cout << (char*) m_serverView << endl;

			ResetEvent(m_clientServer);
			ResetEvent(m_serverClient);
			ReleaseMutex(m_clientCall);
		}
		
		Close();
	}

	return hr;
}
