---
title: "Sincronismo de arquivos no Windows com rsync"
tags: [ "projetos" ]

---
Para sincronizar arquivos de maneira bem feita e por todos os lugares existe uma ferramenta no Unix/Linux chamada [rsync](https://download.samba.org/pub/rsync/rsync.html), já usada por décadas, robusta, e que suporta comunicação via [ssh](https://en.wikipedia.org/wiki/Secure_Shell), um protocolo seguro baseado em chave pública/privada, além de ter embutido funcionalidades como compressão de arquivos. Essa ferramenta é mantida apenas para Unix/Linux, mas através do [Cygwin](https://cygwin.com/install.html) é possível utilizá-la em uma instalação Windows. O único requisito é manter uma pasta com a instalação padrão do Cygwin e os módulos necessários (openssh, rsync e as libs base do Cygwin).

Em um teste inicial que fiz para Windows, em uma instalação enxuta do Cygwin, consegui manter um servidor ssh escutando a porta padrão (22) e clientes rsync utilizarem essa forma de comunicação para realizar o sincronismo dos arquivos. Um wrapper seria o mais indicado, já que o Cygwin necessita seguir alguns procedimentos, como logar via bash, por exemplo (do contrário o mapeamento com os driver Windows não funciona). Já existe pelo menos um wrapper desenvolvido para Windows pela empresa [AboutMyX](http://www.aboutmyip.com/AboutMyXApp/DeltaCopy.jsp) chamado DeltaCopy, com código-fonte disponível. A solução embute o rscync do Cygwin no Windows e através de interfaces desenvolvidas através do Borland C++ Builder são configurados paths de origem e destino para backup remoto.

Há um guia para não-iniciantes no site [How-To-Geek](http://www.howtogeek.com/175008/the-non-beginners-guide-to-syncing-data-with-rsync/) que demonstra como o sincronismo de arquivos funciona pelo rsync de maneira transparente, seja pelo Linux ou Windows. Além disso, a solução por ssh permite que o servidor use a solução Cygwin ou qualquer outra que implemente o protocolo, por exemplo o [OpenSSH for Windows](https://github.com/PowerShell/Win32-OpenSSH), uma solução cujos fontes a Microsoft liberou no GitHub (ainda em fase de testes).

No site How-To-Geek existe um [tutorial](http://www.howtogeek.com/howto/41560/how-to-get-ssh-command-line-access-to-windows-7-using-cygwin/) para configurar o client e servidor ssh do Cygwin. Após esse setup inicial o uso do rsync se torna automático. A cópia de uma pasta foi feita com o seguinte comando:

```cmd
rsync -a -e 'ssh' /cygdrive/c/Temp/files/src/ Caloni@localhost:/cygdrive/c/Temp/files/dest
```
