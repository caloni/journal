#include "..\EvenOdd\EvenOdd.h"
#include <stdio.h>

typedef bool (*FP_IsEven)(int);

FP_IsEven IsEvenUpdate = nullptr;
bool IsEvenUpdateReady = false;

static int EvenTotal = 0;
static int OddTotal = 0;


bool IsEven(int number)
{
    if( IsEvenUpdateReady )
        return IsEvenUpdate(number);

    bool even = (number % 2) == 0;

    if( even )
        ++EvenTotal;
    else
        ++OddTotal;

    printf("FINAL VERSION: %d par, %d impar, %d total\n", EvenTotal, OddTotal, EvenTotal + OddTotal);

    return even;
}
