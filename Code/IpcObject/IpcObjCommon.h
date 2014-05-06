/*
Defini��es e fun��es de impacto global ao sistema. Ex: Trace, asserts.
*/
#pragma once
#include <windows.h>
#include <assert.h>

//
// Dispositivo de trace.
//
#ifdef _DEBUG
#define LOG(args) IpcObjectTrace args
#define ASSERT(expr) assert(expr)
#else
#define LOG(args)
#define ASSERT(expr)
#endif


/** @brief Gera linha de log para depura��o do sistema de objetos. */
VOID IpcObjectTrace(PCSTR strFmt, ...);

/** @brief Zera mem�ria sem depender da LIBC. */
void RealZeroMemory(PVOID Destination, SIZE_T Length);

/** @brief Copia mem�ria sem depender da LIBC. */
void RealCopyMemory(PVOID Destination, const VOID* Source, SIZE_T Length);

/** @brief Obt�m os atributos de seguran�a de um objeto. */
PSECURITY_ATTRIBUTES GetIpcObjectSecurity(HANDLE object);

/** @brief Libera os atributos de seguran�a obtidos em GetIpcObjectSecurity. */
BOOL FreeIpcObjectSecurity(PSECURITY_ATTRIBUTES objSec);


/** Retorna o n�mero de elementos de um array do tipo T.

@param T Tipo do array.

@param Sz N�mero de elementos do array.

@param tArray Vari�vel do array.

@return N�mero de elementos do array que � passado para par�metro.
*/
template<typename T, size_t Sz>
size_t SizeofArray(const T (&tArray)[Sz])
{
	return Sz;
}
