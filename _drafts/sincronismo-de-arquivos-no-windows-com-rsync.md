---
title: "Sincronismo de arquivos no Windows com rsync"
tags: [ "cygwin", "ssh", "server", "windows", "rsync" ]

---
Para sincronizar arquivos de maneira bem feita e por todos os lugares existe uma ferramenta no Unix/Linux chamada [rsync](https://download.samba.org/pub/rsync/rsync.html), já usada por décadas, robusta, e que suporta comunicação via [ssh](https://en.wikipedia.org/wiki/Secure_Shell), um protocolo seguro baseado em chave pública/privada, além de ter embutido funcionalidades como compressão de arquivos. Essa ferramenta é mantida apenas para Unix/Linux, mas através do [Cygwin](https://cygwin.com/install.html) é possível utilizá-la em uma instalação Windows. O único requisito é manter uma pasta com a instalação padrão do Cygwin e os módulos necessários (openssh, rsync e as libs base do Cygwin).

Este tutorial pode ser dividido em duas partes. Na primeira parte foi feita a instalação e configuração de um serviço SSH no Windows usando o Cygwin e acessá-lo através de um pacote gerado através do próprio server. A vantagem é que tudo pode ser feito de maneira seguran, através de comunicação encriptada e com o armazenamento de chaves podendo ser inacessível para qualquer outro usuário.

```cmd
rsync -a -e './ssh' /cygdrive/c/Temp caloni@192.168.0.133:/cygdrive/c/Temp/testclient
````

