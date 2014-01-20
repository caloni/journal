A surpresa de hoje foi descobrir (vejam só) que o remove_if, como todo algoritmo da STL, deve ser olhado de perto antes de usado. Nesse caso em específico porque, apesar do nome, a função NÃO remove elementos, mas os sobrescreve.

Imagine uma função que usa remove_if para remover todas as idades de potenciais lolitas:

[github,RemoveIf/RemoveIfVector.cpp,cpp]

Ou até sua contraparte usando um array C:

[github,RemoveIf/RemoveIfArray.cpp,cpp]

Um uso trivial pode não cuspir um resultado trivial, ou seja, os elementos não serão removidos como se espera:

[github,RemoveIf/RemoveIfErrado.cpp,cpp]

<a href="http://www.flickr.com/photos/caloni/12056971746/" title="RemoveIfErrado por caloni, no Flickr"><img src="http://farm4.staticflickr.com/3720/12056971746_f5852acde2_o.png" alt="RemoveIfErrado"></a>

Isso ocorre porque o comportamento do remove_if é copiar todos os elementos que retornem false (não remova) e pular elementos que retornem true (remova). No entanto, o tamanho do contêiner, e consequentemente seu ponteiro end(), permanecem o mesmo.

<a href="http://www.flickr.com/photos/caloni/12056450103/" title="RemoveIfComportamento por caloni, no Flickr"><img src="http://farm6.staticflickr.com/5485/12056450103_9cc50c00e2_o.png" alt="RemoveIfComportamento"></a>

Para obtermos qual seria o "novo end()", precisamos obter esse valor do retorno de remove_if. Com base nisso, podemos alterar o tamanho do contêiner ajustado:

[github,RemoveIf/RemoveIf.cpp,cpp]
