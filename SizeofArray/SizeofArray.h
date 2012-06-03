#pragma once


/** Retorna o tamanho do array, em n�mero de elementos (e n�o em bytes).
*/
template<typename ArrayT, unsigned long Size>
unsigned long SizeofArray(const ArrayT (&arg)[Size])
{
	return Size;
}
