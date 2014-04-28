#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <stdlib.h>

using namespace std;


bool BestSwitch(const vector<string>& devices, const vector<string>& _outlets, __int64 yesOrNotSwitches, int& switchCount, __int64& switches, bool& isPossible)
{
	const int N = devices.size();
	const int L = devices[0].size();

	for( int i = 0; i < L; ++i )
	{
		__int64 currSwitch = ((__int64)1 << i);

		if( yesOrNotSwitches & currSwitch ) // tem switch
		{
			vector<string> outlets = _outlets;

			for( size_t o = 0; o < outlets.size(); ++o )
			{
				string& outlet = outlets[o];
				outlet[i] = outlet[i] == '0' ? '1' : '0';
			}

			sort(outlets.begin(), outlets.end());

			if( devices == outlets )
			{
				switches |= currSwitch; // adicionando o bit que trocamos
				if( ! isPossible ) // se ainda não era possível...
				{
					++switchCount; // ... precisa de mais uma troca
					isPossible = true;
				}
			}
			else
			{
				yesOrNotSwitches &= ~currSwitch;
				int switch1 = switchCount + 1;
				int switch2 = switchCount;
				bool possible1 = isPossible;
				bool possible2 = isPossible;

				if( BestSwitch(devices, outlets, yesOrNotSwitches, switch1, switches, possible1) ) // trocando
				{
					if( ! isPossible ) // se ainda não era possível
					{
						switchCount = switch1;
						isPossible = true;
					}
					else switchCount = min(switchCount, switch1);
				}

				if( BestSwitch(devices, _outlets, yesOrNotSwitches, switch2, switches, possible2) ) // não trocando
				{
					if( ! isPossible ) // se ainda não era possível
					{
						switchCount = switch2;
						isPossible = true;
					}
					else switchCount = min(switchCount, switch2);
				}
			}
		}
	}

	return isPossible;
}


int main(int argc, char* argv[])
{
	ostream* pOs = nullptr;
	istream* pIs = nullptr;

	if( argc != 3 )
	{
		pIs = &cin;
		pOs = &cout;
	}
	else
	{
		pIs = new ifstream(argv[1]);
		pOs = new ofstream(argv[2]);
	}

	istream& is = *pIs;
	ostream& os = *pOs;

	int cases = 0;
	is >> cases;

	for( int caseNum = 0; caseNum < cases; ++caseNum )
	{
		__int64 switches = 0;
		__int64 yesOrNoSwitches = 0;
		int switchCount = 0;
		vector<string> devices;
		vector<string> outlets;

		int N = 0;
		int L = 0;

		os << "Case #" << caseNum+1 << ": ";
		is >> N >> L;

		vector<int> devicesBits(L);
		vector<int> outletsBits(L);

		for( int i = 0; i < N; ++i )
		{
			string outlet;
			is >> outlet;
			outlets.push_back(outlet);
			for( size_t j = 0; j < outlet.size(); ++j )
				outletsBits[j] += outlet[j] == '1' ? 1 : 0;
		}

		for( int i = 0; i < N; ++i )
		{
			string device;
			is >> device;
			devices.push_back(device);
			for( size_t j = 0; j < device.size(); ++j )
				devicesBits[j] += device[j] == '1' ? 1 : 0;
		}
		sort(devices.begin(), devices.end());

		bool isPossible = true;

		for( int i = 0; i < L; ++i )
		{

			if( N % 2 == 0 && devicesBits[i] == N / 2 ) // precisa ou não ligar o switch
			{
				yesOrNoSwitches |= ((__int64)1 << i);
			}
			else if( devicesBits[i] == (N - outletsBits[i]) ) // precisa ligar o switch
			{
				switches |= ((__int64)1 << i);
				++switchCount;
			}
			else if( devicesBits[i] != outletsBits[i] ) // não dá pra configurar
			{
				isPossible = false;
				break;
			}
		}

		if( isPossible )
		{
			for( size_t i = 0; i < outlets.size(); ++i )
			{
				string& outlet = outlets[i];
				for( size_t j = 0; j < outlet.size(); ++j )
				{
					if( ((__int64)1<<j) & switches )
						outlet[j] = outlet[j] == '0' ? '1' : '0';
				}
			}

			sort(outlets.begin(), outlets.end());
			isPossible = devices == outlets;

			// não é possível, mas talvez ligar algum dos sim/não switches resolva
			if( ! isPossible && yesOrNoSwitches )
			{
				BestSwitch(devices, outlets, yesOrNoSwitches, switchCount, switches, isPossible);
			}
		}

		if( isPossible )
			os << switchCount << endl; 
		else 
			os << "NOT POSSIBLE\n";
	}
}
