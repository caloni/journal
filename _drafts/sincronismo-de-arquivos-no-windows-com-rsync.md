---
title: "Sincronismo de arquivos no Windows com rsync"
tags: [ "projetos" ]

---
Para sincronizar arquivos de maneira bem feita e por todos os lugares existe uma ferramenta no Unix/Linux chamada [rsync](https://download.samba.org/pub/rsync/rsync.html), já usada por décadas, robusta, e que suporta comunicação via [ssh](https://en.wikipedia.org/wiki/Secure_Shell), um protocolo seguro baseado em chave pública/privada, além de ter embutido funcionalidades como compressão de arquivos. Essa ferramenta é mantida apenas para Unix/Linux, mas através do [Cygwin](https://cygwin.com/install.html) é possível utilizá-la em uma instalação Windows. O único requisito é manter uma pasta com a instalação padrão do Cygwin e os módulos necessários (openssh, rsync e as libs base do Cygwin).

Em um teste inicial que fiz para Windows, em uma instalação enxuta do Cygwin, consegui manter um servidor ssh escutando a porta padrão (22) e clientes rsync utilizarem essa forma de comunicação para realizar o sincronismo dos arquivos. Um wrapper seria o mais indicado, já que o Cygwin necessita seguir alguns procedimentos, como logar via bash, por exemplo (do contrário o mapeamento com os driver Windows não funciona). Já existe pelo menos um wrapper desenvolvido para Windows pela empresa [AboutMyX](http://www.aboutmyip.com/AboutMyXApp/DeltaCopy.jsp) chamado DeltaCopy, com código-fonte disponível. A solução embute o rscync do Cygwin no Windows e através de interfaces desenvolvidas através do Borland C++ Builder são configurados paths de origem e destino para backup remoto.

Há um guia para não-iniciantes no site [How-To-Geek](http://www.howtogeek.com/175008/the-non-beginners-guide-to-syncing-data-with-rsync/) que demonstra como o sincronismo de arquivos funciona pelo rsync de maneira transparente, seja pelo Linux ou Windows. Além disso, a solução por ssh permite que o servidor use a solução Cygwin ou qualquer outra que implemente o protocolo, por exemplo o [OpenSSH for Windows](https://github.com/PowerShell/Win32-OpenSSH), uma solução cujos fontes a Microsoft liberou no GitHub (ainda em fase de testes).

No site How-To-Geek existe um [tutorial](http://www.howtogeek.com/howto/41560/how-to-get-ssh-command-line-access-to-windows-7-using-cygwin/) para configurar o client e servidor ssh do Cygwin. Após esse setup inicial o uso do rsync se torna automático. A cópia de uma pasta foi feita com o seguinte comando:

Foram montados cliente e servidor a partir de uma instalação simples do Cygwin (no servidor):

#### 1. Instalar o Cygwin na máquina servidora checando além do default os módulos openssh e rsync.

Ambos estão na categoria Net e devem estar selecionados:

![](/images/cygwin-setup-openssh.png)

![](/images/cygwin-setup-rsync.png)

#### 2. Gerar o pacote client copiando o rsync.exe, ssh.exe e as DLLs necessárias.

Você pode gerar uma batch com os comandos abaixo:

```cmd
@echo off
mkdir rsync
pushd rsync
copy \cygwin64\bin\cygcom_err-2.dll
copy \cygwin64\bin\cygcrypto-1.0.0.dll
copy \cygwin64\bin\cyggcc_s-seh-1.dll
copy \cygwin64\bin\cyggssapi_krb5-2.dll
copy \cygwin64\bin\cygiconv-2.dll
copy \cygwin64\bin\cygintl-8.dll
copy \cygwin64\bin\cygk5crypto-3.dll
copy \cygwin64\bin\cygkrb5-3.dll
copy \cygwin64\bin\cygkrb5support-0.dll
copy \cygwin64\bin\cygssp-0.dll
copy \cygwin64\bin\cygwin1.dll
copy \cygwin64\bin\cygz.dll
copy \cygwin64\bin\rsync.exe
copy \cygwin64\bin\ssh.exe
popd
```

#### 3. No server, em um bash iniciado com privilégios de admin (e elevado), rodar o script ssh-host-config para instalar o daemon de ssh.

#### 4. Ao ser questionado sobre o valor do daemon para o CYGWIN, digitar ntsec.

#### 5. Iniciar o daemon... quer dizer, serviço do Windows, com o comando net start sshd.

#### 6. Para configurar as chaves de conexão do ssh executar, agora em um bash da conta usada, o script ssh-user-config.

#### 7. Copiar a pasta com os arquivos para o cliente na máquina que irá conectar com o servidor.

#### 8. Em um prompt nessa máquina, dentro da pasta copiada, executar para testar o comando ssh <ip-do-servidor>.

#### 9. Usar o usuário (case sensitive) e a senha (do Windows) do servidor quando questionado.

#### 10. Funcionando a conexão é possível passar para o teste de cópia de arquivo.

#### 11. Teste de cópia de arquivos: rsync -a /cygdrive/c/pasta-de-teste/ user@localhost:/cygdrive/c/pasta-de-teste-destino.


O próximo problema ainda não resolvido é evitar a pergunta da senha e usar a chave privada do cliente.


### Problemas mais comuns

#### Erro de acesso negado, mesmo digitando a senha certa para o usuário

Verifique se o nome do usuário bate o case com o usuário remoto. Os usuários no Linux são case sensitive, então uma letra maiúscula pode fazer com que o serviço de ssh não encontre o usuário.

