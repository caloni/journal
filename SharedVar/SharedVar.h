/** @file
Aloca��o de vari�veis mapeadas na mem�ria global do sistema 
atrav�s de mem�ria mapeada.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 11/2006
*/
#pragma once
#include <windows.h>
#include <tchar.h>
#include <new>


/** @brief Aloca uma vari�vel em mem�ria mapeada, permitindo a qualquer processo 
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


/** @brief Abre uma vari�vel que foi criada em mem�ria mapeada, permitindo ao 
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


/** @brief Libera visualiza��o de uma vari�vel em mem�ria mapeada. Quando o 
�ltimo processo liberar a �ltima visualiza��o, a vari�vel � eliminada da 
mem�ria.
*/
template<typename T>
VOID FreeSharedVariable(HANDLE varH, T* pVar)
{
	if( pVar )
		UnmapViewOfFile(pVar);
	if( varH )
		CloseHandle(varH);
}


/** @brief Classe helper para as nossas fun��es de aloca��o de vari�veis 
compartilhadas com o mundo. */
template<typename T>
class SharedVar
{
public:
	/** @brief Exce��o lan�ada quando n�o � poss�vel mapear a mem�ria.
	@see SharedVar().
	*/
	struct SharedVarException : public std::bad_alloc
	{
		SharedVarException(DWORD err) : m_err(err) { }
		DWORD m_err; ///< C�digo de erro.
	};


	/** @brief Constr�i o objeto j� alocando mem�ria mapeada com o nome 
	especificado.
	
	@param varName Nome da mem�ria mapeada. Esse nome deve seguir as mesmas 
	regras quando � usada diretamente a fun��o API <b>CreateFileMapping</b>.

	@return Caso n�o seja poss�vel mapear a mem�ria especificada, � lan�ada
	uma exce��o do tipo SharedVarException.
	*/
	SharedVar(PCTSTR varName)
	{
		m_memPointer = 0;
		m_memHandle = AllocSharedVariable(&m_memPointer, varName);

		if( ! m_memHandle || ! m_memPointer )
			throw SharedVarException( GetLastError() );
	}


	/** @brief Libera recursos alocados para a mem�ria mapeada. */
	~SharedVar()
	{
		FreeSharedVariable(m_memHandle, m_memPointer);
	}


	/** @brief Retorna refer�ncia para vari�vel alocada. */
	operator T& ()
	{
		return *m_memPointer;
	}

	/** @brief Retorna o endere�o da vari�vel alocada. */
	T* operator & ()
	{
		return m_memPointer;
	}

	/** @brief Copia vari�vel recebida como par�metro na vari�vel alocada. */
	T& operator = (const T& rvalue)
	{
		return *m_memPointer = rvalue;
	}
	
	/** @brief Retorna o tamanho em bytes do tipo da vari�vel alocada. */
	size_t Size()
	{
		return sizeof(T);
	}

private:
	// prote��o contra constru��o c�pia e atribui��o
	SharedVar(const SharedVar& obj);
	SharedVar& operator = (const SharedVar& obj);


	/** @brief Ponteiro para mem�ria alocada com o tamanho do 
	tipo da vari�vel. */
	T* m_memPointer;

	/** @brief Handle para mem�ria mapeada que foi criada/aberta durante
	a aloca��o. */
	HANDLE m_memHandle;
};
