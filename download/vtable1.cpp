#include <iostream>

class C
{
public:
        void method()
	{
		std::cout << "C::method\n";
	}
};

class D : public C
{
public:
        void method()
	{
		std::cout << "D::method\n";
	}
};

void func(C* c)
{
        c->method(); 
}

int main()
{
        D d;
        func(&d); // passa endere�o de C "dentro de D"
}

