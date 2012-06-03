
void For();
void ForEach();
void ForSum();
void ForEachSum();
void Sort();


void EvenOrOdd(int number);

struct Sum
{
	Sum(int& _total) : total(_total) {}
	
	void operator () (int number)
	{
		total += number;
	}
	
	int& total;
};
