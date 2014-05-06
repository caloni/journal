#include "BabelBaseLib.h"
#include "sqlite3.h"
#include <stdio.h>


namespace Babel
{

	class Translator : public ITranslator
	{
	public:
		Translator();
		~Translator();

		bool __stdcall GetDefinition(const char* word);
		bool __stdcall GetNextDefinition();

		const char* __stdcall Word();
		const char* __stdcall Definition();
		const char* __stdcall Source();

	private:
		bool Step();

		char m_word[BABEL_MAX_WORD_SIZE];
		char m_definition[BABEL_MAX_DEFINITION_SIZE];
		char m_source[BABEL_MAX_SOURCE_SIZE];

		sqlite3* m_file;
		sqlite3_stmt* m_statement;
		int m_lastError;
	};

}
