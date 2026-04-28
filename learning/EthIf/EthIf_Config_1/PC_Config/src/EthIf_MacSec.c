/*******************************************************************************
**                        KPIT Technologies Limited                           **
**                                                                            **
** KPIT Technologies Limited owns all the rights to this work. This work      **
** shall not be copied, reproduced, used, modified or its information         **
** disclosed without the prior written authorization of KPIT Technologies     **
** Limited.                                                                   **
**                                                                            **
** SRC-MODULE: EthIf_Internal.c                                               **
**                                                                            **
** TARGET    : All                                                            **
**                                                                            **
** PRODUCT   : AUTOSAR EthIf                                                  **
**                                                                            **
** PURPOSE   : AUTOSAR EthIf API's                                            **
**                                                                            **
** PLATFORM DEPENDANT [yes/no]: no                                            **
**                                                                            **
** TO BE CHANGED BY USER [yes/no]: no                                         **
**                                                                            **
** Design ID : ETHIF_SDD_0032                                                 **
*******************************************************************************/

/*******************************************************************************
**                      Revision History                                      **
********************************************************************************
** Date          Changed By      Description                                  **
********************************************************************************
** 27-Feb-2026   Atreya          As per feature #CP-21187 and #CP-1450,       **
**               Chiplunkar,     Initial Version                              **
*******************************************************************************/
#include "EthIf.h"
#if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
#include "EthIf_IntTypes.h"
#include "EthIf_External.h"
#include "Rte_Csm_Type.h"
#include "Csm.h"

/*******************************************************************************
**                                Version Check                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
Mka_Stats_Tx_ScType EthIf_MacSecTxScStat[ETHIF_SW_MACSEC_CTRL_COUNT];
Mka_Stats_Tx_SecYType EthIf_MacSecTxSecYStat[ETHIF_SW_MACSEC_CTRL_COUNT];

/*******************************************************************************
**                              In-line Functions                             **
*******************************************************************************/

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

/*******************************************************************************
**                      Function Definitions                                  **
*******************************************************************************/

/*******************************************************************************
** Function Name        : EthIf_MacSecUpdateTxStats                           **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : This API will Calculate MPDU Statistics             **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : LucCtrlIdx, LusDataLen, LusPacketLength             **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0797                                      **
*******************************************************************************/
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
void EthIf_MacSecUpdateTxStats(uint8 LucCtrlIdx, uint16 LusDataLen,
                               uint16 LusPacketLength)
{
  Mka_Stats_Tx_ScType * LpTxScStat;
  Mka_Stats_Tx_SecYType * LpTxSecYStat;
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController_PB * LpStaticController_PB;
  #endif
  const EthIf_ControllerMacsecContextType * LpCtrlCtx;
  const EthIf_MacsecSaContextType *LpTxSa;
  uint8 LucSWMacSecIndex;

  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  LpStaticController_PB = &EthIf_GaaStaticController_PB[LucCtrlIdx];
  #endif

  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  if (ETHIF_SW_MACSEC == LpStaticController_PB->usmacsec)
  #else
  if (ETHIF_SW_MACSEC == EthIf_GpConfigPtr->pController[LucCtrlIdx].usmacsec)
  #endif
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LucSWMacSecIndex = LpStaticController_PB->ucSWMacSecIndex;
    #else
    LucSWMacSecIndex = EthIf_GpConfigPtr->pController[LucCtrlIdx].ucSWMacSecIndex;
    #endif
    LpCtrlCtx = &EthIf_GaaMacsecController[LucSWMacSecIndex];
    if (ETHIF_TRUE == LpCtrlCtx->IsMacSecOperational)
    {
      LpTxSa = &LpCtrlCtx->TxSa[LpCtrlCtx->ActiveTxAnIndex];

      LpTxScStat = &EthIf_MacSecTxScStat[LucSWMacSecIndex];
      LpTxSecYStat = &EthIf_MacSecTxSecYStat[LucSWMacSecIndex];

      switch (LpCtrlCtx->ConfidentialityOffset)
      {
        case MKA_CONFIDENTIALITY_NONE:
          LpTxScStat->OutPkts_Protected++;
          LpTxSecYStat->OutOctets_Protected += LusDataLen;
          break;
        case MKA_CONFIDENTIALITY_OFFSET_0:
          LpTxScStat->OutPkts_Encrypted++;
          LpTxSecYStat->OutOctets_Encrypted += LusDataLen;
          break;
        case MKA_CONFIDENTIALITY_OFFSET_30:
          LpTxScStat->OutPkts_Encrypted++;
          LpTxSecYStat->OutOctets_Protected += ETHIF_THIRTY;
          LpTxSecYStat->OutOctets_Encrypted += LusDataLen - ETHIF_THIRTY;
          break;
        case MKA_CONFIDENTIALITY_OFFSET_50:
          LpTxScStat->OutPkts_Encrypted++;
          LpTxSecYStat->OutOctets_Protected += ETHIF_FIFTY;
          LpTxSecYStat->OutOctets_Encrypted += LusDataLen - ETHIF_FIFTY;
          break;
        default:
          break; 
      }

      if (!((ETHIF_TRUE == LpCtrlCtx->IsProtectFramesEnabled) &&
            (ETHIF_TRUE == LpTxSa->Active)))
      {
        LpTxSecYStat->OutPkts_Untagged++;
      }
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      if (LusPacketLength > LpStaticController_PB->ucCtrlMTU)
      #else
      if (LusPacketLength > EthIf_GpConfigPtr->pController[LucCtrlIdx].ucCtrlMTU)
      #endif
      {
        LpTxSecYStat->OutPkts_TooLong++;
      }
    }
  }
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_GenerateSecTAG                                **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : This API will generate SecTAG                       **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : LucCtrlIdx, LusDataLen                              **
**                                                                            **
** InOut parameter      : LpDataPtr                                           **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : boolean                                             **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0796                                      **
*******************************************************************************/
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
void EthIf_GenerateSecTAG(
  const EthIf_ControllerMacsecContextType * LpCtrlCtx,
  EthIf_MacsecSaContextType *LpTxSa, uint8 ** LpDataPtr, uint16 LusDataLen)
{
  EthIf_SecTAGType * LpSecTAG;
  boolean LblConfEn = ETHIF_FALSE;

  LpSecTAG = (EthIf_SecTAGType *) (*LpDataPtr);

  LblConfEn = (LpCtrlCtx->ConfidentialityOffset != MKA_CONFIDENTIALITY_NONE);

  /* Populate TCI Field */
  LpSecTAG->ucTCIAN = ETHIF_ZERO;
  /* Version Value */
  LpSecTAG->ucTCIAN |= ETHIF_BITUPDATE(ETHIF_ZERO, ETHIF_SEVEN);
  /* End Station Identifier : Set to True for Point to Point Communication */
  LpSecTAG->ucTCIAN |= ETHIF_BITUPDATE(ETHIF_TRUE, ETHIF_SIX);

  /* SC set to False as SCI is disabled for Point to Point Communication */
  LpSecTAG->ucTCIAN |= ETHIF_BITUPDATE(ETHIF_FALSE, ETHIF_FIVE);
  /* SC Broadcast flag set to False for Point to Point Communication */
  LpSecTAG->ucTCIAN |= ETHIF_BITUPDATE(ETHIF_FALSE, ETHIF_FOUR);
  /* Data Confidentiality */
  LpSecTAG->ucTCIAN |= ETHIF_BITUPDATE(LblConfEn, ETHIF_THREE);
  /* Data Integrity */
  LpSecTAG->ucTCIAN |= ETHIF_BITUPDATE(LblConfEn, ETHIF_TWO);
  /* Populate Association number */
  LpSecTAG->ucTCIAN |= LpTxSa->An & ETHIF_THREE;

  /* Populate short Length */
  if (LusDataLen < 48)
  {
    LpSecTAG->ucShortLen = (uint8)LusDataLen;
  }
  else
  {
    LpSecTAG->ucShortLen = ETHIF_ZERO;
  }

  /* Populate Packet Number */
  ETHIF_32BWORDTOBYTEARR(LpSecTAG->ucPacketNum, (uint32 *)(&LpTxSa->NextPn));
  LpTxSa->NextPn++;

  /* Not Populating Secure Channel Identifier for Point to Point */

  /* Update the Buffer Pointer */
  *LpDataPtr += ETHIF_SIX;
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_ValidateSecTAG                                **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : This API will Validate SecTAG of the payload        **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : LucCtrlIdx, LpDataPtr, LuldataLen                   **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : boolean                                             **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0799                                      **
*******************************************************************************/
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
boolean EthIf_ValidateSecTAG(uint8 LucCtrlIdx,
                          const uint8 * LpDataPtr, uint32 LuldataLen)
{
  ETHIF_UNUSED(LucCtrlIdx);
  ETHIF_UNUSED(LpDataPtr);
  ETHIF_UNUSED(LuldataLen);
  return ETHIF_TRUE;
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_GenerateICV                                   **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : This API will generate and populate ICV             **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : LucCtrlIdx, LuldataLen                              **
**                                                                            **
** InOut parameter      : LpDataPtr                                           **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0795                                      **
*******************************************************************************/
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_GenerateICV(uint8 LucCtrlIdx,
                                 uint8 * LpDataPtr, uint32 LuldataLen)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController_PB* LpStaticController_PB;
  #endif
  const EthIf_ICVHandlerType * LpICVHandler;
  uint8 EthIf_ICV[ETHIF_MACSEC_ICV_LENGTH];

  Std_ReturnType LddReturnValue = E_NOT_OK;
  uint32 LulICVLength = (uint32)ETHIF_MACSEC_ICV_LENGTH;
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  LpStaticController_PB = &EthIf_GaaStaticController_PB[LucCtrlIdx];
  #endif

  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  if (ETHIF_SW_MACSEC == LpStaticController_PB->usmacsec)
  #else
  if (ETHIF_SW_MACSEC == EthIf_GpConfigPtr->pController[LucCtrlIdx].usmacsec)
  #endif
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpICVHandler = &EthIf_ICVHandlers[LpStaticController_PB->ucSWMacSecIndex];
    #else
    LpICVHandler = &EthIf_ICVHandlers[
      EthIf_GpConfigPtr->pController[LucCtrlIdx].ucSWMacSecIndex];
    #endif
    /* Callout to generate ICV using Csm */
    LddReturnValue = EthIf_ICV_Generation_Callout(
      LpICVHandler->ucICVGenRefJobId, LpICVHandler->ucICKKeyRefKeyId,
      LpDataPtr, LuldataLen, EthIf_ICV, &LulICVLength);

    if (E_OK == LddReturnValue)
    {
      /* Copy ICV into Data pointer */
      ETHIF_MEMCPY((LpDataPtr + LuldataLen), EthIf_ICV,
                   ETHIF_MACSEC_ICV_LENGTH);
    }
  }
  return(LddReturnValue);
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_ValidateICV                                   **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : This API will Validate ICV of the payload           **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : LucCtrlIdx, LpDataPtr, LuldataLen                   **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : boolean                                             **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0798                                      **
*******************************************************************************/
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
boolean EthIf_ValidateICV(uint8 LucCtrlIdx,
                          const uint8 * LpDataPtr, uint32 LuldataLen)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController_PB* LpStaticController_PB;
  #endif
  const EthIf_ICVHandlerType * LpICVHandler;
  Crypto_VerifyResultType LddVerify;
  uint32 LulICVOffset;

  boolean LblReturnValue = ETHIF_FALSE;
  Std_ReturnType LddReturnValue = E_NOT_OK;
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  LpStaticController_PB = &EthIf_GaaStaticController_PB[LucCtrlIdx];
  #endif

  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  LpICVHandler = &EthIf_ICVHandlers[LpStaticController_PB->ucSWMacSecIndex];
  #else
  LpICVHandler = &EthIf_ICVHandlers[
    EthIf_GpConfigPtr->pController[LucCtrlIdx].ucSWMacSecIndex];
  #endif
  LulICVOffset = LuldataLen - ETHIF_MACSEC_ICV_LENGTH;
  /* Call Csm_MacVerify to validate the MPDU */
  LddReturnValue = Csm_MacVerify(LpICVHandler->ucICVValidateRefJobId,
    CRYPTO_OPERATIONMODE_SINGLECALL, LpDataPtr, LulICVOffset,
    (LpDataPtr + LulICVOffset), ETHIF_MACSEC_ICV_LENGTH, &LddVerify);

  if ((E_OK == LddReturnValue) && (CRYPTO_E_VER_OK == LddVerify))
  {
    LblReturnValue = ETHIF_TRUE;
  }
  return LblReturnValue;
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
