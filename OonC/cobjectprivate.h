#ifndef COBJECTPRIVATE_3038926E_F7DD_4262_81DA_0FF6945D9341
#define COBJECTPRIVATE_3038926E_F7DD_4262_81DA_0FF6945D9341
#include "CObject.h"
#include <time.h>

typedef struct StObjectPrivate
{
   time_t m_tCreation;

} CObjectPrivate;

void Init_CObjectPrivate(CObject *pThis);

#endif /*COBJECTPRIVATE_3038926E_F7DD_4262_81DA_0FF6945D9341*/
