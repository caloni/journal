#include <windows.h>
#include <dbghelp.h>
#include <iostream>
#include <shlwapi.h>

#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "shlwapi.lib")


using namespace std;


HANDLE proc;

BOOL CALLBACK EnumSymbolsCallback(
  __in      PSYMBOL_INFO pSymInfo,
  __in      ULONG SymbolSize,
  __in_opt  PVOID UserContext
)
{
    cout << pSymInfo->Name << endl;
    return TRUE;
}


int main(int argc, char* argv[])
{
    if( argc == 2 )
    {
        DWORD pid = atoi(argv[1]);
        proc = OpenProcess(PROCESS_CREATE_THREAD | 
            PROCESS_QUERY_INFORMATION |
            PROCESS_VM_OPERATION |
            PROCESS_VM_WRITE |
            PROCESS_VM_READ,
            TRUE,
            pid);

        if( proc != INVALID_HANDLE_VALUE )
        {
            if( SymInitialize(proc, NULL, TRUE) )
                cout << "ok\n";
            else
                cout << "error " << GetLastError() << endl;

            if( SymEnumSymbols(proc, NULL, "EvenOdd!IsEvenUpdate*", EnumSymbolsCallback, NULL) )
            {
                cout << "ok\n";
            }
            else
            {
                cout << "error " << GetLastError() << endl;
            }

            CloseHandle(proc);
        }
    }
}
