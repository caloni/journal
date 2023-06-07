---
categories:
- coding
date: '2022-07-12T21:02:54-03:00'
tags: null
title: Como converter qualquer projeto antigo do Visual Studio em CMake
---

Aproveita esse pedaço abaixo de `CMakeLists.txt` como colinha e cria na pasta do seu projeto.

```
cmake_minimum_required(VERSION 3.16)
project (meu_projeto)
add_executable(meu_projeto
```

Lista os arquivos de extensão `.cpp` e joga a listagem no final do `CMakeLists.txt`. Por fim adiciona o fecha-parênteses que está faltando.

```
dir /b *.cpp >> CMakeLists.txt
echo ) >> CMakeLists.txt
```

Pronto. Se seu projeto não tem dependências externas é só compilar.

```
mkdir build && cd build
cmake ..
cmake --build .
```

Os erros mais comuns a partir daí são erros de inclusão e linkedição. Para os erros de inclusão use comandos no `CMakeLists.txt` como `target_include_directories`. Para os erros de linkedição use os comandos `target_link_libraries` para incluir as libs e `target_link_directories` para dizer que em pasta estão. Você deve encontrar essas informações no projeto original do Visual Studio.

Em geral é melhor seguir esse passo-a-passo de como gerar um projeto limpo em vez de tentar converter. Se houver algum truque na solução Visual Studio ela deverá ser portada de maneira mais documentada e pronta para portabilidade. Com o CMake você não terá mais que se preocupar com qual versão do Visual Studio irá trabalhar, e quando migrar basta instalar ou apontar a versão correta (direto no comando cmake).

A tempo: trate os arquivos da solução do Visual Studio gerados pelo cmake como temporários. Não use controle de fonte em cima deles. Isso o ajudará a manter a base do seu projeto apenas nos arquivos `CMakeLists.txt`.