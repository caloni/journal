---
author: admin
comments: true
date: 2014-04-29 16:53:52+00:00
layout: post
slug: que-geleia-de-mocoto
title: Que geleia de mocotÃ³
wordpress_id: 1745
categories:
- CÃ³digo
- Devaneando
tags:
- campeonatos
- code jam
- eventos
---

[![cowblet-cover](https://farm3.staticflickr.com/2921/14065766204_532438eaed_n.jpg)](https://www.flickr.com/photos/120157483@N04/14065766204/)





A primeira bateria de problemas da segunda fase do Code Jam me mostrou o porquÃª do seu nome: meu cÃ©rebro ficou feito geleia (ha ha ha). NÃ£o consegui resolver nenhum problema a tempo, mas nÃ£o culpo o C++, que passei a usar para essa fase. Ã burrice aliada a pressÃ£o. Duas horas e meia para entender um problema Ã© o tipo de coisa que me deixa pensando mais no tempo do que no problema. Tenho que melhorar isso.





De qualquer forma, esse final de semana que passou foi dedicado a resolver o primeiro problema e quem sabe escrever um post a respeito. Imagino que todos tenham acesso ao [enunciado](http://code.google.com/codejam/contest/2984486/dashboard#s=p0) e aos casos de teste, mas, por via das dÃºvidas, aqui vai uma descriÃ§Ã£o adaptada:





VocÃª Ã© um fazendeiro hi-tech com uma vaca que tem um tablet. NÃ£o especifica se esse cowblet Ã© um iPad, mas Ã© uma possibilidade, jÃ¡ que como nenhum plugue parece encaixar nas tomadas de sua fazenda, Ã© muito provÃ¡vel que vocÃª seja um ApplemanÃ­aco com um monte de gadgets que precisam de conversor vindos direto do eBay.





[![cowblet](https://farm3.staticflickr.com/2908/14065766314_c07ca93e24_o.jpg)](https://www.flickr.com/photos/120157483@N04/14065766314/)





AtravÃ©s do eBay tambÃ©m veio um engenheiro chinÃªs cuja missÃ£o Ã© resolver esse gato que o Sr. Fazendeiro fez em sua fazenda. Tudo que ele precisa fazer Ã© girar gigantescos switches (ou disjuntores) que invertem a polaridade binÃ¡ria de cada um dos pino dos conectores das tomadas. Quando um plugue de dispositivo e uma tomada possuem a mesma configuraÃ§Ã£o de bits Ã© possÃ­vel conectÃ¡-los. O objetivo final Ã© que todos os N plugues conectem nas N tomadas depois de virados Y switches, sendo que quanto menos switches melhor (afinal, eles sÃ£o gigantescos, e o chinÃªs supÃµe-se que seja pequeno).





[![ProblemA](https://farm8.staticflickr.com/7178/14065765834_6bf738e020_o.png)](https://www.flickr.com/photos/120157483@N04/14065765834/)





O primeiro pensamento do programador preguiÃ§oso (go, horse, go!) manda que usemos a velha forÃ§a bruta e testemos todas as combinaÃ§Ãµes possÃ­veis de disjuntores, peguemos o com menor nÃºmero de bits setados (inicialmente, todos estÃ£o em 0) e zÃ¡s! PorÃ©m, o caso de teste tamanho _large_ pressupÃµe que o limite de pinos das tomadas pode chegar a 40, o que seria responsÃ¡vel por nada mais nada menos que 2^40 combinaÃ§Ãµes diferentes, ou 1.099.511.627.776 para ser exato. Isso dÃ¡ mais de 1 trilhÃ£o! Mesmo que nosso cÃ³digo seja extremamente rÃ¡pido e demore apenas um milissegundo para cada combinaÃ§Ã£o, serÃ£o mais de 34 anos desperdiÃ§ados, que poderiam estar melhor investidos minerando bitcoins.





[![Tomadao](https://farm3.staticflickr.com/2928/14042619426_9b12f4468d_o.png)](https://www.flickr.com/photos/120157483@N04/14042619426/)





Dessa forma, temos que traÃ§ar uma soluÃ§Ã£o baseada nas combinaÃ§Ãµes entre as tomadas e plugues, que, pelos limites da versÃ£o large dos casos de teste, podem ter a quantidade de 150, o que dÃ¡ 150*150 = 22500 combinaÃ§Ãµes de XOR.





[![WP_000773](https://farm6.staticflickr.com/5127/14065469895_97cb03a986_n.jpg)](https://www.flickr.com/photos/120157483@N04/14065469895/)





Sim, de XOR. O XOR aqui pode ser usado para detectarmos qual a combinaÃ§Ã£o de switches precisamos para que cada tomada encaixa em cada dispositivo. Esse Ã© o nosso conjunto universo de giros de disjuntores. Com esse conjunto em mÃ£os fica fÃ¡cil saber quais combinaÃ§Ãµes sÃ£o possÃ­veis de encaixar todos os dispositivos: basta contar!





```cpp
#include "CodeJam.h"
#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <string>

using namespace std;

static const int MAX_SWITCHES = 40;


__int64 StringToInt(const string& s, int L)
{
	__int64 ret = 0;
	for( int l = L; l; --l )
	{
		char c = s[L-l];
		if( c == '1' )
			ret |= ((__int64)1 << (l-1));
	}
	return ret;
}

int CountBits(__int64 value)
{
	int ret = 0;
	while( value )
	{
		if( value & 1 )
			++ret;
		value >>= 1;
	}
	return ret;
}

void TestCase(std::istream& is, std::ostream& os)
{
	int N, L;
	is >> N >> L;

	vector<__int64> outlets;
	vector<__int64> devices;
	string eletricItem;

	for( int n = 0; n < N; ++n )
	{
		is >> eletricItem;
		outlets.push_back(StringToInt(eletricItem, L));
	}

	for( int n = 0; n < N; ++n )
	{
		is >> eletricItem;
		devices.push_back(StringToInt(eletricItem, L));
	}

	typedef map<__int64, set<int>> SwitchOcurrences;
	SwitchOcurrences switchOcurrences;
	
	for( int n = 0; n < N; ++n )
	{
		__int64 outlet = outlets[n];

		for( int n2 = 0; n2 < N; ++n2 )
		{
			__int64 device = devices[n2];
			__int64 connection = outlet ^ device;
			switchOcurrences[connection].insert(n);
		}
	}

	for(auto it = switchOcurrences.begin(); it != switchOcurrences.end(); )
	{
		if( it->second.size() != N )
			it = switchOcurrences.erase(it);
		else
			++it;
	}

	if( switchOcurrences.size() )
	{
		int switches = MAX_SWITCHES + 1;

		for_each(switchOcurrences.begin(), switchOcurrences.end(), [&](const SwitchOcurrences::value_type& ocurrence)
		{
			int bits = CountBits(ocurrence.first);
			switches = min(switches, bits);
		});

		os << switches << endl;
	}
	else os << "NOT POSSIBLE\n";
}

```






_ObservaÃ§Ã£o: note que retirei o wrapper costumeiro dos exercÃ­cios do Code Jam para nÃ£o poluir demais o exemplo com cÃ³digo. E, na verdade, essa parte do cÃ³digo estÃ¡ compartilhada com todas as soluÃ§Ãµes (reuse!)._





O que aprendi dessa pequena aventura foi: nÃ£o importa o quanto um problema pareÃ§a fÃ¡cil, anotar em um pedaÃ§o de papel Ã© o caminho mais curto entre a mente e o cÃ³digo.





Que venha a segunda bateria de problemas!



