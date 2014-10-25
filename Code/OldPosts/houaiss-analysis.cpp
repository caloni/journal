#include <iostream>
#include <string>

int main()
{
	char cmd; // comando da linha atualmente lida
	string line; // linha atualmente lida
	int count = 0; // contador de palavras

	while( getline(cin, line) )
	{
		cmd = line[0]; // guardamos o comando
		line.erase(0, 1); // tiramos o comando da linha
		format(line); // formata��o da linha (explica��es adiante)

		switch( cmd ) // que comando � esse?
		{
		case '*': // verbete
			++count;
			cout << '\n' << line << '\n';
			break;

		case ':': // defini��o
			cout << line << "<br>\n";
			break;
		}
	}

	return 0;
}


 
