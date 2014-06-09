#include "base32.c"
#include <iostream>
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
	static unsigned __int64 increment = 0;
	static unsigned __int64 currentID = 0;

	FILETIME ft;
	ULARGE_INTEGER ll;
	GetSystemTimeAsFileTime (&ft);
	ll.LowPart = ft.dwLowDateTime;
	ll.HighPart = ft.dwHighDateTime;

	unsigned __int64 line64 = (unsigned __int64) ll.QuadPart;
	unsigned __int64 line64Off = GetFileTimeOff();
	unsigned __int64 smallLine64 = ( line64 - line64Off ) / 10000; // de 100 nano para 1 mili

	if( smallLine64 != currentID )
	{
		currentID = smallLine64;
		increment = 0;
	}
	else ++increment;

	unsigned __int64 inverted = BinaryInvert(currentID + increment);
	char encoded[200] = { };
	size_t sz = BinaryCountNonZeroBytes(inverted);
	unsigned __int64 invertedWithOff = (inverted >> 8) | sz;
	size_t offset = sizeof(inverted) - sz + 1;
	base32_encode((unsigned char*) &inverted + offset, (int) sz + 1, (unsigned char*) encoded, 200);

	string ret = encoded;
	if( ret[ret.size()-1] == 'A' )
		int x = 0;
	return encoded;
}


string UniqueIDDate(const string& id)
{
	unsigned __int64 decoded = 0;
	base32_decode((unsigned char*) id.c_str(), (unsigned char*) &decoded, 200);
	size_t off = (decoded >> (sizeof(decoded) - 1) * 8);
	unsigned __int64 shifted = decoded << (off * 8);
	unsigned __int64 diverted = BinaryInvert(shifted) * 10000; // de 1 mili para 100 nano
	unsigned __int64 timeOff = GetFileTimeOff();
	unsigned __int64 divertedPlusOff = diverted + timeOff;

	ULARGE_INTEGER ll = { };
	ll.QuadPart = (ULONGLONG) divertedPlusOff;
	FILETIME ft = { };
	ft.dwLowDateTime = ll.LowPart;
	ft.dwHighDateTime = ll.HighPart;
	SYSTEMTIME st = { };
	FILETIME lft = { };
	FileTimeToLocalFileTime(&ft, &lft);
	FileTimeToSystemTime(&ft, &st);

	if( st.wYear == 1980 )
		int x = 0;

	ostringstream os;
	os << st.wYear << "-" << st.wMonth << "-" << st.wDay
		<< " " << st.wHour << ":" << st.wMinute 
		<< ":" << st.wSecond << ":" << st.wMilliseconds;
	return os.str();
}


//int main()
//{
//	for( int i = 0; i < 100; ++i )
//	{
//		string id = GenerateUniqueID();
//		cout << id << ": " << UniqueIDDate(id) << endl;
//	}
//}
