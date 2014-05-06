#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>



//
// Captura senha com no maximo n caracteres.
// No lugar do que o usuario digita, caracteres msk serao colocados.
// Se msk for igual a zero, o echo serah desabilitado.
//
int getPassword(char *pwd, int n, char msk = '*')
{
   if( n <= 0 ) return -1;
   
   int c;
   char *p = pwd;
   *p = 0;
   
   while( (c = getch()) != '\r' )
   {
      // o usuario digitou uma tecla estendida
      if( c == 0xE0 || c == 0x00 )
      {
         getch();
         continue;
      }
      else
      // temos mais de um caractere e usuario pressiona backspace
      if( p - pwd && c == '\b' )
      {
         if( msk )
            printf("\b \b");
         *--p = 0;
      }
      else
      // nao atingimos o limite e o caractere eh imprimivel
      if( p - pwd < n - 1 && isprint(c) )
      {
         if( msk )
            putchar(msk);
         *p++ = c;
         *p = 0;
      }
   }
   
   putchar('\n');
   return int(p - pwd);
}



//
// E Deus disse: "int main!"
//
int main()
{
   char pwd[500];
   printf("Digite sua senha:");
   getPassword(pwd, sizeof(pwd));
   printf("A senha digitada foi: %s\n", pwd);
   system("pause");
}
