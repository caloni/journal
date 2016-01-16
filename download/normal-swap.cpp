/** Troca o valor entre duas vari�veis inteiras. Ou seja, ao final da fun��o
a vari�vel first ir� conter o valor da vari�vel second e vice-versa.
*/
void normalSwap(int &first, int& second)
{
	int third = first;
	first = second;
	second = third; // cont�m o valor de first
}

int main()
{
	int first = 13;
	int second = 42;

	cout << "first: " << first << ", second: " << second << endl;
	normalSwap(first, second);
	cout << "first: " << first << ", second: " << second << endl;
}