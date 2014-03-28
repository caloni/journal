<a href="https://www.flickr.com/photos/caloni/13469488213/" title="moda-lambda por caloni, no Flickr"><img src="https://farm3.staticflickr.com/2932/13469488213_d22f6b1e92_o.jpg" alt="moda-lambda"></a>

A nova moda de programar C++ nos últimos anos com certeza é usar lambda. Mas, afinal, o que é lambda? Bom, pra começar, é um nome muito feio.

O que esse nome quer dizer basicamente é que agora é possível criar função dentro de função. Não só isso, mas passar funções inteiras, com protótipo, corpo e retorno, como parâmetro de função.

Isso significa que finalmente os algoritmo da STL vão ser úteis e não um "pain in the ass".

Por exemplo, antes, tínhamos que fazer o seguinte malabarismo para mexer com arrays/vetores/listas:

[github,ModaLambda/ModaDeMode.cpp,cpp]

Imagine que para cada interação devíamos criar uma função que manipulasse os elementos do vetor.

Uma alternativa que costumava utilizar era a de roubar na brincadeira e criar um tipo dentro da função (permitido) e dentro desse tipo criar uma função (permitido):

[github,ModaLambda/ModaDeModeGambi.cpp,cpp]

Apesar disso gerar INTERNAL_COMPILER_ERROR em muitos builds com o Visual Studio 2003 (e o rápido, mas anos noventa, Visual Studio 6) na maioria das vezes o código compilava e rodava sem problemas. No entanto, deixava um rastro sutil de gambi no ar...

Agora isso não é mais necessário. Desde o Visual Studio 2010 (que eu uso) a Microsoft tem trabalhado essas novidades do padrão no compilador, e aos poucos podemos nos sentir mais confortáveis em usar essas modernices sem medo. Por exemplo:

[github,ModaLambda/ModaLambda.cpp,cpp]

"Caraca, mas o que é esse código alienígena?", diria alguém como eu alguns anos atrás (talvez até meses). Bom, nada vem de graça em C++ e dessa vez houve algumas mudanças meio drásticas na sintaxe para acomodar o uso dessa lambida inline.

[github,ModaLambda/ModaLambdaEsqueleto.cpp,cpp]

E não é só isso. Tem muito mais esquisitices de onde veio essa.
