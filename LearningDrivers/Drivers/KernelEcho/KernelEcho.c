/** @file KernelEcho.c
 *
 * @brief Projeto de testes que suporta WriteFile e ReadFile.
 * @author Wanderley Caloni <wanderley@caloni.com.br>
 * @date 2008-08
 */
#include "KernelEcho.h"
#include "Common.h"
#include <ntddk.h>


//
// Vari�veis globais.
//
LIST_ENTRY g_bufferList;  ///< Ponta da lista de buffers enviados.
KMUTEX g_bufferListLock;  ///< Prote��o de sincronismo da lista g_BufferList.

/// Nome do dispositivo.
UNICODE_STRING g_deviceName 
	= RTL_CONSTANT_STRING(L"\\Device\\EchoDevice");

/// Nome do link simb�lico do dispositivo, vis�vel em userland.
UNICODE_STRING g_symbolicLink
	= RTL_CONSTANT_STRING(L"\\DosDevices\\EchoDevice");


/** @brief Ponto de entrada do nosso driver.
 * Inicializa vari�veis globais e fun��es de callback.
 */
NTSTATUS DriverEntry(IN PDRIVER_OBJECT drvObj, 
		IN PUNICODE_STRING pusRegistryPath)
{
	NTSTATUS ret = STATUS_SUCCESS;
	PDEVICE_OBJECT devObj = NULL;

	__try
	{
		LOG(("Starting driver"));

		// Inicializando vari�veis globais.
		InitializeListHead(&g_bufferList);
		KeInitializeMutex(&g_bufferListLock, 0);

		// Rotinas de callback.
		drvObj->DriverUnload = OnDriverUnload;
		drvObj->MajorFunction[IRP_MJ_CREATE] = OnCreateOnClose;
		drvObj->MajorFunction[IRP_MJ_CLOSE] = OnCreateOnClose;
		drvObj->MajorFunction[IRP_MJ_WRITE] = OnWrite;
		drvObj->MajorFunction[IRP_MJ_READ] = OnRead;

		ret = IoCreateDevice(drvObj, 0, &g_deviceName, 
			FILE_DEVICE_UNKNOWN, 0, FALSE, &devObj);
		RAISE_IF_ERROR(ret);

		devObj->Flags |= DO_BUFFERED_IO;

		ret = IoCreateSymbolicLink(&g_symbolicLink, &g_deviceName);
		RAISE_IF_ERROR(ret);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		ret = GetExceptionCode();
		LOGE((ret, "Exception at " __FUNCTION__));

		if( devObj )
			IoDeleteDevice(devObj);
	}

	return ret;
}


/** @brief Fun��o de callback que trata das finaliza��es necess�rias.
 */
VOID OnDriverUnload(IN PDRIVER_OBJECT drvObj)
{
	NTSTATUS ret = STATUS_SUCCESS;
	PLIST_ENTRY listEntry;
	PBUFFER_ENTRY bufferEntry;

	LOG(("Unloading driver"));

	while( ! IsListEmpty(&g_bufferList) )
	{
		listEntry = RemoveHeadList(&g_bufferList);
		bufferEntry = CONTAINING_RECORD(listEntry, BUFFER_ENTRY, entry);
		ExFreePool(bufferEntry);
	}

	ret = IoDeleteSymbolicLink(&g_symbolicLink);
	IoDeleteDevice(drvObj->DeviceObject);
}


NTSTATUS OnCreateOnClose(IN PDEVICE_OBJECT devObj, IN PIRP irp)
{
	NTSTATUS ret = STATUS_SUCCESS;

	irp->IoStatus.Status = ret;
	irp->IoStatus.Information = 0;

	return ret;
}


NTSTATUS OnWrite(IN PDEVICE_OBJECT devObj, IN PIRP irp)
{
	NTSTATUS ret = STATUS_SUCCESS;
	PIO_STACK_LOCATION stackLocation = IoGetCurrentIrpStackLocation(irp);
	PVOID userBuffer = irp->AssociatedIrp.SystemBuffer;
	ULONG userBufferSz = stackLocation->Parameters.Write.Length;
	PBUFFER_ENTRY bufferEntry = NULL;
	BOOLEAN lockAcquired = FALSE;

	__try
	{
		ASSERT( userBuffer != NULL );

		LOG(("Writing to the device"));

		bufferEntry = ExAllocatePoolWithTag(PagedPool, 
			sizeof(BUFFER_ENTRY) + userBufferSz, TAG_NAME);
		if( ! bufferEntry ) ret = STATUS_NO_MEMORY;
		RAISE_IF_ERROR(ret);

		bufferEntry->buffer = &bufferEntry[1];
		bufferEntry->size = userBufferSz;
		RtlCopyMemory(bufferEntry->buffer, userBuffer, userBufferSz);

		ret = KeWaitForMutexObject(&g_bufferListLock, UserRequest, 
			KernelMode, FALSE, NULL);
		RAISE_IF_ERROR(ret);

		lockAcquired = TRUE;
		InsertTailList(&g_bufferList, &bufferEntry->entry);

		irp->IoStatus.Information = userBufferSz;
		ret = STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		ret = GetExceptionCode();
		LOGE((ret, "Exception ocurred at " __FUNCTION__));

		if( bufferEntry )
			ExFreePool(bufferEntry);
		irp->IoStatus.Information = 0;
	}

	if( lockAcquired )
		KeReleaseMutex(&g_bufferListLock, FALSE);

	irp->IoStatus.Status = ret;
	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return ret;
}


NTSTATUS OnRead(IN PDEVICE_OBJECT devObj, IN PIRP irp)
{
	NTSTATUS ret = STATUS_SUCCESS;
	PIO_STACK_LOCATION stackLocation = IoGetCurrentIrpStackLocation(irp);
	PVOID userBuffer = irp->AssociatedIrp.SystemBuffer;
	ULONG userBufferSz = stackLocation->Parameters.Read.Length;
	PLIST_ENTRY entry;
	PBUFFER_ENTRY bufferEntry = NULL;
	BOOLEAN lockAcquired = FALSE;

	__try
	{
		ASSERT( userBuffer != NULL );

		LOG(("Reading from the device"));

		ret = KeWaitForMutexObject(&g_bufferListLock, 
			UserRequest, KernelMode, FALSE, NULL);
		RAISE_IF_ERROR(ret);
		lockAcquired = TRUE;

		if( IsListEmpty(&g_bufferList) )
			ExRaiseStatus(STATUS_NO_MORE_ENTRIES);

		entry = RemoveHeadList(&g_bufferList);
		bufferEntry = CONTAINING_RECORD(entry, BUFFER_ENTRY, entry);

		if( userBufferSz < bufferEntry->size )
			ExRaiseStatus(STATUS_BUFFER_TOO_SMALL);

		RtlCopyMemory(userBuffer, bufferEntry->buffer, bufferEntry->size);
		irp->IoStatus.Information = bufferEntry->size;

		ExFreePool(bufferEntry);
		ret = STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		ret = GetExceptionCode();

		if( ret != STATUS_NO_MORE_ENTRIES || ret != STATUS_BUFFER_TOO_SMALL )
			LOGE((ret, "Exception at " __FUNCTION__));

		if( bufferEntry )
			InsertHeadList(&g_bufferList, &bufferEntry->entry);

		irp->IoStatus.Information = 0;
	}

	if( lockAcquired )
		KeReleaseMutex(&g_bufferListLock, FALSE);

	irp->IoStatus.Status = ret;
	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return ret;
}
