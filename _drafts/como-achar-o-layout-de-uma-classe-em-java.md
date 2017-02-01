---
title: "Como achar o layout de uma classe em Java"
tags: [ "projetos", "java", "mobile" ]

---
Há um método simples e rápido para isso. Quem desenha o layout no Android é o __LayoutInflater__. Em toda classe java você irá encontrar uma chamada ao método __inflate__. Nele é necessário passar o id do layout que se deseja carregar, e este id é o id principal de um xml que contém o layout do elemento visual da tela (Activity ou Fragment).
