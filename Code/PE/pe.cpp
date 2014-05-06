/** @file
Classe utilitária que manipula um PE (Portable Executable).

@author Wanderley Caloni Jr <wanderley@caloni.com.br>
@date 10.2005
*/
#include "pe.h"


PE::PE()
{
	ZeroPEData();
}


/** Abre um PE baseado em um ponteiro genérico. Imagina-se que foi obtido 
ao chamar a função API GetModuleHandle() que na verdade obtem o endereço 
base onde o módulo foi carregado.
*/
HRESULT PE::Open(PVOID base)
{
	HRESULT ret = E_UNEXPECTED;

	if( ! IsBadReadPtr(base, sizeof(IMAGE_DOS_HEADER)) )
	{
		m_pe.dosHeader = (PIMAGE_DOS_HEADER) base;
		m_pe.ntHeader = (PIMAGE_NT_HEADERS32) ( (LPBYTE) m_pe.dosHeader + m_pe.dosHeader->e_lfanew );

		if( ! IsBadReadPtr(m_pe.ntHeader, sizeof(IMAGE_NT_HEADERS32)) )
		{
			if( m_pe.dosHeader->e_magic == 'ZM' && 
				m_pe.ntHeader->Signature == 'EP' )
			{
				m_pe.optHeader = &m_pe.ntHeader->OptionalHeader;
				m_pe.dirsHeader = m_pe.optHeader->DataDirectory;
				m_pe.sectHeader = (PIMAGE_SECTION_HEADER) ( m_pe.ntHeader + 1 );
				m_pe.dataBody = (PVOID) ( m_pe.sectHeader + m_pe.ntHeader->FileHeader.NumberOfSections );
				ret = S_OK;
			}
		}
	}
	return ret;
}


/** Abre um PE baseado em um arquivo em disco. O arquivo é mapeado e sua 
seus dados são lidos e escritos diretamente do arquivo. Qualquer 
modificação feita no PE é feita de fato no arquivo em disco. */
HRESULT PE::Open(PSTR file, BOOL writeable)
{
	HRESULT hr;
	DWORD fileFlag = writeable ? ( GENERIC_READ | GENERIC_WRITE ) : GENERIC_READ;
	DWORD pageFlag = writeable ? PAGE_READWRITE : PAGE_READONLY;
	DWORD mapFlag = writeable ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ;

	hr = m_file.Create(file, fileFlag,  0, OPEN_EXISTING);

	if( SUCCEEDED(hr) )
	{
		hr = m_fileMap.MapFile(m_file, 0, 0, pageFlag, mapFlag);

		if( SUCCEEDED(hr) )
			hr = Open(m_fileMap.GetData());
		else
			m_file.Close();
	}

	return hr;
}


/** Fecha os recursos relacionados ao PE atualmente aberto. Se for um 
arquivo em disco, o mapeamento e o handle para ele serão liberados. */
VOID PE::Close()
{
	if( m_fileMap )
		m_fileMap.Unmap();

	if( m_file.m_h != INVALID_HANDLE_VALUE )
		m_file.Close();

	ZeroPEData();
}


/** Acessa diretamente os ponteiros para os headers mais acessados na 
estrutura de um PE. Estrutura criada para facilitar o acesso aos headers. */
const PE::PEData* PE::operator -> ()
{
	return &m_pe;
}


FARPROC PE::RVAExportFuncToPointer(DWORD funcRva)
{
	PVOID ret = NULL;
	PIMAGE_DATA_DIRECTORY expDir = &m_pe.dirsHeader[IMAGE_DIRECTORY_ENTRY_EXPORT];

	// se o ponteiro estiver apontando para dentro do diretório
	// de funções exportadas, se trata de uma função forwarded
	if( IsInsideRVARange(funcRva, expDir->VirtualAddress, expDir->Size) )
	{
		ret = RVAForwardFuncToPointer(funcRva);
	}
	else // senão, é função nossa mesmo
	{
		ret = RVAToPointer<VOID>(funcRva);
	}

	return reinterpret_cast<FARPROC>( ret );
}


FARPROC PE::RVAForwardFuncToPointer(DWORD funcRva)
{
	FARPROC ret = NULL;
	PSTR dllFuncName = RVAToPointer<CHAR>(funcRva);

	if( dllFuncName )
	{
		PSTR funcName = strchr(dllFuncName, '.');

		if( funcName )
		{
			CHAR dllName[MAX_PATH] = "";
			size_t size = ( funcName - dllFuncName ) + 1;
			lstrcpyn(dllName, dllFuncName, int(size));

			HMODULE dll = GetModuleHandle(dllName);
			HMODULE dllLoad = dll ? dll : LoadLibrary(dllName);

			if( dllLoad )
			{
				PE pe;
				HRESULT hr = pe.Open(dllLoad);

				if( SUCCEEDED(hr) )
				{
					++funcName; // tira o '.' inicial
					ret = pe.GetProcAddress(funcName);
					pe.Close();
				}
			}
				
			if( dllLoad )
				FreeLibrary(dllLoad), dllLoad = NULL;
		}
	}

	return ret;
}


DWORD PE::RVAToOffset(DWORD rva)
{
	if( PIMAGE_SECTION_HEADER sectH = FindSectionForRva(rva) )
		return rva - sectH->VirtualAddress + sectH->PointerToRawData;
	return 0;
}


DWORD PE::PointerToRVA(PVOID ptr)
{
	DWORD offset = DWORD( (LPBYTE) ptr - (LPBYTE) m_pe.dosHeader );

	if( !m_fileMap ) // memória
		return offset;
	else // arquivo mapeado
	{
		if( PIMAGE_SECTION_HEADER sectH =
			FindSectionForOffset(offset) )
		{
			return DWORD( offset - 
				sectH->PointerToRawData + 
				sectH->VirtualAddress );
		}
	}

	return 0;
}


DWORD PE::PointerToOffset(PVOID ptr)
{
	DWORD rva = PointerToRVA(ptr);
	return RVAToOffset(rva);
}


DWORD PE::ShiftRVA(DWORD rva, DWORD offset)
{
	PBYTE ptr = RVAToPointer<BYTE>(rva);
	ptr += offset;
	return PointerToRVA(ptr);
}


PIMAGE_SECTION_HEADER PE::FindSectionForRva(DWORD rva)
{
	PIMAGE_SECTION_HEADER sectH = m_pe.sectHeader;
	DWORD nSects = m_pe.ntHeader->FileHeader.NumberOfSections;

	for( DWORD i = 0; i < nSects; ++i, ++sectH )
	{
		// se o endereço estiver dentro da section
		if( sectH->VirtualAddress <= rva &&
			rva < sectH->VirtualAddress + sectH->SizeOfRawData )
		{
			return sectH;
		}
	}
	return 0;
}


PIMAGE_SECTION_HEADER PE::FindSectionForOffset(DWORD offset)
{
	PIMAGE_SECTION_HEADER sectH = m_pe.sectHeader;
	DWORD nSects = m_pe.ntHeader->FileHeader.NumberOfSections;

	for( DWORD i = 0; i < nSects; ++i, ++sectH )
	{
		// se o endereço estiver dentro da section
		if( sectH->PointerToRawData <= offset &&
			offset < sectH->PointerToRawData + sectH->SizeOfRawData )
		{
			return sectH;
		}
	}
	return 0;
}


BOOL PE::IsInsideRVARange(DWORD rva, DWORD start, DWORD size)
{
	DWORD end = start + size;
	BOOL ret = ( rva >= start && rva < end ) ? TRUE : FALSE;
	return ret;
}

DWORD PE::GetIATFunctionOffset(PSTR dllname, PSTR fname)
{
	PIMAGE_IMPORT_DESCRIPTOR impDesc = RVAToPointer<IMAGE_IMPORT_DESCRIPTOR>(
		m_pe.dirsHeader[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	while( impDesc->Name )
	{
		if( !_stricmp(dllname, RVAToPointer<CHAR>(impDesc->Name)) )
		{
			PIMAGE_THUNK_DATA orThunkData = 
				RVAToPointer<IMAGE_THUNK_DATA>(impDesc->OriginalFirstThunk);

			PIMAGE_THUNK_DATA thunkData = 
				RVAToPointer<IMAGE_THUNK_DATA>(impDesc->FirstThunk);

			while( orThunkData->u1.AddressOfData )
			{
				PIMAGE_IMPORT_BY_NAME impByName = 
					RVAToPointer<IMAGE_IMPORT_BY_NAME>(orThunkData->u1.AddressOfData);

				if( !_stricmp(fname, (PSTR)impByName->Name) )
				{
					DWORD offset = PointerToOffset(thunkData);

					if( PIMAGE_SECTION_HEADER sectH = FindSectionForOffset(offset) )
					{
						return m_pe.optHeader->ImageBase + PointerToRVA(thunkData);
					}
				}

				++orThunkData;
				++thunkData;
			}
		}

		++impDesc;
	}

	return 0;
}


FARPROC* PE::GetIATFunctionAddress(PVOID faddr)
{
	FARPROC* ret = NULL;
	PIMAGE_IMPORT_DESCRIPTOR impDesc = RVAToPointer<IMAGE_IMPORT_DESCRIPTOR>(
		m_pe.dirsHeader[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	while( ret == NULL && impDesc->Name )
	{
		PIMAGE_THUNK_DATA thunkData = 
			RVAToPointer<IMAGE_THUNK_DATA>(impDesc->FirstThunk);

		while( thunkData->u1.AddressOfData )
		{
			FARPROC* currAddr = reinterpret_cast<FARPROC*>(&thunkData->u1.AddressOfData);

			if( faddr == *currAddr )
			{
				ret = currAddr;
				break;
			}

			++thunkData;
		}

		++impDesc;
	}

	return ret;
}


FARPROC PE::GetProcAddress(PSTR procName)
{
	PVOID ret = NULL;
	PIMAGE_EXPORT_DIRECTORY expDir = 
		RVAToPointer<IMAGE_EXPORT_DIRECTORY>(m_pe.dirsHeader[
			IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

			if( expDir )
			{
				PDWORD names = RVAToPointer<DWORD>(expDir->AddressOfNames);
				PWORD ords = RVAToPointer<WORD>(expDir->AddressOfNameOrdinals);
				PDWORD funcs = RVAToPointer<DWORD>(expDir->AddressOfFunctions);

				if( names && ords && funcs )
				{
					for( DWORD nameIdx = 0; nameIdx < expDir->NumberOfNames; ++nameIdx )
					{
						PSTR name = RVAToPointer<CHAR>(names[nameIdx]);

						if( lstrcmpi(name, procName) == 0 )
						{
							WORD ordinal = ords[nameIdx];
							DWORD func = funcs[ordinal];
							ret = RVAExportFuncToPointer(func);
							break;
						}
					}
				}
			}

			return reinterpret_cast<FARPROC>( ret );
}


DWORD PE::GetNumberOfFunctions()
{
	DWORD ret = 0;
	PIMAGE_EXPORT_DIRECTORY expDir = 
		RVAToPointer<IMAGE_EXPORT_DIRECTORY>(m_pe.dirsHeader[
			IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

			if( expDir )
			{
				ret = expDir->NumberOfFunctions;
			}

			return ret;
}


FARPROC PE::GetProcAddress(DWORD ordinal)
{
	PVOID ret = NULL;
	PIMAGE_EXPORT_DIRECTORY expDir = 
		RVAToPointer<IMAGE_EXPORT_DIRECTORY>(m_pe.dirsHeader[
			IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

			if( expDir )
			{
				DWORD internalOrdinal = ordinal - expDir->Base;

				if( internalOrdinal < expDir->NumberOfFunctions )
				{
					PDWORD funcs = RVAToPointer<DWORD>(expDir->AddressOfFunctions);
					DWORD func = funcs[internalOrdinal];
					ret = RVAExportFuncToPointer(func);
				}
			}

			return reinterpret_cast<FARPROC>( ret );
}


FARPROC PE::GetProcAddressByIndex(DWORD index)
{
	PVOID ret = NULL;
	PIMAGE_EXPORT_DIRECTORY expDir = 
		RVAToPointer<IMAGE_EXPORT_DIRECTORY>(m_pe.dirsHeader[
			IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

			if( expDir && index < expDir->NumberOfFunctions )
			{
				PDWORD funcs = RVAToPointer<DWORD>( expDir->AddressOfFunctions );
				DWORD func = funcs[index];
				ret = RVAExportFuncToPointer(func);
			}

			return reinterpret_cast<FARPROC>( ret );
}


PSTR PE::GetProcName(FARPROC func)
{
	PSTR ret = NULL;
	PVOID funcAddr = reinterpret_cast<PVOID>( func );
	PIMAGE_EXPORT_DIRECTORY expDir = RVAToPointer<IMAGE_EXPORT_DIRECTORY>(
		m_pe.dirsHeader[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	if( expDir )
	{
		PDWORD names = RVAToPointer<DWORD>(expDir->AddressOfNames);
		PWORD ords = RVAToPointer<WORD>(expDir->AddressOfNameOrdinals);
		PDWORD funcs = RVAToPointer<DWORD>(expDir->AddressOfFunctions);

		if( names && ords && funcs )
		{
			for( DWORD funcIdx = 0; !ret && funcIdx < expDir->NumberOfFunctions; ++funcIdx )
			{
				FARPROC funcAddr = RVAExportFuncToPointer(funcs[funcIdx]);

				if( funcAddr == func )
				{
					DWORD ordinal = funcIdx;

					for( DWORD ordIdx = 0; ordIdx < expDir->NumberOfNames; ++ordIdx )
					{
						if( ordinal == ords[ordIdx] )
						{
							ret = RVAToPointer<CHAR>( names[ordIdx] );
							break;
						}
					}
				}
			}
		}
	}

	return ret;
}


PIMAGE_SECTION_HEADER PE::FindSectionByName(PSTR sname)
{
	PIMAGE_SECTION_HEADER sectH = m_pe.sectHeader;
	DWORD nSects = m_pe.ntHeader->FileHeader.NumberOfSections;

	for( DWORD i = 0; i < nSects; ++i, ++sectH )
		if( !_stricmp(sname, (PSTR) sectH->Name) )
			return sectH;

	return 0;
}


PIMAGE_IMPORT_DESCRIPTOR PE::RemoveImportDescriptor(
	PIMAGE_IMPORT_DESCRIPTOR impDesc)
{
	PIMAGE_IMPORT_DESCRIPTOR ret = impDesc;

	PSTR dllname = RVAToPointer<CHAR>(impDesc->Name);
	ZeroMemory(dllname, strlen(dllname));

	PIMAGE_THUNK_DATA orThunkData =
		RVAToPointer<IMAGE_THUNK_DATA>(impDesc->OriginalFirstThunk);

	PIMAGE_THUNK_DATA thunkData = 
		RVAToPointer<IMAGE_THUNK_DATA>(impDesc->FirstThunk);

	while( orThunkData->u1.AddressOfData )
	{
		PIMAGE_IMPORT_BY_NAME impByName = 
			RVAToPointer<IMAGE_IMPORT_BY_NAME>(orThunkData->u1.AddressOfData);

		impByName->Hint = 0;
		ZeroMemory(impByName->Name, strlen((PSTR) impByName->Name));

		++orThunkData;
		++thunkData;
	}

	PIMAGE_IMPORT_DESCRIPTOR nextImpDesc = impDesc + 1;
	do CopyMemory(impDesc++, nextImpDesc++, sizeof(*impDesc));
	while( impDesc->Name );

	return ret;
}


PIMAGE_THUNK_DATA PE::RemoveImportDescriptorFunction(PIMAGE_THUNK_DATA thunk)
{
	PIMAGE_THUNK_DATA ret = thunk;
	PIMAGE_IMPORT_BY_NAME impByName = 
		RVAToPointer<IMAGE_IMPORT_BY_NAME>(thunk->u1.AddressOfData);

	impByName->Hint = 0;
	ZeroMemory(impByName->Name, strlen((PSTR) impByName->Name));

	PIMAGE_THUNK_DATA nextThunk = thunk + 1;
	do CopyMemory(thunk++, nextThunk++, sizeof(*thunk));
	while( thunk->u1.AddressOfData );

	return ret;
}


VOID PE::ZeroPEData()
{
	ZeroMemory(&m_pe, sizeof(m_pe));
}
