---
categories: null
date: '2023-06-09T21:31:30-03:00'
draft: true
tags: null
title: Critical Sections and Outlook
---

Today was a happy crush analysis day, because I receive an Outlook memory dump the moment it frooze the main window in the beginning of the process. I asked for one because it could be useful to another analysis I was doing with Office server communication.

I discovered that the main window thread was waiting for a critical section used by a part of the Office code called `OsfIdentityManager` when calling the function `EnsureInit`. The code responsible for acquire the lock is associated with the name `OsfLock`. The same cs was trying to be acquired by another thread. Looking at another similar threads it seems a group of worker threads starting at the entry `mso20win32client!CIOPort::ThreadProc`. Bellow is the result of the `!cs` command in WinDbg:

```
DebugInfo          = 0x00000195b9ce4f00
Critical section   = 0x0000019581b2d590 (+0x19581B2D590)
LOCKED
LockCount          = 0x2
WaiterWoken        = No
OwningThread       = 0x0000000000018e9c
RecursionCount     = 0x1
LockSemaphore      = 0xFFFFFFFF
SpinCount          = 0x00000000020007d0
```

The moment the dump was generated the thread that had acquired this cs was inside the same function `OsfIdentityManager::EnsureInit` and several frames above was calling the native API `ntdll!NtQueryFullAttributesFile`, when I discovered that this application is using [Microsoft Application Virtualization (App-V)](https://learn.microsoft.com/en-us/windows/application-management/app-v/appv-getting-started) based on the hook it was installed:

```
. 58  Id: 661c.18e9c Suspend: -1 Teb: 0000003c`34078000 Unfrozen
      Start: mso20win32client!CIOPort::ThreadProc (00007ff8`70ba3ef0)
      Priority: 0  Priority class: 32  Affinity: fffff
 # Child-SP          RetAddr               Call Site
00 0000003c`382fedc8 00007ff9`089c52a8     ntdll!NtQueryFullAttributesFile+0x14
01 0000003c`382fedd0 00007ff9`089b9483     AppVIsvSubsystems64!vfs_query_full_attributes_request::pass_through+0x28
02 0000003c`382fee00 00007ff9`089b384b     AppVIsvSubsystems64!AppV::VFS::Filesys::virtualize_nt_query_full_attributes+0xc3
03 0000003c`382ff050 00007ff9`089b61fe     AppVIsvSubsystems64!vfs_hooks::NtQueryFullAttributesFile+0xeb
04 0000003c`382ff100 00007ff9`7774849f     AppVIsvSubsystems64!vfs_hooks::hooked_NtQueryFullAttributesFile+0x2e
05 0000003c`382ff130 00007ff8`70b957c0     KERNELBASE!GetFileAttributesExW+0x9f
06 0000003c`382ff230 00007ff8`70b95779     mso20win32client!MsoGetFileAttributesW+0x20
...
```

> Virtual applications are installed on centrally managed servers and delivered to users as a service in real time and on an as-needed basis. Users launch virtual applications from familiar access points and interact with them as if they were installed locally.

Considering that that is the thread that is locking the cs that is freezing other two threads, including the main window thread, I believe that somehow there is a connection with the delivery service from the App-V with the Outlook slowliness when opening or perhaps in other moments where a unavailable module is requests. Results from the web have the usual troubleshooting sugestions as restore Office installation, run in safe mode and disable AppData network redirection, but I am not sure if this is the case.

Two main commands used in WinDbg for the analysis was `!uniqstack` to show the... unique stacks in the process, and `!cs`, to show all critical sections and its states.