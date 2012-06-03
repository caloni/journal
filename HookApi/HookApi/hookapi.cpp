#include <windows.h>
#include <iostream>
#include "liveasm.h"
#include "pe.h"

#define SIZEOF_ARRAY(ar) (sizeof(ar)/sizeof((ar)[0])) // ks.h
#define LIB_CODE_SIZE ( 0x1000 )
#define EXE_BASE_ADDR ( 0x01000000 )

using namespace std;

BOOL AllocWriteProcessMemoryCode(HANDLE process, PVOID entryPoint, PVOID loadLibCodeAddr);
DWORD AllocLoadLibraryCode(HANDLE process, PVOID entryPoint, PCSTR dllPath);
BOOL GetEntryPoint(HANDLE process, PVOID* entryPoint);
DWORD InjectDll(PPROCESS_INFORMATION processInfo, PCSTR dllPath);


#pragma pack( push )
#pragma pack( 1 )

struct WriteProcessMemoryCode
{
	// empilha parâmetros de WriteProcessMemory
	PushCode<PVOID> pushWritten;
	PushCode<DWORD> pushSize;
	PushCode<PVOID> pushBuffer;
	PushCode<PVOID> pushBaseAddress;
	PushCode<PVOID> pushProcess;

	// chama WriteProcessMemory
	CallCode callWriteProcessMemory;

	RetCode ret;

	SIZE_T written;
	BYTE memoryCode[MAX_PATH];

	WriteProcessMemoryCode()
	{
		written = 0;
		ZeroMemory(memoryCode, sizeof(memoryCode));
		callWriteProcessMemory.movEax.value = 
			GetProcAddress(GetModuleHandle("kernel32"), "WriteProcessMemory");
	}
};


struct HiddenCallCode
{
	CallCode call;
	WriteProcessMemoryCode writeProcessMemory;
	RetCode ret;

	HiddenCallCode()
	{
	}
};


struct LoadLibraryCode
{
	// empilha endereço de dllPath
	PushCode<PVOID> pushDllPath;

	// chama a função LoadLibraryA
	CallCode callLoadLibraryA;

	// retorna para função chamadora
	RetCode ret;

	// path da dll
	CHAR dllPath[MAX_PATH];

	LoadLibraryCode()
	{
		ZeroMemory(dllPath, sizeof(dllPath));
		callLoadLibraryA.movEax.value = 
			GetProcAddress(GetModuleHandle("kernel32"), "LoadLibraryA");
	}
};


#pragma pack( pop )


BOOL AllocHiddenCallCode(HANDLE process, PVOID entryPoint, PVOID loadLibCodeAddr)
{
	BOOL ret = FALSE;
	PVOID callCodeAddr = VirtualAllocEx(process, NULL, sizeof(HiddenCallCode),
		MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if( callCodeAddr )
	{
		PushJmpCode pushJmpCode;
		HiddenCallCode hiddenCallCode;

		pushJmpCode.pushEax.movEax.value = entryPoint;
		pushJmpCode.jmpEax.movEax.value = callCodeAddr;

		DWORD wasRead = 0;
		if( ReadProcessMemory(process, entryPoint, hiddenCallCode.writeProcessMemory.memoryCode, 
			sizeof(PushJmpCode), &wasRead) )
		{
			DWORD written = 0;
			if( WriteProcessMemory(process, entryPoint, &pushJmpCode, 
				sizeof(pushJmpCode), &written) )
			{
				hiddenCallCode.call.movEax.value = loadLibCodeAddr;

				WriteProcessMemoryCode& wpmParams = hiddenCallCode.writeProcessMemory;
				wpmParams.pushBaseAddress.movEax.value = entryPoint;
				wpmParams.pushBuffer.movEax.value = static_cast<PBYTE>(callCodeAddr)
					+ offsetof(HiddenCallCode, writeProcessMemory) 
					+ offsetof(WriteProcessMemoryCode, memoryCode);
				wpmParams.pushProcess.movEax.value = GetCurrentProcess();
				wpmParams.pushSize.movEax.value = sizeof(PushJmpCode);
				wpmParams.pushWritten.movEax.value = static_cast<PBYTE>(callCodeAddr)
					+ offsetof(HiddenCallCode, writeProcessMemory) 
					+ offsetof(WriteProcessMemoryCode, written);

				if( WriteProcessMemory(process, callCodeAddr, &hiddenCallCode, 
					sizeof(hiddenCallCode), &written) )
				{
					ret = TRUE;
				}
			}
		}
		else
		{
			DWORD err = GetLastError();
			VirtualFreeEx(process, callCodeAddr, sizeof(HiddenCallCode), MEM_RELEASE);
			SetLastError(err);
		}
	}

	return ret;
}


DWORD AllocLoadLibraryCode(HANDLE process, PVOID entryPoint, PCSTR dllPath)
{
	DWORD ret = ERROR_SUCCESS;
	PVOID loadLibCodeAddr = VirtualAllocEx(process, NULL, LIB_CODE_SIZE,
		MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if( loadLibCodeAddr )
	{
		LoadLibraryCode loadLibCode;

		lstrcpyn(loadLibCode.dllPath, dllPath, SIZEOF_ARRAY(loadLibCode.dllPath));
		loadLibCode.pushDllPath.movEax.value = static_cast<PBYTE>(loadLibCodeAddr)
			+ offsetof(LoadLibraryCode, dllPath);

		loadLibCode.callLoadLibraryA.movEax.value = GetProcAddress(GetModuleHandle("kernel32"), "LoadLibraryA");

		DWORD written = 0;
		BOOL writeProcMem = WriteProcessMemory(process, loadLibCodeAddr, 
			&loadLibCode, sizeof(loadLibCode), &written);

		if( ! writeProcMem || ! AllocHiddenCallCode(process, entryPoint, loadLibCodeAddr) )
		{
			ret = GetLastError();
			VirtualFreeEx(process, loadLibCodeAddr, LIB_CODE_SIZE, MEM_RELEASE);
			loadLibCodeAddr = NULL;
		}
	}
	else
		ret = GetLastError();

	return ret;
}


BOOL GetEntryPoint(HANDLE process, PVOID* entryPoint)
{
	BOOL ret = FALSE;
	DWORD err = ERROR_SUCCESS;
	PVOID baseAddress = reinterpret_cast<PVOID>( EXE_BASE_ADDR );
	const DWORD toRead = sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS32);
	BYTE buffer[toRead];
	DWORD wasRead = 0;

	if( ReadProcessMemory(process, baseAddress, buffer, toRead, &wasRead) )
	{
		if( toRead == wasRead )
		{
			HRESULT hr;
			PE pe;

			hr = pe.Open(static_cast<PVOID>( buffer ));

			if( SUCCEEDED(hr) )
			{
				DWORD dwEntryPoint = pe->ntHeader->OptionalHeader.AddressOfEntryPoint 
					+ EXE_BASE_ADDR;

				*entryPoint = reinterpret_cast<PVOID>( dwEntryPoint );
				ret = TRUE;

				pe.Close();
			}
			else
				err = HRESULT_CODE(hr);
		}
	}
	else
		err = GetLastError();

	if( err != ERROR_SUCCESS )
		SetLastError(err);

	return ret;
}


DWORD InjectDll(PPROCESS_INFORMATION processInfo, PCSTR dllPath)
{
	DWORD ret = ERROR_SUCCESS;
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo->dwProcessId);
	PVOID entryPoint = NULL;

	if( process && GetEntryPoint(process, &entryPoint) )
	{
		ret = AllocLoadLibraryCode(process, entryPoint, dllPath);

		if( ret == ERROR_SUCCESS )
			ResumeThread(processInfo->hThread);
		else
			TerminateProcess(processInfo->hProcess, ret);

		CloseHandle(process);
	}
	else
		ret = GetLastError();

	return ret;
}


void LoadLib()
{
	__asm
	{
		mov eax, 0x12345678
		call eax
	}
}


int main(int argc, char* argv[])
{
	//LoadLib();

	if( argc == 2 )
	{
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;

		BOOL result = CreateProcess(argv[1], NULL, NULL, NULL, 
			FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);

		if( result )
		{
			InjectDll(&pi, "D:\\Projetos\\temp\\hookapi\\hookapidll\\Debug\\hookapidll.dll");

			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
	}
}
