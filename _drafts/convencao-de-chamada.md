void func()
{

}
?
int main()
{
   func("sbrubles");
   return 0;
}?

caso vc ponha void func(void) j� n�o funciona. :P

Por que C � zoado :P

N�o existem muitas regras de sintaxe a serem seguidas na linguagem. Antigamente se n�o colocasse nenhum tipo de retorno era como se fosse int. Da mesma forma, se n�o colocar par�metros vale tudo. � como se fossem os tr�s pontinhos do printf. Afinal, voc� n�o ia querer ficar repetindo os par�metros no .c   e no .h, n�o � mesmo :D

Tinha tamb�m a declara��o arcaica:

void func()
char* sbrubles;
{
}

Ent�o, supostamente um va_args pra ler "alguma coisa" certamente leria os par�metros. Achei engra�ado que isso compilou.


Sabe se este par�metro s� fica inutilizado ou chega a dar algum problema mais s�rio?

Sim, sua suposi��o a respeito do va_args faz todo sentido.

O padr�o de chamada da linguagem (lembra disso?) � cdecl. Isso quer dizer que o chamador � que "limpa a sujeira" depois da chamada. Isso � o que permite o milagre do printf receber n argumentos.

S� vai dar problema se definir outro padr�o de chamada ou se a fun��o chamada mexer no que n�o devia (se esperar outros tipos ou n�mero de argumentos, por exemplo).

