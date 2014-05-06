#ifndef CSHAPE_436006ED_7A25_4f76_98ED_D52810B9EC7C
#define CSHAPE_436006ED_7A25_4f76_98ED_D52810B9EC7C

#include "CObject.h"

typedef struct StShape
{
   CObject m_upper; /* Classe-mae. */
   
   int (*m_GetNumberOfSides)();
   void (*m_Draw)();

   struct StShapeProtected *m_protected;

} CShape;

void Init_CShape(CShape *pThis);
void Terminate_CShape(CShape *pThis);
DECLARE_ALLOCATORS(CShape)

#endif /*CSHAPE_436006ED_7A25_4f76_98ED_D52810B9EC7C*/
