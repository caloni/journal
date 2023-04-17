---
categories:
- code
date: '2023-04-11T17:30:32-02:00'
draft: true
link: https://youtu.be/Ic7OO3Uw6J0
tags:
- interview
title: Árvore de segmentos
---

Não existe sequer uma entrada em português na Wikipedia sobre Segment Tree, uma árvore binária específica para guardar intervalos. E este acredito ser um assunto importante para testes de entrevista ou competições de programação porque ele é muito útil para alguns problemas. Vamos dar uma olhada em como ela funciona.

Em primeiro lugar, ela é uma árvore binária. No entanto, seus ramos representam intervalos. A raiz possui o intervalo inteiro (mínimo e máximo) e os ramos vão se dividindo em intervalos menores, até que as folhas indiquem apenas um elemento.

É importante notar que uma árvore de segmento é maior que simplesmente um array, mas diferente de um array, a árvore brilha quando precisamos somar intervalos.  Como ela está estruturada de maneira que cada ramo contém a soma de seus galhos, para obter a maioria dos intervalos sua complexidade desce de O(N) para O(log N).

{{< image src="segment_tree.png" >}}

Dessa forma, podemos concluir que o espaço ocupado por uma árvore binária para implementar um segment tree completo deve ocupar por volta de `2*N-1`, o espaço para implementar uma árvore binária completa com N folhas.

No entanto, o tamanho para estocar uma segment tree não é esse, mas tipicamente `4*N`. O motivo disso é que nós precisamos de `2*next_power_of_two(N)-1` para garantir que as divisões da árvore todas vão estar representadas, mas como custa processamento descobrir qual a próxima potência de 2 que é maior que N uma aproximação válida é usar `4*N`.

Vamos observar a implementação de uma árvore de segmentos. A primeira coisa é
alocar o espaço necessário em um vetor. Digamos que nossa árvore irá conter os
intervalos de 1 a 1000 (inclusive).

``` vector<int> tree[4*1000]; ```

Pronto, nossa árvore está pronta. =)

Vamos atualizar algum valor nela. Por exemplo, definir o valor 42 para o node
666:

``` void update(int node, int left, int right, int pos, int value, vector<int>&
tree) { if (left == right) { tree[node] = value; } else { int nodeLeft = 2 *
node; int nodeRight = 2 * node + 1; int middle = (left + right) / 2; if (pos <=
middle) update(nodeLeft, left, middle, pos, value, tree); else
update(nodeRight, middle+1, right, pos, value, tree); tree[node] =
tree[nodeLeft] + tree[nodeRight]; } }

int main() { vector<int> tree(4 * 1000); update(1, 1, 999, 666, 42, tree); }
```

Algumas informações relevantes sobre esses parâmetros:

 - node é a localização do ramo atual;
 - left é o início do intervalo que estamos;
 - right é o final do intervalo que estamos;
 - pos é o número do ramo que pretendemos trocar;
 - value é o valor que pretendemos colocar no ramo;
 - tree é a árvore de segmentos.

Todos esses parâmetros existem porque a função update é recursiva e ela precisa
passar a localização dentro do array no formato de um mapa para uma árvore
binária. A busca também segue o mesmo princípio, de O(log N), ou seja, para
encontrar a posição desejada (variável pos) a função irá seguir limitando o
intervalo entre left e right até que ambos tenham o mesmo valor, situação em
que estaremos em uma folha.

Depois da atualização vem a parte interessante: os ramos acima da folha são
atualizados com a soma entre seus ramos esquerdo e direito, recursivamente.
Isso quer dizer que o valor 42 irá ecoar por todos os ramos de cima até chegar
na raiz, que também irá conter 42, já que este é o primeiro valor diferente de
zero de toda a árvore.

Vamos definir mais alguns valores em outras posições para em seguida
implementar a soma:

``` int main() { vector<int> tree(4 * 1000); update(1, 1, 999, 666, 42, tree);
update(1, 1, 999, 600, 58, tree); update(1, 1, 999, 700, 45, tree); update(1,
1, 999, 999, 55, tree); } ```

Com isso a soma dos seguintes intervalos deve contar os seguintes totais:

 - o intervalo [666,666] deve conter o valor 42, da única folha selecionada;
 - o intervalo [600,700] deve conter o valor 145, da soma de 666, 600 e 700;
 - o intervalo [600,999] deve conter o valor 200, adiciona da soma anterior de
   999;
 - a raiz, ou o intervalo [1,999] deve conter o mesmo valor;
 - intervalos abaixo de [1,599] devem conter 0.

Vamos implementar a função de soma e descobrir.

``` int sum(int node, int left, int right, int posLeft, int posRight, const
vector<int>& tree) { if (posLeft > posRight) return 0; if (posLeft == left &&
posRight == right) return tree[node]; int nodeLeft = 2 * node; int nodeRight =
2 * node + 1; int middle = (left + right) / 2; return sum(nodeLeft, left,
middle, posLeft, min(posRight, middle), tree) + sum(nodeRight, middle + 1,
right, max(posLeft, middle + 1), posRight, tree); }

int main() { vector<int> tree(4 * 1000);

    update(1, 1, 999, 666, 42, tree); update(1, 1, 999, 600, 58, tree);
    update(1, 1, 999, 700, 45, tree); update(1, 1, 999, 999, 55, tree);

    vector<vector<int>> intervals = { {666, 666}, {600, 700}, {600, 999}, {1,
    999}, {1, 599} };

    for (const vector<int>& i : intervals) { int isum = sum(1, 1, 999, i[0],
    i[1], tree); cout << "the interval [" << i[0] << "," << i[1] << "] has the
    value " << isum << endl; } } ```

Mais uma vez, existem muitos parâmetros porque a função é recursiva e precisa
se localizar, e o princípio é o mesmo da função update, de usar as variáveis
como um mapas para navegar por um array.

 - node é a localização do ramo atual;
 - left é o início do intervalo que estamos;
 - right é o final do intervalo que estamos;
 - posLeft é o início do intervalo que queremos a soma;
 - posRight é o final do intervalo que queremos a soma;
 - tree é a árvore de segmentos.

Note que a única variável que de fato indexa o array é a variável node. Porém,
qual vai ser o índice de node é determinado pelos cálculos que giram em torno
de ir para a direita ou para a esquerda pela árvore. Se for pela esquerda o
próximo índice é o índice em node vezes 2, pois existem node no nível em que
estamos, e se for pela direita o próximo índice é node vezes 2 mais um, que é o
próximo após o ramo da esquerda.

Se ficou difícil de entender, lembre-se que a busca em uma árvore binária segue
a mesma lógica daquele jogo de adivinhação, em que você chuta um número de X a
Y e a pessoa que sabe qual o número irá dizer se o número é maior ou menor do
que você chutou. Como você é muito esperto irá sempre dividir a faixa de onde
está para acertar o número o mais rápido possível.

Por exemplo, vamos supor que você deve chutar qual número é de 1 a 100. O
número é 64.

 - seu chute inicial é 50;
 - a resposta: mais alto;
 - seu próximo chute é 75 (entre 50 e 100);
 - a resposta: mais baixo;
 - seu próximo chute é 63;
 - a resposta: mais alto;
 - seu próximo chute é 67;
 - mais baixo;
 - 65;
 - mais baixo;
 - 64;
 - acertou!

Entre 100 possíveis chutes foram feitos 6, ou cerca de log 100 chutes.
Exatamente como é feita a busca na árvore binária, seja de segmentos ou não.
Essa é a grande vantagem de usar o mapa para se localizar no array como uma
árvore binária, pois a busca não será linear.

O fato da árvore ser de segmentos é apenas um detalhe que incorre em mantermos
atualizados os nodes com a soma de todos os ramos abaixo, algo custoso a
princípio, mas que na hora de obter a soma de intervalos faz valer a pena.
