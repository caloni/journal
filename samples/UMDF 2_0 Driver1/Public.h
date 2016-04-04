/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    driver and application

--*/

//
// Define an Interface Guid so that app can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_UMDF2_0Driver1,
    0x60595ccf,0xbbec,0x4e1d,0xb7,0x06,0xa5,0xd0,0x7e,0xed,0x7b,0x9b);
// {60595ccf-bbec-4e1d-b706-a5d07eed7b9b}
