---
categories:
- coding
date: '2023-02-24T22:36:06-03:00'
title: Não passei em uma entrevista técnica
---

Fui ingênuo em minha primeira entrevista técnica depois de muito tempo sem fazer. Pensei que iam avaliar minha tentativa de resolver problemas, e não para resolver mesmo, naquela meia-hora de call, silêncio e compartilhamento de tela. Na metade da entrevista, depois de explicar para a entrevistadora, uma das arquitetas sênior do lugar onde pretendia trabalhar, como resolvi um [bug no algoritmo blowfish de tempo real asm de um boot de hd] ela me pede pra fazer um teste de programação. O teste consiste em comprimir números repetidos de uma string, algo digno de campeonato de programação.

Até aí tudo bem, fui fazendo. Depois de umas três retentativas de compilar, rodar, depurar no "passou por aqui" (não havia depurador no ambiente web), estava quase pronto, faltando um detalhe recursivo no método pedido. Esse último detalhe que não deu tempo.

O mais curioso é que fiquei com isso na cabeça. Após a entrevista fui ao dentista. Na sala de espera acessei o [replit] pelo celular, refiz o código e consegui resolver. Mas já era tarde. Eu estava na sala de espera do dentista, meia-hora depois da entrevista ter terminado.

```
#include <iostream>

using namespace std;

string compress(string input)
{
    string orInput;
    do
    {
        orInput = input;
        for (size_t i = 0; i < input.size() - 1; ++i)
        {
            if (input[i] == input[i + 1])
            {
                size_t end = i + 2;
                while (end < input.size() && input[end] == input[i])
                {
                    ++end;
                }

                input.erase(i, end - i);
                --i;
            }
        }
    } while (orInput != input);

    return input;
}

int main()
{
    string input = "235554431";
    string output = compress(input);

    cout << "input: " << input << '\n'
        << "output: " << output << endl;
}
```

O objetivo estava cumprido:

```
input: 235554431
output: 21
```

Na noite seguinte, antes de acordar, "sonhei" com uma solução mais elegante. Acho que me lembro. A ideia-chave era apenas reiniciar do começo da string após cada compressão.

```
#include <iostream>

using namespace std;

string compress(string input)
{
    for (size_t i = 0; i < input.size() - 1; ++i)
    {
        if (input[i] == input[i + 1])
        {
            size_t end = i + 2;
            while (end < input.size() && input[end] == input[i])
            {
                ++end;
            }

            input.erase(i, end - i);
            i = -1;
            continue;
        }
    }

    return input;
}

int main()
{
    string input = "235554431";
    string output = compress(input);

    cout << "input: " << input << '\n'
        << "output: " << output << endl;
}
```

Incrível como as ideias vêm quando menos esperamos. E elas não vêm na meia-hora que você mais precisa. Uma eternidade, agora pensando melhor.

[bug no algoritmo blowfish de tempo real asm de um boot de hd]: {{< ref o-bug-mais-bizarro-que-ja-resolvi >}}
[replit]: https://replit.com/@Caloni/Compress#compress_numbers_in_strings.cpp