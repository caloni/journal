#include <iostream>

using namespace std;

/** Troca o valor entre duas vari�veis inteiras. Ou seja, ao final da fun��o
a vari�vel first ir� conter o valor da vari�vel second e vice-versa.
*/
void anormalSwap(int &first, int& second)
{
	first = first ^ second; // first cont�m first e second juntos
	second = first ^ second; // firstXORsecond XOR second = first
	first = first ^ second; // second = first. logo, firstXORsecond XOR first = second
}

int main()
{
	int first = 13;
	int second = 42;

	cout << "first: " << first << ", second: " << second << endl;
	anormalSwap(first, second);
	cout << "first: " << first << ", second: " << second << endl;
} 
