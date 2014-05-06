#include "stdafx.h"
#include "SecondCpp.h"

void SecondCppLeak()
{
	int * leak = new int[1000];
}
