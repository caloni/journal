#ifndef __BABELBASE_H_8E80D150_4768_448E_ACFE_3BF63267C0A2__
#define __BABELBASE_H_8E80D150_4768_448E_ACFE_3BF63267C0A2__

#include "..\..\BabelBaseLib\BabelBaseLib.h"

#define BABELBASE_DLL "BabelBase.dll"

#define BABELBASE_FUNC_GETTRANSLATOR "GetTranslator"
#define BABELBASE_FUNC_FREETRANSLATOR "FreeTranslator"


namespace Babel
{

	/// Aloca um novo definidor de palavras.
	typedef ITranslator* (FGetTranslator)();

	/// Desaloca um definidor de palavras pré-alocado.
    typedef void (FFreeTranslator)(ITranslator* translator);

}

#endif // __BABELBASE_H_8E80D150_4768_448E_ACFE_3BF63267C0A2__
