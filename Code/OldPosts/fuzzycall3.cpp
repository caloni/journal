#include <windows.h>
#include <iostream>
#include <time.h>

using namespace std;

// declaramos que existe uma classe com esse nome
class FuzzyCall;

// ponteiro para m�todos da classe acima
typedef void (FuzzyCall::*FP_Fuzzy)();

// ponteiro para inteiros da classe acima
typedef int FuzzyCall::*PI_Fuzzy;

/** Classe que faz chamada de um m�todo aleat�rio. */
class FuzzyCall
{
public:
	FuzzyCall()
	{
		int bingoStone = rand() % 100; // tira uma pedra do saco
		m_bingoStone = bingoStone; // guarda a pedra que tiramos do saco
	}

	int m_bingoStone; // pedra que tiramos do saco de bingo

	// retorna a pedra que tiramos do saco de bingo
	void PrintStone()
	{
		cout << "this: " << hex << this << ", member: " << dec << m_bingoStone << endl;
	}
};

/** No princ�pio Deus disse: 'int main!'
*/
int main()
{
	srand(GetTickCount()); // chacoalha o saco de bingo

	FuzzyCall fuzzyObject1;
	FuzzyCall fuzzyObject2;
	FuzzyCall fuzzyObject3;
	FP_Fuzzy pMethod = &FuzzyCall::PrintStone;
	PI_Fuzzy pMember = &FuzzyCall::m_bingoStone;

	// podemos chamar o mesmo m�todo para diversos objetos
	(fuzzyObject1.*pMethod)();
	(fuzzyObject2.*pMethod)();
	(fuzzyObject3.*pMethod)();

	// separador
	cout << endl;
	
	// podemos chamar o mesmo m�todo para diversos objetos
	cout << "this: " << hex << &fuzzyObject1 << ", member: " << dec << fuzzyObject1.*pMember << endl;
	cout << "this: " << hex << &fuzzyObject2 << ", member: " << dec << fuzzyObject2.*pMember << endl;
	cout << "this: " << hex << &fuzzyObject3 << ", member: " << dec << fuzzyObject3.*pMember << endl;
}// Codigo-fonte disponivel no GitHub (Caloni/Caloni.com.br) 
