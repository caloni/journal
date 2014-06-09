# Três posts sobre evolução Windows

Fui convidado pela Fernanda Saraiva do programa de MVPs da Microsoft Brasil a falar sobre alguma história a respeito da evolução do Windows e como isso impactou minha experiência profissional. Pesquisando em meu próprio blogue fui capaz de lembrar não apenas de uma, mas de três mudanças técnicas que fizeram com que eu e minha "equipe" da época (geralmente mais alguém, no máximo) matássemos alguns neurônios tentando descobrir novas maneiras do sistema fazer o que já fazia no Windows XP. Irei compartilhar uma por vez no que tem sido o meu post semanal que eu apelidei carinhosamente de Post da Terça. Já faz mais de um mês que consigo publicar pelo menos na terça algo de novo, e espero manter esse ritmo.

## Eles querem que a GINA vá embora...

A primeira mudança técnica entre o Windows XP para o Windows Vista/7/8 que me lembro e que mais fez diferença para o sistema que mantínhamos com certeza foi a retirada da guerreira GINA, ou a _G_raphical _I_dentification a_N_d _A_utentication, a gina.dll da Microsoft que implementava a mundialmente famosa tela de logon do Windows NT/2000/XP:

<a href="https://www.flickr.com/photos/120157483@N04/14354351486/" title="Windows XP dá as boas vindas por wanderleycaloni, no Flickr"><img src="https://farm4.staticflickr.com/3871/14354351486_a296ee1352_z.jpg" alt="Windows XP dá as boas vindas"></a>

Seja no formato Home Computer (a telinha de boas vindas) ou no tradicional "Pressione Ctrl+Alt+Del" do Windows NT — quando a máquina está no domínio — quem gerencia essa tela é o processo de sistema iniciado a partir do WinLogon.exe. O WINLOGON carrega a nossa amiga gina.dll que é quem realiza a autenticação dos usuários.

Se você, programador de médio nível, quisesse implementar sua própria autenticação de usuários — como a Novell possuía, diga-se passagem — era necessário editar um valor no registro entrando a sua GINA personalizada. Lógico que ela deveria ter todas as funções documentadas implementadas e exportadas para que o WINLOGON conseguisse se comunicar, como a famigerada [WlxInitialize](http://msdn.microsoft.com/en-us/library/windows/desktop/aa380567%28v=vs.85%29.aspx), que recebia a lista de ponteiros de funções para os outros eventos a ser tratados.

[github,Code/EvolucaoWindows/auto-logon-gina.cpp,cpp]

Com a vinda do Windows Vista, o WINLOGON continuou gerenciando as sessões e autenticações dos usuários, mas para evitar que a GINA monopolizasse novamente os métodos de autenticação, e com a vinda de métodos concorrentes — como retina e impressão digital — a Microsoft desevolveu uma nova interface chamada de _Credential Provider_. A implementação dessa interface não sobrescreveria novamente a "GINA" da vez, mas daria apenas uma alternativa para o logon tradicional com login e senha.

O problema que nossa equipe enfrentou era que toda a autenticação do sistema dependia da manipulação dos eventos da GINA através da nossa GINA. Com ela colocada de escanteio, os logins parariam de funcionar.

<a href="https://www.flickr.com/photos/120157483@N04/14377533845/" title="gina por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2919/14377533845_095c2016ec_m.jpg" alt="gina"></a>

Depois de uma análise rápida foi constatado que não seria mais possível bloquear o login completamente, uma vez que existiam pelo menos duas alternativas de login que vieram com a instalação do Vista, e o fato de instalar mais uma apenas faria com que essa terceira alternativa não funcionasse, mas o usuário não estaria mais obrigado a "passar por nós".

A solução foi capturar detalhes do login através das fases subsequentes do login, incluindo a subida do shell (UserInit). Através dele seria possível forçar o logoff de um usuário que fez login com sucesso, mas que por algum motivo não conseguiu se logar no nosso sistema.

Nem sempre o que estava rodando já há anos é a solução mais bonita. Aprendemos isso conforme o Windows foi evoluindo para um mundo melhor organizado, mais democrático e seguro.


## Dando cabo do WinDbg

Na semana passada falei sobre a ideia de comentar algumas mudanças entre o Windows XP e o novo mundo Vista/7/8/<fim-dos-tempos> que fizeram com que adaptássemos algum código que obviamente não funcionaria mais. Falamos sobre a famigerada GINA (ou famiGINADA), e agora apenas vou comentar brevemente sobre o sistema de boot, que também mudou.

<a href="https://www.flickr.com/photos/120157483@N04/14378401335/" title="boot por wanderleycaloni, no Flickr"><img src="https://farm4.staticflickr.com/3846/14378401335_8362edcc90_m.jpg" alt="boot"></a>

Na verdade, pouca coisa mudou, mas foi o suficiente para [dar problemas na hora de usar o WinDbg](http://driverentry.com.br/blog/?p=943). Tradicionalmente, o boot era gerenciado no Windows através de um arquivo localizado na raiz da partição ativa (configuração da [MBR](http://www.caloni.com.br/blog/archives/o-boot-no-windows-sem-windows)) chamado boot.ini. Dentro dele temos uma estrutura semelhante a um .INI (duh), onde a informação que vemos lá podia ser configurada nas configurações do Computador no Windows XP.

<a href="https://www.flickr.com/photos/120157483@N04/14375095441/" title="Configuração do boot no Windows 7 por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2936/14375095441_4a9ac87a35_o.png" alt="Configuração do boot no Windows 7"></a>

Hoje em dia esse arquivo nem existe mais, o que pode dar um friozinho na barriga ("caramba, não vai mais bootar!!"). Agora, para sistemas baseados em BIOS há uma pasta Boot na raiz e um arquivo chamado bcd. Para os mais moderninhos, baseados em EFI, ele fica na partição EFI. Ah, EFI é Extensible Firmware Interface, e faz parte da especificação da [UEFI](http://en.wikipedia.org/wiki/Unified_Extensible_Firmware_Interface) (Universal blá blá blá), mais ou menos um padrão que define como deve ser feita a comunicação entre hardware e sistema operacional.

Para a edição desse novo arquivo (ou partição) é necessário que seja usada a ferramenta _BCDEdit_ no Windows. É ela que agora configura qual partição está ativa e, mais importante para escovadores de bits, qual pode ser depurada pela porta serial através do WinDbg.

Porta serial? Mas que coisa antiga, hein? Pois é, muita coisa mudou desde o Windows Vista, mas algumas coisas precisam permanecer... compatíveis.

Mas... se você precisar:
<a href="https://www.flickr.com/photos/120157483@N04/14191895328/" title="Null Cable Modem Wiring por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5510/14191895328_44a2ec8338_z.jpg" alt="Null Cable Modem Wiring"></a>
