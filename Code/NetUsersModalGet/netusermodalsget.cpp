// netusermodalsget.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int wmain(int argc, WCHAR* argv[])
{
   DWORD dwLevel = 2;
   USER_MODALS_INFO_2 *pBuf = NULL;
   NET_API_STATUS nStatus;
   LPTSTR pszServerName = NULL;

   if (argc > 2)
   {
      fwprintf(stderr, L"Usage: %s [\\\\ServerName]\n", argv[0]);
      exit(1);
   }
   // The server is not the default local computer.
   //
   if (argc == 2)
      pszServerName = argv[1];
   //
   // Call the NetUserModalsGet function; specify level 0.
   //
   nStatus = NetUserModalsGet(pszServerName,
                              dwLevel,
                              (LPBYTE *)&pBuf);
   //
   // If the call succeeds, print the global information.
   //
   if (nStatus == NERR_Success)
   {
      if (pBuf != NULL)
      {
         /*printf("\tMinimum password length:  %d\n", pBuf->usrmod0_min_passwd_len);
         printf("\tMaximum password age (d): %d\n", pBuf->usrmod0_max_passwd_age/86400);
         printf("\tMinimum password age (d): %d\n", pBuf->usrmod0_min_passwd_age/86400);
         printf("\tForced log off time (s):  %d\n", pBuf->usrmod0_force_logoff);
         printf("\tPassword history length:  %d\n", pBuf->usrmod0_password_hist_len);*/
         printf("\tDomain Name: %S\n", pBuf->usrmod2_domain_name);
      }
   }
   // Otherwise, print the system error.
   //
   else
      fprintf(stderr, "A system error has occurred: %d\n", nStatus);
   //
   // Free the allocated memory.
   //
   if (pBuf != NULL)
      NetApiBufferFree(pBuf);

   return 0;
}
