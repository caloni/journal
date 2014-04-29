<a href="https://www.flickr.com/photos/120157483@N04/14065766204/" title="cowblet-cover por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2921/14065766204_501914665d_o.jpg" alt="cowblet-cover"></a>

A primeira bateria de problemas da segunda fase do Code Jam me mostrou o porquê do seu nome: meu cérebro ficou feito geleia (ha ha ha). Não consegui resolver nenhum problema a tempo, mas não culpo o C++, que passei a usar para essa fase. É burrice aliada a pressão. Duas horas e meia para entender um problema é o tipo de coisa que me deixa pensando mais no tempo do que no problema. Tenho que melhorar isso.

De qualquer forma, esse final de semana que passou foi dedicado a resolver o primeiro problema e quem sabe escrever um post a respeito. Imagino que todos tenham acesso ao [enunciado](http://code.google.com/codejam/contest/2984486/dashboard#s=p0) e aos casos de teste, mas, por via das dúvidas, aqui vai o problema:

Você é um fazendeiro hi-tech com uma vaca que tem um tablet. Não especifica se esse cowblet é um iPad, mas é uma possibilidade, já que como nenhum plugue parece encaixar nas tomadas de sua fazenda, é muito provável que você seja um Applemaníaco com um monte de gadgets que precisam de conversor vindos direto do eBay.

<a href="https://www.flickr.com/photos/120157483@N04/14065766314/" title="cowblet por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2908/14065766314_c07ca93e24_o.jpg" alt="cowblet"></a>

Veio também do eBay um engenheiro chinês cuja missão é resolver esse gato que o Sr. Fazendeiro fez em sua fazenda. Tudo que ele precisa fazer é girar gigantescos switches (ou disjuntores) que invertem a polaridade binária de cada um dos pino dos conectores das tomadas. Quando um plugue de dispositivo e uma tomada possuem a mesma configuração de bits é possível conectá-los. O objetivo final é que todos os N plugues conectem nas N tomadas depois de virados Y switches, sendo que quanto menos switches melhor (eles são gigantescos, lembra?)

<a href="https://www.flickr.com/photos/120157483@N04/14065765834/" title="ProblemA por wanderleycaloni, no Flickr"><img src="https://farm8.staticflickr.com/7178/14065765834_6bf738e020_o.png" alt="ProblemA"></a>

O primeiro pensamento do programador preguiçoso manda que usemos a velha força bruta e testemos todas as combinações possíveis de disjuntores, peguemos o com menor número de bits setados (inicialmente, todos estão em 0) e zás! Porém, o caso de teste large pressupõe que o limite de pinos das tomadas pode chegar a 40, o que seria responsável por nada mais nada menos que 2^40 possibilidades, ou 1.099.511.627.776 para ser exato (isso dá mais de 1 trilhão, BTW). Mesmo que nosso código seja extremamente rápido e demora apenas um milissegundo para cada combinação, serão mais de 34 anos desperdiçados, que poderiam estar melhor investidos minerando bitcoins.

<a href="https://www.flickr.com/photos/120157483@N04/14062137592/" title="ProblemA-SwitchGigante por wanderleycaloni, no Flickr"><img src="https://farm8.staticflickr.com/7374/14062137592_6fd24e607c_o.png" alt="ProblemA-SwitchGigante"></a>

Dessa forma, temos que traçar uma solução baseada nas combinações entre as tomadas e plugues, que, pelos limites da versão large dos casos de teste, 
