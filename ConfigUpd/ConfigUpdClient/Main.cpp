#include "..\ConfigUpdLib\ConfigUpdLib.h"
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <string>

using namespace std;


int main(int argc, char* argv[])
{
	if( argc == 4 )
	{
		DWORD err = ConfigUpd::SendFile(argv[1], argv[2], atoi(argv[3]));
	}
	else while( true )
	{
		string command;

		cout << "Type some command (sendfile [filepath], quit): ";
		cin >> command;
	
		if( command == "quit" )
		{
			cout << "Exiting... have a nice day!" << endl;
			break;
		}
		else if( command == "sendfile" )
		{
			string filePath;

			cin.get();
			getline(cin, filePath);
			cout << "Sending file " << filePath << "..." << endl;
			DWORD err = ConfigUpd::SendFile("localhost", filePath.c_str(), 0);

			if( err == ERROR_SUCCESS )
				cout << "File sent successfully!\n";
			else 
				cout << "Error " << err << " sending file" << endl;
		}
		else cout << "Unknown command!" << endl;
	}
}
