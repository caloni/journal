#include <windows.h>
#include "../hookapi/pe.h"
#include "../hookapi/liveasm.h"


PVOID GetProcAddressOfGetProcAddress(PE& pe)
{
	PVOID ret = NULL;

	PIMAGE_IMPORT_DESCRIPTOR importDescriptor = 
      pe.RVAToPointer<IMAGE_IMPORT_DESCRIPTOR>(
      pe->dirsHeader[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	while( importDescriptor->Name )
	{
		// o nome da dll sendo lida
		PSTR dllName = pe.RVAToPointer<CHAR>(importDescriptor->Name);

		// a lista de nomes e hints das funções da dll
		PIMAGE_THUNK_DATA impNameTable = 
			pe.RVAToPointer<IMAGE_THUNK_DATA>(importDescriptor->OriginalFirstThunk);

		// se feito o bound, os endereços das funções; caso constrário, == impName
		PIMAGE_THUNK_DATA impAddrTable = 
			pe.RVAToPointer<IMAGE_THUNK_DATA>(importDescriptor->FirstThunk);

		while( impNameTable->u1.AddressOfData )
		{
			// conjunto hint/name da função lida
			PIMAGE_IMPORT_BY_NAME impByName = 
				pe.RVAToPointer<IMAGE_IMPORT_BY_NAME>(impNameTable->u1.AddressOfData);

			// nome da função sendo lida
			PSTR funcName = reinterpret_cast<PSTR>( impByName->Name );

			// endereço da função sendo lida (se feito o bound)
			PVOID funcAddr = reinterpret_cast<PVOID>( impAddrTable->u1.Function );

			if( lstrcmpi(dllName, "kernel32.dll") == 0
				&& lstrcmp(funcName, "GetProcAddress") == 0 )
			{
				ret = funcAddr;
				break;
			}

			++impNameTable; // próxima função importada
			++impAddrTable; // próximo endereço de função
		}

		++importDescriptor; // próxima dll
	}

	return ret;
}


void HookGPA(PVOID base)
{
	PE pe;
	HRESULT hr;

	hr = pe.Open(base);

	if( SUCCEEDED(hr) )
	{
		PVOID gpa = GetProcAddressOfGetProcAddress(pe);
		pe.Close();
	}
}


BOOL WINAPI DllMain(HINSTANCE, DWORD reason, PVOID)
{
	if( reason == DLL_PROCESS_ATTACH )
	{
		MessageBox(NULL, "Hook", "", 0);
		__asm int 3
		HookGPA(GetModuleHandle(NULL));
	}

	return TRUE;
}
