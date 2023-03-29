---
categories: null
date: '2023-03-29T21:02:57-02:00'
draft: true
tags: null
title: X64 Deep Dive
---

Recomento esta leitura para se habituar às idiossincrasias sobre o formato assembly do x64, especialmente se você costuma depurar assembly para Windows. O artigo cobre sobre o formato PE e as novas features para suportar 64 bits, além de explicar em detalhes o funcionamento de mecanismos que mudaram, como o tratamento de exceção (o unwinding no código).

# Recortes

> On X64, the first 4 parameters are always passed in registers and the rest of the parameters are passed via the stack. This is one of main causes of grief during debugging since register values tend to change as functions execute and it becomes difficult to determine the original parameter values that were passed to a function, half-way into its execution. Other than this one issue with retrieving parameters, x64 debugging is not that different from x86 debugging.