#include <iostream>

int main()
{
    int x = 0x00000001;
    int y = 4;
    int z = x << y; // isso desloca 4 bits para a "esquerda"
    std::cout << z // WHAT??
        << "\nestranho..." << std::endl; // WHAT^^2!?!?!?!??!?!
}

