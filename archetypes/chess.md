---
categories:
- chess
date: '{{ .Date }}'
tags:
- draft
title: {{ replace .TranslationBaseName "-" " " | title }}
---

{{< chess board="cavaloni" src="{{ .TranslationBaseName }}.txt">}}
