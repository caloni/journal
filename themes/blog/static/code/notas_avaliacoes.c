/*
  Escreva um programa que leia as notas das duas avaliações normais e a nota da avaliação
  optativa. Caso o aluno não tenha feito a optativa deve ser fornecido o valor –1. Calcular a
  média do semestre considerando que a prova optativa substitui a nota mais baixa entre as
  duas primeiras avaliações. Escrever a média e mensagens que indiquem se o aluno foi
  aprovado, reprovado ou está em exame, de acordo com as informações abaixo:

  Aprovado : media >= 6.0
  Reprovado: media < 3.0
  Exame : media >= 3.0 e < 6.
*/

#include <stdio.h>

//#define m = media

int main()
{
  float media, n1, n2, n3;
  printf("digite as notas\n");
  scanf("%f %f %f", &n1, &n2, &n3);

  if (n1<n2 && n1<n3)
  {
    n1 =n3;  
  }
  else if (n2<n1 && n2<n3 )
  {
    n2=n3;
  }

  media = (n2+n1)/2;

  //Aprovado : media >= 6.0
  if( media >= 6.0 )
  {
    printf ("\n\naprovado.\n"); 
  }
  //Reprovado: media < 3.0
  else if( media < 3.0 )
  {
    printf ("\n\nreprovado.\n");
  }
  //Exame : media >= 3.0 e < 6.
  else
  {
    printf ("\n\nfinal.\n");
  }
}
