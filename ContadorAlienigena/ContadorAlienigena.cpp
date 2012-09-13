#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>


void Increment(int counter[], int base)
{
    if( ++counter[0] == base )
    {
        counter[0] = 0;
        Increment(&counter[1], base);
    }
}


void Print(int counter[], int counterSize, char alien[])
{
    bool mostSign = false;
    for( int i = counterSize - 1; i >= 0; --i )
    {
        if( counter[i] || mostSign || i == 0 )
        {
            printf("%c", alien[counter[i]]);
            mostSign = true;
        }
    }
}


int main()
{
    char alien[100] =  { };
    int counter[20] = { };
    int count = 0;
    int base = 0;

    printf("Digite a sequencia alienigena: ");
    scanf("%s", alien);

    printf("Devo contar ate quando? ");
    scanf("%d", &count);

    base = strlen(alien);

    for( int i = 0; i <= count; ++i )
    {
        Print(counter, 20, alien);
        printf("\n");
        Increment(counter, base);
    }
}
