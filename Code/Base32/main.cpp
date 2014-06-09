#include "base32.c"
#include <time.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <Windows.h>

using namespace std;


template<typename T>
T BinaryInvert(T x)
{
	const size_t SizeOfT = sizeof(T);
	T y;
	unsigned char *dst = (unsigned char *)&y;
	unsigned char *src = (unsigned char *)&x;

	for( size_t i = 0, j = SizeOfT - 1; i < SizeOfT; ++i, --j )
		dst[i] = src[j];

	return y;
}


template<typename T>
size_t BinaryCountNonZeroBytes(T x)
{
	const size_t SizeOfT = sizeof(T);
    size_t sz = SizeOfT;
	unsigned char *src = (unsigned char *)&x;

	for( size_t i = 0; i < SizeOfT && src[i] == 0; ++i, --sz )
        ;

	return sz;
}


template<typename T>
size_t BinaryCountNonZeroBytesInverted(T x)
{
	const size_t SizeOfT = sizeof(T);
    size_t sz = 0;
	unsigned char *src = (unsigned char *)&x;

	for( size_t i = SizeOfT - 1; i && src[i] == 0; --i, ++sz )
        ;

	return sz;
}


unsigned __int64 GetFileTimeOff()
{
	FILETIME ftOff = { };
	SYSTEMTIME st = { };
	st.wYear = 1979;
	st.wMonth = 12;
	st.wDay = 14;
	SystemTimeToFileTime(&st, &ftOff);
	ULARGE_INTEGER llOff;
	llOff.LowPart = ftOff.dwLowDateTime;
	llOff.HighPart = ftOff.dwHighDateTime;
	return (unsigned __int64) llOff.QuadPart;
}


string GenerateUniqueID()
{
	static size_t increment = 0;
	static size_t currentID = 0;

	size_t secs = (size_t) time(nullptr); // unix timestamp

	if( secs > currentID )
	{
		if( currentID + increment >= secs )
			currentID += increment + 1;
		else
			currentID = secs;

		increment = 0;
	}
	else ++increment;

	size_t newID = currentID + increment;
	char encoded[200] = { };
	base32_encode((unsigned char*) &newID, (int) sizeof(newID), (unsigned char*) encoded, (int) sizeof(encoded));

	return encoded;
}


string UniqueIDDate(const string& id)
{
	size_t decoded = 0;
	base32_decode((unsigned char*) id.c_str(), (unsigned char*) &decoded, (int) sizeof(decoded));
	time_t secs = (time_t) decoded;

	if( tm* plt = localtime(&secs) )
	{
		tm lt = *plt;
		ostringstream os;
		os << lt.tm_year + 1900 
			<< "-" << setw(2) << setfill('0') << lt.tm_mon + 1
			<< "-" << setw(2) << setfill('0') << lt.tm_mday
			<< " " << setw(2) << setfill('0') << lt.tm_hour
			<< ":" << setw(2) << setfill('0') << lt.tm_min 
			<< ":" << setw(2) << setfill('0') << lt.tm_sec;
		return os.str();
	}

	return "";
}


int main()
{
	for( int i = 0; i < 10; ++i )
	{
		string id = GenerateUniqueID();
		cout << id << ": " << UniqueIDDate(id) << endl;
	}

	Sleep(15 * 1000);

	for( int i = 0; i < 10; ++i )
	{
		string id = GenerateUniqueID();
		cout << id << ": " << UniqueIDDate(id) << endl;
	}
}
