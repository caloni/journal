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

	operator HMODULE () { return reinterpret_cast<HMODULE>( m_pe.dosHeader ); }

   /// Abre o PE a partir de um ponteiro (em mem�ria).
   HRESULT Open(PVOID base);

   /// Abre o PE a partir de um arquivo no disco.
   HRESULT Open(PSTR file, BOOL writeable = FALSE);

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

	FARPROC RVAExportFuncToPointer(DWORD funcRva);
	FARPROC RVAForwardFuncToPointer(DWORD funcRva);

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
   
   /// Descobre se um RVA est� dentro do range especificado.
   BOOL IsInsideRVARange(DWORD rva, DWORD start, DWORD size);

   /// Obt�m o offset do IAT de onde est� uma determinada fun��o de uma DLL.
   DWORD GetIATFunctionOffset(PSTR dllname, PSTR fname);
   
   /// Obt�m o endere�o do IAT de onde est� o endere�o de uma determinada fun��o.
   FARPROC* GetIATFunctionAddress(PVOID faddr);

	/// Obt�m o endere�o da entrada de determinada fun��o atrav�s de seu nome.
	FARPROC GetProcAddress(PSTR procName);
	
	/// Obt�m o endere�o da entrada de determinada fun��o atrav�s de seu ordinal.
	FARPROC GetProcAddress(DWORD ordinal);
	
	/// Obt�m o endere�o da entrada de determinada fun��o atrav�s de seu �ndice.
	FARPROC GetProcAddressByIndex(DWORD index);

	/// Retorna o n�mero de fun��es exportadas.
	DWORD GetNumberOfFunctions();

	/// Retorna o nome de uma fun��o exportada atrav�s de seu endere�o.
	PSTR GetProcName(FARPROC func);

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
