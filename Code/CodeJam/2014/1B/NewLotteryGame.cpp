#include "CodeJam.h"
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <time.h>

using namespace std;

typedef __int64 Int;

map<int, Int> Wins;


void TestCase(std::istream& is, std::ostream& os)
{
	Int A, B, K;
	is >> A >> B >> K;
	Int wins = 0;

    if( A < K || B < K )
        wins = A * B;
    else
    {
        wins = (K) * (K);

        for( Int a = 0; a < A; ++a )
        {
            for( Int b = K; b < B; ++b )
            {
				Int result = a & b;
                if( result < K )
                    wins++;
            }
        }

        for( Int b = 0; b < K; ++b )
        {
            for( Int a = K; a < A; ++a )
            {	
                Int result = a & b;
                if( result < K )
                    wins++;
            }
        }
    }

	char buf[100];
	sprintf(buf, "%I64u", wins);
	os << buf << endl;
}

void TestCaseTry1(std::istream& is, std::ostream& os)
{
	Int A, B, K;
	is >> A >> B >> K;
	Int wins = 0;

	if( A < K || B < K)
	{
		wins = A * B;
	}
	else if( A == K && B == K )
	{
		wins = A * B + 2;
	}
	else if( A == K+1 && B == K+1 )
	{
		wins = A * B - 4;
	}
	else
	{
		//os << A << ' ' << B << ' ' << K << endl;
		for( Int a = 0; a < A; ++a )
		{
			for( Int b = 0; b < B; ++b )
			{
				Int result = a & b;
				if( result < K )
					wins++;
			}
		}
	}

	char buf[100];
	sprintf(buf, "%I64u", wins);
	os << buf << endl;
}
