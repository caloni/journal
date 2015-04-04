void func()
{

}
?
int main()
{
   func("sbrubles");
   return 0;
}?

caso vc ponha void func(void) já não funciona. :P

Por que C é zoado :P

Não existem muitas regras de sintaxe a serem seguidas na linguagem. Antigamente se não colocasse nenhum tipo de retorno era como se fosse int. Da mesma forma, se não colocar parâmetros vale tudo. É como se fossem os três pontinhos do printf. Afinal, você não ia querer ficar repetindo os parâmetros no .c   e no .h, não é mesmo :D

Tinha também a declaração arcaica:

void func()
char* sbrubles;
{
}

Então, supostamente um va_args pra ler "alguma coisa" certamente leria os parâmetros. Achei engraçado que isso compilou.


Sabe se este parâmetro só fica inutilizado ou chega a dar algum problema mais sério?

Sim, sua suposição a respeito do va_args faz todo sentido.

O padrão de chamada da linguagem (lembra disso?) é cdecl. Isso quer dizer que o chamador é que "limpa a sujeira" depois da chamada. Isso é o que permite o milagre do printf receber n argumentos.

Só vai dar problema se definir outro padrão de chamada ou se a função chamada mexer no que não devia (se esperar outros tipos ou número de argumentos, por exemplo).

