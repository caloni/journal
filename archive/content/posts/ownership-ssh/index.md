---
categories:
- coding
date: '2021-10-19T12:40:32-03:00'
tags:
- debug
title: Como resolver ownership do SSH
---

Primeiro tente logar com o usuário enquanto roda o comando abaixo no server para ver mensagens de log:

    # see logs from ssh server
    journalctl -f -t sshd

Se for problema de ownership então resolva dessa forma:

    # fix ownership for ssh server users
    chmod go-w ~
    chmod 700 .ssh
    chmod 600 .ssh/authorized_keys