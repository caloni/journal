#include <windows.h>
#include <iostream>

using namespace std;


void func(int count)
{
    int x = 0;

    for( int i = 0; i < count * 1000; ++i )
    {
        x += i;
        x *= x;
    }
}


void PrintThreadTimes(FILETIME& kernelTime1, FILETIME& kernelTime2, FILETIME& userTime1, FILETIME& userTime2)
{
    __int64 kt1 = ( __int64(kernelTime1.dwHighDateTime) << 32 ) | kernelTime1.dwLowDateTime;
    __int64 kt2 = ( __int64(kernelTime2.dwHighDateTime) << 32 ) | kernelTime2.dwLowDateTime;

    __int64 ut1 = ( __int64(userTime1.dwHighDateTime) << 32 ) | userTime1.dwLowDateTime;
    __int64 ut2 = ( __int64(userTime2.dwHighDateTime) << 32 ) | userTime2.dwLowDateTime;

    __int64 kt = kt2 - kt1;
    __int64 ut = ut2 - ut1;
    cout << "Kernel: " << kt << endl;
    cout << "User: " << ut << endl;
}


int main()
{
    FILETIME startTime, endTime;
    FILETIME kernelTime1, kernelTime2;
    FILETIME userTime1, userTime2;
    BOOL result = TRUE;
    
    result = result && GetThreadTimes(GetCurrentThread(), &startTime, &endTime, &kernelTime1, &userTime1);
    func(100);
    result = result && GetThreadTimes(GetCurrentThread(), &startTime, &endTime, &kernelTime2, &userTime2);
    PrintThreadTimes(kernelTime1, kernelTime2, userTime1, userTime2);

    result = result && GetThreadTimes(GetCurrentThread(), &startTime, &endTime, &kernelTime1, &userTime1);
    func(1000);
    result = result && GetThreadTimes(GetCurrentThread(), &startTime, &endTime, &kernelTime2, &userTime2);
    PrintThreadTimes(kernelTime1, kernelTime2, userTime1, userTime2);

    result = result && GetThreadTimes(GetCurrentThread(), &startTime, &endTime, &kernelTime1, &userTime1);
    func(10000);
    result = result && GetThreadTimes(GetCurrentThread(), &startTime, &endTime, &kernelTime2, &userTime2);
    PrintThreadTimes(kernelTime1, kernelTime2, userTime1, userTime2);

    return result ? 0 : 1;
}
