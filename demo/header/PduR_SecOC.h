#ifndef __PduR_SecOC__
#define __PduR_SecOC__

#include "Std_Types.h"
#include "ComStack_Types.h"

//PDU的传输请求。
Std_ReturnType PduR_SecOCTransmit(PduIdType TxPduId, const PduInfoType *PduInfoPtr);

//下层通信接口模块确认PDU的传输或传输PDU失败。
void PduR_SecOCIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result);

void PduR_SecOCIfRxIndication(PduIdType RxPduId,const PduInfoType* PduInfoPtr);

#endif