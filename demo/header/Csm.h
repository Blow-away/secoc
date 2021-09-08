#ifndef __Csm__
#define __Csm__

#include "Std_Types.h"
#include "Csm_Types.h"

Std_ReturnType Csm_MacGenerate(uint32 jobId, Crypto_OperationModeType mode, const uint8 *dataPtr, uint32 dataLength, uint8 *macPtr, uint32 *macLengthPtr);


Std_ReturnType Csm_MacVerify(
    uint32 jobId,
    Crypto_OperationModeType mode,
    const uint8* dataPtr,
    uint32 dataLength,
    const uint8* macPtr,
    const uint32 macLength,
    Crypto_VerifyResultType* verifyPtr 
);


#endif
