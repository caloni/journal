/** @file

@brief Defini��es e fun��es de impacto global ao sistema. Ex: Trace, asserts.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 08/2006
*/
#include "ipcobjcommon.h"
#include <tchar.h>
#include <aclapi.h>
#include <stdio.h>


#ifdef _DEBUG

/** Gera linha de log para depura��o do sistema de objetos.

@param strFmp String com formata��o id�ntica � usada na fun��o sprintf da LIBC.
*/
VOID IpcObjectTrace(PCSTR strFmt, ...)
{
	CHAR msgBuf[200] = "";

	va_list vl;
	va_start(vl, strFmt);
	vsprintf(msgBuf, strFmt, vl);
	va_end(vl);

	lstrcatA(msgBuf, "\r\n");
	OutputDebugStringA(msgBuf);
}

#endif


/** Chama a fun��o dispon�vel na kernel32.dll.

@remark N�o chamar dentro da DllMain.
*/
void RealZeroMemory(PVOID Destination, SIZE_T Length)
{
	HMODULE kernel32 = GetModuleHandle(_T("kernel32.dll"));

	if( kernel32 )
	{
		typedef void (WINAPI FZM)(PVOID, SIZE_T);

		if( FZM* zm = (FZM*) GetProcAddress(kernel32, "RtlZeroMemory") )
			zm(Destination, Length);
	}
}


/** Chama a fun��o dispon�vel na kernel32.dll.

@remark N�o chamar dentro da DllMain.
*/
void RealCopyMemory(PVOID Destination, const VOID* Source, SIZE_T Length)
{
	HMODULE kernel32 = GetModuleHandle(_T("kernel32.dll"));

	if( kernel32 )
	{
		typedef void (WINAPI FZM)(PVOID, const VOID*, SIZE_T);

		if( FZM* zm = (FZM*) GetProcAddress(kernel32, "RtlMoveMemory") )
			zm(Destination, Source, Length);
	}
}


/** Obt�m os atributos de seguran�a de um objeto.

@param object Handle para objeto ipc.

@return Se for constru�do com sucesso, ponteiro para o descritor de seguran�a
do objeto ipc. Caso contr�rio, NULL. Deve ser usada a fun��o GetLastError
para mais informa��es.
*/
PSECURITY_ATTRIBUTES GetIpcObjectSecurity(HANDLE object)
{
	PSECURITY_ATTRIBUTES ret = reinterpret_cast<PSECURITY_ATTRIBUTES>(
		LocalAlloc(LPTR, sizeof(SECURITY_ATTRIBUTES)) );

	ASSERT(ret != NULL);

	if( ret )
	{
		DWORD handleFlags = 0;
		GetHandleInformation(object, &handleFlags);
		ret->bInheritHandle = ( handleFlags & HANDLE_FLAG_INHERIT )
			? TRUE : FALSE;

		DWORD err = GetSecurityInfo(object, SE_KERNEL_OBJECT, 
			0, NULL, NULL, NULL, NULL,
			reinterpret_cast<PSECURITY_DESCRIPTOR*>(&ret->lpSecurityDescriptor));

		// condi��o para a plataforma 9x (95/98/ME)
		if( err == ERROR_CALL_NOT_IMPLEMENTED )
		{
			FreeIpcObjectSecurity(ret);
			ret = NULL;
			err = ERROR_SUCCESS;
		}

		ASSERT(err == ERROR_SUCCESS);
		SetLastError(err);
	}

	return ret;
}


/** Libera os atributos de seguran�a obtidos em GetIpcObjectSecurity.

@param objSec Handle para objeto ipc.

@return Se for desalocado com sucesso, TRUE. Caso contr�rio, FALSE. Deve ser 
usada a fun��o GetLastError para mais informa��es.
*/
BOOL FreeIpcObjectSecurity(PSECURITY_ATTRIBUTES objSec)
{
	BOOL ret = FALSE;

	if( objSec )
	{
		if( objSec->lpSecurityDescriptor )
			LocalFree(objSec->lpSecurityDescriptor);
		LocalFree(objSec);
		ret = TRUE;
	}

	return ret;
}
