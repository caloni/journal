#include "stdafx.h"
#include "cdaytoday.h"

#include <windows.h>

#include <atlfile.h>

#include <ctime>
#include <fstream>

// Alocação de memória no ATL 7.1.
#pragma comment(lib, "atlthunk.lib")


using namespace std;



CDayToDay::CDayToDay(void)
{
}



CDayToDay::~CDayToDay(void)
{
}



DWORD CDayToDay::SaveMessage(const String &sMessage)
{
   DWORD dwRet = ERROR_SUCCESS;
   CAtlFile fout;

   dwRet = fout.Create(_T("daytoday.log"), 
                       FILE_GENERIC_WRITE, 
                       FILE_SHARE_READ, 
                       OPEN_ALWAYS);

   if( SUCCEEDED(dwRet) )
   {
      ULONGLONG ullSize;
      String sDT = GetCurrentDT() + _T(" ");

      fout.Seek(0, FILE_END);

      // UNICODE support.
      if( sizeof(Char) == sizeof(TCHAR) && 
          SUCCEEDED(fout.GetSize(ullSize)) && !ullSize )
      {
         fout.Write("\xFF\xFE", 2);
      }

      fout.Write(sDT.c_str(), sDT.size() * sizeof(Char));
      fout.Write(sMessage.c_str(), sMessage.size() * sizeof(Char));
      fout.Write(_T("\r\n"), 2 * sizeof(Char));
      fout.Close();
   }

   return dwRet;
}



template<>
CDayToDay::String CDayToDay::AnsiToString<WCHAR>(const char *szStr)
{
   String sRet;
   const size_t sz = strlen(szStr) + 1;
   LPWSTR pwzTemp = new WCHAR[sz];

   mbstowcs(pwzTemp, szStr, sz);
   sRet = pwzTemp;
   
   delete [] pwzTemp;
   return sRet;
}



CDayToDay::String CDayToDay::GetCurrentDT(const char *pszFmt)
{
   char szDT[100] = "";
   time_t tNow = time(0);
   tm *tmNow = localtime(&tNow);

   if( tmNow )
      strftime(szDT, 
               sizeof(szDT), 
               pszFmt ? pszFmt : "%Y.%m.%d %H.%M", 
               tmNow);

   return AnsiToString<Char>(szDT);
}
