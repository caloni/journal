---
categories:
- code
date: '2022-06-25T20:32:56-03:00'
tags:
- draft
title: Awk é seu próximo passo na produtividade do dia-a-dia
---

# Introduction

Você aprendeu expressões regulares, grep e sed, e acredita já conseguir lidar com análise de logs e código com bastante velocidade, certo? Pois bem, este post contém um ou dois truques com o uso de outra ferramenta chamada awk que irá impulsionar ainda mais sua análise de texto.

 - Casos de sucesso: leitura de DNA
 - Uso básico e um tutorial recomendado
 - Funcionamento interno e performance
 - Programação, depuração e profiling

# [DNA and AWK](https://livefreeordichotomize.com/2019/06/04/using_awk_and_r_to_parse_25tb/)

![](/img/dna-chips.png)

```
select * from intensityData limit 10;
select * from intensityData where snp = 'rs123456';
```

> Eight minutes and 4+ terabytes of data queried later I had my results. Athena charges you by data searched at the reasonable rate of $5 per TB. So this single query cost $20 and eight minutes. If we ever wanted to run a model over all the data we better be ready to wait roughly 38 years and pay $50 million. Clearly this wasn’t going to work.

Lessons Learned:
 - There's no cheap way to parse 25tb of data at once.
 - Sorting is hard, especially when data is distributed.
 - Never, ever, try and make 2.5 million partitions. (cost: $1k+ USD)
 - Sometimes bespoke data needs bespoke solutions.
 - Don't sleep on the basics. Someone probably solved your problem in the 80s.
 - [gnu parallel](https://www.gnu.org/software/parallel/) is magic and everyone should use it.
 - Associative arrays in AWK are super powerful.


## Don't sleep on the basics

```
yp1234,577,1,3
yp5678,577,3,5
yp9012,132,8,9

# This will create the two files 577.csv and 132.csv in your current directory.
awk -F, '{ print > $2 ".csv" }' file.csv
```


## gnu parallel

```
parallel --block 100M --pipe  \
        "awk -F '\t' '{print \$1\",...\"$30\">\"chunked/{#}_chr\"\$15\".csv\"}'"
```


# [Back to the basics](https://www.grymoire.com/Unix/Awk.html)

> 
> Awk also maintains a delicate balance between being a line-oriented utility like grep and a full programming language. - Andy Oram
>

```
pattern { action }

BEGIN { print "START" }
      { print         }
END   { print "STOP"  }

BEGIN { print "File\tOwner"}
{ print $8, "\t", $3}
END { print " - DONE -" }
```

## Notas

 - Awk tem intermamente dois tipos primitivos (fora o array associativo): `float` e `string`.
 - Awk converte "123X" em 0; Nawk converte em 123.
 - A value of 0 is false, while anything else is true.
 - Undefined variables has the value of 0.
 - Unlike AWK, NAWK lets you use booleans as integers.
 - A positional variable is not a special variable, but a function triggered by the dollar sign.
 - All interpreter scripts accept one and only one argument; so, `/bin/awk -f`.

```
# do the same thing: print the first field on the line.
print $1;
X=1;
print $X;
```

If you had four fields, and wanted to print out the second and fourth field, there are two ways. This is the first:

```
#!/bin/awk -f
{
  $1="";
  $3="";
  print;
}
```

and the second

```
#!/bin/awk -f
{
  print $2, $4;
}
```

Similar, but not identical. The number of spaces between the values vary. There are two reasons for this:

 - The actual number of fields does not change.
 - There is a field separator that specifies what character to put between the fields on output.

Other cool stuff:

 - If you change the field separator before you read the line, the change affects what you read.
 - If you change it after you read the line, it will not redefine the variables.

Trick to re-evaluate the fields: `$0=$0`


# [An Stack Overflow Issue](https://stackoverflow.com/questions/43513975/awk-gawk-performance)

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

What about [mawk](https://invisible-island.net/mawk/)?


# [Code](https://www.gnu.org/software/gawk/manual/gawk.html), [debug](https://www.gnu.org/software/gawk/manual/html_node/Debugging.html) and [profile](https://www.gnu.org/software/gawk/manual/gawk.html#Profiling)


# Reference

 - [Gawk Guide](https://www.gnu.org/software/gawk/manual/gawk.html)
 - [Tutorial from an Experient Mind](https://www.grymoire.com/Unix/Awk.html)
 - [Awk Performance](https://stackoverflow.com/questions/43513975/awk-gawk-performance)
 - [Mawk](https://invisible-island.net/mawk/)
 - [DNA](https://livefreeordichotomize.com/2019/06/04/using_awk_and_r_to_parse_25tb/)
 - [Gnu Parallel](https://www.gnu.org/software/parallel/)

