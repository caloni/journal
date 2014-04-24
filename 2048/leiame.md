Pronto, posso programar em paz. O jogo 2048 � uma l�stima para todos os trabalhadores intelectuais que dependem de suas mentes para produzir algo que preste. Ele gerou mais posts no Hacker News do que a moda dos bitcoins (talvez n�o) e mais projetos no GitHub do que a busca para a cura do c�ncer (talvez n�o). (Obviamente que este post vai gerar [mais um commit Python no reposit�rio de artigos](https://github.com/Caloni/Caloni.com.br).)

N�o sou f� de jogos, e dos poucos que participei logo parei (exce��es honrosas: Portal e Portal 2, esses malditos). Posso dizer o mesmo de 2048, a vers�o de uma esp�cie de jogo j� conhecido feita pelo italiano [Gabriele Cirulli](http://gabrielecirulli.github.io/2048/) em um fds para ele descobrir se seria capaz de faz�-lo. Ele fez e o jogo e de quebra o �ndice de produtividade mundial cair.

Houve pelo menos dois projetos de I.A. para resolver o problema que consiste em dobrar n�meros m�ltiplos de 2 em um quadrado 4 x 4 at� que se consiga o quadrado com o valor 2048 (e al�m). O artigo de [Nicola Pezzotti](http://diaryofatinker.blogspot.it/2014/03/an-artificial-intelligence-for-2048-game.html) explica o mais efetivo deles, de autoria de [Robert Xiao](https://github.com/nneonneo/2048-ai) (eu acho). O programa desenvolvido por Xiao otimiza o tabuleiro do jogo guardando-o em um inteiro de 64 bits, deixando 4 bits para cada casa, mais que o suficiente para que seja armazenada a pot�ncia de 2 localizada no quadrado (o limite fica sendo de 2 ** 16, ou 65536). Ao rodar a vers�o execut�vel console ele imprime cada posi��o do tabuleiro em um formato f�cil de ser lido, como este:

Move #69, current score=584
1356
0051
0012
0000

Como pode-se perceber, cada n�mero diferente de zero cont�m a pot�ncia de dois que ocupa a casa (1 � igual a 2, 5 � igual a 2 ** 5 = 32 e assim por diante). Para alinhar corretamente o tabuleiro os n�meros est�o impressos em hexadecimal, ou seja, os valores v�lidos v�o de 0 a f (15).

Isso j� seria o suficiente para desvendar os movimentos da I.A., mas nada como um apelo visual. Que tal um v�deo?

http://youtu.be/GVTCej6zwAk

Feito com a ajuda de um tabuleiro-template e um pouco do poder Python+PIL:

[github,2048/ai2048draw.py,py]

Note que a estrat�gia basicamente � ordenar as casas em um lado e, assim que acumular valores o suficiente, consolidar tudo na �ltima casa. Nem sempre isso � poss�vel, pois uma virada de jogo pode deixar a casa com o maior valor no meio de um dos lados. Nesse caso, � interessante ver como a I.A. se sai, j� que com apenas uma execu��o ela foi at� 8192 e mais um 4096. D�-lhe, computador!

Observa��o importante: para que o conversor Python funcione ele exige um arquivo que tenha *apenas* as posi��es do tabuleiro dispostas como nas 4 linhas acima. Nada que um grep "^[0-9a-f][0-9a-f][0-9a-f][0-9a-f]$" na sa�da do jogo n�o resolva.

Dica: como criar seus pr�prios v�deos a partir de uma [porrada de PNGs](https://trac.ffmpeg.org/wiki/Create%20a%20video%20slideshow%20from%20images).

Ah, sim, se voc� pretende analisar a estrat�gia do jogo passo-a-passo, acho que � melhor voc� dar uma olhada no [log do jogo](https://github.com/Caloni/Caloni.com.br/tree/master/2048) (ou us�-lo para gerar seus PNGs).

Brinde: vers�o 2048 do [Vida de Programador](http://goistsg.github.io/2048-vdp/)!
