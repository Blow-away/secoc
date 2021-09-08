#ifndef __SecOC_Types__
#define __SecOC_Types__


#include <stdbool.h>

#include "Platform_Types.h"

typedef uint8 VerificationResultType;

typedef enum { //SecOC状态
    SECOC_UNINIT, //SecOC未初始化
    SECOC_INIT    //SecOC初始化
} SecOC_StateType;

typedef enum { //新接收报文的处理方式
    QUEUE,   //排队
    REJECT,  // 直接丢弃
    REPLACE  // 替换当前值
} SecOCReceptionOverflowStrategy_type;

typedef struct {
    uint8 addr_st; // a/spdu起始地址
    uint8 addr_cpy; // a/spdu所占容量(字节)
    uint8 apduBlen; // apdu长度
    uint8 spduBlen; // spdu长度
    type_t type; // 报文类型(direct trigger tp upperTp)
    uint8 abc; // 认证用计数器
    uint8 slock; // tp模式下 底层通信模块需要锁定spdu区域 不允许读写
} SecOCintermediate_type;

typedef struct {
    uint8 addr_st;  // 存储空间a/spdu起始区域
    uint8 addr_cpy;  // 存储空间a/spdu所占容量  8*(单/多)
    uint8 apduBlen;  // apdu所用长度
    uint8 spduBlen;  // spdu所用长度
    type_t type;  // 报文类型enum【drirect, trigger, tp, upperTp】
    uint8 abc; // 验证用计数器
    uint8 avac; // 验证用计数器
    uint8 slock;  //tp模式下 底层通信模块需要锁定spdu区域 不允许读写
    uint8 rxall； //tp数据是否收完整，初始FALSE
}SecOCintermediateRx_type;


typedef struct {
    uint32 SecOCBufferLength;        //0-4294967295 1
} SecOCSameBufferPduCollection_type; //0..*

typedef struct {
    uint32 CsmJobId;
    //1

    //	CsmJobInterfaceUsePort_type CsmJobInterfaceUsePort;
    //	//1    [CRYPTO_USE_FNC  / CRYPTO_USE_PORT / CRYPTO_USE_PORT_OPTIMIZED]
    //
    //	boolean CsmJobPrimitiveCallbackUpdateNotification;
    //	//0..1
    //
    //	uint32 CsmJobPriority;
    //	//1
    //
    //	CsmProcessingMode_type CsmProcessingMode;
    //	//1   [CRYPTO_PROCESSING_ASYNC/ CRYPTO_PROCESSING_SYNC]
    //
    //	const CsmInOutRedirections_type * const CsmInOutRedirectionRef;
    //	//0..1
    //
    //	const CsmKey_type *const CsmJobKeyRef;
    //	//1
    //
    //	const CsmCallback_type *const CsmJobPrimitiveCallbackRef;
    //	//0..1
    //
    //	const CsmPrimitives_type *const CsmJobPrimitiveRef;
    //	//1
    //
    //	const CsmQueue_type *const CsmJobQueueRef;
    //	//1

} CsmJob_type; //此结构体可能在csm模块中

typedef enum {   // 报文所属类型  if/ tp模式
    SECOC_IFPDU,
    SECOC_TPPDU
} SecOCPduType_type;

typedef struct {
    const uint8 SecOCIPduSize;
    const uint8 SecOCIPduSt;
    uint8 SecOCIPducurpos;
    uint8 lock;
    void *const SecOCIpduDataPtr;
    uint8 SecOCIPdulen;
} SecOCPdu_type;

typedef struct {
    SecOCPduType_type SecOCPduType; //1                             报文类型  SECOC_IFPDU, SECOC_TPPDU
    uint16 SecOCTxAuthenticLayerPduId; //1                          id号，同结构体数组的索引号
    const SecOCPdu_type *const SecOCTxAuthenticLayerPduRef; //1     填 空，赞不考虑
} SecOCTxAuthenticPduLayer_type;

typedef struct {
    uint8 SecOCAuthPduHeaderLength; //[0 .. 4] //0..1               动态pdu的apdu长度，咱不考虑
    uint16 SecOCTxSecuredLayerPduId; //1                            id, 同结构体数组索引
    const SecOCPdu_type *const SecOCTxSecuredLayerPduRef; //1       填 空，赞不考虑
} SecOCTxSecuredPdu_type;

typedef struct {
    uint8 SecOCAuthPduHeaderLength; //[0..4] //0..1
    uint16 SecOCTxAuthenticPduId; //1
    const SecOCPdu_type *const SecOCTxAuthenticPduRef; //1
} SecOCTxAuthenticPdu_type; //                                      分帧模式, 暂不考虑

typedef struct {
    uint16 SecOCTxCryptographicPduId; //1
    const SecOCPdu_type *const SecOCTxCryptographicPduRef; //1
} SecOCTxCryptographicPdu_type;//                                   分帧模式, 暂不考虑

typedef struct {
    uint16 SecOCMessageLinkLen;  //分帧时用来识别acpdu关系部分长度
    /* Length of the Message Linker inside the Authentic I-PDU in bits.
	1
	*/

    uint16 SecOCMessageLinkPos;  //分帧时用来识别acpdu关系部分起始位置
    /* The position of the Message Linker inside the Authentic I-PDU in bits.
	1
	*/

} SecOCUseMessageLink_type;

typedef struct {
    const SecOCTxAuthenticPdu_type SecOCTxAuthenticPdu; //1
    const SecOCTxCryptographicPdu_type SecOCTxCryptographicPdu; //1
    const SecOCUseMessageLink_type SecOCUseMessageLink; //0..1
} SecOCTxSecuredPduCollection_type;

typedef struct {  //以下成员只能配置一个
    const SecOCTxSecuredPdu_type SecOCTxSecuredPdu; //[0..4] //0..1   非分帧模式  
    const SecOCTxSecuredPduCollection_type SecOCTxSecuredPduCollection; //0..1   分帧模式  暂不考虑
} SecOCTxSecuredPduLayer_type;

typedef struct {
    uint32 SecOCSecuredTxPduLength; // 参与mac生成截取的apdu的长度 （byte单位）
    /*This parameter defines the length (in bytes) of the area within the Pdu
which shall be secured
	1
	*/

    uint32 SecOCSecuredTxPduOffset; // 参与mac生成截取的apdu的起始位置 （byte单位）
    /*This parameter defines the start position (offset in bytes) of the area within
the Pdu which shall be secured
	1
	*/
} SecOCTxPduSecuredArea_type;

typedef struct {
    uint16 SecOCAuthenticationBuildAttempts; //0..1     单个apdu认证可尝试的次数上限
    uint16 SecOCAuthInfoTruncLength; //1                生成的mac长度
    uint16 SecOCDataId; //1                             参数mac生成时数据的一部分 
    uint16 SecOCFreshnessValueId; //1                   新鲜值id， 调用fv()时传入

    uint8 SecOCFreshnessValueLength; //1                新鲜值完整长度， 调用fv()时传入
    uint8 SecOCFreshnessValueTruncLength; //0 .. 64 1   新鲜值裁剪长度， 调用fv()时可能传入

    boolean SecOCProvideTxTruncatedFreshnessValue; //1  与SecOCGeneral_type.SecOCQueryFreshnessValue 共同决定调用的新鲜值函数
    boolean SecOCUseTxConfirmation; //0..1              是否要调用确认函数

    const SecOCSameBufferPduCollection_type *const SecOCSameBufferPduRef; //0..1    填空 咱不考虑
    const CsmJob_type *const SecOCTxAuthServiceConfigRef; //? 1                     csm相关服务 暂时空
    const SecOCTxAuthenticPduLayer_type SecOCTxAuthenticPduLayer; //1               pdu类型 SECOC_IFPDU/SECOC_TPPDU
    const SecOCTxSecuredPduLayer_type SecOCTxSecuredPduLayer; //1                   帧模式：  分帧 （只能配置一个）
    const SecOCTxPduSecuredArea_type SecOCTxPduSecuredArea; //0..1                  参数mac生成的apdu安全部分

} SecOCTxPduProcessing_type; //0..*

typedef enum {      // 验证状态通知模式
    BOTH,           // 验证成功失败都通知
    FAILURE_ONLY,   // 验证失败通知
    NONE            // 验证成功失败都不通知
} SecOCVerificationStatusPropagationMode_type;

typedef enum {  
    CFUNC,      //使用函数式接口
    RTE         //使用运行时环境接口
} SecOCQueryFreshnessValue_type;

typedef struct {
    SecOCQueryFreshnessValue_type SecOCPduType; //[SECOC_IFPDU/SECOC_TPPDU] 枚举类型  报文类型
    //1
    const SecOCPdu_type *const SecOCRxAuthenticLayerPduRef;     // 填 “空” ，咱不考虑
    //1
} SecOCRxAuthenticPduLayer_type;

typedef struct {
    uint32 SecOCSecuredRxPduLength; //[0..4294967295]  参与计算mac的apdu长度
    //1 This parameter defines the length (in bytes) of the area within the Pdu which is secured
    uint32 SecOCSecuredRxPduOffset; //[0..4294967295]  参与计算mac的apdu起始位置
    //1 This parameter defines the start position (offset in bytes) of the area within the Pdu which is secured
} SecOCRxPduSecuredArea_type; //决定哪段参与计算mac  若不存在则全部authentic
//0..1

typedef struct {
    uint8 SecOCAuthPduHeaderLength; //[0..4]                动态长度pdu参数，决定apdu长度， 暂不考虑
    //0..1
    uint16 SecOCRxSecuredLayerPduId; //[0..65535]           id, 同结构体索引号
    //1
    boolean SecOCSecuredRxPduVerification;  //              决定报文是否需要验证，若=false，则直接提取apdu，
    //1
    const SecOCPdu_type *const SecOCRxSecuredLayerPduRef; //填“空”，暂不考虑
    //1
} SecOCRxSecuredPdu_type;

typedef struct {
    uint8 SecOCAuthPduHeaderLength; //[0..4]                动态长度pdu参数，决定apdu长度， 暂不考虑
    //0..1
    uint16 SecOCRxAuthenticPduId; //[0..65535]              id, 同结构体索引号
    //1
    const SecOCPdu_type *const SecOCRxAuthenticPduRef;    //填“空”，暂不考虑       
    //1
} SecOCRxAuthenticPdu_type;  //分帧模式 暂不考虑    

typedef struct {
    uint16 SecOCRxCryptographicPduId; //[0..65535]          id, 同结构体索引号
    //1
    const SecOCPdu_type *const SecOCRxCryptographicPduRef;//填“空”，暂不考虑       
    //1
} SecOCRxCryptographicPdu_type;//分帧模式 暂不考虑    

typedef struct {
    boolean SecOCSecuredRxPduVerification;      //
    //1
    const SecOCRxAuthenticPdu_type SecOCRxAuthenticPdu; //
    //1
    const SecOCRxCryptographicPdu_type SecOCRxCryptographicPdu; //
    //1
    const SecOCUseMessageLink_type SecOCUseMessageLink; //
    //0..1
} SecOCRxSecuredPduCollection_type;  //分帧模式 暂不考虑

typedef struct {  // 以下成员只能配置一个
    const SecOCRxSecuredPdu_type SecOCRxSecuredPdu; //  非分帧模式
    //0..1
    const SecOCRxSecuredPduCollection_type SecOCRxSecuredPduCollection; // 分帧模式  暂不考虑
    //0..1
} SecOCRxSecuredPduLayer_type;

typedef struct {
    uint16 SecOCAuthDataFreshnessLen; //[0..65535](default value --)            暂时固定不配置， 数据部分做fv的长度
    //0..1

    uint16 SecOCAuthDataFreshnessStartPosition; //[0..65535](default value --)  暂时固定不配置， 数据部分做fv的起始位置
    //0..1

    uint16 SecOCAuthenticationBuildAttempts; //[0..65535](default value --)     单个spdu认证可尝试的次数上限
    //0..1

    uint16 SecOCAuthenticationVerifyAttempts; //[0..65535](default value 0)     单个spdu验证可尝试的次数上限
    //0..1

    uint16 SecOCAuthInfoTruncLength; //[1..65535](default value --)             裁剪mac长度，调用csm_Macverify传入
    //1

    uint16 SecOCDataId; //[0..65535](default value --)                          生成mac时数据一部分
    //1

    uint16 SecOCFreshnessValueId; //[0..65535](default value --)                新鲜值id，构造fv()传入
    //1

    uint8 SecOCFreshnessValueLength; //[0..64](default value --)                新鲜值完整长度，构造fv()传入
    //1

    uint8 SecOCFreshnessValueTruncLength; //[0..64](default value --)           裁剪新鲜值长度， 构造fv()时需要传入
    //1

    SecOCReceptionOverflowStrategy_type SecOCReceptionOverflowStrategy; //[QUEUE/REJECT/REPLACE] 数据类型 暂时固定REPLACE  新报文覆盖旧报文数据
    //1

    uint16 SecOCReceptionQueueSize; //[1..65535](default value --)              填空，接收队列长度，暂不考虑
    //0..1

    boolean SecOCUseAuthDataFreshness; //(default value false)                  暂时固定false
    //1

    SecOCVerificationStatusPropagationMode_type SecOCVerificationStatusPropagationMode; //[BOTH/FAILURE_ONLY/NONE] 枚举类型 暂不考虑
    //1

    const CsmJob_type *const SecOCRxAuthServiceConfigRef; //Symbolic name reference to [ CsmJob ]  调用csm_Macverify相关参数 暂不考虑
    //1

    const SecOCSameBufferPduCollection_type *const SecOCSameBufferPduRef; //Reference to [ SecOCSameBufferPduCollection ]  暂不考虑
    //0..1

    const SecOCRxAuthenticPduLayer_type SecOCRxAuthenticPduLayer;   //          报文类型   SECOC_IFPDU/SECOC_TPPDU        
    //1

    const SecOCRxPduSecuredArea_type SecOCRxPduSecuredArea; //                  用于加密的数据部分 长度和偏移
    //0..1

    const SecOCRxSecuredPduLayer_type SecOCRxSecuredPduLayer; //                分帧模式，暂时只配置SecOCRxSecuredPdu_type
    //1

} SecOCRxPduProcessing_type;

typedef struct {
    uint32 SecOCSecuredTxPduLength;
    /*This parameter defines the length (in bytes) of the area within the Pdu
which shall be secured
	1
	*/

    uint32 SecOCSecuredTxPduOffset;
    /*This parameter defines the start position (offset in bytes) of the area within
the Pdu which shall be secured
	1
	*/
} SecOCTxPduSecuredArea_type;

#endif