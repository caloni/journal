// diskdump.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>



DWORD DiskDump(LPTSTR ptzDisk)
{
   DWORD dwRet;
   HANDLE hFile;

   hFile = CreateFile(ptzDisk, 
                      GENERIC_READ | GENERIC_WRITE, 
                      FILE_SHARE_READ, 
                      NULL, 
                      OPEN_EXISTING, 
                      0, 
                      NULL);

   if( hFile != INVALID_HANDLE_VALUE )
   {
      int i = 0;
      DWORD dwRead, dwWritten;
      BYTE abBuf[512];

      while( ReadFile(hFile, abBuf, sizeof(abBuf), &dwRead, NULL) != FALSE )
      {
         if( SetFilePointer(hFile, 
                - (int) sizeof(abBuf), 
                NULL, 
                FILE_CURRENT) != ERROR_SUCCESS ) 
         {
            break;
         }
         
         if( WriteFile(hFile, 
                abBuf, 
                sizeof(abBuf), 
                &dwWritten, NULL) == FALSE )
         {
            break;
         }

         if( !(++i % 100) )
            printf(".");
      }

      CloseHandle(hFile);
   }
   else 
      dwRet = GetLastError();

   return dwRet;
}

int _tmain(int argc, _TCHAR* argv[])
{
   if( argc == 2 )
      return (int) DiskDump(argv[1]);
   return 0;
}
