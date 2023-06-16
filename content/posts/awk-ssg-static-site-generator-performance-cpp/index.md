---
categories:
 - blogging
 - coding
date: '2023-06-15'
tags: null
title: Usando Awk para gerar sites e medidas de performance
---

Hoje achei um projeto muito genial e óbvio chamado Zodica, que é um static site generator escrito em awk. Mas claro! Gerar sites estaticamente é sobre manipular texto de entrada para texto de saída. Como não pensei nisso antes?

Baixei [o projeto](https://github.com/nuex/zodiac) e comecei a fazer alguns testes de performance. Porém, como ele depende de bash tive que rodar em um WSL (Debian) e o I/O não ficou dos mais rápidos. Estava demorando quase um arquivo por segundo. Isso para meus mais de 4k arquivos iria demorar uma eternidade.

Aproveitei o embalo e criei um projeto bem básico em C++ que apenas abre um arquivo, lê o conteúdo e escreve em outro arquivo (o baseline). Ele demorou 12 segundos no máximo para operar em todos os posts. Bom, mas poderia ser melhor.

```
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;


std::string ReadEntireFile(std::ifstream& in)
{
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

int main()
{
    std::string path = ".";
    for (const auto& entry : fs::directory_iterator(path))
    {
        string path = entry.path().string();
        ifstream ifs(path);
        ofstream ofs(path + ".html");

        if( ifs && ofs )
        {
            string content = ReadEntireFile(ifs);
            ofs << content;
        }
    }
}
```

Fiz um teste usando o próprio awk, mas ainda não sei como fazer para manipular vários arquivos de uma vez e acabei criando uma batch, que acho que deixou a execução desnecessariamente lenta. Volto para isso depois.

```
@echo off
awk -f markdown.awk post.md > post.html
rem 4K lines
```

Por fim, apenas para comparação com a API nativa, executei um comando copy que apenas copia todos os arquivos para uma nova pasta. O resultado: 6 segundos. Mas OK, é apenas uma cópia de apontamento no file system. O comando nem precisaria abrir para leitura. Acho que devo ficar com os 12 segundos como baseline, mesmo, ou cerca de 300 arquivos por segundo.

```
mkdir copia
copy *.md copia > NUL
```
