#include "ForRangedLoop.h"
#include "InitializerList.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

template<typename T> void PrintAlignOf()
{
	std::cout << "alignof " << typeid(T).name() << " is " << alignof(T) << std::endl;
}


alignas(char) struct AlignTest
{
	int c;
	double d;
	char c2;
};


void AlignOf()
{
	PrintAlignOf<char>();
	PrintAlignOf<int>();
	PrintAlignOf<float>();
	PrintAlignOf<double>();
	PrintAlignOf<__int64>();
	PrintAlignOf<AlignTest>();

	AlignTest test;
	test.c = 0x12345678;
	test.c2 = 'a';
	test.d = 0.9;
}


void GenericAuto()
{
	// 'Identity' is a lambda that accepts an argument of any type and
	// returns the value of its parameter.  
	auto Identity = [](auto a) { return a; };
	int three = Identity(3);
	char const* hello = Identity("hello");
	
	// Conversion to function pointer for capture-less lambdas
	int(*fpi)(int) = Identity;
	char(*fpc)(char) = Identity;

	std::for_each(hello, hello + 5, Identity);
}


struct S {
	static const int size;
};
const int limit = 2 * S::size; // dynamic initialization
const int S::size = 256;
const int z = numeric_limits<int>::max(); // dynamic initialization


class DefaultFuncs
{
public:
	DefaultFuncs() = default;
	~DefaultFuncs();

private:
	string m_member;
};

//DefaultFuncs::DefaultFuncs() = default;

DefaultFuncs::~DefaultFuncs() = default;


void DefaultFuncsTest()
{
	DefaultFuncs def;
}


class DeletedFuncsBase
{
public:
	void Call();
};

class DeletedFuncs : public DeletedFuncsBase
{
public:
	void Call() = delete;
};


void DeletedGlobalFunc() = delete;

void DeletedFuncsTest()
{
	DeletedFuncs del;
	//del.Call();
	DeletedGlobalFunc();
}


int main()
{
	DefaultFuncsTest();
	GenericAuto();
	ForRangedLoop();
	InitializerList();
	AlignOf();
	return 0;
}
