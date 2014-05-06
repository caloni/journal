#include "../../pe.h"
#include <hresult.h>
#include <atlbase.h>
#include <atlfile.h>
#include <iostream>
#include <iomanip>

using namespace std;


void TestProcNames(PSTR dllName, HMODULE dll, PE& pe)
{
	for( DWORD i = 0; i < pe.GetNumberOfFunctions(); ++i )
	{
		FARPROC func = pe.GetProcAddressByIndex(i);
		PSTR procName = pe.GetProcName(func);

		if( procName )
		{
			FARPROC dllFunc = GetProcAddress(dll, procName);

			if( func != dllFunc )
			{
				cout << dllName << '!' << procName
					<< ": address mismatch ("
					<< " PEAddr: 0x" << hex << setw(8) << setfill('0') << func
					<< " APIAddr: 0x" << hex << setw(8) << setfill('0') << dllFunc
					<< ")\n";
			}
		}
	}
}


HRESULT TestDll(PSTR dllName)
{
	HResult hr;

	try
	{
		HMODULE dll;
		HRESULT hrTest;
		PE peTest;

		hrTest = peTest.Open(dllName);

		if( SUCCEEDED(hrTest) )
		{
			peTest.Close();
			dll = LoadLibrary(dllName);

			if( dll )
			{
				PE pe;
				hr = pe.Open(reinterpret_cast<PVOID>( dll ));
				TestProcNames(dllName, dll, pe);
				FreeLibrary(dll), dll = NULL;
			}
			else
			{
				cout << dllName << ": error " << GetLastError() << endl;
			}
		}
	}
	catch(HResult& e)
	{
		cout << dllName << ": error 0x" << hex << setw(8) << setfill('0') << e.m_hr 
		<< " analyzing \"" << dllName << "\"\n";
		hr.m_hr = e;
	}
	
	return hr;
}


int main(int argc, char* argv[])
{
	HResult hr;

	if( argc >= 3 && lstrcmpi(argv[1], "test") == 0 )
	{
		for( int i = 2; i < argc; ++i )
		{
			TestDll(argv[i]);
		}
	}
	else
	if( argc >= 3 )
	{
		try
		{
			HMODULE dll;
			PE pe;

			dll = LoadLibrary(argv[1]);
			
			if( dll )
			{
				hr = pe.Open(reinterpret_cast<PVOID>( dll ));

				FARPROC apiAddress = GetProcAddress(GetModuleHandle("kernel32"), "GetProcAddress");
				PVOID iatAddress = pe.GetIATFunctionAddress(apiAddress);

				for( int i = 2; i < argc; ++i )
				{
					FARPROC apiAddress = GetProcAddress(dll, argv[i]);
					FARPROC peAddress = pe.GetProcAddress(argv[i]);

					cout << "Function: " << argv[1]
						<< " PEAddr: 0x" << hex << setw(8) << setfill('0') << peAddress
						<< " APIAddr: 0x" << hex << setw(8) << setfill('0') << apiAddress
						<< endl;
				}

				FreeLibrary(dll), dll = NULL;
			}
		}
		catch(HResult& e)
		{
			cout << "Error 0x" << hex << setw(8) << setfill('0') << e.m_hr << endl;
		}
	}

	return (int) hr;
}
