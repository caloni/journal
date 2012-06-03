/** @file KernelEcho.h
 *
 * @brief Estruturas e protótipos.
 * @author Wanderley Caloni <wanderley@caloni.com.br>
 * @date 2008-08
 */
#pragma once
#include <ntddk.h>


/** @brief Definição para o tipo para ser armazenado em nossa
* lista de buffers.
*/
typedef struct _BUFFER_ENTRY
{
	PVOID buffer;     ///< Buffer enviado.
	ULONG size;       ///< Tamanho do buffer.
	LIST_ENTRY entry; ///< Nó para a lista.

} BUFFER_ENTRY, *PBUFFER_ENTRY;


//
// Protótipos
//
VOID OnDriverUnload(IN PDRIVER_OBJECT pDriverObject);
NTSTATUS OnCreateOnClose(IN PDEVICE_OBJECT devObj, IN PIRP irp);
NTSTATUS OnWrite(IN PDEVICE_OBJECT devObj, IN PIRP irp);
NTSTATUS OnRead(IN PDEVICE_OBJECT devObj, IN PIRP irp);
