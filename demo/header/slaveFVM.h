#ifndef __SlaveFVM__
#define __SlaveFVM__

#include "Platform_Types.h"

void FVM_updatePreValue(
    uint16 SecOCFreshnessValueID, 
    uint8 SecOCFreshnessValue);
Std_ReturnType FVM_GetTxFreshness(
    uint16 SecOCFreshnessValueID, 
    uint8 SecOCFreshnessValue, 
    uint32 SecOCFreshnessValueLength);
Std_ReturnType FVM_GetTxFreshnessTruncData( 
    uint16 SecOCFreshnessValueID,
    uint8 SecOCFreshnessValue,
    uint32 SecOCFreshnessValueLength,
    uint8 SecOCTruncatedFreshnessValue,
    uint32 SecOCTruncatedFreshnessValueLength);
void FreshnessManagement_GetTxFreshnessTruncData();
void FreshnessManagement_GetTxFreshness();
void FreshnessManagement_SPduTxConfirmation();
void SecOC_SPduTxConfirmation(uint16 SecOCFreshnessValueID);

#endif