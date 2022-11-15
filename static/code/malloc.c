#include <stdlib.h>
#include <windows.h>

int main()
{
  for( ; ; )
  {
    malloc(1024 * 1024);
    Sleep(100);
  }
}

