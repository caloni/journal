#include <iostream>

struct MinhaEstrutura
{
    MinhaEstrutura()
    {
        x = 0;
        y = 2;
        c = 'C';
    }

    int x;
    int y;
    char c;
};

int main()
{
    MinhaEstrutura me;
    std::cout << "x: " << me.x << ", y: " << me.y << ", c: " << me.c << std::endl;
}

