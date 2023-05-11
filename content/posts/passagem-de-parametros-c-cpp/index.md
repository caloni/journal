---
categories:
- code
date: '2022-11-20T16:10:12-03:00'
link: https://youtu.be/i9Focx4UXnc
tags:
- ccpp
title: Passagem de Parâmetros C vs C++
---

Em C não é necessário declarar os parâmetros que uma função recebe. Isso é implícito. A linguagem C foi criada para ser um amontoado de assembly descrito com um pouco de syntax sugar e juntado pelo linker através de uma tabela de nomes. O que importa são os nomes. Vamos exemplificar:

```
int main()
{
  int c = func(10, 20);
  return c;
}

func(int a, int b)
{
  return a + b;
}
```

Viu? Você nem precisa conhecer o nome na hora de chamar a função no main. Duvida? Veja o vídeo desse post. Se você colocar cada função em um arquivo separado mesmo assim o compilador compila e o linker acha tudo e tudo funciona.

Isso pode gerar alguns problemas em tempo de execução, já que o linker, assim como o compilador, confia cegamente no programador. Isso acontece porque antigamente os programadores eram bons.

```
int main()
{
  int c = func(10);
  return c;
}

func(int a, int b)
{
  return a + b;
}
```

Para que programadores que não são bons conseguissem programar em código nativo algumas facilidades surgiram na linguagem C++, como a declaração de função sendo levada a sério. No padrão C isso também foi levado em conta. Se você possui uma declaração de função o compilador irá checar os argumentos.

```
func(int a, int b);

int main()
{
  int c = func(10);
  return c;
}

func(int a, int b)
{
  return a + b;
}

error C2198: 'func': too few arguments for call
```

Em C++ com o mesmo código:

```
error C4430: missing type specifier - int assumed
error C2660: 'func': function does not take 1 arguments
error C4430: missing type specifier
```

Aí vem a questão de como declarar uma função em C++ que não recebe nada. Isso é uma boa prática, mas não obrigatório.

```
int func();

int main()
{
  int c = func();
  return c;
}

int func()
{
  return 23;
}
```

O código acima compila normalmente. E roda. Mas se você por algum motivo está com projeto que tem C arcaico misturado com C++ e precisa definir melhor como as funções se comportam faz assim:

```
int func(void);
```

Isso vai funcionar em C e em C++. Em C, se você usar essa declaração e mesmo assim passar argumentos ele irá dar um warning:

```
warning C4087: 'func': declared with 'void' parameter list
```

Mas vai deixar rodar. Em C++ não tem jeito. Em C++ ele assume que você não é um bom programador.