# Se iterando com os pseudo-ponteiros: uma breve introdu��o

Como [j� vimos algumas vezes](http://www.caloni.com.br/blog/archives/remove_if-ate-remove-so-que-diferente), a STL n�o prima por interfaces intuitivas, preferindo abstra��es que criem um leque de ferramentas gen�ricas para tratamento uniformizado de cole��es de dados atrav�s de algoritmos. O problema disso � que novos programadores da linguagem ter�o que aprender uma maneira nova de lidar com problemas baseada na percep��o dos criadores do padr�o na �poca em que foi lan�ado de como seria a evolu��o da ci�ncia da computa��o nos pr�ximos anos. Muitos dos conceitos ali abordados realmente se tornaram padr�o _de facto_, mas na briga pela expans�o da linguagem quem perdeu por muito tempo foi o pr�prio desenvolvedor, que teve que se contentar com uma lista de algoritmos gen�ricos parcialmente compilado.

Dito isto, a abstra��o dos iteradores � a coisa mais linda do mundo.

<a href="https://www.flickr.com/photos/120157483@N04/14307157649/" title="iteradores-como-ponteiros-a-imagem por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5500/14307157649_516dd123ba_z.jpg" alt="iteradores-como-ponteiros-a-imagem"></a>

## Iteradeiros ou Ponteradores?

Os dois �nicos conceitos que � preciso se lembrar para sempre quando se trata de iteradores � que ele:

 - Um iterador se comporta como um ponteiro opaco.
 - O final de um cont�iner est� sempre um elemento al�m do �ltimo.

Um ponteiro pode ser iterador, mas n�o o contr�rio!

Tudo que um ponteiro faz de �til em C/C++ foi emprestado para a STL usar em seus cont�iners, e tudo em que ele � prejudicial tentou ficar de fora. E o que um ponteiro faz de �til?

 - Um ponteiro pode apontar para elementos sem conhecermos sua posi��o.
 - Podemos incrementar ou decrementar ponteiros para caminhar em listas.
 - Dois ponteiros podem ser subtra�dos para sabermos a dist�ncia entre dois elementos.

[github,Code/IteradoresNaSTL/iteradores-como-ponteiros.cpp,cpp]

<a href="https://www.flickr.com/photos/120157483@N04/14492489942/" title="iteradores-como-ponteiros por wanderleycaloni, no Flickr"><img src="https://farm4.staticflickr.com/3853/14492489942_716878c004_z.jpg" alt="iteradores-como-ponteiros"></a>

Da mesma forma, opera��es como c�pia, movimenta��o, ordena��o, caotiza��o, pode ser feito usando dois ponteiros/iteradores de dois cont�iners distintos, desde que algumas regras b�sicas sejam seguidas, como _um iterador deve sempre apontar para algo v�lido_ (ou disponibilizar alguma abstra��o que insira novos elementos em um cont�iner menor). Veremos essas regras em um pr�ximo post sobre o tema. Apontarei para ele _aqui_.
