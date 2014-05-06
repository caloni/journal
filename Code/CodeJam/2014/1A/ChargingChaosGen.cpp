#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <string>

using namespace std;

int main()
{
	const char bits[] = { '0', '1' };
	ofstream in("A-test.in");
	ofstream out("A-test.cmp");

	int T = 100;
	in << T << endl;

	for( int t = 0; t < T; ++t )
	{
		int N = rand() % 10 + 1;
		int L = rand() % 9 + 2;

		in << N << ' ' << L << endl;

		vector<string> devices;
		for( int n = 0; n < N; ++n )
		{
			string device;
			for( int l = 0; l < L; ++l )
				device.push_back(bits[rand() % 2]);
			devices.push_back(device);
		}

		vector<string> outlets = devices;
		int switchCount = 0;
		for( int l = 0; l < L; ++l )
		{
			if( rand() % 2 )
			{
				++switchCount;
				for( int n = 0; n < N; ++n )
				{
					string& outlet = outlets[n];
					outlet[l] = outlet[l] == '0' ? '1' : '0';
				}
			}
		}

		for( int n = 0; n < N; ++n )
			in << devices[n] << ' ';
		in << endl;
		for( int n = 0; n < N; ++n )
			in << outlets[n] << ' ';
		in << endl;

		out << "Case #" << t+1 << ": " << switchCount << endl;
	}
}
