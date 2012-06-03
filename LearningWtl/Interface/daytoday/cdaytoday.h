#pragma once

#include <fstream>
#include <string>



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
   { return String(szString); }
};
