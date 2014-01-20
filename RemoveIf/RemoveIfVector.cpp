void RemoveIfLolita(vector<int>& ages)
{
	remove_if(ages.begin(), ages.end(), [&](int age) { return age < 18; } );
}