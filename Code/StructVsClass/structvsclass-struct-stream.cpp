#include <iostream>

struct MinhaEstrutura
{
    MinhaEstrutura() { x = 0; y = 2; c = 'C'; }
    int x; int y; char c;
};

std::ostream& operator << (std::ostream& os, const MinhaEstrutura& me)
{
    std::cout << "x: " << me.x << ", y: " << me.y << ", c: " << me.c;
    return os;
}

int main()
{
    MinhaEstrutura me;
    std::cout << me << std::endl;
}

