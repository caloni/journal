// H2HC.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <string.h>


int _tmain(int argc, _TCHAR* argv[]);


void RunShellCode()
{
    printf("ShellCode surprise!");
}


typedef void (*ShellCodeFunction)();

void PrintName()
{
    static char shellCode[] = "Caloni0123456789" "$$$$" "RRRR";

    char name[10];
    printf("Please tell me your name: ");

    // prepare shell code
    if( void* sc = strstr(shellCode, "RRRR") )
    {
        int (*fn)(int , _TCHAR* []) = _tmain;
        memcpy(sc, fn, sizeof(void*));
    }
    if( void* sc = strstr(shellCode, "$$$$") )
    {
        void (*fn)() = RunShellCode;
        memcpy(sc, fn, sizeof(void*));
    }

    //gets(name);
    memcpy(name, shellCode, sizeof(shellCode));

    printf("Thanks, %s!\n", name);
}


int _tmain(int argc, _TCHAR* argv[])
{
    PrintName();
    return 0;
}

