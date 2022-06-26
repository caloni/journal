---
categories:
- code
date: '2022-06-25T20:32:56-03:00'
link: https://stackoverflow.com/questions/43513975/awk-gawk-performance
tags:
- draft
title: Awk é seu próximo passo na produtividade do dia-a-dia
---

# An Stack Overflow Issue

Este post foi resultado da minha pesquisa sobre AWK e performance. Ele contém alguns insights que acredito serem úteis para otimizar o código AWK. Nem sempre a linguagem é a ferramenta ideal para trabalhar dados, como o rapaz da dúvida mencionou (ele estava tentando parsear XML), mas mesmo assim as respostas focaram na implementação interna do AWK para entender como tornar o processamento massivo de dados factível, ou pelo menos mais rápido. Isso pode ser útil no dia-a-dia.

>
> "The Enlightened Ones say that....
> 
> You should never use C if you can do it with a script;
>
> You should never use a script if you can do it with awk;
>
> Never use awk if you can do it with sed;
>
> Never use sed if you can do it with grep."
>

```
yes 'SomeSampleText SomeOtherText 33 1970 YetAnotherText 777 abc 1 AndSomeMore' | head -12000000 > bigsample.txt
time gawk 'BEGIN {a = 0;} {if ($5 == "YetAnotherText") a ++;} END {print "a: " a;}' bigsample.txt
time gawk 'BEGIN {a = 0;} {if ($0 ~ /YetAnotherText/) a ++;} END {print "a: " a;}' bigsample.txt
time gawk 'BEGIN {a = 0;} /YetAnotherText/ {a ++;} END {print "a: " a;}' bigsample.txt
time gawk 'BEGIN {a = 0;} {if (NF == 9) a ++;} END {print "a: " a;}' bigsample.txt
time gawk 'BEGIN {a = 0;} {if ($1 == "SomeSampleText") a ++;} END {print "a: " a;}' bigsample.txt
time gawk 'BEGIN {a = 0;} {if ($9 == "AndSomeMore") a ++;} END {print "a: " a;}' bigsample.txt
```

```
yes "<SomeSampleText:SomeOtherText=33>1970<YetAnotherText:777=abc>1<AndSomeMore>" | head -12000000 > bigsample.txt
time gawk 'BEGIN {FS = "<|:|=";} {if ($5 == "YetAnotherText") a ++;} END {print "a: " a;}' bigsample.txt
time gawk 'BEGIN {FS = "<|:|=";} {if ($0 ~ /YetAnotherText/) a ++;} END {print "a: " a;}' bigsample.txt
time gawk 'BEGIN {FS = "<|:|=";} /YetAnotherText/ {a ++;} END {print "a: " a;}' bigsample.txt
time gawk 'BEGIN {FS = "<|:|=";} {if (NF == 8) a ++;} END {print "a: " a;}' bigsample.txt
time gawk 'BEGIN {FS = "<|:|=";} {if ($2 == "SomeSampleText") a ++;} END {print "a: " a;}' bigsample.txt
time gawk 'BEGIN {FS = "<|:|=";} {if ($8 == "AndSomeMore>") a ++;} END {print "a: " a;}' bigsample.txt
```

```
time grep -c YetAnotherText bigsample.txt
```

## Conclusions

 - It seems that parsing and splitting into fields is faster when there is one simple delimiter, instead of several delimiters.
 - Usually getting $N is faster than getting $M, where N < M
 - In some cases, searching for /pattern/ in the whole line is faster than comparing $N == "pattern", especially if N is not one of the first fields of the line
 - Getting NF can be slow because the line has to be parsed and fields calculated, and more so if there are several delimiters

```
yes 'a b c d' | head -12000000 > bigsample.txt
time gawk '{if(NF==5)print("a")}' bigsample.txt
time gawk '{if($4=="Hahaha")print("a")}' bigsample.txt
time gawk '{if($1=="Hahaha")print("a")}' bigsample.txt
time gawk '/Hahaha/{if($4=="Hahaha")print("a")}' bigsample.txt
```

```
xml sample 300k times bigger
time gawk 'BEGIN{FS="[<:=]"}NF>=4{a++}END{print a+0}' bigsample.txt
time gawk 'BEGIN{FS="<|:|="}NF>=4{a++}END{print a+0}' bigsample.txt
time gawk 'BEGIN{FS="<|:|="}NF>=4&&/:SubNetwork/{a++}END{print a+0}' bigsample.txt
time gawk 'BEGIN{FS=":SubNetwork"}NF>=2{a++}END{print a+0}' bigsample.txt
time gawk '/:SubNetwork/{a++}END{print a}' bigsample.txt
```

> 
> show that if you use your :SubNetwork as field separator, it's the fastest.
> 

## Mawk

No meio das minhas pesquisas também encontrei este projeto chamado [mawk](https://invisible-island.net/mawk/), que é uma versão otimizada do AWK. Instalei pelos fontes do projeto na minha máquina e fiz os mesmos testes do post do Stack Overflow com resultados absurdamente mais rápidos. Ou seja, vale uma visita.
