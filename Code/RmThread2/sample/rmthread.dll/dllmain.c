/** @file
 * Sample DLL for the RmThread project.
 *
 * @date 05/2004
 * @author Wanderley Caloni Jr. <wanderley@caloni.com.br>
 */

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <tchar.h>
#include <stdio.h>



/** Module name. */
TCHAR g_tzModuleName[] = _T("RmThread.dll");

/**
 * Handle for the thread that calls DllMain with ul_reason_for_call 
 * equals to DLL_PROCESS_ATTACH. Closed when DllMain is called with 
 * ul_reason_for_call equals to DLL_PROCESS_DETACH.
 */
HANDLE g_hThrDllMain = INVALID_HANDLE_VALUE;



/**
 * Sample function, called remotely for RmThread.exe.
 */
DWORD WINAPI RmThread(LPVOID lpParameter)
{
   HMODULE hDll = (HMODULE) lpParameter;
   LPCTSTR ptzMsg = 
           _T("Congratulations! You called RmThread.dll successfully!");

   // Wait DllMain termination.
   WaitForSingleObject(g_hThrDllMain, INFINITE);

   //TODO: Put your remote code here.
   MessageBox(NULL,
              ptzMsg,
              g_tzModuleName,
              MB_OK | MB_ICONINFORMATION);
   
   // Do what the function name says.
   FreeLibraryAndExitThread(hDll, 0);
}



/**
 * In the beginning, uncle Bill said: "DllMain!!"
 */
BOOL APIENTRY DllMain(
   HANDLE hModule,
   DWORD ul_reason_for_call,
   LPVOID lpReserved
)
{
   switch( ul_reason_for_call )
   {
      case DLL_PROCESS_ATTACH:
      {
         DWORD dwThrId;

         // Fill global variable with handle copy of this thread.
         BOOL bRes =
         DuplicateHandle(GetCurrentProcess(),
                         GetCurrentThread(),
                         GetCurrentProcess(),
                         &g_hThrDllMain,
                         0, 
                         FALSE, 
                         0);
         if( bRes == FALSE ) break;

         // Call function that do the useful stuff with its DLL handle.
         CloseHandle(CreateThread(NULL, 
                                  0,
                                  RmThread,
                                  (LPVOID) LoadLibrary(g_tzModuleName),
                                  0, 
                                  &dwThrId));
      }
      break;

      case DLL_THREAD_ATTACH: 
      break;

      case DLL_THREAD_DETACH:
      break;

      case DLL_PROCESS_DETACH:
      {
         // Close DllMain thread handle called in DLL_PROCESS_ATTACH event.
         CloseHandle(g_hThrDllMain);
      }
      break;
   }

   return TRUE;
}
