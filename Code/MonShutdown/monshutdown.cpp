#include <windows.h>

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
   HWND hTrayWin;

   hTrayWin = FindWindow("Shell_TrayWnd", NULL);
   
   if( hTrayWin )
      SendMessage(hTrayWin, WM_SYSCOMMAND, SC_MONITORPOWER, 2);

   return 0;
}
