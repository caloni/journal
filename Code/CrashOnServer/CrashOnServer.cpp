#define _CRT_SECURE_NO_WARNINGS
#include <string.h>

void func(char* str)
{
    // mas nem vai dar tempo de inicializar!
    strcpy(str, "Aqui jaz um programa t�o legal...");
}

int main()
{
    char* myString = nullptr; // ainda n�o inicializamos...
    func(myString);
}
