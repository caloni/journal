# O novo "como n�o dar step into" do Visual Studio 2012/13

Toda vez que instalo um Visual Studio novo e come�o a depurar sempre surge a necessidade de faz�-lo calar a boca nos step intos da STL, Boost, ATL e coisas-que-sei-que-n�o-vai-dar-pau. (Obviamente, quando d� pau, preciso ir no disassembly e cutucar a STL para ela me entregar qual o problema com o meu cont�iner.)

Nas edi��es antigas da IDE (at� o 2010) existia uma configura��o no registro para isso. Desde o Visual Studio 2012 isso mudou, e agora existe um arquivo em _%programfiles(x86)%\Microsoft Visual Studio 11(ou12).0\Common7\Packages\Debugger\Visualizers_ chamado default.natstepfilter (gostei do detalhe do "nat": "nat thou step into, little bestard!"). Ele � um XML que j� vem preenchido com algumas op��es interessante:

[github,Code/NatStepInto/natstepinto.xml,xml]

Podemos simplesmente adicionar mais duas op��es para o parzinho STL/Boost:

[github,Code/NatStepInto/natstepinto-add.xml,xml]

A boa nova, pelo menos para o Visual Studio 2013, � que agora � poss�vel, se quisermos, entrar nas fun��es que ser�o ignoradas:

<a href="https://www.flickr.com/photos/120157483@N04/14786101612/" title="Step Into Specific no Visual Studio 2013"><img src="https://farm3.staticflickr.com/2933/14786101612_688e12a363_c.jpg" alt="Step Into Specific no Visual Studio 2013"></a>

Eu n�o sei qual vai ser a pr�xima novidade do step into, mas para mim, j� est� bem �timo.
