/** @file Decryptor.h

@brief Desofuscador do dicion�rio Houaiss.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 2008-04

@desc Permite usar o conte�do dos arquivos do dicion�rio eletr�nico Houaiss 
por meio da interface do dicion�rio Babylon, independente
da interface oficial.

Linque:
Houaiss: http://www.dicionariohouaiss.com.br

Obs.:
Esse programa n�o se destina � pirataria. Ele n�o fornece nenhum dos
dois dicion�rios, que devem ser adquiridos legalmente. Sua �nica
fun��o � permitir o uso de um �timo dicion�rios em uma excelente
interface de busca de significado de palavras.

Bom proveito!
*/
#pragma once
#include "Common.h"
#include <windows.h>

DWORD Decryptor(CString houaissFileName, H2BFiles* files, H2BOperation& op);
