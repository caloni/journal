---
author: admin
comments: true
date: 2011-10-14 01:52:03+00:00
layout: post
slug: coders-at-work-brendan-eich-threads-e-depuracao
title: 'Coders at Work: Brendan Eich, threads e depuração'
wordpress_id: 1208
categories:
- Blogging
tags:
- depuração
- livros
- multithreading
- programadores
---

<blockquote>_"Proofs are hard. Most people are lazy. Larry Wall is right. Laziness should be a virtue. So that’s why I prefer automation. Proofs are something that academics love and most programmers hate." - _Brendan Eich</blockquote>




[caption id="attachment_1211" align="aligncenter" width="150" caption="Brendan Eich, Pai do JavaScript"][![](/images/1202470502643-150x150.jpg)](/images/1202470502643.jpg)[/caption]


Esse pequeno trecho da entrevista de Brendan Eich, de [Coders at Work](http://www.codersatwork.com/brendan-eich.html), revela parte das frustações que os programadores de linha de frente sofrem com os ambientes de depuração, muitas vezes aquém dos desafios atuais. Sinceramente, não sinto isso em meu dia-a-dia, e acho o Visual Studio um excelente depurador com interface (mas que perde feio para o WinDbg em casos mais hardcore). Porém, fica a percepção curiosa do criador do JavaScript.


Sobre SGI

_"Diagnosing it was hard because it was timing-sensitive. It had to do with these machines being abused by terminal concentrators. People were hooking up a bunch of PTYs to real terminals. Students in a lab or a bunch of people in a mining software company in Brisbane, Australia in this sort of ’70s sea of cubes with a glass wall at ’70s sea of cubes with a glass wall at the end, behind which was a bunch of machines including the SGI two-processor machine. That was hard and I’m glad we found it. These bugs generally don’t linger for years but they are really hard to find. And you have to sort of suspend your life and think about them all the time and dream about them and so on. You end up doing very basic stuff, though. It’s like a lot of other bugs. You end up bisecting—you know “wolf fence.” You try to figure out by monitoring execution and the state of memory and try to bound the extent of the bug and control flow and data that can be addressed. If it’s a wild pointer store then you’re kinda screwed and you have to really start looking at harder-to-use tools, which have only come to the fore recently, thanks to those gigahertz processors, like Valgrind and Purify."_

Ferramentas de Depuração Avançadas

[caption id="attachment_1212" align="aligncenter" width="129" caption="Logo da Valgrind"][![](/images/ValgrindLogo.png)](/images/ValgrindLogo.png)[/caption]

_"Instrumenting and having a checked model of the entire memory hierarchy is big. Robert O’Callahan, our big brain in New Zealand, did his own debugger based on the Valgrind framework, which efficiently logs every instruction so he can re-create the entire program state at any point. It’s not just a time-traveling debugger. It’s a full database so you see a data structure and there’s a field with a scrogged value and you can say, “Who wrote to that last?” and you get the full stack. You can reason from effects back to causes. Which is the whole game in debugging. So it’s very slow. It’s like a hundred times slower than real time, but there’s hope."_

_"Or you can use one of these faster recording VMs—they checkpoint only at system call and I/O boundaries. They can re-create corrupt program states at any boundary but to go in between those is harder. But if you use that you can probably close in quickly at near real time and then once you get to that stage you can transfer it into Rob’s Chronomancer and run it much slower and get all the program states and find the bug."_

Depuradores da Indústria

_"Debugging technology has been sadly underresearched. That’s another example where there’s a big gulf between industry and academia: the academics are doing proofs, sometimes by hand, more and more mechanized thanks to the POPLmark challenge and things like that. But in the real world we’re all in debuggers and they’re pieces of shit from the ’70s like GDB."_

[caption id="attachment_1213" align="aligncenter" width="200" caption="Mascote do GDB"][![](/images/mascot.jpg)](/images/mascot.jpg)[/caption]




<blockquote>_"In the real world one big split is between people who use symbolic debuggers and people who use print statements."_ - Peter Seibel</blockquote>




_"Yeah. So I use GDB, and I’m glad GDB, at least on the Mac, has a watch-point facility that mostly works. So I can watch an address and I can catch it changing from good bits to bad bits. That’s pretty helpful. Otherwise I’m using printfs to bisect. Once I get close enough usually I can just try things inside GDB or use some amount of command scripting. But it’s incredibly weak. The scripting language itself is weak. I think Van Jacobson added loops and I don’t even know if those made it into the real GDB, past the FSF hall monitors."_

Multithreading

_"But there’s so much more debugging can do for you and these attempts, like Chronomancer and Replay, are good. They certainly changed the game for me recently. But I don’t know about multithreading. There’s The multithreaded stuff, frankly, scares me because before I was married and had kids it took a lot of my life. And not everybody was ready to think about concurrency and all the possible combinations of orders that are out there for even small scenarios. Once you combine code with other people’s code it just gets out of control. You can’t possibly model the state space in your head. Most people aren’t up to it. I could be like one of these chestthumpers on Slashdot—when I blogged about “Threads suck” someone was saying, “Oh he doesn’t know anything. He’s not a real man.” Come on, you idiot. I got a trip to New Zealand and Australia. I got some perks. But it was definitely painful and it takes too long. As Oscar Wilde said of socialism, “It takes too many evenings.”_
