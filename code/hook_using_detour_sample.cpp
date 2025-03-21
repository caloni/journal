#include "pch.h"
#include <detours.h>
#include <windows.h>
#include <wincred.h>
#include <strsafe.h>
#include <mutex>
#include <vector>

using namespace std;

decltype(ChangeWindowMessageFilter)* Orig_ChangeWindowMessageFilter = ::ChangeWindowMessageFilter;
decltype(ChangeWindowMessageFilterEx)* Orig_ChangeWindowMessageFilterEx = ::ChangeWindowMessageFilterEx;
decltype(SendMessageA)* Orig_SendMessageA = ::SendMessageA;
decltype(SendMessageW)* Orig_SendMessageW = ::SendMessageW;
decltype(GetMessageA)* Orig_GetMessageA = ::GetMessageA;
decltype(GetMessageW)* Orig_GetMessageW = ::GetMessageW;
decltype(PeekMessageA)* Orig_PeekMessageA = ::PeekMessageA;
decltype(PeekMessageW)* Orig_PeekMessageW = ::PeekMessageW;
decltype(CreateFileW)* Orig_CreateFileW = ::CreateFileW;
decltype(ReadFile)* Orig_ReadFile = ::ReadFile;
decltype(CredReadA)* Orig_CredReadA = ::CredReadA;
decltype(CredReadW)* Orig_CredReadW = ::CredReadW;


LRESULT WINAPI Hook_SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI Hook_SendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL WINAPI Hook_ChangeWindowMessageFilter(UINT message, DWORD dwFlag);
BOOL WINAPI Hook_ChangeWindowMessageFilterEx(HWND hwnd, UINT message, DWORD action, PCHANGEFILTERSTRUCT pChangeFilterStruct);
BOOL WINAPI Hook_GetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
BOOL WINAPI Hook_GetMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
BOOL WINAPI Hook_PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
BOOL WINAPI Hook_PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
HANDLE WINAPI Hook_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
BOOL WINAPI Hook_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
BOOL WINAPI Hook_CredReadW (LPCWSTR TargetName, DWORD Type, DWORD Flags, PCREDENTIALW *Credential);
BOOL WINAPI Hook_CredReadA (LPCSTR TargetName, DWORD Type, DWORD Flags, PCREDENTIALA *Credential);

struct Hook
{
    PVOID& original;
    PVOID hook;
};

static Hook g_installedHooks[] =
{
    //{ (PVOID&)Orig_ChangeWindowMessageFilter, Hook_ChangeWindowMessageFilter },
    //{ (PVOID&)Orig_ChangeWindowMessageFilterEx, Hook_ChangeWindowMessageFilterEx },
    //{ (PVOID&)Orig_SendMessageA, Hook_SendMessageA },
    //{ (PVOID&)Orig_SendMessageW, Hook_SendMessageW },
    //{ (PVOID&)Orig_SendMessageW, Hook_SendMessageW },
    //{ (PVOID&)Orig_GetMessageA, Hook_GetMessageA },
    //{ (PVOID&)Orig_GetMessageW, Hook_GetMessageW },
    //{ (PVOID&)Orig_PeekMessageA, Hook_PeekMessageA },
    //{ (PVOID&)Orig_PeekMessageW, Hook_PeekMessageW },
    //{ (PVOID&)Orig_CreateFileW, Hook_CreateFileW },
    //{ (PVOID&)Orig_ReadFile, Hook_ReadFile },
    { (PVOID&)Orig_CredReadA, Hook_CredReadA },
    { (PVOID&)Orig_CredReadW, Hook_CredReadW },
};

static std::mutex g_logLock{};
static std::wstring g_hookFileName = L"C:\\temp\\hook.log";

static std::string GetLocalTimeStamp()
{
    static constexpr size_t timestamp_size = 23; // "YYYY-MM-DD hh:mm:ss.fff"
    char timeStamp[timestamp_size + 1]; // +1 for null terminator
    
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf_s(timeStamp, sizeof(timeStamp),
        "%04d-%02d-%02d %02d:%02d:%02d.%03d",
        st.wYear, st.wMonth, st.wDay,
        st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    return std::string(timeStamp, timestamp_size);
}

static const char* GetProcessName()
{
    static char st_processName[MAX_PATH]{};
    if( !*st_processName )
    {
        GetModuleFileNameA(NULL, st_processName, MAX_PATH);
        if( char* slash = strrchr(st_processName, '\\') )
        {
            memmove(st_processName, slash + 1, strlen(slash));
        }
    }
    return st_processName;
}

static void LogWrite(const char* str)
{
    HANDLE h = Orig_CreateFileW(g_hookFileName.c_str(), FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if( h != INVALID_HANDLE_VALUE )
    {
        char buf[500];
        //sprintf_s(buf, "%s [%d:%d] [%s] %s\r\n", GetLocalTimeStamp().c_str(), static_cast<int>(GetCurrentProcessId()), static_cast<int>(GetCurrentThreadId()), GetProcessName(), str);
        sprintf_s(buf, "[%s] %s\r\n", GetProcessName(), str);
        DWORD bufLen = (DWORD) strlen(buf);
        DWORD written = 0;
        if( WriteFile(h, buf, bufLen, &written, NULL) && bufLen == written )
        {
            SetFilePointer(h, 0, NULL, FILE_END);
        }
        CloseHandle(h);
    }
}

template <typename... Args>
void LogFormat(const char* format, Args... args)
{
    HRESULT hr;
    std::vector<char> buf(256);
    do
    {
        hr = StringCbPrintfA(buf.data(), buf.size() * sizeof(char), format, args...);
        if (hr == S_OK)
        {
            break;
        }
        if (hr == STRSAFE_E_INVALID_PARAMETER)
        {
            return;
        }
        buf.resize(buf.size() * 2);

    } while (hr == STRSAFE_E_INSUFFICIENT_BUFFER);

    LogWrite(buf.data());
}

LRESULT WINAPI Hook_SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT l = Orig_SendMessageA(hWnd, Msg, wParam, lParam);
    LogFormat("Hook_SendMessageA %x %d", hWnd, Msg);
    return l;
}

LRESULT WINAPI Hook_SendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT l = Orig_SendMessageW(hWnd, Msg, wParam, lParam);
    LogFormat("Hook_SendMessageW %x %d", hWnd, Msg);
    return l;
}

BOOL WINAPI Hook_ChangeWindowMessageFilter(UINT message, DWORD dwFlag)
{
    BOOL l = Orig_ChangeWindowMessageFilter(message, dwFlag);
    LogFormat("Hook_ChangeWindowMessageFilter %d %x", message, dwFlag);
    return l;
}

BOOL WINAPI Hook_ChangeWindowMessageFilterEx(HWND hwnd, UINT message, DWORD action, PCHANGEFILTERSTRUCT pChangeFilterStruct)
{
    BOOL l = Orig_ChangeWindowMessageFilterEx(hwnd, message, action, pChangeFilterStruct);
    LogFormat("Hook_ChangeWindowMessageFilterEx %x %d %x", hwnd, message, action);
    return l;
}

BOOL WINAPI Hook_GetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
    BOOL l = Orig_GetMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
    LogFormat("Hook_GetMessageA %d %x %x", l, hWnd, lpMsg ? lpMsg->message : 0);
    return l;
}

BOOL WINAPI Hook_GetMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
    BOOL l = Orig_GetMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
    LogFormat("Hook_GetMessageW %d %x %x", l, hWnd, lpMsg ? lpMsg->message : 0);
    return l;
}

BOOL WINAPI Hook_PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
    BOOL l = Orig_PeekMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
    LogFormat("Hook_PeekMessageA %d %x %x", l, hWnd, lpMsg ? lpMsg->message : 0);
    return l;
}

BOOL WINAPI Hook_PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
    BOOL l = Orig_PeekMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
    LogFormat("Hook_PeekMessageW %d %x %x", l, hWnd, lpMsg ? lpMsg->message : 0);
    return l;
}

HANDLE WINAPI Hook_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    HANDLE h = Orig_CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    LogFormat("Hook_CreateFileW %S", lpFileName, h);
    return h;
}

BOOL WINAPI Hook_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
    BOOL l = Orig_ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
    //LogFormat("Hook_ReadFile %x", hFile);
    return l;
}

BOOL WINAPI Hook_CredReadW (LPCWSTR TargetName, DWORD Type, DWORD Flags, PCREDENTIALW *Credential)
{
    BOOL l = Orig_CredReadW(TargetName, Type, Flags, Credential);
    LogFormat("Hook_CredReadW %S", TargetName);
    return l;
}

BOOL WINAPI Hook_CredReadA (LPCSTR TargetName, DWORD Type, DWORD Flags, PCREDENTIALA *Credential)
{
    BOOL l = Orig_CredReadA(TargetName, Type, Flags, Credential);
    LogFormat("Hook_CredReadA %s", TargetName);
    return l;
}

static void InstallHooks()
{
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    for (auto& hook : g_installedHooks)
    {
        DetourAttach(&(PVOID&)hook.original, hook.hook);
    }
    DetourTransactionCommit();
}

static void UninstallHooks()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    for (auto& hook : g_installedHooks)
    {
        DetourDetach(&hook.original, hook.hook);
    }
    DetourTransactionCommit();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (DetourIsHelperProcess())
    {
        return TRUE;
    }
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        //MessageBoxA(NULL, "Ready to install hooks", "Dll1", 0);
        LogFormat("Installing hooks");
        InstallHooks();
        LogFormat("Hooks installed");
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        LogFormat("Uninstalling hooks");
        UninstallHooks();
        LogFormat("Hooks uninstalled");
    }
    return TRUE;
}

