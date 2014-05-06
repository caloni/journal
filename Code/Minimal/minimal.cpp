/* Um programa estrutural minimo do Windows 95 */

#include <windows.h>

LRESULT CALLBACK WindowFunc(HWND, UINT, WPARAM, LPARAM);

char szWinName[] = "MyWin"; /* nome da classe de janela */

int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, 
                   LPSTR lpszArgs, int nWinMode)
{
   HWND hwnd;
   MSG msg;
   WNDCLASSEX wcl;
   
   /* Define uma classe de janela. */
   wcl.hInstance = hThisInst; /* handle para esta ocorrencia */
   wcl.lpszClassName = szWinName; /* nome da classe de janela */
   wcl.lpfnWndProc = WindowFunc; /* funcao de janela */
   
   wcl.style = 0; /* estilo padrao */
   wcl.cbSize = sizeof(WNDCLASSEX); /* estabelece o tamanho de WNDCLASSEX */
   
   wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION); /* icone grande */
   wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO); /* icone pequeno */
   
   wcl.hCursor = LoadCursor(NULL, IDC_ARROW); /* estilo do cursor */
   wcl.lpszMenuName = NULL; /* nenhum menu */
   
   wcl.cbClsExtra = 0; /* nenhuma informacao extra */
   wcl.cbWndExtra = 0; /* eh necessaria */
   
   /* Faz com que o fundo da janela fique branco. */
   wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
   
   /* Registra a classe de janela. */
   if( !RegisterClassEx(&wcl) ) return 0;
   
   /* Agora que uma classe de janela foi registrada, pode ser criada 
      uma janela */
   hwnd = CreateWindow(
      szWinName, /* nome da classe de janela */
      "Programa Estrutura para Windows 95", /* titulo */
      WS_OVERLAPPEDWINDOW, /* estilo de janela - normal */
      CW_USEDEFAULT, /* coordenada X - deixa o Windows decidir */
      CW_USEDEFAULT, /* coordenada Y - deixa o Windows decidir */
      CW_USEDEFAULT, /* largura - deixa o Windows decidir */
      CW_USEDEFAULT, /* altura - deixa o Windows decidir */
      HWND_DESKTOP, /* nenhuma janela de origem */
      NULL, /* nenhum menu */
      hThisInst, /* handle desta ocorrencia do programa */
      NULL /* nenhum argumento adicional */
   );
   
   /* Exibe a janela. */
   ShowWindow(hwnd, nWinMode);
   UpdateWindow(hwnd);
   
   /* Cria o loop de mensagens. */
   while( GetMessage(&msg, NULL, 0, 0) )
   {
      TranslateMessage(&msg); /* permite a utilizacao do teclado */
      DispatchMessage(&msg); /* retorna o controle para o Windows */
   }
   
   return int(msg.wParam);
}

/* Esta funcao eh chamada pelo Windows 95 e sao passadas
   mensagens a partir da fila de mensagens.
*/
LRESULT CALLBACK WindowFunc(HWND hwnd, UINT message, 
                            WPARAM wParam, LPARAM lParam)
{
   switch( message ) {
      case WM_DESTROY: /* encerra o programa */
         PostQuitMessage(0);
         break;
      default:
         /* Permite que o Windows 95 processe quaisquer mensagens
            nao especificadas na instrucao switch anterior */
         return DefWindowProc(hwnd, message, wParam, lParam);
   }
   return 0;
}
