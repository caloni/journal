/** @file Biblioteca de log para debug.

@author Wanderley Caloni
@date dez.2006
*/
#pragma once
#include <windows.h>

/** @brief Envia mensagem formatada para o console e para a saída de 
debug do sistema (OutputDebugString).*/
void LogDebug(PCSTR format, ...);
