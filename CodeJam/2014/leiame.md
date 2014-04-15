<a href="https://www.flickr.com/photos/120157483@N04/13878076723/" title="jam2014 por wanderleycaloni, no Flickr"><img src="https://farm4.staticflickr.com/3776/13878076723_dc8556a364_o.jpg" alt="jam2014"></a>

N�o costumo participar de campeonatos de programa��o por alguns motivos vagos: � perda de tempo (n�o ganho nada com isso), sou um p�ssimo programador (ou pasteleiro), d� pregui�a (esse � o mais v�lido) e por a� vai o mimimi. Dessa forma, sempre passei ileso de eventos como o atual Google Code Jam, que pretende levar a categoria de c�digo ofuscado para um novo patamar.

No entanto, esse ano apareceram dois motivos que me levaram a gastar cinco minutos de paci�ncia com as historinhas bestas da equipe do Google. Primeiro o Python, que desde 2013 tem renovado em mim a sensa��o que programar ainda � divertido (e que o pessoal da Microsoft e do padr�o C++ tinham tirado de mim h� muito tempo com seus compiladores cada vez mais complexos/lentos e as IDEs que demoram o tempo do cafezinho para abrir). Segundo o que move o mundo: a concorr�ncia. Minha dign�ssima esposa, levada por alguns pontos-extra na faculdade (uma iniciativa at� que louv�vel do professor), resolveu participar da primeira fase (a classifica��o desta fase tamb�m dava pontos).

O fato � que depois desses cinco minutos eu simplesmente n�o consegui parar at� o minuto final das 23 horas (hor�rio de Bras�lia) de domingo, quando o tempo-limite esgotou. O aspecto mais divertido do Code Jam � que h� liberdade total para a ferramenta que voc� pretende usar: linguagens de programa��o, Excel, uma calculadora ou apenas seu c�rebro. Voc� recebe uma "miss�o" e um arquivo de entrada e precisa cuspir um arquivo de sa�da de acordo com a miss�o. Apenas isso. O resto fica por conta da criatividade dos codadores e gambiarreiros de plant�o.

Todos os exerc�cios levam em considera��o um arquivo de entrada que possui em sua primeira linha o n�mero de testes que ser�o feitos e em seguida um n�mero determinado de linhas e par�metros, geralmente divididos por espa�o. O primeiro problema, por exemplo, apenas considerava a suposi��o de cartas em pequeno truque de m�gica e recebia como entrada a disposi��o dessas cartas junto com a escolha da fileira que o participante dizia onde estava a carta escolhida.

2
1 2 3 4
5 6 7 8
9 10 11 12
13 14 15 16
3
1 2 5 4
3 11 6 15
9 10 7 12
13 14 8 16

[github,CodeJam/2014/magictrick.py,python]

O segundo exerc�cio j� envolvia um jogo bem divertido em que o jogador ficava clicando em cookies como se n�o houvese amanh�. Esse deu um pouco mais de trabalho, mas foi mais divertido que o primeiro.

[github,CodeJam/2014/cookieclicker.py,python]

J� o terceiro... o terceiro passa. Vamos para o quarto, um dos mais instigantes, pois envolve duas regras distintas de um jogo e a otimiza��o das melhores estrat�gias para ambos. Isso consumiu bem mais tempo que os outros dois iniciais, pois lembro de ter me isolado por uma hora para conseguir colocar tudo na cabe�a.

[github,CodeJam/2014/war.py,python]

J� o terceiro foi um fracasso total. Tentei de todas as maneiras resolver o impasse de descobrir qual disposi��o de um jogo de campo minado poderia ser resolvido em apenas um clique (parece o jogo oposto do viciado clicador de cookies), mas falhei miseravelmente. E desconfio o porqu�. Primeiro entendo que meu perfeccionismo me impediu de realizar uma checagem padr�o para exce��es j� conhecidas (quando h� apenas uma linha ou coluna, quando h� apenas um espa�o sem minas, etc). Eu pensei: se o Google fez esse problema, ele deve ter bolado alguma solu��o gen�rica que independa de ifs. Bom, n�o que eu saiba. Depois de terminado o tempo dei uma olhada em algumas solu��es dos competidores e n�o achei nenhuma solu��o que usasse algum algoritmo maluco e gen�rico (n�o achei nenhum indiano, contudo).

Eis a solu��o porca e mal-resolvida (alguns pontos do c�dido foram feitos depois de ver o c�digo de outrem):

[github,CodeJam/2014/mine.py,python]

N�o, eu n�o usei o Google para descobrir a l�gica por tr�s do problema. Vai que os caras ficam monitorando quem fica fazendo pesquisas. E, n�o, tampouco usei o Bing. N�o sou masoquista a esse ponto.

PS: Bom, estou na pr�xima fase. Veremos o que o futuro nos espera. Esse programador foi fisgado pelo campeonato de past�is.
