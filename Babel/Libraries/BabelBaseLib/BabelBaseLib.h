/** @file BabelBaseLib.h

@brief Biblioteca básica do projeto Babel.

@author Wanderley Caloni <wanderley@caloni.com.br>
@date 2009-06
*/
#ifndef __BABELBASELIB_H_8E80D150_4768_448E_ACFE_3BF63267C0A2__
#define __BABELBASELIB_H_8E80D150_4768_448E_ACFE_3BF63267C0A2__

#define BABEL_FILE "Babel.dat"

#define BABEL_MAX_WORD_SIZE 200
#define BABEL_MAX_DEFINITION_SIZE 4000
#define BABEL_MAX_SOURCE_SIZE 50


/// Espaço de nomes do projeto Babel.
namespace Babel
{

	/// Buscador de definições para uma dada palavra
	class ITranslator
	{
	public:
		/// Obtém a primeira definição da palavra procurada
		virtual bool __stdcall GetDefinition(const char* word) = 0;

		/// Obtém a próxima definição da palavra procurada
		virtual bool __stdcall GetNextDefinition() = 0;

		virtual const char* __stdcall Word() = 0;
		virtual const char* __stdcall Definition() = 0;
		virtual const char* __stdcall Source() = 0;
	};


	/// Aloca um novo definidor de palavras.
	extern "C" ITranslator* GetTranslator();

	/// Desaloca um definidor de palavras pré-alocado.
	extern "C" void FreeTranslator(ITranslator* translator);


	/// Importador de novas definições para palavras
	class IImporter
	{
	public:
		/// Importa uma nova definição de palavra.
		virtual bool __stdcall ImportDefinition(const char* word,
			const char* definition,
			const char* source) = 0;
	};


	/// Aloca um novo importador de palavras.
	extern "C" IImporter* GetImporter();

	/// Desaloca um importador de palavras pré-alocado.
	extern "C" void FreeImporter(IImporter* importer);


	void SqlEscapeString(char* dest, const char* src, size_t maxSize);


} // namespace Babel


#endif // __BABELBASELIB_H_8E80D150_4768_448E_ACFE_3BF63267C0A2__
