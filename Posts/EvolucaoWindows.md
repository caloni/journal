# Tr�s posts sobre evolu��o Windows

Fui convidado pela Fernanda Saraiva do programa de MVPs da Microsoft Brasil a falar sobre alguma hist�ria a respeito da evolu��o do Windows e como isso impactou minha experi�ncia profissional. Pesquisando em meu pr�prio blogue fui capaz de lembrar n�o apenas de uma, mas de tr�s mudan�as t�cnicas que fizeram com que eu e minha "equipe" da �poca (geralmente mais algu�m, no m�ximo) mat�ssemos alguns neur�nios tentando descobrir novas maneiras do sistema fazer o que j� fazia no Windows XP. Irei compartilhar uma por vez no que tem sido o meu post semanal que eu apelidei carinhosamente de Post da Ter�a. J� faz mais de um m�s que consigo publicar pelo menos na ter�a algo de novo, e espero manter esse ritmo.

## Eles querem que a GINA v� embora...

A primeira mudan�a t�cnica entre o Windows XP para o Windows Vista/7/8 que me lembro e que mais fez diferen�a para o sistema que mant�nhamos com certeza foi a retirada da guerreira GINA, ou a _G_raphical _I_dentification a_N_d _A_utentication, a gina.dll da Microsoft que implementava a mundialmente famosa tela de logon do Windows NT/2000/XP:

<a href="https://www.flickr.com/photos/120157483@N04/14354351486/" title="Windows XP d� as boas vindas por wanderleycaloni, no Flickr"><img src="https://farm4.staticflickr.com/3871/14354351486_a296ee1352_z.jpg" alt="Windows XP d� as boas vindas"></a>

Seja no formato Home Computer (a telinha de boas vindas) ou no tradicional "Pressione Ctrl+Alt+Del" do Windows NT � quando a m�quina est� no dom�nio � quem gerencia essa tela � o processo de sistema iniciado a partir do WinLogon.exe. O WINLOGON carrega a nossa amiga gina.dll que � quem realiza a autentica��o dos usu�rios.

Se voc�, programador de m�dio n�vel, quisesse implementar sua pr�pria autentica��o de usu�rios � como a Novell possu�a, diga-se passagem � era necess�rio editar um valor no registro entrando a sua GINA personalizada. L�gico que ela deveria ter todas as fun��es documentadas implementadas e exportadas para que o WINLOGON conseguisse se comunicar, como a famigerada [WlxInitialize](http://msdn.microsoft.com/en-us/library/windows/desktop/aa380567%28v=vs.85%29.aspx), que recebia a lista de ponteiros de fun��es para os outros eventos a ser tratados.

[github,Code/EvolucaoWindows/auto-logon-gina.cpp,cpp]

Com a vinda do Windows Vista, o WINLOGON continuou gerenciando as sess�es e autentica��es dos usu�rios, mas para evitar que a GINA monopolizasse novamente os m�todos de autentica��o, e com a vinda de m�todos concorrentes � como retina e impress�o digital � a Microsoft desevolveu uma nova interface chamada de _Credential Provider_. A implementa��o dessa interface n�o sobrescreveria novamente a "GINA" da vez, mas daria apenas uma alternativa para o logon tradicional com login e senha.

O problema que nossa equipe enfrentou era que toda a autentica��o do sistema dependia da manipula��o dos eventos da GINA atrav�s da nossa GINA. Com ela colocada de escanteio, os logins parariam de funcionar.

<a href="https://www.flickr.com/photos/120157483@N04/14377533845/" title="gina por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2919/14377533845_095c2016ec_m.jpg" alt="gina"></a>

Depois de uma an�lise r�pida foi constatado que n�o seria mais poss�vel bloquear o login completamente, uma vez que existiam pelo menos duas alternativas de login que vieram com a instala��o do Vista, e o fato de instalar mais uma apenas faria com que essa terceira alternativa n�o funcionasse, mas o usu�rio n�o estaria mais obrigado a "passar por n�s".

A solu��o foi capturar detalhes do login atrav�s das fases subsequentes do login, incluindo a subida do shell (UserInit). Atrav�s dele seria poss�vel for�ar o logoff de um usu�rio que fez login com sucesso, mas que por algum motivo n�o conseguiu se logar no nosso sistema.

Nem sempre o que estava rodando j� h� anos � a solu��o mais bonita. Aprendemos isso conforme o Windows foi evoluindo para um mundo melhor organizado, mais democr�tico e seguro.


## Dando cabo do WinDbg

Na semana passada falei sobre a ideia de comentar algumas mudan�as entre o Windows XP e o novo mundo Vista/7/8/<fim-dos-tempos> que fizeram com que adapt�ssemos algum c�digo que obviamente n�o funcionaria mais. Falamos sobre a famigerada GINA (ou famiGINADA), e agora apenas vou comentar brevemente sobre o sistema de boot, que tamb�m mudou.

<a href="https://www.flickr.com/photos/120157483@N04/14378401335/" title="boot por wanderleycaloni, no Flickr"><img src="https://farm4.staticflickr.com/3846/14378401335_8362edcc90_m.jpg" alt="boot"></a>

Na verdade, pouca coisa mudou, mas foi o suficiente para [dar problemas na hora de usar o WinDbg](http://driverentry.com.br/blog/?p=943). Tradicionalmente, o boot era gerenciado no Windows atrav�s de um arquivo localizado na raiz da parti��o ativa (configura��o da [MBR](http://www.caloni.com.br/blog/archives/o-boot-no-windows-sem-windows)) chamado boot.ini. Dentro dele temos uma estrutura semelhante a um .INI (duh), onde a informa��o que vemos l� podia ser configurada nas configura��es do Computador no Windows XP.

<a href="https://www.flickr.com/photos/120157483@N04/14375095441/" title="Configura��o do boot no Windows 7 por wanderleycaloni, no Flickr"><img src="https://farm3.staticflickr.com/2936/14375095441_4a9ac87a35_o.png" alt="Configura��o do boot no Windows 7"></a>

Hoje em dia esse arquivo nem existe mais, o que pode dar um friozinho na barriga ("caramba, n�o vai mais bootar!!"). Agora, para sistemas baseados em BIOS h� uma pasta Boot na raiz e um arquivo chamado bcd. Para os mais moderninhos, baseados em EFI, ele fica na parti��o EFI. Ah, EFI � Extensible Firmware Interface, e faz parte da especifica��o da [UEFI](http://en.wikipedia.org/wiki/Unified_Extensible_Firmware_Interface) (Universal bl� bl� bl�), mais ou menos um padr�o que define como deve ser feita a comunica��o entre hardware e sistema operacional.

Para a edi��o desse novo arquivo (ou parti��o) � necess�rio que seja usada a ferramenta _BCDEdit_ no Windows. � ela que agora configura qual parti��o est� ativa e, mais importante para escovadores de bits, qual pode ser depurada pela porta serial atrav�s do WinDbg.

Porta serial? Mas que coisa antiga, hein? Pois �, muita coisa mudou desde o Windows Vista, mas algumas coisas precisam permanecer... compat�veis.

Mas... se voc� precisar:
<a href="https://www.flickr.com/photos/120157483@N04/14191895328/" title="Null Cable Modem Wiring por wanderleycaloni, no Flickr"><img src="https://farm6.staticflickr.com/5510/14191895328_44a2ec8338_z.jpg" alt="Null Cable Modem Wiring"></a>
