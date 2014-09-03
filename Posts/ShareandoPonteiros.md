# Shareando Ponteiros

Apesar de j� ter palestrado algumas vezes sobre Boost e STL, acho que eu nunca escrevi muito sobre esses assuntos no blogue. Acho que o tamanho dessas bibliotecas assusta um pouco. Mas temos que come�ar de algum lugar, certo? E j� que � pra come�ar, eu gostaria muito de saber de voc�, programador miser�vel, que passou poucas e boas nesses 10 anos de padr�o 98 brincando com templates quando eles ainda estavam em beta: se fosse para melhorar um aspecto da sua vida de c�digo, qual seria? Qual � aquela coisa que te atormenta como insetos vidrados no seu monitor noite adentro?

Que tal aloca��o de mem�ria e ponteiros? Vamos matar dois coelhos com um template s�?

## A triste realidade do c�digo legado

"Ah, mas tem que usar alguma biblioteca bizarra com milh�es de depend�ncias e que vai quebrar todo o fonte aqui da empresa. Sem contar que vai ter que passar de novo pelos unit tests, vai dar erro de compila��o, a LIB XPTO n�o funciona sem dar tr�s pulinhos virado para a cafeteira e..."

Cada caso � um caso, existe o melhor dos mundos e o pior. Mas (quase) todos t�m solu��o. Mesmo que tudo que voc� tenha dispon�vel seja um bartante e um clipe, podemos tentar alguma m�gica/gambiarra/adapta��o t�cnica. Vamos ver os casos mais comuns:

## Aqui no trampo n�o tem frescura: posso usar C++11 (acho que at� 14, 17, 34...), Visual Studio mais novo, Windows 9

Um cen�rio perfeito para come�ar. A �nica coisa que voc� precisa fazer em seus novos projetos e refatora��es � incluir um �nico cabe�alho:

[github,../Code/ShareandoPonteiros/include-shared-ptr.cpp,cpp]

E pronto! Se abriu um mundo m�gico onde as aloca��es ser�o compartilhadas entre fun��es sem se perder quem deleta o qu�. N�o precisa nem checar se o ponteiro � nulo, basta alocar direto e jogar para dentro do nosso mais novo smart pointer da STL:

[github,../Code/ShareandoPonteiros/usando-shared-ptr.cpp,cpp]

E pronto: voc� nunca mais vai precisar se preocupar com quem deleta o ponteiro, nem quantas c�pias desse ponteiro andam por a�. O shared_ptr da STL, assim como a vers�o que j� tem faz um tempo no boost, mant�m um contador de refer�ncia para cada c�pia do objeto que mant�m o mesmo ponteiro "dentro de si". S� quando esse contador chegar a zero, ou seja, n�o h� mais ningu�m referenciando essa regi�o da mem�ria, o ponteiro � deletado.

O std::shared_ptr funciona desde o SP1 do Visual Studio 2010. Sem Service Pack ou em vers�es mais antigas pode haver dispon�vel no namespace tr1, resqu�cio de quando esse novo padr�o ainda estava em defini��o.

## Aqui no trampo vivemos na era pr�-jur�ssica, onde pessoas mais velhas torcem o nariz quando veem um tal de template.

Vou imaginar que voc� usa o Visual Studio 2003, um dos primeiros da safra ".NET", que, mais uma vez, N�O TEM QUALQUER RELA��O COM C++ .NET.

Bem, nesse caso, "welcome... to the desert... of the double":

    ------ Build started: Project: VS2003, Configuration: Debug Win32 ------
    Compiling...
    usando-shared-ptr.cpp
    shared-ptr.cpp(15) : error C2039: 'shared_ptr' : is not a member of 'std'
    shared-ptr.cpp(15) : error C2143: syntax error : missing ';' before '<'
    shared-ptr.cpp(18) : error C2146: syntax error : missing ';' before identifier 'CreatePerson'
    shared-ptr.cpp(18) : error C2501: 'PersonRef' : missing storage-class or type specifiers
    shared-ptr.cpp(20) : error C2064: term does not evaluate to a function taking 1 arguments
    shared-ptr.cpp(20) : warning C4508: 'CreatePerson' : function should return a value; 'void' return type assumed
    shared-ptr.cpp(23) : error C2146: syntax error : missing ')' before identifier 'person'
    shared-ptr.cpp(23) : error C2182: 'GetName' : illegal use of type 'void'
    shared-ptr.cpp(23) : error C2059: syntax error : ')'
    shared-ptr.cpp(24) : error C2143: syntax error : missing ';' before '{'
    shared-ptr.cpp(24) : error C2447: '{' : missing function header (old-style formal list?)
    shared-ptr.cpp(28) : error C2146: syntax error : missing ')' before identifier 'person'
    shared-ptr.cpp(28) : error C2182: 'GetSurName' : illegal use of type 'void'
    shared-ptr.cpp(28) : error C2059: syntax error : ')'
    shared-ptr.cpp(29) : error C2143: syntax error : missing ';' before '{'
    shared-ptr.cpp(29) : error C2447: '{' : missing function header (old-style formal list?)
    shared-ptr.cpp(33) : error C2146: syntax error : missing ')' before identifier 'person'
    shared-ptr.cpp(33) : error C2182: 'GetAge' : illegal use of type 'void'
    shared-ptr.cpp(33) : error C2059: syntax error : ')'
    shared-ptr.cpp(34) : error C2143: syntax error : missing ';' before '{'
    shared-ptr.cpp(34) : error C2447: '{' : missing function header (old-style formal list?)
    shared-ptr.cpp(38) : error C2146: syntax error : missing ')' before identifier 'person'
    shared-ptr.cpp(38) : error C2182: 'GetPhone' : illegal use of type 'void'
    shared-ptr.cpp(38) : error C2059: syntax error : ')'
    shared-ptr.cpp(39) : error C2143: syntax error : missing ';' before '{'
    shared-ptr.cpp(39) : error C2447: '{' : missing function header (old-style formal list?)
    shared-ptr.cpp(43) : error C2146: syntax error : missing ')' before identifier 'person'
    shared-ptr.cpp(43) : error C2182: 'PrintPerson' : illegal use of type 'void'
    shared-ptr.cpp(43) : error C2059: syntax error : ')'
    shared-ptr.cpp(44) : error C2143: syntax error : missing ';' before '{'
    shared-ptr.cpp(44) : error C2447: '{' : missing function header (old-style formal list?)
    shared-ptr.cpp(53) : error C2146: syntax error : missing ';' before identifier 'person'
    shared-ptr.cpp(53) : error C2065: 'person' : undeclared identifier
    shared-ptr.cpp(54) : error C3861: 'person': identifier not found, even with argument-dependent lookup
    shared-ptr.cpp(55) : error C3861: 'person': identifier not found, even with argument-dependent lookup
    shared-ptr.cpp(56) : error C3861: 'person': identifier not found, even with argument-dependent lookup
    shared-ptr.cpp(57) : error C3861: 'person': identifier not found, even with argument-dependent lookup
    shared-ptr.cpp(58) : error C3861: 'person': identifier not found, even with argument-dependent lookup
    VS2003 - 37 error(s), 1 warning(s)

Pois �, 37 erros. Depois perguntam por que as pessoas ficam com medo de programar em C++...

Por�m, a corre��o � mais simples do que parece: baixar o [boost](http://www.boost.org/) e trocar o nome do namespace.

[github,../Code/ShareandoPonteiros/usando-boost-shared-ptr.cpp,cpp]

_ATEN��O! Nos meus testes a �nica vers�o funcionando com o VS2003 foi a 1.47. Mas j� � alguma coisa_

## Aqui n�o tem jeito, n�o. O pessoal olha feio quando usamos classe e a palavra boost � proibida de ser usada no escrit�rio.

N�o existe situa��o dif�cil que n�o possa piorar. Por�m, mesmo nesse caso ainda h� algo a se fazer, j� que smart pointer utilizam mecanismos existentes na linguagem C++ desde os prim�rdios (ou bem pr�ximo disso). Tudo que voc� precisa para criar seu pr�prio shared_ptr � do construtor padr�o, do destrutor padr�o, do construtor de c�pia e dos operadores de atribui��o e ponteiro. E, claro, n�o se esque�a de usar template se for permitido. Se n�o for, a coisa complica, mas n�o se torna imposs�vel.

[github,../Code/ShareandoPonteiros/shared_ptr.h,cpp]

[github,../Code/ShareandoPonteiros/usando-meu-shared-ptr.cpp,cpp]

E � isso. A li��o de hoje �: quem quer, arruma um jeito. Quem n�o quer, uma desculpa.
