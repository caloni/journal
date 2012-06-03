#pragma once
#include <algorithm>
#include <iostream>
#include <assert.h>

template<typename T, size_t Sz>
struct TArray
{
   TArray()
   {
      for( size_t i = 0; i < Sz; ++i )
         m_values[i] = T();
   }

   const T& operator [] (size_t ind) const
   {
      assert(ind < Sz);
      return m_values[ind];
   }
   
   T& at(size_t ind)
   {
      assert(ind < Sz);
      return m_values[ind];
   }

   T m_values[Sz];
};


template<typename T, size_t Sz>
bool operator == (const TArray<T, Sz>& l, const TArray<T, Sz>& r)
{
   for( size_t i = 0; i < Sz; ++i )
      if( l[i] != r[i] )
         return false;
   return true;
}


template<typename T, size_t Sz>
std::ostream& operator << (std::ostream& os, const TArray<T, Sz>& r)
{
   for( size_t i = 0; i < Sz - 1; ++i )
   {
      os.width(2);
      os.fill('0');
      os << r[i] << ' ';
   }

   os.width(2);
   os.fill('0');
   os << r[Sz - 1];

   return os;
}
