#ifndef COBJECT_FBA54CF8_A4FB_495c_8AB6_1DF1D280BCC4
#define COBJECT_FBA54CF8_A4FB_495c_8AB6_1DF1D280BCC4

#include "OOnC.h"



/**
 * Definicao da classe.
 */
typedef struct StObject
{
   void *m_this;
   void (*m_delete)(void *pThis);
   
   struct StObjectPrivate *m_private;
   void (*m_PrintCreationTime)(struct StObject *pThis);

} CObject;


/** Operadores comuns. */
void Init_CObject(CObject *pThis);
void Terminate_CObject(CObject *pThis);
DECLARE_ALLOCATORS(CObject)

#endif /*COBJECT_FBA54CF8_A4FB_495c_8AB6_1DF1D280BCC4*/
