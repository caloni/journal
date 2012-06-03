/** @file Conversor.h

@brief Conversor de Houaiss para Babylon.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 2008-04

@desc Permite que os arquivos do Houaiss sejam usados para construir
um dicion�rio do Babylon atrav�s do Babylon Builder.

Linque:
Houaiss: http://www.dicionariohouaiss.com.br
Babylon: http://www.babylon.com
Babylon Builder: http://www.babylon.com/display.php?id=15&tree=3&level=2

Obs.:
Esse programa n�o se destina � pirataria. Ele n�o fornece o dicion�rio, 
que devem ser adquirido legalmente. Sua �nica fun��o � permitir o uso 
de um �timo dicion�rios independente de sua �nica interface.

Bom proveito!
*/
#pragma once
#include "Common.h"
#include <windows.h>

DWORD Conversor(H2BFiles* files, H2BOperation& op);
