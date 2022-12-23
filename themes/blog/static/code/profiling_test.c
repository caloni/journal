
int Profiling10()
{
  int i, j = 0;
  for( i = 0; i < 10; ++i )
  {
    for( i = 0; i < 10; ++i )
    {
      j += 3;
    }
  }

  return j / 2;
}

int Profiling100()
{
  int i, j = 0;
  for( i = 0; i < 100; ++i )
  {
    for( i = 0; i < 100; ++i )
    {
      j += 3;
    }
  }

  return j / 2;
}

int Profiling1000()
{
  int i, j = 0;
  for( i = 0; i < 1000; ++i )
  {
    for( i = 0; i < 1000; ++i )
    {
      j += 3;
    }
  }

  return j / 2;
}

int Profiling()
{
  int i, x = 0;
  for( i = 0; i < 10; ++i )
  {
    x += Profiling10();
    x += Profiling100();
    x += Profiling1000();
  }
  return x;
}

int main()
{
  int x;
  x = Profiling();
}

