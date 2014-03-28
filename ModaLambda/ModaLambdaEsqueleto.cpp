#include <algorithm>
#include <iostream>

int main()
{
	int arrayDeInts[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	std::for_each( // estou chamando uma função aqui
		&arrayDeInts[0], &arrayDeInts[10], // começo e final (begin e end para STL)
		[&] // opa! sintaxe nova. os [] dizem que começa uma função lambida e 
			//o & diz que posso acessar todo mundo

		(int umInt) // depois dos []s vem o formato de recebimento de parâmetros de uma função
			// (no caso, como é um array de ints, o parâmetro para o for_each tem que ser um int)

		{ // iniciamos a função dentro da chamada do for_each

			std::cout << umInt << ' '; // podemos acessar umInt, arrayDeInts 
			// e qualquer variável da função ou objeto (se fosse um objeto)

		} // finalizamos a função dentro da chamada do for_each

	); // e precisamos fechar a chamada do for_each depois dessa suruba toda

}