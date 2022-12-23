#pragma once
#include <windows.h>
#include <tchar.h>

/** Aloca uma vari�vel em mem�ria mapeada, permitindo a qualquer processo
com direitos enxerg�-la e alter�-la.
*/

template<typename T>
HANDLE AllocSharedVariable(T** pVar, PCTSTR varName)
{
	DWORD varSize = sizeof(T);
	HANDLE ret = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
		0, varSize, varName);

	if( ret )
	{
		*pVar = (T*) MapViewOfFile(ret, FILE_MAP_ALL_ACCESS, 0, 0, 0);

		if( ! *pVar )
		{
			DWORD err = GetLastError();
			CloseHandle(ret);
			SetLastError(err);
		}
	}
	else
		*pVar = NULL;

	return ret;
}

/** Abre uma vari�vel que foi criada em mem�ria mapeada, permitindo ao
processo atual enxergar e alterar uma vari�vel criada por outro processo.
*/
template<typename T>
HANDLE OpenSharedVariable(T** pVar, PCTSTR varName)
{
	DWORD varSize = sizeof(T);
	HANDLE ret = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, varName);

	if( ret )
	{
		*pVar = (T*) MapViewOfFile(ret, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, varSize);

		if( ! *pVar )
		{
		DWORD err = GetLastError();
		CloseHandle(ret);
		ret = NULL;
		SetLastError(err);
		}
	}
	else
		*pVar = NULL;

	return ret;
}

/** Libera visualiza��o de uma vari�vel em mem�ria mapeada. Quando o �ltimo processo
liberar a �ltima visualiza��o, a vari�vel � eliminada da mem�ria.
*/
template<typename T>
VOID FreeSharedVariable(HANDLE varH, T* pVar)
{
	if( pVar )
		UnmapViewOfFile(pVar);
	if( varH )
		CloseHandle(varH);
}