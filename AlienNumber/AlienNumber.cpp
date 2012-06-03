#include <iostream>
#include <map>
#include <list>
#include <string>

using namespace std;

typedef map<char, int> SourceLanguage;
typedef map<int, char> TargetLanguage;


void CreateSourceLanguage(const string& source, SourceLanguage& sourceLang, int& baseNum)
{
	for( size_t i = 0; i < source.size(); ++i )
	{
		sourceLang[source[i]] = i;
	}
	baseNum = source.size();
}


void CreateTargetLanguage(const string& target, TargetLanguage& targetLang, int& baseNum)
{
	for( size_t i = 0; i < target.size(); ++i )
	{
		targetLang[i] = target[i];
	}
	baseNum = target.size();
}


int AlienToI(const string& input, SourceLanguage sourceLang, int baseNum)
{
	int ret = 0;
	const char* str = input.c_str();

	do
	{
		ret *= baseNum;
		ret += sourceLang[*str++];
	}
	while( *str );

	return ret;
}


void PrintAlienNumber(int number, TargetLanguage targetLang, int baseNum)
{
	list<char> output;

	do
	{
		int digit = number % baseNum;
		output.push_front(targetLang[digit]);
		number /= baseNum;
	}
	while( number );

	for( list<char>::const_iterator curr = output.begin(), end = output.end();
		curr != end; curr++ )
	{
		cout << *curr;
	}
}


int main()
{
	int tests;
	cin >> tests;

	for( int i = 1; i <= tests; ++i )
	{
		string input, source, target;
		SourceLanguage sourceLang;
		int sourceBase;
		TargetLanguage targetLang;
		int targetBase;

		cin >> input >> source >> target;
		CreateSourceLanguage(source, sourceLang, sourceBase);
		CreateTargetLanguage(target, targetLang, targetBase);

		int number = AlienToI(input, sourceLang, sourceBase);

		cout << "Case #" << i << ": ";
		PrintAlienNumber(number, targetLang, targetBase);
		cout << endl;
	}
}
