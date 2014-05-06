#include "stdafx.h"
#include <tchar.h>
#include <windows.h>


const TCHAR g_tzRussAlphabet[] = _T("F<RLTAU{B:RKVYJGSHCNED}IOPf,rltau[b;rkvyjgshcned]iop");
const TCHAR g_tzPortAlphabet[] = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

HHOOK g_hHook;
BOOL g_bOn = TRUE;



DWORD SetFinishEvt()
{
   HANDLE hFinishEvt = 
      OpenEvent(EVENT_MODIFY_STATE, FALSE, _T("RusKeyFinishEvent"));

   if( hFinishEvt )
   {
      SetEvent(hFinishEvt);
      CloseHandle(hFinishEvt);
   }

   return ERROR_SUCCESS;
}



LRESULT CALLBACK HookProc(int code, WPARAM wParam, LPARAM lParam)
{
   if( g_hHook == NULL )
   {
      HANDLE hFile = CreateFile(_T("RusKey.dat"),
         GENERIC_READ, 
         FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
         NULL, 
         OPEN_EXISTING, 
         0, 
         NULL);

      if( hFile != INVALID_HANDLE_VALUE )
      {
         DWORD dwRead;
         ReadFile(hFile, &g_hHook, sizeof(g_hHook), &dwRead, NULL);
         CloseHandle(hFile);
      }
   }

   if( code == HC_ACTION )
   {
      LPMSG pMsg = (LPMSG) lParam;

      switch( pMsg->message )
      {
         case WM_CHAR:
         {
            if( (TCHAR) pMsg->wParam == _T('q') )
            {
               g_bOn = g_bOn == FALSE ? TRUE : FALSE;
               if( g_bOn == FALSE )
                  return TRUE;
            }

            if( g_bOn != FALSE )
            {
               LPTSTR ptzChar = 
                  _tcschr(g_tzRussAlphabet, (TCHAR) pMsg->wParam);

               if( ptzChar )
               {
                  size_t offset = ptzChar - g_tzRussAlphabet;
                  pMsg->wParam = (WPARAM) g_tzPortAlphabet[offset];
               }
            }
         }
      }
   }

   return CallNextHookEx(g_hHook, code, wParam, lParam);
}



BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
   return TRUE;
}
