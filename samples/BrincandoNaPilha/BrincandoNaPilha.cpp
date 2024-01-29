#include <stdarg.h>

int soma(int argc, ...)
{
	int ret = 0;
	int* argv = &argc + 1;

	while ( argc-- )
	{
		int next = *argv++;
		ret += next;
	}

	return ret;
}
