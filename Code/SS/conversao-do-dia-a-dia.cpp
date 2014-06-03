#include <iostream>
#include <string>
#include <sstream> // digam oi para nosso novo amiguinho!

std::string FuncaoCorriqueira(int x, int y)
{
    std::ostringstream ss; // credo, que tipo eh esse?
    ss << (x + y);
    return ss.str();
}

int main()
{
    std::cout << FuncaoCorriqueira(20, 42);
}

