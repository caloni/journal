---
categories:
- code
date: '2022-06-09T11:43:45-03:00'
tags:
- windbg
title: Windbg Everywhere
---

# Usando WinDbg em situações onde não há Visual Studio

Com essa premissa me vem à mente os seguintes cenários:

 - Não é possível instalar Visual Studio
   - Só copiar a pasta onde está rodando o windbg.exe.
 - Arquivos de dump muito grandes
   - Processo que capota consumindo o mundo de memória.
   - Cópia de arquivos em alguns ambientes demoraria séculos.
 - Processos que não podem parar
   - Bug raro de acontecer e se parar o processo já era.
   - Manter processo em observação até reprodução (memory leaks macabros).
 - Único lugar que acontece o bug
   - É raro, mas acontece sempre.

## Colinha de todo dia

 - !analyze -v
 - !sym noisy, .symfix, .sympath
 - .reload /f, x
 - bp, bl, bu, g
 - db, dd, dv, da, du
 - ~, k, kv, kvn

## Leak de memória

 - !address -summary
 - !heap -stat, -srch AllocSize

## [Máquina com rede e com firewall](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/process-servers--user-mode-)

 - WinDbg é de hackudos, então [firewall não funciona](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/two-firewalls) com ele graças ao proxy reverso.

## Preciso do kernel

 - Começa a parte divertida: [gerar tela azul](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/forcing-a-system-crash-from-the-keyboard).

## Boot da máquina

 - bcdedit /dbgsettings, /copy {current}, set debug on
 - VM: \\.\pipe\kd
 - windbg.exe -k com:pipe,port=\\.\pipe\kd,resets=0,reconnect -b
 - Para bem no [código-fonte de David Cutler](https://systemroot.gitee.io/pages/apiexplorer/d6/d5/4_2kdinit_8c-source.html) para um debugger portável do kernel.
 - !process 0 0

## [Serviços no limite da existência](https://www.infoq.com/br/presentations/depurando-ate-o-fim-do-mundo/)

 - I get by with a [little help] [from my kernel](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/controlling-the-user-mode-debugger-from-the-kernel-debugger).

## Profiling

 - Para ver [quanto tempo leva](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/wt--trace-and-watch-data-) chamar uma função.

## Engenharia reversa

 - [Breakpoints condicionais](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/setting-a-conditional-breakpoint).
 - [Quebrando Houaiss].

## [Servidor de símbolos](https://docs.microsoft.com/en-us/windows/win32/debug/using-symstore)

 - Use o [symstore](https://docs.microsoft.com/en-us/windows/win32/debug/using-symstore) para montar uma fazenda de símbolos.
 - Use o [SymProxy](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/symproxy) para [publicar](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/http-symbol-stores).
 - O SymProxy também é útil como um... proxy =).

## [GFlags](https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/gflags)

 - Heap tagging e stack trace database para caçar leaks de memória.
 - Coleta silenciosa de dumps.
 - Chamar kernel debugger ao iniciar processo (parâmetro `-d` do ntsd.exe).

[little help]: {{< relref "kernel-mode-user-mode" >}}
[Quebrando Houaiss]: {{< relref "conversor-de-houaiss-para-babylon-parte-1" >}}