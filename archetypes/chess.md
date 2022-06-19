---
categories:
- blog
date: '{{ .Date }}'
tags:
- draft
- chess
title: {{ replace .TranslationBaseName "-" " " | title }}
---

{{< chess board="cavaloni" src="{{ .TranslationBaseName }}.txt">}}
