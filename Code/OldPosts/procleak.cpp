#include <windows.h>
#include <stdio.h>


int main()
{
	DWORD pid;

	while( scanf("%d", &pid) == 1 )
	{
		HANDLE proc = OpenProcess(SYNCHRONIZE, FALSE, pid);
	}
}

// Codigo-fonte disponivel no GitHub (Caloni/Caloni.com.br) 
