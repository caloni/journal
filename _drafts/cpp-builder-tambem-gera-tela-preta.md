---
title: C++ Builder também gera tela preta
---

Há muuuuuuito tempo atrás, quando meu blog era uma pequena mancha vermelha com pontinhos pretos na blogosfera (okay, continua sendo uma mancha vermelha com pontinhos) eu escrevia artigos sobre o C++ Builder, a evolução da "Emê Éfe Cê" e a inspiração do .NERD. Bom, esse post volta timidamente o tema "RADs que não dependem de framework".

Hoje vamos esquecer um pouco esse mundo cheio de botões, cursores, animações 3D e efeitos sonoros e programar um pouco no velho, simples, eficaz e funcional console em modo texto. Para a talvez surpresa do programador C++ Builder, saiba que ele também suporta a criação de projetos com essas telas chatas sem eventos OnMouseDown e sem 4294967296 de cores. Basta ir na opção "File, New, Other..., Console Wizard" e pronto! Tudo o que você sempre quis: cin e cout.



//---------------------------------------------------------------------------
 
#pragma hdrstop
 
//---------------------------------------------------------------------------
 
#pragma argsused
int main(int argc, char* argv[])
{
   return 0;
}

//---------------------------------------------------------------------------
Bom, para não dizer que esse artigo é muito chato, vamos fazer um programa que conta o número de palavras da entrada padrão que começam com a letra 'C':

#include <iostream>
#include <string>
 
#pragma hdrstop
#pragma argsused

 
using namespace std;
 
int main(int argc, char* argv[])
{
   int ret = 0;
   string word;

 
   while( cin >> word )
   {
      if( word[0] == 'C' || word[0] == 'c' )
         ret++;
   }
 
   cout << ret << " words starting with the letter \'C\'." << endl;

 
   return ret;
}
Agora vamos testar:

Entrada
=======
 Caçador abatido

 Dois caçadores caminham na floresta quando um deles, subitamente, cai no 
chão com os olhos revirados. Não parece estar respirando.
 O outro caçador pega o celular, liga para o serviço de emergência e diz: 
"Meu amigo morreu! O que eu faço?". Com voz pausada, o atendente explica: 
"Mantenha a calma. A primeira coisa a fazer é ter certeza de que ele está morto".
 Vem um silêncio. Logo depois, se ouve um tiro.
 A voz do caçador volta à linha. Ele diz: "OK. E agora?".
 
 de http://pt.wikipedia.org/wiki/Piada

Saída
=====
 13 words starting with the letter 'C'.
Muitas vezes, quando é necessário fazer um teste simples ou até mesmo um aplicativo de teste que recebe comandos em modo texto, o console pode ser útil, e até mais flexível que uma interface com os controles já definidos. Portanto, é bom saber que o seu ambiente permite a criação desse tipo de programa. Nunca se sabe quando pode precisar de uma tela preta.

Exercícios:

Fazer um programa que conta o número de palavras que começam com vogal.
Fazer um programa que conta as palavras e ordena-as por letra, imprimindo o resultado.
