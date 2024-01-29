---
author: admin
comments: true
date: 2014-06-24 12:00:42+00:00
layout: post
slug: se-iterando-com-os-pseudo-ponteiros-uma-breve-introducao
title: 'Se iterando com os pseudo-ponteiros: uma breve introdução'
wordpress_id: 1971
categories:
- C++
tags:
- abstrações
- c++
- introdução
- iteradores
- linguagem
- ponteiros
- stl
---

Como [já vimos algumas vezes](http://www.caloni.com.br/blog/remove_if-ate-remove-so-que-diferente), a STL não prima por interfaces intuitivas, preferindo abstrações que criem um leque de ferramentas genéricas para tratamento uniformizado de coleções de dados através de algoritmos. O problema disso é que novos programadores da linguagem terão que aprender uma maneira nova de lidar com problemas baseada na percepção dos criadores do padrão na época em que foi lançado de como seria a evolução da ciência da computação nos próximos anos. Muitos dos conceitos ali abordados realmente se tornaram padrão _de facto_, mas na briga pela expansão da linguagem quem perdeu por muito tempo foi o próprio desenvolvedor, que teve que se contentar com uma lista de algoritmos genéricos parcialmente compilado.





Dito isto, a abstração dos iteradores é a coisa mais linda do mundo.





[![iteradores-como-ponteiros-a-imagem](https://farm6.staticflickr.com/5500/14307157649_516dd123ba_z.jpg)](https://www.flickr.com/photos/120157483@N04/14307157649/)





## Iteradeiros ou Ponteradores?





Os dois únicos conceitos que é preciso se lembrar para sempre quando se trata de iteradores é que ele:







  * Um iterador se comporta como um ponteiro opaco.


  * O final de um contêiner está sempre um elemento além do último.





Um ponteiro pode ser iterador, mas não o contrário!





Tudo que um ponteiro faz de útil em C/C++ foi emprestado para a STL usar em seus contêiners, e tudo em que ele é prejudicial tentou ficar de fora. E o que um ponteiro faz de útil?







  * Um ponteiro pode apontar para elementos sem conhecermos sua posição.


  * Podemos incrementar ou decrementar ponteiros para caminhar em listas.


  * Dois ponteiros podem ser subtraídos para sabermos a distância entre dois elementos.





```cpp

template<typename T>
T VaiSomandoVaiSomando(T begin, T end)
{
    T current = begin;
    T next = begin + 1;

    while( next != end )
    {
        *next = *current + *next;
        ++current;
        ++next;
    }

    return current;
}

#include <vector>
#include <iostream>

int main()
{
    int aInts[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    std::vector<int> vInts;
    for( int i = 0; i < 10; ++i )
        vInts.push_back(i);

    std::cout << *VaiSomandoVaiSomando(&aInts[0], &aInts[10]) << std::endl;
    std::cout << *VaiSomandoVaiSomando(vInts.begin(), vInts.end()) << std::endl;
}


```






[![iteradores-como-ponteiros](https://farm4.staticflickr.com/3853/14492489942_716878c004_z.jpg)](https://www.flickr.com/photos/120157483@N04/14492489942/)





Da mesma forma, operações como cópia, movimentação, ordenação, caotização, pode ser feito usando dois ponteiros/iteradores de dois contêiners distintos, desde que algumas regras básicas sejam seguidas, como _um iterador deve sempre apontar para algo válido_ (ou disponibilizar alguma abstração que insira novos elementos em um contêiner menor). Veremos essas regras em um próximo post sobre o tema. Apontarei para ele _aqui_.



