#include <windows.h>
#include <iostream>
#include <time.h>

using namespace std;

// declaramos que existe uma classe com esse nome
class FuzzyCall;

// ponteiro para m�todos da classe acima
typedef void (FuzzyCall::*FP_Fuzzy)();

/** Classe que faz chamada de um m�todo aleat�rio. */
class FuzzyCall
{
public:

	FuzzyCall()
	{
		srand(GetTickCount()); // chacoalha o saco de bingo
	}

   FP_Fuzzy GiveMeAMethod() { return m_methods[rand() % 3]; }

private:
	void MethodOne()   { cout << "One!\n"; }
	void MethodTwo()   { cout << "Two!\n"; }
	void MethodThree() { cout << "Three!\n"; }

	static FP_Fuzzy m_methods[3];
};

/** Array com os m�todos que podem ser chamados aleatoriamente. */
FP_Fuzzy FuzzyCall::m_methods[3] = { &MethodOne, &MethodTwo, &MethodThree };

/** Recebe um ponteiro para um m�todo de FuzzyCall e chama com um objeto local. */
void passThrough(FP_Fuzzy pMethod)
{
	FuzzyCall fuzzyObject; // esse � o objeto local
	( fuzzyObject.*pMethod )(); // essa � a chamada
}

/** No princ�pio Deus disse: 'int main!'
*/
int main()
{
	FuzzyCall fuzzyObject1;
	FP_Fuzzy pMethod;

	// pegamos um m�todo da classe qualquer
	pMethod = fuzzyObject1.GiveMeAMethod();

	// e passamos para uma outra fun��o
	passThrough(pMethod);
} 
