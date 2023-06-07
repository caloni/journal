---
categories:
- coding
date: '2023-06-06'
tags:
- reversing
title: OnBoarding no baixo nível
---

Uhu! Estou de volta para as baixarias =). Irei trabalhar com engenharia reversa, serviços e drivers no Windows, talvez algumas telas azuis da morte e alguns travamentos inexplicáveis. Ou seja, só diversão.

E vou aproveitar meu processo de onboarding para recomendar as ferramentas que já utilizo no meu dia-a-dia e as novas que meus colegas me passaram.

## SDK ou Visual Studio

No Visual Studio já vem o SDK de desenvolvimento para Windows. E dentro dele vem as libs e headers para você conseguir compilar programas em C e C++ que usam a API do Windows.

Porém, o importante de lá é que existem ferramentas para realizar análise de executáveis, como o DumpBin, e debug remoto através de uma porta TCP; na seção de VMs falo sobre isso.

## WDK

O WDK é o SDK para kernel mode. Lá haverá libs e headers também, mas mais focadas para o desenvolvimento de drivers. E uma ferramenta, ou melhor dizendo, um conjunto de ferramentas que não pode faltar e que está lá é o...

## WinDbg ou Debugging Tools

Sem WinDbg a vida fica um pouco mais triste. Dentro do pacote Debugging Tools há ele e suas variantes, como o kd, ntsd e cdb. Também há o gflags, para setar várias flags importantíssimas na depuração do sistema operacional, e uma documentação bem extensa para desenvolver scripts e extensões para depuração, além de outras coisas mais, como um servidor de símbolos.

## [x64dbg](https://x64dbg.com/)

Não conhecia este depurador. Ele é fonte aberto, o que já é interessante. Baixei e pretendo olhar mais de perto. Jurava que iriam me sugerir o OllyDbg pelo livro que estou lendo de reversa. Este carinha deve ter sido lançado após o livro.

## [PeTools](https://github.com/petoolse/petools/releases)

Uma ferramenta de análise de PE, ou Portable Executable, o formato dos EXEs e DLLs do Windows. Eu particularmente uso o DumpBin, mas vou dar uma olhada nesta recomendação que deve agilizar um pouco as análises.

## [Resource Hacker](http://angusj.com/resourcehacker/)

Usava esse carinha há muitíssimo tempo atrás, para uma tarefa não-nobre: editar na mão os resources pós-compilados do C++ Builder porque ele vivia cheio de bugs. Ah, e também manipular chaves criptográficas guardadas nos resources. Ele também é bem legal para achar novos ícones e incrementar seu computador, além de um ótimo editor de strings.

## [WinMerge](https://winmerge.org/)

Eu já tentei usar o Vim no modo de comparação, mas WinMerge para Windows não se compara. Ele é simples e eficiente. Mantenho apenas o executável no meu cinto de utilidades e ele consegue se virar para pastas inteiras. Suas configurações permitem ignorar detalhes como final de linha e indentação.

## [HxD](https://mh-nexus.de/en/hxd/)

Um editor hexa que virou queridinho depois que um colega da Open me apresentou. Ele é daqueles um executável apenas que faz todo o serviço de mostrar dados em hexadecimal de qualquer arquivo ou mesmo da memória (ou do disco).

## [Sysinternals Suite](https://learn.microsoft.com/en-us/sysinternals/downloads/sysinternals-suite)

Não vou perder tempo descrevendo cada uma das fantásticas ferramentas da SysInternals, então tomaí: um toolkit cheio delas.

## [RegShot](https://sourceforge.net/projects/regshot/)

Não conhecia este aqui e fiquei muito empolgado. Ele faz comparação entre registros! Imagina a maravilha que isso será para alguns de meus projetos que envolvem diretivas de acesso...

## [Wireshark](https://www.wireshark.org/)

Essencial para analisar tráfego de rede no detalhe, já me quebrou muitos galhos quando precisava entender erros de comunicação com a bolsa de valores. Sim, eles também erram. Quem diria.

## [ArtMoney](https://artmoney.ru/)

Este é um scanner de memória feito para aplicar patches em jogos. Que feio! Porém, para um reverseiro pode ser útil para encontrar padrões e reconhecer comportamento.

## [IDA](https://hex-rays.com/ida-free/)

IDA é um disassembler que brilha na análise estática. Ele ajuda muito a reconstruir código-fonte ou entender comportamento de funções inteiras apenas lendo o assembly e os nomes sugestivos dos calls e jumps que a ferramenta lhe fornece.

## [ShareX](https://getsharex.com/)

Esta é uma ferramenta para gerar screenshots e vídeos, ou seja, gerar evidência. Qualquer uma na verdade serve, mas essa foi a sugestão do colega e pretendo guardar aqui.

## VMWare

Virtualizar o Windows é essencial para instalar drivers instáveis, analisar telas azuis e o comportamento do sistema como um todo, do user mode ao kernel. Portanto, trate de arrumar algumas das soluções disponíveis. Eu uso VMWare desde que foi lançada e já comprei algumas licenças de novas versões dos caras, porque eles são foda. Você pode usar também VirtualBox ou outras por aí. O importante é virtualizar.

Por falar nisso, é importante lembrar que o Windows 11 requer um chip de TPM 2.0 disponível para ser instalado, e por isso você irá precisar do VMWare 17 ou superior. Se atente a [este guia de instalação] para conseguir rodar o Win11 virtualizado.

## Remote Debugging

A depuração remota é importantíssima quando se precisa acessar múltiplos ambientes, muitos deles montados de uma maneira bem específica para reproduzir um bug. Por isso o uso do msvsmon, disponível na instalação do Visual Studio, é primordial.

Você pode encontrar as versões para x86 e x64 do msvsmon dentro da pasta de instalação do Visual Studio. Por exemplo:

```
C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\IDE\Remote Debugger
```

Para instalar na máquina guest tudo que você precisa é copiar essas pastas e rodar qualquer um dos msvsmon.exe disponíveis. Rode pelo menos uma vez para acertar a porta no Firewall do Windows. Depois disso pode rodar como tarefa automática.

Eu acho muito útil configurar um serviço que rode o msvsmon, de forma que mesmo antes de logar já é possível depurar programas. Para isso eu uso uma outra ferramenta fantástica, o [NSSM](https://nssm.cc/download), mas qualquer outra ferramenta que transforme um programa em serviço já serve.

Usando o NSSM configure o programa Msvsmon para rodar como um serviço. Aqui está uma configuração bem direta, sem autenticação, para usar o msvsmon através do nssm:

```
nssm.exe install msvsmon c:\tools\rmdbg\x86\msvsmon.exe /anyuser /noauth /timeout 9999999 /silent
```

Use o path completo de onde está o msvsmon.exe na máquina guest para rodar este comando. O serviço é instalado por padrão como automático, então ao reiniciar a máquina ele já estará em execução.

[este guia de instalação]: https://www.digitalcitizen.life/install-windows-11-virtual-machine/

## Kernel Debugging

Qual a graça de depurar apenas os programas em user mode? E aquela tela azul marota, ou acompanhar o boot da máquina em câmera hiperlenta? Aproveite que já está com sua VM bonitinha no ar e configure o debug de kernel rodando um cmd como administrador e executando alguns comandos no bcdedit.

```
bcdedit /copy {current} /d Debug
bcdedit /debug {new-id} ON
bcdedit /dbgsettings SERIAL DEBUGPORT:1 BAUDRATE:115200
```

Isso irá criar uma segunda opção de boot que irá aparecer quando ligar a máquina (timeout padrão de 30 segundos para escolher). Se configurar debug pela porta serial crie uma na máquina virtual apontando para um named pipe. No WinDbg conecte usando este pipe (e.g. `\\.\pipe\com_1´).

Agora começa a diversão =).