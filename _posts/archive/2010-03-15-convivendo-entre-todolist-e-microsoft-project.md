---
author: admin
comments: true
date: 2010-03-15 09:00:14+00:00
layout: post
slug: convivendo-entre-todolist-e-microsoft-project
title: Convivendo entre TodoList e Microsoft Project
wordpress_id: 858
categories:
- Ferramentas
- Projetos
---

O próximo artigo sobre escovação de bits ainda está no forno. Tirar férias (de 40 dias) é uma escassez de ideias! No momento, posso explicar a facilidade que tive para continuar usando o [TodoList](http://www.caloni.com.br/blog/todolist) para gerenciar minha equipe e ainda assim sincronizar nossas tarefas em um cronograma do **Microsoft Project**.

As razões de eu usar o TodoList são meio óbvias: ele faz tudo que eu preciso para organizar minhas tarefas do dia-a-dia e é [portátil](http://www.caloni.com.br/blog/sdelete). Enquanto isso, o Project, além de não ser portátil (eu preciso levar comigo o instalador de 200 MB? E Instalar?) possui um formato difícil de mudar, já que foi feito para projetar o mundo e não para ser [compartilhado facilmente](http://pt.wikipedia.org/wiki/XML).



Mas vamos lá. Tudo que precisamos é de uma edição atual do TodoList e do Microsoft Project. A primeira coisa que devemos fazer é exportar as tarefas que queremos do TodoList para um [CSV](http://pt.wikipedia.org/wiki/Comma-separated_values) padrão, usando as colunas que gostaríamos de importar para o Project:

![tarefas-no-todolist.png](/images/tarefas-no-todolist.png)

![exportar-do-todolist.png](/images/exportar-do-todolist.png)

![tarefas-no-excel.png](/images/tarefas-no-excel.png)

Depois vem a parte complicada, mas nem tanto. Abrimos o projeto para onde queremos importar essas tarefas e **escolhemos a opção Abrir novamente**, só que dessa vez selecionando o nosso amigo tarefas-exportadas.**CSV**.

![project-limpo.png](/images/project-limpo.png)

Só que antes de importarmos, calma lá. **Temos que criar uma nova coluna que irá guardar os IDs das tarefas do TodoList**, para que nas próximas importações consigamos mesclar os dados já existentes. Portanto, crie uma nova coluna (pode ser qualquer NúmeroX não-alocado ainda) com um nome significativo.

Agora podemos partir para a importação. Imaginando que seja a primeira, vamos criar um mapeamento inicial para essa primeira migração:

![novo-mapa.png](/images/novo-mapa.png)

Na hora de escolher quem é que, só precisamos definir quais colunas no Project correspondem a quais colunas do TodoList, e lembrar de alocar o ID na nossa coluna especial.

![mapeamento.png](/images/mapeamento.png)

![salvar-mapa.png](/images/salvar-mapa.png)

Mais alguns Next da vida e pronto! Temos nossas tarefas devidamente importadas.

![project-cheio.png](/images/project-cheio.png)


#### Mesclando dados


Mas é claro que todo esse trabalho não valeria a pena se tivéssemos que (arght) mexer no Project. Para evitar esse trabalho impuro, continuamos atualizando o andamento dos projetos no nosso pequeno, leve e sagaz TodoList e, quando precisarmos, é só importarmos novamente os dados, só que dessa vez **usando um mapa já salvo** (siga os screenshots acima) e **marcando nosso ID do TodoList como chave**. Dessa forma as tarefas já importadas são apenas atualizadas, e não criadas novamente. Esse é o famoso "[pulo do gato](http://radiobandeirantes.com.br/sobre.asp?PDT=28&ID=91)" (que eu ouvia matinalmente na minha época de office-boy).

![usar-mapa-existente.png](/images/usar-mapa-existente.png)

![definir-chave.png](/images/definir-chave.png)


#### Boa notícia


Depois de eu pesquisar toda essa trama, descobri que o uso do Project não será necessário. Sorte minha. Agora, se você não tiver sorte...
