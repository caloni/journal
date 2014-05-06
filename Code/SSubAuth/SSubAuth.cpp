#include <windows.h>
#include <subauth.h>
#include <tchar.h>

NTSTATUS NTAPI Msv1_0SubAuthenticationFilter(
	NETLOGON_LOGON_INFO_CLASS LogonLevel,
	PVOID LogonInformation,
	ULONG Flags,
	PUSER_ALL_INFORMATION UserAll,
	PULONG WhichFields,
	PULONG UserFlags,
	PBOOLEAN Authoritative,
	PLARGE_INTEGER LogoffTime,
	PLARGE_INTEGER KickoffTime
	)
{
	PNETLOGON_LOGON_IDENTITY_INFO pLogonInfo 
		= (PNETLOGON_LOGON_IDENTITY_INFO) LogonInformation;

	if( lstrcmpi(pLogonInfo->UserName.Buffer, _T("paulo")) == 0 )
		return STATUS_INVALID_WORKSTATION;

	return STATUS_SUCCESS;
}


NTSTATUS NTAPI Msv1_0SubAuthenticationRoutine(
  NETLOGON_LOGON_INFO_CLASS LogonLevel,
  PVOID LogonInformation,
  ULONG Flags,
  PUSER_ALL_INFORMATION UserAll,
  PULONG WhichFields,
  PULONG UserFlags,
  PBOOLEAN Authoritative,
  PLARGE_INTEGER LogoffTime,
  PLARGE_INTEGER KickoffTime
)
{
	return STATUS_SUCCESS;
}
