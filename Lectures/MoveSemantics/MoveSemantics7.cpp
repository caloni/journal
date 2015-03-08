#include "MoveSemantics.h"

using namespace std;

void PassByReference4(int& x)
{
    x = x + 11;
    cout << "Pass by reference: " << x << endl;
}

void PassByReference4(int&& x)
{
    x = x + 11;
    cout << "Pass by reference (rvalue): " << x << endl;
}

template<typename T>
void PassByReferenceT(T&& x)
{
    PassByReference4(std::forward<T>(x));
}

void MoveSemantics7()
{
    int x = 20;

    PassByReferenceT(x);
    PassByReferenceT(20);
}
