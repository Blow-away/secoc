/*
 * PduR_CanIf.h
 *
 *  Created on: 2021Äê9ÔÂ7ÈÕ
 *      Author: zhao chenyang
 */

#ifndef PDUR_CANIF_H_
#define PDUR_CANIF_H_

#include "Std_Types.h"
#include "ComStack_Types.h"

void PduR_CanIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result);
void PduR_CanIfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

#endif /* PDUR_CANIF_H_ */
