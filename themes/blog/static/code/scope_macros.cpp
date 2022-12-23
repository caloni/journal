#include <iostream>

#define LOG(text) std::cout << __FUNCTION__ << " " << text << std::endl

using namespace std;

void MyFunction()
{
  LOG("calling me");
}

class MyClass
{
public:
  MyClass()
  {
    LOG("calling me");
  }

  ~MyClass()
  {
    LOG("calling me");
  }

  void MyMethod()
  {
    LOG("calling me");
  }
};

int main()
{
  MyClass c;
  MyFunction();
  c.MyMethod();
}

