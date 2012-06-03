#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <winternl.h>

namespace WaitJob
{


// Supported methods.
extern enum SupportedMethods
{
   suppNone = 0x0000,
   suppNT   = 0x0001,
   supp9X   = 0x0002,
   supp2K   = 0x0004,
};

extern DWORD m_dwSupported;


// Loaded DLLs.
extern HMODULE m_hSuppNTDll;
extern HMODULE m_hSupp9XDll;
extern HMODULE m_hSupp2KDll;


// NT Undocumented.
extern NTSTATUS (WINAPI *NtQueryInformationProcess)(HANDLE ProcessHandle,
      PROCESSINFOCLASS ProcessInformationClass, PVOID ProcessInformation,
      ULONG ProcessInformationLength, PULONG ReturnLength);
      
extern BOOL (WINAPI *EnumProcesses)(DWORD* lpidProcess, DWORD cb, 
      DWORD* cbNeeded);
      
extern BOOL (WINAPI *IsParentProcess)(DWORD dwPid, DWORD dwParent);


// 9X Toolhelp32.
extern HANDLE (WINAPI *CreateToolhelp32Snapshot)(DWORD dwFlags, 
      DWORD th32ProcessID);

extern BOOL (WINAPI *Process32First)(HANDLE hSnapshot,
      LPPROCESSENTRY32 lppe);

extern BOOL (WINAPI *Process32Next)(HANDLE hSnapshot, 
      LPPROCESSENTRY32 lppe);
      
extern BOOL (WINAPI *CloseToolhelp32SnapshotHandle)(HANDLE h);


// W2K+ Job Objects.
extern HANDLE (WINAPI *CreateJobObject)(
      LPSECURITY_ATTRIBUTES lpJobAttributes, LPCTSTR lpName);
      
extern BOOL (WINAPI *AssignProcessToJobObject)(HANDLE hJob, 
      HANDLE hProcess);

extern BOOL (WINAPI *SetInformationJobObject)(HANDLE hJob, 
      JOBOBJECTINFOCLASS JobObjectInfoClass, LPVOID lpJobObjectInfo, 
      DWORD cbJobObjectInfoLength);


DWORD VerifySupportedMethods();


} //namespace WaitJob
