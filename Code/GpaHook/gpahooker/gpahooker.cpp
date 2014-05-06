#include <pe.h>
#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <atlbase.h>
#include <iostream>
#include <assert.h>

#define ASSERT assert

#pragma comment(lib, "CodeProtection.lib")


using namespace std;


VOID Error(HRESULT ret)
{
	TCHAR msgError[MAX_PATH] = _T("");

	if( FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ret, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		msgError, sizeof(msgError), NULL) )
	{
		cout << msgError << endl;
	}
	else
	{
		ASSERT(0);
	}
}


DWORD GetEntryPointAddress(const PROCESS_INFORMATION& pi, PVOID baseAddress, PVOID* pOEP)
{
	HRESULT ret = S_OK;
	PE pe;
	IMAGE_DOS_HEADER dosHeader;
	SIZE_T read = 0;

	if( ReadProcessMemory(pi.hProcess, baseAddress, 
		&dosHeader, sizeof(dosHeader), &read) )
	{
		IMAGE_NT_HEADERS32 ntHeader;
		read = 0;

		if( ReadProcessMemory(pi.hProcess, ((PBYTE)baseAddress) + dosHeader.e_lfanew,
			&ntHeader, sizeof(ntHeader), &read) )
		{
			/// @sa PE::RVAToPointer
			*pOEP = ((LPBYTE) baseAddress) + ntHeader.OptionalHeader.AddressOfEntryPoint;
		}
	}

	return HRESULT_CODE(ret);
}


DWORD GetBaseAddress(PCTSTR exePath, PVOID* baseAddress)
{
	HRESULT ret;
	PE pe;

	ret = pe.Open(CT2A(exePath), FALSE);

	if( SUCCEEDED(ret) )
		*baseAddress = *(PVOID*) &pe->optHeader->ImageBase;

	return HRESULT_CODE(ret);
}


#pragma pack(push, 1)

struct ForwardCallCode
{
	// isso é um jmp disfarçado
	//BYTE int3; // int 3 (breakpoint)
	BYTE push01; PVOID pLoadLibraryCode; // push pLoadLibraryCode
	BYTE ret01; // ret

	ForwardCallCode()
	{
		//int3 = 0xCC;
		push01 = 0x68;
		ret01 = 0xC3;
	}
};


struct LoadLibraryCode
{
	// chama LoadLibraryA
	BYTE push01; PSTR pdllPath; // push pdllPath
	BYTE movEax01; PVOID loadLibraryA; // mov eax, LoadLibraryA
	WORD callEax01; // call eax

	// chama WriteProcessMemory que restaura o entry point
	BYTE push02; SIZE_T* pWritten; // push pWritten
	BYTE push03; SIZE_T nSize; // push nSize
	BYTE push04; PVOID pEntryPointCode; // push pEntryPointCode
	BYTE push05; PVOID pEntryPoint01; // push pEntryPoint
	BYTE push06; HANDLE hProcess; // push hProcess

	BYTE movEax02; PVOID writeProcessMemory; // mov eax, WriteProcessMemory
	WORD callEax02; // call eax

	// retorna para entry Point
	BYTE push07; PVOID pEntryPoint02;
	BYTE ret01;

	// parâmetros
	CHAR dllPath[MAX_PATH];
	PBYTE entryPointCode[sizeof(ForwardCallCode)];
	SIZE_T written;

	LoadLibraryCode()
	{
		push01 = push02 = push03 = push04 = 0x68;
		push05 = push06 = push07 = 0x68;
		movEax01 = movEax02 = 0xB8;
		callEax01 = callEax02 = 0xD0FF;
		ret01 = 0xC3;
	}
};

#pragma pack(pop)


DWORD PatchEntryPoint(const PROCESS_INFORMATION& pi, PVOID entryPoint)
{
	DWORD ret = ERROR_SUCCESS;
	LoadLibraryCode* procLoadLibrary = (LoadLibraryCode*) VirtualAllocEx(pi.hProcess, NULL, 
		sizeof(LoadLibraryCode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if( procLoadLibrary )
	{
		LoadLibraryCode loadLibrary;
		SIZE_T read = 0;

		if( ReadProcessMemory(pi.hProcess, entryPoint, loadLibrary.entryPointCode, 
			sizeof(loadLibrary.entryPointCode), &read) )
		{
			lstrcpyA(loadLibrary.dllPath, "gpahook.dll");
			loadLibrary.pdllPath = procLoadLibrary->dllPath;
			loadLibrary.loadLibraryA = LoadLibraryA;
			loadLibrary.pWritten = &procLoadLibrary->written;
			loadLibrary.nSize = sizeof(loadLibrary.entryPointCode);
			loadLibrary.pEntryPointCode = procLoadLibrary->entryPointCode;
			loadLibrary.pEntryPoint01 = loadLibrary.pEntryPoint02 = entryPoint;
			loadLibrary.hProcess = GetCurrentProcess();
			loadLibrary.writeProcessMemory = WriteProcessMemory;

			DWORD dummy;

			if( WriteProcessMemory(pi.hProcess, procLoadLibrary, 
				&loadLibrary, sizeof(loadLibrary), &dummy) )
			{
				if( VirtualProtectEx(pi.hProcess, entryPoint, sizeof(ForwardCallCode),
					PAGE_EXECUTE_READWRITE, &dummy) )
				{
					ForwardCallCode fwdCode;
					fwdCode.pLoadLibraryCode = procLoadLibrary;

					DWORD written = 0;

					if( ! WriteProcessMemory(pi.hProcess, entryPoint, 
						&fwdCode, sizeof(fwdCode), &dummy) )
					{
						ret = GetLastError();
					}
				}
				else
					ret = GetLastError();
			}
			else
				ret = GetLastError();
		}
		else
			ret = GetLastError();
	}
	else
		ret = GetLastError();

	return ret;
}


DWORD PatchProcess(PCTSTR exePath, const PROCESS_INFORMATION& pi)
{
	DWORD ret;
	PVOID baseAddr = NULL;

	ret = GetBaseAddress(exePath, &baseAddr);

	if( ret == ERROR_SUCCESS )
	{
		PVOID entryPoint = NULL;

		ret = GetEntryPointAddress(pi, baseAddr, &entryPoint);

		if( ret == ERROR_SUCCESS )
		{
			ret = PatchEntryPoint(pi, entryPoint);
		}
	}

	return ret;
}


DWORD GpaHook(TCHAR* args[])
{
	DWORD ret = ERROR_SUCCESS;
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi = { };

	if( CreateProcess(args[0], args[1], NULL, NULL, TRUE,
		CREATE_SUSPENDED, NULL, NULL, &si, &pi) )
	{
		ret = PatchProcess(args[0], pi);
		ResumeThread(pi.hThread);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else
		ret = GetLastError();

	Error(HRESULT_FROM_WIN32(ret));
	return ret;
}


int _tmain(int argc, TCHAR* argv[])
{
	DWORD ret = ERROR_INVALID_PARAMETER;

	if( argc >= 2 )
		ret = GpaHook(&argv[1]);

	return int(ret);
}
