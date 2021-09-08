#include "CanIf.h"
#include "PduR_CanIf.h"

Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType *PduInfoPtr)
{
    pduId = TxPduId;
    // 将PduInfoPtr的数据存储进入 spdu数组
    // src: PduInfoPtr
    // dst: spdu
    uint8 len = PduInfoPtr->SduLength;
    uint8 *src = PduInfoPtr->SduDataPtr;
    uint8 *dst = spdu;
    memcpy(dst, src, len);
    return E_OK;
}

// 发送结果
void send_result(Std_ReturnType result)
{
    PduR_CanIfTxConfirmation(pduId, result);
}
