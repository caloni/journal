---
categories: [ "code" ]
date: "2011-10-18"
tags: [ "debug", "tools", "draft" ]
title: "Depuração de emergência com receita de bolo no WinDbg"
---
Continuando o papo sobre o que fazer para analisar rapidamente um crash no servidor com o pacote WinDbg, na maioria das vezes a exceção lançada pelo processo está diretamente relacionada com um acesso indevido à memória, o que tem diversas vantagens sobre problemas mais complexos:

  - Possui localização precisa de onde ocorreu a violação, incluindo o nome do arquivo e a linha do código;
  - Não corrompe a pilha ou, se corrompe, não chega a afetá-la a ponto da thread ficar irreconhecível;
  - A thread que contém a janela de crash é a culpada imediata, então basta olhar a pilha.

Resumindo: basta olhar a pilha! Mas, para isso ser efetivo, precisaremos do PDB do executável que gerou o crash, pois através dele é possível puxar a tal localização da violação de acesso. Isso quer dizer que se você mantiver o executável, e DLL também é executável, juntinho com seu PDB, ou pelo menos facilmente localizável, sua vida será muito mais fácil e florida. Também significa que poderá começar a beber cerveja mais cedo.

Mesmo que em alguns momentos-surpresa apareça uma ou outra tela indesejada.

O comando mais útil na maioria desses casos é mostrar a pilha no modo verbose, usando o comando kv seguido de enter. Porém, antes disso, precisamos:
	
  1. Ajeitar o path dos símbolos;
  2. Recarregar o PDB do executável suspeito;
  3. Mostrar a pilha de todas as threads até descobrir a culpada.

Todos esses comandos podem ser vistos abaixo. São, respectivamente, .symfix, .reload e novamente o kv, com a diferença de que para todas threads.
    
    0:001> .symfix
    0:001> .reload /f CrashOnServer.exe
    *** WARNING: Unable to verify checksum for C:\Users\wanderley.caloni\Documents\Projetos\Caloni\Posts\Debug\CrashOnServer.exe
    0:001> kv
    Child-SP RetAddr  : Args to Child               : Call Site
    0030f918 77679198 : 00000000`00000000 `00000000 : ntdll!DbgBreakPoint
    0030f920 775e244d : 00000000`00000000 `00000000 : ntdll!DbgUiRemoteBreakin+0x38
    0030f950 00000000 : 00000000`00000000 `00000000 : ntdll!RtlUserThreadStart+0x25
    0:001> ~* kv
    
       0  Id: 1dc.978 Suspend: 1 Teb: 00000000`7efdb000 Unfrozen
    Child-SP RetAddr  : Args to Child                       : Call Site
    0008ea48 751f282c : 00000000`77770190 00000000`001dfb50 : wow64cpu!CpupSyscallStub+0x9
    0008ea50 7526d07e : 00000000`00000000 00000000`775b3501 : wow64cpu!WaitForMultipleObjects32+0x32
    0008eb10 7526c549 : 00000000`00000000 00000000`7ffe0030 : wow64!RunCpuSimulation+0xa
    0008eb60 775cae27 : 00000000`003b3710 00000000`7efdf000 : wow64!Wow64LdrpInitialize+0x429
    0008f0b0 775c72f8 : 00000000`00000000 00000000`00000000 : ntdll!LdrpInitializeProcess+0x1780
    0008f5b0 775b2ace : 00000000`0008f670 00000000`00000000 : ntdll! ?? ::FNODOBFM::`string'+0x2af20
    0008f620 00000000 : 00000000`00000000 00000000`00000000 : ntdll!LdrInitializeThunk+0xe

Ops! Um pequeno desvio do curso. Estamos rodando um processo de 32 bits dentro de um SO 64 bits, no exemplo um Windows 7. Isso pode acontecer e é bom saber o que fazer nesse caso. Seguimos com os comandos .load wow64exts e .effmach x86, que irá carregar a extensão de wow64 do depurador e iniciar a tradução da stack para 32 bits.

    0:001> .load wow64exts
    0:001> .effmach x86
    Effective machine: x86 compatible (x86)
    0:001:x86> ~* kv
    
       0  Id: 1dc.978 Suspend: 1 Teb: 7efdb000 Unfrozen
    ChildEBP RetAddr  Args to Child
    001df24c 761a0bdd 00000002 001df29c 00000001 ntdll_77760000!NtWaitForMultipleObjects+0x15 (FPO: [5,0,0])
    001df2e8 7727162d 001df29c 001df310 00000000 KERNELBASE!WaitForMultipleObjectsEx+0x100 (FPO: [Non-Fpo])
    001df330 77271921 00000002 7efde000 00000000 KERNEL32!WaitForMultipleObjectsExImplementation+0xe0 (FPO: [Non-Fpo])
    001df34c 77299b2d 00000002 001df380 00000000 KERNEL32!WaitForMultipleObjects+0x18 (FPO: [Non-Fpo])
    001df3b8 77299bca 001df498 00000001 00000001 KERNEL32!WerpReportFaultInternal+0x186 (FPO: [Non-Fpo])
    001df3cc 772998f8 001df498 00000001 001df468 KERNEL32!WerpReportFault+0x70 (FPO: [Non-Fpo])
    001df3dc 77299875 001df498 00000001 38239b1e KERNEL32!BasepReportFault+0x20 (FPO: [Non-Fpo])
    001df468 777d0df7 00000000 777d0cd4 00000000 KERNEL32!UnhandledExceptionFilter+0x1af (FPO: [Non-Fpo])
    001df470 777d0cd4 00000000 001dfb34 7778c550 ntdll_77760000!__RtlUserThreadStart+0x62 (FPO: [SEH])
    001df484 777d0b71 00000000 00000000 00000000 ntdll_77760000!_EH4_CallFilterFunc+0x12 (FPO: [Uses EBP] [0,0,4])
    001df4ac 777a6ac9 fffffffe 001dfb24 001df5e8 ntdll_77760000!_except_handler4+0x8e (FPO: [Non-Fpo])
    001df4d0 777a6a9b 001df598 001dfb24 001df5e8 ntdll_77760000!ExecuteHandler2+0x26
    001df580 7777010f 001df598 001df5e8 001df598 ntdll_77760000!ExecuteHandler+0x24
    001df584 001df598 001df5e8 001df598 001df5e8 ntdll_77760000!KiUserExceptionDispatcher+0xf (FPO: [2,0,0])
    WARNING: Frame IP not in any known module. Following frames may be wrong.
    001df9ac 010d141e 00000000 00000000 00000000 0x1df598
    001dfa90 010d19af 00000001 00321410 00321c70 CrashOnServer!main+0x2e (FPO: [Non-Fpo]) (CONV: cdecl)
        [c:\users\wanderley.caloni\documents\projetos\caloni\posts\crashonserver\crashonserver.cpp @ 13]
    001dfae0 010d17df 001dfaf4 77273677 7efde000 CrashOnServer!__tmainCRTStartup+0x1bf (FPO: [Non-Fpo]) (CONV: cdecl)
        [f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c @ 555]
    001dfae8 77273677 7efde000 001dfb34 77799f02 CrashOnServer!mainCRTStartup+0xf (FPO: [Non-Fpo]) (CONV: cdecl)
        [f:\dd\vctools\crt_bld\self_x86\crt\src\crtexe.c @ 371]
    001dfaf4 77799f02 7efde000 6b3e1b48 00000000 KERNEL32!BaseThreadInitThunk+0xe (FPO: [Non-Fpo])
    001dfb34 77799ed5 010d1109 7efde000 00000000 ntdll_77760000!__RtlUserThreadStart+0x70 (FPO: [Non-Fpo])
    001dfb4c 00000000 010d1109 7efde000 00000000 ntdll_77760000!_RtlUserThreadStart+0x1b (FPO: [Non-Fpo])
    
    #  1  Id: 1dc.1b0 Suspend: 1 Teb: 7efd8000 Unfrozen
    ChildEBP RetAddr  Args to Child
    0056ffe8 00000000 00000000 00000000 00000000 ntdll_77760000!RtlUserThreadStart (FPO: [0,2,0])

Nosso depurador favorito acusa uma pilha que contém a função WerpReportFault. Nessa mesma thread a última linha conhecida nossa está no arquivo crashonserver.cpp:13. E essa situação, caro leitor, é dez por cento de tudo o que você precisa saber sobre WinDbg para resolver, mas que já resolve noventa por cento dos casos que irá encontrar em produção. Belo custo-benefício, não?
