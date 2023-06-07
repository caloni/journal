---
categories:
- reading
- writting
date: '2023-03-14'
tags:
- books
title: Algorithms for Dummies
---

De John Mueller e Luca Massaron, este livro mantém a fama da série "for Dummies", com livros de conteúdo bem estruturado, acessível para a maioria dos leitores e ainda útil para quem já está acostumado com um termo ou dois do assunto, ou até o assunto inteiro.

A linguagem usada para resolver a criação ou uso de algoritmos conhecidos é o Python, o que inibe a barreira de entrada até mesmo de quem nunca programou na vida. Não contentes com isso, o autores colocaram dois capítulos no início que ensinam o leitor o bê-a-bá do Python e das bibliotecas utilizadas. O código-fonte está todo disponível em um repositório do GitHub.

Existe uma introdução sobre complexidade de algoritmos que é corrida, mas igualmente útil, pois ajuda a não nos aprofundarmos tanto assim na matemática envolvida quando se fala em velocidade e economia de recursos ao executar um código na prática, mas ao mesmo tempo mantém na memória a questão de como desenhar o algoritmo terá impacto profundo em sua execução com uma massa de dados maior que a ideal.

Ainda estou lendo, mas já consegui uma frase intrigante logo no início, referente a como escrever soluções recursivas, intuitivas por natureza, difíceis de mantê-la sob controle.

> When working with recursion, you must remain proactive or endure the consequences later.

> Without organization, making any sort of decision becomes impossible. An algorithm might end up going in circles or giving inconvenient output.

> When faced with difficult problems, you quickly discover that no magic potion exists for making wishes come true or silver bullets to dispel bad things. Similarly, no algorithmic technique saves the day every time. That's the no-free-lunch principle often quoted in the book. The good news is, you can arm yourself with different general techniques and test them all on your problem, because you have a good chance that something will work well.

> A greedy algorithm reaches a problem solution using sequential steps where, at each step, it makes a decision based on the best solution at that time, without considering future consequences or implications.

> Two elements are essential for distinguishing a greedy algorithm: At each turn, you always make the best decision you can at that particular instant. You hope that making a series of best decisions results in the best final solution.

> Puzzle problems, in which the number of possible decisions can become huge, are more frequent than you expect. Some problems of this type have already been solved, but many others aren't, and we can't even transform them (yet) into versions we know how to solve. Until someone is smart enough to find a generic solution for these problems, a greedy approach may be the easiest way to approach them, provided that you accept that you won't always be getting the best solution but a roughly acceptable one instead (in many cases). These difficult problems vary in characteristics and problem domain. Different examples of difficult problems are protein unfolding (which can help cure cancer) or breaking strong password encryption, such as the popular RSA cryptosystem

> In the 1960s, researchers found a common pattern for all of them: They are all equally difficult to solve. This pattern is called the NP-completeness theory (NP stands for nondeterministic polynomial). In a sense, these problems distinguish themselves from others because it's not yet possible to find a solution in a reasonable time —that is, in polynomial time. Polynomial time means that an algorithm runs in powers of the number of inputs (known as P problems). Linear time is polynomial time because it runs O(n1). Also quadratic O(n2) and cubic O(n3) complexities are polynomial time, and though they grow quite fast, they don't compare to NP-complete complexity, which is usually exponential time, that is, O(cn).

> (...) when you can break down a problem into nested subproblems can dynamic programming beat brute-force approaches that repeatedly rework the same subproblems.

> You can trace dynamic programming to two large families of solutions: Bottom-up: Builds an array of partial results that aggregate into a complete solution Top-down: Splits the problem into subproblems, starting from the complete solution (this approach is typical of recursive algorithms) and using memoization (defined in the next section) to avoid repeating computations more than once