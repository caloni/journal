#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

using namespace std;

typedef map<string, string> Quaternion;
Quaternion g_quat;

string MultString(string s, int m)
{
	string ret;
	for (int i = 0; i < m; ++i)
		ret += s;
	return ret;
}

void BuildQuartenion()
{
	g_quat["11"] = "1";
	g_quat["i1"] = "i";
	g_quat["j1"] = "j";
	g_quat["k1"] = "k";
	g_quat["1i"] = "i";
	g_quat["ii"] = "-1";
	g_quat["ji"] = "-k";
	g_quat["ki"] = "j";
	g_quat["1j"] = "j";
	g_quat["ij"] = "k";
	g_quat["jj"] = "-1";
	g_quat["kj"] = "-i";
	g_quat["1k"] = "k";
	g_quat["ik"] = "-j";
	g_quat["jk"] = "i";
	g_quat["kk"] = "-1";
}

string Calc(string s)
{
	if (s.size() == 0) return "";
	if (s.size() == 1) return s;

	bool sign = false;
	if (s[0] == '-')
	{
		sign = true;
		if (s.size() == 2) 
			return s;
		s.erase(0, 1);
	}

	string sub = s.substr(0, 2);
	s.erase(0, 2);
	string result = g_quat[sub];
	if (sign && result.size() == 2)
		result.erase(0, 1);
	if (result == "1")
		result = sub.substr(0, 1);
	if (result == "-1")
		result = "-" + sub.substr(0, 1);

	return Calc(result + s);
}

bool k(string str)
{
	if (str.size() < 1) return false;

	for (size_t off = 0; off < str.size(); ++off)
	{
		string calc = Calc(str.substr(0, off+1));
		if (calc == "k")
		{
			return true;
		}
	}

	return false;
}

bool jk(string str)
{
	if (str.size() < 2) return false;

	for (size_t off = 0; off < str.size(); ++off)
	{
		string calc = Calc(str.substr(0, off+1));
		if (calc == "j")
		{
			bool rest = k(str.substr(off+1));
			if (rest)
			{
				return true;
			}
		}
	}

	return false;
}

bool ijk(string str)
{
	if (str.size() < 3) return false;
	if (str == "ijk") return true;

	for (size_t off = 0; off < str.size(); ++off)
	{
		string calc = Calc(str.substr(0, off+1));
		if( calc == "i")
		{
			bool rest = jk(str.substr(off+1));
			if (rest)
			{
				return true;
			}
		}
	}

	return false;
}

int main()
{
	BuildQuartenion();

	int T;
	cin >> T;

	for (int t = 1; t <= T; ++t)
	{
		int chars;
		int mult;
		string str;
		cin >> chars;
		cin >> mult;
		cin >> str;

		str = MultString(str, mult);

		if (ijk(str))
			cout << "Case #" << t << ": YES" << endl;
		else
			cout << "Case #" << t << ": NO" << endl;
	}
}
