/** @file

Main header of DLL file that (un)installs and handles the msg hook.

@author Wanderley Caloni Jr <wanderley@caloni.com.br>
@date 10/2005
*/
#pragma once
#include <windows.h>

HRESULT WINAPI StartHook();
HRESULT WINAPI StopHook();
