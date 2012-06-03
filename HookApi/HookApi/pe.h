/** @file
Classe utilit�ria para visualizar e editar a estrutura de um PE 
(Portable Executable) em mem�ria e a partir de um arquivo em disco.

@author Wanderley Caloni Jr <wanderley@caloni.com.br>
@date 10.2005

@sa http://www.microsoft.com/whdc/system/platform/firmware/PECOFF.mspx
*/
#pragma once
#include <windows.h>
#include <atlbase.h>
#include <atlfile.h>
#include <assert.h>


/** Classe utilit�ria para ver e editar um PE. */
class PE
{
public:
   /// Ponteiros para os headers mais acessados no PE.
   struct PEData
   {
      PIMAGE_DOS_HEADER dosHeader;
      PIMAGE_NT_HEADERS32 ntHeader;
      PIMAGE_OPTIONAL_HEADER optHeader;
      PIMAGE_DATA_DIRECTORY dirsHeader;
      PIMAGE_SECTION_HEADER sectHeader;
      PVOID dataBody;
   };

public:
   PE();

   /// Abre o PE a partir de um ponteiro (em mem�ria).
   HRESULT Open(PVOID base);

   /// Abre o PE a partir de um arquivo no disco.
   HRESULT Open(PTSTR file);

   /// Fecha o PE atualmente aberto.
   VOID Close();

   /// Acessa o PE como se fosse um struct em mem�ria.
   const PEData* operator -> ();

   /// Converte um Relative Virtual Address dentro do PE em 
   /// um ponteiro para o tipo especificado.
   template<typename CastT>
   CastT* RVAToPointer(DWORD rva)
   {
      if( !m_fileMap ) // mem�ria
         return (CastT*) ( (LPBYTE) m_pe.dosHeader + rva );
      else // arquivo mapeado
         return (CastT*) ( (LPBYTE) m_pe.dosHeader + RVAToOffset(rva) );
   }

   /// Converte um Relative Virtual Address dentro do 
   /// PE para seu offset no arquivo.
   DWORD RVAToOffset(DWORD rva);
   
   /// Converte um ponteiro gen�rico para seu 
   /// equivalente Relative Virtual Address.
   DWORD PointerToRVA(PVOID ptr);

   /// Converte um ponteiro gen�rico para seu offset dentro do arquivo PE.
   DWORD PointerToOffset(PVOID ptr);

   /// Eu n�o sei o que isso faz.
   DWORD ShiftRVA(DWORD rva, DWORD offset);

   /// Encontra o section que cont�m o RVA especificado.
   PIMAGE_SECTION_HEADER FindSectionForRva(DWORD rva);

   /// Encontra o section que cont�m o offset especificado.
   PIMAGE_SECTION_HEADER FindSectionForOffset(DWORD offset);

   /// Obt�m o offset do IAT de onde est� uma determinada fun��o de uma DLL.
   DWORD GetIATFunctionOffset(PSTR dllname, PSTR fname);

   /// Encontra um section pelo seu nome.
   PIMAGE_SECTION_HEADER FindSectionByName(PSTR sname);

   /// Apaga um import descriptor da IAT.
   PIMAGE_IMPORT_DESCRIPTOR RemoveImportDescriptor(PIMAGE_IMPORT_DESCRIPTOR impDesc);

   /// Apaga uma fun��o do import descriptor.
   PIMAGE_THUNK_DATA RemoveImportDescriptorFunction(PIMAGE_THUNK_DATA thunk);

protected:
   /// Zera a estrutura que guarda os ponteiros para headers do PE.
   VOID ZeroPEData();

private:
   PEData m_pe;
   CAtlFile m_file;
   CAtlFileMapping<> m_fileMap;
};
