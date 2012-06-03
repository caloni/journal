// MyEncrypter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>


using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    char toenc[100];
    char result[100];
    char desenc[100];

    cout << "Digite o que deseja encriptar: ";
    cin >> toenc;

    size_t sz = strlen(toenc);

    for( size_t i = 0; i < sz; ++i )
    {
        char c = 0;

        c = toenc[i];
        c = c - 0x0B;
        result[i] = c;
    }
    result[sz] = 0; // final da string

    for( size_t i = 0; i < sz; ++i )
    {
        char c = 0;

        c = result[i];
        c = c + 0x0B;
        desenc[i] = c;
    }
    desenc[sz] = 0; // final da string

    return 0;
}
