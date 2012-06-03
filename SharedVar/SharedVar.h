/** @file
Alocação de variáveis mapeadas na memória global do sistema 
através de memória mapeada.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 11/2006
*/
#pragma once
#include <windows.h>
#include <tchar.h>
#include <new>


/** @brief Aloca uma variável em memória mapeada, permitindo a qualquer processo 
com direitos enxergá-la e alterá-la.
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


/** @brief Abre uma variável que foi criada em memória mapeada, permitindo ao 
processo atual enxergar e alterar uma variável criada por outro processo.
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


/** @brief Libera visualização de uma variável em memória mapeada. Quando o 
último processo liberar a última visualização, a variável é eliminada da 
memória.
*/
template<typename T>
VOID FreeSharedVariable(HANDLE varH, T* pVar)
{
	if( pVar )
		UnmapViewOfFile(pVar);
	if( varH )
		CloseHandle(varH);
}


/** @brief Classe helper para as nossas funções de alocação de variáveis 
compartilhadas com o mundo. */
template<typename T>
class SharedVar
{
public:
	/** @brief Exceção lançada quando não é possível mapear a memória.
	@see SharedVar().
	*/
	struct SharedVarException : public std::bad_alloc
	{
		SharedVarException(DWORD err) : m_err(err) { }
		DWORD m_err; ///< Código de erro.
	};


	/** @brief Constrói o objeto já alocando memória mapeada com o nome 
	especificado.
	
	@param varName Nome da memória mapeada. Esse nome deve seguir as mesmas 
	regras quando é usada diretamente a função API <b>CreateFileMapping</b>.

	@return Caso não seja possível mapear a memória especificada, é lançada
	uma exceção do tipo SharedVarException.
	*/
	SharedVar(PCTSTR varName)
	{
		m_memPointer = 0;
		m_memHandle = AllocSharedVariable(&m_memPointer, varName);

		if( ! m_memHandle || ! m_memPointer )
			throw SharedVarException( GetLastError() );
	}


	/** @brief Libera recursos alocados para a memória mapeada. */
	~SharedVar()
	{
		FreeSharedVariable(m_memHandle, m_memPointer);
	}


	/** @brief Retorna referência para variável alocada. */
	operator T& ()
	{
		return *m_memPointer;
	}

	/** @brief Retorna o endereço da variável alocada. */
	T* operator & ()
	{
		return m_memPointer;
	}

	/** @brief Copia variável recebida como parâmetro na variável alocada. */
	T& operator = (const T& rvalue)
	{
		return *m_memPointer = rvalue;
	}
	
	/** @brief Retorna o tamanho em bytes do tipo da variável alocada. */
	size_t Size()
	{
		return sizeof(T);
	}

private:
	// proteção contra construção cópia e atribuição
	SharedVar(const SharedVar& obj);
	SharedVar& operator = (const SharedVar& obj);


	/** @brief Ponteiro para memória alocada com o tamanho do 
	tipo da variável. */
	T* m_memPointer;

	/** @brief Handle para memória mapeada que foi criada/aberta durante
	a alocação. */
	HANDLE m_memHandle;
};
