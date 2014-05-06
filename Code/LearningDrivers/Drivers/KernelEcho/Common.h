/** @file Common.h
 *
 * @brief Funções e defines úteis para todo o projeto.
 * @author Wanderley Caloni <wanderley@caloni.com.br>
 * @date 2008-08
 */
#pragma once
#include <ntddk.h>

#define DRIVER_NAME "KernelEcho"
#define TAG_NAME 'ohcE'

#define RAISE_IF_ERROR(ntStatus)\
	if( ! NT_SUCCESS(ntStatus) ) ExRaiseStatus(ntStatus)

#define LOG(args) Log args
#define LOGE(args) LogE args


VOID __cdecl Log(IN PCH format, ...);
VOID __cdecl LogE(IN NTSTATUS errorCode, IN PCH format, ...);
