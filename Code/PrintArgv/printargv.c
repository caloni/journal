#include "printargv.h"
#include <stdio.h>

int printargv(FILE *fout, int argc, char *argv[])
{
   int i = 0;
   
   while( i++ < argc )
      fprintf(fout, "%d: %s\n", i, argv[i - 1]);
      
   return argc;
}
