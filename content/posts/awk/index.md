---
categories:
- code
date: '2020-06-07'
link: https://github.com/Caloni/lectures/blob/master/awk.pdf
tags: null
title: Awk
---

# Introduction

Meu amigo sugeriu que aprender awk poderia ser útil de várias maneiras. Uma delas para organizar finanças pessoais. Dei uma lida em alguns tutoriais, sendo que o melhor custo benefício foi o [Awk in 20 Minutes](https://ferd.ca/awk-in-20-minutes.html), de Fred Hebert (ele é o mesmo autor de um livro sobre erlang). Gostei. É sobre tratamento de texto como sed, mas em uma versão estendida e criada na época com a mesma sintaxe de C.

Agora que vou palestrar sobre no Native Floripa de 2022 eu dei uma olhada em outro tutorial muito bom que eu vou chamar de [Tutorial from an Experient Mind](https://www.grymoire.com/Unix/Awk.html), de Bruce Barnett. Ele vai um pouco além porque é de alguém interessado em contar alguns macetes que dão uma visão mais interna da linguagem. E para referência eu considero o [Gawk Guide](https://www.gnu.org/software/gawk/manual/gawk.html) como um ótimo compilado.

## 1977

Por ela ser uma ferramenta antiga, criada por Alfred Aho, Peter Weinberger e Brian Kernighan na década de 70, AWK usa conceitos antigos, como field e record. Ela foi criada para formatar texto em formato de planilha, ou banco de dados. Um field, ou campo, é uma coluna na planilha, e um record, ou registro, é uma linha dessa planilha. Imagine que você pode usar awk para manipular e extrair dados de qualquer texto que contenha esse padrão, sendo que os detalhes como o separador de campos e registros, por padrão espaço e nova-linha, podem ser alterados no começo do programa ou via parâmetros.

```
BEGIN {
  print "FS:", FS;
  print "RS:", RS;
  print "OFS:", OFS;
  print "ORS:", ORS;
}
{
  print "NF:", NF
  print "NR:", NR
  print "$0:", $0;
  print "$1:", $1;
  print "$2:", $2;
  print "$NR:", $NR;
  print "";
}
```

## One line at a time

```
pattern { action }
        { action }
pattern 
```

Se não houver padrão então a ação é executada sempre. Se não houver ação, então sempre que o padrão for encontrado em uma linha o programa imprime a linha. Ou seja, há um valor default para padrão e ação.

```
awk '/test/'
awk '{ print }'
awk '/test/ { print }'
awk '{ if( $0 ~ "test" ) print }'
awk '{ if( $0 ~ "test" ) print $2 }'
```


> 
> Awk maintains a delicate balance between being a line-oriented utility like grep and a full programming language. - Andy Oram
>

## Program

Um Hello World interessante do AWK é imprimir a lista de arquivos e seus proprietários de uma pasta se baseando no comando `ls -l`. Isso demonstra os conceitos básicos da linguagem e ainda é útil.

```
pattern { action }

BEGIN { print "START" }
      { print         }
END   { print "STOP"  }

BEGIN { print "File\tOwner"}
{ print $9, "\t", $3}
END { print " - DONE -" }

ls -l | awk '{ print $9, "\t", $3}'
```

## How to run

O uso do parâmetro `-f` é para conseguirmos rodar programas AWK sem se preocupar se ele é um script. Dessa forma ele funciona como se fosse um bash script:

```
#!/bin/sh
# Linux users have to change $8 to $9
awk '
BEGIN { print "File\tOwner" }
{ print $8, "\t", $3}
END { print " - DONE -" }
'
```

## Tricks

 - Awk tem internamente dois tipos primitivos (fora o array associativo): `float` e `string`.
 - Awk converte "123X" em 0; Nawk converte em 123.
 - A value of 0 is false, while anything else is true.
 - Undefined variables has the value of 0.
 - Unlike AWK, NAWK lets you use booleans as integers.
 - A positional variable is not a special variable, but a function triggered by the dollar sign.
 - All interpreter scripts accept one and only one argument; so, `/bin/awk -f`.

## Research

 - [Análise de DNA]
 - [Análise de performance]

## Reference

 - [Awk in 20 Minutes](https://ferd.ca/awk-in-20-minutes.html), de Fred Hebert.
 - [Tutorial from an Experient Mind](https://www.grymoire.com/Unix/Awk.html), de Bruce Barnett.
 - [Gawk Guide](https://www.gnu.org/software/gawk/manual/gawk.html).


[Análise de DNA]: {{< relref "awk-dna" >}}
[Análise de performance]: {{< relref "awk-performance" >}}
