---
categories:
 - blogging
date: '2022-12-04T20:56:46-03:00'
tags:
title: Como Eu Reviso Meus Textos
---

Este é um post que deve ficar desatualizado em breve porque eu vivo refinando a forma de revisar meus textos, mas nunca registrei como faço isso. Essa é minha oportunidade. Bem, a última grande reforma nesse processo surgiu depois que dediquei os anos 2020 e 2021 para revisar desde o começo todos os meus três mil posts já escritos. A partir dessa revigorante experiência foi possível subir um pouquinho mais no [meu platô] de habilidades de escrita para algo que eu considero próximo do ideal. Se não é ideal, pelo menos é uma forma automática de trabalhar escrita e reescrita dos meus rascunhos.

A base para a mudança foi essa frase de Stephen King em seu livro [On Writing]:

> escreva com a porta fechada; reescreva com a porta aberta

Isso quer dizer que nós escritores podemos, a princípio, colocar tudo no papel. Papel aceita tudo mesmo. Porém, existe um filtro na hora de publicar a versão final para outros lerem. E este filtro é o que deve ser ativado quando nós fechamos a porta. A reescrita, talvez, seja a parte mais importante do ato de escrever, pois é nela que é possível transformar nossos caóticos pensamentos em algo que possa ser traduzido pelo leitor.

Mas devaneio. O que eu gostaria de falar neste post é mais sobre o operacional, do ponto de vista deste blog, que pertence a um programador, e portanto contém um roteiro para automatizar as partes chatas.

Vou começar descrevendo o que eu faço para publicar novos posts. Bem, eu utilizo o [publicador hugo](https://gohugo.io/), que transforma meus arquivos texto em páginas html estáticas, o que permite colocar em qualquer servidor de arquivos. A partir daí é apenas a boa e velha navegação Web 1.0 com alguns poucos scripts para auxiliar nas buscas.

Para começar um novo post no hugo existe um comando. Este comando é automatizado por mim em um script em que ele já usa um modelo de arquivo com a marcação de rascunho ligada, o que quer dizer que o post se publicado nesse momento não irá aparecer na lista de novidades, já que eu retiro os rascunhos de lá. No entanto, eu mantenho uma lista apenas para esses rascunhos, por comodidade e porque eu acredito que o processo de escrever texto é muito próximo de escrever código: não há versão final. O que chamamos de final é a melhor versão que conseguimos publicar neste momento, mas sempre é possível melhorar. É por isso que os rascunhos ficam publicados. Não que alguém mereça lê-los. Considere esses textos como minha branch de desenvolvimento, que no caso de um escritor solitário se confunde com a master. Se houvesse um grupo de escritores trabalhando para este blog provavelmente teríamos uma develop e uma master.

Pois bem. Rascunhos são importantes, mas precisam se manter à distância até que tenhamos uma versão minimamente publicável: um release. Antes que isso aconteça os posts vão sendo publicados em um cantinho à parte, e junto disso eu publico dois livros digitais para uso próprio que vão para meu leitor de ebooks. Um deles contém todos os posts e uso para referência offline. O outro, mais atualizado, é minha branch develop. Ele contém apenas os rascunhos. Que eu preciso ler para poder retrabalhar. E é por isso que ele existe. Eu lerei no momento oportuno, mas, e isso é importante, nunca na hora de escrever. A hora da leitura crítica do próprio texto deve ser diferente e distante da hora da escrita. Por quê isso? Para que o leitor e escritor não se confundam. É preciso haver esse distanciamento para aguçar nosso lado crítico, de quem lê o texto de outra pessoa e acha que poderia ser melhor. No momento de ler meus próprios rascunhos eu sou o revisor deste Pull Request.

E eu posso pedir por modificações.

Os pedidos que eu faço são anotações de trechos do texto. Essas anotações qualquer leitor de ebooks lhe permite jogar para um arquivo texto. Que é o que eu faço. Isso é automático no caso do Kindle, acredito que seja para a maioria dos leitores. Esse arquivo fica no leitor, esperando meu próximo round de escritas. Quando chega esse momento eu copio este arquivo localmente, salvo em um arquivo do meu próprio repositório de textos. Vou dar um exemplo atual:

```
Blogue do Caloni (Wanderley Caloni)
- Highlight Loc. 66342  | Added on Saturday, 3 December 22 00:26:00 GMT-03:00

minhas anotações de leitura. [^1] 
==========
Blogue do Caloni (Wanderley Caloni)
- Highlight Loc. 66346  | Added on Saturday, 3 December 22 00:27:01 GMT-03:00

loquaz, densa, 
==========
Blogue do Caloni (Wanderley Caloni)
- Highlight Loc. 66348  | Added on Saturday, 3 December 22 00:27:28 GMT-03:00

cada detalhe dessa questão é explorada em mais detalhes. 
==========
Blogue do Caloni (Wanderley Caloni)
- Highlight Loc. 66357  | Added on Saturday, 3 December 22 00:29:03 GMT-03:00

eles, quase da mesma forma, as atividades que para eles 
==========
```

Esse não é um formato muito bom para trabalhar, então eu jogo para um arquivo de rascunhos e deixo apenas as linhas que eu anotei de fato.

```
minhas anotações de leitura. [^1] 
loquaz, densa, 
cada detalhe dessa questão é explorada em mais detalhes. 
eles, quase da mesma forma, as atividades que para eles 
```

Então eu edito essas linhas para fazer parte de uma busca coletiva através da função matchadd do Vim.

```
%s/\(.*\) $/call matchadd("search", "\1")/
```

Com isso cada linha conterá um comando válido do Vim para adicionar o trecho sublinhado como parte de um grupo de busca.

```
call matchadd("search", "minhas anotações de leitura. [^1]")
call matchadd("search", "loquaz, densa,")
call matchadd("search", "cada detalhe dessa questão é explorada em mais detalhes.")
call matchadd("search", "eles, quase da mesma forma, as atividades que para eles")
```

Dessa forma eu salvo as modificações e rodo o arquivo como um arquivo de comandos do Vim com `:so %`. O editor irá então realçar todos os itens deste grupo de busca em qualquer arquivo que eu abra. E agora é a hora de abrir os rascunhos ou textos que andei revisando.

Aí chega o momento do bom senso. Eu não escrevo anotações na hora da revisão: eu apenos sublinho. Considero o suficiente. Com base nos trechos que sublinhei eu devo melhorar o parágrafo, a frase ou corrigir a ortografia de alguma palavra. Pode ser qualquer coisa. Muitas vezes eu sublinho um parágrafo inteiro porque ele está problemático. Quando eu fico repetindo palavras no meio da frase eu sublinho o início do uso de uma palavra e o final com a mesma palavra. Enfim, para um bom entendedor um sublinhado basta.

Depois de revisados os textos eles estão prontos ou para serem publicados em sua versão final, quando eles perdem a marcação de rascunho e ganham a categoria e tags que merecem, ou para voltarem para nova revisão. Textos muito longos ou problemáticos ganham várias rodadas de revisão.

Agora é a hora de obter os novos textos que andei escrevendo para inserir também no meu novo livro de rascunhos. Esses textos podem estar em qualquer lugar. Pode ser um email que mando para mim mesmo, mensagens salvas do Telegram ou até a área de transferência atual do computador onde estou. Qualquer origem é válida, assim como a qualidade inicial. "Escreva com a porta fechada", lembra?

Pois bem. Salvos os novos rascunhos em um novo commit, publico as novas modificações e os novos textos em novas versões do blog e dos livros, incluindo o livro de rascunhos. E tudo começa novamente até a próxima leitura.

[meu platô]: {{< ref "livro-mastering-the-key-to-success-and-long-term-fulfillment" >}}
[On Writing]: {{< ref "on-writing-stephen-king" >}}
