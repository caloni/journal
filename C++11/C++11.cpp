// C++11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>


using namespace std;


void* func()
{
    return nullptr;
}

class C
{
 // não funciona ainda int a=7; //C++11 only
public:
 C();
};


struct A
{
 // aindanão funcionaA()=default; //C++11
 // aindanão funcionavirtual ~A()=default; //C++11
    //void func() = delete;
};


void f(int) //#1
{
}


void f(char *)//#2
{
}


void test()
{
    //C++03
    f(0); //which f is called?
    //C++11
    f(nullptr); //unambiguous, calls #2
}


class M //C++11 delegating constructors
{
 int x, y;
 char *p;
public:
 M(int v) : x(v), y(0),  p(new char [100])  {} //#1 target
 // não funciona ainda M(): M(0) {cout<<"delegating ctor"<<endl;} //#2 delegating
};



class Movable
{
    string name;
    int age;

public:
    Movable()
        : age()
    {
    }

    Movable(const string& _name, int _age)
        : name(_name), age(_age)
    {
    }

    Movable(string&& _name)
        : name(_name), age()
    {
    }

    Movable (Movable&& source) //move constructor
//        : name(source.name), age(source.age)
    {
        *this = move(source);
    }

    Movable (Movable& source) //move constructor
        : name(source.name), age(source.age)
    {
    }

    Movable& operator=(Movable&& source) //move assignment operator
    {
        *this = move(source);
        //name = source.name;
        //age = source.age;
        return *this;
    }
};


int main()
{
    Movable m1("Joao", 15);
    Movable m2("Pedro", 16);

    Movable m3(m1);
    Movable m4 = string("Paulo");

    m4 = m2;

    test();

    // não funciona ainda int* a = new int[3] { 1, 2, 0 }; //C++11 only


    // não funciona aindavector<int> ints = { 1, 2, 3 };


    char s[]="Hello World!";
    int Uppercase = 0; //modified by the lambda

    for_each(s, s+sizeof(s), [&Uppercase] (char c) {
        if (isupper(c))
            Uppercase++;
    });

    auto x = 0;
    auto y = 3 * 2;
    auto z = func();
    auto s2 = s;

    cout<< Uppercase<<" uppercase letters in: "<< s<<endl;
}
