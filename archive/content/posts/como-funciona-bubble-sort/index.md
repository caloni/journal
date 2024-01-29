---
categories:
- coding
date: '2023-04-09'
tags:
- interview
title: Como funciona o bubble sort
---

Uma das piores ordenações possíveis, mas uma das mais simples de entender, é a bubble sort. Ela é passada para estudantes de computação porque é um algoritmo possível de explicar sem entrar em muitos detalhes do seu funcionamento, e também porque seu funcionamento é intuitivo.

O objetivo do algoritmo é ordenar uma lista.

Para conseguir ordenar o algoritmo precisa comparar todos os elementos e trocar as posições dos elementos desordenados, um a um.

Como a lista é varrida de cabo a rabo os elementos na posição mais errada possível vão caminhando em direção à sua posição correta.

Ou seja, se o primeiro elemento estiver na última posição são feitas N-1 trocas, do último para o penúltimo, do penúltimo para o antepenúltimo e assim por diante, até que o loop que varre todos os elementos faça a última comparação, entre o primeiro e o segundo elemento.

Note que é necessário fazer o caminho reverso para mover um elemento que está em primeiro, mas que deveria estar em último.

Para isso o loop que percorre a lista inteira precisa primeiro garantir que todo o resto da lista está ordenado.

Para conseguir isso a cada passada do primeiro loop é feito outro loop com o resto da lista.

Isso é feito da seguinte forma em C++:

```
vector<int> BubbleSort(vector<int> array)
{
    for (size_t i = 0; i < array.size(); ++i)
    {
        for (size_t j = 0; j < array.size() - 1 - i; ++j)
        {
            if (array[j] > array[j + 1])
            {
                swap(array[j], array[j + 1]);
            }
        }
    }
    return array;
}
```

Se você leu o código deve ter percebido que existe um pequeno truque na hora de estipular o final do loop mais interno: ele diminui do tamanho do array menos o que já foi percorrido do elemento do loop externo. Em outras palavras, quanto mais avançarmos para a frente com o índice do loop externo menos avançamos para o final do array.

Isso é feito porque sabemos que já foram feitas i comparações antes, o que quer dizer que os elementos na posição size-i já foram devidamente movidos para dentro do intervalo que será comparado na passada deste próximo loop interno.

Para sentir o passo a passo dessas iterações, observe como os elementos de uma lista completamente desordenada se comporta e até onde vão as comparações do loop interno e, o mais importante, por que ele não precisa ir mais além.

```
i = 0
6 5 4 3 2 1
j - - - - j
5 4 3 2 1 6

i = 1
5 4 3 2 1 6
j - - - j
4 3 2 1 5 6

i = 2
4 3 2 1 5 6
j - - j
3 2 1 4 5 6

i = 3
3 2 1 4 5 6
j - j
2 1 3 4 5 6

i = 4
2 1 3 4 5 6
j j
1 2 3 4 5 6
```

Parece um algoritmo rápido, mas isso acontece porque está escondido cada uma das comparações e trocas do loop interno. Por exemplo, no primeiro bloco acima foram feitas cinco comparações e cinco trocas (N-1).

A complexidade deste algoritmo é de O(n^2) comparações e O(n^2) trocas.

Ou seja, nada bom. Mas fácil de entender =)