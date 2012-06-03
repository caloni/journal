#include "stdafx.h"
#include "waitjob.h"


using namespace WaitJob;



void PrintHelp()
{
   _tprintf(
      _T("\n")
      _T("WaitJob v.1.0 (by Wanderley Caloni Jr)\n\n")
      _T("Examples:\n")
      _T("waitjob filename.exe\n")
      _T("waitjob \"filename.exe param1 param2 param3\"\n")
      _T("\n")
   );
}



int _tmain_2k(int argc, _TCHAR *argv[])
{
   DWORD dwRet = ERROR_SUCCESS;

   switch( argc )
   {
      case 2:
         dwRet = CreateJob(argv[1]);
         break;
      
      default:
         PrintHelp();
         break;
   }

   return (int) dwRet;
}



int _tmain_9x(int argc, _TCHAR* argv[])
{
   DWORD dwRet = ERROR_SUCCESS;

   switch( argc )
   {
   case 2:
      {
         PROCESS_INFORMATION pi;
      
         dwRet = CreateProc(&pi, argv[1], CREATE_NEW_CONSOLE);

         if( dwRet == ERROR_SUCCESS )
            dwRet = WaitAndReleaseProc(&pi);

         break;
      }
   
   default:
      PrintHelp();
      break;
   }

   return (int) dwRet;
}



int _tmain(int argc, _TCHAR *argv[])
{
   int iRet;
   int iMethod; // 0 - NT, 1 - 9X, 2 - WK.
   DWORD dwVer = GetVersion();

   if( dwVer < 0x80000000 )
   { 
      if( (LOBYTE(LOWORD(dwVer))) >= 5 )
         iMethod = 2;
      else 
         iMethod = 0;
   }
   else
      iMethod = 1;

   WaitJob::Initializate();

   switch( iMethod )
   {
   case 0: 
   case 1:
      iRet = _tmain_9x(argc, argv);
      break;

   case 2: 
      iRet = _tmain_2k(argc, argv);
      break;
   }
   
   WaitJob::Terminate();
   return iRet;
}
