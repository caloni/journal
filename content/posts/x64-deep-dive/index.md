---
categories:
- reading
- code
date: '2023-04-05'
link: https://codemachine.com/articles/x64_deep_dive.html
tags:
- debug
- videos
title: Como analisar assembly x64
---

Recomendo a leitura do artigo "X64 Deep Dive" para se habituar às idiossincrasias sobre o formato assembly do x64, especialmente se você costuma depurar assembly para Windows. O artigo descreve as novas funcionalidades que suportam os 64 bits do formato do executável Windows, o Portable Executable, além de explicar em detalhes o funcionamento de mecanismos que mudaram, como o tratamento de exceção (e o unwinding no código).

Criei [um repositório](https://github.com/Caloni/x64-deep-dive) para praticar alguns desses assuntos e recriar algum código-fonte para mostrar como o Visual Studio gera código em x64 e como depurar este código. Através deste repo e do [vídeo que pretendo gravar](https://youtu.be/wp7-BD9pOOc) a respeito caminharemos pelas mudanças desde o x86 para aumentarmos nossas habilidades em debugging de código x64. Entre algumas mudanças segue uma lista do que considerei mais importante:

 - Fastcall é a convenção de chamada default para x64.
 - RBP não é mais usado como frame pointer.
 - A última chamada da função pode ser otimizada com tail elimination.
 - Com isso o FPO (Frame Pointer Omission) pode comer solto.
 - RSP se mantém inalterado no corpo da função, entre o prólogo e o epílogo (gostei disso).
 - A técnica de homing space (opcional na compilação) salva os 4 primeiros parâmetros passados na pilha para a memória.
 - O Child-SP (RSP) é usado para caminhar tanto pelos parâmetros quanto pelas variáveis locais.

# Testes

## FastCall

```
/*
"Fastcall registers are used to pass parameters to functions. Fastcall is the
default calling convention on X64 where in the first 4 parameters are passed via
the registers RCX, RDX, R8, R9." - X64 Deep Dive

int TestFastCall() {
push        rdi
sub         rsp,30h # RBP is no longer used as frame pointer.
	int res = FastCallTest(1, 2, 3, 4);
mov         r9d,4
mov         r8d,3
mov         edx,2
mov         ecx,1
call        FastCall
mov         dword ptr [res],eax
	return res;
mov         eax,dword ptr [res]
}
add         rsp,30h # RBP is no longer used as frame pointer.
pop         rdi
ret
*/
```

## Tail Call Elimination

```
/*
"X64 compiler can optimize the last call made from a function by replacing it
with a jump to the callee. This avoids the overhead of setting up the stack
frame for the callee." - x64 Deep Dive

# TailCall3
	return a + b;
lea         eax,[rcx+rdx]
}
ret

# TestTailCallElimination
	TailCall1(1);
	TailCall2(2);
	return TailCall3(1, 2);
mov         edx,2
lea         ecx,[rdx-1]
jmp         TailCall3

*/
```

## Frame Pointer Omission

```
/*
"Unlike the X86 CPU where the EBP register is used to access parameters and
local variables on the stack, X64 functions do not make use of the RBP register
for this purpose i.e. do not use the EBP register as a
frame pointer." - x64 Deep Dive

# Win32 (x86)
int TestFramePointerOmission() {
	return 1;
00601593  mov         eax,1
}
00601599  ret

# Win62 (x64)
int TestFramePointerOmission() {
	return 1;
00007FF7D6E215E2  mov         eax,1
}
00007FF7D6E215E8  ret
*/
```

## RSP Is The Same

```
/*
Since RSP is used to reference both parameters and local variables in x64,
the side effect and feature of x64 function is that RSP does not change
thru all its body, changing only in prolog (begin) and epilog (end) parts
of the function.

# Win32
int RSPIsTheSame(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8) {
# prolog-begin
push        ebp
mov         ebp,esp
# prolog-end
	RSPIsTheSameCall1(p1);
mov         eax,dword ptr [p1]
push        eax # RSP--
call        RSPIsTheSameCall1
add         esp,4 # RSP++
	RSPIsTheSameCall4(p1, p2, p3, p4);
mov         ecx,dword ptr [p4]
push        ecx # RSP--
mov         edx,dword ptr [p3]
push        edx # RSP--
mov         eax,dword ptr [p2]
...
call        RSPIsTheSameCall4
add         esp,10h # RSP++
	RSPIsTheSameCall8(p1, p2, p3, p4, p5, p6, p7, p8);
mov         edx,dword ptr [p8]
push        edx # RSP--
...
call        RSPIsTheSameCall8
add         esp,20h # RSP++
	return 1;
mov         eax,1
}
# epilog-begin
pop         ebp
# epilog-end
ret

# Win64
int RSPIsTheSame(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8) {
# prolog-begin
mov         dword ptr [rsp+20h],r9d
mov         dword ptr [rsp+18h],r8d
mov         dword ptr [rsp+10h],edx
mov         dword ptr [rsp+8],ecx
push        rdi
sub         rsp,40h # RSP last change
# prolog-end
	RSPIsTheSameCall1(p1);
mov         ecx,dword ptr [p1]
call        RSPIsTheSameCall1
	RSPIsTheSameCall4(p1, p2, p3, p4);
mov         r9d,dword ptr [p4]
...
call        RSPIsTheSameCall4
	RSPIsTheSameCall8(p1, p2, p3, p4, p5, p6, p7, p8);
mov         eax,dword ptr [p8]
mov         dword ptr [rsp+38h],eax
...
call        RSPIsTheSameCall8
	return 1;
mov         eax,1
}
# epilog-begin
add         rsp,40h # RSP restore
pop         rdi
# epilog-end
ret
*/
```

## Homing Space

```
/*
"(...) homing space and is used to store parameter values if either the function
accesses the parameters by address instead of by value or if the function is
compiled with the /homeparams flag. The minimum size of this homing space is
0x20 bytes or four 64-bit slots, even if the function takes less than 4
parameters. When the homing space is not used to store parameter values, the
compiler uses it to save non-volatile registers." - x64 Deep Dive

"The register based parameter homing space exists only for non-leaf 
functions." - x64 Deep Dive

# Win64 Debug
int HomingSpaceNonLeaf(int p1, int p2, int p3, int p4) {
mov         dword ptr [rsp+20h],r9d # homing space saving params
mov         dword ptr [rsp+18h],r8d
mov         dword ptr [rsp+10h],edx
mov         dword ptr [rsp+8],ecx
push        rdi
sub         rsp,20h

# Win64 Release
int TestHomingSpaceNonLeaf() {
sub         rsp,28h
	int ret = HomingSpaceNonLeaf(1, 2, 3, 4);
mov         edx,2 # even begin non-leaf function, params are not saved in release
lea         r9d,[rdx+2]
lea         r8d,[rdx+1]
lea         ecx,[rdx-1]
call        HomingSpaceNonLeaf
*/
```

## Child-SP

```
/*
"The value of the Child-SP register displayed by the debugger's "k" command
represents the address at which the stack pointer (RSP) points to, as the point
where the function displayed in that frame, has finished executing its prolog.
The next item that would be pushed on the stack would be the return address of
the function as it invokes its callees. Since X64 functions do not modify the
value of RSP after the function prolog, any stack accesses performed by the rest
of the function are done relative to this position of the stack pointer. This
includes access to stack based parameters and local variables." - x64 Deep Dive

*/
# Win64 Debug
			p5 = i + j + (i % 2 ? p1 : p2) + (j % 2 ? p3 : p4);
mov         eax,dword ptr [rsp+0Ch]
cdq
and         eax,1
xor         eax,edx
sub         eax,edx
test        eax,eax
je          ChildSPF3+0BBh
mov         rax,qword ptr [p1]
mov         eax,dword ptr [rax]
mov         dword ptr [rsp+14h],eax # reference child-sp
jmp         ChildSPF3+0C6h
mov         rax,qword ptr [p2]
mov         eax,dword ptr [rax]
mov         dword ptr [rsp+14h],eax # reference child-sp
mov         eax,dword ptr [rsp+10h] # reference child-sp
cdq
and         eax,1
xor         eax,edx
sub         eax,edx
test        eax,eax
je          ChildSPF3+0E3h
mov         rax,qword ptr [p3]
mov         eax,dword ptr [rax]
mov         dword ptr [rsp+18h],eax # reference child-sp
jmp         ChildSPF3+0EEh
mov         rax,qword ptr [p4]
mov         eax,dword ptr [rax]
mov         dword ptr [rsp+18h],eax # reference child-sp
mov         eax,dword ptr [rsp+10h] # reference child-sp
mov         ecx,dword ptr [rsp+0Ch] # reference child-sp
add         ecx,eax
mov         eax,ecx
add         eax,dword ptr [rsp+14h] # reference child-sp
add         eax,dword ptr [rsp+18h] # reference child-sp
mov         rcx,qword ptr [p5]
mov         dword ptr [rcx],eax
```

## Parameter Retrieval

```
/*
"(...) as execution progresses within the function body, the contents of the
parameter registers change and the initial parameter value gets overwritten. So,
to determine the value of these register based parameters at any point during
function execution, one needs to find out - where is the value of the parameter
being read from and where is the value of the parameter being written to?
Answers to these questions can be found by performing a sequence of steps in the
debugger which can be grouped as follows: Determine if the parameters are loaded
into the registers from memory. If so, the memory location can be examined to
determine the parameter values. Determine if the parameters are loaded from
non-volatile registers and if those registers are saved by the callee. If so,
the saved non-volatile register values can be examined to determine the
parameter values. Determine if the parameters are saved from the registers into
memory. If so, the memory location can be examined to determine the parameter
values. Determine if the parameters are saved into non-volatile registers and if
those registers are saved by the callee. If so, the saved non-volatile register
values can be examined to determine the parameter values." - x64 Deep Dive

# Win64 Release
1. Parameters are loaded into the registers from memory.
mov         dword ptr [rbp+30h],5 # p5 = 5
mov         dword ptr [rbp+28h],6 # p6 = 6
mov         dword ptr [rbp+20h],7 # p7 = 7
mov         dword ptr [rbp+18h],8 # p8 = 8
call        ChildSPF1

2. Parameters are loaded from non-volatile registers and those registers
are saved by the callee.

	int oldP7 = *g_ParameterRetrieval_p7_retrieval;
	return p1 + p2 + p3 + p4 + p5 + p6 + oldP7 + p8;
mov         eax,dword ptr [rdx]
mov         r10,rcx
add         eax,dword ptr [r8]
add         eax,dword ptr [r9]
mov         rdx,qword ptr [p5]
mov         rcx,qword ptr [g_ParameterRetrieval_p7_retrieval]

3. Parameters are saved from the registers into memory.

# Win64 Release
	g_ParameterRetrieval_p7_retrieval = &p7;
mov         r10,qword ptr [p7]
mov         r11,rcx
	return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8;
mov         rcx,qword ptr [p8]
mov         qword ptr [g_ParameterRetrieval_p7_retrieval],r10 # save nonvolatile register


4. Parameters are saved into non-volatile registers and those registers
are saved by the callee.

# Win64 Release
int ParameterRetrieval3(int& p1, int& p2, int& p3, int& p4, int& p5, int& p6, int& p7, int& p8) {
mov         qword ptr [rsp+8],rbx
mov         qword ptr [rsp+10h],rsi
mov         qword ptr [rsp+18h],rdi
mov         qword ptr [rsp+20h],r14
push        r15 # saves what will be p7
	int oldP7 = p7;
	for (int i = 0; i < 7; ++i) {
		p7 += p1;
mov         r10d,dword ptr [rcx]
mov         r14,rcx
mov         r15,qword ptr [p7] # going to use p7
mov         rbx,rdx
mov         rdi,r9
mov         esi,dword ptr [r15] # Parameters are saved into non-volatile registers...
add         r10d,esi
...
	}
	int ret = p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8;
...
	p7 = oldP7;
	return ret;
}
...
mov         dword ptr [r15],esi # ... and those registers are saved by the callee.
mov         rsi,qword ptr [rsp+18h]
pop         r15
ret
*/
```

# Recortes do artigo original

> On X64, the first 4 parameters are always passed in registers and the rest of the parameters are passed via the stack. This is one of main causes of grief during debugging since register values tend to change as functions execute and it becomes difficult to determine the original parameter values that were passed to a function, half-way into its execution. Other than this one issue with retrieving parameters, x64 debugging is not that different from x86 debugging.

> Unfortunately, there is no silver bullet to finding parameters. All the techniques here depend heavily on the X64 assembler instructions generated by the compiler. If the parameters are not in "reachable memory", there is simply no way to get them. Having private symbols for modules and functions that appear in the call stack doesn't help too much either. Private symbols do tell the number and types of parameters a function takes, but that's about it. It does not tell what those parameter values are. 

>
> Determine if the parameters are loaded from non-volatile registers and if those registers are saved by the callee. 
>
> Determine if the parameters are saved from the registers into memory. 
>
> Determine if the parameters are saved into non-volatile registers and if those registers are saved by the callee. 
>
> Each one of the techniques requires disassembling the caller and the callee functions involved in the parameter passing. 
>

> The debugger's ".frame /r" command displays the values of non-volatile registers when a particular function was executing. 

> it is important to always verify parameter register overwrites 

> It is important to examine the instructions up to the call to the next function (...) to ascertain that these non-volatile registers are not being overwritten. 
