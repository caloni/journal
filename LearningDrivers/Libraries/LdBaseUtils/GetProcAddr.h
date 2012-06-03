/**********
******
***         GetProcAddr.h
**
**          Header do m�dulo de obten��o de endere�os
**          APIs de Kernel Mode dinamicamente
**
**          Fernando Roberto da Silva
**          fernando@driverentry.com.br
*/

#include <ntddk.h>

#undef ExAllocatePool
#undef ExFreePool

typedef NTSTATUS    (NTAPI *NTPROC)();
typedef NTPROC      *PNTPROC;

NTSTATUS RtlGetProcAddress(IN PVOID     pBaseAddress,
                           IN LPCSTR    pszFunctionName,
                           IN NTPROC    *pProcedure);

NTSTATUS RtlGetModuleBase(IN LPCSTR     pszModuleName,
                          OUT PVOID*    ppBaseAddress);

