/*
Definições e funções de impacto global ao sistema. Ex: Trace, asserts.
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


/** @brief Gera linha de log para depuração do sistema de objetos. */
VOID IpcObjectTrace(PCSTR strFmt, ...);

/** @brief Zera memória sem depender da LIBC. */
void RealZeroMemory(PVOID Destination, SIZE_T Length);

/** @brief Copia memória sem depender da LIBC. */
void RealCopyMemory(PVOID Destination, const VOID* Source, SIZE_T Length);

/** @brief Obtém os atributos de segurança de um objeto. */
PSECURITY_ATTRIBUTES GetIpcObjectSecurity(HANDLE object);

/** @brief Libera os atributos de segurança obtidos em GetIpcObjectSecurity. */
BOOL FreeIpcObjectSecurity(PSECURITY_ATTRIBUTES objSec);


/** Retorna o número de elementos de um array do tipo T.

@param T Tipo do array.

@param Sz Número de elementos do array.

@param tArray Variável do array.

@return Número de elementos do array que é passado para parâmetro.
*/
template<typename T, size_t Sz>
size_t SizeofArray(const T (&tArray)[Sz])
{
	return Sz;
}
