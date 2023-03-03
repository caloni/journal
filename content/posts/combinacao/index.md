---
categories: null
date: '2023-03-03T10:10:18-02:00'
draft: true
tags: null
title: Combinação
---

Tentando agilizar um dos desafios de programação que tenho treinado encontro como fazer combinação em C++ ou C de maneira mais rápida que a fórmula oficial que requer calcular o fatorial e mais algumas contas. O nome desse problema é NCK, de N Chooses K, onde N é o número de elementos e K a amostragem obtida para a combinação. O resultado é instantâneo, pois se trata de um algoritmo O(n).

unsigned nChoosek( unsigned n, unsigned k )
{
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int result = n;
    for( int i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}