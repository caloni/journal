/** @file VS2013CPP11.h
@brief Ponto de in�cio dos testes de compila��o.

@desc Optei por usar o modelo de Unit Test (com a ajuda da LIB unit--) onde n�o existe ponto de in�cio
dos testes (o main fica dentro da LIB). Dessa forma, qualquer CPP pode ser inclu�do e dentro dele os testes
que devem ser realizados. Todos os CPPs devem incluir o header do unit-- para conseguirem criar suas su�tes
de teste.
*/
#pragma once
#include "..\unit--\unit--.h"
