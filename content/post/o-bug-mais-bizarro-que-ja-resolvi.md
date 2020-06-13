---
categories: [ "code" ]
date: "2020-05-10"
tags: [ "draft", "debug" ]
title: "O Bug Mais Bizarro Que Já Resolvi"
---
Máquina IBM velha e empoeirada, criptografia blowfish, assembly 16 bits, programa residente e nenhum depurador funcionando. Tudo o que eu tinha era o conhecimento que ia adquirindo do sistema, e talvez um pouco de imaginação, naquela fatídica semana de abril de 2008. Isolado em uma sala, tudo que eu tinha em volta eram papéis com anotações que ia fazendo conforme observava algo que poderia ser uma pista. Qualquer coisa servia. Do outro lado da sala, uma estagiária recém-chegada na empresa observava de longe, talvez com uma certa curiosidade, talvez medo, aquele rapaz ligar e desligar um desktop empoeirado enquanto a cada vez que apertava o botão de Power ele olhava fixamente para a tela por uma, às vezes duas horas seguidas. Às vezes ele ficava a manhã inteira observando um único e singelo boot na câmera mais lenta possível. Cada mov demorava alguns minutos de reflexão.

Tudo isso começou com O cara do suporte, um sujeito bonachão que parecia atrair bugs para nossos sistemas só de olhar para eles. Não eram os piores bugs, mas com certeza os mais bizarros. Bizarro no sentido de que você nunca imaginaria um erro desses ocorrendo. Era como se existisse um bug na própria Matrix. Como se um gato aparecesse duas vezes seguidas, mas esse gato preto sequer existia no cenário que ele encontrou dessa vez.

O sujeito chegou na nossa sala de desenvolvimento falando dessa máquina que tinha acabado de chegar do cliente. Haviam instalado a criptografia de disco, mas esta não é uma história em que dados serão perdidos, pois o Windows ainda mostrava o seu logo esvoaçante segundos depois de ligarmos o velho desktop. O problema era que se você desligasse essa máquina que acabou de ligar e ligasse novamente, segundos depois nada mais acontecia. Tela preta. Nada de logo esvoaçante nem cursor piscando. O disco nem se mexia.

Mas o bizarro não era isso, mas o que vinha depois. Você desligava a pobre máquina novamente. Apertava o botão de Power. E ela subitamente ligava, com tudo no lugar: logo do Windows, barulhinho irritante da sua tela de boas vindas e o disco do HD magnético piscando freneticamente. Tudo certo mais uma vez na terra do Tio Bill.

Então o bug era esse: nos boots ímpares a máquina não bootava. Nos boots pares não havia nada de errado (ou vice-versa). Antes que você comece a confabular o que poderia ser, um cacoete que todos nós, programadores, costumamos ter, não há relação com energia ou memória RAM. Você podia desligar a máquina e tirar da tomada. Ir tomar um café. Uma hora depois coloca a tomada de novo e a liga. A bendita não funciona. Tire a tomada novamente. Mais um café. Desenergizada novamente, botão de Power. E tudo estava certinho.

A criptografia desse sistema operava em dois níveis, necessários naquela época. O PC é uma monstruosidade construída em camadas legadas uma em cima da outra. Abaixo de tudo existe a BIOS que controla todo mundo. Até um certo ponto, pelo menos. O que importa é que nesse primeiro momento não existe sistema operacional. Não existe a querida proteção de memória que os SOs implementam (com a ajuda da arquitetura) para isolar os programas, onde qualquer violação de memória é tratada graciosamente com uma mensagem de erro. Não, mano. Aqui é o modo real. Fica esperto, que se um ponteiro ficar doido você vai levar tiro pra tudo quanto é lado. Ou como diria Morpheus: "Welcome... to the desert... of the real."

Nesse ambiente pesadão e promíscuo, onde as memórias se encostam e trocam de valores sem qualquer pudor, programas residentes se mantém em memória através do hook de interrupções. Interrupções são as funções originais da BIOS, ponteiros de funções com código carregado da sua memória, e um hook é se colocar na frente dessas funções trocando o ponteiro. Então, por exemplo, se um programa roda e consegue sobrescrever o endereço da interrupção responsável por escrever na tela, esse programa pode ligar e desligar pixels que o programa original nem imagina. Esse seria o antepassado do gemidão, versão ASCII Art.

No caso de um programa de criptografia de disco as interrupções mais importantes são a de disco. E como é importante para o sistema operacional que pretende conseguir subir, ele deve conseguir ler seus dados do disco, mesmo criptografados. Esses dados precisam ser descriptografados antes que exista um driver de criptografia no ar. É aí que entra o programa residente desse sistema que deu problema.

É claro que contando isso para vocês a posteriori parece mais fácil, mas meu primeiro instinto foi espetar o WinDbg nessa máquina, para rapidamente descobrir que não existia kernel do Windows para ser depurado. O Windows nem conseguiu subir ainda, quanto mais deixar as pessoas depurarem ele. Então a solução para depuração foi o SoftIce 16 bits, que funciona até que bem quando você não tem outros programas residentes querendo espaço no disco. Como o programa de criptografia que instalava um hook na int13 (essa é a interrupção de disco). Nesse caso o depurador de modo real travava nas primeiras interrupções. Não havia memória o suficiente ou as chamadas das ints entravam em conflito. De qualquer forma, o barato ficava loko.

Então o jeito foi usar o debug.com. Para os mais novinhos, este era um programa que vinha no pacote MS-DOS e em alguns Windows mais velhos que consistia em um depurador de modo real). Era possível carregar um segmento de um arquivo ou da memória real para este depurador e ele seguia passo a passo para você a execução do programa. Em assembly de modo real, claro.

Esse foi o jeito que eu consegui ir entendendo o fluxo de execução, pois eram muitos valores e variáveis. Eventualmente o debug.com também travava, mas isso não importava tanto, pois era possível ir mapeando seu funcionamento aos poucos, anotando as descoberta uma a uma em um pedaço de papel. Uma técnica que pode ser interessante se você se encontrar em tal situação é escrever as ints 3 (interrupção de brakpoint) diretamente na memória do programa e deixar ela ser ativada para depois que capotar sobrescrever com o código antigo. Eventualmente isso também travava. Daí nesse momento o jeito era fingir que estava tudo bem e continuar a execução de um outro ponto, anotando em um papel o estado dos registradores e da memória até o momento, para depois ir ligando os pontos.

Depois de alguns dias nesse modus operandi, em que o mundo externo não importava mais, e que eu só enxergava movs, pushs e pops na hora do café, comecei a vislumbrar a possibilidade de existir um bug no código do algoritmo de criptografia. Esse algoritmo é o Blowfish), um cifrador simétrico em bloco. Seu funcionamento é basicamente pegar um bloco de dados a serem criptografados, aplicar uma chave, e cuspir o mesmo tamanho do bloco de volta. Aplicando a mesma chave novamente obtém-se o bloco original. Não lembro como tive esse insight, mas essa alternância simétrica fazia tocar os sinos na minha cabeça de que o bug bizarro dos boots ímpares e pares poderia estar relacionado. Só não sabia ainda como.

Pois bem: bora aprender como funciona esse algoritmo passo a passo, pois o código usado no sistema estava obviamente em assembly. Não é um código difícil em C, mas extenso em Assembly. De qualquer forma, tudo é possível se você está trancado em uma sala sem ninguém para importunar. Tudo que você precisa é de tempo e paciência. E café. Não se esqueça disso.

Mas, enfim, não conseguirei descrever melhor o cenário em que eu estava naquela semana. Tudo que me lembro é que de fato foi uma semana de 40 ou mais horas. E tornando agora esta história que já está longa em algo mais curto, segue o commit que corrigiu essa bagaça:

    commit 56f510... (tag: 1.49)
    Author: root <root@desenv05x>
    Date: Mon Apr 21 03:26:50 2008 -0300
    
    Corrigido bug na encriptacao por 
        Blowfish em assembly.
    
    O problema ocorria por causa da 
    inversao desnecessaria do iv, e 
    existia apenas na funcao de 
    encriptacao (escritas no disco), 
    funcionando normalmente, portanto, 
    na desencriptacao (leituras 
    no disco).
    
    Esse bug provavelmente afetava 
    somente boots de sistemas que 
    tentavam escrever no disco 
    antes de entrar o driver.
    
    -  xor     4[si], eax
    +  xor     [si] eax
       add     bx, #4
       seg     es
       mov   eax, [bx]
    -  xor     [si], eax
    +  xor     4[si], eax
    ...
    -  xchg    eax, 4[si]
    -  bswap   eax
       mov     [si], eax
    +  mov     eax, 4[si]
    +  bswap   eax
    +  mov     4[si], eax

O IV, ou Initialization Vector, é um array de bytes usado em algoritmos criptográficos para diminuir a previsibilidade da série de bytes resultantes do algoritmo. Sem o IV pode-se usar força bruta com várias chaves até encontrar a certa. Com o IV, que é alterado de maneira previsível, mas difícil de rastrear, a mesma chave gera séries de bytes completamente diferentes, impedindo esse tipo de ataque.

O que estava acontecendo nesse caso para o boot estar intermitente era que, como comentado no commit, as escritas em disco durante o boot gravavam a série de bytes com um IV invertido. Isso na hora de ler entregaria os dados errados, obviamente, e a máquina não bootaria. Porém, pelo blowfish ser simétrico, e pelo boot ser o mesmo (com os mesmos dados no disco), uma segunda escrita feita em um segundo boot inverteria o IV já invertido, gravando os dados originalmente invertidos corretamente, e a vida seguia feliz (até a música de boas vindas do Windows).

Descrevendo a descoberta desse bug hoje, 12 anos após o ocorrido, ainda não entendo como consegui descobri-lo, mas isso exigiu tanta concentração que me lembro com um prazer indescritível de ter sido capaz de fazê-lo. Todo o tempo despendindo se tornou uma marca de felicidade em minha memória, gravada em meu HD temporário desta vida. Lembrarei dela com carinho, e como ela está criptografada também, entenderei que em alguns momentos ela irá soar amarga, mas em vários outros irei ter certeza de ter sido um feito e tanto para um ser humano entender uma máquina em seus detalhes mais obscuros. Essa é a verdadeira felicidade desta profissão.
