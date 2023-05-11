---
categories:
- code
- reading
date: '2023-04-07'
tags: null
title: Cracking the code interview
---

Este livro foi recomendado pela minha amiga para treinar para as entrevistas técnicas que ando fazendo. Escolhi ler este em seguida após terminar o [Algorithm For Dummies]({{< ref algorithms-for-dummies >}}). As primeiras anotações são como compor o CV e qual a estratégia de cada big tech nos seus processos. Escapei esta parte, não estou interessado em trabalhar em um Google da vida. Porém, há alguns detalhes que achei relevante recortar.

Estava em cerca de 8% do livro e desisti. Suas dicas são deveras avançadas para quem está apenas querendo tirar a ferrugem de algoritmos. Iniciei um outro chamado A Common-Sense Guide to Data Structures que parece mais a minha cara.

# Recortes

>
> False negatives are acceptable. This is sad (and frustrating for candidates), but true. From the company's perspective, it's actually acceptable that some good candidates are rejected. The company is out to build a great set of employees. They can accept that they miss out on some good people.
> (...)
> They're far more concerned with false positives: people who do well in an interview but are not in fact very good.
>

> Could you learn it as needed? Sure. But it's very difficult to know that you should use a binary search tree if you don't know of its existence. And if you do know of its existence, then you pretty much know the basics.

> Whiteboards also tend to encourage candidates to speak more and explain their thought process. When a candidate is given a computer, their communication drops substantially.

>
> If you are thinking right now that you have too much experience and can't fit it all on one or two pages, trust me, you can. Long resumes are not a reflection of having tons of experience; they're a reflection of not understanding how to prioritize content.
> (...)
> For each role, try to discuss your accomplishments with the following approach: "Accomplished X by implementing Y which led to z". Here's an example:
> - Reduced object rendering time by 75% by implementing distributed caching, leading to a 10% reduction in log-in time.
> Here's another example with an alternate wording:
> - Increased average match accuracy from 1.2 to 1.5 by implementing a new comparison algorithm based on windiff.
>

> Which one is faster? The first one does one for loop and the other one does two for loops. But then, the first solution has two lines of code per for loop rather than one. If you're going to count the number of instructions, then you'd have to go to the assembly level and take into account that multiplication requires more instructions than addition, how the compiler would optimize something, and all sorts of other details. This would be horrendously complicated, so don't even start going down this road. Big O allows us to express how the runtime scales. We just need to accept that it doesn't mean that O(N) is always better than O(N2).

> We already said that we drop constants. Therefore, 0( N2 + N2) would be O ( N2 ). If we don't care about that latter N2 term, why would we care about N? We don't. You should drop the non-dominant terms.

> If your algorithm is in the form "do this, then, when you're all done, do that" then you add the runtimes. If your algorithm is in the form "do this for each time you do that" then you multiply the runtimes.

> This is a good takeaway for you to have. When you see a problem where the number of elements in the problem space gets halved each time, that will likely be a 0( log N) runtime.

> Rather than making assumptions, let's derive the runtime by walking through the code.