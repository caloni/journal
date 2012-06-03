#include "CObjectPrivate.h"



void Init_CObjectPrivate(CObject *pThis)
{
   pThis->m_private->m_tCreation = time(NULL);
}
