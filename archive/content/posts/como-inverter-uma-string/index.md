---
categories:
- coding
date: '2023-04-09'
tags:
- interview
title: Como inverter uma string
---

O bom de estar praticando para fazer entrevistas técnicas é ter material para novos postes. E este poste é sobre um assunto bem simples para quem já sabe como funcionam strings, mas complexo o suficiente para quem nunca ouviu falar de memória no computador.

Vamos começar pela resposta. O algoritmo que deve ser seguido é:

 - Declare a string que você quer inverter;
 - Obtenha o tamanho dessa string;
 - Faça um loop do começo ao fim ou do fim ao começo;
 - No loop atualize a posição inicial e final da string;
 - Troque de posição os elementos final e inicial;
 - Pronto.

Simples, não? Em C++:

```
string ReverseString(string s)
// string passada por parâmetro
{
    size_t begin = 0; // posição inicial
    size_t end = s.size() - 1; // posição final

    while (begin < end) // enquanto não invertemos as posições
    {
        char buf = s[begin]; // troca elementos de posição
        s[begin] = s[end];
        s[end] = buf;

        ++begin; // atualize a posição inicial e final da string
        --end;
    }

    return s; // string invertida
}
```