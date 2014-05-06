/**
 * Exemplo de utilizacao de OOnC.
 *
 * Este projeto exemplifica como utilizar o suporte a orientacao a objetos
 * em linguagem C por intermedio da estrutura OOnC.
 *
 * @author Wanderley Caloni Jr. <wanderley.caloni@ig.com.br>
 * @date 04.2004
 */
#include "CSquare.h"
#include "CCircle.h"
#include "CTriangle.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>



/**
 * Imprime ajuda do programa.
 */
void PrintHelp()
{
   printf("Como usar: OOnC forma1 [forma2] [forma3] [...]\n\n"
          "Onde forma pode ser:\n\n"
          "Square: Um quadrado\n"
          "Circle: Um circulo\n"
          "Triangle: Um triangulo\n");
}



/**
 * Cria um array dinamico de ponteiros para CShape com argc elementos,
 * cada um deles contendo uma instancia do tipo especificado.
 */
int GetShapes(CShape **cShapes, int argc, char *argv[])
{
   int i = 0;

   while( i < argc )
   {
      if( !stricmp("square", argv[i]) )
         cShapes[i] = &New_CSquare()->m_upper;
      else
      if( !stricmp("circle", argv[i]) )
         cShapes[i] = &New_CCircle()->m_upper;
      else
      if( !stricmp("triangle", argv[i]) )
         cShapes[i] = &New_CTriangle()->m_upper;
      else
         cShapes[i] = New_CShape();
         
      ++i;
      Sleep(1500); // Para alterar tempo de criacao.
   }
   
   return i;
}



/**
 * E no principio Deus disse: 'int main!'
 */
int main(int argc, char *argv[])
{
   if( argc > 1 )
   {
      CShape **cShapes = (CShape**) malloc((argc - 1) * sizeof(CShape*));

      if( cShapes )
      {
         // Obtem as formas especificadas como argumentos.
         int i = 0, j;

         printf("aguarde...");
         j = GetShapes(cShapes, argc - 1, &argv[1]);
         printf("\r             \r");

         while( i < j )
         {
            cShapes[i]->m_Draw(cShapes[i]);
            
            printf("Tempo de criacao: ");
            cShapes[i]->m_upper.m_PrintCreationTime(&cShapes[i]->m_upper);

            printf("Numero de lados: %d\n", 
               cShapes[i]->m_GetNumberOfSides(cShapes[i]));

            cShapes[i]->m_upper.m_delete(cShapes[i]->m_upper.m_this);
            ++i;
         }
      }
   }
   else PrintHelp();

   system("pause");
   return 0;
}
