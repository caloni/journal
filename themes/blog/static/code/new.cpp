#include <chrono> // miliseconds
#include <memory> // new/delete
#include <thread> // sleep_for
#include <iostream> // cout, endl
#include <exception> // exception


struct Memory
{
  Memory(size_t quantoPreciso)
  {
    m = new char[quantoPreciso];
    ref = new int(1);
  }

  Memory(Memory& obj)
  {
      m = obj.m;
      ref = obj.ref;
      ++(*ref);
  }

  ~Memory()
  {
      if (ref)
      {
          --(*ref);
          if (*ref == 0)
          {
              delete[] m;
              delete ref;
          }
      }
  }

  char* m;
  int* ref;
};

void UsaMemory(std::shared_ptr<char> m)
{
}

int main()
{
  for( ; ; )
  {
      std::shared_ptr<char> m(new char[1024 * 1024]);
      UsaMemory(m);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

