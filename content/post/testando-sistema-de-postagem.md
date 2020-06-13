---
categories: [ "code" ]
date: "2016-04-10"
tags: [ "draft",  ]
title: "Testando sistema de postagem"
---
Bom, depois de criar um script para basicamente apenas escrever o texto dos filmes que assisto e buscar uma imagem agradável para meu finado blogue de Cinema, o próximo passo foi portar esse mesmo método para meus dois outros blogues: o da minha empresa, a BitForge e esse aqui. O processo envolve algo a mais: buscar as imagens usadas (que muitas vezes não é só uma). Porém, nada mais que isso.

O problema mesmo é publicar nas redes sociais. Um detalhe típico do funcionamento dessas redes bem apontou o blogger veterano Hossein Derakhshan, que ficou preso por seis anos e descreveu a mudança que a web sofreu nesse pouquíssimo tempo para a história, mas muitíssimo para a internet. De acordo com ele, postar apenas links não farão muito efeito, mesmo que você seja um escritor conhecido (o caso dele). Para fazer efeito, você precisa de imagens. Pessoas gostam de imagens. De gatinhos, melhor ainda.

Porém, qual imagem que pode ser usada para um blogue técnico e que chame a atenção?

No Cine Tênis Verde fica fácil achar uma imagem, pois filmes são formados por elas (cerca de 170 mil delas, se for um filme de duas horas). Aqui no Blogue do Caloni, tenho que me limitar a abstrações e metáforas.

O que muitas vezes tem funcionado, como minha série Básico do Básico:

 - Binário
 - Tipos
 - Ponteiros
 - Assembly
 - Programação
 - Depuração

De qualquer forma, posso continuar utilizando o título do artigo como base para minha pesquisa.

Postar no Twitter é algo relativamente fácil. O script abaixo faz isso com dois pés no joelho:

    def PublishToTwitter(postInfo):
        """
        https://pypi.python.org/pypi/twitter
        """
        t = twitter.Twitter(auth=twitter_credentials.auth)
        
        with open("C:\\daytoday\\caloni.github.io\\images\\" + postInfo["permalink"] + ".jpg", "rb") as imagefile:
        	imagedata = imagefile.read()
        t_up = twitter.Twitter(domain='upload.twitter.com', auth=twitter_credentials.auth)
        id_img1 = t_up.media.upload(media=imagedata)["media_id_string"]
        st = postInfo['title'] + '\n\n' + postInfo['tagline'] + '\n\n' + postInfo['shortlink'].encode('utf-8')
        if len(st) > 120: # giving space to image attachment
            st = stars + ' ' + postInfo['title'] + '\n\n' + '\n\n' + postInfo['shortlink'].encode('utf-8')
        t.statuses.update(status=st, media_ids=",".join([id_img1]))

Já postar no Facebook é mais ou menos uma tortura. As chaves de acesso costumam expirar, e para conseguir uma que não expira este tutorial é femonenal, pois economiza muito, muito tempo de pesquisa.

Curiosamente, o código para postar é muito semelhante ao do Twitter, até mais simples, talvez:

    def PublishToFacebook(postInfo):
        """
        http://nodotcom.org/python-facebook-tutorial.html
        """
        with open("C:\\daytoday\\caloni.github.io\\images\\" + postInfo["permalink"] + ".jpg", "rb") as imagefile:
        	imagedata = imagefile.read()
    
        st = postInfo['title'] + '\n\n' + postInfo['paragraph'] + '\n\n' + baseUrl + postInfo['permalink']
        post = facebook_credentials.auth.put_photo(image=imagedata, message=st)

