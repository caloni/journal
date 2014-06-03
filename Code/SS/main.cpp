#include "ss.h"
#include <iostream>

int main()
{
    for( int i = 0; i < 5; ++i )
    {
        std::string s = ss() << "Teste numero " << i;
        std::cout << s << std::endl;
    }
}

