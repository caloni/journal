---
author: admin
comments: true
date: 2011-10-09 16:57:56+00:00
layout: post
slug: coders-at-work-jamie-zawinski-e-douglas-crockford
title: 'Coders at Work: Jamie Zawinski e Douglas Crockford'
wordpress_id: 1201
categories:
- Blogging
tags:
- entrevistas
- estudo
- leituras
- metodologia
- processos
- programação
---

<blockquote>_"Personally I have never believed that it is possible to be a good coder without being a good programmer nor a good programmer without being a good designer, communicator, and thinker."_ - Jamie Zawinski</blockquote>




Como [Joel](http://www.joelonsoftware.com/items/2009/09/23.html) e [Atwood](http://www.codinghorror.com/blog/2009/10/the-interview-with-the-programmer.html) disseram, a leitura de [Coders At Work](http://www.codersatwork.com/) é tão útil quanto ler o código dos outros, só que em um estilo mais condensado, que se aproveita das décadas de experiência dessa gente para aprimorarmos nossos processos de desenvolvimento e, muitas vezes, a forma de pensarmos sobre software.

[caption id="attachment_1202" align="aligncenter" width="203" caption="Coders at Work: Programadores no Hall da Fama"][![](/images/cover-big-203x300.jpg)](/images/cover-big.jpg)[/caption]


[
](/images/cover-big.jpg)


No meu estilo de leitura circular, adaptada do brilhante (maluco?) método de [Dmitry Vostokov](http://www.softwaregeneralist.com/category/books/), as coisas vão mais devagar, e estou apenas no início do livro, tendo passado por **Jamie Zawinski** (desenvolvedor da equipe original do Netscape), Brad Fitzpatrick (criador do Live Journal) e terminado recentemente **Douglas Crockford**. O artigo de Joel sobre Zawinski demonstra seu apreço pelo codificador pensante, ou aquele que faz as coisas acontecerem e não fica preso eternamente na armadilha da arquitetura. Eu acredito que as seguintes passagens do livro demonstram seu pensamento melhor do que se eu fosse tentar traduzi-los.


#### Jamie Zawinski


[caption id="attachment_1217" align="aligncenter" width="186" caption="Jamie Zawinski"][![](/images/jamie.jpg)](/images/jamie.jpg)[/caption]

_"Personally I have never believed that it is possible to be a good coder without being a good programmer nor a good programmer without being a good designer, communicator, and thinker. (...) Start converting it into the bad one until it stops working. That’s primary tool of reverse engineering. (...) Your competitor’s six-month 1.0 has crap code and they’re going to have to rewrite it in two years but, guess what: they can rewrite it because you don’t have a job anymore. (...) The design process is definitely an ongoing thing; you never know what the design is until the program is done. So I prefer to get my feet wet as early as possible; get something on the screen so I can look at it sideways.(...) I’ve noticed that one thing that separates good programmers from bad programmers is that good programmers are more facile at jumping between layers of abstraction—they can keep the layers distinct while making changes and choose the right layer to make changes in. (...) I think one of the most important things, for me anyway, when building something from the ground up like that is, as quickly as possible, getting the program to a state that you, the programmer, can use it. Even a little bit. Because that tells you where to go next in a really visceral way. (...) I don’t want to be a mathematician but I’m not going to criticize someone who is a mathematician. It’s weird that people often confuse those two pursuits. People who are into very theoretical computer science are thought of in this same way as people who are shipping desktop applications. And they don’t really have a lot to do with each other. (...) Then there was another book that everybody thought was the greatest thing ever in that same period—Design Patterns—which I just thought was crap. It was just like, programming via cut and paste. Rather than thinking through your task you looked through the recipe book and found something that maybe, kinda, sorta felt like it, and then just aped it. That’s not programming; that’s a coloring book. (...)"_


#### Douglas Crockford


De certa forma, o mesmo pragmatismo pode ser observado em Douglas Crockford, que utiliza o método de leitura de código tanto na entrevista por candidatos ("traga-me o código que tem orgulho de ter escrito e explique-o pra mim") quanto no dia-a-dia do projeto, para que todos entendam e aproveitem a evolução do projeto como um todo, além de constituir, na minha visão, uma das melhores dicas de auto-management que uma equipe de programadores poderia ter.

[caption id="attachment_1218" align="aligncenter" width="300" caption="Douglas Crockford"][![](/images/crockford-300x300.png)](/images/crockford.png)[/caption]

_"One of the things I’ve been pushing is code reading. I think that is the most useful thing that a community of programmers can do for each other—spend time on a regular basis reading each other’s code. There’s a tendency in project management just to let the programmers go off independently and then we have the big merge and then we have the big merge and if it builds then we ship it and we’re done and we forget about it. One of the consequences of that is that if you have weak or confused programmers you’re not aware of their actual situation until much too late. And so the risks to the project, that you’re that you’re going to have to build with stuff that’s bad and the delays that that causes, that’s unacceptable. The other thing is that you may have brilliant programmers on the project who are not adequately mentoring the other people on the team. Code reading solves both of those problems._"

**Seibel**: Can you talk a bit about how you conduct a code reading?

_"At each meeting, someone’s responsible for reading their code, and they’ll walk us through everything, and the rest of us will observe. It’s a really good chance for the rest of the team to understand how their stuff is going to have to fit with that stuff."_

_"We get everybody around the table; everybody gets a stack of paper. We also blow it up on the screen. And we all read through it together. And we’re all commenting on the code as we go along. People say, “I don’t understand this comment,” or, “This comment doesn’t seem to describe the code.” That kind of stuff can be so valuable because as a programmer you stop reading your own comments and you’re not aware that you’re misdirecting the reader. Having the people you work with helping to keep your code clean is a huge service—you find defects that you never would’ve found on your own."_

_"I think an hour of code reading is worth two weeks of QA. It’s just a really effective way of removing errors. If you have someone who is strong reading, then the novices around them are going to learn a lot that they wouldn’t be learning otherwise, and if you have a novice reading, he’s going to get a lot of really good advice."_

**Seibel**: So if you don’t clean up every seventh cycle you may be faced with the choice of whether or not to do a big rewrite. How do you know when, if ever, it’s time for a big rewrite?

_"Generally the team knows when it’s time. Management finds out a lot later. The team is getting beat up pretty regularly, making too many pretty regularly, making too many bugs; the code’s too big, it’s too slow; we’re falling behind. They know why. It’s not because they became stupider or lazier. It’s because the code base is no longer serving the purpose that it needs to."_
