#include <windows.h>

DWORD ClassicHandleLeak()
{
	DWORD ret = 0;
	HKEY hKey;

	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Something", 
		0, GENERIC_READ, &hKey) == ERROR_SUCCESS )
	{
		DWORD retSz = sizeof(ret);

		if (RegQueryValueEx(hKey, L"SomeValue", NULL, NULL, 
			(PBYTE) &ret, &retSz) == ERROR_SUCCESS)
		{
			// success!
			return ret;
		}

		RegCloseKey(hKey);
	}

	return ret;
}

int main()
{
	while (true)
	{
		HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId());
		Sleep(1000);
	}
}