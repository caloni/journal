#pragma once
#include <string>
#include <windows.h>

class Service
{
public:
	Service(std::string name, std::string description);
	bool Install();
	bool Uninstall();
	bool Run();
	void Debug();

private:
	static void WINAPI ServiceMain(DWORD argc, char* argv[]);
	static void WINAPI ServiceControlHandler(DWORD controlCode);

	void ServiceMain();
	void WaitForDebugger();
	void Control(DWORD controlCode);

	virtual void Main() = 0;

protected:
	std::string	_name;
	std::string _description;
	::SERVICE_STATUS _status;
	::SERVICE_STATUS_HANDLE	_statusHandle;

private:
	static Service* Instance;
};
