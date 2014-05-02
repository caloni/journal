#include "CodeJam.h"
#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <string>

using namespace std;


const int MAX_NODES = 1000;

map<int, map<int, int>> nodes;


int CalcRecursiveNodeCuts(int root, int node)
{
    auto& edges = nodes[node];
    int size = (int) edges.size();
    int cuts = size == 1 ? 1 : size == 3 ? 0 : size == 2 ? 1 : size - 3;
    int firstChild = size > 1 ? MAX_NODES : 0;
    int secondChild = size > 2 ? MAX_NODES : 0;

    for( auto e = edges.begin(); e != edges.end(); ++e )
    {
        if( e->first != root )
        {
            int& nodeCuts = e->second;
            if( nodeCuts == -1 )
                nodeCuts = CalcRecursiveNodeCuts(node, e->first);
            if( nodeCuts < firstChild )
            {
                secondChild = firstChild == MAX_NODES ? secondChild : firstChild;
                firstChild = nodeCuts;
            }
            else if( nodeCuts < secondChild )
            {
                secondChild = nodeCuts;
            }
        }
    }

    cuts += firstChild + secondChild;
    edges[root] = cuts;
    return cuts;
}


int CalcRoot(int root)
{
    auto& edges = nodes[root];
    int size = (int) edges.size();
    int cuts = 0;
    int firstChild = MAX_NODES;
    int secondChild = size > 1 ? MAX_NODES : 0;

    for( auto e = edges.begin(); e != edges.end(); ++e )
    {
        int& nodeCuts = e->second;
        if( nodeCuts < firstChild )
        {
            secondChild = firstChild;
            firstChild = nodeCuts;
        }
        else if( nodeCuts < secondChild )
        {
            secondChild = nodeCuts;
        }
    }

    cuts += firstChild + secondChild;
    return cuts;
}


void TestCase(std::istream& is, std::ostream& os)
{
    int N = 0;
    is >> N;

    for( int n = 1; n < N; ++n )
    {
        int x, y;
        is >> x >> y;
        nodes[x][y] = -1;
        nodes[y][x] = -1;
    }

    os << endl;
    for( auto child = nodes.begin(); child != nodes.end(); ++child )
    {
        for( auto root = child->second.begin(); root != child->second.end(); ++root )
        {
            os << "Root " << root->first << " -> Node " << child->first << " = " << CalcRecursiveNodeCuts(root->first, child->first) << endl;
        }
    }

    int cuts = MAX_NODES;
    for( auto child = nodes.begin(); child != nodes.end(); ++child )
    {
        cuts = min(cuts, CalcRoot(child->first));
    }
    os << cuts << endl;
}

