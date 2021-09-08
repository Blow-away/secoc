#ifndef __PduR_CanIf__
#define __PduR_CanIf__

#include "Std_Types.h"
#include "ComStack_Types.h"

void PduR_CanIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result);
void PduR_CanIfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

#endif