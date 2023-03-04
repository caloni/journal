---
categories:
 - code
date: '2023-03-04'
tags: null
title: Combinação
---

Tentando agilizar um dos desafios de programação que tenho treinado encontro como fazer combinação em C++ ou C de maneira mais rápida que a fórmula oficial que requer calcular o fatorial e mais algumas contas. O nome desse problema é NCK, de N Chooses K, onde N é o número de elementos e K a amostragem obtida para a combinação. O resultado é instantâneo, pois se trata de um algoritmo O(n).

No entanto, apliquei a mudança no código sem entender exatamente o que estava acontecendo. Para isso decidi perguntar para o ChatGPT.

## Wanderley Caloni

Olá. Me explica por que o código abaixo que implementa em C o algoritmo NCK funciona?

```
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
```

## ChatGPT

Olá!

Este código implementa o algoritmo de combinação NCK, que calcula o número de maneiras diferentes de escolher k elementos a partir de um conjunto de n elementos. Ele funciona de forma eficiente usando a fórmula matemática de combinação:

```
nCk = n! / (k! * (n-k)!)
```

Onde n! representa o fatorial de n (o produto de todos os números inteiros positivos de 1 a n).

O código começa verificando se k é maior que n. Se for, retorna 0, já que não há maneira de escolher k elementos de um conjunto de tamanho menor que k. Em seguida, verifica se k é maior que a metade de n. Se for, redefine k para n-k. Isso é feito porque, por exemplo, 6C4 é igual a 6C2, então, ao calcular a combinação, é mais eficiente calcular a menor das duas.

Se k for igual a 0, o código retorna 1, pois há apenas uma maneira de escolher 0 elementos de um conjunto (não escolher nenhum).

O código então inicializa um resultado como n e executa um loop de 2 a k. Em cada iteração do loop, ele multiplica o resultado pelo próximo número de n-i+1 e divide pelo valor de i. Isso é feito para calcular o valor de nCk com a fórmula acima.

No final do loop, o código retorna o resultado.

Espero ter ajudado!

## Wanderley Caloni

Vamos por partes. Poderia me explicar por que ocorre que 6C4 e 6C2 tenham o mesmo resultado?

## ChatGPT

Claro, vamos lá!

A notação "nCk" é usada para representar o número de combinações de k elementos que podem ser escolhidos de um conjunto de n elementos. A ordem em que os elementos são escolhidos não importa.

A fórmula geral para nCk é:

```
nCk = n! / (k! * (n-k)!)
```

Agora, vamos ver por que 6C4 e 6C2 têm o mesmo resultado.

Para calcular 6C4, usamos a fórmula acima e substituímos n por 6 e k por 4:

```
6C4 = 6! / (4! * (6-4)!) = 6! / (4! * 2!) = (6 * 5 * 4 * 3 * 2 * 1) / ((4 * 3 * 2 * 1) * (2 * 1)) = 15
```

Para calcular 6C2, usamos a mesma fórmula e substituímos n por 6 e k por 2:

```
6C2 = 6! / (2! * (6-2)!) = 6! / (2! * 4!) = (6 * 5 * 4 * 3 * 2 * 1) / ((2 * 1) * (4 * 3 * 2 * 1)) = 15
```

Como podemos ver, o resultado é o mesmo para ambas as combinações, que é 15. Isso ocorre porque escolher 4 elementos de um conjunto de 6 é o mesmo que não escolher 2 elementos. Em outras palavras, se temos 6 elementos e escolhemos 4, então, os outros 2 elementos não escolhidos são automaticamente determinados. Portanto, as duas combinações são iguais.

Espero ter ajudado!

## Wanderley Caloni

Certo. Nesse caso qualquer NCK é igual a outro desde que N1 seja igual a N2 e K1 seja o complemento de K2 referente a N?

## ChatGPT

Sim, você está correto! Isso é conhecido como a propriedade de complementaridade de combinações.

A propriedade de complementaridade de combinações afirma que, se escolhermos k elementos de um conjunto de n elementos, então o número de maneiras de escolher os k elementos é o mesmo que o número de maneiras de escolher os n-k elementos restantes.

Isso pode ser visto usando a fórmula de combinação:

```
nCk = n! / (k! * (n-k)!)
```

Se escolhermos k elementos, então o número de maneiras de fazer isso é nCk. Mas se escolhermos os n-k elementos restantes, então o número de maneiras de fazer isso é (n choose n-k), ou seja:

```
nC(n-k) = n! / ((n-k)! * k!)
```

Observe que a única diferença entre as duas fórmulas é que k e (n-k) foram trocados. Isso significa que nCk e nC(n-k) são iguais!

Por exemplo, se quisermos encontrar 5C3, podemos aplicar a propriedade de complementaridade e dizer que 5C3 é o mesmo que 5C2, já que 2 é o complemento de 3 em relação a 5. Portanto:

```
5C3 = 5C2 = 10
```

Espero ter esclarecido!

## Wanderley Caloni

Esclareceu de maneira impecável. O segundo ponto que gostaria de entender em sua explicação inicial é a parte do código que inicializa o resultado como n e executa um loop de 2 a k. Por que esses valores são utilizados para a obtenção do resultado?

## ChatGPT

Bom, o loop de 2 a k no código é usado para calcular o resultado da combinação, aplicando a fórmula:

```
nCk = n! / (k! * (n-k)!)
```

No loop, a variável result é inicializada com o valor de n, que representa o número de elementos no conjunto original. Em seguida, o loop itera de i=2 até i=k, que é o número de elementos que queremos escolher.

Dentro do loop, o resultado é atualizado multiplicando o result pelo próximo elemento do conjunto n-i+1 e dividindo pelo i. A fórmula exata usada para atualizar o resultado é:

```
result *= (n-i+1);
result /= i;
```

Essa atualização é feita para cada elemento escolhido, para garantir que todas as combinações possíveis sejam consideradas. No final do loop, result conterá o valor final da combinação de n elementos escolhidos k.

A inicialização de result com o valor de n é feita porque cada elemento escolhido para a combinação pode ser selecionado apenas uma vez. Então, no início, temos n opções para escolher o primeiro elemento. Depois de selecionar o primeiro elemento, restam n-1 opções para escolher o segundo elemento, n-2 opções para escolher o terceiro elemento, e assim por diante. Multiplicando esses valores, obtemos o fatorial de n até o n-k+1.

No entanto, como a ordem dos elementos escolhidos não importa em uma combinação, precisamos dividir pelo fatorial de k no final para eliminar as permutações redundantes, resultando na fórmula:

```
nCk = n! / (k! * (n-k)!)
```

Espero que isso esclareça como a inicialização de result com o valor de n e o loop de 2 a k são usados para calcular o resultado da combinação.

## Wanderley Caloni

Ficou um pouco mais claro. No fundo este algoritmo é uma maneira inteligente de aproveitar os recursos computacionais para se chegar a um resultado sem precisar recalcular os inúmeros fatoriais como na fórmula original. Você teria alguma maneira de otimizar este algoritmo ainda mais?


## ChatGPT

Sim, você está correto novamente! O algoritmo NCK apresentado é uma forma mais eficiente de calcular combinações sem a necessidade de calcular fatoriais.

Uma otimização adicional que pode ser feita é a redução do número de divisões realizadas no loop. Em vez de dividir o resultado por i a cada iteração, podemos acumular o produto dos i's e dividir pelo fatorial de k no final.

Dessa forma, o loop seria atualizado para:

```
int result = n;
int i;
for (i = 2; i <= k; ++i) {
    result *= (n - i + 1);
    product *= i;
}
return result / product;
```

Essa otimização reduz o número de divisões realizadas no loop de k para apenas uma, na linha final. Isso pode ajudar a melhorar a eficiência do algoritmo, especialmente para valores maiores de n e k.

Além disso, pode ser útil implementar uma verificação adicional para o caso em que k é muito pequeno em relação a n, já que nesse caso pode ser mais rápido usar a fórmula de combinação direta, em vez de executar o loop. Por exemplo, para valores de k abaixo de 5 ou 6, pode ser mais rápido simplesmente calcular a combinação diretamente usando a fórmula de combinação, em vez de executar o loop.

Espero que isso ajude!

## Wanderley Caloni

Muito interessante sua sugestão de otimização. Acredito que isso tenha relação com a maneira com que o computador executa divisões, sendo mais rápido aplicar k-2 multiplicações e apenas uma divisão do que k-2 divisões, certo?

## ChatGPT

Sim, exatamente! A divisão é geralmente mais cara do que a multiplicação para a maioria das CPUs modernas. Portanto, minimizar o número de divisões pode ajudar a melhorar o desempenho do algoritmo. No caso do algoritmo NCK, reduzir o número de divisões pode ser particularmente importante porque a divisão é executada a cada iteração do loop, o que pode ser caro para valores grandes de k.

A otimização que propus, que usa um produto acumulado em vez de divisões repetidas, ajuda a reduzir significativamente o número de divisões necessárias para calcular a combinação, tornando o algoritmo mais eficiente em termos de tempo de execução.

## Wanderley Caloni

Certo. Por enquanto é só. Obrigado pelas suas esclarecedoras explicações.

## ChatGPT

De nada! Fico feliz em ter ajudado. Se tiver mais alguma dúvida, é só perguntar.

