<a href="https://www.flickr.com/photos/120157483@N04/14065766204/" title="cowblet-cover por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2921/14065766204_532438eaed_n.jpg" alt="cowblet-cover"></a>

A primeira bateria de problemas da segunda fase do Code Jam me mostrou o porqu� do seu nome: meu c�rebro ficou feito geleia (ha ha ha). N�o consegui resolver nenhum problema a tempo, mas n�o culpo o C++, que passei a usar para essa fase. � burrice aliada a press�o. Duas horas e meia para entender um problema � o tipo de coisa que me deixa pensando mais no tempo do que no problema. Tenho que melhorar isso.

De qualquer forma, esse final de semana que passou foi dedicado a resolver o primeiro problema e quem sabe escrever um post a respeito. Imagino que todos tenham acesso ao [enunciado](http://code.google.com/codejam/contest/2984486/dashboard#s=p0) e aos casos de teste, mas, por via das d�vidas, aqui vai uma descri��o adaptada:

Voc� � um fazendeiro hi-tech com uma vaca que tem um tablet. N�o especifica se esse cowblet � um iPad, mas � uma possibilidade, j� que como nenhum plugue parece encaixar nas tomadas de sua fazenda, � muito prov�vel que voc� seja um Appleman�aco com um monte de gadgets que precisam de conversor vindos direto do eBay.

<a href="https://www.flickr.com/photos/120157483@N04/14065766314/" title="cowblet por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2908/14065766314_c07ca93e24_o.jpg" alt="cowblet"></a>

Atrav�s do eBay tamb�m veio um engenheiro chin�s cuja miss�o � resolver esse gato que o Sr. Fazendeiro fez em sua fazenda. Tudo que ele precisa fazer � girar gigantescos switches (ou disjuntores) que invertem a polaridade bin�ria de cada um dos pino dos conectores das tomadas. Quando um plugue de dispositivo e uma tomada possuem a mesma configura��o de bits � poss�vel conect�-los. O objetivo final � que todos os N plugues conectem nas N tomadas depois de virados Y switches, sendo que quanto menos switches melhor (afinal, eles s�o gigantescos, e o chin�s sup�e-se que seja pequeno).

<a href="https://www.flickr.com/photos/120157483@N04/14065765834/" title="ProblemA por wanderleycaloni, no Flickr"><img src="https://farm8.staticflickr.com/7178/14065765834_6bf738e020_o.png" alt="ProblemA"></a>

O primeiro pensamento do programador pregui�oso (go, horse, go!) manda que usemos a velha for�a bruta e testemos todas as combina��es poss�veis de disjuntores, peguemos o com menor n�mero de bits setados (inicialmente, todos est�o em 0) e z�s! Por�m, o caso de teste tamanho _large_ pressup�e que o limite de pinos das tomadas pode chegar a 40, o que seria respons�vel por nada mais nada menos que 2^40 combina��es diferentes, ou 1.099.511.627.776 para ser exato. Isso d� mais de 1 trilh�o! Mesmo que nosso c�digo seja extremamente r�pido e demore apenas um milissegundo para cada combina��o, ser�o mais de 34 anos desperdi�ados, que poderiam estar melhor investidos minerando bitcoins.

<a href="https://www.flickr.com/photos/120157483@N04/14042619426/" title="Tomadao por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2928/14042619426_9b12f4468d_o.png" alt="Tomadao"></a>

Dessa forma, temos que tra�ar uma solu��o baseada nas combina��es entre as tomadas e plugues, que, pelos limites da vers�o large dos casos de teste, podem ter a quantidade de 150, o que d� 150*150 = 22500 combina��es de XOR.

<a href="https://www.flickr.com/photos/120157483@N04/14065469895/" title="WP_000773 por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5127/14065469895_97cb03a986_n.jpg" alt="WP_000773"></a>

Sim, de XOR. O XOR aqui pode ser usado para detectarmos qual a combina��o de switches precisamos para que cada tomada encaixa em cada dispositivo. Esse � o nosso conjunto universo de giros de disjuntores. Com esse conjunto em m�os fica f�cil saber quais combina��es s�o poss�veis de encaixar todos os dispositivos: basta contar!

[github,CodeJam/2014/1A/ChargingChaos.cpp,cpp]

_Observa��o: note que retirei o wrapper costumeiro dos exerc�cios do Code Jam para n�o poluir demais o exemplo com c�digo. E, na verdade, essa parte do c�digo est� compartilhada com todas as solu��es (reuse!)._

O que aprendi dessa pequena aventura foi: n�o importa o quanto um problema pare�a f�cil, anotar em um peda�o de papel � o caminho mais curto entre a mente e o c�digo.

Que venha a segunda bateria de problemas!
