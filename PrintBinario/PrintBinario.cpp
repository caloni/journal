// PrintBinario.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <bitset>
#include <string>
#include <vector>


using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	vector<int> ints;
	int x;
	while( cin >> x )
		ints.push_back(x);
	for( size_t i = 0; i < ints.size(); ++i )
		cout << ints[i] << endl;
}

