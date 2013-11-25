#include "ForRangedLoop.h"
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

void ForRangedLoop()
{
	vector<int> myInts = { 5, 3, 7, 1, 8, 3, 9 };

	cout << "Unsorted: ";
	for (auto i : myInts)
		cout << i << ' ';
	cout << endl;

	sort(myInts.begin(), myInts.end());
	cout << "Sorted: ";
	for (auto i : myInts)
		cout << i << ' ';
	cout << endl;
}
