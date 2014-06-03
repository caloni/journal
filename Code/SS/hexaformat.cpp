#include <iostream>
#include <stdlib.h>

int main()
{
    int x = 0x00AB3451;
    printf("int x = 0x%08X;\n", x); // nao eh tao legivel, mas da conta do recado
    std::cout << "int x = 0x" << std::hex << x << ";" << std::endl; // pois eh, parece que melhoramos mesmo com streams...
}

