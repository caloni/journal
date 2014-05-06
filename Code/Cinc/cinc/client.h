/** Módulo de chamada do lado cliente.
@author Wanderley Caloni Jr <wanderley@caloni.com.br>
@date jan.2006
*/
#pragma once
#include <windows.h>


namespace Cinc
{


class Client
{
public:
	static const int DefaultTimeout = INFINITE;

	Client(PCTSTR baseName);
	~Client();

	HRESULT Call();

	DWORD Timeout();
	void Timeout(DWORD timeout);

private:
	Client(const Client&) { }
	operator = (const Client&) { }

	HRESULT Open();
	HRESULT Close();

	TCHAR m_baseName[MAX_PATH];

	HANDLE m_sharedMemory;
	HANDLE m_clientCall;
	HANDLE m_clientServer;
	HANDLE m_serverClient;
	PVOID m_serverView;
	bool m_opened;

	DWORD m_timeout;
};


} // namespace Cinc
