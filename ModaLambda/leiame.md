<a href="https://www.flickr.com/photos/caloni/13469488213/" title="moda-lambda por caloni, no Flickr"><img src="https://farm3.staticflickr.com/2932/13469488213_d22f6b1e92_o.jpg" alt="moda-lambda"></a>

A nova moda de programar C++ nos �ltimos anos com certeza � usar lambda. Mas, afinal, o que � lambda? Bom, pra come�ar, � um nome muito feio.

O que esse nome quer dizer basicamente � que agora � poss�vel criar fun��o dentro de fun��o. N�o s� isso, mas passar fun��es inteiras, com prot�tipo, corpo e retorno, como par�metro de fun��o.

Isso significa que finalmente os algoritmo da STL v�o ser �teis e n�o um "pain in the ass".

Por exemplo, antes, t�nhamos que fazer o seguinte malabarismo para mexer com arrays/vetores/listas:

[github,ModaLambda/ModaDeMode.cpp,cpp]

Imagine que para cada intera��o dev�amos criar uma fun��o que manipulasse os elementos do vetor.

Uma alternativa que costumava utilizar era a de roubar na brincadeira e criar um tipo dentro da fun��o (permitido) e dentro desse tipo criar uma fun��o (permitido):

[github,ModaLambda/ModaDeModeGambi.cpp,cpp]

Apesar disso gerar INTERNAL_COMPILER_ERROR em muitos builds com o Visual Studio 2003 (e o r�pido, mas anos noventa, Visual Studio 6) na maioria das vezes o c�digo compilava e rodava sem problemas. No entanto, deixava um rastro sutil de gambi no ar...

Agora isso n�o � mais necess�rio. Desde o Visual Studio 2010 (que eu uso) a Microsoft tem trabalhado essas novidades do padr�o no compilador, e aos poucos podemos nos sentir mais confort�veis em usar essas modernices sem medo. Por exemplo:

[github,ModaLambda/ModaLambda.cpp,cpp]

"Caraca, mas o que � esse c�digo alien�gena?", diria algu�m como eu alguns anos atr�s (talvez at� meses). Bom, nada vem de gra�a em C++ e dessa vez houve algumas mudan�as meio dr�sticas na sintaxe para acomodar o uso dessa lambida inline.

[github,ModaLambda/ModaLambdaEsqueleto.cpp,cpp]

E n�o � s� isso. Tem muito mais esquisitices de onde veio essa.
