#pragma once


/** Retorna o tamanho do array, em número de elementos (e não em bytes).
*/
template<typename ArrayT, unsigned long Size>
unsigned long SizeofArray(const ArrayT (&arg)[Size])
{
	return Size;
}
