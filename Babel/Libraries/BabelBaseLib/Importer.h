#include "BabelBaseLib.h"
#include "sqlite3.h"
#include <windows.h>


namespace Babel
{

	class Importer : public IImporter
	{
	public:

		Importer();
		~Importer();

		bool __stdcall ImportDefinition(
				const char* word,
				const char* definition,
				const char* source);

	private:
		char m_word[BABEL_MAX_WORD_SIZE];
		char m_definition[BABEL_MAX_DEFINITION_SIZE];
		char m_source[BABEL_MAX_SOURCE_SIZE];

		sqlite3* m_babelFile;
		int m_lastError;
	};

}
