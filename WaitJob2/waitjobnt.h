#pragma once
#include <tlhelp32.h>


namespace WaitJob { namespace NT
{


HANDLE WINAPI CreateToolhelp32Snapshot(DWORD dwFlags,
   DWORD th32ProcessID);

BOOL WINAPI Process32First(HANDLE hSnapshot,
      LPPROCESSENTRY32 lppe);

BOOL WINAPI Process32Next(HANDLE hSnapshot, 
      LPPROCESSENTRY32 lppe);
      
BOOL WINAPI CloseToolhelp32SnapshotHandle(HANDLE hSnapshot);


} } // namespace WaitJob::NT.
