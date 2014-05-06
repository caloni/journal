= Modelo para novos projetos/componentes =

A disposi��o das pastas visa seguir um modelo padronizado de organiza��o de um projeto (componente standalone) ou componente (que pode ser usado em projetos maiores). � necess�rio, al�m de seguir a padroniza��o de pastas, seguir a padroniza��o dos conceitos aqui apresentados.


= Controle de fonte =

Foi criado um commit inicial que visa servir de ponto de partida para a constru��o do hist�rico de modifica��es de projetos. Se todos os projetos come�arem do mesmo ponto fica mais f�cil fazer futuros processos de refactoring, em que pega-se um projeto e coloca-se em outro e vice-versa. O controle de fonte utilizado � o Bazaar, mas pode ser escolhido qualquer outro distribu�do, desde que exista um ponto inicial de commit.


= Pastas =

Build. Essa pasta cont�m tudo que � necess�rio para compilar e testar o projeto como um todo. Idealmente a execu��o da batch build.bat deve executar todo o processo. Ap�s a compila��o, � de compet�ncia dos componentes na subpasta Tests fazer os testes b�sicos do projeto para se certificar de que tudo est� funcionando como deveria.

Common. Aqui devem ser colocados aqueles includes que servem para v�rios pontos do projeto. Est� exemplificado pelo arquivo de vers�o (Version.h), pois todos os arquivos devem referenciar uma �nica vers�o do produto. Podem existir Outras defini��es b�sicas, como nome do produto, dos arquivos, etc. � aqui que s�o gravadas as interfaces que permitem depend�ncia circular entre os componentes (e.g. Interface de componentes COM).

Docs. Aqui deve ser colocada toda a documenta��o que diz respeito ao projeto. A organiza��o interna ainda n�o foi definida, pois imagina-se ser poss�vel usar diversas fontes, como doxygen, casos de uso, bugs, arquivos de projeto e UML. Foi exemplificado com o arquivo todo.txt e changes.txt, que deve ter sempre a lista de coisas a fazer e a lista de coisas j� feitas, respectivamente, tendo, portanto, que ser sempre atualizados.

Drivers. Essa � a parte onde ficam todos os componentes que rodam em kernel mode. Por se tratar de um dom�nio espec�fico e muitas vezes compartilhar c�digo-fonte de maneira n�o-heterodoxa (e.g. sem uso de LIBs), faz sentido existir uma pasta que agrupe esses elementos. Dentro da pasta existem subpastas para cada driver, exemplificados em Driver1 e Driver2.

Install. Todas as coisas relacionadas com instala��o, desinstala��o e atualiza��o do software deve vir nessa pasta. Foi reservada uma subpasta para cada item, n�o sendo obrigat�ria sua divis�o. Tamb�m existe uma pasta de DLLs, onde possivelmente existam telas personalizadas e biblioteca de uso comum pelos instaladores (o desinstalador conversa com o instalador e assim por diante).

Interface. Todas as telas de um programa devem ser colocadas nessa pasta. Essa � uma divis�o que deve ser seguida conceitualmente. Por exemplo, se existir um gerenciador de alguma coisa no produto, as telas do gerenciador e o comportamento da interface ficam nessa pasta, mas o comportamento intr�nseco do sistema (regras de neg�cio) devem ficar em Libraries. Para exemplificar o uso, foram criadas as Interface1 e Interface2.

Libraries. O ponto central do projeto, deve conter o c�digo mais importante. Imagine a pasta Libraries como a intelig�ncia de um projeto, de onde todos os outros componentes se utilizam para que a l�gica do software seja sempre a mesma. As outras partes do projeto lidam com aspectos t�cnicos, enquanto o Libraries cont�m as regras abstratas de funcionamento. Opcionalmente ela pode ser est�tica ou din�mica, caso onde foi criada a subpasta DLLs. Por�m, elas devem ser divididas por fun��o em bibliotecas est�ticas, como foi exemplificado em Library1 e Library2.

Resources. A origem de todas as imagens, sons, cursores, etc de um projeto devem residir primeiramente na pasta Resources. A divis�o interna desse item fica a crit�rio do designer respons�vel, pois ele pode dividir tanto por fun��o (Install, Interface) quanto por elementos (Images, Sounds).

Services. Al�m dos drivers e das interfaces alguns projetos necessitam de processos "invis�veis" que devem fazer algo no sistema. Isso inclui servi�os do Windows, GINAs, componentes COM e coisas do g�nero. Devem ser colocados nessa pasta e distribu�dos como no exemplo, em Service1 e Service2.

Tools. Al�m dos componentes essenciais para o funcionamento do software tamb�m existem aqueles componentes que fornecem mais poder ao usu�rio, ao pessoal do suporte ou ao pr�prio time de desenvolvimento. Essas s�o as ferramentas de suporte que permitem a f�cil identifica��o de erros no programa ou a configura��o mais avan�ada de um item que a Interface n�o cobre. Adicionalmente foi colocada a subpasta Develop, que deve conter ferramentas usadas estritamente durante a fase de desenvolvimento.


= Testes =

Todos os componentes que disponibilizarem unidades de testes devem conter uma pasta Tests dentro de si. Essa padroniza��o permite facilmente a localiza��o de testes internos aos componentes. Al�m disso, os arquivos execut�veis de testes devem sempre terminar seu nome com Test, o que permite a automatiza��o do processo de teste durante o build.
