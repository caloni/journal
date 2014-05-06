#include <boost\bimap.hpp>
#include <boost\bimap\multiset_of.hpp>
#include <string>

using namespace std;

typedef boost::bimap<std::string, boost::bimaps::multiset_of<int>> MBimap;
typedef boost::bimap<std::string, boost::bimaps::multiset_of<int>>::value_type vMBimap;

int main()
{
    MBimap _bimap;

    _bimap.insert(vMBimap("Um", 1));
    _bimap.insert(vMBimap("UmFull", 1));
    _bimap.insert(vMBimap("Dois", 2));
    _bimap.insert(vMBimap("Três", 3));

    string ret;
    std::pair<MBimap::right_iterator, MBimap::right_iterator> p = _bimap.right.equal_range(4);
	while (p.first != p.second)
	{
		MBimap::right_iterator ri = p.first;
		ret = ri->get_left();
		++p.first;
	}
}

