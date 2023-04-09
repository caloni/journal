---
categories:
- code
date: '2023-04-08T23:03:44-02:00'
draft: true
tags:
- interview
title: Como inverter uma lista ligada
---

Inverter uma string ou qualquer array em geral é muito simples se for pensar: itere do começo ao fim e do fim ao começo trocando as posições dos primeiros elementos com os últimos. Caminhe até a metade. Fim.

No entanto, para uma lista ligada a coisa não é tão intuitiva assim. É necessário um certo manejo e um certo entendimento de como uma lista é estruturada durante a troca de ponteiros.

O passo a passo parece simples:

- Declare três nodes: anterior, atual e seguinte;
- Enquanto no node atual o node anterior será nulo;
- Deixe o próximo do atual ser o anterior para inverter a lista;
- Em cada iteração do loop os nodes atual e anterior são incrementados por 1.

Esse desafio tem seus truques. O importante na lógica abaixo é atravessar a lista mantendo o tracking dos elementos seguinte e anterior. Tendo o elemento atrual, anterior e próximo a troca de posições se torna simples, mas não tão simples quanto você deve estar imaginando porque:

- o node seguinte se torna o próximo do atual;
- o próximo do atual se torna o anterior (aqui é a invertida);
- o anterior se torna o atual (aqui é o passado);
- o atual se torna o próximo;
- continue até que o antigo final da lista se torne o novo head, apontando para o último anterior.

```
beginning
  next ->  next ->  next ->  next ->  null
 *pres ->  next ->  next ->  next ->  null // present = head->next

 first iteration
  pres -> *foll ->  next ->  next ->  null                   // 1
  pres -> *null     foll ->  next ->  next ->  null          // 2
 *prec ->  null     foll ->  next ->  next ->  null          // 3
  prec ->  null    *pres ->  next ->  next ->  null          // 4

 second iteration
  prec ->  null     pres -> *foll ->  next ->  null          // 1
  prec ->  null     pres -> *prec     foll ->  next -> null  // 2
 *pres ->  prec ->  null     foll ->  next ->  null          // 3
  prec ->  next ->  null    *pres ->  next ->  null          // 4

 third iteration
  prec ->  next ->  null     pres -> *foll ->  null          // 1
  prec ->  next ->  null     pres -> *prec     foll -> null  // 2
 *pres ->  prec ->  next ->  null     foll ->  null          // 3
  prec ->  next ->  next ->  null    *pres ->  null          // 4

end
 *head ->  prec ->  next ->  next ->  null  // head->next = preceding;
```

Os passos numerados estão de acordo com o código C++ abaixo:

```
shared_ptr<LinkedList> LinkedListReverse(shared_ptr<LinkedList> head)
{
    shared_ptr<LinkedList> present = head->next;
    shared_ptr<LinkedList> preceding = nullptr;
    shared_ptr<LinkedList> following;

    while (present != nullptr)
    {
        following = present->next; // 1
        present->next = preceding; // 2
        preceding = present;       // 3
        present = following;       // 4
    }

    shared_ptr<LinkedList> rhead = make_shared<LinkedList>();
    rhead->value = 0;
    rhead->next = preceding;
    return rhead;
}
```

