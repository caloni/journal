#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

int main()
{
	double number = 100.432;
	cout << setprecision(log10(number) + 3) << number;
}
