/** Módulo de chamada do lado servidor.
@author Wanderley Caloni Jr <wanderley@caloni.com.br>
@date jan.2006
*/
#pragma once
#include <windows.h>


namespace Cinc
{


class Server
{
public:
	static const int BufferSize = 4096;
	static const int DefaultTimeout = INFINITE;

	Server(PCTSTR baseName);
	~Server();

	HRESULT Start(bool wait);
	HRESULT Stop();
	operator bool () const;

	DWORD Timeout();
	void Timeout(DWORD timeout);

private:
	Server(const Server&) { }
	operator = (const Server&) { }

	HRESULT Create();
	HRESULT Close();
	static DWORD WINAPI StopThread(PVOID);

	bool AlreadyRunning() const;
	static DWORD WINAPI WorkerThread(PVOID);

	TCHAR m_baseName[MAX_PATH];

	HANDLE m_sharedMemory;
	HANDLE m_clientCall;
	HANDLE m_clientServer;
	HANDLE m_serverClient;
	PVOID m_serverView;
	bool m_opened;

	HANDLE m_workerThread;
	DWORD m_timeout;
	bool m_started;
};


} // namespace Cinc
