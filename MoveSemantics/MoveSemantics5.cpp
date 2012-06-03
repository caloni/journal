#include "MoveSemantics.h"

using namespace std;

void PassByReference2(int& x)
{
    x = x + 11;
    cout << "Pass by reference: " << x << endl;
}

void PassByReference2(int&& x)
{
    x = x + 11;
    cout << "Pass by reference (rvalue): " << x << endl;
}

void MoveSemantics5()
{
    int x = 20;

    PassByReference2(x);
    PassByReference2(20);
    PassByReference2(static_cast<int&&>(x));
    PassByReference2(std::move(x));
}
