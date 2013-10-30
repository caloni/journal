/** Exemplo de como usar as fun��es de aloca��o de mem�ria compartilhada
AllocSharedVariable, OpenSharedVariable e FreeSharedVariable.
*/
int _tmain(int argc, PTSTR argv[])
{
	try
	{
		// passou algum par�metro: l� a vari�vel compartilhada e exibe
		if( argc > 1 )
		{
			system("pause");

			// array de 100 TCHARs
			SharedVar<TCHAR [100]> sharedVar(_T(SHARED_VAR));

			_tprintf(_T("Frase secreta: \'%s\'\n"), *sharedVar);
			_tprintf(_T("Pressione <enter> para retornar..."));
			getchar();
		}
		else // n�o passou par�metro: escreve na vari�vel 
		// compartilhada e chama nova inst�ncia
		{
			// array de 100 TCHARs
			SharedVar<TCHAR [100]> sharedVar(_T(SHARED_VAR));

			PTSTR cmd = new TCHAR[ _tcslen(argv[0]) + 10 ];
			_tcscpy(cmd, _T("\""));
			_tcscat(cmd, argv[0]);
			_tcscat(cmd, _T("\" 2"));

			_tcscpy(*sharedVar,
			_T("Vassora de sa, vassora de su, vassora de tuturuturutu!"));
			_tsystem(cmd);

			delete [] cmd;
		}
	}
	catch(DWORD err)
	{
		_tprintf(_T("Erro %08X.\n"), err);
	}

	return 0;
}// Codigo-fonte disponivel no GitHub (Caloni/Caloni.com.br) 
