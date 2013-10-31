// ReadEntireFileStl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>


using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	while( --argc )
	{
		fstream ifs(argv[argc], ios::in | ios::out | ios::binary);
		string entireFile = "#include \"stdafx.h\"\r\n" + string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
		ifs.seekg(ios::beg);
		copy(entireFile.begin(), entireFile.end(), ostreambuf_iterator<char>(ifs));

		cout << entireFile << endl;
	}

	return 0;
}
