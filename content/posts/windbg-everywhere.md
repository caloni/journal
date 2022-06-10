---
categories:
- code
date: '2022-06-09T11:43:45-03:00'
draft: true
tags:
- draft
- windbg
title: Windbg Everywhere
---

# Usando WinDbg em situações onde não há Visual Studio

Com essa premissa me vem à mente os seguintes cenários:

## Não é possível instalar Visual Studio

O WinDbg vem com o Debugging Tools for Windows ou pelo Microsoft Store. De qualquer forma, onde ele estiver rodando você pode copiar a pasta e jogar em qualquer outra máquina e ele vai funcionar.

## Arquivos de dump muito grandes

Pode ocorrer do processo consumir muita memória (e até capotar por isso) e ter gerado um arquivo de dump gigante

## Processos que não podem parar

## Único lugar que acontece o bug

## Máquina com rede e com firewall

## Preciso do kernel

## Boot da máquina

O WinDbg consegue depurar o kernel desde muito cedo, até quando o Windows ainda não entrou no modo protegido. Nesse momento o que temos é memória em modo real e memória física, acessível não pode endereços virtuais, mas por localização na placa RAM. E o WinDbg consegue [acessar essa memória também](/acessando-memoria-fisica-no-windbg).

## Serviços no limite da existência

## Sem símbolos

## Engenharia reversa

