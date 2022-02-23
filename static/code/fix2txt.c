#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
  if( argc == 2 )
  {
    FILE* fin = fopen(argv[1], "rb");
    char* dot = strrchr(argv[1], '.');
    FILE* fout;

    strcpy(dot, ".txt");
    fout = fopen(argv[1], "wb");

    if( fin && fout )
    {
      int ch;
      
      while( 1 )
      {
        ch = fgetc(fin);

        if( ch == EOF )
          break;

        if( ch == '\1' || ch == '\2' || ch == '\3' )
          ch = ' ';

        fputc(ch, fout);
      }
    }
  }
}

