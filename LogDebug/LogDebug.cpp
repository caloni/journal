/** @file Biblioteca de log para debug.

@author Wanderley Caloni
@date dez.2006
*/
#define _CRT_SECURE_NO_DEPRECATE

#include "logdebug.h"
#include <stdarg.h>
#include <stdio.h>


/** Chama as funções printf e OutputDebugString com a mensagem 
formatada mais caracteres de nova linha (\r\n).

@remark A mensagem não pode ultrapassar 0x500 bytes no total.
*/
void LogDebug(PCSTR format, ...)
{
	va_list vaList;
	CHAR msg[0x500] = { };

	va_start(vaList, format);
	int printCount = vsprintf(msg, format, vaList);
	va_end(vaList);

	strcat(msg, "\r\n");

	OutputDebugString(msg);
	printf(msg);
}
