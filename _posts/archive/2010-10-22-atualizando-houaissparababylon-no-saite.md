---
author: admin
comments: true
date: 2010-10-22 01:26:20+00:00
layout: post
slug: atualizando-houaissparababylon-no-saite
title: Atualizando HouaissParaBabylon no saite
wordpress_id: 952
categories:
- Projetos
tags:
- houaiss2babylon
---

O [último comentário no meu último artigo](http://www.caloni.com.br/blog/houaissparababylon-versao-beta#comment-19312) sobre o conversor Houaiss para Babylon me fez lembrar de algo muito importante: eu não atualizei o branch do saite com a última versão. Deve ser por isso que as pessoas estão tendo problemas com o uso do código. Resolvo isso já:



	
  * [HouaissParaBabylon](../public/uploads/houaissparababylon12.7z)


Essa é a versão 1.2 descrita no meu [último artigo sobre o projeto](http://www.caloni.com.br/blog/houaiss-para-babylon-12).

De qualquer forma, qual não foi minha surpresa quando tentei recompilar o projeto e ocorreram erros no atlcom. Depois de uma [breve pesquisa](http://social.msdn.microsoft.com/Forums/en-US/vclanguage/thread/ad698507-d62c-4e7b-bb7a-12a03b939594) descobri que precisava rodar alguns "patches" para o include funcionar direito. Então, provavelmente, Willians, era esse o problema. Tente de novo.


