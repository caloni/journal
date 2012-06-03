#define _CRT_SECURE_NO_WARNINGS

#include "Translator.h"
#include <stdio.h>
#include <string.h>


Babel::Translator::Translator()
{
	m_statement = 0;
	m_lastError = sqlite3_open(BABEL_FILE, &m_file);

	if( m_lastError != SQLITE_OK )
	{
		sqlite3_close(m_file);
		m_file = 0;
	}
}


Babel::Translator::~Translator()
{
	if( m_statement )
		m_lastError = sqlite3_finalize(m_statement);

	if( m_file )
		m_lastError = sqlite3_close(m_file);
}


bool __stdcall Babel::Translator::GetDefinition(const char* word)
{
	bool ret = false;
	m_definition[0] = m_source[0] = 0;

	if( m_file )
	{
		if( m_statement )
		{
			m_lastError = sqlite3_finalize(m_statement);
			m_statement = 0;
		}

		char buffer[BABEL_MAX_DEFINITION_SIZE];
		SqlEscapeString(m_word, word, BABEL_MAX_WORD_SIZE);
		sprintf(buffer, "SELECT * FROM Babel WHERE word=\'%s\';", m_word);

		m_lastError = sqlite3_prepare(m_file, buffer, 
			BABEL_MAX_DEFINITION_SIZE, &m_statement, 0);

		if( m_lastError == SQLITE_OK )
			ret = Step();
	}

	return ret;
}


bool __stdcall Babel::Translator::GetNextDefinition()
{
	bool ret = false;

	if( m_file )
		ret = Step();
	
	return ret;
}


bool Babel::Translator::Step()
{
	bool ret = false;

	if( m_statement )
	{
		switch( int step = sqlite3_step(m_statement) )
		{
		case SQLITE_ROW:
			{
				const char* word = (const char*)
					sqlite3_column_text(m_statement, 0);
				strncpy(m_word, word, BABEL_MAX_WORD_SIZE);

				const char* def = (const char*)
					sqlite3_column_text(m_statement, 1);
				strncpy(m_definition, def, BABEL_MAX_DEFINITION_SIZE);
				
				const char* src = (const char*)
					sqlite3_column_text(m_statement, 2);
				strncpy(m_source, src, BABEL_MAX_SOURCE_SIZE);
				
				ret = true;
			}
			break;

		case SQLITE_DONE:
			m_lastError = sqlite3_finalize(m_statement);
			m_statement = 0;
			break;

		default:
			m_lastError = step;
			sqlite3_finalize(m_statement);
			m_statement = 0;
			break;
		}
	}

	return ret;
}


const char* __stdcall Babel::Translator::Word()
{
	return m_word;
}


const char* __stdcall Babel::Translator::Definition()
{
	return m_definition;
}


const char* __stdcall Babel::Translator::Source()
{
	return m_source;
}
