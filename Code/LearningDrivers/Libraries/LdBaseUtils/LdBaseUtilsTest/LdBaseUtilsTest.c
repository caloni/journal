/** @file LdBaseUtilsTest.c
 *
 * @brief Inicio do driver de teste da biblioteca de utilitarios para drivers.
 *
 * @author Fernando Roberto da Silva <fernando@driverentry.com.br>
 * @date 2008-08
 */
#include "../GetProcAddr.h"


//-f--> Tipo para o ponteiro de função ExFreePoolWithTag
typedef VOID (NTAPI* PF_EX_FREE_POOL_WITH_TAG)
(
    IN PVOID  P,
    IN ULONG  Tag 
);



VOID OnDriverUnload(PDRIVER_OBJECT     pDriverObj)
{
    //-f--> Esta rotina está aqui apenas para permitir
    //      que o driver seja terminado, mesmo vazia.
}



/****
***
**           Era uma vez um Driver...
**
*/
NTSTATUS DriverEntry(PDRIVER_OBJECT     pDriverObj,
                     PUNICODE_STRING    pusRegistryPath)
{
    NTSTATUS                    nts;
    PVOID                       pBaseAddress, pTemp;
    PF_EX_FREE_POOL_WITH_TAG    pfExFreePoolWithTag;

    //-f--> Seta rotina de finalização
    pDriverObj->DriverUnload = OnDriverUnload;

    //-f--> Obtem o endereço base
    nts = RtlGetModuleBase("ntoskrnl.exe",
                           &pBaseAddress);

    //-f--> Testar retorno não mata ninguem, mas a falta
    //      pode matar seu sistema
    if (!NT_SUCCESS(nts))
        return nts;

    //-f--> Alocando memória para teste do Free
    pTemp = ExAllocatePoolWithTag(NonPagedPool,
                                  10,
                                  'tseT');

    //-f--> Obtem o endereço da API
    nts = RtlGetProcAddress(pBaseAddress,
                            "ExFreePoolWithTag",
                            (NTPROC*)&pfExFreePoolWithTag);

    if (NT_SUCCESS(nts))
    {
        //-f--> Se o sistema implementa esta API, então
        //      obteremos sucesso e poderemos chama-la
        pfExFreePoolWithTag(pTemp,
                            'tseT');
    }
    else
    {
        //-f--> Quem não tem cão, caça com gato.
        ExFreePool(pTemp);
    }

    return STATUS_SUCCESS;
}

