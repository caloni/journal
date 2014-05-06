#include <string>
#include <sstream>
#include <iostream>


using namespace std;


string ReturnString(int number)
{
	ostringstream os;
	os << "This is the number " << number << ", that is a really good number.";
	return os.str();
}


int main(int argc, char* argv[])
{
	while( --argc )
	{
		string s = ReturnString(atoi(argv[argc]));
		cout << "basic_string has " << sizeof(s) << " bytes\n";
		cout << s << endl;
	}
}
