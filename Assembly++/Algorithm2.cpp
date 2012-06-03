#include "Algorithm.h"
#include <iostream>

using namespace std;


void EvenOrOdd(int number)
{
	if( number % 2 )
		cout << number << ": odd\n";
	else
		cout << number << ": even\n";
}

int sumSample(int a, int b)
{
	return a + b;
}
