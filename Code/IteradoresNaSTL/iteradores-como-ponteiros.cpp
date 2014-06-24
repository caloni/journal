
template<typename T>
T VaiSomandoVaiSomando(T begin, T end)
{
    T current = begin;
    T next = begin + 1;

    while( next != end )
    {
        *next = *current + *next;
        ++current;
        ++next;
    }

    return current;
}

#include <vector>
#include <iostream>

int main()
{
    int aInts[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    std::vector<int> vInts;
    for( int i = 0; i < 10; ++i )
        vInts.push_back(i);

    std::cout << *VaiSomandoVaiSomando(&aInts[0], &aInts[10]) << std::endl;
    std::cout << *VaiSomandoVaiSomando(vInts.begin(), vInts.end()) << std::endl;
}

