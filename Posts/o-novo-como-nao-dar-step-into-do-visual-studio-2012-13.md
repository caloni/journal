# O novo "como não dar step into" do Visual Studio 2012/13

Toda vez que instalo um Visual Studio novo e começo a depurar sempre surge a necessidade de fazê-lo calar a boca nos step intos da STL, Boost, ATL e coisas-que-sei-que-não-vai-dar-pau. (Obviamente, quando dá pau, preciso ir no disassembly e cutucar a STL para ela me entregar qual o problema com o meu contêiner.)

Nas edições antigas da IDE (até o 2010) existia uma configuração no registro para isso. Desde o Visual Studio 2012 isso mudou, e agora existe um arquivo em _%programfiles(x86)%\Microsoft Visual Studio 11(ou12).0\Common7\Packages\Debugger\Visualizers_ chamado default.natstepfilter (gostei do detalhe do "nat": "nat thou step into, little bestard!"). Ele é um XML que já vem preenchido com algumas opções interessante:

[github,Code/NatStepInto/natstepinto.xml,xml]

Podemos simplesmente adicionar mais duas opções para o parzinho STL/Boost:

[github,Code/NatStepInto/natstepinto-add.xml,xml]

A boa nova, pelo menos para o Visual Studio 2013, é que agora é possível, se quisermos, entrar nas funções que serão ignoradas:

<a href="https://www.flickr.com/photos/120157483@N04/14786101612/" title="Step Into Specific no Visual Studio 2013"><img src="https://farm3.staticflickr.com/2933/14786101612_688e12a363_c.jpg" alt="Step Into Specific no Visual Studio 2013"></a>

Eu não sei qual vai ser a próxima novidade do step into, mas para mim, já está bem ótimo.
