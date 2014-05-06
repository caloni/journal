/**
 * Sample program for code execution in the another process context.
 *
 * @date 05/2004.
 * @author Wanderley Caloni Jr. <blakhalk@ig.com.br>
 */

#include <tchar.h>

#include <rmthread.h>

/* http://www.codeproject.com/cpp/xgetopt.asp */
#include <xgetopt.h>

#include <stdio.h>



/**
 * Get actual version.
 *
 * @todo Get the version from the resource file.
 */
LPCTSTR GetVersionInfo()
{
   static LPCTSTR tzVInfo = _T("1.0.0.1");
   return tzVInfo;
}



/**
 * Print the program help in the screen.
 */
VOID PrintHelp(
   LPCTSTR lpProgramName
)
{
   LPCTSTR ptzSName = _tcsrchr(lpProgramName, _T('\\'));
   LPCTSTR tzHelp =
   
   _T("RmThread v. %s ")
   _T("Sample by Wanderley Caloni Jr. <blakhalk@ig.com.br>\n")
   _T("How to use: %s [-options]\n")
   _T("  -pxxx   Program Path (default: %%ComSpec%%)\n")
   _T("  -dxxx   DLL File Name (default: RmThread.dll)\n")
   _T("  -axxx   Program Arguments (default: \"\")\n")
   ;

   _tprintf(tzHelp, GetVersionInfo(), ptzSName ? ptzSName + 1 : lpProgramName);
}



/**
 * Print the dwErr description.
 */
VOID PrintError(
   DWORD dwErr
)
{
   LPVOID lpBuf;

   if( FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                      FORMAT_MESSAGE_IGNORE_INSERTS | 
                      FORMAT_MESSAGE_FROM_SYSTEM, 
                     NULL, 
                     dwErr, 
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                     (LPVOID) &lpBuf, 
                     0, 
                     NULL) )
   {
      _ftprintf(stderr, _T("Error %d. %s"), (int)dwErr, lpBuf);
      LocalFree(lpBuf);
   }
}



/**
 * In the beginning, God said: "int main!!"
 */
int _tmain(int argc, TCHAR *argv[])
{
   int iRet = 0;
   TCHAR tzProgPath[MAX_PATH] = _T("%ComSpec%");
   TCHAR tzProgArgs[MAX_PATH] = _T("");
   TCHAR tzDllPath[MAX_PATH] = _T("RmThread.dll");
   int opt;


   // Parameters capture
   while( (opt = getopt(argc, argv, _T("p:d:a:"))) != EOF )
   {
      switch( opt )
      {
         case _T('p'): _tcscpy(tzProgPath, optarg); break;
         case _T('a'): _tcscpy(tzProgArgs, optarg); break;
         case _T('d'): _tcscpy(tzDllPath, optarg);  break;
         case _T('\0'): break;
         case _T('?'):
         default: PrintHelp(argv[0]); return iRet;
      }
   }


   //
   // Main routine.
   //
   {
      HANDLE hProc;

      // Start process and get handle with powers.
      hProc = CreateAndGetProcessGodHandle(tzProgPath, tzProgArgs);

      if( hProc != NULL )
      {
         // Load DLL in the create process context.
         HMODULE hDll = RemoteLoadLibrary(hProc, tzDllPath);

         if( hDll != NULL )
            RemoteFreeLibrary(hProc, hDll);
         else
            _ftprintf(stderr, _T("Error trying to load %s in process %s"), 
                      tzDllPath, tzProgPath);

         CloseHandle(hProc);
      }
      else iRet = (int)GetLastError();
   }

   if( iRet != ERROR_SUCCESS )
      PrintError(iRet);

	return iRet;
}
