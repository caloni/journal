#include "..\EvenOdd\EvenOdd.h"
#include <stdio.h>

bool IsEven(int number)
{
    static int total = 0;

    bool even = (number % 2) == 0;

    total += number;
    printf("Total: %d\n", total);

    return even;
}
