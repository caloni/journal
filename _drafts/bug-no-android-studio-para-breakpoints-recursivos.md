---
title: "Bug no Android Studio para breakpoints recursivos"
tags: [ "projetos", "java", "mobile" ]

---
Você percebe que não há muita depuração em determinados ambientes de programação quando os bugs mais escabrosos acabam sendo os mais comuns.

Este, por exemplo, onde coloquei inúmeros breakpoints no Android Studio para depurar o comportamento de um app gerou constantemente o seguinte erro:

```cmd
Check failed: Thread::Current () GetDebugThread
```

Quando o primeiro erro no log é "Check failed: Thread::Current() != GetDebugThread()" isso [indica](http://stackoverflow.com/questions/40618803/crash-android-app-when-debug) que isso ocorre com "breakpoints aninhados" ("breakpoint in a nested function"). Apaguei todos os breakpoints e o app instantaneamente voltou a rodar. Sim, foi isso mesmo. Apenas um erro na IDE (ou no depurador).
