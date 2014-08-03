#include "stdafx.h"
#include "Service.h"
#include <iostream>

using namespace std;

Service* Service::Instance = nullptr;

Service::Service(std::string name, std::string description)
{
	Instance = this;
	_name = name;
	_description = description;
	_statusHandle = 0;
	SetConsoleTitle(_name.c_str());
}

bool Service::Install()
{
	bool ret = false;

	if (SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE))
	{
		char path[_MAX_PATH + 1];
		GetModuleFileNameA(NULL, path, sizeof(path) / sizeof(path[0]));

		if (SC_HANDLE service = CreateService(scm,
			_name.c_str(), _description.c_str(),
			SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
			SERVICE_AUTO_START, SERVICE_ERROR_IGNORE, path,
			0, 0, 0, 0, 0))
		{
			ret = true;
			CloseServiceHandle(service);
		}
		CloseServiceHandle(scm);
	}

	if (ret)
		cout << "Service installed successfully\n";
	else 
		cout << "Error " << GetLastError() << " installing " << _name << endl;

	return ret;
}

bool Service::Uninstall()
{
	bool ret = false;
	
	if (SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT))
	{
		if (SC_HANDLE service = OpenServiceA(scm, _name.c_str(), SERVICE_QUERY_STATUS | DELETE))
		{
			ret = DeleteService(service) == TRUE;
			CloseServiceHandle(service);
		}
		CloseServiceHandle(scm);
	}

	if (ret)
		cout << "Service uninstalled successfully\n";
	else
		cout << "Error " << GetLastError() << " uninstalling " << _name << endl;

	return ret;
}

bool Service::Run()
{
	char* name = const_cast<char*>(_name.c_str());
	SERVICE_TABLE_ENTRY serviceTable[] = {
		{ name, &Service::ServiceMain },
		{ 0, 0 }
	};
	return StartServiceCtrlDispatcher(serviceTable) == TRUE;
}

void Service::Debug()
{
	_status.dwCurrentState = SERVICE_RUNNING;
	Main();
}

void WINAPI Service::ServiceMain(DWORD /*argc*/, char* /*argv*/[])
{
	Service::Instance->ServiceMain();
}

void WINAPI Service::ServiceControlHandler(DWORD controlCode)
{
	Service::Instance->Control(controlCode);
}

void Service::WaitForDebugger()
{
	CHAR serviceModulePath[MAX_PATH] = "";
	GetModuleFileName(NULL, serviceModulePath, MAX_PATH);
	if (char *ext = strrchr(serviceModulePath, '.'))
		strcpy(ext, ".ini");
	else
		strcat(serviceModulePath, ".ini");

	while (GetPrivateProfileInt(_name.c_str(), "Debug", 0, serviceModulePath)  && !IsDebuggerPresent())
		Sleep(1000);
}

void Service::ServiceMain()
{
	WaitForDebugger();

	_status.dwServiceType = SERVICE_WIN32;
	_status.dwCurrentState = SERVICE_STOPPED;
	_status.dwControlsAccepted = 0;
	_status.dwWin32ExitCode = NO_ERROR;
	_status.dwCheckPoint = 0;
	_status.dwWaitHint = 0;
	_status.dwServiceSpecificExitCode = NO_ERROR;
	_statusHandle = RegisterServiceCtrlHandler(_name.c_str(), &Service::ServiceControlHandler);

	if (_statusHandle)
	{
		_status.dwCurrentState = SERVICE_START_PENDING;
		SetServiceStatus(_statusHandle, &_status);

		_status.dwControlsAccepted |= (SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		_status.dwCurrentState = SERVICE_RUNNING;
		SetServiceStatus(_statusHandle, &_status);

		Main(); // it's alive!

		_status.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(_statusHandle, &_status);

		_status.dwControlsAccepted &= ~(SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);
		_status.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(_statusHandle, &_status);
	}
}

void Service::Control(DWORD controlCode)
{
	switch (controlCode)
	{
	case SERVICE_CONTROL_SHUTDOWN:
		strcpy((char*)(0x00000078), "I don't know what the f*** I'm doing...");
	case SERVICE_CONTROL_STOP:
		_status.dwCurrentState = SERVICE_STOP_PENDING;
		SetServiceStatus(_statusHandle, &_status);
		return;
	}
	SetServiceStatus(_statusHandle, &_status);
}
