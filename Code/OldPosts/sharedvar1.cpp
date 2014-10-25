// aqui definimos uma nova se��o (note o 'shared' usado como atributo)
#pragma section("shared", read, write, shared)

// um conjunto de vari�veis agrupadas para facilitar o compartilhamento
struct EstruturaDoCoracao
{
	int meuIntPreferido;
	char meuCharAmigo;
	double meuNumeroDePontoFlutuanteCamarada;
};

// uma inst�ncia da struct acima para podermos usar nos processo amigos
__declspec(allocate("shared")) EstruturaDoCoracao g_coracao;

int main()
{
	g_coracao.meuCharAmigo = 'C';
	g_coracao.meuIntPreferido = 42;
	g_coracao.meuNumeroDePontoFlutuanteCamarada = 3.14159265358979323846264338;
} 
