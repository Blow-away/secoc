/*
 * ComStack_Types.h
 *
 *  Created on: 2021年9月6日
 *      Author: zhao chenyang
 */

#ifndef COMSTACK_TYPES_H_
#define COMSTACK_TYPES_H_

#include "Platform_Types.h"

typedef uint8 PduIdType;
typedef uint8 PduLengthType;

typedef enum {
    TP_DATACONF = 0x00,
    /*TP_DATACONF indicates that all data, that have been
    copied so far, are confirmed and can be removed from the
    TP buffer. Data copied by this API call are excluded and
    will be confirmed later.*/
    TP_DATARETRY,
    /*TP_DATARETRY indicates that this API call shall
    copyalready copied data in order to recover from an error.
    In this case TxTpDataCnt specifies the offset of the first
    byte to be copied by the API call.*/
    TP_CONFPENDING, //TP_CONFPENDING indicates that the previously copied data must remain in the TP.
} TpDataStateType;

typedef struct {
    TpDataStateType TpDataState; //The enum type to be used to store the state of Tp buffer.
    PduLengthType TxTpDataCnt; //Offset from the current position which identifies the number of bytes to be retransmitted.
} RetryInfoType;

// 数据信息  包括数据指针，meta数据指针，pdu长度
typedef struct {
    uint8 *SduDataPtr;
    uint8 *MetaDataPtr;
    PduLengthType SduLength;
} PduInfoType;

//自定义
typedef enum {
    drirect,
    trigger,
    tp,
    upperTp,
    CanIF,
    canTP,
    FrIF,
    Com,
    CanTp_COM,
    CanTp_TXCopy

} type_t;

typedef enum {
    BUFREQ_OK = 0x00, // Buffer request accomplished successful. This status shall have the value 0.
    BUFREQ_E_NOT_OK,  //Buffer request not successful. Buffer cannot be accessed. This status shall have the value 1.
    BUFREQ_E_BUSY,    //Temporarily no buffer available. It's up the requester to retry request for a certain time. This status shall have the value 2.
    BUFREQ_E_OVFL,    //No Buffer of the required length can be provided. This status shall have the value 3.
} BufReq_ReturnType;

#endif /* COMSTACK_TYPES_H_ */
