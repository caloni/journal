---
categories: [ "code" ]
date: "2020-04-22"
tags: [ "draft", "ccpp" ]
title: "Cast Operator"
---
O código abaixo não é C++ moderno. É 98. Porém, ele já demonstra alguns problemas na linguagem que foram aumentados desde então. Não se sabe exatamente qual a tradução semântica de construções tão parecidas quanto o operador-função e o operador-cast. Enquanto o primeiro serve para transformar objetos em funções chamáveis o segundo serve para extrair tipos de maneira educada.

    struct T 
    {
      explicit operator uint64_t()
      {
        return 10;
      }
    };
    
    int main()
    {
      T t;
      t(); // error: term does not evaluate to a function taking 0 arguments
      uint64_t i = (uint64_t) t; // i == 10
    }

O operador de cast só funciona se um cast estiver envolvido. Caso ele seja um método com explicit o cast precisa ser explícito como no exemplo. Se ele não fosse bastaria uma atribuição normal.

      uint64_t i = t;

Ele não pode simplesmente ser chamado como um operador-função. Até porque podem haver vários deles. Enquanto o operador de função trabalha com overload nos parâmetros o operador de cast trabalha com o retorno. Uma vez eu fiz uma brincadeira que meu amigo Fernando tinha me pedido: como fazer sobrecarga de função pelo retorno. Acredito que o exemplo desse post antigo possa exemplificar melhor o que quero dizer.

Já a diferença sintática e semântica dos operadores de função e cast é sutil, quase inexistente. Como muitas coisas em C++ moderno:

      operator uint64_t(); // cast-operator
      uint64_t operator (); // function-operator

