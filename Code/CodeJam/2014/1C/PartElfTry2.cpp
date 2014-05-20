#include "CodeJam.h"
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <time.h>

using namespace std;


void TestCase(std::istream& is, std::ostream& os)
{
	int P, Q;
	char bar;
	is >> P >> bar >> Q;

	if( Q % P )
	{
		os << "impossible\n";
	}
	else
	{
		int gens = 0;

		while( Q > P )
		{
			Q /= 2;
			++gens;
		}

		if( gens <= 40 )
			os << gens << endl;
		else
			os << "impossible\n";
	}
}

