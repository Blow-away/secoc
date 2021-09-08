#include "PduR_SecOC.h"
#include "CanIf.h"
#include "Com.h"

uint8 testpdur[2] = {9, 9};

void PduR_SecOCIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result) {
    Com_TxConfirmation(TxPduId, result);

    testpdur[0] = TxPduId;
    testpdur[1] = result;
    return;
}

int Com_type[] = {CanIF, CanIF};

// SecOC模块
Std_ReturnType PduR_SecOCTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr) {
	//依据TxPduId进行分配路径

	if (Com_type[TxPduId] == CanIF){   //direct / re_auth
		return CanIf_Transmit(TxPduId, PduInfoPtr);
	} else if (Com_type[TxPduId] == FrIF){ //triggered trans
		return FrIf_Transmit(TxPduId, PduInfoPtr);
	} else if (Com_type[TxPduId] == canTP){  //tp
		return CanTp_Transmit(TxPduId, PduInfoPtr);
	}
//	else if(mode==3){  //upper layer transport protocol
//		return CanTp_Transmit(TxPduId, PduInfoPtr);
//	}
}


void PduR_SecOCIfRxIndication(PduIdType RxPduId,const PduInfoType* PduInfoPtr){
	Com_RxIndication(RxPduId,PduInfoPtr);
}


Std_ReturnType SecOC_IfCancelTransmit(PduIdType TxPduId) {}
void PduR_TpCancelTransmit(PduIdType TxPduId) {}
PduR_TpCancelReceive(PduIdType TxPduId) {}