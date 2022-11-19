#include <stdlib.h>
#include <windows.h>

int main()
{
  void* m[100];
  int i;

  for( ; ; )
  {
    for( i = 0; i < 100; ++i )
    {
      m[i] = malloc(1024 * 1024);
      Sleep(100);
    }

    for( i = 0; i < 80; ++i )
    {
      free(m[i]);
      Sleep(100);
    }
  }
}

