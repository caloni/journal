#pragma once
#include <windows.h>
#include <winwlx.h>
#include <assert.h>


typedef
BOOL
(WINAPI
 FWlxInitialize)(
 LPWSTR        lpWinsta,
 HANDLE        hWlx,
 PVOID         pvReserved,
 PVOID         pWinlogonFunctions,
 PVOID*        pWlxContext
 );

typedef
VOID
(WINAPI
 FWlxShutdown)(
 PVOID      pWlxContext,
 DWORD      ShutdownType
 );

typedef 
int
(WINAPI 
 FWlxLoggedOutSAS)
 (
 PVOID                  pWlxContext,
 DWORD                  dwSasType, 
 PLUID                  pAuthenticationId, 
 PSID                   pLogonSid, 
 PDWORD                 pdwOptions, 
 PHANDLE                phToken, 
 PWLX_MPR_NOTIFY_INFO   pNprNotifyInfo, 
 PVOID*                 pProfile
 );
