#include "CodeJam.h"
#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <string>

using namespace std;

static const int MAX_SWITCHES = 40;


__int64 StringToInt(const string& s, int L)
{
	__int64 ret = 0;
	for( int l = L; l; --l )
	{
		char c = s[L-l];
		if( c == '1' )
			ret |= ((__int64)1 << (l-1));
	}
	return ret;
}

int CountBits(__int64 value)
{
	int ret = 0;
	while( value )
	{
		if( value & 1 )
			++ret;
		value >>= 1;
	}
	return ret;
}

void TestCase(std::istream& is, std::ostream& os)
{
	int N, L;
	is >> N >> L;

	vector<__int64> outlets;
	vector<__int64> devices;
	string eletricItem;

	for( int n = 0; n < N; ++n )
	{
		is >> eletricItem;
		outlets.push_back(StringToInt(eletricItem, L));
	}

	for( int n = 0; n < N; ++n )
	{
		is >> eletricItem;
		devices.push_back(StringToInt(eletricItem, L));
	}

	typedef map<__int64, set<int>> SwitchOcurrences;
	SwitchOcurrences switchOcurrences;
	
	for( int n = 0; n < N; ++n )
	{
		__int64 outlet = outlets[n];

		for( int n2 = 0; n2 < N; ++n2 )
		{
			__int64 device = devices[n2];
			__int64 connection = outlet ^ device;
			switchOcurrences[connection].insert(n);
		}
	}

	for(auto it = switchOcurrences.begin(); it != switchOcurrences.end(); )
	{
		if( it->second.size() != N )
			it = switchOcurrences.erase(it);
		else
			++it;
	}

	if( switchOcurrences.size() )
	{
		int switches = MAX_SWITCHES + 1;

		for_each(switchOcurrences.begin(), switchOcurrences.end(), [&](const SwitchOcurrences::value_type& ocurrence)
		{
			int bits = CountBits(ocurrence.first);
			switches = min(switches, bits);
		});

		os << switches << endl;
	}
	else os << "NOT POSSIBLE\n";
}
