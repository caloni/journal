#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;


int RemoveIfLolita(int* ages, int size)
{
	auto newEnd = remove_if(ages, ages + size, [&](int age) { return age < 18; } );
}

void RemoveIfLolita(vector<int>& ages)
{
	remove_if(ages.begin(), ages.end(), [&](int age) { return age < 18; } );
}


int main()
{
	vector<int> ages;

	ages.push_back(10);
	ages.push_back(21);
	ages.push_back(66);
	ages.push_back(18);
	ages.push_back(16);
	ages.push_back(15);
	ages.push_back(8);
	ages.push_back(24);
	ages.push_back(12);
	ages.push_back(20);
	ages.push_back(13);
	ages.push_back(13);

	RemoveIfLolita(ages);
	cout << "Vector (" << ages.size() << "):\n";
	for_each(ages.begin(), ages.end(), [&](int age) { cout << age << endl; });

	int newAges[] = { 10, 21, 66, 18, 16, 15, 8, 24, 12, 20, 13, 13 };
	const int newAgesSz = (int) ( sizeof(newAges) / sizeof(int) );
	RemoveIfLolita(newAges, newAgesSz);
	cout << "\n\nArray (" << newAgesSz << "):\n";
	for_each(newAges, newAges + newAgesSz, [&] (int age) { cout << age << endl; } );
}
