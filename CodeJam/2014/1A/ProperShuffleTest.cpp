#include "CodeJam.h"
#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <numeric>
#include <stdlib.h>
#include <time.h>

using namespace std;


static bool sccfirst;
static double scc, sccun, sccu0, scclast, scct1, scct2, scct3;
static long totalc = 0; 	   /* Total bytes counted */

void rt_init()
{
    sccfirst = true;	       /* Mark first time for serial correlation */
    scct1 = scct2 = scct3 = 0.0; /* Clear serial correlation terms */
    totalc = 0;
}


void rt_add(int c)
{
	  totalc++;
	/* Update calculation of serial correlation coefficient */
	sccun = c;
	if (sccfirst) {
	    sccfirst = false;
	    scclast = 0;
	    sccu0 = sccun;
	} else {
	    scct1 = scct1 + scclast * sccun;
	}
	scct2 = scct2 + sccun;
	scct3 = scct3 + (sccun * sccun);
	scclast = sccun;
}


void rt_end(double *r_scc)
{
    /* Complete calculation of serial correlation coefficient */
    scct1 = scct1 + scclast * sccu0;
    scct2 = scct2 * scct2;
    scc = totalc * scct3 - scct2;
    if (scc == 0.0) {
       scc = -100000;
    } else {
       scc = (totalc * scct1 - scct2) / scc;
    }
    *r_scc = scc;
}


vector<int> Sequence;


void GoodAlg(vector<int>& seq)
{
	int N = (int) seq.size();
	for( int i = 0; i < N; ++i )
	{
		int p = (rand() % (N - i)) + i;
		swap(seq[i], seq[p]);
	}
}

void BadAlg(vector<int>& seq)
{
	int N = (int) seq.size();
	for( int i = 0; i < N; ++i )
	{
		int p = rand() % N;
		swap(seq[i], seq[p]);
	}
}

double AvgTest(vector<int>& seq)
{
	int badPart = seq.size() / 3;
	double badAvg = (double) accumulate(seq.begin(), seq.begin() + badPart, 0) / (double) distance(seq.begin(), seq.begin() + badPart);
	return badAvg;
}

double StdDev(vector<int>::iterator beg, vector<int>::iterator end)
{
	double avg = (double) accumulate(beg, end, 0) / (double) distance(beg, end);
	double variance = 0.0;
	for( auto it = beg; it != end; ++it )
		variance += pow(avg - (double) *it, 2);
	variance = variance / distance(beg, end);
	double stdDev = sqrt(variance);
	return stdDev;
}

double CheckAlg(vector<int>& seq)
{
	double ret = 0.0;
	rt_init();
	for( int i = 0; i < (int) seq.size(); ++i )
		rt_add(seq[i]);
	rt_end(&ret);
	return ret;

	//double avg = 0.0, begAvg= 0.0, endAvg = 0.0;
	//int greater = 0, lesser = 0;

	//auto beg = seq.begin();
	//auto end = seq.end() - 1;
	//while( beg < end )
	//{
	//	if( begAvg )
	//	{
	//		double oldAvg = begAvg;
	//		begAvg = (begAvg + *beg) / 2;
	//	}
	//	else begAvg = *beg;

	//	if( endAvg )
	//	{
	//		double oldAvg = endAvg;
	//		endAvg = (endAvg + *end) / 2;
	//	}
	//	else endAvg = *end;

	//	if( begAvg < endAvg )
	//		lesser++;
	//	else
	//		greater++;
	//	++beg;
	//	--end;
	//	//if( avg )
	//	//{
	//	//	double oldAvg = avg;
	//	//	avg = (avg + seq[i]) / 2;
	//	//	if( oldAvg < avg )
	//	//		lesser++;
	//	//	else
	//	//		greater++;
	//	//}
	//	//else avg = seq[i];
	//}
	//return greater - lesser;

	//int beg = accumulate(seq.begin(), seq.begin() + seq.size() / 2, 0);
	//int end = accumulate(seq.begin() + seq.size() / 2, seq.end(), 0);
	//return end - beg;

	//static double checkStdDev = 1000 / sqrt(12.0);
	//double stdDev = StdDev(seq.begin(), seq.begin() + seq.size() / 2);
	//return stdDev;

	//int badPart = seq.size() / 7;
	//double totalAvg = (double) accumulate(seq.begin(), seq.end(), 0) / (double) seq.size();
	//double badAvg = (double) accumulate(seq.begin(), seq.begin() + badPart, 0) / (double) distance(seq.begin(), seq.begin() + badPart);
	//double endAvg = (double) accumulate(seq.end() - badPart, seq.end(), 0) / (double) distance(seq.end() - badPart, seq.end());
	//double remainAvg = (double) accumulate(seq.begin() + badPart, seq.end(), 0) / (double) distance(seq.begin() + badPart, seq.end());
	//double totalStdDev = StdDev(seq.begin(), seq.end());
	//double badStdDev = StdDev(seq.begin(), seq.begin() + badPart);
	////bool bad = badAvg < endAvg && badAvg < remainAvg && badAvg < totalAvg;
	//bool bad = badStdDev < totalStdDev;

	//if( bad ) // é ruim, mas pode ser bom...
	//{
	//	//int badCount = 0, goodCount = 0;
	//	//double avg = 0.0;

	//	//for( int i = badPart + 1; i < (int) seq.size(); ++i )
	//	//{
	//	//	if( avg )
	//	//	{
	//	//		avg = double(avg + seq[i]) / 2.0;
	//	//		if( avg < totalAvg )
	//	//			badCount++;
	//	//		else
	//	//			goodCount++;
	//	//	}
	//	//	else avg = seq[i];
	//	//}
	//	//
	//	//bad = badCount > goodCount;

	//	//int badCount = 0;
	//	//int totalParts = seq.size() / badPart;
	//	//for( int p = 1; p < totalParts; ++p )
	//	//{
	//	//	auto beg = seq.begin() + (p*badPart);
	//	//	auto end = seq.begin() + ((p+1)*badPart);
	//	//	double partAvg = (double) accumulate(beg, end, 0) / (double) distance(beg, end);
	//	//	if( badAvg < totalAvg )
	//	//		badCount += 1;
	//	//}
	//	//if( badCount < totalParts / 2 )
	//	//	bad = false;
	//}

	//return ! bad;
}

void TestCase(std::istream& is, std::ostream& os)
{
	void (*algs[])(vector<int>& seq) = { GoodAlg, BadAlg };
	for( int i = 0; i < 200; ++i )
		Sequence.push_back(i);
	srand((unsigned int)time(0));

	for( size_t a = 0; a < 2; ++a )
	{
		int good = 0, bad = 0;
		vector<double> badAvgs;
		vector<double> cmpAvgs;
		vector<int> freq(Sequence.size());

		os << "\nAlg " << a << endl;

		for( int i =0 ; i < 120; ++i )
		{
			vector<int> seq = Sequence;
			algs[a](seq);
			double a = CheckAlg(seq);
			cmpAvgs.push_back(a);
			a >= 0 ? ++good : ++bad;
			os << a << endl;
			//os << (b ? ("GOOD") : ("BAD")) << ' ';
			//badAvgs.push_back(AvgTest(seq));
			for( int j = 0; j < (int) seq.size(); ++j )
			{
				//freq[j] += seq[j];
				//os << seq[j] << ' ';
			}
			//os << endl;
		}
	
		//double totalAvg = (double) accumulate(freq.begin(), freq.end(), 0) / (double) freq.size();
		//os << "TotalAvg: ";
		//for( int i = 0; i < (int) freq.size(); ++i )
			//os << freq[i] - totalAvg << ' ';
		//os << endl;
	
		//os << "TotalSum: ";
		//for( int i = 0; i < (int) freq.size(); ++i )
		//	os << freq[i] << ' ';
		//os << endl;

		//os << "CmpAvgs: ";
		//for( int i = 0; i < (int) cmpAvgs.size(); ++i )
		//	os << cmpAvgs[i] << ' ';
		//os << endl;

		//os << "Goods: " << good << " Bads: " << bad << endl;

		//os << "BadAvgs: ";
		//for( int i = 0; i < (int)badAvgs.size(); ++i )
			//os << badAvgs[i] << ' ';
		//os << endl;
	}	
}
