# Estruturas VS Classes: fight!

<a href="https://www.flickr.com/photos/120157483@N04/14250890913/" title="EpicRapBattleStructVcClass por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5118/14250890913_37a06bf7a2_o.jpg" alt="EpicRapBattleStructVcClass"></a>

Uma dúvida besta e importante ao mesmo tempo que muitos iniciantes em C++ possuem é saber qual a diferença entre um objeto declarado como _class_ e um objeto declarado como _struct_. A causa dessa dúvida é uma linguagem que se derivou de outra (C) que não possuía classes, e portanto criou a palavra-chave _class_ para "ficar bonito", pois, na prática, não muda muita coisa. Tomemos como exemplo o código mais simples de todos:

[github,Code/StructVsClass/structvsclass.cpp,cpp]

Ele compila e roda sem problemas:

<a href="https://www.flickr.com/photos/120157483@N04/14230192924/" title="StructVsClass por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2918/14230192924_fd9c2fb490_z.jpg" alt="StructVsClass"></a>

"Estruturalmente" falando, **MinhaEstrutura** e **MinhaClasse** são idênticas, pois são os detalhes de sintaxe que diferem, e diferem pouco. Abrindo o jogo, a única diferença que poderá ser sentida em usar um ou outro é que **structs possuem seus membros públicos por padrão e classes possuem seus membros privados por padrão**. Apenas isso. O resto, nada muda.

Isso pode ser visto quando adicionamos um construtor para nossos tipos de teste:

[github,Code/StructVsClass/structvsclass2.cpp,cpp]

<a href="https://www.flickr.com/photos/120157483@N04/14230273964/" title="StructVsClass-Construtor por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5073/14230273964_89e37e2487_z.jpg" alt="StructVsClass-Construtor"></a>

Antes não havia problemas para **MinhaClasse** porque o construtor padrão criado para ela é público por default. Porém, explicitando no código um construtor e deixando sua privacidade ligada por padrão temos esse erro que NÃO ocorre em **MinhaEstrutura**.

Mas, então, posso criar todas minhas classes usando a palavra-chave struct?

Isso mesmo! Nada lhe obriga tecnicamente a usar class. Porém, assim como nada lhe obriga a usar uma linha para cada comando na linguagem — afinal, todos poderiam estar na mesma linha separados por ponto-e-vírgula — o uso da palavra _struct_ para classes no sentido de "objetos que possuem inteligência, métodos, herança, polimorfismo e outras firulas" não se enquadra nas boas práticas dos programadores C++.

Geralmente uma _struct_ é uma forma de concatenar tipos primitivos e só. Algumas liberdades além disso geralmente são permitidas, mas desencorajadas, como um construtor que inicia os membros da _struct_ com valores-default.

[github,Code/StructVsClass/structvsclass-struct-constructor.cpp,cpp]

<a href="https://www.flickr.com/photos/120157483@N04/14207416246/" title="StructVsClassStructConstructor por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5274/14207416246_60675f681a_z.jpg" alt="StructVsClassStructConstructor"></a>

E, por que não, uma sobrecarga do operador de stream para imprimirmos diretamente os valores de **MinhaEstrutura** para a saída com apenas um comando?

[github,Code/StructVsClass/structvsclass-struct-stream.cpp,cpp]

<a href="https://www.flickr.com/photos/120157483@N04/14043966560/" title="StructVsClassStreams por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2934/14043966560_422ae353d9_z.jpg" alt="StructVsClassStreams"></a>

Enfim, não há nenhum limite que se aplica à uma _struct_ além do bom senso. A criação da palavra _class_ não foi por falta do que fazer. Ela diz claramente que estamos definindo um objeto que contém usos mais adequados à orientação a objetos de C++ do que a programação estruturada de C, e vice-versa. É uma forma de tornar o código mais legível, mas nada do outro mundo. Sabemos, no final das contas, que o compilador trata as duas (quase) da mesma maneira.

Qual será a próxima batalha épica? Você escolhe!
http://youtu.be/zn7-fVtT16k
