#include <stdio.h>

int gargalo()
{
  int i, j, k = 0;

  for( i = 0; i < 1 * 1000 * 1000; ++i )
  {
    for( j = 0; j < 1 * 10 * 10; ++j )
    {
      k = i + j + k;
    }
  }

  return k;
}

int not_gargalo()
{
  return 42;
}

int soso()
{
  int i, k = 0;

  for( i = 0; i < 1 * 1000 * 1000; ++i )
  {
    k = i + k;
  }

  return k;
}


int redirect()
{
  typedef int (*FUNC)();
  FUNC funcs[] = { not_gargalo, gargalo, soso, NULL };
  int i = 0;
  int ret = 0;

  while( funcs[i] )
  {
    ret += (*funcs[i])();
    i++;
  }

  return ret;
}

int main()
{
  int ret = redirect();
  printf("the number is %d\n", ret);
}

