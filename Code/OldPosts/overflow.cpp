#include <limits.h>
#include <iostream>

int main()
{
	int x = INT_MAX; // m�ximo inteiro que pode ser armazenado no tipo int

	std::cout << x << std::endl; // se � o m�ximo, � um valor positivo
	x = x + 1;  // mas basta um empurr�ozinho para que
	std::cout << x << std::endl; // a casa caia
} 
