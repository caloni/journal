#pragma once

#include <windows.h>

namespace WaitJob
{

/** Initialization function. Must be called before using WaitJob. */
DWORD Initializate();

/** Termination function. Must be called after using WaitJob. */
DWORD Terminate();

/** A simplified CreateProcess(). */
DWORD CreateProc(PROCESS_INFORMATION *ppi, LPTSTR tzProcFile, DWORD dwFlags);

/** Wait for the process and its created child processes. */
DWORD WaitAndReleaseProc(LPPROCESS_INFORMATION ppi);

/** Wait for the process and its created child processes. */
DWORD WaitAndReleaseChildProcs(LPPROCESS_INFORMATION ppi);

} //namespace WaitJob
