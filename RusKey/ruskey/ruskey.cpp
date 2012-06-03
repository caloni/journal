#include "stdafx.h"
#include "ruskey.h"

HINSTANCE g_hInst;
HOOKPROC g_fpHookProc;
HHOOK g_hHook;



DWORD Initialize()
{
   DWORD dwRet = ERROR_SUCCESS;

   g_hInst = LoadLibrary(_T("ruskey.dll"));

   if( g_hInst )
   {
      g_fpHookProc = (HOOKPROC) GetProcAddress(g_hInst, "HookProc");

      if( !g_fpHookProc )
      {
         dwRet = GetLastError();
         FreeLibrary(g_hInst);
      }
   }
   else
      dwRet = GetLastError();

   return dwRet;
}



DWORD WINAPI InstallAndWait(LPVOID)
{
   DWORD dwRet = ERROR_SUCCESS;
   HANDLE hFinishEvt;

   hFinishEvt = CreateEvent(NULL, FALSE, FALSE, _T("RusKeyFinishEvent"));
   if( !hFinishEvt ) return dwRet = GetLastError();

   g_hHook = SetWindowsHookEx(WH_GETMESSAGE, g_fpHookProc, g_hInst, 0);

   if( g_hHook )
   {
      HANDLE hFile = CreateFile(_T("RusKey.dat"), 
         GENERIC_WRITE, 
         FILE_SHARE_READ, 
         NULL, 
         CREATE_ALWAYS, 
         0, 
         NULL);

      if( hFile != INVALID_HANDLE_VALUE )
      {
         DWORD dwWritten;

         if( WriteFile(hFile, 
            &g_hHook, 
            sizeof(g_hHook), 
            &dwWritten, 
            NULL) != FALSE )
         {
            WaitForSingleObject(hFinishEvt, INFINITE);
         }
         else
            dwRet = GetLastError();

         CloseHandle(hFile);
         DeleteFile(_T("RusKey.dat"));
      }
      
      CloseHandle(hFinishEvt);
      UnhookWindowsHookEx(g_hHook);
   }
   else 
      dwRet = GetLastError();

   return dwRet;
}



DWORD Run()
{
   DWORD dwRet = ERROR_SUCCESS;
   HANDLE hThr;
   DWORD dwThr;

   hThr = CreateThread(NULL, 0, InstallAndWait, NULL, 0, &dwThr);

   if( hThr )
   {
      if( WaitForSingleObject(hThr, INFINITE) == WAIT_OBJECT_0 )
         GetExitCodeThread(hThr, &dwRet);
      CloseHandle(hThr);
   }
   else
      dwRet = GetLastError();

   return dwRet;
}



DWORD Terminate()
{
   DWORD dwRet = ERROR_SUCCESS;

   if( g_hInst )
   {
      if( FreeLibrary(g_hInst) == FALSE )
         dwRet = GetLastError();
      g_hInst = NULL;
   }

   return dwRet;
}



int APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
   DWORD dwRet = Initialize();

   if( dwRet == ERROR_SUCCESS )
   {
      dwRet = Run();
      Terminate();
   }

   return dwRet;
}
