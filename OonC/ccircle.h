#ifndef CCIRCLE_171EF6C5_E851_4e09_BFA2_41FA62330531
#define CCIRCLE_171EF6C5_E851_4e09_BFA2_41FA62330531

#include "CShape.h"

typedef struct StCircle
{
   CShape m_upper;
   
} CCircle;

void Init_CCircle(CCircle *pThis);
void Terminate_CCircle(CCircle *pThis);
DECLARE_ALLOCATORS(CCircle)

#endif /*CCIRCLE_171EF6C5_E851_4e09_BFA2_41FA62330531*/
