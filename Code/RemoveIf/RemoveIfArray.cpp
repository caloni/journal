void RemoveIfLolita(int* ages, int size)
{
	remove_if(ages, ages + size, [&](int age) { return age < 18; } );
}