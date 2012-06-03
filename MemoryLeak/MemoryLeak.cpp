#include "stdafx.h"
#include <iostream>
#include <string>
#include <windows.h> // só um threaded-teste...

using namespace std;

DWORD WINAPI AlocaMasNaoDevolveThread(PVOID pvBytes)
{
    __int64 largeBytes = (__int64) pvBytes;
    void* ret = nullptr;
    size_t bytes = (size_t) largeBytes; // assumo o risco de truncar

    if( bytes == largeBytes )
    {
        try
        {
            ret = new char[bytes];
        }
        catch(bad_alloc&)
        {
            cout << "Nao foi possivel alocar " << bytes << " bytes" << endl;
        }
    }
    return ERROR_SUCCESS;
}

void* AlocaMasNaoDevolve(__int64 largeBytes)
{
    void* ret = new char[0];
    CreateThread(NULL, 0, AlocaMasNaoDevolveThread, (PVOID) largeBytes, 0, NULL);
    //void* ret = nullptr;
    //size_t bytes = (size_t) largeBytes; // assumo o risco de truncar

    //if( bytes == largeBytes )
    //{
    //    try
    //    {
    //        ret = new char[bytes];
    //    }
    //    catch(bad_alloc&)
    //    {
    //        cout << "Nao foi possivel alocar " << bytes << " bytes" << endl;
    //    }
    //}

    return ret;
}

int _tmain(int argc, _TCHAR* argv[])
{
    __int64 totalBytes = 0;
    __int64 bytes = 0;

    do
    {
        cout << "Alocar quantos bytes? (zero para sair): ";
        cin >> bytes;

        void* address = AlocaMasNaoDevolve(bytes);

        if( address )
        {
            totalBytes += bytes;

            cout << "Alocados " << bytes << " bytes em " << address
                << " (total ate o momento: " << totalBytes << " bytes)\n";
        }
    }
    while( bytes );

    return int(totalBytes);
}
