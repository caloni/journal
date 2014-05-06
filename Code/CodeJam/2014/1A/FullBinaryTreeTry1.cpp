#include "CodeJam.h"
#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <string>

using namespace std;


void TestCase(std::istream& is, std::ostream& os)
{
    int N = 0;
    map<int, int> counter;
    map<int, int> counter2;
    int cutCounter = 0;

    is >> N;

    for( int n = 1; n < N; ++n )
    {
        int x, y;
        is >> x >> y;
        int& countX = counter[x];
        int& countY = counter[y];
        ++countX;
        ++countY;
    }

    for( auto it = counter.begin(); it != counter.end(); ++it )
    {
        int nodes = it->second;
        int& count = counter2[nodes];
        ++count;
    }

    os << endl;
    for( auto it = counter.begin(); it != counter.end(); ++it )
    {
        int num = it->first;
        int nodes = it->second;
        os << num << ' ' << nodes << endl;
    }

    for( auto it = counter2.begin(); it != counter2.end(); ++it )
    {
        int nodes = it->first;
        if( nodes > 3 )
        {
            int cut = nodes - 3;
            cutCounter += cut * it->second;
            int newNodes = nodes - cut;
            counter2[newNodes] += it->second;
        }
    }

    if( counter2[2] > 1 )
    {
        int cut = counter2[2] - 1;
        cutCounter += cut;
        //counter2[1] += cut;
    }

    if( counter2[3] != 0 && counter2[1] != 0 && counter2[3] < counter2[1]*2 )
    {
        int cut = counter2[1] - counter2[3]*2;
        cutCounter += cut;
    }

    if( counter2[3] != 0 && counter2[1] != 0 && counter2[3] > counter2[1]*2 )
    {
        int cut = counter2[1]*2 - counter2[3];
        cutCounter += cut;
    }

    os << cutCounter << endl;
}

