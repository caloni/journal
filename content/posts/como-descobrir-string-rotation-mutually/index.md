---
categories:
- code
date: '2023-04-09'
tags:
- interview
title: Como descobrir se uma string é mutuamente rotativa
---

Uma string mutuamente rotativa é uma string que se rotacionarmos para a direita ou para a esquerda, com os caracteres "indo parar" do outro lado, é comparável com a string original. Exemplos:

 - "abacate" é mutuamente rotativa com "cateaba";
 - "roma" é mutuamente rotativa com "maro";
 - "ab" é mutuamente rotativa com "ba";
 - "123456" é mutuamente rotativa com "456123".

Há alguns passos simples e um código esperto que consegue verificar isso. Os passos são os seguinte:

 - Inicialize as duas strings em duas variáveis;
 - Veja se o tamanho das duas é similar (se não retorne false);
 - Junte a primeira string com ela mesma (s = s + s);
 - Verifica se a segunda string existe na string duplicada;
 - Se existir quer dizer que uma é rotação da outra.

Ficou confuso? Vai ficar mais simples ao ver a implementação em C++:

```
bool RotationMutually(string s1, string s2)
{
    if (s1.size() != s2.size()) return false;
    s1 += s1;
    return s1.find(s2) != s1.npos;
}
```