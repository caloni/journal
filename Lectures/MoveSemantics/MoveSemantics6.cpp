#include "MoveSemantics.h"

using namespace std;

void PassByReference3(int& x)
{
    x = x + 11;
    cout << "Pass by reference: " << x << endl;
}

void PassByReference3(int&& x)
{
    x = x + 11;
    cout << "Pass by reference (rvalue): " << x << endl;
}

template<typename T> struct MyRemoveReference
{ 
    typedef T type;
};

template<typename T> struct MyRemoveReference<T&>
{
    typedef T type;
};

template<typename T> 
typename remove_reference<T>::type&& 
    MyMove(T&& arg) {
        return ((typename MyRemoveReference<T>::type&&) arg); // remove_reference
}

void MoveSemantics6()
{
    int x = 20;

    PassByReference3(static_cast<int&&>(x));
    PassByReference3(std::move(x));
    PassByReference3(MyMove(x));
}
