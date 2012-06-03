#include "Algorithm.h"
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;


void PopulateNumbers(vector<int>& numbers)
{
	int currNumber;
	
	cout << "Type numbers of 0 to end" 
		<< endl;
	while( cin >> currNumber )
	{
		if( currNumber )
			numbers.push_back(currNumber);
		else
			break;
	}
}


void For()
{
	vector<int> numbers;
	PopulateNumbers(numbers);
	for( size_t i = 0; 
		i < numbers.size();
		++i )
	{
		EvenOrOdd(numbers[i]);
	}
}


void ForEach()
{
	vector<int> numbers;
	PopulateNumbers(numbers);
	for_each(numbers.begin(), 
		numbers.end(), 
		EvenOrOdd);
}


void ForSum()
{
	vector<int> numbers;
	int total = 0;
	Sum sum(total);
	PopulateNumbers(numbers);
	for( size_t i = 0; 
		i < numbers.size();
		++i )
	{
		sum(numbers[i]);
	}
	cout << "Total: " << total << endl;
}


void ForEachSum()
{
	vector<int> numbers;
	int total = 0;
	PopulateNumbers(numbers);
	
	for_each(numbers.begin(), 
		numbers.end(), 
		Sum(total));
	
	cout << "Total: " << total << endl;
}
