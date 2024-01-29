#pragma once

// This is an example of a class exported from the StaticLibraryCLang.lib
class CStaticLibraryCLang
{
public:
    CStaticLibraryCLang();
    // TODO: add your methods here.
};

// This is an example of an exported variable
extern int nStaticLibraryCLang;

// This is an example of an exported function.
int fnStaticLibraryCLang(void);

template<typename... Args>
bool all(Args... args) { return (... && args); }
