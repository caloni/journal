Todo depurador de WinDbg (ou de Visual Studio no modo "roots") sabe que para analisar o conte�do de uma basic_string "grande" (maior que 16 bytes) basta pegar o dword inicial do endere�o do objeto e deferenci�-lo, pois se trata de um ponteiro para char*:

https://raw.github.com/Caloni/Caloni.com.br/master/BasicString/img/basic_string_release.gif

Isso pode ser comprovado atrav�s dos fontes do STL da Microsoft:



https://raw.github.com/Caloni/Caloni.com.br/master/BasicString/img/basic_string_debug.gif