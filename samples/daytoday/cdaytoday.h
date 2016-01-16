#pragma once

#include <fstream>
#include <string>


#if defined(_UNICODE) || defined(UNICODE)
#error UNICODE!!
#endif



class CDayToDay
{
public:
	typedef TCHAR Char;
	typedef std::basic_string<Char> String;
	typedef std::basic_fstream<Char> File;

	CDayToDay(void);
	~CDayToDay(void);

	DWORD SaveMessage(const String &sMessage);
	String GetCurrentDT(const char *psFmt = 0);
	template<typename CharType> String AnsiToString(const char *szStr)
	{ return String(szStr); }

	bool SaveOption(const String &category, const String &option, const String &value);
	bool SaveOption(const String &category, const String &option, int value);
	
	String GetOption(const String &category, const String &option, const String &defaultValue = _T(""));
	int GetOption(const String &category, const String &option, int defaultValue = 0);

private:
	String GetConfigFilePath();
};
