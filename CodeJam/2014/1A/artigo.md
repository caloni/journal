<a href="https://www.flickr.com/photos/120157483@N04/14065766204/" title="cowblet-cover por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2921/14065766204_501914665d_o.jpg" alt="cowblet-cover"></a>

A primeira bateria de problemas da segunda fase do Code Jam me mostrou o porqu� do seu nome: meu c�rebro ficou feito geleia (ha ha ha). N�o consegui resolver nenhum problema a tempo, mas n�o culpo o C++, que passei a usar para essa fase. � burrice aliada a press�o. Duas horas e meia para entender um problema � o tipo de coisa que me deixa pensando mais no tempo do que no problema. Tenho que melhorar isso.

De qualquer forma, esse final de semana que passou foi dedicado a resolver o primeiro problema e quem sabe escrever um post a respeito. Imagino que todos tenham acesso ao [enunciado](http://code.google.com/codejam/contest/2984486/dashboard#s=p0) e aos casos de teste, mas, por via das d�vidas, aqui vai o problema:

Voc� � um fazendeiro hi-tech com uma vaca que tem um tablet. N�o especifica se esse cowblet � um iPad, mas � uma possibilidade, j� que como nenhum plugue parece encaixar nas tomadas de sua fazenda, � muito prov�vel que voc� seja um Appleman�aco com um monte de gadgets que precisam de conversor vindos direto do eBay.

<a href="https://www.flickr.com/photos/120157483@N04/14065766314/" title="cowblet por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2908/14065766314_c07ca93e24_o.jpg" alt="cowblet"></a>

Veio tamb�m do eBay um engenheiro chin�s cuja miss�o � resolver esse gato que o Sr. Fazendeiro fez em sua fazenda. Tudo que ele precisa fazer � girar gigantescos switches (ou disjuntores) que invertem a polaridade bin�ria de cada um dos pino dos conectores das tomadas. Quando um plugue de dispositivo e uma tomada possuem a mesma configura��o de bits � poss�vel conect�-los. O objetivo final � que todos os N plugues conectem nas N tomadas depois de virados Y switches, sendo que quanto menos switches melhor (eles s�o gigantescos, lembra?)

<a href="https://www.flickr.com/photos/120157483@N04/14065765834/" title="ProblemA por wanderleycaloni, no Flickr"><img src="https://farm8.staticflickr.com/7178/14065765834_6bf738e020_o.png" alt="ProblemA"></a>

O primeiro pensamento do programador pregui�oso manda que usemos a velha for�a bruta e testemos todas as combina��es poss�veis de disjuntores, peguemos o com menor n�mero de bits setados (inicialmente, todos est�o em 0) e z�s! Por�m, o caso de teste large pressup�e que o limite de pinos das tomadas pode chegar a 40, o que seria respons�vel por nada mais nada menos que 2^40 possibilidades, ou 1.099.511.627.776 para ser exato (isso d� mais de 1 trilh�o, BTW). Mesmo que nosso c�digo seja extremamente r�pido e demora apenas um milissegundo para cada combina��o, ser�o mais de 34 anos desperdi�ados, que poderiam estar melhor investidos minerando bitcoins.

<a href="https://www.flickr.com/photos/120157483@N04/14062137592/" title="ProblemA-SwitchGigante por wanderleycaloni, no Flickr"><img src="https://farm8.staticflickr.com/7374/14062137592_6fd24e607c_o.png" alt="ProblemA-SwitchGigante"></a>

Dessa forma, temos que tra�ar uma solu��o baseada nas combina��es entre as tomadas e plugues, que, pelos limites da vers�o large dos casos de teste, 
