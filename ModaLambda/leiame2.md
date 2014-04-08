<a href="https://www.flickr.com/photos/120157483@N04/13717451604/" title="lambda-o-retorno por wanderleycaloni, no Flickr"><img src="https://farm4.staticflickr.com/3796/13717451604_33225e217c_o.jpg" alt="Lambda: o Retorno"></a>

Na �ltima vez que foi abordado o tema "lambda na ferida" falamos brevemente sobre como C++ agora permite criar fun��es dentro de fun��es. Hoje vamos apenas falar que aquela constru��o bizarra que criamos fica ainda mais bizarra se precisarmos retornar alguma coisa dessa fun��o ou us�-la mais de uma vez.

O padr�o do lambda � supor que sua fun��o embutida e enlatada n�o precisa retornar nada, o que torna a sintaxe mais simples: � um void AlgumaCoisa(argumentos). No entanto, para algoritmos como o find_if isso n�o funciona, ent�o � necess�rio retornar algo. E, no caso de find_if, cham�-lo mais de uma vez pode ser feito facilmente criando uma vari�vel lambda:

[github,ModaLambda/LambdaReturn.cpp,cpp]

O tipo de retorno que colocamos atrav�s de uma flechinha � obrigat�ria? De fato, n�o. Se eu omiti-la vai funcionar do mesmo jeito porque o �nico ponto de sa�da da minha fun��o retorna um bool.

Esses compiladores est�o ficando cada vez mais espertos.
