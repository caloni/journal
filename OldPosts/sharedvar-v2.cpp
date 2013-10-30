/** Classe helper para as nossas fun��es de aloca��o de vari�veis
compartilhadas com o mundo. */
template<typename T>
class SharedVar
{
public:
	// se conseguir, parab�ns; sen�o, retorna BUM!
	SharedVar(PCTSTR varName)
	{
		m_memPointer = 0;
		m_memHandle = AllocSharedVariable(&m_memPointer, varName);

		if( ! m_memHandle || ! m_memPointer )
			throw GetLastError();
	}

	// libera recursos alocados para a vari�vel
	~SharedVar()
	{
		FreeSharedVariable(m_memHandle, m_memPointer);
	}

	T& operator * ()
	{
		return *m_memPointer;
	}

private:
	// n�o vamos nos preocupar com isso agora
	SharedVar(const SharedVar& obj);
	SharedVar& operator = (const SharedVar& obj);

	T* m_memPointer;
	HANDLE m_memHandle;
};// Codigo-fonte disponivel no GitHub (Caloni/Caloni.com.br) 
