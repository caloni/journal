#include <iostream>
#include <typeinfo>

using namespace std;


class Base
{
public:
	Base()
	{
		cout << "Base()\n";
		m_x = 0;
	}

	~Base()
	{
		cout << "~Base()\n";
	}

	int m_x;
};


// como Base n�o � polim�rfica (como vamos ver), Deriv paga o pato
class Deriv : public Base
{
public:
	Deriv()
	{
		cout << "Deriv()\n";

		m_x = 1;
		m_y = 0;
	}

	virtual ~Deriv() // ela � polim�rfica
	{
		cout << "~Deriv()\n";
	}

	int m_y;
};


void func(Base* b) // eu n�o sei que � uma derivada
{
	cout << typeid(*b).name() << '\n'; // e nem o typeid n�mero 1
}


int main()
{
	Base* b = new Deriv(); // o ponteiro � pra base, mas a inst�ncia � de derivada
	func(b);
} 
