#include <iostream>
#include <stdlib.h>

int main()
{
    int x = 0x00AB3451;
    printf("int x = 0x%08X;\n", x); // n�o � t�o leg�vel, mas d� conta do recado
    std::cout << "int x = 0x" << std::hex << x << ";" << std::endl; // pois �, parece que melhoramos mesmo com streams...
}

