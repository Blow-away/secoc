/*
 * PduR_SecOC.c
 *
 *  Created on: 2021Äê9ÔÂ7ÈÕ
 *      Author: zhao chenyang
 */

#include "PduR_SecOC.h"
#include "CanIf.h"
#include "Com.h"
#include "SecOC.h"

Std_ReturnType PduR_SecOCTransmit(PduIdType TxPduId , const PduInfoType* PduInfoPtr){
	return CanIf_Transmit(TxPduId, PduInfoPtr);
}


void PduR_SecOCIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result) {
    Com_TxConfirmation(TxPduId, result);
}

void PduR_SecOCIfRxIndication(PduIdType RxPduId,const PduInfoType* PduInfoPtr){
	Com_RxIndication(RxPduId,PduInfoPtr);
}
