/**
 * My first device driver that doesn't drives anything.
 *
 * @author Wanderley Caloni Jr. <blakhalk@ig.com.br>
 * @date 26-jul-2004
 *
 * For notes about the project environment, see the readme.txt notes.
 *
 * Remember that you can:
 * - Create the driver project in vs as a normal Win32 App.
 * - Rename the final file extension to sys.
 * - Get the headers and libs needed in microsoft ddk.
 * - Set defines _X86_ and _WIN32_WINNT=0x500.
 * - Turn on the ignore all default libraries option (linker).
 * - Include the libs int64.lib, ntoskrnl.lib and hal.lib.
 * - Set the entry point to your driver's DriverEntry function (linker).
 *
 * Remember that you must:
 * - Set the calling convention to __stdcall (compiler).
 * - Set the image base addres to 0x10000 (linker).
 * - Set the /DRIVER, -subsystem:NATIVE,4.00 options (linker).
 * - Put extern "C" in the DriverEntry function and including ddk headers.
 * - Provide a Unload function address for Stop the driver successfully.
 *
 * Tips and Tricks:
 * - If having trouble to link in vc7, try to use the command line option 
 *   /FORCE:UNRESOLVED (linker).
 */
#include "KeHelloWorld.h"

static VOID DriverUnload(IN PDRIVER_OBJECT pDriverObject);



/*
 * And a BSP said: DriverEntry!
 */
extern "C" NTSTATUS
DriverEntry(
   IN PDRIVER_OBJECT pDriverObject,
   IN PUNICODE_STRING RegistryPath
)
{
   pDriverObject->DriverUnload = DriverUnload;

   return STATUS_SUCCESS;
}



/**
 * And a BSP said: Let's go away!
 */
static VOID DriverUnload(
   IN PDRIVER_OBJECT pDriverObject
)
{
}
