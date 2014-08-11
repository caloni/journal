// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Header.h"
#include <iostream>
#include <windows.h>

void Func1()
{
	int x = 0;
	for (int i = 0; i < 100; ++i)
		x *= i;
}

void Func2()
{
	int x = 0;
	for (int i = 0; i < 1000; ++i)
		x *= i;
}

void Func3()
{
	int x = 0;
	for (int i = 0; i < 10000; ++i)
		x *= i;
}

void Profile()
{
	for (int i = 0; i < 3; ++i)
	{
		Func1();
		Func2();
		Func3();
	}
}

int main(int argc, _TCHAR* argv[])
{
	Profile();
	SomeFunction();
	AnotherFunction();
	return 0;
}

