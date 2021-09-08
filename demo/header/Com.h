#ifndef __Com__
#define __Com__

#include "Std_Types.h"
#include "ComStack_Types.h"

#define TIME 5

void send_message(PduIdType TxPduId);

void Com_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);

void Com_RxIndication(PduIdType RxPduId, const PduInfoType *PduInfoPtr);
#endif