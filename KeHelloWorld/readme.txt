/**
 * NOTES ABOUT THE PROJECT ENVIRONMENT
 *
 * Remember that you can:
 * - Create the driver project in vs as a normal Win32 App.
 * - Rename the final file extension to sys.
 * - Get the headers and libs needed in microsoft ddk.
 * - Define _WIN32_WINNT=0x500 (by example).
 * - Turn on the ignore all default libraries option (linker).
 * - Include the libs int64.lib, ntoskrnl.lib and hal.lib (by example).
 * - Set the entry point to your driver's DriverEntry function (linker).
 * - Set the /DRIVER option (linker, command line).
 *
 * Remember that you must:
 * - Define _X86_.
 * - Set the calling convention to __stdcall (compiler).
 * - Set the image base addres to 0x10000 or allows relocation (linker).
 * - Set the -subsystem:NATIVE,4.00 option (linker, command line).
 * - Put extern "C" in the DriverEntry function (if C++ compilation).
 * - Put extern "C" when including ddk headers (if C++ compilation).
 * - Provide a Unload function address for Stop the driver successfully.
 *
 * Tips and Tricks:
 * - If having trouble to link in vc7, try to use the command line option 
 *   /FORCE:UNRESOLVED (linker).
 */
