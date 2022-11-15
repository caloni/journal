#include <chrono> // miliseconds
#include <memory> // new/delete
#include <thread> // sleep_for

struct Memory
{
  Memory(size_t size)
  {
    memory = new char[size];
  }

  ~Memory()
  {
    //delete memory;
  }

  void* memory;
};

int main()
{
  for( ; ; )
  {
    Memory(1024 * 1024);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

