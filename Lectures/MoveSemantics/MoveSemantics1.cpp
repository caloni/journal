#include "MoveSemantics.h"

using namespace std;

void PassByValue(int x)
{
    x = x + 11;
    cout << "Pass by value: " << x << endl;
}

void PassByValue(int* x)
{
    *x = *x + 11;
    cout << "Pass by value (pointer): " << x << endl;
}

void PassByReference(int& x)
{
    x = x + 11;
    cout << "Pass by reference: " << x << endl;
}

//void PassByReference(const int& x)
//{
//    //x = x + 11;
//    cout << "Pass by reference: " << x << endl;
//}

void PassByReference(int&& x)
{
    x = x + 11;
    cout << "Pass by reference (rvalue): " << x << endl;
}

void MoveSemantics1()
{
    int x = 20;

    PassByValue(x);
    PassByValue(&x);
    PassByReference(x);
    PassByReference(20);
}
