#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <dbghelp.h>
#include <iostream>
#include <shlwapi.h>

#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "shlwapi.lib")


using namespace std;


bool IsEven(int number);

HANDLE g_updateProc = INVALID_HANDLE_VALUE;
typedef bool (*FP_IsEven)(int);

FP_IsEven* IsEvenUpdatePtr = nullptr;
bool* IsEvenUpdateReadyPtr = nullptr;



BOOL CALLBACK EnumSymbolsCallback(
  __in      PSYMBOL_INFO pSymInfo,
  __in      ULONG SymbolSize,
  __in_opt  PVOID UserContext
)
{
    if( strcmp(pSymInfo->Name, "IsEvenUpdate") == 0 )
    {
        IsEvenUpdatePtr = (FP_IsEven*) pSymInfo->Address;
    }
    else if( strcmp(pSymInfo->Name, "IsEvenUpdateReady") == 0 )
    {
        IsEvenUpdateReadyPtr = (bool*) pSymInfo->Address;
    }
    return TRUE;
}


DWORD WINAPI RmThread(LPVOID lpParameter)
{
    cout << "** Sistema de atualizacao dinamica INICIO **\n";

    g_updateProc = OpenProcess(PROCESS_CREATE_THREAD | 
        PROCESS_QUERY_INFORMATION |
        PROCESS_VM_OPERATION |
        PROCESS_VM_WRITE |
        PROCESS_VM_READ,
        TRUE,
        GetCurrentProcessId());

    if( g_updateProc != INVALID_HANDLE_VALUE )
    {
        if( SymInitialize(g_updateProc, NULL, TRUE) )
        {
            cout << "Sucesso inicializando simbolos\n";

            // isso deve ser alterado para recursividade
            if( SymEnumSymbols(g_updateProc, NULL, "EvenOdd!IsEvenUpdate*", EnumSymbolsCallback, NULL) )
            {
                cout << "Sucesso enumerando simbolos\n";

                if( IsEvenUpdatePtr && IsEvenUpdateReadyPtr )
                {
                    cout << "Atualizando sistema...\n";
                    *IsEvenUpdatePtr = IsEven;
                    *IsEvenUpdateReadyPtr = true;
                }
                else cout << "Erro localizando informacoes de atualizacao no modulo original\n";
            }
            else cout << "Erro " << GetLastError() << " enumerando modulos\n";
        }
        else
            cout << "Erro " << GetLastError() << " inicializando simbolos\n";

        CloseHandle(g_updateProc);
    }
    else cout << "Erro " << GetLastError() << " abrindo processo\n";

    cout << "** Sistema de atualizacao dinamica FIM **\n";

    return 0;
}


BOOL APIENTRY DllMain(
   HANDLE hModule,
   DWORD ul_reason_for_call,
   LPVOID lpReserved
)
{
   switch( ul_reason_for_call )
   {
      case DLL_PROCESS_ATTACH:
      {
         DWORD dwThrId;
         CloseHandle(CreateThread(NULL, 
                                  0,
                                  RmThread,
                                  NULL,
                                  0, 
                                  &dwThrId));
      }
      break;
   }

   return TRUE;
}
