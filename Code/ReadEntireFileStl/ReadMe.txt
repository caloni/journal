Antigamente, em meus tempos áureos da programação C++/STL insana, lia um arquivo usando stream em duas lias: abrir o arquivo e copiar. Eu usava alguma coisa como backinserter ou algo que o valha junto com um istreambuf_iterator. Coisas de programador STL no estágio usuário de droga pesada.

No entanto, esse tempo passou e esqueci como usar o tal dos iterators e backinserters, o que me fez gastar hoje um tempo precioso tentando ajudar um amigo a fazer o mesmo (além de dezenas de googadas sem sentido). No entanto, no meio de tanta pesquisa, achamos os seguintes exemplos igualmente insanos:

http://insanecoding.blogspot.com.br/2011/11/how-to-read-in-file-in-c.html

Isso não adiantou muito até que descobri uma maneira de transformar um exemplo da maneira com que precisávamos: inserir um #include "stdafx.h" em um arquivo sem usar um temporário:

[github, ReadEntireFileStl/ReadEntireFileStl.cpp,cpp]

