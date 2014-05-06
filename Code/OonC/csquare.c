#include "CSquare.h"
#include "CShapeProtected.h"
#include <stdio.h>
#include <stdlib.h>



/** Funcoes membro. */
void CSquare_m_Draw();



void Init_CSquare(CSquare *pThis)
{
   Init_CShape(&pThis->m_upper);
   OVERLOAD(m_upper.m_upper, CSquare);
   OVERLOAD_FUNCTION(CSquare, m_upper, m_Draw);
   
   pThis->m_upper.m_protected->m_nSides = 4;
}



void Terminate_CSquare(CSquare *pThis)
{
   Terminate_CShape(&pThis->m_upper);
}



DEFINE_ALLOCATORS(CSquare)



void CSquare_m_Draw(CShape *pThis)
{
   printf("\n**********"
          "\n*        *"
          "\n*        *"
          "\n*        *"
          "\n**********\n");
}
