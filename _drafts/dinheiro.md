---
title: "Dinheiro"
tags: [ "leituras", "finanças" ]
date: 2016-12-20

---
* Do not remove this line (it will not be displayed)
{:toc}

# Projeto Money

O projeto começou como um script em Python para baixar os ZIPs da Bovespa e manipular os dados disponíveis, balanços e demonstrativos em geral. Com isso seria feita a mineração de dados com o objetivo final de tornar simples o batimento e consolidação de informações.

## Cotações

Agora que estou revendo esse projeto parece que a API do Yahoo vai ser muito mais simples de usar e muito melhor de escalar. Existe um [projeto em Python](https://github.com/cgoldberg/ystockquote) muito pequeno e muito simples de usar.

Além do mais, minha estratégia de especulação estava muito ligada ao trabalho de análise para encontrar ativos, o que deve continuar apenas após um algoritmo mais esperto de triggers ser ativado. Esse algoritmo pode estar aliado ao que eu geralmente acompanho: a economia.

## Indicadores Econômicos

Partindo disso, assim como a degustação de vinhos, do geral para o particular, através da economia, analisando sob a ótima da Escola Austríaca, seria possível estar mais antenado aos grandes movimentos globais e regionais, aprimorando a aposta não só em ações e títulos, como moedas (bitcoins) e commodities (ouro).

Para começar de novo seria adequado analisar o movimento da economia brasileira. O índice que tenho de mais completo é o [Indeco](http://www.bcb.gov.br/pec/Indeco/Port/indeco.asp), que possui, agora que eu vi, links estáticos para os diferentes indicadores. Através desses links e com uma lógica de parseamento de planilhas já seria possível ler os dados remotos e atualizar um cache local. Com isso já poderemos gerar alguns gráficos e algumas métricas.

Dos projetos Python que leem Excel, o que me pareceu mais simples foi o [openpyxl](https://openpyxl.readthedocs.io/en/default/tutorial.html#loading-from-a-file). Um exemplo simples de uso:

```py
from openpyxl import load_workbook
wb2 = load_workbook(r'C:\Temp\money\IE1-01.xlsx')
wb = wb2['IE1-01']
print wb['A11'].value
```

#### Mapa de indicadores

Como há uma multidão de dados apenas no Indeco e como eles não estão formatados de uma maneira simples, e é possível ler dados de outras fontes, o ideal é que seja feito um mapa ou uma série de mapas para a criação de gráficos. Talvez a migração desses dados para o Google Sheets. Talvez um banco de dados sqlite.

## Mineração

Há mais de 700 moedas digitais hoje em dia. O [MinerGate](https://minergate.com/altminers/ccminer) tem um jeito fácil de minerar algumas delas.

## Futuro

Depois disso, seria ativar os indicadores dos ativos, fundamentado em para onde a economia deve ir. Muitas vezes não é necessário fazer nada (ex: continuar comprando bitcoins ou títulos). Em outros momentos é importante dar uma olhada em áreas específicas (ex: apostar, como em 2009, em um setor manipulado pelo governo, como construção civil). No entanto, tudo deve partir de geral para o específico, agora que sabemos uma maneira melhor de analisar a economia.

# Axiomas de Zurique (Max Gunther)

> Na vida, para qualquer espécie de ganho - em dinheiro, em estatura pessoal, o que quer que se defina como 'ganho' -, você tem de arriscar um pouco do seu capital material e/ou emocional. Tem de comprometer dinheiro, tempo, amor, alguma coisa. Esta é a lei do universo. 

> O 1º GRANDE AXIOMA: DO RISCO Preocupação não é doença, mas sinal de saúde. Se você não está preocupado, não está arriscando o bastante. 

> 1º AXIOMA MENOR Só aposte o que valer a pena. 

> 2º AXIOMA MENOR Resista à tentação das diversificações. 

> O 2º Grande Axioma: DA GANÂNCIA Realize o lucro sempre cedo demais. 

> 3º AXIOMA MENOR Entre no negócio sabendo quanto quer ganhar; quando chegar lá, caia fora. 

> O 3º Grande Axioma: DA ESPERANÇA Quando o barco começar a afundar, nãoreze. Abandone-o. 

> 4º AXIOMA MENOR Aceite as pequenas perdas com um sorriso, como fatos da vida. Conte incorrer em várias, enquanto espera um grande ganho. 

> O 4º Grande Axioma; DAS PREVISÕES O comportamento do ser humano não é previsível. Desconfio de quem afirmar que conhece uma nesga que seja do futuro. 

> O 5º Grande Axioma: DOS PADRÕES Até começar a parecer ordem, o caos não é perigoso. 

> 5º AXIOMA MENOR Cuidado com a armadilha do Historiador. 

> 6º AXIOMA MENOR Cuidado com a ilusão do Grafista. 

> 7º AXIOMA MENOR Cuidado com a ilusão de Correlação e a ilusão de Causalidade. 

> 8º AXIOMA MENOR Cuidado com a Falácia do Jogador. 

> ''Tudo bem, fiz o meu dever de casa o melhor possível. Acredito que estou apostando numa boa, e que vou ganhar. Porém, como não consigo controlar todas as casualidades capazes de afetar o meu dinheiro, sei que são grandes as minhas possibilidades de estar errado. Portanto, permanecerei atento, pronto a saltar na direção que os fatos indicarem.'' 

> O 6º Grande Axioma: DA MOBILIDADE 

> O 6º Grande Axioma: DA MOBILIDADE Evite lançar raízes. Tolhem seus movimentos. 

> 9º AXIOMA MENOR Numa operação que não deu certo, não se deixe apanhar por sentimentos como lealdade ou saudade. 

> 10º AXIOMA MENOR Jamais hesite em sair de um negócio se algo mais atraente aparecer à sua frente. 

> O 7º Grande Axioma: DA INTUIÇÃO Só se pode confiar num palpite que possa ser explicado. 

> 11º AXIOMA MENOR Jamais confunda palpite com esperança. 

> O 8º Grande Axioma DA RELIGIÃO E DO OCULTISMO É improvável que entre os desígnios de Deus para o Universo se inclua o de fazer você ficar rico. 

> 12º AXIOMA MENOR Se astrologia funcionasse, todos os astrólogos seriam ricos. 

> 13º AXIOMA MENOR Não é necessário exorcizar uma superstição. Podemos curti-la, desde que ela conheça o seu lugar. 

> O 9º Grande Axioma DO OTIMISMO E DO PESSIMISMO Otimismo significa esperar o melhor, mas confiança significa saber como se lidará com o pior. Jamais faça uma jogada por otimismo apenas. 

> O 10º Grande Axioma: DO CONSENSO Fuja da opinião da maioria. Provavelmente está errada. 

> 14º AXIOMA MENOR Jamais embarque nas especulações da moda. Com freqüência, a melhor hora de se comprar alguma coisa é quando ninguém a quer. 

> O 11º Grande Axioma: DA TEIMOSIA Se não deu certo da primeira vez, esqueça. 

> 15º AXIOMA MENOR Jamais tente salvar um mau investimento fazendo ''preço médio''. 

> O 12º Grande Axioma: DO PLANEJAMENTO Planejamentos a longo prazo geram a perigosa crença de que o futuro está sob controle. É importante jamais levar muito a sério os seus planos a longo prazo, nem os de quem quer que seja. 

> 16º AXIOMA MENOR Fuja de investimentos a longo prazo. 

