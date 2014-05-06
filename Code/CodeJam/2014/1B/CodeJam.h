#pragma once
#include <algorithm>
#include <iostream>


template<typename T>
struct Bits
{
	Bits(const T& t) : _t(t) {}
	const T& _t;
};

template<typename T>
Bits<T> bits(const T& t)
{
	return Bits<T>(t);
};

template<typename T>
std::ostream& operator << (std::ostream& os, Bits<T>& b)
{
	string s;
	T x = b._t;
	while( x )
	{
		s += x & 1 ? '1' : '0';
		x >>= 1;
	}
	if( s.empty() )
		s = "0";
	else
		std::reverse(s.begin(), s.end());
	os << s;
	return os;
}

void TestCase(std::istream& is, std::ostream& os);
