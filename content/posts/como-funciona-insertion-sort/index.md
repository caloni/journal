---
categories:
- code
date: '2023-04-09'
tags:
- interview
title: Como funciona o insertion sort
---

Entre os algoritmos de ordenação mais simples de se entender o insertion sort está na lista. E isso acontece porque ele é intuitivo. É mais ou menos como podemos fazer para ordenar um deck de cartas: pegamos item a item e vamos inserindo em um segundo deck, mas dessa vez observando onde cada carta deve ser inserida para que o deck final esteja ordenado.

Implementar isso em código segue o mesmo princípio, mas em vez de ter um segundo deck podemos dividir o deck, ou array, em dois: ordenado e não ordenado. Varremos o array desordenado e movemos cada elemento para a posição em que ele deve ficar no array ordenado.

Para dividir o array usamos um índice. Peguemos o segundo elemento, por exemplo. O primeiro elemento já está ordenado, pois está sozinho. A partir daí verificamos onde o segundo elemento será colocado: onde está ou antes do primeiro elemento. Decidido isso partimos para o terceiro elemento, onde fazemos a mesma comparação, primeiro com o primeiro ou segundo elemento, depois com o outro. Então fazemos a troca (ou não). E assim por diante. No final, quando estivermos comparando o último elemento, todo o deck ordenado estará completo.

Essa lógica computacional é boa de entender porque realiza uma mímica com a lógica humana, passo a passo. Imagine um ser humano pegando a terceira carta, comparando com a segunda, comparando com a primeira, dessa forma realizando a troca entre a primeira e a terceira, por exemplo.

A parte não intuitiva ou mais complexa é que para inserir um elemento que deveria estar na segunda posição, mas está na quinta, é necessário ir realizando a troca entre o quinto e quarto elementos, depois entre o terceiro e o quarto, até chegar na segunda posição. Isso se tratando de um array. Esse algoritmo pode funcionar mais rápido se utilizada uma lista ligada (menos trocas).

Em código C++ ficaria assim:

```
vector<int> InsertionSort(vector<int> array)
{
    for (size_t i = 1; i < array.size(); ++i)
    {
        size_t j = i;
        while ( j > 0 )
        {
            if (array[j-1] > array[j])
            {
                swap(array[j-1], array[j]);
            }
            --j;
        }
    }
    return array;
}
```

