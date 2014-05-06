// removedir.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>



/*
 * Marca pasta szPasta para exclusao apos o reboot e retorna o numero de 
 * pastas remanescentes.
 */
int RemoveDirectory(const char *szInstallPath, const char *szPasta)
{
   int iRet = 0;
   WIN32_FIND_DATA findFileData;
   HANDLE hFind;

   SetCurrentDirectory(szInstallPath);
   
   hFind = FindFirstFile("*", &findFileData);
   
   if( hFind != INVALID_HANDLE_VALUE )
   {
      do
      {
         if( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            if( findFileData.cFileName[0] != '.' )
               if( stricmp(findFileData.cFileName, szPasta) )
                  iRet++;
               else
               {
                  char szDelPath[MAX_PATH];
                  
                  sprintf(szDelPath, "%s\\%s", szInstallPath, findFileData.cFileName);
                  if( RemoveDirectory(szDelPath) == FALSE )
                     MoveFileEx(szDelPath, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
               }
      }
      while( FindNextFile(hFind, &findFileData) == TRUE );
      
      FindClose(hFind);
   }
   
   return iRet;
}



int _tmain(int argc, _TCHAR* argv[])
{
   if( argc > 2 )
   {
      //TODO: Ler do registro o path de instalacao.
      char szInstallPath[MAX_PATH];
      char *p;
      
      strcpy(szInstallPath, argv[1]);
      
      while( p = strrchr(szInstallPath, '\\') )
      {
         *p++ = 0;
         if( RemoveDirectory(szInstallPath, p) )
            break;
      }
   }
   else 
   {
      if( char *p = strrchr(argv[0], '\\') )
      {
         *p++ = 0;
         printf("Apaga todas as pastas do DiretorioRaiz.\n");
         printf("Modo de usar: %s (DiretorioRaiz)\n", p);
      }
   }

   return 0;
}
