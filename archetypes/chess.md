---
categories:
- blog
date: '{{ .Date }}'
tags:
- draft
- chess
title: {{ replace .TranslationBaseName "-" " " | title }}
---

{{< rawhtml >}}
<script>
new PgnViewer(
  {
    boardName: "cavaloni",
    showCoordinates: true,
    pgnFile: '/text/cavaloni.txt',
    pieceSet: 'merida',
    pieceSize: 65
  }
);
</script>

<div id="cavaloni-container"></div>  

<div id="cavaloni-moves"></div>  
{{< /rawhtml >}}
