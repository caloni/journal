// StaticLibraryCLang.cpp : Defines the exported functions for the static library.
//

#include "StaticLibraryCLang.h"

// This is an example of an exported variable
int nStaticLibraryCLang=0;

// This is an example of an exported function.
int fnStaticLibraryCLang(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see StaticLibraryCLang.h for the class definition
CStaticLibraryCLang::CStaticLibraryCLang()
{
    return;
}
