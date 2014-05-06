/**
 * Code execution in another process context.
 *
 * @date 05/2004.
 * @author Wanderley Caloni Jr. <blakhalk@ig.com.br>
 */
#ifndef _RMTHREAD_H_A8323889_11DC_492e_A5BB_232B6D87E41A_
#define _RMTHREAD_H_A8323889_11DC_492e_A5BB_232B6D87E41A_

#include <tchar.h>
#include <windows.h>



/**
 * Prototypes.
 */

/** Run process and get rights for running remote threads. */
HANDLE CreateAndGetProcessGodHandle(LPCTSTR lpApplicationName, 
   LPTSTR lpCommandLine);

/** Load DLL in another process. */
HMODULE RemoteLoadLibrary(HANDLE hProcess, LPCTSTR lpFileName);

/** Free DLL in another process. */
BOOL RemoteFreeLibrary(HANDLE hProcess, HMODULE hModule);


#endif //_RMTHREAD_H_A8323889_11DC_492e_A5BB_232B6D87E41A_
