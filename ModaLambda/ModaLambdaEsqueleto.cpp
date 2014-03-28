#include <algorithm>
#include <iostream>

int main()
{
	int arrayDeInts[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	std::for_each( // estou chamando uma fun��o aqui
		&arrayDeInts[0], &arrayDeInts[10], // come�o e final (begin e end para STL)
		[&] // opa! sintaxe nova. os [] dizem que come�a uma fun��o lambida e 
			//o & diz que posso acessar todo mundo

		(int umInt) // depois dos []s vem o formato de recebimento de par�metros de uma fun��o
			// (no caso, como � um array de ints, o par�metro para o for_each tem que ser um int)

		{ // iniciamos a fun��o dentro da chamada do for_each

			std::cout << umInt << ' '; // podemos acessar umInt, arrayDeInts 
			// e qualquer vari�vel da fun��o ou objeto (se fosse um objeto)

		} // finalizamos a fun��o dentro da chamada do for_each

	); // e precisamos fechar a chamada do for_each depois dessa suruba toda

}