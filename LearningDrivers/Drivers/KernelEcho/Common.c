/** @file Common.c
 *
 * @brief Funções úteis para todo o projeto.
 * @author Wanderley Caloni <wanderley@caloni.com.br>
 * @date 2008-08
 */
#include "Common.h"
#include <ntddk.h>


VOID __cdecl Log(IN PCH format, ...)
{
	KdPrint(("%s 00000000 %s.\n", DRIVER_NAME, format));
}


VOID __cdecl LogE(IN NTSTATUS errorCode, IN PCH format, ...)
{
	KdPrint(("%s %08X %s.\n", DRIVER_NAME, errorCode, format));
}
