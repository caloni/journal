---
categories:
- coding
date: '2023-04-11'
tags:
- interview
title: Como verificar se um número é primo
---

Apesar de existir matemáticos ao redor do mundo tentando responder esta pergunta da maneira computacionalmente mais rápida possível, existe uma forma ingênua e eficiente para números baixos:

- Faça um loop entre 2 e a metade do número;
- Se algum desses números dividir sem resto retorne false;
- Se acabar o loop retorne true: o número é primo.

Note que a mesma lógica pode ser aplicada para obter os fatores de um número, seus divisores, etc.

O código em C++:

```
bool PrimeNumber(int number)
{
    if (number == 2) return true;

    for (int i = 2; i <= (number / 2); ++i)
    {
        if (number % i == 0)
        {
            return false;
        }
    }

    return true;
}
```