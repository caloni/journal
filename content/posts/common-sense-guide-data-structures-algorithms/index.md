---
categories:
- reading
- code
date: '2023-04-10T17:42:06-02:00'
draft: true
tags:
- interview
title: A Common-Sense Guide to Data Structures and Algorithms
---

Meu próximo livro para praticar entrevistas técnica é este A Common-Sense Guide to Data Structures and Algorithms, de Jay Wengrow. Jay queria explicar de maneira menos matemática e alienígena para programadores como algoritmos e estruturas de dados funcionam e como conceber bons algoritmos e medir a eficiência de algoritmos já prontos. Tudo isso serve também para você que deseja passar nas entrevistas técnicas e suas pegadinhas.

Ainda não terminei o livro, mas achei a didática de Jay Wengrow fabulosa, visto que os dois livros anteriores eram muito chatos e burocráticos. Aqui Jay está conversando de programador para programador. Mesmo que você nunca tenha ouvido falar em complexidade de algoritmo e nem seja um programador tão experiente assim vai conseguir entender os fundamentos da estruturas de dados e como manipular dados na memória de diversas maneiras.

Jay vai passando seu conhecimento aos poucos. Primeiro ele simplifica demais e usa analogias que nos deixam em uma zona de conforto. Depois ele vai complicando aos poucos. Quando menos esperamos já estamos sabendo o porquê de detalhes da computação que antigamente apenas aceitávemos como verdade. Ele me faz lembrar muito a didática do Tanenbaum, que constrói um argumento usando bom senso.

Bom, o título de seu livro já denuncia: este é um guia livre de bullshitagem escrito por quem coloca a mão na massa.

# Recortes

> Here’s what the notation means. It expresses the answer to what we’ll call the “key question.” The key question is: if there are N data elements, how many steps will the algorithm take? Go ahead and read that sentence again. Then, emblazon it on your forehead, as this is the definition of Big O Notation that we’ll be using throughout the rest of this book.

> Big O is originally a concept from mathematics, and therefore, it’s often described in mathematical terms. For example, one way of describing Big O is that it describes the upper bound of the growth rate of a function, or that if a function g(x) grows no faster than a function f(x), then g is said to be a member of O(f).

> If you want to dig further into the math behind Big O, check out Introduction to Algorithms by Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, and Clifford Stein (MIT Press, 2009) for a full mathematical explanation. Justin Abrahms also provides a pretty good definition in his article: https://justin.abrah.ms/computer-science/understanding-big-o-formal-definition.html.

> The soul of Big O is what Big O is truly concerned about: how will an algorithm’s performance change as the data increases? This is the soul of Big O. Big O doesn’t want to simply tell you how many steps an algorithm takes. It wants to tell you the story of how the number of steps increase as the data changes.

> Because there will always be some amount of data at which the tides turn, and O(N) takes more steps from that point until infinity, O(N) is considered to be, on the whole, less efficient than O(1) no matter how many steps the O(1) algorithm actually takes.

> The same is true even for an O(1) algorithm that always takes one million steps. As the data increases, there will inevitably reach a point at which O(N) becomes less efficient than the O(1) algorithm, and will remain so up toward an infinite amount of data.
