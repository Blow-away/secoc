#ifndef __SecOC_Cfg__
#define __SecOC_Cfg__

#include "Std_Types.h"
#include "ComStack_Types.h"
#include "SecOC_Types.h"

#define SECOC_NUM_OF_TX_IPDU 2
#define SECOC_NUM_OF_RX_IPDU 2

#define TX_SINGLE_IPDU 1
#define TX_COLLECTION_IPDU 1

#define RX_SINGLE_IPDU 1
#define RX_COLLECTION_IPDU 1
#define RECEPTION_QUEUE_SIZE 1 //接收队列长度

#define SECOC_ERROR_DETECT STD_ON

// 配置值
uint8 fv[8];
uint8 tcfv[8];
uint8 mac[16];

SecOCQueryFreshnessValue_type SecOCPduType = CFUNC;	  // RTE
boolean SecOCProvideTxTruncatedFreshnessValue = true; // false
boolean SecOCTxSecuredPdu = false; // true

uint16 SecOCFreshnessValueID;
uint8 SecOCFreshnessValue;
uint32 SecOCFreshnessValueLength;

uint8 SecOCTruncatedFreshnessValue;
uint32 SecOCTruncatedFreshnessValueLength;

uint16 SecOCDataId;
SecOCTxPduSecuredArea_type SecOCpdu;

uint8 SecOCAuthenticationBuildAttempts;

uint32 jobId;
Crypto_OperationModeType mode;

SecOCTxPduSecuredArea_type SecOCTxPduSecuredArea;
SecOCRxSecuredPduLayer_type SecOCRxSecuredPduLayer;

//临时存储secoc中间结果
SecOCintermediate_type SecOCintermediateTx[SECOC_NUM_OF_TX_IPDU] = {
	{.abc = 0}};

SecOCintermediateRx_type SecOCintermediateRx[SECOC_NUM_OF_RX_IPDU]={
	{
		.abc=0,
		.avac=0
	}
};

#endif