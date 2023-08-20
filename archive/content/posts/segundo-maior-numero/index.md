---
categories:
- coding
date: '2023-04-09'
tags:
- interview
title: Segundo maior número
---

Esta é uma das primeiras questões que peguei para praticar para entrevistas que é ligeiramente mais complicada do que parece, apesar de simples o suficiente para matar em alguns segundos. A questão: como determinar qual o segundo maior número de um array?

Note que não é o maior número, mas o segundo maior. O que parece fácil. O que está implícito e o candidato deve descobrir é que para saber o segundo maior é necessário manter o tracking do primeiro todo o tempo.

Sempre que precisar resolver problemas com segundos ou terceiros elementos você deve manter uma segunda ou terceira variável. A primeira variável mantém o maior elemento e a segunda variável o segundo maior elemento. Sempre que você encontrar algum elemento maior que esses dois você deve atualizá-los de acordo. Depois de varrer toda a lista a segunda variável irá conter o segundo maior número.

Em C++:

```
int SecondLargest(vector<int> array)
{
    int first = max(array[0], array[1]);
    int second = min(array[0], array[1]);

    for (size_t i = 2; i < array.size(); ++i)
    {
        if (array[i] >= first)
        {
            second = first;
            first = array[i];
        }
        else if( array[i] > second )
        {
            second = array[i];
        }
    }

    return second;
}
```