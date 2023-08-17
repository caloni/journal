---
categories:
 - reading
date: '2023-08-13T14:10:55-03:00'
draft: true
tags:
 - books
title: Windows Internals (Pavel Yosifovich, Alex Ionescu and David A. Solomon)
---

Windows Internals foi o primeiro e último livro que li sobre a arquitetura Windows. No entanto, não li apenas uma edição. A cada mudança no SO a Microsoft Press encomenda uma atualização deste livro que começou com a autoria de Helen Custer. O livro originalmente se chamava Inside Windows NT e eu li uma tradução para português muito engraçada onde se traduz threads como fios.

A última edição que tenho notícia que dei uma olhada estávamos na fase do Windows XP, e agora que volto para a área de baixarias é hora de ler a última edição, que já dá alguns spoilers de como está se configurando a nova geração do Windows, composta pelo Windows 10 e 11 e suas intermináveis atualizações.

## Recortes

A simple way of thinking about this is as follows: privilege levels (user versus kernel) enforce power. VTLs, on the other hand, enforce isolation. Although a VTL 1 user-mode application is not more powerful than a VTL 0 application or driver, it is isolated from it.

ARM-based platforms paired with a compatible OS kernel scheduler can maximize processing power when needed (by turning on all cores), strike a balance (by having certain big cores online and other little ones for other tasks), or run in extremely low power modes (by having only a single little core online—enough for SMS and push e-mail). By supporting what are called heterogeneous scheduling policies, Windows 10 allows threads to pick and choose between a policy that satisfies their needs, and will interact with the scheduler and power manager to best support it.

All Windows performance counters are accessible programmatically. For more information, search for “performance counters” in the MSDN documentation.

Because LiveKd relies on physical memory to back the simulated dump, the kernel debugger might run into situations in which data structures are in the middle of being changed by the system and are inconsistent. Each time the debugger is launched, it starts with a fresh view of the system state. If you want to refresh the snapshot, enter the q command to quit the debugger. LiveKd will ask you whether you want to start it again.

Because a full checked build was often unstable and impossible to run in most environments, Microsoft provides a checked kernel and HAL only for Windows 10 and later. This enables developers to obtain the same level of usefulness from the kernel and HAL code they interact with without dealing with the issues that a full checked build would cause. This checked kernel and HAL pair is freely available through the WDK, in the \Debug directory of the root installation path.

Because the hypervisor is the first system component to be launched by the boot loader, it can program the SLAT and I/O MMU as it sees fit, defining the VTL 0 and 1 execution environments. Then, while in VTL 1, the boot loader runs again, loading the secure kernel, which can configure the system further to its needs. Only then is the VTL dropped, which will see the execution of the normal kernel, now living in its VTL 0 jail, unable to escape.

Each Trustlet has a unique identifier and signature, and the secure kernel has hard-coded knowledge of which Trustlets have been created so far. As such, it is impossible to create new Trustlets without access to the secure kernel (which only Microsoft can touch), and existing Trustlets cannot be patched in any way (which would void the special Microsoft signature).

For Windows 8 and later guests, you should instead use local network debugging by exposing a host-only network using a virtual NIC in the guest operating system. This will result in 1,000x performance gain.

If you want to dig into the I/O system and driver model beyond what is presented in this book, read the WDK documentation—especially the Kernel-Mode Driver Architecture Design Guide and Kernel-Mode Driver Reference manuals. You might also find useful Programming the Microsoft Windows Driver Model, Second Edition by Walter Oney (Microsoft Press, 2002) and Developing Drivers with the Windows Driver Foundation by Penny Orwick and Guy Smith (Microsoft Press, 2007).

In addition to including the documentation, the WDK contains header files (in particular, ntddk.h, ntifs.h, and wdm.h) that define key internal data structures and constants as well as interfaces to many internal system routines.

In the case of the Process Explorer tool, the start time of the parent process is taken into account to avoid attaching a child process based on a reused process ID.

In this architecture, the secure kernel is its own separate binary, which is found under the name securekernel.exe on disk. As for IUM, it’s both an environment that restricts the allowed system calls that regular user-mode DLLs can make (thus limiting which of these DLLs can be loaded) and a framework that adds special secure system calls that can execute only under VTL 1. These additional system calls are exposed in a similar way as regular system calls: through an internal system library named Iumdll.dll (the VTL 1 version of Ntdll.dll) and a Windows subsystem–facing library named Iumbase.dll (the VTL 1 version of Kernelbase.dll). This implementation of IUM, mostly sharing the same standard Win32 API libraries, allows for the reduction of the memory overhead of VTL 1 user-mode applications because essentially, the same user-mode code is present as in their VTL 0 counterparts. As an important note, copy-on-write mechanisms, which you’ll learn more about in Chapter 5, prevent VTL 0 applications from making changes to binaries used by VTL 1.

LiveKd is a free tool from Sysinternals that enables you to use the standard Microsoft kernel debuggers just described to examine the running system without booting the system in debugging mode. This approach might be useful when kernel-level troubleshooting is required on a machine that wasn’t booted in debugging mode.

Noninvasive With this option, the debugger simply opens the process with the OpenProcess function. It does not attach to the process as a debugger.

Smss.exe is the first user-mode process created (directly by the kernel), so it cannot be dependent on the Windows subsystem because Csrss.exe (the Windows subsystem process) has not started yet.

Some images (executables) don’t belong to any subsystem. In other words, they don’t link against a set of subsystem DLLs, such as Kernel32.dll for the Windows subsystem. Instead, they link only to Ntdll.dll, which is the lowest common denominator that spans subsystems.

Some kernel debugger commands do not work when used in local kernel debugging mode, such as setting breakpoints or creating a memory dump with the .dump command. However, the latter can be done with LiveKd, described later in this section.

Starting with Windows 7, a separate process is used for each console window on the system: the console window host (Conhost.exe). (A single console window can be shared by multiple console applications, such as when you launch a command prompt from the command prompt. By default, the second command prompt shares the console window of the first.)

Strictly speaking, the hypervisor still runs with the same CPU privilege level (0) as the kernel, but because it uses specialized CPU instructions (VT-x on Intel, SVM on AMD), it can both isolate itself from the kernel while also monitoring it (and applications). For these reasons, you may often hear the term ring -1 thrown around (which is inaccurate).

System processes These are fixed, or hardwired, processes, such as the logon process and the Session Manager, that are not Windows services. That is, they are not started by the Service Control Manager.

The real workhorse of Conhost.exe is a DLL it loads (\Windows\System32\ConhostV2.dll) that includes the bulk of code that communicates with the console driver.

The role of subsystem DLLs is to translate a documented function into the appropriate internal (and generally undocumented) native system service calls implemented mostly in Ntdll.dll. This translation might or might not involve sending a message to the environment subsystem process that is serving the user process.

The scheduling algorithms are enhanced to make optimal use of SMT-enabled machines, such as by scheduling threads on an idle physical processor versus choosing an idle logical processor on a physical processor whose other logical processors are busy.

The secure kernel however, by both running at VTL 1 and being in kernel mode, does have complete access to VTL 0 memory and resources. It can use the hypervisor to limit the VTL 0 OS access to certain memory locations by leveraging CPU hardware support known as Second Level Address Translation (SLAT).

The third possible value for Status is Not Responding. This can happen if a thread within the process that created the user interface has not checked its message queue for UI-related activity for at least 5 seconds.

There are four debuggers included in the tools: cdb, ntsd, kd, and WinDbg. All are based on a single debugging engine implemented in DbgEng.dll, which is documented fairly well in the help file for the tools.

There is no built-in tool to display whether you are running the checked build or the retail build (called the free build) of the kernel. However, this information is available through the Debug property of the Windows Management Instrumentation (WMI) Win32_OperatingSystem class.

To prove that Windows doesn’t keep track of more than just the parent process ID, follow these steps: 1. Press WinKey+R, type cmd, and press Enter to open a Command Prompt window. 2. Type title Parent to change the title of the window to Parent. 3. Type start cmd to open a second Command Prompt window. 4. Type title Child in the second Command Prompt window. 5. Type mspaint in the second Command Prompt window to start Microsoft Paint. 6. Go back to the second Command Prompt window and type exit. Notice that Paint remains. 7. Press Ctrl+Shift+Esc to open Task Manager. 8. If Task Manager is in “less details” mode, click More Details. 9. Click the Processes tab. 10. Find the Windows Command Processor app and expand its node. You should see the title Parent, as in the following screenshot: 11. Right-click the Windows Command Processor entry and select Go to details. 12. Right-click this cmd.exe process and select End Process Tree. 13. Click End Process Tree in the Task Manager confirmation dialog box. The first Command Prompt window will disappear, but you should still see the Paint window because it was the grandchild of the command prompt process you terminated. Because the intermediate process (the parent of Paint) was terminated, there was no link between the parent and the grandchild.

To use any of the kernel-debugging tools to examine internal Windows kernel data structures such as the process list, thread blocks, loaded driver list, memory usage information, and so on, you must have the correct symbol files for at least the kernel image, Ntoskrnl.exe.

To view the debug messages, you can either attach a kernel debugger to the target system (which requires booting the target system in debugging mode), use the !dbgprint command while performing local kernel debugging, or use the Dbgview.exe tool from Sysinternals.

Unless specified otherwise, when you attach to a running process, you use the DebugActiveProcess Windows function to establish a connection between the debugger and the debugee.

Windows Performance Analyzer (WPA), formerly known as XPerf

Windows on a NUMA system still runs as an SMP system, in that all processors have access to all memory. It’s just that node-local memory is faster to reference than memory attached to other nodes. The system attempts to improve performance by scheduling threads on processors that are in the same node as the memory being used. It attempts to satisfy memory-allocation requests from within the node, but it will allocate memory from other nodes if necessary.

With Windows 10 version 1607, one such Pico provider is present: Lxss.sys and its partner Lxcore.sys. As the name suggests, this refers to the Windows Subsystem for Linux (WSL) component, and these drivers make up the Pico provider interface for it. Because the Pico provider receives almost all possible transitions to and from user and kernel mode (be they system calls or exceptions, for example), as long as the Pico process (or processes) running underneath it has an address space that it can recognize, and has code that can natively execute within it, the “true” kernel below doesn’t really matter as long as these transitions are handled in a fully transparent way.

With Windows 8 and later, the console architecture changed yet again. The Conhost.exe process remains, but is now spawned from the console-based process (rather than from Csrss.exe, as in Windows 7) by the console driver (\Windows\System32\Drivers\ConDrv.sys). The process in question communicates with Conhost.exe using the console driver (ConDrv.sys), by sending read, write, I/O control and other I/O request types. Conhost.exe is designated as a server and the process using the console is the client. This change obviates the need for Csrss.exe to receive keyboard input (as part of the raw input thread), send it through Win32k.sys to Conhost.exe, and then use ALPC to send it to Cmd.exe. Instead, the command-line application can directly receive input from the console driver through read/write I/Os, avoiding needless context switching.

With additional hardware changes to various buses such as PCI and USB, it will soon be possible to support an entire class of secure devices, which, when combined with a minimalistic secure HAL, secure Plug-and-Play manager, and secure User-Mode Device Framework, could allow certain VTL 1 applications direct and segregated access to specially designated devices, such as for biometric or smartcard input.

You can configure the system to boot in debugging mode using Bcdedit.exe or Msconfig.exe. (Note that you may have to disable secure boot in the UEFI BIOS settings.)

You can show more columns by right-clicking the header. Available columns are Process (Image) Name, Process ID, Type, Status, Publisher, and Command Line.

You can use the SlPolicy tool from the downloads available for the book to display many of these policy values.

You can use the Tlist.exe tool in the Debugging Tools for Windows to show the process tree by using the /t switch.

for Windows Apps (those hosting the Windows Runtime), Suspended normally occurs when the app loses its foreground status by being minimized by the user. Such processes are suspended after 5 seconds so that they don’t consume any CPU or networking resources, thus allowing the new foreground app to get all machine resources. This is especially important for battery-powered devices, such as tablets and phones.

the Windows subsystem is a required component for any Windows system, even on server systems with no interactive users logged in. Because of this, the process is marked as a critical process (which means if it exits for any reason, the system crashes).

The kernel separates itself from the rest of the executive by implementing OS mechanisms and avoiding policy making. It leaves nearly all policy decisions to the executive, with the exception of thread scheduling and dispatching, which the kernel implements.

Most executive-level objects encapsulate one or more kernel objects, incorporating their kernel-defined attributes.

The kernel uses a data structure called the kernel processor control region (KPCR) to store processor-specific data.

To provide easy access to the KPCR, the kernel stores a pointer to it in the fs register on 32-bit Windows and in the gs register on an x64 Windows system.

Finally, WDF has been open-sourced by Microsoft, and at the time of this writing is available on GitHub at https://github.com/Microsoft/Windows-Driver-Frameworks.

To list the installed drivers, run the System Information tool (Msinfo32.exe). To launch this tool, click Start and then type Msinfo32 to locate it. Under System Summary, expand Software Environment and open System Drivers.

Alternatively, you can list the currently loaded device drivers by selecting the System process in Process Explorer and opening the DLL view.

When you’re troubleshooting or going through a system analysis, it’s useful to be able to map the execution of individual system threads back to the driver or even to the subroutine that contains the code. For example, on a heavily loaded file server, the System process will likely consume considerable CPU time. But knowing that when the System process is running, “some system thread” is running isn’t enough to determine which device driver or OS component is running.

So if threads in the System process are running, first determine which ones are running (for example, with the Performance Monitor or Process Explorer tools). Once you find the thread (or threads) that is running, look up in which driver the system thread began execution. This at least tells you which driver likely created the thread. For example, in Process Explorer, right-click the System process and select Properties. Then, in the Threads tab, click the CPU column header to view the most active thread at the top. Select this thread and click the Module button to see the file from which the code on the top of stack is running. Because the System process is protected in recent versions of Windows, Process Explorer is unable to show a call stack.

The master Smss.exe performs the following one-time initialization steps: 1. It marks the process and the initial thread as critical. If a process or thread marked critical exits for any reason, Windows crashes.

Windows 10 provides a biometric credential provider: face recognition, known as Windows Hello.

Upon successful authentication, Lsass.exe calls a function in the SRM (for example, NtCreateToken) to generate an access token object that contains the user’s security profile. If User Account Control (UAC) is used and the user logging on is a member of the administrators group or has administrator privileges, Lsass.exe will create a second, restricted version of the token. This access token is then used by Winlogon to create the initial process(es) in the user’s session.
