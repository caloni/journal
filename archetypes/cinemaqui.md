---
date: {{ .Date }}
title: "{{ replace .TranslationBaseName "-" " " | title }}"
categories: [ "cinemaqui" ]
imdb: "0000000"
draft: "true"
---

<hr>
Original Title (País, Ano), escrito por <roteirista>, dirigido por <diretor>, com <atores>.
</hr>

<h2>Trailer - {{ replace .TranslationBaseName "-" " " | title }}</h2>
