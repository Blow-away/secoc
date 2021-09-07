/*
 * Com.h
 *
 *  Created on: 2021Äê9ÔÂ6ÈÕ
 *      Author: zhao chenyang
 */

#ifndef COM_H_
#define COM_H_

#include "Std_Types.h"
#include "ComStack_Types.h"

void send_message(PduIdType TxPduId);

void Com_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);

void Com_RxIndication(PduIdType RxPduId, const PduInfoType *PduInfoPtr);


#endif /* COM_H_ */
