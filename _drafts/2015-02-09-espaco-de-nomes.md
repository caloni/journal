---
title: Espaço de nomes
---
Antigamente, lá na linguagem C, todas as funções definidas dentro de um .c e todas as funções declaradas dentro dos .h incluídos existiam globalmente. Ou seja, se calhasse de uma das funções de alguma lib externa ter o mesmo nome de uma outra função de uma outra lib externa essas duas funções dariam um conflito razoável. Porém, um detalhe interessante é que esse conflito poderia acontecer de diferentes maneiras. Vamos analisar alguns mais comuns:

## Conflito #1: Exatamente a mesma assinatura de função (linguagem C)

```cpp
// lib1.h
void PrettyOriginalFunctionName();

// lib2.h
void PrettyOriginalFunctionName();

// lib1.c
#include "lib1.h"

void PrettyOriginalFunctionName()
{
	// do sth
}

// lib2.c
#include "lib2.h"

void PrettyOriginalFunctionName()
{
	// do sth
}


#include "lib1.h"
#include "lib2.h"

int main()
{
	PrettyOriginalFunctionName();
}
```

```
1>------ Build started: Project: Namespaces, Configuration: Debug Win32 ------
1>  NoNamespaces.c
1>  lib2.c
1>  lib1.c
1>  Generating Code...
1>lib2.obj : error LNK2005: _PrettyOriginalFunctionName already defined in lib1.obj
1>C:\Samples\Namespaces\Debug\Namespaces.exe : fatal error LNK1169: one or more multiply defined symbols found
========== Build: 0 succeeded, 1 failed, 0 up-to-date, 0 skipped ==========
```

## Conflito #2: Exatamente a mesma assinatura de função (linguagem C)


A desvantagem de nunca usar namespaces é que nunca saberemos quando o próximo conflito de nomes pode ocorrer. Da mesma forma com que macros são uma má prática em C++, deixar tudo global só funciona bem em projetos pequenos e médios. O bom de C++ é que você pode usar as suas tralhas quando elas parecerem necessárias -- como POO -- e não simplesmente porque esse é o jeito de programar na linguagem (Java).

E quando namespaces são necessários? Resumidamente, quando seu projeto interagir com o mundo de fora. Ou seja, LIBs, APIs, frameworks. Boost, Poco e companhia usam. Se quiser, pode começar a usar desde já. Será uma tralha inútil até algumas centenas de linhas de código. Ou você pode adaptar projetos quando estes ficarem grandes e conhecidos (como Poco e Boost). Como tudo em programação, um pouco de bom gosto e um pouco de senso comum é o que planeja os próximos refactories.

Caso você queira adaptar um projeto existente e não quiser mexer muito em código alheio que o utiliza pode usar a seguinte estratégia:

namespace MyNamespace
{
void MyFunc();
}
using MyNamespace;

Claro que isso invalida o uso do namespace, colocando todos os nomes visíveis no namespace global, embora agora eles possam ser chamados com o prefixo MyNamespace::. No entanto, se houver alguma ambiguidade no caminho agora é fácil de resolver, especificando MyNamespace::MyFunc. Se ainda assim você preferir ir trocando aos poucos todas as referências a MyFunc pelo seu "endereço completo", pode criar um header com o novo namespace e o header original incluir a diretiva using. Dessa forma os fontes velhos continuam funcionando, mas os fontes novos são colocados à prova de namespaces incluindo o header sem using.

//MyNewHeader.h
namespace MyNamespace
{
void MyFunc();
}

//MyHeader.h
#include "MyNewHeader.h"
using MyNamespace;

Eu acharia desnecessário citar que namespaces podem ser aninhados caso o primeiro livro que li sobre isso tivesse esquecido de citar esse detalhe e eu ter vivido alguns meses sem saber de todo esse poder. Sim, você pode colocar diversos namespaces dentro de outros no melhor estilo .NET:

namespace System
{
namespace Out
{
namespace Console
{
void PrintLn(char*, ...);
}
}
}

Isso foi útil para a Microsoft inserir as novidades do C++ temporariamente em um namespace chamado tr1. Depois esses nomes foram aos poucos trazidos para o namespace padrão da STL, o std. O shared_pointer aconteceu assim. Você pode usar a mesma estratégia para funções/classes em versões mais novas caso ambas precisem coexistir em determinado momento.

Uma vez que é usado a diretiva using para um namespace tudo que está lá passa a ser visível no escopo atual sem necessidade de especificar o caminho. Para evitar trazer toda a galera junta é possível especificar nome a nome, semelhante ao comando import do Python.

using std::setprecision
