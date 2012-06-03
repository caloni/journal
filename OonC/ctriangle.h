#ifndef CTRIANGLE_1A16F772_D179_4c39_9745_04E082E43EBF
#define CTRIANGLE_1A16F772_D179_4c39_9745_04E082E43EBF

#include "CShape.h"

typedef struct StTriangle
{
   CShape m_upper;

} CTriangle;

void Init_CTriangle(CTriangle *pThis);
void Terminate_CTriangle(CTriangle *pThis);
DECLARE_ALLOCATORS(CTriangle)

#endif /*CTRIANGLE_1A16F772_D179_4c39_9745_04E082E43EBF*/
