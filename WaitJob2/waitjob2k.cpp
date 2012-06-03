#include "stdafx.h"

#include "waitjob.h"
#include "waitjob_priv.h"
#include "waitjob2k.h"

#include <tchar.h>
#include <windows.h>
#include <stdio.h>



void WaitJob::PrintJobMsg(DWORD dwMsg)
{
   switch( dwMsg )
   {
      case JOB_OBJECT_MSG_END_OF_JOB_TIME:
         _tprintf(_T("JOB_OBJECT_MSG_END_OF_JOB_TIME\n"));
         break;
      
      case JOB_OBJECT_MSG_END_OF_PROCESS_TIME:
         _tprintf(_T("JOB_OBJECT_MSG_END_OF_PROCESS_TIME\n"));
         break;
      
      case JOB_OBJECT_MSG_ACTIVE_PROCESS_LIMIT:
         _tprintf(_T("JOB_OBJECT_MSG_ACTIVE_PROCESS_LIMIT\n"));
         break;
      
      case JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO:
         _tprintf(_T("JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO\n"));
         break;
      
      case JOB_OBJECT_MSG_NEW_PROCESS:
         _tprintf(_T("JOB_OBJECT_MSG_NEW_PROCESS\n"));
         break;
      
      case JOB_OBJECT_MSG_EXIT_PROCESS:
         _tprintf(_T("JOB_OBJECT_MSG_EXIT_PROCESS\n"));
         break;
      
      case JOB_OBJECT_MSG_ABNORMAL_EXIT_PROCESS:
         _tprintf(_T("JOB_OBJECT_MSG_ABNORMAL_EXIT_PROCESS\n"));
         break;
      
      case JOB_OBJECT_MSG_PROCESS_MEMORY_LIMIT:
         _tprintf(_T("JOB_OBJECT_MSG_PROCESS_MEMORY_LIMIT\n"));
         break;
      
      case JOB_OBJECT_MSG_JOB_MEMORY_LIMIT:
         _tprintf(_T("JOB_OBJECT_MSG_JOB_MEMORY_LIMIT\n"));
         break;
   }
}



DWORD WaitJob::WaitJob(HANDLE hJob)
{
   DWORD dwRet = ERROR_SUCCESS;
   HANDLE hPort = 
      CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

   if( hPort )
   {
      JOBOBJECT_ASSOCIATE_COMPLETION_PORT jobAssoc = { NULL, hPort };

      if( SetInformationJobObject(hJob, 
         JobObjectAssociateCompletionPortInformation,
         &jobAssoc, sizeof(jobAssoc)) == TRUE )
      {
         BOOL bBreak = FALSE;
         DWORD dwMsg;
         ULONG_PTR ulKey;
         LPOVERLAPPED pOver;

         while( GetQueuedCompletionStatus(hPort, &dwMsg, &ulKey, 
            &pOver, INFINITE) == TRUE )
         {
            switch( dwMsg )
            {
            case JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO:
               bBreak = TRUE;
               break;
            }
            PrintJobMsg(dwMsg);   
            if( bBreak ) break;
         }
      }
      else 
         dwRet = GetLastError();
   }
   else 
      dwRet = GetLastError();

   return dwRet;
}



DWORD WaitJob::CreateJob(LPTSTR tzProcFile)
{
   DWORD dwRet = ERROR_SUCCESS;
   HANDLE hJob = CreateJobObject(NULL, NULL);

   if( hJob )
   {
      PROCESS_INFORMATION pi;

      if( (dwRet = CreateProc(&pi, tzProcFile, 
         CREATE_SUSPENDED | CREATE_NEW_CONSOLE)) == ERROR_SUCCESS )
      {
         if( AssignProcessToJobObject(hJob, pi.hProcess) == TRUE )
         {
            ResumeThread(pi.hThread);
            dwRet = WaitJob(hJob);
         }
         else
            dwRet = GetLastError();

         CloseHandle(pi.hProcess);
         CloseHandle(pi.hThread);
      }

      CloseHandle(hJob);
   }
   else
      dwRet = GetLastError();

   return dwRet;
}
