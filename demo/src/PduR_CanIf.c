#include "PduR_CanIf.h"
#include "SecOC.h"

void PduR_CanIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result) {
    //	if(mode == 0){
    //		SecOC_TxConfirmation( PduIdType TxPduId, Std_ReturnType result );
    //	}
    //	else if(mode == 1){		// I-PDU from the COM module to the CanIf module
    //		Com_TxConfirmation(PduIdType, Std_ReturnType);
    //	}
    //	else if(mode == 2){		// an I-PDU is gatewayed between two CAN networks (CAN1 and CAN2) using CanIf.	/		 an I-PDU is gatewayed from CAN1 to CAN2 and also received locally by the COM module.
    //		// do none
    //	}
    SecOC_TxConfirmation(TxPduId, result);
}

void PduR_CanIfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr){
    SecOC_RxIndication(RxPduId,PduInfoPtr);
}