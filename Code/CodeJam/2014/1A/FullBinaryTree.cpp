#include "CodeJam.h"
#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <numeric>

using namespace std;


const int MAX_NODES = 1000;

map<int, vector<int>> nodes;


int Count(int root, int node, const vector<int>& children)
{
	int count = 1;
    for( auto c = children.begin(); c != children.end(); ++c )
        if( *c != root )
            count += Count(node, *c, nodes[*c]);
	return count;
}


int Node(int root, int node, const vector<int>& children)
{
    if( children.size() == 1 ) return 0;
    if( children.size() == 2 )
    {
        int child = children[children[0] == root ? 1 : 0];
        return Count(node, child, nodes[child]);
    }
    int cuts = 0;
	vector<int> counts;
    for( auto c = children.begin(); c != children.end(); ++c )
    {
        if( *c != root )
        {
            int cut = Node(node, *c, nodes[*c]);
			int count = Count(node, *c, nodes[*c]);
			counts.push_back(count - cut);
            cuts += cut;
        }
    }
	sort(counts.begin(), counts.end());
	counts.pop_back();
	counts.pop_back();
	cuts += accumulate(counts.begin(), counts.end(), 0);
    return cuts;
}


int Root(int root, const vector<int>& children)
{
    if( children.size() == 1 ) return Count(root, children[0], nodes[children[0]]);
    if( children.size() == 2 ) return Node(root, children[0], nodes[children[0]]) + Node(root, children[1], nodes[children[1]]);
    int cuts = 0;
	vector<int> counts;
    for( auto c = children.begin(); c != children.end(); ++c )
    {
        int cut = Node(root, *c, nodes[*c]);
		int count = Count(root, *c, nodes[*c]);
		counts.push_back(count - cut);
        cuts += cut;
    }
	sort(counts.begin(), counts.end());
	counts.pop_back();
	counts.pop_back();
	cuts += accumulate(counts.begin(), counts.end(), 0);
    return cuts;
}


void TestCase(std::istream& is, std::ostream& os)
{
    int N = 0;
    is >> N;

	nodes.clear();

    for( int n = 1; n < N; ++n )
    {
        int x, y;
        is >> x >> y;
        nodes[x].push_back(y);
        nodes[y].push_back(x);
    }

    int cuts = MAX_NODES;
    for( auto root = nodes.begin(); root != nodes.end(); ++root )
	{
		int newCut = Root(root->first, root->second);
        cuts = min(cuts, newCut);
	}
    os << cuts << endl;
}

