Todo depurador de WinDbg (ou de Visual Studio no modo "roots") sabe que para analisar o conteúdo de uma basic_string "grande" (maior que 16 bytes) basta pegar o dword inicial do endereço do objeto e deferenciá-lo, pois se trata de um ponteiro para char*:

https://raw.github.com/Caloni/Caloni.com.br/master/BasicString/img/basic_string_release.gif

Isso pode ser comprovado através dos fontes do STL da Microsoft:



https://raw.github.com/Caloni/Caloni.com.br/master/BasicString/img/basic_string_debug.gif