#include <iostream>
#include <thread>
#include <mutex>

std::mutex g_lock1;
std::mutex g_lock2;
int g_counter = 0;

void func1()
{
  while( true )
  {
    g_lock1.try_lock();
    g_lock2.lock();
    if( g_is_func1 )
    {
      std::cout << "func1 " << ++g_counter << "\n";
      g_is_func1 = false;
      g_is_func2 = true;
    }
  }
}

void func2()
{
  while( true )
  {
    if( g_is_func2 )
    {
      std::cout << "func2 " << ++g_counter << "\n";
      g_is_func2 = false;
      g_is_func1 = true;
    }
  }
}

int main()
{
  std::thread f1(func1);
  std::thread f2(func2);

  f1.join();
  f2.join();
}

