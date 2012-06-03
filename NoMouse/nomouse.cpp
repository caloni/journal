//
// Author:              Wanderley Caloni Jr.
// Date:                18.set.2003
//
// Description:         Esconde o mouse no canto direito inferior. Para quem 
//                      nao gosta muito do cursor pentelhando pelo desktop.
//                      A tecla de atalho eh Ctrl Direito (para sumir e 
//                      voltar).
//            
// Exemplo retirado de
// http://rattlesnake.at.box.sk/newsread_print.php?newsid=193
//
// This code will only work if you have Windows NT or
// any later version installed, 2k and XP will work.
//

#define _WIN32_WINNT 0x0400

#include <stdio.h>
#include <windows.h>
#include <winuser.h>


// Global Hook handle
HHOOK hKeyHook;         // handle do hook de teclado
POINT lastCursorPos;    // ultima posicao do cursor



//
// This is the function that is "exported" from the
// execuatable like any function is exported from a
// DLL. It is the hook handler routine for low level
// keyboard events.
//
__declspec(dllexport) LRESULT CALLBACK KeyEvent(
   int nCode,      // The hook code
   WPARAM wParam,  // The window message (WM_KEYUP, WM_KEYDOWN, etc.)
   LPARAM lParam   // A pointer to a struct with information about the key
   )
{
   if( (nCode == HC_ACTION) &&       // we may process this event
       ((wParam == WM_SYSKEYDOWN) || // Only react if either a system key...
       (wParam == WM_KEYDOWN)) )     // ...or a normal key was pressed.
   {

      //  This struct contains various information about
      //  the pressed key such as hardware scan code, virtual
      //  key code and further flags. 
      KBDLLHOOKSTRUCT hooked = 
         *((KBDLLHOOKSTRUCT*)lParam);


      // se tecla pressionada for ctrl direito
      if( hooked.vkCode == VK_RCONTROL )
      {
         POINT curCursorPos;
         
         // captura resolucao da tela
         int cs_SM_CXSCREEN = GetSystemMetrics(SM_CXSCREEN);
         int cs_SM_CYSCREEN = GetSystemMetrics(SM_CYSCREEN);
         
         // captura posicao do cursor
         GetCursorPos(&curCursorPos);
         
         if( curCursorPos.x == cs_SM_CXSCREEN - 1 && 
             curCursorPos.y == cs_SM_CYSCREEN - 1 )
         {
            SetCursorPos(lastCursorPos.x, lastCursorPos.y);
         }
         else
         {
            lastCursorPos = curCursorPos;
            SetCursorPos(cs_SM_CXSCREEN - 1, cs_SM_CYSCREEN - 1);
         }
      }
   }

   //  the return value of the CallNextHookEx routine is always
   //  returned by your HookProc routine. This allows other
   //  applications to install and handle the same hook as well.
   return CallNextHookEx(hKeyHook, nCode,wParam,lParam);
}



//
// This is a simple message loop that will be used
// to block while we are logging keys. It does not
// perform any real task ...
//
void MsgLoop()
{
   MSG message;
   while (GetMessage(&message,NULL,0,0)) {
      TranslateMessage( &message );
      DispatchMessage( &message );
   }
}



//
// This thread is started by the main routine to install
// the low level keyboard hook and start the message loop
// to loop forever while waiting for keyboard events.
//
DWORD WINAPI KeyLogger(LPVOID lpParameter)
{

   //  Get a module handle to our own executable. Usually,
   //  the return value of GetModuleHandle(NULL) should be
   //  a valid handle to the current application instance,
   //  but if it fails we will also try to actually load 
   //  ourself as a library. The thread's parameter is the
   //  first command line argument which is the path to our
   //  executable.

   HINSTANCE hExe = GetModuleHandle(NULL);
   if (!hExe) hExe = LoadLibrary((LPCSTR) lpParameter); 

   //  Everything failed, we can't install the hook ... this
   //  never happened, but error handling is important.

   if (!hExe) return 1;

   hKeyHook = SetWindowsHookEx (  // install the hook:
      WH_KEYBOARD_LL,         // as a low level keyboard hook
      (HOOKPROC) KeyEvent,    // KeyEvent function from this executable
      hExe,                   // the module handle to our own executable
      NULL                    // the hook should monitor all threads.
      );

   //  Loop forever in a message loop and if the loop
   //  stops some time, unhook the hook. I could have
   //  added a signal handler for ctrl-c that unhooks
   //  the hook once the application is terminated by
   //  the user, but I was too lazy.
   MsgLoop();
   UnhookWindowsHookEx(hKeyHook);
   
   return 0;
}



//
// E Tio Bill disse: "WinMain!!!"
// The main function just starts the thread that 
// installs the keyboard hook and waits until it
// terminates.
//
INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR argv, INT)
{
   HANDLE hThread;
   DWORD dwThread;
   DWORD exThread;

   // cria a thread que inicia hook de teclado
   hThread = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)
      KeyLogger, (LPVOID) argv, NULL, &dwThread);

   // aguarda a finalizacao da thread
   while( GetExitCodeThread(hThread, &exThread) )
      if( exThread == STILL_ACTIVE )
         Sleep(10);

   return 0;

}
