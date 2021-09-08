#ifndef __SecOC__
#define __SecOC__

#include "Std_Types.h"
#include "ComStack_Types.h"

uint8 apduTx[];
uint8 spduTx[];

int pduId;
uint8 apduRx[];
uint8 spduRx[];

Std_ReturnType SecOC_IfTransmit(PduIdType TxPduId, const PduInfoType *PduInfoPtr);
void SecOC_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);
void  SecOC_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

#endif