---
title: Entendendo a Compilação
---

Esses slides foram compilados a pedido dos organizadores do TDC 2014, já que a palestra que ministrei com esse tema foi para ajudar meu amigo-sócio Rodrigo Strauss que não havia preparado nenhum slide a respeito.

Felizmente eu já havia explicado alguns conceitos-chave para quem programa em C/C++ e precisa -- eu disse: PRECISA -- conhecer todo o passo-a-passo que leva o seu código-fonte a gerar um executável com código de máquina pronto para rodar.

<iframe src="//www.slideshare.net/slideshow/embed_code/43190892" width="640" height="480" frameborder="0" marginwidth="0" marginheight="0" scrolling="no"></iframe>

Como havia [explicado anteriormente]({% post_url 2008-02-15-os-diferentes-erros-na-linguagem-c.md %}), existem três processos principais e clássicos (pode haver mais, dependendo do compilador, ambiente, etc) na formação de um código de máquina a partir de arquivos-fontes escritos em C ou C++ (ou ambos, são intercambiáveis). São eles:

 1. __Preprocessamento__
 2. __Compilação__
 3. __Linkedição__

