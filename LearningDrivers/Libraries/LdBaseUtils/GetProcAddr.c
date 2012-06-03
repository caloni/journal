/**********
******
***         GetProcAddr.c
**
**          Modulo que implementa a obtenção de endereços
**          de APIs de Kernel Mode dinamicamente.
**
**          Fernando Roberto da Silva
**          fernando@driverentry.com.br
*/

#include "GetProcAddr.h"
#include <windef.h>

#pragma pack(push, 1)

//-f--> Todas estas definições foram trazidas do winnt.h

typedef struct _IMAGE_DOS_HEADER        // DOS .EXE header
{
    WORD   e_magic;                     // Magic number
    WORD   e_cblp;                      // Bytes on last page of file
    WORD   e_cp;                        // Pages in file
    WORD   e_crlc;                      // Relocations
    WORD   e_cparhdr;                   // Size of header in paragraphs
    WORD   e_minalloc;                  // Minimum extra paragraphs needed
    WORD   e_maxalloc;                  // Maximum extra paragraphs needed
    WORD   e_ss;                        // Initial (relative) SS value
    WORD   e_sp;                        // Initial SP value
    WORD   e_csum;                      // Checksum
    WORD   e_ip;                        // Initial IP value
    WORD   e_cs;                        // Initial (relative) CS value
    WORD   e_lfarlc;                    // File address of relocation table
    WORD   e_ovno;                      // Overlay number
    WORD   e_res[4];                    // Reserved words
    WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
    WORD   e_oeminfo;                   // OEM information; e_oemid specific
    WORD   e_res2[10];                  // Reserved words
    LONG   e_lfanew;                    // File address of new exe header

} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;


typedef struct _IMAGE_DATA_DIRECTORY
{
    DWORD   VirtualAddress;
    DWORD   Size;

} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16

//
// Optional header format.
//

typedef struct _IMAGE_OPTIONAL_HEADER 
{
    //
    // Standard fields.
    //

    WORD    Magic;
    BYTE    MajorLinkerVersion;
    BYTE    MinorLinkerVersion;
    DWORD   SizeOfCode;
    DWORD   SizeOfInitializedData;
    DWORD   SizeOfUninitializedData;
    DWORD   AddressOfEntryPoint;
    DWORD   BaseOfCode;
    DWORD   BaseOfData;

    //
    // NT additional fields.
    //

    DWORD   ImageBase;
    DWORD   SectionAlignment;
    DWORD   FileAlignment;
    WORD    MajorOperatingSystemVersion;
    WORD    MinorOperatingSystemVersion;
    WORD    MajorImageVersion;
    WORD    MinorImageVersion;
    WORD    MajorSubsystemVersion;
    WORD    MinorSubsystemVersion;
    DWORD   Win32VersionValue;
    DWORD   SizeOfImage;
    DWORD   SizeOfHeaders;
    DWORD   CheckSum;
    WORD    Subsystem;
    WORD    DllCharacteristics;
    DWORD   SizeOfStackReserve;
    DWORD   SizeOfStackCommit;
    DWORD   SizeOfHeapReserve;
    DWORD   SizeOfHeapCommit;
    DWORD   LoaderFlags;
    DWORD   NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];

} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

typedef IMAGE_OPTIONAL_HEADER32             IMAGE_OPTIONAL_HEADER;


//
// Export Format
//

typedef struct _IMAGE_EXPORT_DIRECTORY
{
    DWORD   Characteristics;
    DWORD   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    DWORD   Name;
    DWORD   dwBase;
    DWORD   NumberOfFunctions;
    DWORD   NumberOfNames;
    DWORD   AddressOfFunctions;     // RVA from dwBase of image
    DWORD   AddressOfNames;         // RVA from dwBase of image
    DWORD   AddressOfNameOrdinals;  // RVA from dwBase of image

} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;


#define IMAGE_DIRECTORY_ENTRY_EXPORT        0   // Export Directory


//-f--> As definições abaixo foram trazidas do livro
//      "Windows NT/2000 Native API reference"

typedef enum _SYSTEM_INFORMATION_CLASS
{
    SystemModuleInformation = 11

} SYSTEM_INFORMATION_CLASS;


typedef struct _SYSTEM_MODULE_INFORMATION   // Information Class 11
{
    ULONG Reserved[2];
    PVOID Base;
    ULONG Size;
    ULONG Flags;
    USHORT Index;
    USHORT Unknown;
    USHORT LoadCount;
    USHORT ModuleNameOffset;
    CHAR ImageName[256];

} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;


NTSTATUS
NTAPI
ZwQuerySystemInformation(IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
                         IN OUT PVOID SystemInformation,
                         IN ULONG SystemInformationLength,
                         OUT PULONG ReturnLength OPTIONAL);

#pragma pack(pop)


/****
***
**           RtlGetProcAddress
**
*/
NTSTATUS RtlGetProcAddress(IN PVOID     pBaseAddress,
                           IN LPCSTR    pszFunctionName,
                           IN NTPROC    *pProcedure)
{
    HANDLE                      hMod;
    IMAGE_DOS_HEADER*           DosHeader;
    IMAGE_OPTIONAL_HEADER*      OptHeader;
    IMAGE_EXPORT_DIRECTORY*     pExportTable;
    DWORD*                      dwArrayOfFunctionAddresses;
    DWORD*                      dwArrayOfFunctionNames;
    WORD*                       dwArrayOfFunctionOrdinals;
    DWORD                       dwFunctionOrdinal, dwIdx;
    STRING                      ntFunctionName, ntFunctionNameSearch;
    NTSTATUS                    nts = STATUS_OBJECT_NAME_NOT_FOUND;

    __try
    {
        hMod = pBaseAddress;

        DosHeader = (IMAGE_DOS_HEADER*)hMod;
        OptHeader =(IMAGE_OPTIONAL_HEADER*)((BYTE*)hMod + DosHeader->e_lfanew + 24);
        pExportTable =(IMAGE_EXPORT_DIRECTORY*)((BYTE*) hMod + OptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

        dwArrayOfFunctionAddresses = (DWORD*)((BYTE*)hMod + pExportTable->AddressOfFunctions);
        dwArrayOfFunctionNames = (DWORD*)((BYTE*)hMod + pExportTable->AddressOfNames);
        dwArrayOfFunctionOrdinals = (WORD*)((BYTE*)hMod + pExportTable->AddressOfNameOrdinals);

        RtlInitString(&ntFunctionNameSearch,
            pszFunctionName);

        for(dwIdx = 0; dwIdx < pExportTable->NumberOfNames; dwIdx++)
        {
            RtlInitString(&ntFunctionName,
                (PCHAR)((BYTE*)hMod + dwArrayOfFunctionNames[dwIdx]));

            if (RtlCompareString(&ntFunctionName,
                &ntFunctionNameSearch, TRUE) == 0) 
            {
                dwFunctionOrdinal = dwArrayOfFunctionOrdinals[dwIdx];
                *pProcedure = (NTPROC)((BYTE*)hMod + dwArrayOfFunctionAddresses[dwFunctionOrdinal]);
                nts = STATUS_SUCCESS;
                break;
            }
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        //-f--> Se estivermos em DEBUG vamos chamar a atenção para este fato
        ASSERT(FALSE);
        nts = GetExceptionCode();
    }

    return nts;
}


/****
***
**           RtlGetModuleBase
**
*/
NTSTATUS RtlGetModuleBase(IN LPCSTR     pszModuleName,
                          OUT PVOID*    ppBaseAddress)
{
    NTSTATUS                    nts, ntsInternal;
    ULONG                       ulLength;
    PSYSTEM_MODULE_INFORMATION  pModuleInfo;
    PVOID                       pBuffer = NULL;

    __try
    {
        //-f--> Por motivos religiosos, temos que passar algum ponteiro
        //      no segundo parâmetro da chamada abaixo, mesmo que o número
        //      de bytes a serem utilizados seja zero. Caso contrário,
        //      se passarmos NULL, obteremos STATUS_ACCESS_VIOLATION.
        nts = ZwQuerySystemInformation(SystemModuleInformation,
                                       &ulLength,
                                       0,
                                       &ulLength);

        if (nts != STATUS_INFO_LENGTH_MISMATCH)
        {
            ASSERT(FALSE);
            return nts;
        }

        //-f--> Agora alocamos o buffer necessário para trazer
        //      todos od modulos
        if (!(pBuffer = ExAllocatePool(PagedPool, ulLength)))
        {
            ASSERT(FALSE);
            return STATUS_NO_MEMORY;
        }

        //-f--> Passamos o buffer completo desta vez
        nts = ZwQuerySystemInformation(SystemModuleInformation,
                                       pBuffer,
                                       ulLength,
                                       &ulLength);
        if (NT_SUCCESS(nts))
        {
            nts = STATUS_OBJECT_NAME_NOT_FOUND;

            //-f--> A primeira coisa que recebemos no buffer é a
            //      quantidade de estruturas existem no array que
            //      segue a frente.
            ulLength = *(PULONG)pBuffer;
            pModuleInfo = (PSYSTEM_MODULE_INFORMATION)((PULONG)pBuffer + 1);

            while(ulLength--)
            {
                //-f--> Procuramos pelo módulo que nos interessa
                if (!_stricmp(&pModuleInfo[ulLength].ImageName[pModuleInfo[ulLength].ModuleNameOffset],
                    pszModuleName))
                {
                    //-f--> Precisamos apenas do Endereço Base
                    *ppBaseAddress = pModuleInfo[ulLength].Base;
                    nts = STATUS_SUCCESS;
                    break;
                }
            }
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        //-f--> Se estivermos em DEBUG vamos chamar a atenção para este fato
        ASSERT(FALSE);
        nts = GetExceptionCode();
    }

    //-f--> Libera buffer utilizado na pesquisa
    if (pBuffer)
        ExFreePool(pBuffer);

    return nts;
}
