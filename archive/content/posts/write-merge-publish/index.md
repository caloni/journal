---
categories:
 - blogging
date: '2023-06-25'
tags:
 - projects
title: 'Write, merge, publish'
---

Hoje provei uma ideia ainda melhor que usar a awklib integrada com uma solução C++: o publicador de conteúdo ser um plugin do gawk. Dessa forma o script de transformação de texto está totalmente nas mãos do desenvolvedor (eu) e a velocidade não é impactada nunca (exceto se eu quiser rodar awk em multithreading).

A ideia de arquitetura que está sendo seguida meio por acaso pode ser o core do novo publicador. Dois componentes: merge e publish.

O merge é o componente que obtém conteúdo de N fontes e transforma em um arquivo único em um formato qualquer. Por que único? Porque múltiplos arquivos trabalham mal no SO e o dia a dia de um blogger é muito mais fácil através de um `journal.txt` do que ficar se enveredando nas referências que o sistema de publicação te obriga a usar.

O publish é a transformação do arquivo de texto único para N destinos diferentes: um ou mais blogues e livros, Telegram, RSS, Twitter. Não importa. A definição dos destinos e feito no próprio script awk em conjunto com sua transformação. Por que isso? Porque um escritor não deve se limitar a um único formato. Seu único objetivo na vida é escrever. Seus metadados e o formato do seu texto irão definir para onde vai o conteúdo, mas ele não precisa se preocupar com isso.
