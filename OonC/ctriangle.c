#include "CTriangle.h"
#include "CShapeProtected.h"
#include <stdio.h>
#include <stdlib.h>



/** Funcoes membro */
void CTriangle_m_Draw();



void Init_CTriangle(CTriangle *pThis)
{
   Init_CShape(&pThis->m_upper);
   OVERLOAD(m_upper.m_upper, CTriangle);
   OVERLOAD_FUNCTION(CTriangle, m_upper, m_Draw);

   pThis->m_upper.m_protected->m_nSides = 3;
}



void Terminate_CTriangle(CTriangle *pThis)
{
}



DEFINE_ALLOCATORS(CTriangle)



void CTriangle_m_Draw()
{
   printf("\n    *     "
          "\n   * *    "
          "\n  *   *   "
          "\n *******\n");
}
