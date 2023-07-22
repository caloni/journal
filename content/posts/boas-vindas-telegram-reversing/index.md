---
categories:
 - coding
date: '2023-07-21'
link: https://youtu.be/HYkG4ZG7lxU
tags:
 - reversing
title: Boas-vindas ao canal de Reversing do Telegrama
---

Olá, pessoas do Telegrão que entraram [no canal](https://t.me/reversingclubc). Bora começar a fazer baixarias pra ver se anima? Bom, acho que a primeira coisa que todo reversing de computador precisa fazer é gerar sua primeira telinha azul com uma poesia. Vamos lá?

Primeiro passo: configurar para [gerar tela azul em seu computador](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/forcing-a-system-crash-from-the-keyboard).

Segundo passo: na hora de gerar a tela azul é importante que seja gerado um dump de memória (no português: despejo de memória). E é importante que seja gerado da memória completa. Do contrário nossa poesia pode não estar lá.

Terceiro passo: rebootar a desgraça do Windows para os efeitos fazerem efeitos.

Quarto passo: vamos ver se gera tela azul? Tem que apertar o botão CTRL mais à direita do seu teclado junto da tecla SCROLL LOCK duas vezes (mantendo o CTRL apertado).

É, aqui não funcionou. Meu "teclado" (virtual) da VM deve ser PS2 =p. Se não funcionou também com você verifique isso e mude o registro de acordo.

Corta para os comerciais...

Aê!! Tela azul!! `\/\/\/`

Mas essa não valeu porque não teve poesia...

Quinto passo: escolha a poesia de sua preferência e digite-a no notepad. Vale copiar e colar, também.

Eu vou escolher uma poesia que nossa professora da quinta série leu para a gente:

```
If you can talk with crowds and keep your virtue,
Or walk with Kings—nor lose the common touch,
If neither foes nor loving friends can hurt you,
If all men count with you, but none too much;
If you can fill the unforgiving minute
With sixty seconds’ worth of distance run,
Yours is the Earth and everything that’s in it,
And—which is more—you’ll be a Man, my son!
```

Você vai abrir o bloco de notas e digitar/colar sua poesia, mas, importante, **não irá salvar**. Apenas deixe aberta no bloco de notas. É essa a graça.

E... tela azul again. Porém, dessa vez, vamos deixar gerar o arquivo até o final. Se estiver assistindo ao vídeo pode acelerar até o reboot.

Enquanto gera a tela azul vamos falar sobre o que esse experimento deseja provar. Todo trabalho de engenharia reversa que pretende descobrir como algo que não sabemos como funciona... funciona, precisa utilizar o método científico para avançar. Cria-se uma hipótese, encontra-se elementos que a corrobore essa hipótese e faz-se os experimentos que a comprove. Quando há múltiplas possibilidades também é importante fazer experimentos que desprovem algum caminho a ser seguido.

No caso, a hipótese é que uma tela azul de toda a memória necessariamente irá conter os textos que um usuário estiver digitando no bloco de notas, pois esse texto precisa estar em algum lugar da memória. Certo?

Bom, é o que eu acho. Para saber se isso é verdade podíamos digitar algum texto no bloco de notas e, sem salvar nem nada, gerar uma tela azul. O texto deverá estar no arquivo de dump gerado. Texto simples e aberto, pois é isso o que o usuário estava digitando. Parece um teste simples de ser feito e, até que se prove o contrário, conclusivo, não?

Veremos.

O arquivo de dump gerado deverá ter o mesmo tamanho da memória RAM. Com a ajuda de algum programa que encontre strings em binários vamos fazer uma busca pela poesia. Pode ser o programa [Strings](https://learn.microsoft.com/en-us/sysinternals/downloads/strings), da SysInternals.

```
strings.exe MEMORY.DMP
```

Uma outra opção seria abrir o arquivo em um editor hexadecimal como o [HxD](https://mh-nexus.de/en/hxd/) e buscar pela string.

É, no meu caso a busca do HxD deu mais boa. Pera...

Hum... isso vai demorar. Mas ele vai achar.

Achou.

Acho que é isso. Bem-vindo ao grupo! E se quiser conversar sobre o assunto, outros assuntos, dúvidas técnicas, etc. Fique à vontade. De bits e bytes até moléculas de café, reversing é sobre isso =)
