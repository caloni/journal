---
categories:
- coding
date: '2023-04-11'
tags:
- interview
title: Como inverter um número
---

Existe uma solução para a inversão de um número que não é bonita, mas prática: transforme em string e inverta essa string.

```
int ReverseNumberStringVersion(int number)
{
    string s = to_string(number);
    size_t beg = 0, end = s.size() - 1;
    while (beg < end)
    {
        char buf = s[beg];
        s[beg] = s[end];
        s[end] = buf;
        ++beg;
        --end;
    }
    return stoi(s);
}
```

Contudo, a maneira bonita de se fazer isso é mantendo o domínio do problema na matemática. E dessa forma:

- Pegue o dígito mais à direita do número;
- Some o dígito com o novo número invertido;
- Multiplique o resultado por 10;
- Divida o número por 10.

```
int ReverseNumberNumberVersion(int number)
{
    int reversedNumber = 0;

    while (number) {
        reversedNumber *= 10;
        int next = number % 10;
        reversedNumber += next;
        number /= 10;
    }

    return reversedNumber;
}
```