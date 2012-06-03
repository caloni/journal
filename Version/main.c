/*
wversion is an automatic version incrementor, with a *very* rudimentar parser.
Copyright (C) 2004  William Cesar de Oliveira.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

You could also to contact the author by E-mail: mailto:w99 AT terra.com.br
*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>

#define MAX_SUBSECTIONS 4

const char* engMonths[] = 
{
      "JAN",
      "FEB",
      "MAR",
      "APR",
      "MAY",
      "JUN",
      "JUL",
      "AUG",
      "SEP",
      "OCT",
      "NOV",
      "DEC"
};

const char* ptMonths[] = 
{
      "JAN",
      "FEV",
      "MAR",
      "ABR",
      "MAI",
      "JUN",
      "JUL",
      "AGO",
      "SET",
      "OUT",
      "NOV",
      "DEZ"
};

const char** theMonths = ptMonths;



///////////////////////////////////////////////////////////////////////////////
typedef struct T_CurrentVersion
{
   char prefix[256];
   char postfix[256];
   unsigned subsections;
   unsigned v[MAX_SUBSECTIONS];
} TCurrentVersion;

///////////////////////////////////////////////////////////////////////////////
BOOL haveCurrentVersion = FALSE;
BOOL useHexa = FALSE;
TCurrentVersion CurrentVersion = {"V","",0x04,0x01,0x00,0x00,0x00};

///////////////////////////////////////////////////////////////////////////////
int hextoint(const char *str,unsigned long *ival)
{
	register unsigned long u;
	register const char *cp;

	cp = str;

	if (*cp == '\0')
	    return 0;

	u = 0;
	while (*cp != '\0') 
  {
		if (!isxdigit((int)*cp))
		    return 0;
		if (u >= 0x10000000)
		    return 0;	//overflow
		u <<= 4;
		if (*cp <= '9')		//very ascii dependent
		    u += *cp++ - '0';
		else if (*cp >= 'a')
		    u += *cp++ - 'a' + 10;
		else
		    u += *cp++ - 'A' + 10;
	}
	*ival = u;
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
void Usage(void)
{
  printf("wversion - utilitario para controle automatico de versao\n");
  printf("  modo de uso: version [-c CONTROLFILE] [-o OUTPUTFILE] [-v INITIALVERSION] [-p PREFIX]\n");
  printf("  -i INPUTFILE Nome do arquivo de entrada - default version.h\n");
  printf("  -o OUTPUTFILE Nome do arquivo gerado - default version.h\n");
  printf("  -s STARTVERSION Numero de inicializacao - default (se não houver arquivo de entrada) 1.0-0\n");
  printf("  -p PREFIX prefixo para a string de versão - default \"v\"\n");
  printf("  -t POSTFIX prefixo para a string de versão - default vazio\n");
}

BOOL ParseVersionString(char * parse, TCurrentVersion *currentVersion)
{
   BOOL prefixFound = FALSE;
   char* lastSep;
   char* firstSep;
   char* firstPosfix;
   char* lastPrefix;
   char* s;
   
   //vou analisar a string para identificar a parte que deve ser incrementada.
   //esta deve ser a última parte da string de versão, mas tenho de desconsiderar
   //o posfixo.
   //1.1.1.2test -> test é o posfixo
   //1.1.1.2 ok  -> " ok" é o posfixo.
   //vou pegar o prefixo
   //entendo como fim do prefixo qualquer caracter não númerico antes do primeiro separador.
   //localizo o primeiro separador
   firstSep = strchr(parse, '.');
   if(!firstSep)
   {
      //se não tem nem o primeiro separador aí é problema...
      return FALSE;
   }

   //-->PREFIX
   //ando para trás para achar o primeiro não numérico
   for(lastPrefix=firstSep-1; lastPrefix >= parse; lastPrefix--)
   {
      if(!isdigit(*lastPrefix))
      {
         prefixFound = TRUE;
         break;
      }
   }
   //se achei
   if(prefixFound)
   {
      memcpy(currentVersion->prefix, parse, lastPrefix - parse + 1);
      currentVersion->prefix[lastPrefix - parse + 1] = 0;
   }
   else
   {
      currentVersion->prefix[0] = 0;
      lastPrefix = NULL;
   }

   //-->POSTFIX
   //para localizar o posfixo preciso primeiro do último separador.
   //ele pode ser '.' ou '-'
   lastSep = strrchr(parse,'-');
   if(!lastSep)
   {
      lastSep = strrchr(parse,'.');
      //se não tem o último separador, nada há para fazer.
      if(!lastSep)
         return FALSE;
   }

   //agora vou para frente até que o caracter não seja um dígito
   for(firstPosfix = lastSep +1; *firstPosfix; firstPosfix++)
   {
      if(!isdigit(*firstPosfix))
      {
         break;
      }
   }

   strncpy(currentVersion->postfix, firstPosfix, sizeof(currentVersion->postfix));

   //agora posso isolar o contador de build
   *firstPosfix = 0;

   //vou aproveitar a parte que interessa da versão (tirando o prefixo e o sufixo)
   //para saber quantas seções estão em uso
   currentVersion->subsections = 0;

   //se tem prefixo, pula o último caracter
   if(lastPrefix)
      lastPrefix++;
   else
      lastPrefix = parse;

   if(s = strtok(lastPrefix,".-"))
   {
      do
      {
         if(currentVersion->subsections >= MAX_SUBSECTIONS)
            break;
         currentVersion->v[currentVersion->subsections] = atoi(s);
         currentVersion->subsections++;
      }
      while(s = strtok(NULL,".-"));
   }


   currentVersion->v[currentVersion->subsections -1]++;
//   printf("prefix=[%s] version=[%d] postfix=[%s]\n",currentVersion->prefix,currentVersion->v[currentVersion->subsections-1],currentVersion->postfix);

   return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL ParseInput(const char *fileName, TCurrentVersion *current)
{
  FILE *f;
  char buffer[1024];

  //verifica se o arquivo existe
  f = fopen(fileName,"r");
  if(!f)
  {
    //nao existe, cria e preenche com o default
    f = fopen(fileName,"wb");
    if(!f)
    {
      printf("Nao foi possivel criar o Arquivo %s \n",fileName);
      return FALSE;
    }
    fwrite(&CurrentVersion,sizeof(CurrentVersion),1,f);
  }

  //se abriu, fecha
  if(f)
    fclose(f);

  //e abre de novo, se for o caso
  f = fopen(fileName,"r");
  if(!f)
  {
    printf("Erro ao abrir o arquivo %s\n",fileName);
    return FALSE;
  }


  fseek(f,0,SEEK_SET);

  while(1)
  {
    char *parse;

    if(!fgets(buffer, sizeof(buffer), f))
       break;

    //se a linha esta comentada, ignoro
    if(!strncmp("//",buffer,2))
       continue;

    //verifica se a linha tem um #define no inicio
    //não permite # define como no definição formal do parser pre-compilador.
    parse = strtok(buffer, " \t\r");
    if(parse && !strcmp("#define",parse))
    {
       parse = strtok(NULL, " \t\r");
       if(parse && !strcmp("__VERSION",parse))
       {
         parse = strtok(NULL, "\"\r\n/");
         ParseVersionString(parse, current);
         break;
       }
       else
          continue;
    }
  }

  fclose(f);

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL GenerateOutput(const char *fileName, TCurrentVersion *current)
{
  FILE *f;
  time_t t;
  struct tm *lt;
  char userName[256];
  DWORD userNameSize = sizeof(userName);
  unsigned i;

  //agora já tenho a versão, vou gerar o arquivo header
  f = fopen(fileName,"wb+");
  if(!f)
  {
     printf("Erro ao abrir arquivo de saida %s \n",fileName); 
     return FALSE;
  }

  //o básico
  time(&t);
  lt = localtime(&t);
  fprintf(f,"///@file Arquivo de Controle de Versao\n");
  fprintf(f,"///@warning Gerado automaticamente pelo wversion (w99@terra.com.br)\n");
  fprintf(f,"///@date %d:%d:%d %d/%d/%d\n",lt->tm_hour,lt->tm_min,lt->tm_sec,lt->tm_year % 100 + 2000,lt->tm_mon +1, lt->tm_mday);
  fprintf(f,"\n\n");

  fprintf(f,"///@brief String da Versao, em decimal\n");
  fprintf(f,"///Para edicao manual altere somente esta string, o formato deve ser:\n");
  fprintf(f,"///PREFIXOV1.V2.V3-V4SUFIXO\n");
  fprintf(f,"///Sendo que PREFIXO e SUFIXO sao opcionais mas, se existirem, nao podem conter '.' ou '-'.\n");

  //EM DECIMAL
  fprintf(f,"#define __VERSION\t\"%s",CurrentVersion.prefix);

  for(i=0; i < CurrentVersion.subsections; i++)
  {
    fprintf(f,"%d",CurrentVersion.v[i]);
    if(i < CurrentVersion.subsections -2)
      fprintf(f,".");
    else
       if(i < CurrentVersion.subsections -1)
         fprintf(f,"-");
  }
  fprintf(f,"%s\"\n",CurrentVersion.postfix);

  //EM DECIMAL (PRODUCT_NAME)
  fprintf(f,"#define __PRODUCT_NAME\t\"%s",CurrentVersion.prefix);

  for(i=0; i < CurrentVersion.subsections - 1; i++)
  {
    fprintf(f,"%d",CurrentVersion.v[i]);
    if(i < CurrentVersion.subsections -2)
      fprintf(f,".");
  }
  fprintf(f,"%s\"\n",CurrentVersion.postfix);

  //EM HEXA
  fprintf(f,"\n///@brief String da Versao, em hexa\n");
  fprintf(f,"#define __VERSION_HEXA\t\"%s",CurrentVersion.prefix);

  for(i=0; i < CurrentVersion.subsections; i++)
  {
    fprintf(f,"%x",CurrentVersion.v[i]);
    if(i < CurrentVersion.subsections -2)
      fprintf(f,".");
    else
       if(i < CurrentVersion.subsections -1)
         fprintf(f,"-");
  }
  fprintf(f,"%s\"\n",CurrentVersion.postfix);

  //SEPARADOS
  fprintf(f,"\n//SEGMENTOS SEPARADOS\n");

  fprintf(f,"#define __VERSION_SEGMENTS_COUNT\t%d\n",CurrentVersion.subsections);
  fprintf(f,"#define __VERSION_SEGMENTS\t{",CurrentVersion.subsections);
  for(i=0; i < CurrentVersion.subsections; i++)
  {
    fprintf(f,"0x%02x",CurrentVersion.v[i]);
    if(i < CurrentVersion.subsections -1)
      fprintf(f,",");
  }
  fprintf(f,"}\n");


  if(CurrentVersion.prefix[0])
     fprintf(f,"#define __VERSION_PREFIX\t\"%s\"\n",CurrentVersion.prefix);

  if(CurrentVersion.postfix[0])
     fprintf(f,"#define __VERSION_POSTFIX\t\"%s\"\n",CurrentVersion.postfix);

  for(i=0; i < CurrentVersion.subsections; i++)
  {
     fprintf(f,"#define __VERSION_%d\t0x%02x\n",i+1,CurrentVersion.v[i]);
  }


  //DATA
  fprintf(f,"\n///@brief Data da geracao do Arquivo\n");
  fprintf(f,"#define __VERSION_DATE \"%02d:%02d:%02d %02d/%s/%d\"\n",
         lt->tm_hour,
         lt->tm_min,
         lt->tm_sec,
         lt->tm_mday,
         theMonths[lt->tm_mon], 
         lt->tm_year % 100 + 2000);


#ifdef _WIN32
  if(GetUserName(userName, &userNameSize))
  {
    fprintf(f,"\n///@brief Usuario Logado\n");
    fprintf(f,"#define __VERSION_USER \"%s\"\n", userName);
  }
#endif
  fprintf(f,"\n\n//EOF %s\n\n\n",fileName);
  fclose(f);
  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL ProcessParams(int argc, char *argv[], char *inputFile, char *outputFile, 
                   TCurrentVersion *currentVersion)
{
  int i;
  char *prefix = "";
  char *postfix = "";

  strcpy(outputFile,"version.h");
  strcpy(inputFile,outputFile);

  for(i=1;i<argc;i++)
  {
    //-v version
    if(!strcmp(argv[i],"-v")|| !strcmp(argv[i],"--version"))
    {
      printf("wversion - incrementador de versao.\n");
      printf("Copyright (C) 2004 William C. Oliveira.\n");
      printf("This is free software; see the source for copying conditions.\n");
      printf("There is NO warranty; not even for MERCHANTABILITY or \n");
      printf("FITNESS FOR A PARTICULAR PURPOSE.\n");
      exit(0);
    }
    //-p - prefixo
    if(!strcmp(argv[i],"-p")|| !strcmp(argv[i],"--prefix"))
    {
      //tem q ter mais um parametro
      if(i+1 < argc)
      {
        prefix = argv[i+1];
        i += 1;
      }
      else
        printf("%s: Falta Parametro PREFIXO\n",argv[i]);
      continue;
    }

    //-t - postfixo
    if(!strcmp(argv[i],"-t")|| !strcmp(argv[i],"--postfix"))
    {
      //tem q ter mais um parametro
      if(i+1 < argc)
      {
        postfix = argv[i+1];
        i += 1;
      }
      else
        printf("%s: Falta Parametro POSTFIXO\n",argv[i]);
      continue;
    }

    //-i INPUTFILE
    if(!strcmp(argv[i],"-i")|| !strcmp(argv[i],"--input"))
    {
      //tem q ter mais um parametro
      if(i+1 < argc)
      {
        strncpy(inputFile,argv[i+1],MAX_PATH);
        i += 1;
      }
      else
        printf("%s: Falta Parametro NOMEDOARQUIVO\n", argv[i]);
      continue;
    }

    //-o - OUTPUTFILE
    if(!strcmp(argv[i],"-o")|| !strcmp(argv[i],"--output"))
    {
      //tem q ter mais um parametro
      if(i+1 < argc)
      {
        strncpy(outputFile,argv[i+1],MAX_PATH);
        i += 1;
      }
      else
        printf("%s: Falta Parametro NOMEDOARQUIVO\n", argv[i]);
      continue;
    }

    //-s - STARTVERSION
    if(!strcmp(argv[i],"-s")|| !strcmp(argv[i],"--start"))
    {
      //tem q ter mais um parametro
      if(i+1 < argc)
      {
        haveCurrentVersion = ParseVersionString(argv[i + 1], currentVersion);
        if(!haveCurrentVersion)
        {
          printf("%s: Parametro VERSAOINICIAL invalido [%s]\n", argv[i],argv[i+1]);
          return FALSE;
        }
        i++;
      }
      else
        printf("%s: Falta Parametro VERSAOINICIAL em hexa\n", argv[i]);
      continue;
    }
   
    printf("Parametro Desconhecido (%s)\n",argv[i]);
    Usage();
    return FALSE;
  }

  strncpy(currentVersion->prefix,prefix,sizeof(currentVersion->prefix));
  strncpy(currentVersion->postfix,postfix,sizeof(currentVersion->postfix));

  currentVersion->prefix[sizeof(currentVersion->prefix) -1] = '\0';
  currentVersion->postfix[sizeof(currentVersion->postfix) -1] = '\0';

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
  char inputFile[MAX_PATH];
  char outputFile[MAX_PATH];

  if(!ProcessParams(argc, argv, inputFile, outputFile, &CurrentVersion))
     return 1;

  if(!ParseInput(inputFile, &CurrentVersion))
     return 1;

  if(!GenerateOutput(outputFile, &CurrentVersion))
     return 1;

  return 0;
}
