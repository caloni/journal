#include "..\EvenOdd\EvenOdd.h"

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#pragma comment(lib, "EvenOdd.lib")


void DoProcess()
{
    int nextNumber = rand() % 1000;
    bool even = IsEven(nextNumber);
    printf("%d => %s\n", nextNumber, even ? "par" : "impar");
}


int main()
{
    srand( (unsigned int) time(0) );

    while( true )
    {
        DoProcess();
        Sleep(3000);
    }
}
