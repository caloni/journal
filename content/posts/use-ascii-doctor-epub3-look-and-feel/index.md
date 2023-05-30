---
categories:
- code
date: '2023-05-05T10:03:29-02:00'
draft: true
tags:
- english
title: Use asciidoctor ePUB3 look and feel
---

Here is the background for this issue:

>
> https://docs.asciidoctor.org/epub3-converter/latest/
> 
> roube o tema CSS pra ePUB3 do asciidoctor
> 
> você não precisa usar asciidoc
> 
> eles pegaram quirks de vários e-readers no mercado pra criar esse tema
> 
> abra a homepage aí do link que você pode ler mais sobre o projeto deles
> 
> aí você cria um documento asciidoc vazio e extrai os CSSs pra usar no teu
> 
> se não tiver paciência nem pra instalar o asciidoctor só pra roubar os 
> temas, tem esse ePUB random pronto criado nessa ferramenta aí: 
> https://emilua.gitlab.io/docs2/emilua.epub
>
> LorTigre Arroz 🐍, [1/31/2023 10:59 PM]
> 

So I am going to use ePUB3 style from asciidoctor in the blog book convertion. In order to do that I just need to copy and paste the CSS to the book style sheet and adjust the theme. As a proof of concept, the published book look and feel must be equivalent (not identical) to the asciidoctor style.

Now studying the asciidoc format I believe it is time to another blog conversion. This is going to make supereasy to structure my posts and the metadata will be finally part of the document itself, and not a "puxadinho" in YAML because markdown doesn't know how to do that.

In order to to this, the first task is to test a ePUB conversion from some asciidoc files to a ePUB file, with or without hugo usage. If it work without hugo the publishing step is just a matter of copy files into a structure without activating hugo, but asciidoctor instead.
