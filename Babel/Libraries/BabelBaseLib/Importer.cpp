#define _CRT_SECURE_NO_WARNINGS

#include "Importer.h"
#include <windows.h>
#include <stdio.h>


Babel::Importer::Importer()
{
	m_lastError = sqlite3_open(BABEL_FILE, &m_babelFile);

	if( m_lastError != SQLITE_OK )
	{
		sqlite3_close(m_babelFile);
		m_babelFile = 0;
	}
	else
	{
		m_lastError = sqlite3_exec(m_babelFile, "BEGIN TRANSACTION;", 0, 0, 0);
	}
}


Babel::Importer::~Importer()
{
	if( m_babelFile )
	{
		m_lastError = sqlite3_exec(m_babelFile, "END TRANSACTION;", 0, 0, 0);
		sqlite3_close(m_babelFile);
	}
}


bool __stdcall Babel::Importer::ImportDefinition(const char* word,
									   const char* definition,
									   const char* source)
{
	bool ret = false;

	if( m_babelFile )
	{
		CHAR buffer[BABEL_MAX_WORD_SIZE + BABEL_MAX_DEFINITION_SIZE];
		
		SqlEscapeString(m_word, word, BABEL_MAX_WORD_SIZE);
		SqlEscapeString(m_definition, definition, BABEL_MAX_DEFINITION_SIZE);
		SqlEscapeString(m_source, source, BABEL_MAX_SOURCE_SIZE);
		
		sprintf(buffer, "INSERT INTO Babel VALUES (\'%s\', \'%s\', \'%s\');", 
			m_word, m_definition, m_source);

		m_lastError = sqlite3_exec(m_babelFile, buffer, 0, 0, 0);

		if( m_lastError == SQLITE_OK )
			ret = true;
		else
			ret = false;
	}

	SetLastError(m_lastError);
	return ret;
}
