# Estruturas VS Classes: fight!

<a href="https://www.flickr.com/photos/120157483@N04/14250890913/" title="EpicRapBattleStructVcClass por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5118/14250890913_37a06bf7a2_o.jpg" alt="EpicRapBattleStructVcClass"></a>

Uma d�vida besta e importante ao mesmo tempo que muitos iniciantes em C++ possuem � saber qual a diferen�a entre um objeto declarado como _class_ e um objeto declarado como _struct_. A causa dessa d�vida � uma linguagem que se derivou de outra (C) que n�o possu�a classes, e portanto criou a palavra-chave _class_ para "ficar bonito", pois, na pr�tica, n�o muda muita coisa. Tomemos como exemplo o c�digo mais simples de todos:

[github,Code/StructVsClass/structvsclass.cpp,cpp]

Ele compila e roda sem problemas:

<a href="https://www.flickr.com/photos/120157483@N04/14230192924/" title="StructVsClass por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2918/14230192924_fd9c2fb490_z.jpg" alt="StructVsClass"></a>

"Estruturalmente" falando, **MinhaEstrutura** e **MinhaClasse** s�o id�nticas, pois s�o os detalhes de sintaxe que diferem, e diferem pouco. Abrindo o jogo, a �nica diferen�a que poder� ser sentida em usar um ou outro � que **structs possuem seus membros p�blicos por padr�o e classes possuem seus membros privados por padr�o**. Apenas isso. O resto, nada muda.

Isso pode ser visto quando adicionamos um construtor para nossos tipos de teste:

[github,Code/StructVsClass/structvsclass2.cpp,cpp]

<a href="https://www.flickr.com/photos/120157483@N04/14230273964/" title="StructVsClass-Construtor por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5073/14230273964_89e37e2487_z.jpg" alt="StructVsClass-Construtor"></a>

Antes n�o havia problemas para **MinhaClasse** porque o construtor padr�o criado para ela � p�blico por default. Por�m, explicitando no c�digo um construtor e deixando sua privacidade ligada por padr�o temos esse erro que N�O ocorre em **MinhaEstrutura**.

Mas, ent�o, posso criar todas minhas classes usando a palavra-chave struct?

Isso mesmo! Nada lhe obriga tecnicamente a usar class. Por�m, assim como nada lhe obriga a usar uma linha para cada comando na linguagem � afinal, todos poderiam estar na mesma linha separados por ponto-e-v�rgula � o uso da palavra _struct_ para classes no sentido de "objetos que possuem intelig�ncia, m�todos, heran�a, polimorfismo e outras firulas" n�o se enquadra nas boas pr�ticas dos programadores C++.

Geralmente uma _struct_ � uma forma de concatenar tipos primitivos e s�. Algumas liberdades al�m disso geralmente s�o permitidas, mas desencorajadas, como um construtor que inicia os membros da _struct_ com valores-default.

[github,Code/StructVsClass/structvsclass-struct-constructor.cpp,cpp]

<a href="https://www.flickr.com/photos/120157483@N04/14207416246/" title="StructVsClassStructConstructor por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5274/14207416246_60675f681a_z.jpg" alt="StructVsClassStructConstructor"></a>

E, por que n�o, uma sobrecarga do operador de stream para imprimirmos diretamente os valores de **MinhaEstrutura** para a sa�da com apenas um comando?

[github,Code/StructVsClass/structvsclass-struct-stream.cpp,cpp]

<a href="https://www.flickr.com/photos/120157483@N04/14043966560/" title="StructVsClassStreams por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2934/14043966560_422ae353d9_z.jpg" alt="StructVsClassStreams"></a>

Enfim, n�o h� nenhum limite que se aplica � uma _struct_ al�m do bom senso. A cria��o da palavra _class_ n�o foi por falta do que fazer. Ela diz claramente que estamos definindo um objeto que cont�m usos mais adequados � orienta��o a objetos de C++ do que a programa��o estruturada de C, e vice-versa. � uma forma de tornar o c�digo mais leg�vel, mas nada do outro mundo. Sabemos, no final das contas, que o compilador trata as duas (quase) da mesma maneira.

Qual ser� a pr�xima batalha �pica? Voc� escolhe!
http://youtu.be/zn7-fVtT16k
