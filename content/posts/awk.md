---
categories:
- code
date: '2020-06-07'
tags:
- draft
title: Awk
---

# Introduction

Meu amigo sugeriu que aprender awk poderia ser útil de várias maneiras. Uma delas para organizar finanças pessoais. Dei uma lida em alguns tutoriais, sendo que o melhor custo benefício foi o [Awk in 20 Minutes](https://ferd.ca/awk-in-20-minutes.html), de Fred Hebert (ele é o mesmo autor de um livro sobre erlang). Gostei. É sobre tratamento de texto como sed, mas em uma versão estendida e criada na época com a mesma sintaxe de C.

Agora que vou palestrar sobre no Native Floripa de 2022 eu dei uma olhada em outro tutorial muito bom que eu vou chamar de [Tutorial from an Experient Mind](https://www.grymoire.com/Unix/Awk.html), de Bruce Barnett. Ele vai um pouco além porque é de alguém interessado em contar alguns macetes que dão uma visão mais interna da linguagem. E para referência eu considero o [Gawk Guide](https://www.gnu.org/software/gawk/manual/gawk.html) como um ótimo compilado.

    # comment
    Pattern1 { ACTIONS; }
    
    # comment
    Pattern2 { ACTIONS; }
    
    # comment
    Pattern3 { ACTIONS; }
    
    # comment
    Pattern4 { ACTIONS; }

Por ela ser uma ferramenta antiga, criada por Alfred Aho, Peter Weinberger e Brian Kernighan na década de 70, AWK usa conceitos antigos, como field e record. Ela foi criada para formatar texto em formato de planilha, ou banco de dados. Um field, ou campo, é uma coluna na planilha, e um record, ou registro, é uma linha dessa planilha. Imagine que você pode usar awk para manipular e extrair dados de qualquer texto que contenha esse padrão, sendo que os detalhes como o separador de campos e registros, por padrão espaço e nova-linha, podem ser alterados no começo do programa.

    # can be modified by the user
    BEGIN {

      # Field
      # Separator
      FS = ",";

      # Record
      # Separator (lines)
      RS = "\n";

      # Output
      # Field
      # Separator
      OFS = " ";

      # Output
      # Record
      # Separator
      # (aka lines)
      ORS = "\n";
    }
    # can't be modified by the user
    {
      # Number of Fields
      # in the current Record
      NF

      # Number of Records
      # seen so far
      NR

      # Script Arguments
      ARGV / ARGC
    }

Como toda linguagem usada como ferramenta do dia-a-dia existem os comandos mais úteis:

    # prints $0 
    # (just print will do it)
    { print $0; }  

    # ends the program
    { exit; }      

    # skips to the 
    # next line of input
    { next; }      

    # variable assignment
    { a=$1; b=$0 } 

    # array assignment
    { c[$1] = $2 } 

Existem tópicos mais avançados como funções, seja embutidas ou criadas pelo usuário (inclusive em C), e versões mais novas como nawk e gawk. Assim como existe o vim e existem plugins, sendo que passo muito bem sem usar plugins no meu vim. Acho difícil que um dia precise estender awk, exceto por curiosidade e para aguçar minha criatividade. E como o próprio guia GNU da ferramenta sugere, se seu programa awk atingir algumas centenas de linhas é melhor você refazer em outra linguagem. Python, por exemplo (brincadeira).

    { if (BOOLEAN) { ACTION }
      else if (BOOLEAN) { ACTION }
      else { ACTION }
    }
    { for (i=1; i<x; i++) { ACTION } }
    { for (item in c) { ACTION } }

> 
> Awk also maintains a delicate balance between being a line-oriented utility like grep and a full programming language. - Andy Oram
>

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

# Tricks

 - Awk tem intermamente dois tipos primitivos (fora o array associativo): `float` e `string`.
 - Awk converte "123X" em 0; Nawk converte em 123.
 - A value of 0 is false, while anything else is true.
 - Undefined variables has the value of 0.
 - Unlike AWK, NAWK lets you use booleans as integers.
 - A positional variable is not a special variable, but a function triggered by the dollar sign.
 - All interpreter scripts accept one and only one argument; so, `/bin/awk -f`.

# Reference

 - [Awk in 20 Minutes](https://ferd.ca/awk-in-20-minutes.html), de Fred Hebert.
 - [Tutorial from an Experient Mind](https://www.grymoire.com/Unix/Awk.html), de Bruce Barnett.
 - [Gawk Guide](https://www.gnu.org/software/gawk/manual/gawk.html).
 - [DNA](https://livefreeordichotomize.com/2019/06/04/using_awk_and_r_to_parse_25tb/).
 - [Awk Performance](https://stackoverflow.com/questions/43513975/awk-gawk-performance).
 - [Mawk](https://invisible-island.net/mawk/).
 - [Gnu Parallel](https://www.gnu.org/software/parallel/).

