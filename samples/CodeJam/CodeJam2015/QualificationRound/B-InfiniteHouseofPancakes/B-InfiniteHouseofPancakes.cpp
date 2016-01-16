#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <math.h>

using namespace std;

typedef map<int, int> Diners;


void PrintDiners(Diners& diners)
{
	for (auto it = diners.begin(); it != diners.end(); ++it)
	{
		for (int i = 0; i < it->second; ++i)
			cout << it->first << ' ';
	}
	cout << endl;
}

int BetweenHalfAndMax(Diners& diners, int maxPancakes)
{
	int ret = 0;
	int half = (maxPancakes / 2) + (maxPancakes % 2);
	for (auto it = diners.begin(); it != diners.end(); ++it)
	{
		if (it->first < maxPancakes && it->first > half)
			ret += it->second;
	}
	return ret;
}

int DivideAndConquer(Diners diners, int maxPancakes, list<Diners>& historic)
{
	if (maxPancakes <= 2) return maxPancakes;
	int maxDiners = diners[maxPancakes];
	int half = maxPancakes / 2;
	int halfRest = half + maxPancakes % 2;

	for (int i = 0; i < maxDiners; ++i)
	{
		--diners[maxPancakes];
		++diners[half];
		++diners[halfRest];
		historic.push_back(diners);
	}

	int nextMaxPancakes = maxPancakes;
	while (diners[nextMaxPancakes] == 0 && nextMaxPancakes)
		--nextMaxPancakes;

	list<Diners> myhistoric = historic;
	int divideMinutes = maxDiners + DivideAndConquer(diners, nextMaxPancakes, historic);

	if (divideMinutes < maxPancakes)
	{
		//for (Diners diners : myhistoric)
		//	PrintDiners(diners);
		return divideMinutes;
	}
	return maxPancakes;
}

int main()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; ++t)
	{
		Diners diners;
		int maxPancakes = 0;
		int minutes = 0;

		int dinersTotal;
		cin >> dinersTotal;

		for (int i = 0; i < dinersTotal; ++i)
		{
			int pancakes;
			cin >> pancakes;
			maxPancakes = max(maxPancakes, pancakes);
			++diners[pancakes];
		}

		//PrintDiners(diners);
		list<Diners> historic;
		minutes = DivideAndConquer(diners, maxPancakes, historic);
		//for(Diners diners : historic)
		//	PrintDiners(diners);

		cout << "Case #" << t << ": " << minutes << endl;
	}
}
