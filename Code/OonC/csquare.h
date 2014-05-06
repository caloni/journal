#ifndef CSQUARE_ECD6884E_4FBC_42e8_969F_7D0B8E4D4F5E
#define CSQUARE_ECD6884E_4FBC_42e8_969F_7D0B8E4D4F5E

#include "CShape.h"

typedef struct StSquare
{
   CShape m_upper;

} CSquare;

void Init_CSquare(CSquare *pThis);
void Terminate_CSquare(CSquare *pThis);
DECLARE_ALLOCATORS(CSquare)

#endif /*CSQUARE_ECD6884E_4FBC_42e8_969F_7D0B8E4D4F5E*/
