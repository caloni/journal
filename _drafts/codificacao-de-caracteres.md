---
title: "Codificação de caracteres"
tags: [ ]

---
Para que o git exiba corretamente o encoding dos commits (e dos arquivos utf-8) a melhor maneira é setar a variável de ambiente __LC_ALL=C.UTF-8__.

# "The Absolute Minimum Every Software Developer Absolutely, Positively Must Know About Unicode and Character Sets (No Excuses!)  (joelonsoftware.com)"

> Until now, we've assumed that a letter maps to some bits which you can store on disk or in memory: A -> 0100 0001 In Unicode, a letter maps to something called a code point which is still just a theoretical concept. How that code point is represented in memory or on disk is a whole nuther story. In Unicode, the letter A is a platonic ideal. It's just floating in heaven: A This platonic A is different than B, and different from a, but the same as A and A and A.

> Bizarre convention of storing a FE FF at the beginning of every Unicode string; this is called a Unicode Byte Order Mark and if you are swapping your high and low bytes it will look like a FF FE and the person reading your string will know that they have to swap every other byte.

> In UTF-8, every code point from 0-127 is stored in a single byte. Only code points 128 and above are stored using 2, 3, in fact, up to 6 bytes.

> It does not make sense to have a string without knowing what encoding it uses.

> You can no longer stick your head in the sand and pretend that "plain" text is ASCII. There Ain't No Such Thing As Plain Text.

> If you don't tell me whether a particular string is encoded using UTF-8 or ASCII or ISO 8859-1 (Latin 1) or Windows 1252 (Western European), you simply cannot display it correctly or even figure out where it ends. There are over a hundred encodings and above code point 127, all bets are off.

> How can you read the HTML file until you know what encoding it's in?! Luckily, almost every encoding in common use does the same thing with characters between 32 and 127, so you can always get this far on the HTML page without starting to use funny letters: \<html> \<head> \<meta http-equiv="Content-Type" content="text/html; charset=utf-8"> But that meta tag really has to be the very first thing in the \<head> section because as soon as the web browser sees this tag it's going to stop parsing the page and start over after reinterpreting the whole page using the encoding you specified.

