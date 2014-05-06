// IADsDomain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

HRESULT IADSDomainFunc()
{
   HRESULT hr;
   CComPtr<IADsDomain> iADsDomain;
   BSTR bstr;
   VARIANT var;

   hr = ADsOpenObject(L"LDAP://it4svr",
                      L"", L"",
                      ADS_NO_AUTHENTICATION,
                      __uuidof(IADsDomain), 
                      (void**)&iADsDomain);
   if( FAILED(hr) ) return hr;

   hr = iADsDomain->Get(L"DefaultNameContext", &var);
   if( FAILED(hr) ) return hr;

   return 0;
}


int ScuaFunc()
{
   ScuaInit(FALSE);
   
   {
      CNetSelector cNetSelector;
      char szDomainName[MAX_PATH];

      GetScuaSecurityServerDomain(szDomainName, sizeof(szDomainName));
      
      printf(szDomainName);
   }
   
   return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
   HRESULT hr;
   CoInitialize(NULL);

   ScuaFunc();
   /*hr = IADSDomainFunc();
   if( FAILED(hr) )
      printf("Error %d.\n", (int) hr);*/
   
   CoUninitialize();
	return 0;
}

