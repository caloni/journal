#pragma once

namespace WaitJob
{

DWORD CreateJob(LPTSTR tzProcFile);
DWORD WaitJob(HANDLE hJob);
void PrintJobMsg(DWORD dwMsg);

} //namespace WaitJob
