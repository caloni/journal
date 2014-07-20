#include "..\EvenOdd\EvenOdd.h"
#include <stdio.h>

typedef bool (*FP_IsEven)(int);

FP_IsEven IsEvenUpdate = nullptr;
bool IsEvenUpdateReady = false;


bool IsEven(int number)
{
    if( IsEvenUpdateReady )
        return IsEvenUpdate(number);

    static int total = 0;

    bool even = (number % 2) == 0;

    total += number;
    printf("Total: %d\n", total);

    return even;
}
