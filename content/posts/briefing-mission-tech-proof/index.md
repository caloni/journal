---
categories: null
date: '2023-05-01T21:58:22-02:00'
draft: true
tags: null
title: Briefing, Mission, Tech, Proof
---

Entre os infinitos refinamentos do processo da Intelitrader uma ficou particularmente boa: a descrição dos tickets. Inicialmente sem definição, cada ticket era criado no esquema ad hoc. Isso era ruim, principalmente (ou apenas) para os iniciantes, já que eles não tinham um mapa claro e definido do que precisava ser atingido em cada estória.

A descrição dos tickets foi resumida então em quatro seções obrigatórias, cuja versão em inglês é o título deste poste, mas que em português eu me lembro algo como  Situação, Objetivo, Orientação Técnica e Prove que Funciona. Vamos com as palavras em inglês para simplificar e internacionalizar.

Briefing

Toda estória precisa de um pano de fundo sobre onde estamos e o que aconteceu para este ticket ser criado. É a justificativa de por que é preciso fazer algo em vez de ficar sem fazer nada, que diferente de empresas onde você não gostaria de trabalhar, é o estado natural dos membros do time. Trabalhar por trabalhar é inútil e gera burocracia sem fim, além de suicídios e demissões em massa. Só se deve trabalhar se houver algo para ser feito, e só existe algo a ser feito se existir alguém esperando por isso. O briefing tem o dever de explicar isso.

Mission

Explicada a situação de como estão as coisas e da necessidade de algo ser feito vem a missão ou objetivo. Essa é a parte que o iniciante deve prestar mais atenção, pois é atendendo à missão que você poderá entregar o ticket e encerrar as atividades.

Tech

Todo trabalho na área de TI precisa de um pano de fundo técnico para ser feito, independente do nível que você está. Alguns detalhes são óbvios pelo projeto que se está mexendo, como a linguagem de programação usada. Outros não. Por exemplo, se testes unitários serão implementados nesta estória a seção de Orientação Técnica deve prover as informações de como isso deve ser feito: framework a ser utilizado, exemplos de uso. Se for uma estória de correção, é necessário explicar onde o dev irá mexer para corrigir o bug, ou como será feita a investigação.

Proof

Este é o final da entrega da estória. Aqui descreve-se a evidência que todos esperamos ver, e quando digo todos são todos os envolvidos nesta estória, não necessariamente só pessoal técnico. O cliente pode ver isso. O Product Owner. Pessoas não-técnicas devem ser capazes de entender a evidência de uma entrega não-técnica, ou melhor dizendo, o resultado que o usuário espera ver.