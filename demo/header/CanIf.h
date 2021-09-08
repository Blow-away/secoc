/*
 * CanIf.h
 *
 *  Created on: 2021Äê9ÔÂ6ÈÕ
 *      Author: zhao chenyang
 */

#ifndef CANIF_H_
#define CANIF_H_

#include "Std_Types.h"
#include "ComStack_Types.h"

extern uint8 spdu[];
extern int len[];

void send_result(PduIdType TxPduId, Std_ReturnType result);

Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType *PduInfoPtr);


#endif /* CANIF_H_ */
