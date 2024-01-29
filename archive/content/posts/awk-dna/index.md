---
categories:
- coding
date: '2022-06-25T20:32:56-03:00'
link: https://livefreeordichotomize.com/2019/06/04/using_awk_and_r_to_parse_25tb/
tags: null
title: AWK and DNA
---

{{< image src="dna-chips.png" caption="" >}}

Pesquisando sobre otimização de AWK eu encontro este post em que uma pessoa tenta tornar a análise de sequências de DNA na nuvem algo financeiramente e computacionalmente viável. Ela começa tentando o óbvio, usando SQL nas próprias estruturas de CSV hospedadas na S3, passa por tentativas de particionar os blocos, por entender como paralelizar a operação e termina em uma solução que usa AWK junto de GNU Parallel, redirecionando a saída para que a linguagem R processe.

```
select * from intensityData limit 10;
select * from intensityData where snp = 'rs123456';
```

>
> Eight minutes and 4+ terabytes of data queried later I had my results. Athena charges you by data searched at the reasonable rate of $5 per TB. So this single query cost $20 and eight minutes. If we ever wanted to run a model over all the data we better be ready to wait roughly 38 years and pay $50 million. Clearly this wasn’t going to work.
>

## Lessons Learned

 - There's no cheap way to parse 25tb of data at once.
 - Sorting is hard, especially when data is distributed.
 - Never, ever, try and make 2.5 million partitions. (cost: $1k+ USD)
 - Sometimes bespoke data needs bespoke solutions.
 - Don't sleep on the basics. Someone probably solved your problem in the 80s.
 - [gnu parallel](https://www.gnu.org/software/parallel/) is magic and everyone should use it.
 - Associative arrays in AWK are super powerful.

```
yp1234,577,1,3
yp5678,577,3,5
yp9012,132,8,9

# This will create the two files 577.csv and 132.csv in your current directory.
awk -F, '{ print > $2 ".csv" }' file.csv

parallel --block 100M --pipe  \
        "awk -F '\t' '{print \$1\",...\"$30\">\"chunked/{#}_chr\"\$15\".csv\"}'"
```