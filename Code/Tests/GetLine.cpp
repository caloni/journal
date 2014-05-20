#include "Tests.h"

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;
 

bool LeiaLinha(string& nome, int& distancia)
{
	string a;
	if( getline(cin, nome) )
	{
		if( cin >> distancia >> a )
			return true;
	}

	return false;
}

void GetLine()
{
	int distancia, soma =  0;
	double quantidade = 0.0;
	string nome;
	
	while( LeiaLinha(nome, distancia)){
		cout << nome << endl << distancia << endl;
		soma+= distancia;
		quantidade++;
	}	
		
	cout << fixed << setprecision(1) << soma/quantidade <<"\n";
}
