/** Cast test.
@author Wanderley Caloni <wanderley.caloni@gmail.com>
@date 2020-04
*/

#include <string>

class T {
public:
  inline operator std::string() const {
    return "";
  }

  explicit inline operator uint64_t() const
  {
    return 10;
  }
};

int main()
{
  T t;
  //t(); // error C2064: term does not evaluate to a function taking 0 arguments
  uint64_t i = (uint64_t) t;
}

