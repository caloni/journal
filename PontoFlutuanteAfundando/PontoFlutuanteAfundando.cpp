#include <iostream>

int main()
{
	double d1 = 2358.93;
	double d2 = 1386.93;
	double d3 = d1 - d2;

	std::cout << "d1: " << d1 << "\n";
	std::cout << "d2: " << d2 << "\n";
	std::cout << "d1 - d2 = 3d: " << d3 << "\n";

	// comparando armazenamentos que diferem
	std::cout << "d3 == 972.0: " << std::boolalpha << ( d3 == 972.0 ) << "\n";

	// comparando armazenamentos similares
	std::cout << "d1 == 2358.93: " << std::boolalpha << ( d1 == 2358.93 ) << "\n";
	std::cout << "d2 == 1386.93: " << std::boolalpha << ( d2 == 1386.93 ) << "\n";
}
