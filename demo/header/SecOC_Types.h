/*
 * SecOC_Types.h
 *
 *  Created on: 2021��9��7��
 *      Author: zhao chenyang
 */

#ifndef SECOC_TYPES_H_
#define SECOC_TYPES_H_

#include <stdbool.h>
#include "Platform_Types.h"

typedef enum { //SecOC״̬
    SECOC_UNINIT, //SecOCδ��ʼ��
    SECOC_INIT    //SecOC��ʼ��
} SecOC_StateType;

typedef struct {
    uint8 addr_st; // a/spdu��ʼ��ַ
    uint8 addr_cpy; // a/spdu��ռ����(�ֽ�)
    uint8 apduBlen; // apdu����
    uint8 spduBlen; // spdu����
    type_t type; // ��������(direct trigger tp upperTp)
    uint8 abc; // ��֤�ü�����
    uint8 slock; // tpģʽ�� �ײ�ͨ��ģ����Ҫ����spdu���� ��������д
} SecOCintermediate_type;

typedef struct {
    uint8 addr_st;  // �洢�ռ�a/spdu��ʼ����
    uint8 addr_cpy;  // �洢�ռ�a/spdu��ռ����  8*(��/��)
    uint8 apduBlen;  // apdu���ó���
    uint8 spduBlen;  // spdu���ó���
    type_t type;  // ��������enum��drirect, trigger, tp, upperTp��
    uint8 abc; // ��֤�ü�����
    uint8 avac; // ��֤�ü�����
    uint8 slock;  //tpģʽ�� �ײ�ͨ��ģ����Ҫ����spdu���� ��������д
    uint8 rxall; //tp�����Ƿ�����������ʼFALSE
}SecOCintermediateRx_type;

typedef enum { //�½��ձ��ĵĴ�����ʽ
    QUEUE,   //�Ŷ�
    REJECT,  // ֱ�Ӷ���
    REPLACE  // �滻��ǰֵ
} SecOCReceptionOverflowStrategy_type;

typedef enum {      // ��֤״̬֪ͨģʽ
    BOTH,           // ��֤�ɹ�ʧ�ܶ�֪ͨ
    FAILURE_ONLY,   // ��֤ʧ��֪ͨ
    NONE            // ��֤�ɹ�ʧ�ܶ���֪ͨ
} SecOCVerificationStatusPropagationMode_type;

typedef struct {
    uint32 CsmJobId;
} CsmJob_type; //�˽ṹ�������csmģ����

typedef struct {
    uint32 SecOCBufferLength;        //0-4294967295 1
} SecOCSameBufferPduCollection_type; //0..*

typedef enum {
    CFUNC,      //ʹ�ú���ʽ�ӿ�
    RTE         //ʹ������ʱ�����ӿ�
} SecOCQueryFreshnessValue_type;

typedef struct {
    const uint8 SecOCIPduSize;
    const uint8 SecOCIPduSt;
    uint8 SecOCIPducurpos;
    uint8 lock;
    void *const SecOCIpduDataPtr;
    uint8 SecOCIPdulen;
} SecOCPdu_type;

typedef struct {
    SecOCQueryFreshnessValue_type SecOCPduType; //[SECOC_IFPDU/SECOC_TPPDU] ö������  ��������
    //1
    const SecOCPdu_type *const SecOCRxAuthenticLayerPduRef;     // �� ���ա� ���۲�����
    //1
} SecOCRxAuthenticPduLayer_type;

typedef struct {
    uint32 SecOCSecuredRxPduLength; //[0..4294967295]  �������mac��apdu����
    //1 This parameter defines the length (in bytes) of the area within the Pdu which is secured
    uint32 SecOCSecuredRxPduOffset; //[0..4294967295]  �������mac��apdu��ʼλ��
    //1 This parameter defines the start position (offset in bytes) of the area within the Pdu which is secured
} SecOCRxPduSecuredArea_type; //�����Ķβ������mac  ����������ȫ��authentic
//0..1

typedef struct {
    uint8 SecOCAuthPduHeaderLength; //[0..4]                ��̬����pdu����������apdu���ȣ� �ݲ�����
    //0..1
    uint16 SecOCRxSecuredLayerPduId; //[0..65535]           id, ͬ�ṹ��������
    //1
    boolean SecOCSecuredRxPduVerification;  //              ���������Ƿ���Ҫ��֤����=false����ֱ����ȡapdu��
    //1
    const SecOCPdu_type *const SecOCRxSecuredLayerPduRef; //��ա����ݲ�����
    //1
} SecOCRxSecuredPdu_type;

typedef struct {
    uint8 SecOCAuthPduHeaderLength; //[0..4]                ��̬����pdu����������apdu���ȣ� �ݲ�����
    //0..1
    uint16 SecOCRxAuthenticPduId; //[0..65535]              id, ͬ�ṹ��������
    //1
    const SecOCPdu_type *const SecOCRxAuthenticPduRef;    //��ա����ݲ�����
    //1
} SecOCRxAuthenticPdu_type;  //��֡ģʽ �ݲ�����

typedef struct {
    uint16 SecOCRxCryptographicPduId; //[0..65535]          id, ͬ�ṹ��������
    //1
    const SecOCPdu_type *const SecOCRxCryptographicPduRef;//��ա����ݲ�����
    //1
} SecOCRxCryptographicPdu_type;//��֡ģʽ �ݲ�����

typedef struct {
    uint16 SecOCMessageLinkLen;  //��֡ʱ����ʶ��acpdu��ϵ���ֳ���
    /* Length of the Message Linker inside the Authentic I-PDU in bits.
	1
	*/

    uint16 SecOCMessageLinkPos;  //��֡ʱ����ʶ��acpdu��ϵ������ʼλ��
    /* The position of the Message Linker inside the Authentic I-PDU in bits.
	1
	*/

} SecOCUseMessageLink_type;

typedef struct {
    boolean SecOCSecuredRxPduVerification;      //
    //1
    const SecOCRxAuthenticPdu_type SecOCRxAuthenticPdu; //
    //1
    const SecOCRxCryptographicPdu_type SecOCRxCryptographicPdu; //
    //1
    const SecOCUseMessageLink_type SecOCUseMessageLink; //
    //0..1
} SecOCRxSecuredPduCollection_type;  //��֡ģʽ �ݲ�����


typedef struct {  // ���³�Աֻ������һ��
    const SecOCRxSecuredPdu_type SecOCRxSecuredPdu; //  �Ƿ�֡ģʽ
    //0..1
    const SecOCRxSecuredPduCollection_type SecOCRxSecuredPduCollection; // ��֡ģʽ  �ݲ�����
    //0..1
} SecOCRxSecuredPduLayer_type;

typedef struct {
    uint16 SecOCAuthDataFreshnessLen; //[0..65535](default value --)            ��ʱ�̶������ã� ���ݲ�����fv�ĳ���
    //0..1

    uint16 SecOCAuthDataFreshnessStartPosition; //[0..65535](default value --)  ��ʱ�̶������ã� ���ݲ�����fv����ʼλ��
    //0..1

    uint16 SecOCAuthenticationBuildAttempts; //[0..65535](default value --)     ����spdu��֤�ɳ��ԵĴ�������
    //0..1

    uint16 SecOCAuthenticationVerifyAttempts; //[0..65535](default value 0)     ����spdu��֤�ɳ��ԵĴ�������
    //0..1

    uint16 SecOCAuthInfoTruncLength; //[1..65535](default value --)             �ü�mac���ȣ�����csm_Macverify����
    //1

    uint16 SecOCDataId; //[0..65535](default value --)                          ����macʱ����һ����
    //1

    uint16 SecOCFreshnessValueId; //[0..65535](default value --)                ����ֵid������fv()����
    //1

    uint8 SecOCFreshnessValueLength; //[0..64](default value --)                ����ֵ�������ȣ�����fv()����
    //1

    uint8 SecOCFreshnessValueTruncLength; //[0..64](default value --)           �ü�����ֵ���ȣ� ����fv()ʱ��Ҫ����
    //1

    SecOCReceptionOverflowStrategy_type SecOCReceptionOverflowStrategy; //[QUEUE/REJECT/REPLACE] �������� ��ʱ�̶�REPLACE  �±��ĸ��Ǿɱ�������
    //1

    uint16 SecOCReceptionQueueSize; //[1..65535](default value --)              ��գ����ն��г��ȣ��ݲ�����
    //0..1

    boolean SecOCUseAuthDataFreshness; //(default value false)                  ��ʱ�̶�false
    //1

    SecOCVerificationStatusPropagationMode_type SecOCVerificationStatusPropagationMode; //[BOTH/FAILURE_ONLY/NONE] ö������ �ݲ�����
    //1

    const CsmJob_type *const SecOCRxAuthServiceConfigRef; //Symbolic name reference to [ CsmJob ]  ����csm_Macverify��ز��� �ݲ�����
    //1

    const SecOCSameBufferPduCollection_type *const SecOCSameBufferPduRef; //Reference to [ SecOCSameBufferPduCollection ]  �ݲ�����
    //0..1

    const SecOCRxAuthenticPduLayer_type SecOCRxAuthenticPduLayer;   //          ��������   SECOC_IFPDU/SECOC_TPPDU
    //1

    const SecOCRxPduSecuredArea_type SecOCRxPduSecuredArea; //                  ���ڼ��ܵ����ݲ��� ���Ⱥ�ƫ��
    //0..1

    const SecOCRxSecuredPduLayer_type SecOCRxSecuredPduLayer; //                ��֡ģʽ����ʱֻ����SecOCRxSecuredPdu_type
    //1

} SecOCRxPduProcessing_type;

#endif /* SECOC_TYPES_H_ */