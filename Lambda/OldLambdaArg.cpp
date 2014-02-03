
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
