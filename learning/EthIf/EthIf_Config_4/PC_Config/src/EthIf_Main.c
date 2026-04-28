/*******************************************************************************
**                        KPIT Technologies Limited                           **
**                                                                            **
** KPIT Technologies Limited owns all the rights to this work. This work      **
** shall not be copied, reproduced, used, modified or its information         **
** disclosed without the prior written authorization of KPIT Technologies     **
** Limited.                                                                   **
**                                                                            **
** SRC-MODULE: EthIf_Main.c                                                   **
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
** Design ID : ETHIF_SDD_0015                                                 **
*******************************************************************************/

/*******************************************************************************
**                      Revision History                                      **
********************************************************************************
** Date          Changed By      Description                                  **
********************************************************************************
** 11-Nov-2025   A Naresh        As per feature #CP-12795, Transmission and   **
**                               Reception for MKPDU added MKA_LinkstateChange**
** 10-Oct-2025   A Naresh        As per Bug  #CP-9168 updated UL module's     **
**                               callbacks with EthIfController_Idx           **
**                               instead of  EthDrv_Idx                       **
** 24-Sep-2025   A Naresh        As per Bug  #CP-9145 EthIfPhycontroller      **
**                               accessed via EthIfcontroller for EthDRV_APIs **
** 30-Jul-2025   A Naresh        As per US #CP-1018, Updated static code      **
**                               as per Compiler Abstraction                  **
** 11-Jul-2025   A Naresh        As per US #198921, Initial version work-     **
**                               -product is carry over from R23-11 V1.0.0    **
*******************************************************************************/
/*******************************************************************************
**                   MISRA-C:2012 violations Section                          **
*******************************************************************************/
/**
* #section EthIf_Main_c_QAC_REF_1
* Violates MISRA 2012 Advisory Rule 20.1,
* #include directives should only be preceded by preprocessor directives or
* comments.
*
**/
/* #violates #ref EthIf_Main_c_QAC_REF_1 Violates MISRA 2012 Required Rule 20.1*/
/* PRQA S 5087 EOF */

/* PRQA S 0380 EOF */
/* violates Number of macro definitions exceeds 4095 - program does not */
/* conform strictly to ISO:C99 *//*invalid QAC as total number of macros in */
/* code are less than 4095*/

/* Violates qac-10.1.0-2000 */
/* PRQA S 2000 EOF */

/*******************************************************************************
**                      CERT-C Violations Section                             **
*******************************************************************************/
/*******************************************************************************
**                      CAT Violations Section                                **
********************************************************************************/
/*******************************************************************************
**                      ECG_Checker Violations Section                        **
********************************************************************************/
/**
*ECG_Checker_Justification_EthIf_Main_c_Start : WRN01
*According to the common QAC Justification, the warnings are suppressed at the
*end of the file (EOF). After reviewing all instances, no potential issues were
*identified.
*ECG_Checker_Justification_EthIf_Main_c_Stop : WRN01
**/
/*******************************************************************************
**                       Polyspace Violations Section                         **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
/* Design ID : ETHIF_SDD_0015 */
#include "EthIf.h"
#include "EthIf_IntTypes.h"
#include "SchM_EthIf.h"
#include "Eth.h"
#include "EthIf_Cfg.h"
#if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
#include "Mka.h"
#endif
#if (ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES)
#include "EthSwt.h"
#include "EthSwt_GeneralTypes.h"
#endif
#if (STD_ON == ETHIF_DEV_ERROR_DETECT)
#include "Det.h"
#endif

/*******************************************************************************
**                                Version Check                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"
uint8 EthIf_GucEthIfCtrlIdx_Tx;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"

/*******************************************************************************
**                              In-line Functions                             **
*******************************************************************************/

/*******************************************************************************
**                      Function Definitions                                  **
*******************************************************************************/

/*******************************************************************************
** Function Name        : EthIf_MainFunctionRx                                **
**                                                                            **
** Service ID           : 0x20                                                **
**                                                                            **
** Description          : This API of the EthIf module checks for new received**
**                        frames and issues receive indication in polling     **
**                        mode.                                               **
**                                                                            **
** Sync/Async           : NA                                                  **
**                                                                            **
** Re-entrancy          : NA                                                  **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)    : EthIf_GaaStaticController,  **
**                                                EthIf_GpFirstDriverApis     **
**                                                                            **
** Function(s) invoked  : Eth_Receive                                         **
**                                                                            **
**  Design ID           : ETHIF_SDD_0097                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
void EthIf_MainFunctionRx(void)
{
  #if (STD_OFF == ETHIF_ENABLE_RX_INTERRUPT)
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  Std_ReturnType LddStatusReturn;
  /* PRQA S 1251 1 */
  #if (ETHIF_ZERO < ETHIF_RXINDICATION_ITERATIONS)
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController_PB*  LpStaticController_PB;
  const EthIf_StaticPhyController_PB*  LpPhysicalController_PB;
  #endif
  Eth_RxStatusType LenRxStatusPtr;
  uint8 LucMaxNumberOfIterations;
  uint8 LucCtrlIdx;
  #endif

  LddStatusReturn = (Std_ReturnType)E_OK;

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf module is in INITIALIZED state.
  Module Test Case Reference ETHIF_ETC_028, SWS ID SWS_EthIf_00098" */
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_MAINFUNCRX_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  /* polyspace +5 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf module is in UNINITIALIZED state.
  Module Test Case Reference test_1" */
  /* PRQA S 2100 2 */
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {


    /* SWS_EthIf_00097d */
    /* Loop through all the controllers to receive data */
    /* PRQA S 1251 1 */
    #if (ETHIF_ZERO < ETHIF_RXINDICATION_ITERATIONS)
    /* polyspace +4 RTE:NTL [Justified:Low] "This loop will terminate when
    controller index count reaches number of controllers configured */
    /* PRQA S 2100 3 */
    for (LucCtrlIdx = (uint8)ETHIF_ZERO; LucCtrlIdx
                   < (uint8)ETHIF_NUM_OF_CONTROLLERS; LucCtrlIdx++)
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)

      LpStaticController_PB = &EthIf_GaaStaticController_PB[LucCtrlIdx];

      LpPhysicalController_PB = &EthIf_GaaStaticPhyController_PB[
      (LpStaticController_PB->pPhysCtrl)->ucPhysCtrlIdx];
      #endif
      #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LpDriverApis = LpPhysicalController_PB->pEthDriver;
      #else

      LpDriverApis = (EthIf_GpConfigPtr->pController[LucCtrlIdx].pPhysCtrl)->pEthDriver;
      #endif
      /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
      LpDriverApis will be NULL.
      Module Test Case Reference SWS_EthIf_00097d" */
      if (LpDriverApis != NULL_PTR)
      #endif
      {
        /* SWS_EthIf_00131 */
        LucMaxNumberOfIterations = (uint8)ETHIF_RXINDICATION_ITERATIONS ;
        /* Call the Ethernet drive API to receive all the frames */
        do
        {
          LenRxStatusPtr = ETH_NOT_RECEIVED;
          #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
          /* SWS_EthIf_00097d */
          #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
          (LpDriverApis->pEthReceive)
            (LpPhysicalController_PB->ucCtrlIdx, (uint8)ETHIF_ZERO, &LenRxStatusPtr);
          #else
          (LpDriverApis->pEthReceive)
            ((EthIf_GpConfigPtr->pController[LucCtrlIdx].pPhysCtrl)->ucCtrlIdx, (uint8)ETHIF_ZERO, &LenRxStatusPtr);
          #endif
          #else
          /* SWS_EthIf_00097d */
          /* polyspace +4 RTE:IDP [Justified:Low] "This Pointer will not be out
          of bound as it will have the controller index configured with respect
          to number of configured controllers */
          #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
          Eth_Receive(LpPhysicalController_PB->ucCtrlIdx, (uint8)ETHIF_ZERO, &LenRxStatusPtr);
          #else
          /* polyspace +10 RTE:UNR [Justified:Low] "This condition will be True when
        EthIf_GpConfigPtr is not a NULL PTR.
        Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_00040a" */
          Eth_Receive((EthIf_GpConfigPtr->pController[LucCtrlIdx].pPhysCtrl)->ucCtrlIdx,
                (uint8)ETHIF_ZERO, &LenRxStatusPtr);
          #endif
          #endif
          if (ETH_RECEIVED_MORE_DATA_AVAILABLE == LenRxStatusPtr)
          {
            /* Decrement the interations if frame is received and more data
              available */
            /* SWS_EthIf_00131 */
          /* PRQA S 2100 2 */
          /* PRQA S 3383 4 */
            LucMaxNumberOfIterations--;
          }
          else
          {
            /* SWS_EthIf_00131, SWS_EthIf_00097d */
            LucMaxNumberOfIterations = (uint8)ETHIF_ZERO;
          }
        /* PRQA S 2100 1 */
        }while ((uint8)ETHIF_ZERO != LucMaxNumberOfIterations);
        /* Increment the controller index */

        /* PRQA S 3383 4 */

       }

    /* PRQA S 1251 1 */
    }
    #endif
  } /* End of if((Std_ReturnType)E_OK == LddStatusReturn) */
  #endif /*End of #if(ETHIF_ENABLE_RX_INTERRUPT == STD_OFF) */
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MainFunctionTx                                **
**                                                                            **
** Service ID           : 0x21                                                **
**                                                                            **
** Description          : This API of the EthIf module The function issues    **
**                        transmission confirmations in polling mode.         **
**                                                                            **
** Sync/Async           : NA                                                  **
**                                                                            **
** Re-entrancy          : NA                                                  **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s) :                                **
**                                        EthIf_GaaStaticController,          **
**                                        EthIf_GpFirstDriverApis             **
**                                        EthIf_GucTrcvLinkStateChgMainReload **
**                                        EthIf_GpFirstLinkStateChangeFunc    **
** Function(s) invoked  : Eth_TxConfirmation                                  **
**                        ETHIF_UL_LINKSTATECHANGE_NUM                        **
**                                                                            **
** Design ID            : ETHIF_SDD_0098                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
void EthIf_MainFunctionTx(void)
{
  Std_ReturnType LddStatusReturn;
  #if (STD_OFF == ETHIF_ENABLE_TX_INTERRUPT)

  uint8 LucCtrlIdx;
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController_PB*  LpStaticController_PB;
  const EthIf_StaticPhyController_PB*  LpPhysicalController_PB;
  #endif

  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #else
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController*  LpStaticController;
  #endif
  #endif
  #endif

  LddStatusReturn = (Std_ReturnType)E_OK ;

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf module is in INITIALIZED state.
  Module Test Case Reference ETHIF_ETC_029, SWS ID SWS_EthIf_00124" */
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_MAINFUNCTX_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  /* polyspace +5 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf module is in UNINITIALIZED state.
  Module Test Case Reference test_3" */
  /* PRQA S 2100 1 */
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    #if (STD_OFF == ETHIF_ENABLE_TX_INTERRUPT)


    /* Loop through all the controllers to receive data */
    /* polyspace +4 RTE:NTL [Justified:Low] "This loop will terminate when
    controller index count reaches number of controllers configured */
    /* PRQA S 2100 3 */
    for (LucCtrlIdx = (uint8)ETHIF_ZERO; LucCtrlIdx <
                           (uint8)ETHIF_NUM_OF_CONTROLLERS; LucCtrlIdx++)
    {
      /* Get the pointer to the requested Controller's structure */
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)

      LpStaticController_PB = &EthIf_GaaStaticController_PB[LucCtrlIdx];

      LpPhysicalController_PB = &EthIf_GaaStaticPhyController_PB[
      (LpStaticController_PB->pPhysCtrl)->ucPhysCtrlIdx];
      #endif
      #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LpDriverApis = LpPhysicalController_PB->pEthDriver;
      #else

      LpDriverApis = (EthIf_GpConfigPtr->pController[LucCtrlIdx].pPhysCtrl)->pEthDriver;
      #endif
    /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
    LpDriverApis will be NULL.
    Module Test Case Reference test_3" */
      if (LpDriverApis != NULL_PTR)
      #endif
      {
        #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
        /* SWS_EthIf_00113d, SWS_EthIf_00115a, SWS_EthIf_00115b*/
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        (LpDriverApis->pEthTxConfirmation)(LpPhysicalController_PB->ucCtrlIdx);
        #else
        (LpDriverApis->pEthTxConfirmation)((EthIf_GpConfigPtr->pController[LucCtrlIdx].pPhysCtrl)->ucCtrlIdx);
        #endif
        #else
        /* Update global variable for EthIf_TxConfirmation*/
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        LpStaticController = &EthIf_GaaStaticController[LucCtrlIdx];
        #endif
        SchM_Enter_EthIf_MAIN_PERIOD_PROTECT();
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        EthIf_GucEthIfCtrlIdx_Tx = LpStaticController->ucEthIfCtrlIdx;
        #else
        EthIf_GucEthIfCtrlIdx_Tx = EthIf_GpConfigPtr->pController[LucCtrlIdx].ucEthIfCtrlIdx;
        #endif
        SchM_Exit_EthIf_MAIN_PERIOD_PROTECT();
        /* SWS_EthIf_00113d, SWS_EthIf_00115a, SWS_EthIf_00115b*/
        /* polyspace +4 RTE:IDP [Justified:Low] "This Pointer will not be out of
        bound as it will have the controller index configured with respect to
        number of configured controllers */
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        (void)Eth_TxConfirmation(LpPhysicalController_PB->ucCtrlIdx);
        #else
        (void)Eth_TxConfirmation((EthIf_GpConfigPtr->pController[LucCtrlIdx].pPhysCtrl)->ucCtrlIdx);
        #endif
        #endif

      }

    }
    #endif
  }
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MainFunctionState                             **
**                                                                            **
** Service ID           : 0x05                                                **
**                                                                            **
** Description          : This API of the EthIf module The function checks    **
**                       Link state changes in  polling mode.                 **
**                                                                            **
** Sync/Async           : NA                                                  **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s) :                                **
**                                                                            **
** Function(s) invoked  :                                                     **
**                                                                            **
** Design ID            : ETHIF_SDD_0099                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
void EthIf_MainFunctionState(void)
{
  const EthIf_LinkStateChangeFuncPtr*
    LpLinkStateChangeFunc;
  #if (ETHIF_MKA_LINKSTATECHANGE_NUM > ETHIF_ZERO)
  const EthIf_MkaLinkStateChangeFuncPtr*
    LpMkaLinkStateChangeFunc;
  #endif
  Std_ReturnType LddStatusReturn;
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController*  LpStaticController;
  #if (ETHIF_MKA_LINKSTATECHANGE_NUM > ETHIF_ZERO)
  const EthIf_StaticController_PB*  LpStaticController_PB;
  #endif
  const EthIf_StaticTransceiver_PB*  LpStaticTransceiver_PB;
  #endif
  EthTrcv_LinkStateType LenLinkStatePtr;
  uint8 LucCount;
  uint8 LucCtrlIdx;
  uint8 LucTrcvIdx;
  static EthTrcv_LinkStateType LddLinkStateStatus[ETHIF_NUM_OF_CONTROLLERS];
  const EthIf_TrcvApis*  LpTrcvApis;

  LddStatusReturn = (Std_ReturnType)E_OK ;

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf module is in INITIALIZED state.
  Module Test Case Reference ETHIF_ETC_029, SWS ID SWS_EthIf_00124" */
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_MAINFUNCTX_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  /* polyspace +5 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf module is in UNINITIALIZED state.
  Module Test Case Reference test_3" */
  /* PRQA S 2100 2 */
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    LucTrcvIdx = (uint8)ETHIF_ZERO ;
    /* functionality for mainstate change*/
    #if (STD_ON == ETHIF_PRE_COMPILE_SINGLE)
    LpLinkStateChangeFunc = ETHIF_GPFIRSTLINKSTATECHANGEFUNC;
    #if (ETHIF_MKA_LINKSTATECHANGE_NUM > ETHIF_ZERO)
    LpMkaLinkStateChangeFunc = ETHIF_GPMKALINKSTATECHANGEFUNC;
    #endif
    #else
    LpLinkStateChangeFunc = EthIf_GpConfigPtr->pLinkStateChangeFunc;
    #if (ETHIF_MKA_LINKSTATECHANGE_NUM > ETHIF_ZERO)
    LpMkaLinkStateChangeFunc = EthIf_GpConfigPtr->pMkaLinkStateChangeFunc;
    #endif
    #endif

    /* Loop through all the controllers to receive data */
    #if (ETHIF_ONE < ETHIF_NUM_OF_TRANSCEIVERS)
    do
    {
    #endif
      /* Get the pointer to the requested Controller's structure */
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LpStaticTransceiver_PB = &EthIf_GaaStaticTransceiver_PB[LucTrcvIdx];
      LpTrcvApis = LpStaticTransceiver_PB->pTrcvDriver;
      #else
      LpTrcvApis = EthIf_GpConfigPtr->pTransceiver[LucTrcvIdx].pTrcvDriver;
      #endif

        if (LpTrcvApis != NULL_PTR)
        {
         /* polyspace +4 RTE:COR [Justified:Low] "This condition will call the
         function of EthTrcv with multidriver support enabled */
          #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
          LddStatusReturn = (LpTrcvApis->pEthTrcvGetLinkState)
            (LpStaticTransceiver_PB->ucTrcvIdx, &LenLinkStatePtr);
          #else
          LddStatusReturn = (LpTrcvApis->pEthTrcvGetLinkState)
            (EthIf_GpConfigPtr->pTransceiver[LucTrcvIdx].ucTrcvIdx, &LenLinkStatePtr);
          #endif
        }
        /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be True
        when LpTrcvApis will be invalid.
        Module Test Case Reference test_3" */
        else
        {
          LddStatusReturn = E_NOT_OK;
        }
      do
      {
        SchM_Enter_EthIf_MAIN_PERIOD_PROTECT();
        /* Decrement the EthIf_GucTrcvLinkStateChgMainReload */
        /* PRQA S 2100 2 */
        /* PRQA S 3383 4 */
        EthIf_GucTrcvLinkStateChgMainReload--;
        /* Enable all interrupts */
        SchM_Exit_EthIf_MAIN_PERIOD_PROTECT();
      /* PRQA S 2100 1 */
      }while ((uint8)ETHIF_ZERO != EthIf_GucTrcvLinkStateChgMainReload);

      SchM_Enter_EthIf_MAIN_PERIOD_PROTECT();
      /* reload EthIf_GucTrcvLinkStateChgMainReload with configured value */
      EthIf_GucTrcvLinkStateChgMainReload =
        (uint8)ETHIF_TRCVLINKSTATECHGMAINRELOAD;
      SchM_Exit_EthIf_MAIN_PERIOD_PROTECT();
      /* PRQA S 2100 1 */
      if ((Std_ReturnType)E_OK == LddStatusReturn)
      {
        for (LucCtrlIdx = (uint8)ETHIF_ZERO; LucCtrlIdx
                   < (uint8)ETHIF_NUM_OF_CONTROLLERS; LucCtrlIdx++)
        {
          LucCount = (uint8)ETHIF_ZERO ;
          #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
          LpStaticController = &EthIf_GaaStaticController[LucCtrlIdx];
          #if (ETHIF_MKA_LINKSTATECHANGE_NUM > ETHIF_ZERO)
          LpStaticController_PB = &EthIf_GaaStaticController_PB[LucCtrlIdx];
          #endif

          if (((EthIf_GaaStaticController_PB[LucCtrlIdx].pTrcv) != NULL_PTR) &&
              ((EthIf_GaaStaticTransceiver[LucTrcvIdx].ucEthIfTrcvIdx) ==
               (EthIf_GaaStaticController_PB[LucCtrlIdx].pTrcv)->ucEthIfTrcvIdx))
          {
          #else
          if ((EthIf_GpConfigPtr->pController[LucCtrlIdx].pTrancvr) != NULL_PTR)
          {
            if ((EthIf_GaaStaticTransceiver[LucTrcvIdx].ucTrcvIdx) ==
               (EthIf_GpConfigPtr->pController[LucCtrlIdx].pTrancvr)->ucTrcvIdx)
          #endif
            {
              /* Call <UL>_LinkStateChange.
              Bugzilla 58491d: Checks for transceiver state changes */
              /* SWS_EthIf_00108b */
              #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
              if (LddLinkStateStatus[LpStaticController->ucEthIfCtrlIdx]
                != LenLinkStatePtr)
              #else
              /*polyspace +4 RTE:OBAI [Justified:Low] "LucCtrlIdx point to only
              index 0. hence it will not exceed beyond its boundary. LucCtrlIdx,
              has config for only 1 EthIf. so its within bounds" */
              if (LddLinkStateStatus[EthIf_GpConfigPtr->pController[LucCtrlIdx].ucEthIfCtrlIdx]
                  != LenLinkStatePtr)
              #endif
              {
                #if (ETHIF_MKA_LINKSTATECHANGE_NUM > ETHIF_ZERO)
                #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
                if (LpStaticController_PB->usmacsec == ETHIF_SW_MACSEC)
                #else
                if (EthIf_GpConfigPtr->pController[LucCtrlIdx].usmacsec == ETHIF_SW_MACSEC)
                #endif
                {
                  (void)(*LpMkaLinkStateChangeFunc)
                  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
                  (LpStaticController_PB->pPaeInst->ucPaeIdx, LenLinkStatePtr);
                  #else
                  (EthIf_GpConfigPtr->pController[LucCtrlIdx].pPaeInst->ucPaeIdx, LenLinkStatePtr);
                  #endif
                }
                #endif

                #if (ETHIF_ONE < ETHIF_UL_LINKSTATECHANGE_NUM)
                do
                {
                #endif
                  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
                  (void)(LpLinkStateChangeFunc[LucCount])
                      (LpStaticController->ucEthIfCtrlIdx, LenLinkStatePtr);
                  #else
                  (void)(LpLinkStateChangeFunc[LucCount])
                    (EthIf_GpConfigPtr->pController[LucCtrlIdx].ucEthIfCtrlIdx, LenLinkStatePtr);
                  #endif
                  /* PRQA S 2100 3 */
                  /* PRQA S 3383 2 */
                  #if (ETHIF_ONE < ETHIF_UL_LINKSTATECHANGE_NUM)
                  LucCount++;
                  /* PRQA S 2100 2 */
                  /* PRQA S 1251 1 */
                }while (LucCount < ETHIF_UL_LINKSTATECHANGE_NUM);
                #endif

                #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
                LddLinkStateStatus[LpStaticController->ucEthIfCtrlIdx]
                  = LenLinkStatePtr;
                #else
                LddLinkStateStatus[EthIf_GpConfigPtr->pController[LucCtrlIdx].ucEthIfCtrlIdx]
                  = LenLinkStatePtr;
                #endif
              }
            }
          }
        }
      }
      /* PRQA S 2100 2 */
      /* PRQA S 3383 4 */
      #if (ETHIF_ONE < ETHIF_NUM_OF_TRANSCEIVERS)
      LucTrcvIdx++;
    /* PRQA S 2100 2 */
    /* PRQA S 1251 1 */
    }while (LucTrcvIdx < ETHIF_NUM_OF_TRANSCEIVERS );
    #endif
  }
}
/* PRQA S 5002 3 */
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
