# Desmontando o Aulete Digital

Revendo os comentários a respeito do conversor de [Houaiss para Babylon](http://www.caloni.com.br/blog/archives/tag/houaiss2babylon) (e derivados como [GoldenDict](http://goldendict.org/dictionaries.php) e [Kindle](http://www.caloni.com.br/blog/archives/houaiss-no-kindle)) me veio novamente o nome do projeto Aulete Digital, que é uma iniciativa em conjunto com a **Editora Lexikon** (provavelmente os direitos) e a **MGB Informática** (o software). Também participam do projeto os próprios leitores do dicionário clássico Caldas Aulete, cuja última atualização data dos anos 80 e foi mantido por Francisco J. Caldas Aulete e Antonio Lopes dos Santos Valente por muitas décadas. O projeto tem por objetivo atualizar o dicionário, mas manter as definições originais para consulta. O Houaiss já é um dicionário de respeito (mais que o Novo Aurélio), mas a quantidade de verbetes do Aulete Digital já está chegando à quantidade expressiva de 212.368 definições.

Como eu sei disso? Ora, eu o baixei =)

## Voltando às reversa da engenhoca da parafuseta

O conteúdo do dicionário está disponível [online gratuitamente](http://aulete.uol.com.br/), mas é possível fazer o [download](http://www.auletedigital.com.br/download.html) de uma versão desktop (Windows) que é apenas uma interface para acesso ao mesmo conteúdo. Podemos averiguar isso facilmente ao instalar o dicionário e ver que os arquivos locais que ele mantém não são nada grandes comparados com mais de 200k definições de palavras.

<a href="https://www.flickr.com/photos/120157483@N04/14060397360/" title="Arquivos do Aulete Digital por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5585/14060397360_269641b632_z.jpg" alt="Arquivos do Aulete Digital"></a>

Portanto, nossa ferramenta de trabalho ideal não é o WinDbg, mas o [WireShark](http://www.wireshark.org/), o que torna tudo incrivelmente mais fácil. Quer ver?

Tudo que temos que fazer é instalar o WireShark e iniciar o monitoramento na conexão usada para acesso à Internet:

<a href="https://www.flickr.com/photos/120157483@N04/14258358864/" title="Iniciando Captura com Wire Shark por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5073/14258358864_a97dedc0c3_z.jpg" alt="Iniciando Captura com Wire Shark"></a>

Realizar uma busca pela definição da palavra "programa" (em respeito à [tradição](http://www.caloni.com.br/blog/archives/conversor-de-houaiss-para-babylon-parte-1)):

<a href="https://www.flickr.com/photos/120157483@N04/14256586642/" title="Pesquisando Definição da palavra programa por wanderleycaloni, no Flickr"><img src="https://farm4.staticflickr.com/3741/14256586642_b68233dcf2_z.jpg" alt="Pesquisando Definição da palavra programa"></a>

Parar o rastreamento do WireShark (ele consome tantos recursos quanto um Process Monitor da vida) e realizar a busca por "programa" no que ele capturou:

<a href="https://www.flickr.com/photos/120157483@N04/14072126950/" title="A busca por um programa no WireShark por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5591/14072126950_13188b03ab.jpg" alt="A busca por um programa no WireShark"></a>

<a href="https://www.flickr.com/photos/120157483@N04/14255420521/" title="Resultado da busca por programa no WireShark por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2904/14255420521_b07b0d79e0_z.jpg" alt="Resultado da busca por programa no WireShark"></a>

O primeiro resultado já foi a definição da palavra, sinal que a busca não é feito usando a palavra como índice. A partir do pacote em que encontramos a definição, vamos caminhando para trás no histórico em busca de um request HTTP (porque sim) "suspeito". Como esse:

<a href="https://www.flickr.com/photos/120157483@N04/14235605376/" title="O request HTTP por wanderleycaloni, no Flickr"><img src="https://farm4.staticflickr.com/3697/14235605376_5c057ba964_z.jpg" alt="O request HTTP"></a>

<a href="https://www.flickr.com/photos/120157483@N04/14072086609/" title="Download por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5483/14072086609_8a7e50d296.jpg" alt="Download"></a>

O resultado é um arquivo de download. O que tem dentro dele?

<a href="https://www.flickr.com/photos/120157483@N04/14255422541/" title="AdiVINhando o conteúdo do arquivo por wanderleycaloni, no Flickr"><img src="https://farm4.staticflickr.com/3734/14255422541_055c565706_z.jpg" alt="AdiVINhando o conteúdo do arquivo"></a>

OK, achamos nosso culpado.

## Parte 16.4: Paciência!

A partir da URI do request é possível localizar qual o índice usado para acertar a palavra. Não é difícil, basta olhar de perto:

    200.155.20.226/bin/NDServerAuleteCGI.exe/DocumentoDoID?NomeBase=DicAulete06&IdUsuario=0&IdVerbete=166962&Palavra=&SelBases=

E o verbete "programa" é o de número 166.962. Se alterarmos a URI e acessarmos o endereço abaixo, acharemos a definição para "a":

    http://200.155.20.226/bin/NDServerAuleteCGI.exe/DocumentoDoID?NomeBase=DicAulete06&IdUsuario=0&IdVerbete=0&Palavra=&SelBases=

Realizando o mesmo procedimento do WireShark para o último verbete (no caso, "zwinglianista") encontramos o número do último verbete, que seria 212365, mas apenas para garantir, avançamos esse número até a página não nos retornar nada, onde chegamos em 212367 (indexado a zero). Esse número, provavelmente, deverá crescer. Porém, antes disso, temos algo importante a fazer:

Baixar todos os 212368 verbetes disponíveis!

E isso é mais fácil do que dizer:

    for /L %i in (0, 1, 212368) do curl "http://200.155.20.226/bin/NDServerAuleteCGI.exe/DocumentoDoID?NomeBase=DicAulete06&IdUsuario=0&IdVerbete=%i&Palavra=&SelBases=" > %i.txt

Porém, é preciso tempo. Muito tempo...

<a href="https://www.flickr.com/photos/120157483@N04/14258358484/" title="Muito, muito, muito tempo depois por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5233/14258358484_b6c74c1e5a_z.jpg" alt="Muito, muito, muito tempo depois"></a>

## Parte 212.367: MUITO tempo depois

Quando o download terminar teremos duas centenas de milhares de arquivos dentro de uma única pasta, então eu recomendo não abrir essa pasta pelo Windows Explorer. Aliás, melhor não abrir essa pasta de jeito nenhum. Melhor focar nos arquivos.

E os arquivos, como já foi possível ver no nosso primeiro download de teste, já possuem a parte textual aberta, ou seja, sem truques de XOR e soma 1B dessa vez. Tudo que precisamos é entender onde termina e começa o texto. O resto pode ser aos poucos interpretado com comparações entre verbetes, etc.

Olhando para o "abacate", por exemplo, já é possível entender através da nossa mente de programador que os primeiros bytes parecem indicar o tamanho do texto que se segue, algo muito comum quando temos uma estrutura de tamanho variável (vide Win32 API):

<a href="https://www.flickr.com/photos/120157483@N04/14258747045/" title="abacate por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5277/14258747045_21d413dfb4_z.jpg" alt="abacate"></a>

Outra coisa simples de se notar é que dois ou mais caracteres de espaço juntos parecem dividir as "partes" em que se define todo o texto da definição:

<a href="https://www.flickr.com/photos/120157483@N04/14072097578/" title="abacate definido por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5578/14072097578_6555ac44ca_z.jpg" alt="abacate definido"></a>

A partir daí fica simples iniciar um conversor para transformar o dicionário em uma versão offline para interfaces como Babylon ou GoldenDict.
