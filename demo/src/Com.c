#include "Com.h"
#include "PduR_Com.h"

PduInfoType pdus[2][8] = {
    1, 1, 1, 1, 1, 1, 0, 0,
    2, 2, 2, 2, 2, 2, 0, 0};

void send_message(PduIdType TxPduId)
{
    if (TxPduId >= 2)
    {
        return;
    }
    PduInfoType temp;
    temp.SduDataPtr = pdus + TxPduId;
    temp.SduLength = 6; // 暂取6
    PduR_ComTransmit(TxPduId, &temp);
}

void Com_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    if (result == E_OK)
    {
        // ...
    }
    else
    {
        // ...
    }
}

void Com_RxIndication(PduIdType RxPduId, const PduInfoType *PduInfoPtr)
{
    // 表示收到数据
}
