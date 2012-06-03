/** @file KernelEcho.h
 *
 * @brief Estruturas e prot�tipos.
 * @author Wanderley Caloni <wanderley@caloni.com.br>
 * @date 2008-08
 */
#pragma once
#include <ntddk.h>


/** @brief Defini��o para o tipo para ser armazenado em nossa
* lista de buffers.
*/
typedef struct _BUFFER_ENTRY
{
	PVOID buffer;     ///< Buffer enviado.
	ULONG size;       ///< Tamanho do buffer.
	LIST_ENTRY entry; ///< N� para a lista.

} BUFFER_ENTRY, *PBUFFER_ENTRY;


//
// Prot�tipos
//
VOID OnDriverUnload(IN PDRIVER_OBJECT pDriverObject);
NTSTATUS OnCreateOnClose(IN PDEVICE_OBJECT devObj, IN PIRP irp);
NTSTATUS OnWrite(IN PDEVICE_OBJECT devObj, IN PIRP irp);
NTSTATUS OnRead(IN PDEVICE_OBJECT devObj, IN PIRP irp);
