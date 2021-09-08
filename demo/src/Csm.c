#include "Csm.h"

Std_ReturnType Csm_MacGenerate(
    uint32 jobId,                       //任务id号   暂不考虑
    Crypto_OperationModeType mode,      //处理模式 暂不考虑
    const uint8* dataPtr,               //原始数据DataToAuthentic
    uint32 dataLength,                  //原始数据DataToAuthentic长度
    uint8* macPtr,                      //裁剪后mac存放的地址，从外部输入
    uint32* macLengthPtr                //裁剪的mac长度
){
    //调用 void Mac_Generate(uint8* datavalue, uint8 datalen, uint8* hash);
    Mac_Generate(dataPtr, dataLength, macPtr);
    *macLengthPtr = 16;
    return E_OK;
}

Std_ReturnType Csm_MacVerify(
    uint32 jobId,
    Crypto_OperationModeType mode,
    const uint8* dataPtr,
    uint32 dataLength,
    const uint8* macPtr,
    const uint32 macLength,
    Crypto_VerifyResultType* verifyPtr 
){

}
