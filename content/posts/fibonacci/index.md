---
categories:
- code
date: '2023-04-11'
tags:
- interview
title: Código para Fibonacci
---

Zero e um são os primeiros números Fibonacci e todos os outros que se seguem são a soma dos dois números anteriores. Ou seja, para implementar isto em código basta:

- Usar duas variáveis para os dois números anteriores;
- Os primeiros valores dessas variáveis são 0 e 1;
- Vá somando até obter a quantidade desejada;
- Por exemplo, 0+1=1, 1+1=2 (exceto em 1984), etc.

Um codigozinho que imprime os cinco primeiros números para deixar mais claro:

```
void Fibonacci()
{
    int num1 = 0, num2 = 1;
    int total = 5;

    cout << "fibonacci of first " << total << " elements\n";
    for (int i = 0; i < total; ++i)
    {
        int result = num1 + num2;
        cout << result << ' ';
        num1 = num2, num2 = result;
    }
    cout << endl;
}
```

