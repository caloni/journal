O segundo round da segunda fase do Code Jam passou nesse s�bado. Disl�xico que sou, consegui fazer apenas 8 pontos � como todo mundo � no teste small do problema B, que envolvia apenas dois loops aninhados (a vers�o large fica para outro post). Na verdade, estou aqui para expressar minha gratid�o ao campeonato por ter aprendido mais uma bela li��o vendo o c�digo do primeiro colocado do primeiro round, vulgo [Kaizero](https://code.google.com/codejam/contest/2984486/scoreboard?c=2984486#vf=1), um coreano que deu uma solu��o simples, r�pida e pr�tica para um problema de probabilidade t�o error-prone que at� os ju�zes do Google deram uma lambuja de alguns testes errados (sem contar que houve apenas a categoria small), e me fez pensar em quantas vezes pensamos em demasiado tentando encontrar a solu��o perfeita para algo que simplesmente... n�o precisa.

Basta um hack e [commit](http://pcottle.github.io/learnGitBranching/?NODEMO&defaultTab=remote&command=levels).

# � a incerteza, idiota!

<a href="https://www.flickr.com/photos/120157483@N04/14095237046/" title="Poker Jam"><img src="https://farm8.staticflickr.com/7344/14095237046_60ec978760_z.jpg" alt="Poker Jam"></a>

O problema reza que existem dois algoritmos para embaralhar uma sequ�ncia num�rica (de 0 a N): o bom e o ruim. Ambos tra�am um loop do in�co ao fim pegando aleatoriamente um elemento da lista e trocando de lugar com o elemento que est� sendo varrido no momento.

<a href="https://www.flickr.com/photos/120157483@N04/14118925924/" title="ProperShuffle por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2897/14118925924_300b85ff4c_n.jpg" alt="ProperShuffle"></a>

A diferen�a entre o bom e o ruim � que o bom pega aleatoriamente apenas os elementos DEPOIS do elemento que est� sendo varrido, enquanto o algoritmo ruim pega qualquer um dos elementos SEMPRE. Isso aparentemente e intuitivamente n�o parece interferir na aleatoriedade do embaralhamento, mas se levarmos ao extremo de embaralhar repetidas vezes somando a lista resultante percebemos uma tend�ncia gritante do algoritmo ruim em manter o ordenamento inicial, ou pelo menos na m�dia sempre tender para n�meros menores no in�cio e n�meros maiores no fim, como pode ser visto nesse teste que fiz, gerado pelo Excel:

<a href="https://www.flickr.com/photos/120157483@N04/14142661623/" title="Gr�fico dos Algoritmos de Embaralhamento por wanderleycaloni, no Flickr"><img src="https://farm8.staticflickr.com/7313/14142661623_f58729a795_n.jpg" alt="Gr�fico dos Algoritmos de Embaralhamento"></a>

O que eu tentei fazer durante meu fim-de-semana retrasado e o feriado foi encontrar um detector de aleatoriedade (ali�s, encontrei um bem interessante chamado [ent](http://www.fourmilab.ch/random/)), tanto "na m�o" quanto pesquisando. O que eu n�o imaginava foi que o teste que eu tinha feito no in�cio usando uma simples planilha Excel era a solu��o �bvia (naquelas de � �bvio s� depois que voc� v�). E foi essa a solu��o adotada por Kaizero.


