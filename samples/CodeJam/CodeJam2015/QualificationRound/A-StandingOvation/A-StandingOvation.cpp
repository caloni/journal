#include <iostream>
#include <string>

using namespace std;

int main()
{
	int T;
	cin >> T;
	for (int t = 1; t <= T; ++t)
	{
		int shyMax;
		string people;

		cin >> shyMax;
		cin >> people;

		int neededGuests = 0;
		int totalSoFar = int(people[0] - '0');

		for (int s = 1; s <= shyMax; ++s)
		{
			if (s > totalSoFar)
			{
				int add = s - totalSoFar;
				neededGuests += add;
				totalSoFar += add;
			}

			totalSoFar += int(people[s] - '0');
		}

		cout << "Case #" << t << ": " << neededGuests << endl;
	}
}
