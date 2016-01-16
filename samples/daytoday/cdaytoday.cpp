#include "stdafx.h"
#include "cdaytoday.h"

#include <windows.h>

#include <atlfile.h>

#include <ctime>
#include <fstream>

using namespace std;


#if defined(_UNICODE) || defined(UNICODE)
#error UNICODE!!
#endif


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

		//// UNICODE support.
		//if( sizeof(Char) == sizeof(TCHAR) && 
		//	SUCCEEDED(fout.GetSize(ullSize)) && !ullSize )
		//{
		//	fout.Write("\xFF\xFE", 2);
		//}

		fout.Write(sDT.c_str(), sDT.size() * sizeof(Char));
		fout.Write(sMessage.c_str(), sMessage.size() * sizeof(Char));
		fout.Write(_T("\r\n"), 2 * sizeof(Char));
		fout.Close();
	}

	return dwRet;
}



//template<>
//CDayToDay::String CDayToDay::AnsiToString<WCHAR>(const char *szStr)
//{
//	String sRet;
//	const size_t sz = strlen(szStr) + 1;
//	LPWSTR pwzTemp = new WCHAR[sz];
//
//	mbstowcs(pwzTemp, szStr, sz);
//	sRet = pwzTemp;
//
//	delete [] pwzTemp;
//	return sRet;
//}



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


CDayToDay::String CDayToDay::GetConfigFilePath()
{
	String ret;
	TCHAR retBuffer[MAX_PATH] = _T(".\\daytoday.ini");
	
	if( GetModuleFileName(NULL, retBuffer, MAX_PATH) )
	{
		if( PTSTR p = _tcsrchr(retBuffer, _T('.')) )
			_tcscpy(p, _T(".ini"));
		else
			_tcscpy(retBuffer, _T(".\\daytoday.ini"));
	}
	
	ret = retBuffer;
	return ret;
}


bool CDayToDay::SaveOption(const String &category, const String &option, const String &value)
{
	String configFile = GetConfigFilePath();
	BOOL err = WritePrivateProfileString(category.c_str(), option.c_str(), 
		value.c_str(), configFile.c_str());
	return err ? true : false;
}


bool CDayToDay::SaveOption(const String &category, const String &option, int value)
{
	String configFile = GetConfigFilePath();
	TCHAR valueBuffer[50] = _T("0");

	_itot(value, valueBuffer, 10);

	BOOL err = WritePrivateProfileString(category.c_str(), option.c_str(), 
		valueBuffer, configFile.c_str());
	
	return err ? true : false;
}


CDayToDay::String CDayToDay::GetOption(const String &category, const String &option, const String &defaultValue)
{
	String configFile = GetConfigFilePath();
	String ret;
	TCHAR retBuffer[MAX_PATH * 2] = _T("");
	
	DWORD err = GetPrivateProfileString(category.c_str(), option.c_str(), defaultValue.c_str(), 
		retBuffer, MAX_PATH * 2, configFile.c_str());

	ret = retBuffer;
	return ret;
}


int CDayToDay::GetOption(const String &category, const String &option, int defaultValue)
{
	String configFile = GetConfigFilePath();
	int ret;
	
	ret = GetPrivateProfileInt(category.c_str(), option.c_str(), defaultValue, 
		configFile.c_str());

	return ret;
}
