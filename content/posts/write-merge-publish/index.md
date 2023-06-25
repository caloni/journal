---
categories: null
date: '2023-06-24T21:29:41-03:00'
draft: true
tags: null
title: 'Write, merge, publish'
---

hoje provei uma ideia ainda melhor: o publicador de conteúdo ser um plugin do gawk. dessa forma o script de transformação de texto está totalmente nas mãos do desenvolvedor (eu) e a velocidade não é impactada nunca (exceto se eu quiser rodar awk em multithreading)

a ideia de arquitetura que está sendo seguida meio por acaso pode ser o core do novo publicador. dois componentes: merge e publish.

o merge é o componente que obtém conteúdo de N fontes e transforma em um arquivo único em um formato qualquer. pq único? pq múltiplos arquivos trabalham mal no SO e o dia a dia de um blogger é muito mais fácil através de um journal.txt do que ficar se enveredando nas referências que o sistema de publicação te obriga a usar

o publish é a transformação do arquivo de texto único para N destinos diferentes: um ou mais blogs e livros, telegram, rss, twitter. não importa. a definição dos destinos e feito no próprio script awk em conjunto com sua transformação. pq isso? pq um escritor não deve se limitar a um único formato. seu único objetivo na vida é escrever. seus metadados e o formato do seu texto irá definir para onde vai o conteúdo, mas ele não precisa se preocupar com isso. o publish é seu personal publisher
