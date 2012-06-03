#include "stdafx.h"

#include "waitjob.h"
#include "waitjob_priv.h"
#include "waitjobnt.h"

#include <winternl.h>



namespace WaitJob { namespace NT
{


/** Undocumented NT struct returned by NtQueryInformationProcess(). */
struct ProcessInfo
{
   INT32 ExitStatus;
   LPVOID PebBaseAddress;
   UINT32 AffinityMask;
   INT32 BasePriority;
   UINT32 UniqueProcessId;
   UINT32 InheritedFromUniqueProcessId;
};

struct Snapshot
{
   DWORD dwPos;
   DWORD dwPIs;
   ProcessInfo *pPIs;
};

DWORD FillProcessInfo(ProcessInfo *ppi, UINT32 uiPid);

} } // namespace WaitJob::NT.



/**
 * @remarks Thanks to Network (cubical@hotmail.com). Found at Google groups:
 * http://groups.google.com.br/groups?selm=ePcacAiL%23GA.277%40uppssnewspub0
 * 4.moswest.msn.net&output=gplain.
 */
DWORD WaitJob::NT::FillProcessInfo(ProcessInfo *ppi, UINT32 uiPid)
{
   DWORD dwRet;
   HANDLE ProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, uiPid);

   if( ProcessHandle != NULL )
   {
      dwRet = NtQueryInformationProcess(ProcessHandle,
            ProcessBasicInformation, ppi, sizeof(ProcessInfo), NULL);
      CloseHandle(ProcessHandle);
   }
   else
      dwRet = GetLastError();

   return dwRet;
}



HANDLE WINAPI WaitJob::NT::CreateToolhelp32Snapshot(DWORD dwFlags,
   DWORD th32ProcessID)
{
   Snapshot *pssRet = (Snapshot*) INVALID_HANDLE_VALUE;
   DWORD dwPids[1024];
   DWORD dwNeeded;

   if( EnumProcesses(dwPids, sizeof(dwPids), &dwNeeded) )
   {
      DWORD dwErr;
      pssRet = new Snapshot;
      pssRet->dwPos = 0;
      pssRet->dwPIs = dwNeeded / sizeof(DWORD);
      pssRet->pPIs = new ProcessInfo[pssRet->dwPIs];

      for( DWORD i = 0, j = 0; i < pssRet->dwPIs; ++i, ++j )
      {
         switch( dwErr = FillProcessInfo(&pssRet->pPIs[i], dwPids[j]) )
         {
         case ERROR_SUCCESS:
            continue;
         
         case ERROR_INVALID_PARAMETER:
            --pssRet->dwPIs;
            --i;
            continue;

         case ERROR_ACCESS_DENIED:
            ZeroMemory(&pssRet->pPIs[i], sizeof(pssRet->pPIs[i]));
            pssRet->pPIs[i].UniqueProcessId = dwPids[j];
            continue;

         default:
            CloseToolhelp32SnapshotHandle(pssRet);
            pssRet = (Snapshot*) INVALID_HANDLE_VALUE;
            i = pssRet->dwPIs;
         }
      }
   }

   return (HANDLE) pssRet;
}



BOOL WINAPI WaitJob::NT::Process32First(HANDLE hSnapshot,
      LPPROCESSENTRY32 lppe)
{
   BOOL bRet = FALSE;

   if( (HANDLE) hSnapshot != INVALID_HANDLE_VALUE )
   {
      Snapshot *pss = (Snapshot*) hSnapshot;

      if( pss->dwPos < pss->dwPIs )
      {
         ZeroMemory(lppe, sizeof(PROCESSENTRY32));
         lppe->th32ProcessID = 
            pss->pPIs[pss->dwPos].UniqueProcessId;
         lppe->th32ParentProcessID = 
            pss->pPIs[pss->dwPos].InheritedFromUniqueProcessId;
         ++pss->dwPos;
         bRet = TRUE;
      }
      else
         SetLastError(ERROR_NO_MORE_FILES);
   }
   else
      SetLastError(ERROR_INVALID_HANDLE);

   return bRet;
}



BOOL WINAPI WaitJob::NT::Process32Next(HANDLE hSnapshot, 
      LPPROCESSENTRY32 lppe)
{
   return WaitJob::NT::Process32First(hSnapshot, lppe);
}


      
BOOL WINAPI WaitJob::NT::CloseToolhelp32SnapshotHandle(HANDLE hSnapshot)
{
   BOOL bRet = FALSE;

   if( (HANDLE) hSnapshot != INVALID_HANDLE_VALUE )
   {
      Snapshot *pss = (Snapshot*) hSnapshot;
      delete [] pss->pPIs;
      delete pss;
      bRet = TRUE;
   }
   else
      SetLastError(ERROR_INVALID_HANDLE);

   return bRet;
}
