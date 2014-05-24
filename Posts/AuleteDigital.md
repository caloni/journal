# Desmontando o Aulete Digital

Revendo os coment�rios a respeito do conversor de [Houaiss para Babylon](http://www.caloni.com.br/blog/archives/tag/houaiss2babylon) (e derivados como [GoldenDict](http://goldendict.org/dictionaries.php) e [Kindle](http://www.caloni.com.br/blog/archives/houaiss-no-kindle)) me veio novamente o nome do projeto Aulete Digital, que � uma iniciativa em conjunto com a **Editora Lexikon** (provavelmente os direitos) e a **MGB Inform�tica** (o software). Tamb�m participam do projeto os pr�prios leitores do dicion�rio cl�ssico Caldas Aulete, cuja �ltima atualiza��o data dos anos 80 e foi mantido por Francisco J. Caldas Aulete e Antonio Lopes dos Santos Valente por muitas d�cadas. O projeto tem por objetivo atualizar o dicion�rio, mas manter as defini��es originais para consulta. O Houaiss j� � um dicion�rio de respeito (mais que o Novo Aur�lio), mas a quantidade de verbetes do Aulete Digital j� est� chegando � quantidade expressiva de 212.368 defini��es.

Como eu sei disso? Ora, eu o baixei =)

## Voltando �s reversa da engenhoca da parafuseta

O conte�do do dicion�rio est� dispon�vel [online gratuitamente](http://aulete.uol.com.br/), mas � poss�vel fazer o [download](http://www.auletedigital.com.br/download.html) de uma vers�o desktop (Windows) que � apenas uma interface para acesso ao mesmo conte�do. Podemos averiguar isso facilmente ao instalar o dicion�rio e ver que os arquivos locais que ele mant�m n�o s�o nada grandes comparados com mais de 200k defini��es de palavras.

<a href="https://www.flickr.com/photos/120157483@N04/14060397360/" title="Arquivos do Aulete Digital por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5585/14060397360_269641b632_z.jpg" alt="Arquivos do Aulete Digital"></a>

Portanto, nossa ferramenta de trabalho ideal n�o � o WinDbg, mas o [WireShark](http://www.wireshark.org/), o que torna tudo incrivelmente mais f�cil. Quer ver?

Tudo que temos que fazer � instalar o WireShark e iniciar o monitoramento na conex�o usada para acesso � Internet:

<a href="https://www.flickr.com/photos/120157483@N04/14258358864/" title="Iniciando Captura com Wire Shark por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5073/14258358864_a97dedc0c3_z.jpg" alt="Iniciando Captura com Wire Shark"></a>

Realizar uma busca pela defini��o da palavra "programa" (em respeito � [tradi��o](http://www.caloni.com.br/blog/archives/conversor-de-houaiss-para-babylon-parte-1)):

<a href="https://www.flickr.com/photos/120157483@N04/14256586642/" title="Pesquisando Defini��o da palavra programa por wanderleycaloni, no Flickr"><img src="https://farm4.staticflickr.com/3741/14256586642_b68233dcf2_z.jpg" alt="Pesquisando Defini��o da palavra programa"></a>

Parar o rastreamento do WireShark (ele consome tantos recursos quanto um Process Monitor da vida) e realizar a busca por "programa" no que ele capturou:

<a href="https://www.flickr.com/photos/120157483@N04/14072126950/" title="A busca por um programa no WireShark por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5591/14072126950_13188b03ab.jpg" alt="A busca por um programa no WireShark"></a>

<a href="https://www.flickr.com/photos/120157483@N04/14255420521/" title="Resultado da busca por programa no WireShark por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2904/14255420521_b07b0d79e0_z.jpg" alt="Resultado da busca por programa no WireShark"></a>

O primeiro resultado j� foi a defini��o da palavra, sinal que a busca n�o � feito usando a palavra como �ndice. A partir do pacote em que encontramos a defini��o, vamos caminhando para tr�s no hist�rico em busca de um request HTTP (porque sim) "suspeito". Como esse:

<a href="https://www.flickr.com/photos/120157483@N04/14235605376/" title="O request HTTP por wanderleycaloni, no Flickr"><img src="https://farm4.staticflickr.com/3697/14235605376_5c057ba964_z.jpg" alt="O request HTTP"></a>

<a href="https://www.flickr.com/photos/120157483@N04/14072086609/" title="Download por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5483/14072086609_8a7e50d296.jpg" alt="Download"></a>

O resultado � um arquivo de download. O que tem dentro dele?

<a href="https://www.flickr.com/photos/120157483@N04/14255422541/" title="AdiVINhando o conte�do do arquivo por wanderleycaloni, no Flickr"><img src="https://farm4.staticflickr.com/3734/14255422541_055c565706_z.jpg" alt="AdiVINhando o conte�do do arquivo"></a>

OK, achamos nosso culpado.

## Parte 16.4: Paci�ncia!

A partir da URI do request � poss�vel localizar qual o �ndice usado para acertar a palavra. N�o � dif�cil, basta olhar de perto:

    200.155.20.226/bin/NDServerAuleteCGI.exe/DocumentoDoID?NomeBase=DicAulete06&IdUsuario=0&IdVerbete=166962&Palavra=&SelBases=

E o verbete "programa" � o de n�mero 166.962. Se alterarmos a URI e acessarmos o endere�o abaixo, acharemos a defini��o para "a":

    http://200.155.20.226/bin/NDServerAuleteCGI.exe/DocumentoDoID?NomeBase=DicAulete06&IdUsuario=0&IdVerbete=0&Palavra=&SelBases=

Realizando o mesmo procedimento do WireShark para o �ltimo verbete (no caso, "zwinglianista") encontramos o n�mero do �ltimo verbete, que seria 212365, mas apenas para garantir, avan�amos esse n�mero at� a p�gina n�o nos retornar nada, onde chegamos em 212367 (indexado a zero). Esse n�mero, provavelmente, dever� crescer. Por�m, antes disso, temos algo importante a fazer:

Baixar todos os 212368 verbetes dispon�veis!

E isso � mais f�cil do que dizer:

    for /L %i in (0, 1, 212368) do curl "http://200.155.20.226/bin/NDServerAuleteCGI.exe/DocumentoDoID?NomeBase=DicAulete06&IdUsuario=0&IdVerbete=%i&Palavra=&SelBases=" > %i.txt

Por�m, � preciso tempo. Muito tempo...

<a href="https://www.flickr.com/photos/120157483@N04/14258358484/" title="Muito, muito, muito tempo depois por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5233/14258358484_b6c74c1e5a_z.jpg" alt="Muito, muito, muito tempo depois"></a>

## Parte 212.367: MUITO tempo depois

Quando o download terminar teremos duas centenas de milhares de arquivos dentro de uma �nica pasta, ent�o eu recomendo n�o abrir essa pasta pelo Windows Explorer. Ali�s, melhor n�o abrir essa pasta de jeito nenhum. Melhor focar nos arquivos.

E os arquivos, como j� foi poss�vel ver no nosso primeiro download de teste, j� possuem a parte textual aberta, ou seja, sem truques de XOR e soma 1B dessa vez. Tudo que precisamos � entender onde termina e come�a o texto. O resto pode ser aos poucos interpretado com compara��es entre verbetes, etc.

Olhando para o "abacate", por exemplo, j� � poss�vel entender atrav�s da nossa mente de programador que os primeiros bytes parecem indicar o tamanho do texto que se segue, algo muito comum quando temos uma estrutura de tamanho vari�vel (vide Win32 API):

<a href="https://www.flickr.com/photos/120157483@N04/14258747045/" title="abacate por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5277/14258747045_21d413dfb4_z.jpg" alt="abacate"></a>

Outra coisa simples de se notar � que dois ou mais caracteres de espa�o juntos parecem dividir as "partes" em que se define todo o texto da defini��o:

<a href="https://www.flickr.com/photos/120157483@N04/14072097578/" title="abacate definido por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5578/14072097578_6555ac44ca_z.jpg" alt="abacate definido"></a>

A partir da� fica simples iniciar um conversor para transformar o dicion�rio em uma vers�o offline para interfaces como Babylon ou GoldenDict.
