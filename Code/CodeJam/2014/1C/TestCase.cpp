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

	int gens = 0;

	while( Q > P )
	{
		if( Q % 2 )
		{
			os << "impossible\n";
			return;
		}
		else Q /= 2;

		gens++;
	}

	if( P > Q )
	{
		if( Q % (P-Q) )
			os << "impossible\n";
		else
			os << gens << endl;
	}
	else os << gens << endl;
}

