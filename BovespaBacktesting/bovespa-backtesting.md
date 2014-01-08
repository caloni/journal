Eu n�o sou apenas um programador: sou um especulador. Ou, para quem ficou com medo, um investidor. Ficou bonito, agora? Trocando em mi�dos, isso quer dizer que muitas vezes aposto na bolsa de valores, aquela onde as pessoas ganham e perdem dinheiro loucamente. Por�m, assim como fa�o com minha carreira de desenvolvedor, n�o deixo de estudar e aprimorar minhas habilidades. Tirando alguns anos de estudo com livros de finan�as, economia e contabilidade, foi com base nisso que eu fiz uma s�rie de scripts que realiza opera��es de backtesting nos pap�is da Bovespa.

Que man� backtesting?

Backtesting � uma maneira dos especuladores terem uma no��o de qu�o bom ou ruim � sua estrat�gia de compra e venda. � uma maneira profissional de se aproximar do mercado ca�tico das a��es. Basicamente um backtesting simula o que o especulador faria na vida real com um hist�rico razo�vel de varia��o de pre�os das a��es que pretende operar. Se esse monte de palavras novas neste blogue est� te deixando com medo, recomendo dar uma passada no [Senhor Mercado](http://www.senhormercado.com.br/) (l� voc� ir� tamb�m aprender mais sobre t�cnicas de backtesting).

Vamos supor que minha ideia de estrat�gia seja comprar quando o pre�o de uma determinada a��o estiver na metade do seu topo hist�rico e vender quando ele estiver no dobro do momento da compra. Uma estrat�gia bem tosca, mas se fizer dinheiro, quem liga para vaidade? Outra estrat�gia mais refinada usa m�dias m�veis para estabelecer pontos de compra e venda dependendo da tend�ncia do mercado. Qual das duas d� mais dinheiro? Existem duas maneiras de saber: a dolorosa e a indolor. A dolorosa seria sacar uma grana do banco e come�ar a operar em sua corretora favorita seguindo ambas as estrat�gias e ver qual te deixou mais rico e qual te levou � fal�ncia. A indolor seria baixar o hist�rico de pre�os dos pap�is que est� interessado em usar essas estrat�gias e rodar uma simula��o que opere seguindo ambas as estrat�gias e descubra qual � a perdedora. Qual voc� preferiria?

OK, esse assunto j� est� ficando bem mon�tono para quem acompanha um blogue de programa��o. Vamos ao c�digo!

GitHub na veia

O [projeto que mantenho no GitHub](https://github.com/Caloni/BovespaBacktesting) possui algumas ideias que gostaria de compartilhar com todos que est�o interessados em realizar um backtesting, independente de sua estrat�gia. A primeira delas seria de onde baixar o hist�rico de pre�os de maneira simples e barata. Eu recomendo e uso o software [Grafix](http://www.grafix2.com/), que consegue baixar as informa��es diretamente do saite da Bovespa e realizar os ajustes necess�rios para montar e exibir as informa��es. Com base no banco de dados do Grafix � que o BovespaBacktesting (meu projeto) importa as informa��es que ele precisa. Ele ir� importar apenas os c�digos que estiverem em uma lista dispon�vel no arquivo data/filterCodes relativo de onde o script estiver rodando. Esse arquivo � apenas texto com um c�digo por linha.

[github,BovespaBacktesting/import-quote.py]

A partir dessa importa��o � poss�vel realizar queries com as varia��es di�rias, semanais e mensais dos pre�os dos ativos conhecidos (a mesma lista de c�digo). A pr�pria lista de ativos conhecidos est� dispon�vel atrav�s de uma fun��o, tornando a itera��o simples e direta.

[github,BovespaBacktesting/load-quote.py]

Com essas informa��es de pre�o � poss�vel aplicar qualquer tipo de indicador. O BovespaBackteting possui apenas os mais usuais, mas basta implementar a l�gica de tratamento em Python, o que n�o deve consumir nem muito tempo nem muitos neur�nios, pois com o hist�rico dispon�vel tudo fica mais f�cil.

[github,BovespaBacktesting/indicators.py]

As fun��es-macro calculam trades (opera��es) a partir de alguns par�metros definidos no c�digo ou por par�metros. As vers�es do BovespaBacktesting foram variando nesse sentido. Ainda n�o h� uma maneira saud�vel de comparar diversas estrat�gias, pois o que eu tenho feito basicamente � alterar alguns par�metros, rodar o backtesting e exportar para um CSV (fun��o j� dispon�vel).

[github,BovespaBacktesting/calc.py]

J� existem algumas firulas caso voc� esteja pensando em uma estrat�gia em que seja vi�vel viver de operar, como c�lculo de sal�rio e a inclus�o de vari�veis que levem em conta que parte do dinheiro ganho ser� usado. Ainda � um c�digo bem tosco, mas funciona e pode ser o ponto de entrada de quem deseja conhecer mais sobre o mercado de a��es e como os profissionais conseguem tirar dinheiro deste grande cassino.
