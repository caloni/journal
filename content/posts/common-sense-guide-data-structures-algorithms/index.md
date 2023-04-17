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

Meu próximo livro para praticar entrevistas técnicas é este de Jay Wengrow. Jay queria explicar de maneira menos matemática e alienígena para programadores como algoritmos e estruturas de dados funcionam e como conceber bons algoritmos e medir a eficiência de algoritmos já prontos. Tudo isso serve também para você que deseja passar nas entrevistas técnicas e suas pegadinhas.

Ainda não terminei o livro, mas achei a didática de Jay Wengrow fabulosa, visto que os dois livros anteriores, [Algorithm for Dummies]{{< ref algorithms-for-dummies >}} e [Cracking the code interview]({{< ref cracking-the-coding-interview >}} eram muito chatos e burocráticos. Aqui Jay está conversando de programador para programador. Mesmo que você nunca tenha ouvido falar em complexidade de algoritmo e nem seja um programador tão experiente assim vai conseguir entender os fundamentos de estruturas de dados e como manipulá-los na memória de diversas maneiras.

Jay vai passando seu conhecimento aos poucos. Primeiro ele simplifica demais e usa analogias que nos deixam em uma zona de conforto. Depois ele vai complicando aos poucos. Quando menos esperamos já estamos sabendo o porquê de detalhes da computação que antigamente apenas aceitávamos como verdade. Ele me faz lembrar muito a didática do Tanenbaum, que constrói um argumento usando bom senso.

Bom, o título de seu livro já denuncia: este é um guia livre de bullshitagem escrito por quem coloca a mão na massa.

# Recortes

> Here’s what the notation means. It expresses the answer to what we’ll call the “key question.” The key question is: if there are N data elements, how many steps will the algorithm take? Go ahead and read that sentence again. Then, emblazon it on your forehead, as this is the definition of Big O Notation that we’ll be using throughout the rest of this book.

> Big O is originally a concept from mathematics, and therefore, it’s often described in mathematical terms. For example, one way of describing Big O is that it describes the upper bound of the growth rate of a function, or that if a function g(x) grows no faster than a function f(x), then g is said to be a member of O(f).

> If you want to dig further into the math behind Big O, check out Introduction to Algorithms by Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, and Clifford Stein (MIT Press, 2009) for a full mathematical explanation. Justin Abrahms also provides a pretty good definition in his article: https://justin.abrah.ms/computer-science/understanding-big-o-formal-definition.html.

> The soul of Big O is what Big O is truly concerned about: how will an algorithm’s performance change as the data increases? This is the soul of Big O. Big O doesn’t want to simply tell you how many steps an algorithm takes. It wants to tell you the story of how the number of steps increase as the data changes.

> Because there will always be some amount of data at which the tides turn, and O(N) takes more steps from that point until infinity, O(N) is considered to be, on the whole, less efficient than O(1) no matter how many steps the O(1) algorithm actually takes.

> The same is true even for an O(1) algorithm that always takes one million steps. As the data increases, there will inevitably reach a point at which O(N) becomes less efficient than the O(1) algorithm, and will remain so up toward an infinite amount of data.

> This is actually the reason why this algorithm is called Bubble Sort: in each pass-through, the highest unsorted value “bubbles” up to its correct position.

> In reality, however, Selection Sort is described in Big O as O(N2), just like Bubble Sort. This is because of a major rule of Big O that I’m now introducing for the first time: Big O Notation ignores constants.

> This is simply a mathematical way of saying that Big O Notation never includes regular numbers that aren’t an exponent. We simply drop these regular numbers from the expression.

> However, when two algorithms fall under the same classification of Big O, it doesn’t necessarily mean that both algorithms have the same speed. After all, Bubble Sort is twice as slow as Selection Sort even though both are O(N2). So, while Big O is perfect for contrasting algorithms that fall under different classifications of Big O, when two algorithms fall under the same classification, further analysis is required to determine which algorithm is faster.

> Big O Notation only takes into account the highest order of N when we have multiple orders added together.

> That is, if we have an algorithm that takes N4 + N3 + N2 + N steps, we only consider N4 to be significant—and just call it O(N4). Why is this?

> One type of problem in which recursion is a natural fit is when we need to delve into multiple layers of a problem without knowing how many layers there are.
