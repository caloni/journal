#include "CObject.h"
#include "CObjectPrivate.h"
#include <stdlib.h>
#include <stdio.h>


/** Funcoes membro. */
void CObject_m_PrintCreationTime(struct StObject *pThis);


void Init_CObject(CObject *pThis)
{
   INIT(CObject)
   INIT_FUNCTION(CObject, m_PrintCreationTime);
   
   pThis->m_private = (CObjectPrivate*) malloc(sizeof(CObjectPrivate));
   Init_CObjectPrivate(pThis);
}



void Terminate_CObject(CObject *pThis)
{
   free(pThis->m_private);
}



void CObject_m_PrintCreationTime(struct StObject *pThis)
{
   printf("%d\n", (int) pThis->m_private->m_tCreation);
}



DEFINE_ALLOCATORS(CObject)
