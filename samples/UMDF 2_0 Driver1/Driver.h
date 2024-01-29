/*++

Module Name:

    driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    User-mode Driver Framework 2.0

--*/

#define INITGUID

#include <windows.h>
#include <wdf.h>

#include "device.h"
#include "queue.h"
#include "trace.h"

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD UMDF2_0Driver1EvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP UMDF2_0Driver1EvtDriverContextCleanup;

