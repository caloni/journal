
int oldlambda()
{
	struct F
	{
		int operator() (int x, int y)
		{
			return x + y;
		}
	}f;

	int result = f(2, 3);
	return result;
}


int lambda()
{
	auto f = [&](int x, int y) -> int
	{
		return x + y;
	};

	int result = f(2, 3);
	return result;
}


int main()
{
	oldlambda();
	lambda();
}
