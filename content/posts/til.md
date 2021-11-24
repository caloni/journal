---
categories:
- blog
date: '2020-12-08'
link: https://www.reddit.com/r/todayilearned/
tags:
- lists
title: 'TIL: Today I Learned'
---

Se você guardar o mínimo de conhecimento que aprendeu em um dia de sua vida e relembrá-lo ao longo dos anos não vai perceber qualquer diferença, pois com a internet fica fácil recuperar o conhecimento com nova pesquisa. No entanto, na internet está também você e seu conhecimento em forma de posts, comentários, podcasts ou qualquer tipo de conteúdo que você produziu. Não custa manter a rotina de gravar o que foi aprendido em um dia para tentar capturar a essência de cada um deles. Quem sabe algo de esclarecedor sobre nós mesmos não pode surgir disso.

 - 2021-04-21: meu blogue não é um blogue, mas um [jardim digital].

 - 2020-08-07: ao usar ponto (.) antes de rodar um comando no terminal Linux é possível definir variáveis de ambiente. Com isso consigo definir o ambiente para fazer cross compiling em Golang no Linux para Windows, gravando as variáveis em um arquivo executável "export GOOS=windows; export GOARCH=amd64; etc" e chamando com ". meu_arquivo" no terminal.

 - 2020-10-22: para desabilitar expiração de senhas no Oracle o comando `alter user user_name identified by new_password account unlock;` resolve. Para as senhas não expirarem novamente em ambiente de desenv é só usar o comando `ALTER PROFILE "DEFAULT" LIMIT PASSWORD_VERIFY_FUNCTION NULL;` uma vez no banco.

 - 2020-10-22: para apagar tags dos posts é possível usar o seguinte comando `sed -e "s/\[ \"tag\"[,]*/[/" -e "s/[,]* \"tag\" ]/ ]/" -e "s/, \"tag\", /, /"`.

 - 2020-10-26: para dar sleep em C++ moderno com resolução de milissegundos é `std::this_thread::sleep_for(std::chrono::milliseconds(200));`. É tão intuitivo que achei preciso anotar.

 - 2020-11-21: para eliminar qualquer nova-linha em uma string C após ler uma linha da entrada usar uma desconhecida função padrão: `buffer[strcspn(buffer, "\r\n")] = 0;`.

 - 2020-12-08: é possível abrir uma VPN pelo WSL e direcionar a porta RDP de uma máquina remota para uma porta local no Windows e acessar máquinas via RDP em uma porta local. Para a VPN uso o Openvpn. Para o redirect um projeto no GitHub chamado [rinetd].

 - 2021-11-22: quando fechamos um socket no Windows na parte cliente ele fica uns 4 minutos ainda no estado TIME_WAIT, o que está certo de acordo com o protocolo TCP/IP. No entanto, nem sempre a arquiteta consegue dar conta desse comportamento, como quando infinitas conexões e desconexões estão acontecendo. Nesse caso o jeito é liberar o socket o mais rápido possíve. E existe um jeito, bem específico. É preciso setar a opção de linger como 0, chamar o shutdown apenas para RECEIVE e aí sim fechar a conexão. Funciona de imediato. Porém, deve ser usado com moderação, pois um socket que está rapidamente disponível para o sistema pode conter bytes vindos da outra ponta que serão ainda processados. Eu não sei se isso pode ocorrer de fato, foi uma observação que vi no fórum onde encontrei a solução do linger. Fica o aviso.

[rinetd]: https://github.com/boutell/rinetd
[jardim digital]: https://joelhooks.com/digital-garden
