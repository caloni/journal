#include "BabelBaseLib.h"
#include "Translator.h"
#include "Importer.h"


extern "C" Babel::ITranslator* Babel::GetTranslator()
{
	return new Babel::Translator;
}


extern "C" void Babel::FreeTranslator(Babel::ITranslator* translator)
{
	Translator* t = static_cast<Translator*>(translator);
	delete t;
}


extern "C" Babel::IImporter* Babel::GetImporter()
{
	return new Babel::Importer;
}


extern "C" void Babel::FreeImporter(Babel::IImporter* importer)
{
	Importer* i = static_cast<Importer*>(importer);
	delete i;
}


void Babel::SqlEscapeString(char* dest, const char* src, size_t maxSize)
{
	size_t currSize = 1;

	while( *src && currSize < maxSize )
	{
		switch( *src )
		{
		case '\'':
			if( maxSize - currSize >= 2 )
			{
				*dest++ = *src++;
				*dest++ = '\'';
				currSize += 2;
			}
			else currSize = maxSize;
			break;

		default:
			*dest++ = *src++;
			++currSize;
			break;
		}
	}

	*dest = 0;
}
