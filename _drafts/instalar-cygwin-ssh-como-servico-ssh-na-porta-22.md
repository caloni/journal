---
title: "Instalar Cygwin SSH Como Serviço na Porta 22"
tags: [ "cygwin", "ssh", "server", "windows" ]

---
Foram montados cliente e servidor a partir de uma instalação simples do Cygwin (no servidor):

#### Instalar o Cygwin na máquina servidora checando além do default os módulos openssh e rsync.

Ambos estão na categoria Net e devem estar selecionados:

![](/images/cygwin-setup-openssh.png)

![](/images/cygwin-setup-rsync.png)

#### No server, em um bash iniciado com privilégios de admin (e elevado), rodar o script ssh-host-config para instalar o daemon de ssh.

![](/images/cygwin-bash-run-as-admin.png)

```cmd
Copying skeleton files.
These files are for the users to personalise their cygwin experience.

They will never be overwritten nor automatically updated.

'./.bashrc' -> '/home/Caloni//.bashrc'
'./.bash_profile' -> '/home/Caloni//.bash_profile'
'./.inputrc' -> '/home/Caloni//.inputrc'
'./.profile' -> '/home/Caloni//.profile'

Caloni@vmw7efi ~
$ ssh-host-config

*** Info: Generating missing SSH host keys
ssh-keygen: generating new host keys: RSA DSA ECDSA ED25519
*** Info: Creating default /etc/ssh_config file
*** Info: Creating default /etc/sshd_config file

*** Info: StrictModes is set to 'yes' by default.
*** Info: This is the recommended setting, but it requires that the POSIX
*** Info: permissions of the user's home directory, the user's .ssh
*** Info: directory, and the user's ssh key files are tight so that
*** Info: only the user has write permissions.
*** Info: On the other hand, StrictModes don't work well with default
*** Info: Windows permissions of a home directory mounted with the
*** Info: 'noacl' option, and they don't work at all if the home
*** Info: directory is on a FAT or FAT32 partition.
*** Query: Should StrictModes be used? (yes/no) no

*** Info: Privilege separation is set to 'sandbox' by default since
*** Info: OpenSSH 6.1.  This is unsupported by Cygwin and has to be set
*** Info: to 'yes' or 'no'.
*** Info: However, using privilege separation requires a non-privileged account
*** Info: called 'sshd'.
*** Info: For more info on privilege separation read /usr/share/doc/openssh/README.privsep.
*** Query: Should privilege separation be used? (yes/no) no
*** Info: Updating /etc/sshd_config file

*** Query: Do you want to install sshd as a service?
*** Query: (Say "no" if it is already installed as a service) (yes/no) yes
*** Query: Enter the value of CYGWIN for the daemon: [] ntsec
*** Info: On Windows Server 2003, Windows Vista, and above, the
*** Info: SYSTEM account cannot setuid to other users -- a capability
*** Info: sshd requires.  You need to have or to create a privileged
*** Info: account.  This script will help you do so.

*** Info: It's not possible to use the LocalSystem account for services
*** Info: that can change the user id without an explicit password
*** Info: (such as passwordless logins [e.g. public key authentication]
*** Info: via sshd) when having to create the user token from scratch.
*** Info: For more information on this requirement, see
*** Info: https://cygwin.com/cygwin-ug-net/ntsec.html#ntsec-nopasswd1

*** Info: If you want to enable that functionality, it's required to create
*** Info: a new account with special privileges (unless such an account
*** Info: already exists). This account is then used to run these special
*** Info: servers.

*** Info: Note that creating a new user requires that the current account
*** Info: have Administrator privileges itself.

*** Info: No privileged account could be found.

*** Info: This script plans to use 'cyg_server'.
*** Info: 'cyg_server' will only be used by registered services.
*** Query: Do you want to use a different name? (yes/no) yes
*** Query: Enter the new user name: cyg_server
*** Query: Reenter: cyg_server

*** Query: Create new privileged user account 'VMW7EFI\cyg_server' (Cygwin name: 'cyg_server')? (yes/no) yes
*** Info: Please enter a password for new user cyg_server.  Please be sure
*** Info: that this password matches the password rules given on your system.
*** Info: Entering no password will exit the configuration.
*** Query: Please enter the password:
*** Query: Reenter:

*** Info: User 'cyg_server' has been created with password 'cygwin'.
*** Info: If you change the password, please remember also to change the
*** Info: password for the installed services which use (or will soon use)
*** Info: the 'cyg_server' account.


*** Info: The sshd service has been installed under the 'cyg_server'
*** Info: account.  To start the service now, call `net start sshd' or
*** Info: `cygrunsrv -S sshd'.  Otherwise, it will start automatically
*** Info: after the next reboot.

*** Info: Host configuration finished. Have fun!

Caloni@vmw7efi ~
$ net start sshd
The CYGWIN sshd service is starting.
The CYGWIN sshd service was started successfully.

Caloni@vmw7efi ~
$
```

Através do Service Manager é possível ver o serviço instalado com o nome CYGWIN sshd:

![](/images/cygwin-sshd-service.png)

Através do Process Explorer ou TCP View é possível ver que a porta 22 está em listening pelo serviço sshd.exe:

![](/images/sshd-procexp-listening-port22.png)

Para que a comunicação seja feita remotamente é necessário adicionar uma regra de exceção ao Firewall do Windows e a qualquer firewall intermediário para a porta 22.

![](/images/windows-firewall-allow-port22-ssh.png)

Para configurar as chaves pública/privada de conexão do ssh, agora em um bash da conta usada, o script ssh-user-config.

```cmd
Caloni@vmw7efi ~
$ ssh-user-config
*** Query: Shall I create a SSH2 RSA identity file for you? (yes/no) yes
*** Info: Generating /home/Caloni/.ssh/id_rsa
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
*** Query: Do you want to use this identity to login to this machine? (yes/no) yes
*** Info: Adding to /home/Caloni/.ssh/authorized_keys
*** Query: Shall I create a SSH2 DSA identity file for you? (yes/no) no
*** Query: Shall I create a SSH2 ECDSA identity file for you? (yes/no) no
*** Query: Shall I create a (deprecated) SSH1 RSA identity file for you? (yes/no) no

*** Info: Configuration finished. Have fun!
```

![](/images/cygwin-openssh-generate-user-keys.png)

#### Gerar o pacote client e instalar em uma máquina de teste.

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

É importante notar que usar versões diferentes do Cygwin, especialmente as DLLs de suporte, pode causar problemas no sincronismo. Por isso é aconselhável que a geração do pacote cliente seja feito através dos binários da versão servidor.

Copiar a pasta com os arquivos para o cliente na máquina que irá conectar com o servidor.

Copiar a pasta .ssh da conta que será usada para acessar via SSH o servidor.

Tornar o acesso a esta pasta exclusivo da conta em questão (do contrário ela será rejeitada pela ferramenta ssh).

Em um prompt nessa máquina, dentro da pasta copiada, executar para testar o comando ssh <ip-do-servidor>.

Após esse comando o prompt deverá ser o da máquina remota, em um bash fornecido pelo cygwin remoto. A partir daí todos os comandos refletirão no servidor.

É importante notar que o nome da conta Windows deve manter o mesmo case usado pelo serviço SSH (ex: tudo minúsculo). Do contrário a comunicação, seja via senha ou chave privada, não será possível, dando erro de autenticação.

![](/images/cygwin-windows-account-case.png)

Caso o case do nome da conta seja acertado, será necessário reiniciar o serviço de SSH.

#### Teste de cópia de arquivo via rsync

```cmd
rsync -a -e './ssh' /cygdrive/c/Temp caloni@192.168.0.133:/cygdrive/c/Temp/testclient
````

