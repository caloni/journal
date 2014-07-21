# Integrando BitBucket/GitHub com Trello

Eu nem acredito que estou escrevendo sobre desenvolvimento web, mas como foi algo que me fez dedicar algumas horas do meu fim-de-semana, e n�o encontrei facilmente uma solu��o j� feita, acredito que pode ser �til para mais algu�m que usa Trello e GitHub (ou BitBucket).

Mas o que � Trello? Basicamente � um TodoList feito da maneira mais inteligente poss�vel: uma lista de listas de listas! Os espa�os, ou desktops, onde voc� organiza suas tarefas s�o chamados de Boards. Em cada board vivem L listas, e em cada lista vivem C cards. Cada card pode conter coment�rios, hist�rico de mudan�as, labels, checklists, due dates e todas as tranqueiras que geralmente existe em uma lista de tarefas. � um sistema online, desenvolvido pela empresa do Joel Spolsky (o mesmo do excelente blogue de programador [Joel on Software](http://www.joelonsoftware.com/) (ou em [portugu�s](http://brazil.joelonsoftware.com/), e que cont�m algo que eu adoro em sistemas web: atalhos!

<a href="https://www.flickr.com/photos/120157483@N04/14518444517/" title="Atalhos do Trello"><img src="https://farm6.staticflickr.com/5579/14518444517_f550bb7bcb_o.png" alt="Atalhos do Trello"></a>

A ideia que tive foi usar os webhooks dos saites de reposit�rios de fontes para permitir comentar dentro dos cards o commit que foi feito, sua mensagem e o linque para o commit. OK, mas por que n�o usar o sistema de issues dos j� feitos pra isso GitHub e BitBucket? Ele j� faz isso muito melhor. De fato. Por�m, fica espalhado pelos reposit�rios, e n�o � sempre que uma tarefa envolve c�digo (comprar p�o, por exemplo). Al�m do mais, praticamente qualquer servi�o desses oferece hooks para a integra��o de outros projetos/servi�os, ent�o se um dia nascer mais um sistema de controle de fonte ou mais um saite que organiza essas tralhas haver� um hook e consequentemente mais uma adapta��o do meu c�digo PHP.

E por que PHP? Bom, PHP � uma linguagem f�cil de mexer (se parece com C, mas � um script) e praticamente qualquer servidor web do universo, mesmo o mais baratinho, vem com o pacote Apache + PHP (e geralmente uma base MySql). Dessa forma, � uma solu��o que pode ser implantada f�cil e rapidamente.

## Comentando no Trello

Vamos come�ar pelo mais dif�cil que o resto vai f�cil: comentar pela API do Trello. Sua [API � beta](https://trello.com/docs/), assim como sua documenta��o, ent�o tive arrancar significado inexistente em seu help, mas acabou funcionando. Como qualquer API web, voc� precisa de uma chave, segredo e a permiss�o do usu�rio. Com essa permiss�o � poss�vel comentar em todas as boards que esse usu�rio espec�fico tem acesso.

Pelo menos a parte de [gera��o de chave/segredo](� simples). Se voc� clicou nesse linque, j� conseguiu gerar uma =).

Depois disso, mesmo nessa p�gina j� � poss�vel conseguir uma chave de acesso para o seu usu�rio.

<a href="https://www.flickr.com/photos/120157483@N04/14704960285/" title="Pedindo autoriza��o para o Trello"><img src="https://farm4.staticflickr.com/3880/14704960285_af9a772c2e_o.png" alt="Pedindo autoriza��o para o Trello"></a>

Para comentar em PHP, basta usar alguns m�todos:

..\Code\GitHubBitBucketTrelloIntegration\commentOnTrello.php

As informa��es _AQUI_VAI_SUA_CHAVE_ e _AQUI_VAI_SEU_TOKEN_DE_ACESSO_ voc� j� obteve no linque de gera��o de key/secret. J� o _ID_DO_CARD_ � algo que depende de em qual lista seu card est�, mas felizmente tamb�m existe um shortlink �nico e imut�vel para cada card no sistema:

<a href="https://www.flickr.com/photos/120157483@N04/14518342338/" title="ID �nico de um Card"><img src="https://farm4.staticflickr.com/3858/14518342338_6f134a993d_o.png" alt="ID �nico de um Card"></a>

Basta usar o ID em Base64-ou-o-que-o-valha no lugar de _ID_DO_CARD_ que j� estamos OK.

<a href="https://www.flickr.com/photos/120157483@N04/14724898543/" title="Hello, World!"><img src="https://farm3.staticflickr.com/2927/14724898543_8b758cc7ac_o.png" alt="Hello, World!"></a>

Muito bem. Primeira parte da miss�o conclu�da.

## Terminando com GitHub

Como o GitHub � um dos servi�os de reposit�rio de fontes mais famoso, vamos come�ar por ele. Basicamente voc� deve ir no seu reposit�rio do cora��o (essa � a parte ruim: se voc� tem mais de um cora��o, vai ter que repetir esse mesmo procedimento para todos os outros reposit�rios dos seus outros cora��es), Settings, Webhooks & Services.

<a href="https://www.flickr.com/photos/120157483@N04/14724974103/" title="Adicionando um WebHook ao GitHub"><img src="https://farm3.staticflickr.com/2915/14724974103_f8f8fb7e78_o.png" alt="Adicionando um WebHook ao GitHub"></a>

Lembre-se de colocar seu c�digo PHP em um servidor vis�vel na web. Lembre-se tamb�m de usar o m�todo de envio urlencoded do payload para simplificar seu tratamento. Para simplificar ainda mais o processo, coloque qualquer coisa no segredo (n�o validaremos neste post, mas #ficadica de seguran�a se voc� n�o quer que outros acessem seu PHP inadvertidamente).

Pois bem. No c�digo que ir� receber o payload do GitHub precisamos de duas coisas: saber qual [a estrutura que vai ser recebida](https://developer.github.com/webhooks/) e _como localizar o id do card onde iremos enviar a informa��o_. Nesse caso, mais uma vez, para simplificar, vamos procurar pelo pr�prio linque permanente do cart�o na mensagem do commit. Ali�s, doS commitS (sendo um push, � prov�vel que o evento seja gerado com diversos commits aninhados).

..\Code\GitHubBitBucketTrelloIntegration\onGitHubCommit.php

Agora � s� testar. Posso pegar esse mesmo artigo e comit�-lo no [reposit�rio do blogue](https://github.com/Caloni/Caloni.com.br) usando o linque �nico do card da tarefa de escrever este artigo. Ou seja, aqui � Inception na veia, merm�o!

<a href="https://www.flickr.com/photos/120157483@N04/14702001611/" title="Comitando o artigo para gerar evento que ir� comentar no Trello para continuar este artigo que estou comitando.."><img src="https://farm6.staticflickr.com/5595/14702001611_acb9318ba7_o.png" alt="Comitando o artigo para gerar evento que ir� comentar no Trello para continuar este artigo que estou comitando.."></a>

O que vai deixar voc� perplexo � entender como esse texto est� sendo comitado antes mesmo de eu comitar este texto ;).

<a href="https://www.flickr.com/photos/120157483@N04/14518530070/" title="Resultado do meu commit"><img src="https://farm4.staticflickr.com/3880/14518530070_25f639d741_o.png" alt="Resultado do meu commit"></a>

E o neg�cio � r�pido, viu?

