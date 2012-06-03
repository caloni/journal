#include "CShape.h"
#include "CShapeProtected.h"
#include <stdio.h>
#include <stdlib.h>



/** Funcoes membro. */
int CShape_m_GetNumberOfSides(CShape *pThis);
void CShape_m_Draw();



void Init_CShape(CShape *pThis)
{
   Init_CObject(&pThis->m_upper);
   OVERLOAD(m_upper, CShape);

   INIT_FUNCTION(CShape, m_GetNumberOfSides);
   INIT_FUNCTION(CShape, m_Draw);
   
   pThis->m_protected = (CShapeProtected*) malloc(sizeof(CShapeProtected));
   pThis->m_protected->m_nSides = 0;
}



void Terminate_CShape(CShape *pThis)
{
   free(pThis->m_protected);
   Terminate_CObject(&pThis->m_upper);
}



DEFINE_ALLOCATORS(CShape)



int CShape_m_GetNumberOfSides(CShape *pThis)
{
   return pThis->m_protected->m_nSides;
}



void CShape_m_Draw()
{
   printf("\n*shapeless*\n");
}
