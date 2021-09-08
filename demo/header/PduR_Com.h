#ifndef __PduR_Com__
#define __PduR_Com__

#include "Std_Types.h"
#include "ComStack_Types.h"

Std_ReturnType PduR_ComTransmit(PduIdType TxPduId, const PduInfoType *PduInfoPtr);
#endif