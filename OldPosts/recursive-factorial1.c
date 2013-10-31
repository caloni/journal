int factorial(int n)
{
	if (n > 1)
		return factorial(n - 1) * n;
	else
		return 1;
}

int main()
{
	return factorial(1000);
}
// Codigo-fonte disponivel no GitHub (Caloni/Caloni.com.br) 
