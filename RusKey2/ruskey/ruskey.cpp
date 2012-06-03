/** @file

Main file of executable file that handles the start/stop of the msg hook.

@author Wanderley Caloni Jr <wanderley@caloni.com.br>
@date 10/2005
*/
#include "../ruskeydll/ruskeydll.h"
#include <windows.h>
#include <tchar.h>

#define MODULE_NAME _T("RusKey")


/** Prints a error MsgBox with the system description for the error code. */
void Error(HRESULT ret)
{
   PTSTR msgText;
   DWORD fmtMsgFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER 
      | FORMAT_MESSAGE_FROM_SYSTEM;

   if( FormatMessage(fmtMsgFlags, NULL, ret, 0, (PTSTR) &msgText, 0, 0) )
   {
      MessageBox(NULL, msgText, _T("RusKey"), MB_OK | MB_ICONINFORMATION);
      LocalFree(msgText);
   }
}


/** Start the DLL hook and wait until it terminates. */
HRESULT RunRusKey()
{
   HRESULT ret = S_OK;
   ATOM hkId = GlobalAddAtom(_T("ruskey"));

   if( RegisterHotKey(NULL, hkId, MOD_CONTROL | MOD_ALT, 'U') )
   {
      MSG msg;

      ret = StartHook();

      if( SUCCEEDED(ret) )
      {
         BOOL validMsg;

         bool enabled = true;

         MessageBox(NULL, _T("RusKey v. beta\r\n")
            _T("by Wanderley Caloni Jr <wanderley@caloni.com.br>\r\n\r\n")
            _T("Press Ctrl + Alt + U to exit the russian mode.")
            _T(" Click OK to continue."),
            MODULE_NAME,
            MB_OK | MB_ICONINFORMATION);
      
         while( (validMsg = GetMessage(&msg, NULL, 0, 0)) != 0 )
         {
            if( validMsg == -1 )
            {
               Error(ret);
               break;
            }

            switch( msg.message )
            {
            case WM_HOTKEY:
               if( msg.wParam == hkId )
               {
                  PostMessage(NULL, WM_QUIT, 0, 0);
               }
            break;
            }
         }

         if( enabled )
            ret = StopHook();
      }
      else
      {
         Error(ret);
      }
   }
   else
   {
      Error(ret);
   }

   return ret;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
   return (int) RunRusKey();
}
