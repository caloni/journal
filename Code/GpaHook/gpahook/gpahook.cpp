#include <pe.h>
#include <windows.h>

#pragma comment(lib, "CodeProtection.lib")


#pragma pack(push, 1)

struct OutputDebugStringHookFunc
{
	BYTE push01; PVOID strPtr; // push strPtr
	BYTE movEax01; PVOID outpDbg; // mov eax, outpDbg
	WORD callEax01; // call eax

	BYTE push02; PVOID origFunc; // push origFunc
	BYTE ret01; // ret

	CHAR str[100]; // ver strPtr
};


struct WriteFileHookFunc
{
	BYTE push01; PVOID lpOverlapped; // push lpOverlapped
	BYTE push02; PDWORD pdwWritten; // push pdwWritten
	BYTE push03; DWORD dwToWrite; // push dwToWrite
	BYTE push04; PVOID pBuffer; // push pBuffer
	BYTE push05; HANDLE hFile; // push hFile

	BYTE movEax01; PVOID writeFile; // mov eax, WriteFile
	WORD callEax01; // call eax

	BYTE push06; PVOID origFunc; // push origFunc
	BYTE ret01; // ret

	CHAR buffer[100]; // ver pBuffer
	DWORD dwWritten; // ver pdwWritten
};

#pragma pack(pop)



FARPROC WINAPI OnGetProcAddressWriteFile(HMODULE hModule, LPCSTR lpProcName)
{
	static HANDLE traceFile = INVALID_HANDLE_VALUE;

	FARPROC ret = GetProcAddress(hModule, lpProcName);

	if( ret )
	{
		if( traceFile == INVALID_HANDLE_VALUE )
		{
			traceFile = CreateFile(_T("C:\\GpaHook.log"), GENERIC_WRITE, 
				0, NULL, CREATE_ALWAYS, 0, NULL);
		}

		if( traceFile != INVALID_HANDLE_VALUE )
		{
			CHAR gpaTrace[200] = "GetProcAddress: ";
			DWORD written = 0;
			lstrcatA(gpaTrace, lpProcName);
			lstrcatA(gpaTrace, "\r\n");
			WriteFile(traceFile, gpaTrace, lstrlenA(gpaTrace), &written, NULL);

			PVOID newFunc = VirtualAlloc(NULL, sizeof(WriteFileHookFunc), 
				MEM_COMMIT, PAGE_EXECUTE_READWRITE);

			if( newFunc )
			{
				WriteFileHookFunc& func = *(WriteFileHookFunc*)newFunc;

				func.push01 = func.push02 = func.push03 = 0x68;
				func.push04 = func.push05 = func.push06 = 0x68;
				func.movEax01 = 0xB8;
				func.callEax01 = 0xD0FF;
				func.ret01 = 0xC3;

				func.pBuffer = func.buffer;
				lstrcpyA(func.buffer, lpProcName);
				lstrcatA(func.buffer, "\r\n");

				func.lpOverlapped = NULL;
				func.pdwWritten = &func.dwWritten;
				func.dwToWrite = lstrlenA(func.buffer);
				func.hFile = traceFile;

				func.writeFile = WriteFile;
				func.origFunc = ret;

				ret = (FARPROC) newFunc;
			}
		}
	}

	return ret;
}


FARPROC WINAPI OnGetProcAddressOutputDebugString(HMODULE hModule, LPCSTR lpProcName)
{
	FARPROC ret = GetProcAddress(hModule, lpProcName);

	if( ret )
	{
		CHAR gpaTrace[200] = "GetProcAddress: ";

		lstrcatA(gpaTrace, lpProcName);
		lstrcatA(gpaTrace, "\r\n");
		OutputDebugStringA(gpaTrace);

		PVOID newFunc = VirtualAlloc(NULL, sizeof(OutputDebugStringHookFunc), 
			MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		if( newFunc )
		{
			OutputDebugStringHookFunc& func = *(OutputDebugStringHookFunc*)newFunc;

			func.push01 = func.push02 = 0x68;
			func.movEax01 = 0xB8;
			func.callEax01 = 0xD0FF;
			func.ret01 = 0xC3;

			func.strPtr = func.str;
			lstrcpyA(func.str, lpProcName);
			lstrcatA(func.str, "\r\n");
			func.outpDbg = OutputDebugStringA;
			func.origFunc = ret;

			ret = (FARPROC) newFunc;
		}
	}

	return ret;
}


void ChangeGetProcAddress()
{
	HRESULT hr;
	PE pe;

	hr = pe.Open(GetModuleHandle(NULL));
	
	if( SUCCEEDED(hr) )
	{
		FARPROC gpaAddr = GetProcAddress(GetModuleHandle(_T("kernel32")), "GetProcAddress");
		FARPROC* iatAddr = (FARPROC*) pe.GetIATFunctionAddress(gpaAddr);

		if( iatAddr )
		{
			DWORD oldProt = 0;

			if( VirtualProtect(iatAddr, 4, PAGE_EXECUTE_READWRITE, &oldProt) )
			{
				*iatAddr = (FARPROC) OnGetProcAddressWriteFile;
				VirtualProtect(iatAddr, 4, oldProt, &oldProt);
			}
		}

		pe.Close();
	}
}


VOID GpaHookInitialize()
{
	return;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if( fdwReason == DLL_PROCESS_ATTACH )
	{
		MessageBox(NULL, _T("DllMain"), _T("GpaHook.dll"), 0);
		ChangeGetProcAddress();
	}

	return TRUE;
}
