/** VS 2017 15.3 Preview 5.0 test
@author Wanderley Caloni <wanderley.caloni@gmail.com>
@date 2017-07-31
@see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4128.html#an-iterables-end-may-have-a-different-type-than-its-begin
*/

#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>

using namespace std;

// For determining whether an iterator refers to a null value:
struct null_sentinel
{
    template<typename I>
    friend bool operator==(I i, null_sentinel) { return 0 == *i; }
    template<typename I>
    friend bool operator!=(I i, null_sentinel) { return 0 != *i; }
    // ... and friends
};

int main(int argc, char* argv[])
{
    vector<int> v1 = { 2, 4, 6, 8, 10, 3, 5, 7, 9, 11 };
    for_each(v1.begin(), v1.end(), [=](auto el) { cout << el << endl; });
    for_each(argv[1], null_sentinel(), [=](auto el) { cout << el << endl; });
}

