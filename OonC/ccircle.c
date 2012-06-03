#include "CCircle.h"
#include "CShapeProtected.h"
#include <stdio.h>
#include <stdlib.h>



/** Funcoes membro */
void CCircle_m_Draw();



void Init_CCircle(CCircle *pThis)
{
   Init_CShape(&pThis->m_upper);
   OVERLOAD(m_upper.m_upper, CCircle);
   OVERLOAD_FUNCTION(CCircle, m_upper, m_Draw);
   
   pThis->m_upper.m_protected->m_nSides = 0;
}



void Terminate_CCircle(CCircle *pThis)
{
   Terminate_CShape(&pThis->m_upper);
}



DEFINE_ALLOCATORS(CCircle)



void CCircle_m_Draw()
{
   printf("\n     *  *  *     "
          "\n  *           *  "
          "\n *             * "
          "\n*               *"
          "\n*               *"
          "\n *             * "
          "\n  *           *  "
          "\n     *  *  *   \n");
}
