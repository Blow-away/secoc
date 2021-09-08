#include "SecOC.h"
#include "SecOC_Types.h"
#include "SecOC_Cfg.h"
#include "PduR_SecOC.h"
#include "Csm.h"
#include "slaveFVM.h"

extern SecOCintermediate_type SecOCintermediateTx[];
extern SecOCintermediateRx_type SecOCintermediateRx[];
const SecOCTxPduProcessing_type *tmpSecOCTxPduProcessing;
const SecOCRxPduProcessing_type *tmpSecOCRxPduProcessing;
SecOC_StateType _secOCState = SECOC_UNINIT;

/*
00313 若SecOCSecuredTxPduLength + SecOCSecuredTxPduOffset >tx PduInfoPtr->SduLength. pdu丢弃， 返回 e not ok
58: 连续区域存储a pdu 和 athentication information
31: 计算athentication information = authenticator, tcfv
60: 在main scheduled中根据31描述的方法计算authenticator
61: 在main scheduled中生成spdu
62: 为PduR_SecOCTransmit提供完成spdu
63: pduR通过 SecOC_[If|Tp]TxConfirmation 反馈低层结果（确认传输/错误），secoc把结果上报上层PduR_SecOC[If|Tp]TxConfirmation
64： SecOC_TxConfirmation通知后释放对应spdu
*/
//Std_ReturnType SecOC_IfTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
Std_ReturnType SecOC_IfTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr) {
    //	VALIDATE_STATE_INIT_RETURN();

    // 判断SecOC是否已初始化(_secOCState)
    if (SECOC_INIT != _secOCState)
    {
        return E_NOT_OK; // 未初始化
    }

    // 判断id合法性，超过发送报文的配置数量
    if (TxPduId >= SECOC_NUM_OF_TX_IPDU)
    {
        return E_NOT_OK; //非secoc内txpduid
    }

    //	if(tmpSecOCTxPduProcessing[TxPduId]->SecOCTxPduSecuredArea.SecOCSecuredTxPduOffset+tmpSecOCTxPduProcessing[TxPduId]->SecOCTxPduSecuredArea.SecOCSecuredTxPduLength > PduInfoPtr->SduLength){
    //		return E_NOT_OK;
    //	}

    // 拷贝数据
    // src: SecOCintermidate[TxPduId]
    // dst: PduInfoPtr
    uint8 len = PduInfoPtr->SduLength;
    uint8 *src = PduInfoPtr->SduDataPtr;

    SecOCintermediate_type intermediate = SecOCintermediateTx[TxPduId];
    uint8 *dst = apduTx + intermediate.addr_st;

    intermediate.apduBlen = len;
    memcpy(dst, src, len);

    /*
	在tx-Ipdu存储区复制PduInfoPtr的所指向的值，长度
	(这里是否会有存储判断？ 覆盖，拒绝等差别 )？？？
	*/

    return E_OK;

    //	uint8 idx=0;
    //	for(idx; idx<SECOC_NUM_OF_TX_IPDU; idx++){
    //		SecOCTxPduProcessing_type cur = tmpSecOCTxPduProcessing[num];
    //		if(cur.SecOCTxAuthenticPduLayer.SecOCTxAuthenticLayerPduId == TxPduId){
    //			cur.SecOCTxAuthenticPduLayer.SecOCTxAuthenticLayerPduRef->SduDataPtr=PduInfoPtr->SduDataPtr;
    //			cur.SecOCTxAuthenticPduLayer.SecOCTxAuthenticLayerPduRef->MetaDataPtr=PduInfoPtr->MetaDataPtr;
    //			cur.SecOCTxAuthenticPduLayer.SecOCTxAuthenticLayerPduRef->SduLength=PduInfoPtr->SduLength;
    //			return E_OK;
    //		}//覆盖
    //	}
    //	return E_NOT_OK;
}


//报文种类
type_t idmode1[] = {drirect, trigger, tp, upperTp}; // 0 表示用SecOC_IfTransmit发， 1 表示用SecOC_TpTransmit

//0x40 同步 可重入不同的PduId。不可重入相同的PduId。
//下层通信接口模块确认PDU的传输或传输PDU失败。
void SecOC_TxConfirmation(PduIdType TxPduId, Std_ReturnType result) {
    //	VALIDATE_STATE_INIT();

    // 判断SecOC是否已初始化(_secOCState)
    if (SECOC_INIT != _secOCState)
    {
        return E_NOT_OK; // 未初始化
    }

    // 判断id合法性，超过发送报文的配置数量
    if (TxPduId >= SECOC_NUM_OF_TX_IPDU)
    {
        return E_NOT_OK; //非secoc内txpduid
    }

    // 释放spdu
    SecOCintermediate_type intermediate = SecOCintermediateTx[TxPduId];
    intermediate.spduBlen = 0;

    // trigger若发送成功，则释放s-pdu缓,tcfv
    if (intermediate.type == trigger)
    {
        if (result == E_NOT_OK)
        {
            return; // 函数结束
        }
    }

    // 继续传递
    if (result == E_OK) {
        PduR_SecOCIfTxConfirmation(TxPduId, result);
    }
}

/* 
1. direct 接收数据
2. 将spdu保存在本地
*/
void  SecOC_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr){
    pduId = RxPduId;
    uint8 len = PduInfoPtr->SduLength;
    uint8 *src = PduInfoPtr->SduDataPtr;
    uint8 *dst = spduRx;
    memcpy(dst, src, len);
}


/*
67: 为PduR_SecOCTransmit提供完成spdu； 当低层准备好传输spdu，会调用PduR_<Lo>TriggerTransmit， pdur会将请求发给secoc，并在secoc中拷贝spdu
68: secoc中拷贝spdu给请求SecOC_TriggerTransmit
150: 若调用SecOC_TriggerTransmit单secoc没有可用spdu， 返回E_NOT_OK；
低层可反馈传输结果（成功/失败）通过PduR_<Lo>TxConfirmation-> SecOC_TxConfirmation, SECOC再通过pdur通知上层（发送original ipdu的源），见63.
由于上下层更新率不同，下层请求时，上层可能没更新apdu。secoc此时支持重发apdu，重新认证。
69: 成功传输，secoc调用SecOC_TxConfirmation，释放authentication info，保留apdu，重新去计算spdu。
*/
Std_ReturnType SecOC_TriggerTransmit(PduIdType TxPduId, PduInfoType PduInfoPtr) {
    // 判断SecOC是否已初始化(_secOCState)
    if (SECOC_INIT != _secOCState)
    {
        return E_NOT_OK;
    }

    // 判断id合法性，超过发送报文的配置数量
    if (TxPduId >= SECOC_NUM_OF_TX_IPDU || idmode1[TxPduId] != trigger)
    {
        return E_NOT_OK; //非secoc内txpduid, 非trigger
    }

    /*从存储区spdu获取值*/
    SecOCintermediate_type intermediate = SecOCintermediateTx[TxPduId];
    /*150 没数据  可以通过长度和指针=null判断  */
    if (intermediate.spduBlen < PduInfoPtr.SduLength)
    {
        return E_NOT_OK;
    }

    // 把传递过来的指针内空间 拷贝上spdu中的值
    uint8 *dst = PduInfoPtr.SduDataPtr;
    uint8 *src = apduTx + intermediate.addr_st;
    PduInfoPtr.SduLength = intermediate.spduBlen;
    memcpy(dst, src, PduInfoPtr.SduLength);
    // PduInfoPtr->SduLength = SecOCintermidate[TxPduId].slen;

    return E_OK;
}

BufReq_ReturnType SecOC_CopyTxData(PduIdType id, PduInfoType info, RetryInfoType retry, PduLengthType availableDataPtr) {
    // VALIDATE_STATE_INIT();
    prepare();
    // 判断secoc是否已初始化
    if (SECOC_INIT != _secOCState)
    {
        return BUFREQ_E_NOT_OK;
    }

    // 判断id合法性 是否超过发送报文的配置数量
    if (id >= SECOC_NUM_OF_TX_IPDU)
    {
        return BUFREQ_E_NOT_OK; //非secoc内txpduid
    }

    //72 73
    SecOCintermediate_type intermediate = SecOCintermediateTx[id];
    if (SecOCintermediateTx[id].spduBlen != 0)
    {
        if (info.SduLength > intermediate.spduBlen)
        {
            return BUFREQ_E_BUSY;
        }

        intermediate.slock = 1;

        uint8 *dst = (void *)info.SduDataPtr;
        uint8 *src = apduTx + intermediate.addr_st;
        memcpy(dst, src, info.SduLength);

        //需要配合动态化的空间
        availableDataPtr = intermediate.spduBlen - info.SduLength;

        return BUFREQ_OK;
    }
    else
    {
        return BUFREQ_E_NOT_OK;
    }
}

void SecOC_TpTxConfirmation(PduIdType TxPduId, Std_ReturnType result) {
    // 判断secoc是否已初始化
    if (SECOC_INIT != _secOCState)
    {
        return;
    }

    // 判断id合法性 是否超过发送报文的配置数量
    if (TxPduId >= SECOC_NUM_OF_TX_IPDU)
    {
        return; //非secoc内txpduid
    }

    SecOCintermediate_type intermediate = SecOCintermediateTx[TxPduId];
    // 释放spdu
    intermediate.spduBlen = 0; //被调用就释放
    intermediate.slock = 0;

    if (GET_TxSecOCPduType(TxPduId) == SECOC_IFPDU)
    {
        PduR_SecOCIfTxConfirmation(TxPduId, result);
    }
    if (GET_TxSecOCPduType(TxPduId) == SECOC_TPPDU)
    {
        PduR_SecOCTpTxConfirmation(TxPduId, result);
    }
}

uint8 tmpinter[64];
uint8 tmppduinfo[64];

PduInfoType tmp = {
    .SduDataPtr = &tmppduinfo
};
PduInfoType *secoctmp = &tmp;

/*
00313  若SecOCSecuredTxPduLength + SecOCSecuredTxPduOffset >tx PduInfoPtr->SduLength. pdu丢弃， 返回 e not ok
*/
Std_ReturnType SecOC_TpTransmit(PduIdType TxPduId, PduInfoType PduInfoPtr) {
    // 判断secoc是否已初始化 || 判断id合法性 是否超过发送报文的配置数量
    if (SECOC_INIT != _secOCState || TxPduId >= SECOC_NUM_OF_TX_IPDU)
    {
        return E_NOT_OK;
    }

    // can not understand
    if (tmpSecOCTxPduProcessing[TxPduId].SecOCTxPduSecuredArea.SecOCSecuredTxPduOffset
    +
    tmpSecOCTxPduProcessing[TxPduId].SecOCTxPduSecuredArea.SecOCSecuredTxPduLength 
    > 
    PduInfoPtr.SduLength) {
		return E_NOT_OK;
	}

	const PduInfoType* info = secoctmp;
	secoctmp->SduLength = PduInfoPtr.SduLength;
	BufReq_ReturnType rst= BUFREQ_E_NOT_OK;
	RetryInfoType* retry;
	PduLengthType* availableDataPtr;
	if(8 < PduInfoPtr.SduLength) return E_NOT_OK;
	while(rst!=BUFREQ_OK){
		rst = PduR_SecOCTpCopyTxData(TxPduId, info, retry, availableDataPtr);
	}

    SecOCintermediate_type intermediate = SecOCintermediateTx[TxPduId];
    // = info->SduDataPtr;
    uint8 *src = PduInfoPtr.SduDataPtr;
    uint8 *dst = apduTx + intermediate.addr_st;
    intermediate.spduBlen = PduInfoPtr.SduLength;
    memcpy(dst, src, PduInfoPtr.SduLength);

    return E_OK;
}

//0x4a 同步 可重入不同的PduId。不可重入相同的PduId。
//E_OK:目标模块成功执行取消。
//E_NOT_OK: 目标模块拒绝取消。
//请求取消较低层通信模块中正在进行的PDU传输。
Std_ReturnType SecOC_IfCancelTransmit(PduIdType TxPduId)
{
    Std_ReturnType SecOC_IfCancelTransmit(TxPduId);
}

//0x4a 同步 可重入不同的PduId。不可重入相同的PduId。
//E_OK:目标模块成功执行取消。
//E_NOT_OK: 目标模块拒绝取消。
Std_ReturnType SecOC_TpCancelTransmit(PduIdType TxPduId) {
    PduR_TpCancelTransmit(TxPduId);
}

//0x4d 同步 可重入
//SecOC利用该接口去表示Secured I-PDU已经开始进行传输到PDUR
void SecOC_SPduTxConfirmation(uint16 SecOCFreshnessValueID) {
}

/*SECOC 未初始化 调用此函数将直接返回，报错（可选）
 * 由SecOCMainFunctionPeriodTx 进行配置
 */
void SecOC_MainFunctionTx(void) {
    // SecOC是否已初始化
    if (_secOCState != SECOC_INIT)
        return;
    
    // 遍历所有apdu存储空间
    uint8 idx = 0;
    Std_ReturnType result;
    PduInfoType *PduInfoPtr;
    for (idx; idx < SECOC_NUM_OF_TX_IPDU; idx++)
    {
        SecOCintermediate_type intermediate = SecOCintermediateTx[idx];
        // 若存在值且spdu未锁
        if (intermediate.apduBlen > 0 && intermediate.slock == 0)
        {
            authenticate(intermediate, &PduInfoPtr);
            if (PduInfoPtr->SduLength > 0)
            { //authenticate 成功
                result = PduR_SecOCTransmit(idx, PduInfoPtr);
            }
        }
    }
}

void SecOC_MainFunctionRx(void) {
    // SecOC是否已初始化
    if (_secOCState != SECOC_INIT)
        return;

    for (int i = 0; i < SECOC_NUM_OF_TX_IPDU; i++) {
        SecOCintermediateRx_type SecOCintermediate = SecOCintermediateRx[i];
        if (SecOCintermediate.spduBlen > 0 && SecOCintermediate.slock == 0) {
            VerificationResultType verificationResult;
            SecOCRxPduProcessing_type curSecOCRxPduProcessing;
            verify(SecOCintermediate, verificationResult, curSecOCRxPduProcessing);
        }
    }
}

//提供空间存储要接收到的要处理的报文数据，将报文数据保存到指定空间中
void SecOC_RxIndication(PduIdType RxPduId, PduInfoType PduInfoPtr) {
	//	if(verify direct){
	//		prepare();
	//		copy();
	//	}else if(re-authenti){
	//		prepare();
	//		copyBuffer();
	//	}
	VALIDATE_STATE_INIT();
	uint8 idx,AuthStartPosition,AuthLen;

	//判断是否有数据部分做新鲜值
	if(tmpSecOCRxPduProcessing->SecOCUseAuthDataFreshness == true){
        AuthStartPosition = tmpSecOCRxPduProcessing->SecOCAuthDataFreshnessStartPosition;
        AuthLen = tmpSecOCRxPduProcessing->SecOCAuthDataFreshnessLen;
    }

	//暂定replace，覆盖方式	保存了s-pdu和a-pdu，以及它们的长度
	switch (tmpSecOCRxPduProcessing->SecOCReceptionOverflowStrategy)
	{
	case REPLACE:
        memcpy((uint8 *)(SecOCintermediateRx[RxPduId].spduBlen + spduRx), PduInfoPtr.SduDataPtr, PduInfoPtr.SduLength);
        SecOCintermediateRx[RxPduId].spduBlen = PduInfoPtr.SduLength;
        SecOCintermediateRx[RxPduId].abc = 0;
        SecOCintermediateRx[RxPduId].avac = 0;
        break;
	case REJECT: // 当有在使用的secure pdu， 拒绝新来的数据
		if(SecOCintermediateRx[RxPduId].spduBlen == 0){
            memcpy((uint8 *)(SecOCintermediateRx[RxPduId].spduBlen + spduRx), PduInfoPtr.SduDataPtr, PduInfoPtr.SduLength);
            SecOCintermediateRx[RxPduId].spduBlen = PduInfoPtr.SduLength;
        }
		break;
	case QUEUE: // 当有在使用的secure pdu， 排队新来的数据，SecOCReceptionQueueSize为上限
		/*
		if(SecOCintermidateRx[RxPduId] 未用满队列){  // 需要设计数据类型（循环队列？）
			排队
		}
		*/
		break;

	default:
		printf("throw err to get wrong SecOCReceptionOverflowStrategy");
		break;
	}

	// memcpy((uint8*)SecOCintermidateRx[RxPduId].sdatachar,PduInfoPtr->SduDataPtr,PduInfoPtr->SduLength);
	// memcpy((uint8*)SecOCintermidateRx[RxPduId].adatachar,PduInfoPtr->SduDataPtr+AuthStartPosition,AuthLen);
	// SecOCintermidateRx[RxPduId].slen=PduInfoPtr->SduLength;
	// SecOCintermidateRx[RxPduId].len=AuthLen;
}

BufReq_ReturnType SecOC_CopyRxData(PduIdType id, PduInfoType info, PduLengthType bufferSizePtr) {
	/*copyData();
	if(success){
		return BUFREQ_OK;
	}else{
		return BUFREQ_E_NOT_OK;
	}//  BUFREQ_E_BUSY/ BUFREQ_E_OVFL*/
	VALIDATE_STATE_INIT();
	BufReq_ReturnType r = BUFREQ_OK;
	uint8 AuthStartPosition,AuthLen;
    AuthStartPosition = tmpSecOCRxPduProcessing->SecOCAuthDataFreshnessStartPosition;
    AuthLen = tmpSecOCRxPduProcessing->SecOCAuthDataFreshnessLen;
    boolean sizeOk;		//判断目前是否有足够的内存可以存储传输过来的数据
	//boolean dirOk;
	//boolean lockOk;
	//目前覆盖原数据
    sizeOk = 1;
    //sizeOk=currentSize>=info->SduLength;
	//dirOk = SecoCIPduDirection == RECEIVE;
	//lockOk = isPduBufferLocked(PduId);
	if(sizeOk){
		memcpy((uint8*)(SecOCintermediateRx[id].addr_st + spduRx), info.SduDataPtr, info.SduLength);
		SecOCintermediateRx[id].spduBlen = info.SduLength;
		memcpy((uint8*)(SecOCintermediateRx[id].addr_st + apduRx),info.SduDataPtr + AuthStartPosition, AuthLen);
        SecOCintermediateRx[id].apduBlen = AuthLen;
        //*bufferSizePtr=Size-currentSize;
	}
	else{
		r = BUFREQ_E_NOT_OK;
	}
	return r;
}

void SecOC_TpRxIndication(PduIdType id, Std_ReturnType result) {
	VALIDATE_STATE_INIT();
	empty();
	if (true) {
		result = E_OK;
	} else {
		result = E_NOT_OK;
	}
}

//0x4c 同步 不可重入
//E_OK:目标模块成功执行取消。
//E_NOT_OK: 目标模块拒绝取消。
//请求取消较低层传输协议模块中正在进行的PDU接收。
Std_ReturnType SecOC_TpCancelReceive (PduIdType RxPduId) {
    PduR_TpCancelReceive(RxPduId);
}

#define bit(i) (1 << i)
#define addr2uint8p(address, index) (address + (index + 8 - 1) / 8)
#define testbit(data, index) (bit(index) & data)
#define setbit(address, index) *addr2uint8p(address, index) |= bit(index % 8)
#define clearbit(address, index) *addr2uint8p(address, index) &= !bit(index % 8)

Std_ReturnType authenticate(SecOCintermediate_type SecOCintermediate) {
    Std_ReturnType result;
    if (SecOCPduType == CFUNC && SecOCProvideTxTruncatedFreshnessValue) {
        result = FVM_GetTxFreshnessTruncData(
            SecOCFreshnessValueID,
            SecOCFreshnessValue,
            SecOCFreshnessValueLength,
            SecOCTruncatedFreshnessValue,
            SecOCTruncatedFreshnessValueLength);
        if (result == E_NOT_OK) {
            SecOCintermediate.spduBlen = 0;
            return;
        } else if (result == BUFREQ_E_BUSY) {
            SecOCintermediate.abc += 1;
            if (SecOCintermediate.abc<SecOCAuthenticationBuildAttempts) {
                return;
            } else if (SecOCintermediate.abc == SecOCAuthenticationBuildAttempts) {
                //if ()
                SecOCintermediate.spduBlen = 0;
                return;
            }
        }
    } else if (SecOCPduType == CFUNC && !SecOCProvideTxTruncatedFreshnessValue) {
        result = FVM_GetTxFreshness(
            SecOCFreshnessValueID,
            SecOCFreshnessValue,
            SecOCFreshnessValueLength);
        if (result == E_NOT_OK) {
            SecOCintermediate.abc += 1;
        }
    }
    else if (SecOCPduType == RTE && SecOCProvideTxTruncatedFreshnessValue) {
        FreshnessManagement_GetTxFreshnessTruncData();
    }
    else {
        FreshnessManagement_GetTxFreshness();
    }

    // 构造
    // secocdataID (16bits) | secured part (apdu) | fv
    uint8* DataToAuthenticator;
    uint8 tmplen = SecOCTxPduSecuredArea.SecOCSecuredTxPduLength;
    uint8 DataToAuthenticatorlen = 16 + tmplen + 64;
    uint8 *addr_apdu = spduTx + SecOCintermediateTx->addr_st;
    uint8 *offser_apdu;

    DataToAuthenticator = (uint8 *)malloc(sizeof(uint8) * ((DataToAuthenticatorlen + 7) / 8));

    // secocdataID
    for (int i = 0; i < 16; i++) {
        if (testbit(SecOCDataId, i)) {
            setbit(DataToAuthenticator, i);
        } else {
            clearbit(DataToAuthenticator, i);
        }
    }
    // apdu
    uint8 temp_off = SecOCTxPduSecuredArea.SecOCSecuredTxPduOffset;
    for (int i = temp_off; i < temp_off + tmplen; i++) {
        uint8 apdu = *(addr_apdu + (i - 16 + 7) / 8);
        if (bit((i-temp_off)%8) & apdu) {
            setbit(DataToAuthenticator, i);
        } else {
            clearbit(DataToAuthenticator, i);
        }
    }
    // fv
    for (int i = 16 + tmplen; i < 16 + tmplen + 64; i++) {
        uint8 fv_data = *(fv + (i - 16 - SecOCintermediate.apduBlen + 7) / 8);
        if (bit((i-16-tmplen)%8) & fv_data) {
            setbit(DataToAuthenticator, i);
        } else {
            clearbit(DataToAuthenticator, i);
        }
    }

    // SecOCTxAuthServiceConfigRef
    uint8 *macPtr;
    uint8 *macLength;

    Std_ReturnType macrst = Csm_MacGenerate(jobId, mode, DataToAuthenticator, DataToAuthenticatorlen, macPtr, macLength);
    if (macrst == E_NOT_OK) {
        SecOCintermediate.abc++;
        if (SecOCintermediate.abc < SecOCAuthenticationBuildAttempts) {
            return;
        } else if (SecOCintermediate.abc == SecOCAuthenticationBuildAttempts) {
            // if ()
            SecOCintermediate.spduBlen = 0;
            return;
        }
    }
    else if (macrst == BUFREQ_E_BUSY) {
        SecOCintermediate.abc++;
        return;
    }

    if (SecOCTxSecuredPdu) {
        if (SecOCintermediate.apduBlen*8 + SecOCTruncatedFreshnessValueLength + macLength > SecOCintermediate.addr_cpy) {
            return;
        }
        uint8 *spdu;
        uint8 *addr_apdu = SecOCintermediateTx + SecOCintermediate.addr_st;
        uint8 alen = SecOCintermediate.apduBlen * 8;

        for (int i = 0; i < alen; i++) {
            uint8 tmp = *(addr_apdu + (i + 7) / 8);
            if (testbit(tmp, i)) {
                setbit(spdu, i);
            } else {
                clearbit(spdu, i);
            }
        }

        for (int i = alen; i < alen + SecOCTruncatedFreshnessValueLength; i++) {
            if (testbit(SecOCTruncatedFreshnessValue, (i-alen)%8)) {
                setbit(spdu, i);
            } else {
                clearbit(spdu, i);
            }
        }

        for (int i = alen + SecOCFreshnessValueLength; i < alen + SecOCFreshnessValueLength + macLength; i++) {
            uint8 tmp = *(mac + (i - alen - SecOCFreshnessValueLength + 7) / 8);
            if (testbit(tmp, (i-alen-SecOCFreshnessValueLength)%8)) {
                setbit(spdu, i);
            } else {
                clearbit(spdu, i);
            }
        }
    }

    const SecOCTxSecuredPduCollection_type SecOCTxSecuredPduCollection;

    SecOCQueryFreshnessValue_type SecOCQueryFreshnessValue;

    FreshnessManagement_SPduTxConfirmation();

    boolean SecOCUseTxConfirmation = true; // false
    if (SecOCUseTxConfirmation) {
        SecOC_SPduTxConfirmation(SecOCFreshnessValueID);
    }

    // 通知fv更新新鲜值
    FVM_updatePreValue(SecOCFreshnessValueID, SecOCFreshnessValue);

    // 认证成功 调用
    // PduR_SecOCTransmit()
}

void verify(SecOCintermediateRx_type SecOCintermediateRx, VerificationResultType verificationResult, SecOCRxPduProcessing_type curSecOCRxPduProcessing) {
    // ipdu | fv | mac
    uint8 *addr_pdu = spduRx + SecOCintermediateRx.addr_st;
    // ipdu
    uint8 *ipdu;
    uint8 tmplen = SecOCintermediateRx.spduBlen * 8;
    for (int i = 0; i < tmplen; i++)
    {
        if (bit(i) & addr2uint8(addr_pdu, i))
        {
            setbit(ipdu, i);
        }
        else
        {
            clearbit(ipdu, i);
        }
    }
    // fv
    for (int i = tmplen; i < tmplen + SecOCFreshnessValueLength; i++)
    {
        if (bit(i) & addr2uint8(addr_pdu, i))
        {
            setbit(fv, i);
        }
        else
        {
            clearbit(fv, i);
        }
    }
    // mac
    uint8 maclength;
    uint8 *mac;
    for (int i = tmplen + SecOCFreshnessValueLength; i < tmplen + SecOCFreshnessValueLength + maclength; i++)
    {
        if (bit(i) & addr2uint8(addr_pdu, i))
        {
            setbit(mac, i);
        }
        else
        {
            clearbit(mac, i);
        }
    }

    // 获取fv
    Std_ReturnType result;
    result = FVM_GetTxFreshness(
        SecOCFreshnessValueID,
        SecOCFreshnessValue,
        SecOCFreshnessValueLength);
    if (result == BUFREQ_E_BUSY)
    {
        SecOCintermediateRx.abc += 1;
        if (SecOCintermediateRx.abc < SecOCAuthenticationBuildAttempts)
            ;
    }
    else if (result == E_NOT_OK)
    {
        SecOCintermediateRx.spduBlen = 0;
    }
    else if (result == E_OK)
    {
        // 构造datatoauth
        // 构造
        // secocdataID (16bits) | secured part (apdu) | fv
        uint8 *DataToAuthenticator;
        uint8 DataToAuthenticatorlen = 16 + tmplen + 64;

        DataToAuthenticator = (uint8 *)malloc(sizeof(uint8) * ((DataToAuthenticatorlen + 7) / 8));

        uint8 *addr_apdu = spduTx + SecOCintermediateTx->addr_st;

        // secocdataID
        for (int i = 0; i < 16; i++)
        {
            uint8 tmp = *(DataToAuthenticator + ((i + 7) / 8));
            if ((1 << i) & SecOCDataId)
            {
                tmp |= 1 << (i % 8);
            }
            else
            {
                tmp &= !(1 << (i % 8));
            }
        }
        // apdu
        for (int i = 16; i < 16 + tmplen; i++)
        {
            uint8 tmp = *(DataToAuthenticator + ((i + 7) / 8));
            uint8 apdu = *(addr_apdu + (i - 16 + 7) / 8);
            if ((1 << i) & apdu)
            {
                tmp |= 1 << (i % 8);
            }
            else
            {
                tmp &= !(1 << (i % 8));
            }
        }
        // fv
        for (int i = 16 + tmplen; i < 16 + tmplen + 64; i++)
        {
            uint8 tmp = *(DataToAuthenticator + ((i + 7) / 8));
            uint8 fv_data = *(fv + (i - 16 - tmplen + 7) / 8);
            if ((1 << i) & fv_data)
            {
                tmp |= 1 << (i % 8);
            }
            else
            {
                tmp &= !(1 << (i % 8));
            }
        }

        Crypto_VerifyResultType *verifyPtr;
        result = Csm_MacVerify(jobId, mode, ipdu, tmplen, mac, maclength, verifyPtr);
        if (result == BUFREQ_E_BUSY)
        {
            SecOCintermediateRx.abc++;
        }
        else if (result == E_NOT_OK)
        {
            SecOCintermediateRx.spduBlen = 0;
        }
        else
        {
            if (*verifyPtr == 1)
            { // 验证成功
            }
            else
            { // 验证失败
                SecOCintermediateRx.abc = 0;
                SecOCintermediateRx.avac += 1;
                if (SecOCintermediateRx.avac < SecOCAuthenticationBuildAttempts)
                {
                    SecOCintermediateRx.spduBlen = 0;
                    SecOCintermediateRx.apduBlen = 0;
                }
            }
        }
    }
}

BufReq_ReturnType SecOC_StartOfReception(PduIdType id, PduInfoType info,
                                         PduLengthType TpSduLength, PduLengthType bufferSizePtr)
{
    // 判断secoc是否已初始化
    // VALIDATE_STATE_INIT();
    prepare();
    if (SECOC_INIT != _secOCState)
    {
        return BUFREQ_E_NOT_OK;
    }

    // 判断id合法性 是否超过发送报文的配置数量
    if (id >= SECOC_NUM_OF_TX_IPDU)
    {
        return BUFREQ_E_NOT_OK; //非secoc内txpduid
    }

    SecOCintermediate_type intermediate = SecOCintermediateTx[id];
    type_t r;

    if (TpSduLength > bufferSizePtr)
    {
        return BUFREQ_E_OVFL;
    }

    intermediate.slock = 1;

    //SecOC_RxIndication(id,info);
    uint8 AuthHeadlen, Alen, Freshnesslen, Maclen, Slen;
    AuthHeadlen = tmpSecOCRxPduProcessing->SecOCRxSecuredPduLayer.SecOCRxSecuredPdu.SecOCAuthPduHeaderLength;
    Alen = tmpSecOCRxPduProcessing->SecOCAuthDataFreshnessLen;
    Freshnesslen = tmpSecOCRxPduProcessing->SecOCFreshnessValueTruncLength;
    Maclen = tmpSecOCTxPduProcessing->SecOCAuthInfoTruncLength;
    Slen = AuthHeadlen + Alen + Freshnesslen + Maclen;
    BufReq_ReturnType r = BUFREQ_OK;

    if (intermediate.type == upperTp)
    { //upper layer tp
        r = PduR_SecOCTpStartOfReception();
    }
    else
    {
        if (TpSduLength == 0)
        {
            r = BUFREQ_E_NOT_OK;
        }
        else if (tmpSecOCRxPduProcessing->SecOCReceptionOverflowStrategy == REJECT)
        {
            r = BUFREQ_E_NOT_OK;
        }
        else if (AuthHeadlen > 0 && info.SduLength < Slen)
        { //待修正，文档描述不清楚
            r = BUFREQ_E_NOT_OK;
        } //  BUFREQ_E_BUSY/ BUFREQ_E_OVFL
    }

    return r;

    /*if(success){
			return BUFREQ_OK;
		}else{
			return BUFREQ_E_NOT_OK;
		}//  BUFREQ_E_BUSY/ BUFREQ_E_OVFL*/
    
    // if (SecOCRxSecuredPduLayer.SecOCRxSecuredPduCollection != NULL) {
    //     return;
    // }
    Std_ReturnType result;
    result = FVM_GetTxFreshnessTruncData(
        SecOCFreshnessValueID,
        SecOCFreshnessValue,
        SecOCFreshnessValueLength,
        SecOCTruncatedFreshnessValue,
        SecOCTruncatedFreshnessValueLength);
    if (result == E_NOT_OK) {
        SecOCintermediateRx[0].spduBlen = 0;
        return;
    }
    else if (result == BUFREQ_E_BUSY) {
        SecOCintermediateRx[0].abc += 1;
        if (SecOCintermediateRx[0].abc < SecOCAuthenticationBuildAttempts) {
            return;
        }
        else if (SecOCintermediateRx[0].abc == SecOCAuthenticationBuildAttempts) {
            //if ()
            SecOCintermediateRx[0].spduBlen = 0;
            return;
        }
    }

    // 构造
    // secocdataID (16bits) | secured part (apdu) | fv
    uint8 *DataToAuthenticator;
    uint8 tmplen = SecOCTxPduSecuredArea.SecOCSecuredTxPduLength;
    uint8 DataToAuthenticatorlen = 16 + tmplen + 64;
    uint8 *addr_apdu = spduTx + SecOCintermediateTx->addr_st;
    uint8 *offser_apdu;
    DataToAuthenticator = (uint8 *)malloc(sizeof(uint8) * ((DataToAuthenticatorlen + 7) / 8));

    // secocdataID
    for (int i = 0; i < 16; i++) {
        if (testbit(SecOCDataId, i)) {
            setbit(DataToAuthenticator, i);
        } else {
            clearbit(DataToAuthenticator, i);
        }
    }
    // apdu
    uint8 temp_off = SecOCTxPduSecuredArea.SecOCSecuredTxPduOffset;
    for (int i = temp_off; i < temp_off + tmplen; i++) {
        uint8 apdu = *(addr_apdu + (i - 16 + 7) / 8);
        if (bit((i-temp_off)%8) & apdu) {
            setbit(DataToAuthenticator, i);
        } else {
            clearbit(DataToAuthenticator, i);
        }
    }
    // fv
    for (int i = 16 + tmplen; i < 16 + tmplen + 64; i++) {
        uint8 fv_data = *(fv + (i - 16 - SecOCintermediateRx[0].apduBlen + 7) / 8);
        if (bit((i-16-tmplen)%8) & fv_data) {
            setbit(DataToAuthenticator, i);
        } else {
            clearbit(DataToAuthenticator, i);
        }
    }

    // SecOCTxAuthServiceConfigRef
    uint8 *macPtr;
    uint8 *macLength;

    Std_ReturnType macrst = Csm_MacGenerate(jobId, mode, DataToAuthenticator, DataToAuthenticatorlen, macPtr, macLength);
    if (macrst == E_NOT_OK) {
        SecOCintermediateRx[0].abc++;
        if (SecOCintermediateRx[0].abc < SecOCAuthenticationBuildAttempts) {
            return;
        } else if (SecOCintermediateRx[0].abc == SecOCAuthenticationBuildAttempts) {
            // if ()
            SecOCintermediateRx[0].spduBlen = 0;
            return;
        }
    }
    else if (macrst == BUFREQ_E_BUSY) {
        SecOCintermediateRx[0].abc++;
        return;
    }
}

Std_ReturnType SecOC_GetTxFreshnessTruncData ( 
	uint16 SecOCFreshnessValueID, 
	uint8* SecOCFreshnessValue, 
	uint32* SecOCFreshnessValueLength, 
	uint8* SecOCTruncatedFreshnessValue,  
	uint32* SecOCTruncatedFreshnessValueLength 
){
    return FVM_GetTxFreshnessTruncData(SecOCFreshnessValueID, SecOCFreshnessValue, SecOCFreshnessValueLength, SecOCTruncatedFreshnessValue, SecOCTruncatedFreshnessValueLength); //slaveFVM.h 中函数
}

Std_ReturnType SecOC_GetTxFreshness ( 
	uint16 SecOCFreshnessValueID, 
	uint8* SecOCFreshnessValue,    //输出
	uint32* SecOCFreshnessValueLength 
){
    return FVM_GetTxFreshness(SecOCFreshnessValueID, SecOCFreshnessValue, SecOCFreshnessValueLength); 
}