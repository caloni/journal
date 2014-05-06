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


struct Words
{
    Words(std::istream& is)
    {
		int N;
		is >> N;
        for( int i = 0; i < N; ++i )
        {
            string _w;
            is >> _w;
            w.push_back(_w);
        }
    }

    int size()
    {
        int ret = 0;
        for_each(w.begin(), w.end(), [&](const string& s) { ret = max(ret, (int) s.size()); });
		return ret;
    }

    char letter(int pos)
    {
        map<char, int> counter;
        for_each(w.begin(), w.end(), [&](const string& s)
        {
            if( s.size() > pos )
                counter[s[pos]]++;
            else
                counter[0]++;
        });

        int retCount = 0;
        char ret = ' ';
        for( auto it = counter.begin(); it != counter.end(); ++it )
        {
            if( it->second > retCount )
            {
                ret = it->first;
                retCount = it->second;
            }
        }

        return ret;
    }

    int adjust(int pos, char lastChar, char letter)
    {
        int moves = 0;

        for_each(w.begin(), w.end(), [&](string& s)
        {
			if( moves == -1 ) return;
            if( s.size() > pos )
            {
                if( s[pos] == letter ) return;
                if( s[pos] == lastChar )
                {
                    s.erase(pos, 1);
                    moves++;
                }
                else if( pos > 0 && lastChar == letter )
                {
                    s.insert(pos, 1, lastChar);
                    moves++;
                }
                else moves = -1;
            }
			else if( letter == 0 )
			{
				return;
			}
            else if( lastChar == letter )
            {
                s.insert(pos, 1, lastChar);
                moves++;
            }
            else moves = -1;
        });

        return moves;
    }

    vector<string> w;
};

void TestCase(std::istream& is, std::ostream& os)
{
    Words words(is);

    int moves = 0;
    char lastChar = ' ';
    for( int i = 0; i < words.size(); ++i )
    {
        char c = words.letter(i);

        if( int m = words.adjust(i, lastChar, c) )
		{
			if( m == -1 )
			{
				moves = m;
				break;
			}
			else if( m )
			{
				moves += m;
				i -= 1;
				continue;
			}
		}

		if( c != 0 )
			lastChar = c;
    }

    if( moves == -1 )
        os << "Fegla Won\n";
    else
        os << moves << endl;
}

