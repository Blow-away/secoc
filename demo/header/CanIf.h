#ifndef __CanIf__
#define __CanIf__
#include "Std_Types.h"
#include "ComStack_Types.h"

#define TIME 5

int pduId;
uint8 spdu[];

void send_result(Std_ReturnType result);

Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType *PduInfoPtr);
#endif