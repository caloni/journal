---
categories:
- code
date: '2023-04-08T22:57:37-02:00'
draft: true
tags:
- interview
title: 'Onde fica o meio de uma lista ligada?'
---

É simples descobrir o meio de um array: pegue seu tamanho e divida por dois. Agora, para uma lista ligada, mesmo que você saiba qual o índice do meio, não é por meio de índices que acessamos seus elementos, mas por ponteiros.

Nesse caso é necessário dar uma de esperto:

 - Declare dois ponteiros: primeiro e segundo;
 - Ambos são inicializados para o início da lista;
 - Faça um loop que percorra a lista até o final;
 - A cada iteração incremente o primeiro ponteiro em um nodes;
 - A cada iteração incremente o segundo ponteiro em dois nodes;
 - Quando o primeiro ponteiro atingir o final o segundo estará no meio.

O princípio de contador de lista ligadas é a contagem de iterações do começo até o final da lista, mas para manter o tracking de uma posição relativa como o meio dessa lista é ncessário manter um segundo contador.

Através dessa mesma lógica você poderia encontrar posições arbitrárias no meio da lista, como o terceiro elemento onde seu valor começa com a letra 'p', etc.

Abaixo um código em C++ para não ficar tão abstrato:

```
shared_ptr<LinkedList> LinkedListTraverseToTheMiddle(shared_ptr<LinkedList>& head)
{
    shared_ptr<LinkedList> next = head;
    shared_ptr<LinkedList> nextDouble = head;
    bool secondNext = true;

    while (nextDouble->next)
    {
        if (secondNext)
        {
            next = next->next;
        }
        nextDouble = nextDouble->next;
        secondNext = !secondNext;
    }

    return next;
}
```

