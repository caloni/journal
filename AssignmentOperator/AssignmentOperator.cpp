#include <Windows.h>
#include <string>
#include <iostream>

using namespace std;


template<typename LoadLibraryFunction, typename FreeLibraryFunction>
class LibraryHandle
{
public:

	LibraryHandle(LoadLibraryFunction loadFunc, FreeLibraryFunction freeFunc)
		: m_loadFunc(loadFunc), m_freeFunc(freeFunc), m_library(nullptr) { }

	~LibraryHandle()
	{
		if( IsValid() )
			m_freeFunc(m_library);
	}

	LibraryHandle(const LibraryHandle& rhs)
	{
		m_loadFunc = nullptr;
		m_freeFunc = nullptr;
		m_library = nullptr;
		*this = rhs;
	}

	LibraryHandle& operator = (const LibraryHandle& rhs)
	{
		if( IsValid() )
			m_freeFunc(m_library);
		m_loadFunc = rhs.m_loadFunc;
		m_freeFunc = rhs.m_freeFunc;
		if( rhs.IsValid() )
		{
			m_library = m_loadFunc(rhs.m_libName.c_str());
			if( IsValid() )
				m_libName = rhs.m_libName;
		}
		return *this;
	}

	bool Load(const char* libName)
	{
		m_library = m_loadFunc(libName);
		if( IsValid() )
		{
			m_libName = libName;
			return true;
		}
		return false;
	}

	bool IsValid() const
	{
		return m_library != nullptr;
	}

	void* m_library;
	std::string m_libName;
	LoadLibraryFunction m_loadFunc;
	FreeLibraryFunction m_freeFunc;
};

typedef HMODULE (WINAPI *LoadLibraryFunctionWindows)(const char*);
typedef BOOL (WINAPI *FreeLibraryFunctionWindows)(void*);
typedef LibraryHandle<LoadLibraryFunctionWindows, FreeLibraryFunctionWindows> LibraryHandleWindows;

LibraryHandleWindows CreateLibraryHandle()
{
	LibraryHandleWindows ret((LoadLibraryFunctionWindows) LoadLibraryA, (FreeLibraryFunctionWindows) FreeLibrary);
	return ret;
}


int main()
{
	LibraryHandleWindows libHandle = CreateLibraryHandle();
	if( libHandle.Load("KERNEL32") )
	{
		cout << "Load OK\n";
	}
}
