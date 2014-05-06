#include "stdafx.h"
#include "waitjob.h"
#include "waitjob_priv.h"
#include "waitjobnt.h"



// WaitJob Variables.
namespace WaitJob
{


// Supported methods.
DWORD m_dwSupported;


// Loaded DLLs.
HMODULE m_hSuppNTDll;
HMODULE m_hSupp9XDll;
HMODULE m_hSupp2KDll;


// NT Undocumented.
BOOL (WINAPI *EnumProcesses)(DWORD* lpidProcess, DWORD cb, 
      DWORD* cbNeeded);

NTSTATUS (WINAPI *NtQueryInformationProcess)(HANDLE ProcessHandle,
      PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation,
      ULONG ProcessInformationLength, PULONG ReturnLength);
      

// 9X Toolhelp32.
HANDLE (WINAPI *CreateToolhelp32Snapshot)(DWORD dwFlags, 
      DWORD th32ProcessID);

BOOL (WINAPI *Process32First)(HANDLE hSnapshot,
      LPPROCESSENTRY32 lppe);

BOOL (WINAPI *Process32Next)(HANDLE hSnapshot, 
      LPPROCESSENTRY32 lppe);
      
BOOL (WINAPI *CloseToolhelp32SnapshotHandle)(HANDLE h);


// W2K+ Job Objects.
HANDLE (WINAPI *CreateJobObject)(
      LPSECURITY_ATTRIBUTES lpJobAttributes, LPCTSTR lpName);
      
BOOL (WINAPI *AssignProcessToJobObject)(HANDLE hJob, 
      HANDLE hProcess);

BOOL (WINAPI *SetInformationJobObject)(HANDLE hJob, 
      JOBOBJECTINFOCLASS JobObjectInfoClass, LPVOID lpJobObjectInfo,
      DWORD cbJobObjectInfoLength);


} // namespace WaitJob.



DWORD WaitJob::Initializate()
{
   return VerifySupportedMethods();
}



DWORD WaitJob::Terminate()
{
   if( m_hSuppNTDll )
      FreeLibrary(m_hSuppNTDll);
   if( m_hSupp9XDll )
      FreeLibrary(m_hSupp9XDll);
   if( m_hSupp2KDll )
      FreeLibrary(m_hSupp2KDll);
   return ERROR_SUCCESS;
}



DWORD WaitJob::VerifySupportedMethods()
{
#define GETPROCADDR(var, name)\
   *((FARPROC*)&var) = GetProcAddress(hDll, #name)

   HMODULE hDll;
   m_dwSupported = suppNone;


   // 9X Toolhelp32.
   if( hDll = m_hSupp9XDll = LoadLibrary("kernel32") )
   {
      GETPROCADDR(CreateToolhelp32Snapshot, CreateToolhelp32Snapshot);
      GETPROCADDR(Process32First, Process32First);
      GETPROCADDR(Process32Next, Process32Next);
      GETPROCADDR(CloseToolhelp32SnapshotHandle, CloseHandle);
   }
   // NT Undocumented.
   if( CreateToolhelp32Snapshot )
      m_dwSupported |= supp9X;
   else
   {
      if( hDll = m_hSuppNTDll = GetModuleHandle("ntdll") )
      {
         GETPROCADDR(NtQueryInformationProcess, NtQueryInformationProcess);
      }
      if( hDll = m_hSuppNTDll = LoadLibrary("psapi") )
      {
         GETPROCADDR(EnumProcesses, EnumProcesses);
      }
      if( NtQueryInformationProcess && EnumProcesses )
      {
         CreateToolhelp32Snapshot = NT::CreateToolhelp32Snapshot;
         Process32First = NT::Process32First;
         Process32Next = NT::Process32Next;
         CloseToolhelp32SnapshotHandle = NT::CloseToolhelp32SnapshotHandle;
         m_dwSupported |= suppNT;
      }
   }


   // W2K+ Job Objects.
   if( hDll = m_hSupp2KDll = LoadLibrary("kernel32") )
   {
      #ifdef UNICODE
      GETPROCADDR(CreateJobObject, CreateJobObjectW);
      #else
      GETPROCADDR(CreateJobObject, CreateJobObjectA);
      #endif
      GETPROCADDR(AssignProcessToJobObject, AssignProcessToJobObject);
      GETPROCADDR(SetInformationJobObject, SetInformationJobObject);
   }
   if( CreateJobObject )
      m_dwSupported |= supp2K;

   return ERROR_SUCCESS;

#undef GETPROCADDR
}



/**
 * Create a process and return its PROCESS_INFORMATION.
 *
 * @param ppi The CreateProcess() lpProcessInformation parameter.
 * @param tzProcFile The CreateProcess() lpCommandLine parameter.
 * @param dwFlags The CreateProcess() dwCreationFlags parameter.
 *
 * @return The same value returned by CreateProcess().
 */
DWORD WaitJob::CreateProc(PROCESS_INFORMATION *ppi, LPTSTR tzProcFile, DWORD dwFlags)
{
   DWORD dwRet = ERROR_SUCCESS;
   STARTUPINFO si = { sizeof(si) };

   if( CreateProcess(NULL, tzProcFile, NULL, NULL, FALSE, dwFlags, 
      NULL, NULL, &si, ppi) == FALSE )
   {
      dwRet = GetLastError();
   }

   return dwRet;
}



DWORD WaitJob::WaitAndReleaseChildProcs(LPPROCESS_INFORMATION ppi)
{
   DWORD dwRet = ERROR_SUCCESS;
   HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

   if( hSnapshot != INVALID_HANDLE_VALUE )
   {
      PROCESSENTRY32 pe;
      BOOL bRet = Process32First(hSnapshot, &pe);

      if( bRet != FALSE )
      {
         do
         {
            if( ppi->dwProcessId == pe.th32ParentProcessID )
            {
               HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE,
                  pe.th32ProcessID);

               if( hProc )
               {
                  PROCESS_INFORMATION pi;

                  memset(&pi, 0, sizeof(pi));
                  pi.hProcess = hProc;
                  pi.dwProcessId = pe.th32ProcessID;

                  dwRet = WaitAndReleaseProc(&pi);
               }
               else
               if( (dwRet = GetLastError()) == ERROR_INVALID_PARAMETER )
                  dwRet = ERROR_SUCCESS;
            }
         }
         while( Process32Next(hSnapshot, &pe) != FALSE 
               && dwRet == ERROR_SUCCESS );
      }

      CloseToolhelp32SnapshotHandle(hSnapshot);
   }
   
   return dwRet;
}



DWORD WaitJob::WaitAndReleaseProc(LPPROCESS_INFORMATION ppi)
{
   DWORD dwRet;

   if( WaitForSingleObject(ppi->hProcess, INFINITE) 
      == WAIT_OBJECT_0 )
   {
      dwRet = WaitAndReleaseChildProcs(ppi);
   }
   else
      dwRet = GetLastError();

   if( ppi->hProcess )
      CloseHandle(ppi->hProcess);

   if( ppi->hThread )
      CloseHandle(ppi->hThread);

   return dwRet;
}
