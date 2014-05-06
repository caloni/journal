/** @file

Main file of DLL file that (un)installs and handles the msg hook.

@author Wanderley Caloni Jr <wanderley@caloni.com.br>
@date 10/2005
*/
#include <tchar.h>
#include <windows.h>

#define MODULE_NAME _T("ruskey.dll")

#pragma section( "shared", read, write, shared )


LRESULT CALLBACK HookProc(int code, WPARAM wParam, LPARAM lParam);

/** The replace charset. The character is found by its index 
inside the original array.
*/
const TCHAR g_tzRussAlphabet[] 
   = _T("F<RLTAU{B:RKVYJGSHCNED}IOPf,rltau[b;rkvyjgshcned]iop");

/** The original charset. The character is replaced by its index
equivalent into the replace charset.
*/
const TCHAR g_tzPortAlphabet[] 
   = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

/** The handle for the installed hook.
@remark This variable will be shared between all processes.
*/
__declspec(allocate("shared")) HHOOK g_hHook;


/** Exported function that installs the msg hook. */
HRESULT WINAPI StartHook()
{
   HRESULT ret = S_OK;

   g_hHook = SetWindowsHookEx(WH_GETMESSAGE, HookProc, GetModuleHandle(MODULE_NAME), 0);

   if( g_hHook == NULL )
   {
      ret = HRESULT_FROM_WIN32( GetLastError() );
   }

   return ret;
}


/** Exported function that uninstalls the msg hook. */
HRESULT WINAPI StopHook()
{
   HRESULT ret = S_OK;

   if( !UnhookWindowsHookEx(g_hHook) )
   {
      ret = HRESULT_FROM_WIN32( GetLastError() );
   }

   return ret;
}


/** The procedure that handles the msg hook.

Basically it replaces the character typed by the pseudo-russian 
phoneme character, if found.
*/
LRESULT CALLBACK HookProc(int code, WPARAM wParam, LPARAM lParam)
{
   if( code == HC_ACTION )
   {
      LPMSG pMsg = (LPMSG) lParam;

      switch( pMsg->message )
      {
         case WM_CHAR:
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

   return CallNextHookEx(g_hHook, code, wParam, lParam);
}


/** In the beginning God said: 'DllMain!'
*/
BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
   return TRUE;
}
