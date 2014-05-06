void RemoveIfLolita(int* ages, int size)
{
	remove_if(ages, ages + size, [&](int age) { return age < 18; } );
}

void RemoveIfLolita(vector<int>& ages)
{
	remove_if(ages.begin(), ages.end(), [&](int age) { return age < 18; } );
}
