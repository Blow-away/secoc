#include "PduR_Com.h"
#include "SecOC_Types.h"
#include "SecOC_Cfg.h"
#include "SecOC.h"

type_t idmode[] = {drirect, trigger, tp, upperTp}; // 0 表示用SecOC_IfTransmit发， 1 表示用SecOC_TpTransmit

// 调用PduR模块的SecOC_IfTransmit
Std_ReturnType PduR_ComTransmit(PduIdType TxPduId, const PduInfoType *PduInfoPtr) {
    if (TxPduId >= sizeof(idmode))
    {
        return E_NOT_OK;
    }
    else
    {
        if (idmode[TxPduId] == drirect || idmode[TxPduId] == trigger || idmode[TxPduId] == tp)
        {
            return SecOC_IfTransmit(TxPduId, PduInfoPtr);
        }
        else if (idmode[TxPduId] == upperTp)
        {
            // return SecOC_TpTransmit(TxPduId, PduInfoPtr );
            return E_OK;
        }
        return E_NOT_OK;
    }
}
