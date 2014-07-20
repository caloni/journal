#include "Recursion.h"
#include <iostream>

using namespace std;


void FactorialPrint(int n)
{
	int fact1 = Factorial(n);
	int fact2 = FactorialTail(n);
	cout << "Factorial for " << n << ": " << fact1 << endl;
	cout << "FactorialTail for " << n << ": " << fact2 << endl;
}


int Factorial(int n)
{
	if (n > 1)
		return Factorial(n - 1) * n;
	else
		return 1;
}


int FactorialTail(int n, int a)
{
	if (n < 0)
		return 0;
	else if (n == 0)
		return 1;
	else if (n == 1)
		return a;
	else
		return FactorialTail(n - 1, n * a);
}
