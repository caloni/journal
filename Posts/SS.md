# SS

Uma das coisas mais cretinas e dif�ceis para os iniciantes em C++ � conseguir formatar strings de maneira f�cil, r�pida e indolor. Infelizmente, a biblioteca de printf da linguagem C est� fechada para reforma, pois ela � extremamente error-prone e n�o-intuitiva. Por�m, se a printf � n�o-intuitiva, o que dizer << daqueles << sinais << de << flechinhas apontando para cout? Bem melhor, n�o?

[github,Code/SS/shift.cpp,cpp]

<a href="https://www.flickr.com/photos/120157483@N04/14330337121/" title="ShiftEstranho por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2918/14330337121_409bbee4f7_o.png" alt="ShiftEstranho"></a>

A resposta �, pra variar, depende. Se voc� combinar com seu c�rebro que o operador de shift que voc� aprendeu em C para cout n�o tem a mesma sem�ntica, OK. No fundo eu acredito que os criadores dessa sobrecarga de operador pensaram sinceramente que hoje em dia quase ningu�m conhece os operadores de shift bin�rio, ent�o tudo bem reaproveit�-lo de uma maneira mais miguxa.

Por�m, isso depende da maneira com que voc� usa streams C++. Vai haver momentos de sua vida que voc� vai se questionar por que tiraram todo o controle, a eleg�ncia e simplicidade de um bom printf, quando os [homens eram homens](http://www.caloni.com.br/blog/archives/programadores-de-verdade-nao-usam-java) e sabiam configurar jumpers para instalar a nova placa EISA.

[github,Code/SS/hexaformat.cpp,cpp]

<a href="https://www.flickr.com/photos/120157483@N04/14353868563/" title="Formata��o Dif�cil do Cout por wanderleycaloni, no Flickr"><img src="https://farm4.staticflickr.com/3863/14353868563_a2588b266c_o.png" alt="Formata��o Dif�cil do Cout"></a>

## A coisa mais f�cil do jeito mais dif�cil

A quest�o dos streams fica mais complicada quando precisamos realizar atividades corriqueiras no c�digo, como retornar uma string formatada, ou at� mesmo transformar um inteiro em string.

[github,Code/SS/conversao-do-dia-a-dia.cpp,cpp]

J� pensou termos que criar uma fun��o dessas sempre que quisermos converter n�meros em string? Ou pior, ter que fazer o que fizemos dentro dessa fun��o: declarar um ostringstream (um cout com buffer interno de string), us�-lo como cout e obter seu buffer interno atrav�s do m�todo str. Tudo isso para converter um n�mero para string.

Quando uma tarefa muito comum exige mais de dois passos para ser realizada � de bom tom criarmos algum c�digo reutiliz�vel, certo? Um c�digo que trar� de uma vez por todas a solu��o final!

## SS

[github,Code/SS/ss.h,cpp]

O c�digo acima serve bem ao nosso prop�sito de formatar strings em uma linha como um cout, mas retornar uma string no lugar. Ele � simples, ele � direto, ele tem defeitos que n�o vem ao caso (como n�o suportar endl), mas pode ser usado de maneira... simples e direta!

[github,Code/SS/main.cpp,cpp]

OK, o c�digo de exemplo foi idiota, mas voc� pegou a ideia. Tudo que precisamos fazer para reutilizar essa pequena classe � defin�-la (ss() resolve) e us�-la. Seu conversor de string retorna o buffer interno de ostringstream para n�s como num passe de m�gica.

_Obs.: Com certeza deve existir uma centena de bibliotecas que implementam algo do g�nero, s� que melhor. Essa � a t�pica fica isolante para continuar trabalhando._
