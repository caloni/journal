#include "CodeJam.h"
#include <fstream>

using namespace std;


int main(int argc, char* argv[])
{
	ostream* pOs = nullptr;
	istream* pIs = nullptr;

	if( argc != 3 )
	{
		pIs = &cin;
		pOs = &cout;
	}
	else
	{
		pIs = new ifstream(argv[1]);
		pOs = new ofstream(argv[2]);
	}

	istream& is = *pIs;
	ostream& os = *pOs;

	int cases = 0;
	is >> cases;

	for( int caseNum = 0; caseNum < cases; ++caseNum )
	{
		os << "Case #" << caseNum+1 << ": ";
		TestCase(is, os);
	}
}
