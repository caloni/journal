#include <iostream>

double round(double r)
{
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

int main()
{
	double d1 = 2358.93;
	double d2 = 1386.93;
	double d3 = round(d1) - round(d2);

	std::cout << "d1: " << d1 << "\n";
	std::cout << "d2: " << d2 << "\n";
	std::cout << "d1 - d2 = 3d: " << d3 << "\n";
	std::cout << "d3 == 972.0: " << std::boolalpha << ( d3 == 972.0 ) << "\n";
}
