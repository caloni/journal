#include "stdafx.h"
#include <tinyxml\tinyxml.h>
#include <algorithm>
#include <iostream>
#include <string>

#pragma comment(lib, "tinyxml.lib")

using namespace std;


struct ShadowCopyInfo
{
	std::string ShadowCopy;
	std::string txtUsuario;
	std::string txtPathOrigem;
	std::string txtPath;
	std::string txtSerialOrigem;
	std::string txtSerialDestino;
	std::string txtProcesso;
	std::string txtWorkStation;
	int origemUsb;
	int destinoUsb;
	std::string dataProcesso;
	std::string dataProcessoCompact;
	int numTamanhoArquivo;
};


bool ReadShadowCopyInfo(const std::string& path, ShadowCopyInfo& si)
{
	TiXmlDocument doc(path.c_str());

	if( doc.LoadFile() )
	{
		if( TiXmlElement* root = doc.FirstChildElement("Shadow-Copy") )
		{
			if( TiXmlElement* element = root->FirstChildElement("ShadowCopy") ) 
			{
				if( const char* txt = element->GetText() )
					si.ShadowCopy = txt; 
			}

			if( TiXmlElement* element = root->FirstChildElement("txtUsuario") ) 
			{
				if( const char* txt = element->GetText() )
					si.txtUsuario = txt; 
			}

			if( TiXmlElement* element = root->FirstChildElement("txtPathOrigem") ) 
			{
				if( const char* txt = element->GetText() )
					si.txtPathOrigem = txt; 
			}

			if( TiXmlElement* element = root->FirstChildElement("txtPath") ) 
			{
				if( const char* txt = element->GetText() )
					si.txtPath = txt; 
			}

			if( TiXmlElement* element = root->FirstChildElement("txtSerialOrigem") ) 
			{
				if( const char* txt = element->GetText() )
					si.txtSerialOrigem = txt; 
			}

			if( TiXmlElement* element = root->FirstChildElement("txtSerialDestino") ) 
			{
				if( const char* txt = element->GetText() )
					si.txtSerialDestino = txt; 
			}

			if( TiXmlElement* element = root->FirstChildElement("txtProcesso") ) 
			{
				if( const char* txt = element->GetText() )
					si.txtProcesso = txt; 
			}

			if( TiXmlElement* element = root->FirstChildElement("txtWorkStation") ) 
			{
				if( const char* txt = element->GetText() )
					si.txtWorkStation = txt; 
			}

			if( TiXmlElement* element = root->FirstChildElement("origemUsb") ) 
			{
				if( const char* txt = element->GetText() )
					si.origemUsb = atoi(txt); 
			}

			if( TiXmlElement* element = root->FirstChildElement("destinoUsb") ) 
			{
				if( const char* txt = element->GetText() )
					si.destinoUsb = atoi(txt); 
			}

			if( TiXmlElement* element = root->FirstChildElement("dataProcesso") ) 
			{
				if( const char* txt = element->GetText() )
				{
					si.dataProcesso = txt;
					si.dataProcessoCompact = txt;
					char rmChars[] = "-: ";
					for (size_t i = 0; i < strlen(rmChars); ++i)
					{
						si.dataProcessoCompact.erase(std::remove(si.dataProcessoCompact.begin(), si.dataProcessoCompact.end(), 
							rmChars[i]), si.dataProcessoCompact.end());
					}
				}
			}

			if( TiXmlElement* element = root->FirstChildElement("numTamanhoArquivo") ) 
			{
				if( const char* txt = element->GetText() )
					si.numTamanhoArquivo = atoi(txt); 
			}
		}

		return true;
	}

	return false;
}


int _tmain(int argc, char* argv[])
{
	if( argc >= 2 )
	{
		ShadowCopyInfo si;
		if( ReadShadowCopyInfo(argv[1], si) )
			cout << "OK!\n";
	}
	return 0;
}
