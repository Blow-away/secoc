/*
 * PduR_Com.c
 *
 *  Created on: 2021Äê9ÔÂ6ÈÕ
 *      Author: zhao chenyang
 */
#include "PduR_Com.h"
#include "SecOC.h"

Std_ReturnType PduR_ComTransmit(PduIdType TxPduId, const PduInfoType *PduInfoPtr){
	return SecOC_IfTransmit(TxPduId, PduInfoPtr);
}
