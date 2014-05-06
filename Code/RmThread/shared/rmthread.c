/**
 * Code execution in another process context.
 *
 * @date 05/2004.
 * @author Wanderley Caloni Jr. <blakhalk@ig.com.br>
 */
#include "rmthread.h"

#include <strsafe.h>

#include <stdlib.h>
#include <stdio.h>



/**
 * Run CreateProcess with specified parameters and get handle that allows
 * to allocate memory and to run threads for this process.
 */
HANDLE CreateAndGetProcessGodHandle(
   LPCTSTR lpApplicationName, 
   LPTSTR lpCommandLine
)
{
   HANDLE hRet = NULL;
   STARTUPINFO si = { sizeof(si) };
   PROCESS_INFORMATION pi;
   TCHAR tzApplicationName[MAX_PATH] = _T("");
   BOOL bRes;
   
   // If string are empty, invalidate pointer and use the lpCommandLine.
   if( lpApplicationName && !*lpApplicationName )
      lpApplicationName = NULL;
   else
      ExpandEnvironmentStrings(lpApplicationName, 
                               tzApplicationName, 
                               sizeof(tzApplicationName));

   bRes = 
   CreateProcess(*tzApplicationName ? tzApplicationName : lpApplicationName,
                 lpCommandLine,
                 NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, 
                 &si, &pi);

   if( bRes != FALSE )
   {
      hRet = OpenProcess(PROCESS_CREATE_THREAD | 
                          PROCESS_QUERY_INFORMATION |
                          PROCESS_VM_OPERATION |
                           PROCESS_VM_WRITE |
                          PROCESS_VM_READ,
                         TRUE,
                         pi.dwProcessId);

      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
   }
   
   return hRet;
}



/**
 * Load DLL in another process.
 */
HMODULE RemoteLoadLibrary(
   HANDLE hProcess, 
   LPCTSTR lpFileName
)
{
   LPCSTR tzLoadLibrary =
   #ifdef UNICODE
   "LoadLibraryW"
   #else
   "LoadLibraryA"
   #endif
   ;

   HMODULE hRet = NULL;
   TCHAR tzFileName[MAX_PATH] = { 0 };
   LPVOID lpCodeMem;
   SIZE_T stCodeMem;
   
   // Make a safe copy of the module path to be opened.
   StringCbCopy(tzFileName, sizeof(tzFileName), lpFileName);

   // Allocate memory in the another process.
   stCodeMem = ( _tcslen(tzFileName) + 1 ) * sizeof(TCHAR);
   lpCodeMem = VirtualAllocEx(hProcess, 
                              NULL,
                              stCodeMem,
                              MEM_COMMIT,
                              PAGE_EXECUTE_READWRITE);

   if( lpCodeMem != NULL )
   {
      // Write tzFileName in the allocated memory in the another process.
      if( WriteProcessMemory(hProcess, 
                              lpCodeMem, 
                              tzFileName,
                              stCodeMem,
                              &stCodeMem) )
      {
         HANDLE hThr;
         DWORD dwThrId;
         FARPROC fpLoadLibrary;
         
         fpLoadLibrary =
         GetProcAddress(GetModuleHandle(_T("Kernel32")), tzLoadLibrary);

         // Create remote thread that loads the tzFileName module.
         hThr =
         CreateRemoteThread(hProcess,
                              NULL,
                              0,
                              (LPTHREAD_START_ROUTINE) fpLoadLibrary,
                              (LPVOID) lpCodeMem,
                              0,
                              &dwThrId);

         if( hThr != NULL )
         {
            // Get address where the module was loaded.
            WaitForSingleObject(hThr, INFINITE);
            GetExitCodeThread(hThr, (LPDWORD)&hRet);
            CloseHandle(hThr);
         }
      }

      VirtualFreeEx(hProcess, lpCodeMem, 0, MEM_RELEASE);
   }
   
   return hRet;
}



/**
 * Free DLL in another process.
 */
BOOL RemoteFreeLibrary(
   HANDLE hProcess,
   HMODULE hModule
)
{
   BOOL bRet;
   HANDLE hThr;
   DWORD dwThrId;
   FARPROC fpFreeLibrary;

   fpFreeLibrary =
   GetProcAddress(GetModuleHandle(_T("Kernel32")), "FreeLibrary");

   // Create remote thread that free hModule.
   hThr =
   CreateRemoteThread(hProcess,
                      NULL,
                      0,
                      (LPTHREAD_START_ROUTINE) fpFreeLibrary,
                      (LPVOID) hModule,
                      0,
                      &dwThrId);

   // Get FreeLibrary call return value.
   if( hThr != NULL )
   {
      WaitForSingleObject(hThr, INFINITE);
      GetExitCodeThread(hThr, (LPDWORD)&bRet);
      CloseHandle(hThr);
   }

   return bRet;
}
