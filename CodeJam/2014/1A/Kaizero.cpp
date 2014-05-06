/** @author Kaizero
@desc Vers�o comentada (em portugu�s) e desofuscada do c�digo do 
Code Jam 2014, 1A, problema 3 (Proper Shuffle)
por Wanderley Caloni (wanderley@caloni.com.br).
*/
#pragma warning(disable:4996) // warning, pra que te quero...
#include<stdio.h>
#include<algorithm>
#include<vector>
#include<time.h>

using namespace std;


// as vari�veis monossil�bicas...
int w[1001], C[1001][1001], O[1001];


// Note que uma delas (C) � uma tabela gigantesca:

// 1     2    3     4   ...  1001
// 2
// 3
// ...
// 1001

// tabela verdade?
bool v[1001];


struct A
{	
	int ord, R;

	bool operator <(const A &p)const
	{
		return R < p.R;
	}
}
p[1000];


int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int i, TC, T, n, j;

	srand((unsigned)time(NULL)); // mexendo o saco de bingo...

	// a parte mais demorada: construir um contador gigante estilo 
	// Excel com 3 milh�es de itera��es
	for (i = 0; i < 3000000; i++)
	{
		// 1. Preenchemos o array sequencial.
		for (j = 0; j < 1000; j++)
		{
			w[j] = j;
		}

		// 2. Realizamos o algorimo ruim.
		for (j = 0; j < 1000; j++)
		{
			swap(w[j], w[rand() % 1000]);
		}

		// 3. Pesamos o resultado do algoritmo ruim.
		for (j = 0; j < 1000; j++)
		{
			C[j][w[j]]++;
		}
	}

	// agora a parte "f�cil"...

	// ler n�mero de casos de teste (sempre 120)
	scanf("%d", &TC);
	for (T = 1; T <= TC; T++) // iterar por cada linha
	{
		scanf("%d", &n);
		p[T].ord = T; // guardando sua posi��o

		// lendo os n�meros de todos os casos
		for (i = 0; i < n; i++)
		{
			scanf("%d", &O[i]);
			p[T].R += C[i][O[i]]; // mas gravando o peso de cada posi��o (c�lculo de 3M)
		}
	}

	// ordenando pelo peso de cada posi��o
	sort(p + 1, p + TC + 1);
	for (i = 1; i <= 60; i++)
		v[p[i].ord] = true; // chegamos ao valor emp�rico que os primeiros
		// 60 valores menores s�o bons algoritmos (note: EMP�RICO, foram feitos testes pr�vios)

	for (i = 1; i <= TC; i++)
	{
		printf("Case #%d: ", i);
		if (v[i])printf("GOOD\n");
		else printf("BAD\n");
	}
}