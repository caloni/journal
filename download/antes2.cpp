// Papai, o que que � isso?
// Ora, filho, apenas uma defini��o de sin�nimo da struct
// _MINHASTRUCT, cujo nome n�o � usado, para dois nomes
// em mai�sculas, apesar se n�o serem defines, com uma
// nomenclatura de ponteiro que eu nunca vi na vida (obs: 
// papai programa em um sistema n�o-Windows).
//
typedef struct _MINHASTRUCT {
   int x;
   int y;
}
MINHASTRUCT, *LPMINHASTRUCT;

// muitas linhas abaixo...

void func(LPMINHASTRUCT ms)
{
   // o que diabos � um LP, mesmo?
}

int main()
{
   MINHASTRUCT ms;
   func(&ms);
}
