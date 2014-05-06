/**
 * Orientacao a Objetos com linguagem C.
 *
 * Esse cabecalho permite que se programe na linguagem C utilizando varios
 * conceitos da orientacao a objetos, como heranca e polimorfismo.
 *
 * @author Wanderley Caloni Jr. <wanderley.caloni@ig.com.br>
 * @date 04-2004
 */
#ifndef OONC_D53D60D9_1F91_454f_B690_EC9F6527D110
#define OONC_D53D60D9_1F91_454f_B690_EC9F6527D110



/**
 * ALOCACAO DINAMICA.
 */
#define DECLARE_OPERATOR_NEW(CLASS) \
CLASS *New_ ## CLASS()

#define DECLARE_OPERATOR_DELETE(CLASS) \
void Delete_ ## CLASS(CLASS *pThis)

#define DECLARE_OPERATOR_INTERNAL_DELETE(CLASS) \
void InternalDelete_ ## CLASS (void *pThis)

#define DEFINE_OPERATOR_NEW(CLASS) \
CLASS *New_ ## CLASS () \
{ \
   CLASS *pThis = (CLASS*) malloc(sizeof(CLASS)); \
   Init_ ## CLASS (pThis); \
   return pThis; \
} \

#define DEFINE_OPERATOR_DELETE(CLASS) \
void Delete_ ## CLASS (CLASS *pThis) \
{ \
   Terminate_ ## CLASS (pThis); \
   free(pThis); \
} \

#define DEFINE_OPERATOR_INTERNAL_DELETE(CLASS) \
void InternalDelete_ ## CLASS (void *pThis) \
{ \
   Delete_ ## CLASS((CLASS*) pThis); \
}

#define DECLARE_ALLOCATORS(CLASS) \
DECLARE_OPERATOR_NEW(CLASS); \
DECLARE_OPERATOR_DELETE(CLASS); \
DECLARE_OPERATOR_INTERNAL_DELETE(CLASS);

#define DEFINE_ALLOCATORS(CLASS) \
DEFINE_OPERATOR_NEW(CLASS) \
DEFINE_OPERATOR_DELETE(CLASS) \
DEFINE_OPERATOR_INTERNAL_DELETE(CLASS)



/**
 * CONSTRUCAO, HERANCA e POLIMORFISMO.
 * Inicializacao e sobrecarga de funcoes-membro da classe.
 */
#define INIT(CLASS) \
   pThis->m_this = pThis; \
   pThis->m_delete = InternalDelete_ ## CLASS;

#define OVERLOAD(WHERE, CLASS) \
   pThis-> WHERE .m_this = pThis; \
   pThis-> WHERE .m_delete = InternalDelete_ ## CLASS;

#define INIT_FUNCTION(CLASS, FUNC) \
   pThis-> FUNC = CLASS ## _ ## FUNC
#define OVERLOAD_FUNCTION(CLASS, WHERE, FUNC) \
   pThis-> WHERE . FUNC = CLASS ## _ ## FUNC

#endif /*OONC_D53D60D9_1F91_454f_B690_EC9F6527D110*/
