#include <iostream>
#include <string>

#include <stdio.h>
#include <string.h>



/**
 * Definicao de constantes.
 */
const char REBUILDER_PROGRAM_VERSION[] = "0.01 alpha";



/**
 * Codigos de erro do programa.
 */
enum NRBErr {
   REBUILDER_ERR_SUCCESS =          0,       // Sucesso.
   REBUILDER_ERR_NOT_SUCCESS =     -1,       // Nao sucesso.
   REBUILDER_ERR_SYNTAX =          -2,       // Erro de sintaxe.
};



/**
 * Imprime ajuda do programa na saida.
 */
void PrintHelp(char *szProgName)
{
   char *p = strrchr(szProgName, '\\');
   printf("ReBuilder - Resource Reader/Converter of RCs to help Wanderley\n"
          "Version %s\n"
          "Use mode: %s -operation [-params] file(s)\n\n"
          "Operations\n"
          "-c : Get strings from specified DFMs\n\n",
          REBUILDER_PROGRAM_VERSION,
          p ? p + 1 : szProgName
          );
}



/**
 * Le strings do arquivo de recursos especificado e escreve no 
 * arquivo de recursos destino.
 */
NRBErr ReadResStrings(char *szFileName)
{
   FILE *fIn = fopen(szFileName, "r");

   if( fIn )
   {
      while( char *p = fgets(szLine, sizeof(szLine), fIn) )
      {
         
      }

      fclose(fIn);
   }
   
   return REBUILDER_ERR_SUCCESS;
}



/**
 * E no principio Deus disse: "int main!"
 */
int main(int argc, char *argv[])
{
   NRBErr iRet = REBUILDER_ERR_NOT_SUCCESS;

   // Se sintaxe nao estiver correta, exibir ajuda e sair.
   if( argc < 3 || argv[1][0] != '-' ) {
      PrintHelp(argv[0]);
      return iRet = REBUILDER_ERR_SYNTAX;
   }

   // Identifica e realiza operacao especificada pelo usuario.
   switch( argv[1][1] )
   {
      case 'c':
         for( int i = 2; argv[i]; ++i )
            if( (iRet = ReadResStrings(argv[i])) != REBUILDER_ERR_SUCCESS ) 
               break;
         break;

      default:
         PrintHelp(argv[0]);
         printf("Sorry, I don't know what to do.\n");
         break;
   }

   return (int)iRet;
}
