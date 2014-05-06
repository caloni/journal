#pragma warning(disable:4996)
#include<stdio.h>
#include<algorithm>
#include<vector>
#include<time.h>
#include<iostream>

using namespace std;


int w[1001], C[1001][1001], O[1001];


// Note que uma delas (C) é uma tabela gigantesca:

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
	// Excel com 3 milhões de iterações
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

	for( i = 0; i < 1001; ++i )
	{
		for( j = 0; j < 1001; ++j )
		{
			printf("%d;", C[j][i]);
		}
		printf("\n");
	}
}
