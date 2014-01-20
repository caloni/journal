A surpresa de hoje foi descobrir (vejam s�) que o remove_if, como todo algoritmo da STL, deve ser olhado de perto antes de usado. Nesse caso em espec�fico porque, apesar do nome, a fun��o N�O remove elementos, mas os sobrescreve.

Imagine uma fun��o que usa remove_if para remover todas as idades de potenciais lolitas:

[github,RemoveIf/RemoveIfVector.cpp,cpp]

Ou at� sua contraparte usando um array C:

[github,RemoveIf/RemoveIfArray.cpp,cpp]

Um uso trivial pode n�o cuspir um resultado trivial, ou seja, os elementos n�o ser�o removidos como se espera:

[github,RemoveIf/RemoveIfErrado.cpp,cpp]

<a href="http://www.flickr.com/photos/caloni/12056971746/" title="RemoveIfErrado por caloni, no Flickr"><img src="http://farm4.staticflickr.com/3720/12056971746_f5852acde2_o.png" alt="RemoveIfErrado"></a>

Isso ocorre porque o comportamento do remove_if � copiar todos os elementos que retornem false (n�o remova) e pular elementos que retornem true (remova). No entanto, o tamanho do cont�iner, e consequentemente seu ponteiro end(), permanecem o mesmo.

<a href="http://www.flickr.com/photos/caloni/12056450103/" title="RemoveIfComportamento por caloni, no Flickr"><img src="http://farm6.staticflickr.com/5485/12056450103_9cc50c00e2_o.png" alt="RemoveIfComportamento"></a>

Para obtermos qual seria o "novo end()", precisamos obter esse valor do retorno de remove_if. Com base nisso, podemos alterar o tamanho do cont�iner ajustado:

[github,RemoveIf/RemoveIf.cpp,cpp]
