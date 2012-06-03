#include "stdafx.h"



VOID
KeCreateDeviceUnload(
   IN PDRIVER_OBJECT pDriverObject
)
{
   PDEVICE_OBJECT pDevObj = pDriverObject->DeviceObject;

   while( pDevObj != NULL )
   {
      PDEVICE_OBJECT pDevObjTmp = pDevObj->NextDevice;
      IoDeleteDevice(pDevObj);
      pDevObj = pDevObjTmp;
   }
   
   //TODO: Organize the symbolic link name.
   UNICODE_STRING wsSymbLinkName;
   RtlInitUnicodeString(&wsSymbLinkName, L"\\??\\KCD1");
   IoDeleteSymbolicLink(&wsSymbLinkName);

}



extern "C"
NTSTATUS
DriverEntry(
   IN PDRIVER_OBJECT pDriverObject,
   IN PUNICODE_STRING pRegisterPath
)
{
   NTSTATUS ntsRet;
   PDEVICE_OBJECT pDeviceObject;
   
   UNICODE_STRING wsDeviceName;
   RtlInitUnicodeString(&wsDeviceName, L"\\Device\\KeCreateDevice0");

   pDriverObject->DriverUnload = KeCreateDeviceUnload;

   ntsRet =
   IoCreateDevice(pDriverObject,
                  0,
                  &wsDeviceName,
                  FILE_DEVICE_UNKNOWN,
                  FILE_DEVICE_SECURE_OPEN,
                  FALSE,
                  &pDeviceObject);
                  
   if( NT_SUCCESS(ntsRet) )
   {
      UNICODE_STRING wsSymbLinkName;
      RtlInitUnicodeString(&wsSymbLinkName, L"\\??\\KCD1");

      ntsRet =
      IoCreateSymbolicLink(&wsSymbLinkName, &wsDeviceName);
      
      if( NT_ERROR(ntsRet) )
         IoDeleteDevice(pDeviceObject);
   }

   return ntsRet;
}
