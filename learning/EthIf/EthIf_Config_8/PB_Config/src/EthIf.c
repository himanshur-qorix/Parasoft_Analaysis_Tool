/*******************************************************************************
**                        KPIT Technologies Limited                           **
**                                                                            **
** KPIT Technologies Limited owns all the rights to this work. This work      **
** shall not be copied, reproduced, used, modified or its information         **
** disclosed without the prior written authorization of KPIT Technologies     **
** Limited.                                                                   **
**                                                                            **
** SRC-MODULE: EthIf.c                                                        **
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
** Design ID : ETHIF_SDD_0013                                                 **
*******************************************************************************/

/*******************************************************************************
**                      Revision History                                      **
********************************************************************************
** Date          Changed By      Description                                  **
********************************************************************************
** 27-Feb-2026   Atreya          As per feature #CP-21187 and #CP-1450, added **
**               Chiplunkar,     functions for MacSec support                 **
**               Vini Kadam                                                   **
** 03-Feb-2026   Atreya          As per #CP-24814, updated Ethif_TransmitPdu  **
**               Chiplunkar      for TxConfirmation handelling                **
** 20-Dec-2025   Atreya          As per #CP-10982, updated PDU based          **
**               Chiplunkar      Reception support(LsduR) using               **
**                               EthIf_ReleaseRxBuffer                        **
** 11-Nov-2025   A Naresh        As per feature #CP-12795, Transmission and   **
**                               Reception for MKPDU added MKA_LinkstateChange**
** 07-Nov-2025   Devika Ramesh   As per #CP-10982, updated PDU based          **
**                               transmission support(LsduR) using            **
**                               Ethif_TransmitPdu                            **
** 10-Oct-2025   A Naresh        As per Bug  #CP-9168 updated UL module's     **
**                               callbacks with EthIfController_Idx           **
**                               instead of  EthDrv_Idx                       **
** 24-Sep-2025   A Naresh        As per Bug  #CP-9145 EthIfPhycontroller      **
**                               accessed via EthIfcontroller for EthDRV_APIs **
** 17-Sep-2025   A Naresh        As per US  #CP-7847 Hardware TimeStamp,      **
**                               updated code for EthIfClkUnit                **
** 12-Sep-2025   A Naresh        As per US  #CP-6076, SecurityEvents updated  **
**                               code for SecurityEvent call                  **
** 30-Jul-2025   A Naresh        As per US #CP-1018, Updated static code      **
**                               as per Compiler Abstraction                  **
** 11-Jul-2025   A Naresh        As per US #198921, Initial version work-     **
**                               -product is carry over from R23-11 V1.0.0    **
*******************************************************************************/
/*******************************************************************************
**                   MISRA-C:2012 violations Section                          **
*******************************************************************************/
/**
* #section EthIf_c_QAC_REF_1
* Violates MISRA 2012 Advisory Rule 20.1,
* #include directives should only be preceded by preprocessor directives or
* comments.
* JUSTIFICATION: As per SRS_BSW_00437 in AUTOSAR_SRS_BSW General document
* Memory mapping is provided as start and stop memory sections. As neither
* executable code nor symbols are included.This violation is an approved
* exception without side effects by AUTOSAR Standard.
*
* #section EthIf_c_QAC_REF_2
* Violates MISRA 2012 Required Rule 2.2,
* Statement has no side-effect, it can be removed.
* JUSTIFICATION: As per requirement, when the functionality is disabled the
* parameters remain unused, hence they are declared as void.
**/

/* PRQA S 0380 EOF */
/* violates Number of macro definitions exceeds 4095 - program does not */
/* conform strictly to ISO:C99 */
/*invalid QAC as total number of macros in */
/* code are less than 4095*/

/* #violates #ref EthIf_c_QAC_REF_1 Violates MISRA 2012 Required Rule 20.1*/
/* PRQA S 5087 EOF */

/* Violates M3CM-2: Rule-2.1 (Advisory), rcma-3.1.0-1503 */

/* Violates qac-10.1.0-2000 */
/* PRQA S 2000 EOF */
/* Violates qac-10.1.0-2100 */
/* PRQA S 2100 EOF */
/* Violates qac-10.1.0-2100 */
/* PRQA S 2101 EOF */
/* Violates qac-10.1.0-3383 */
/* PRQA S 3383 EOF */
/*******************************************************************************
**                      CERT-C Violations Section                             **
*******************************************************************************/
/* Violates certccm-2.1.0-5001 */
/* PRQA S 5001 EOF */
/* Violates certccm-2.1.0-5002 */
/* PRQA S 5002 EOF */
/*******************************************************************************
**                      CAT Violations Section                                **
*******************************************************************************/
/*******************************************************************************
**                      ECG_Checker Violations Section                        **
*******************************************************************************/
/**
*ECG_Checker_Justification_EthIf_c_Start : WRN01
*According to the common QAC Justification, the warnings are suppressed at the
*end of the file (EOF). After reviewing all instances, no potential issues were
*identified.
*ECG_Checker_Justification_EthIf_c_Stop : WRN01
**/
/*******************************************************************************
**                      LogicAnalyzer Violations Section                      **
********************************************************************************/
/**
*LogicalAnalyzer_Justification_EthIf_c_Start : ERR04
*Manually verified that protection is not required for accessing global
*variable Based on Re-entrancy and usage of global varaible.
*LogicalAnalyzer_Justification_EthIf_c_Stop : ERR04
**/
/*******************************************************************************
**                       Polyspace Violations Section                         **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
/* Design ID : ETHIF_SDD_0363,ETHIF_SDD_0013,ETHIF_SDD_0420 */
#include "EthIf.h"
#include "SchM_EthIf.h"
#include "EthIf_IntTypes.h"
#include "EthIf_Cfg.h"
#include "Eth.h"
#if (STD_ON == ETHIF_ENABLE_WETH_API)
#include "WEth.h"
#include "WEthTrcv.h"
#endif
#include "TcpIp.h"
#include "EthSM.h"
#if (ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES)
#include "EthSwt.h"
#include "EthSwt_GeneralTypes.h"
#endif
#if ((STD_ON == ETHIF_DEV_ERROR_DETECT) || (STD_ON == ETHIF_SAFE_KSAR))
#include "Det.h"
#endif
#if (STD_ON == ETHIF_SECURITY_EVENT_ENABLED)
#include "IdsM.h"
#endif
#if (ETHIF_NO_OF_TXPDUS > ETHIF_ZERO)
#include "LSduR_EthIf.h"
#endif
/*******************************************************************************
**                                Version Check                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static Std_ReturnType  EthIf_RxIndication_DetCheck(
  uint8 CtrlIdx, const Eth_DataType * DataPtr);
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static boolean  EthIf_RxIndicationLinearSearch(Eth_FrameType FrameType,
  #if ((ETHIF_NO_OF_RXPDUS > ETHIF_ZERO) && \
       (ETHIF_PRE_COMPILE_SINGLE == STD_ON))
  uint8 PhyCtrlIdx, uint8 * LpRxPduIndex,
  #endif
  uint8 * LpRxIndex);
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#if (ETHIF_NO_OF_TXPDUS > ETHIF_ZERO)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static Std_ReturnType EthIf_TransmitInputValidation(PduIdType TxPduId,
  const PduInfoType* PduInfoPtr);
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif


#if (ETHIF_NO_OF_TXPDUS > ETHIF_ZERO) && (STD_ON == ETHIF_VLAN_SUPPORT)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static Std_ReturnType EthIf_TransmitVLANSupport(uint8* VlanInfoPtr,
  uint8 LucCtrlIdx, uint8 LucPriority, const Eth_FrameType LddFrameType);
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/* Design ID: ETHIF_SDD_0397 */
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
EthIf_StateType EthIf_GddInit;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

/* Design ID  : ETHIF_SDD_0394 */
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"
uint8 EthIf_GucTrcvLinkStateChgMainReload;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"

/* Design ID  : ETHIF_SDD_0605*/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"
uint8 EthIf_GucTrcvEthIfCtrlIdx;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"

/* Design ID  : ETHIF_SDD_0408 */
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"
uint8 EthIf_GucEthEthIfCtrlIdx;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"

/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
static EthIf_Controller EthIf_GSaaController
     [ETHIF_NUM_OF_CONTROLLERS];
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

#if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
static EthIf_TxStateType EthIf_GSaaTxState[ETHIF_SW_MACSEC_CTRL_COUNT];
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
EthIf_ControllerMacsecContextType EthIf_GaaMacsecController
     [ETHIF_SW_MACSEC_CTRL_COUNT];
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
EthIf_MacSecSeqBitsType EthIf_GaaMacSecSeqBits[ETHIF_SW_MACSEC_CTRL_COUNT];
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif

/* Design ID  : ETHIF_SDD_0394 */
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
Eth_ModeType EthIf_GaaStatusController[ETHIF_NUM_OF_CONTROLLERS];
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
Eth_ModeType EthIf_GaaStatusPhysController
    [ETHIF_NUM_OF_PHYSICAL_CONTROLLERS];
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
Eth_ModeType EthIf_GaaStatusTrcv[ETHIF_NUM_OF_TRANSCEIVERS];
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

#if (STD_ON == ETHIF_PRE_COMPILE_SINGLE)
/* Design ID  : ETHIF_SDD_0xxx */
#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
const EthIf_MkaLinkStateChangeFuncPtr*
  EthIf_GpMkaLinkStateChangeFunc;
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
#endif

#if (ETHIF_PRE_COMPILE_SINGLE == STD_OFF)
/* Design ID  : ETHIF_SDD_0596 */
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
const EthIf_ConfigType*  EthIf_GpConfigPtr;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"

/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
const EthIf_TxConFuncPtr*
  EthIf_GpFirstTxConfirmationFunc;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"

/* Design ID  : ETHIF_SDD_0598 */
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
const EthIf_LinkStateChangeFuncPtr*
  EthIf_GpFirstLinkStateChangeFunc;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"

/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
const EthIf_StaticController*
  EthIf_GpFirstStaticController;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"

/* Design ID  : ETHIF_SDD_0401 */
#if (ETHIF_ETH_DRIVER_COUNT > ETHIF_ONE)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
const EthIf_DrvApis*  EthIf_GpFirstDriverApis;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
#endif

/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
const EthIf_TrcvApis*  EthIf_GpFirstTrcvApis;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"

/* Design ID  : ETHIF_SDD_0402 */
#if (ETHIF_SWT_DRIVER_COUNT > ETHIF_ONE)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
const EthIf_SwtApis*  EthIf_GpFirstSwtApis;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
#endif

/* PRQA S 4640 1 */
#if (ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES)
#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
const EthIf_StaticSwitch*  EthIf_GpFirstStaticSwitch;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
#endif

/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
const EthIf_FrameTypeToRxIndFunc*
  EthIf_GpFirstFrameTypeToRxIndFunc;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
#endif

#if (STD_ON == ETHIF_MEASUREMENT_DATA_API)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_VAR_CLEARED_32
#include "EthIf_MemMap.h"
uint32 EthIf_GulVlanDiagMeasData;
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_VAR_CLEARED_32
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
**                              In-line Functions                             **
*******************************************************************************/

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static Std_ReturnType  EthIf_SetControllerMode_DetCheck(
  uint8 CtrlIdx, Eth_ModeType CtrlMode);
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static BufReq_ReturnType  EthIf_ProvideTxBuffer_DetCheck(
  uint8 CtrlIdx, const Eth_BufIdxType* BufIdxPtr, const uint16* LenBytePtr);
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static Std_ReturnType  EthIf_SetTrcvMode_ActiveDown(
   uint8 CtrlIdx, Eth_ModeType TrcvMode);
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#if (STD_ON == ETHIF_VLAN_SUPPORT)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static void  EthIf_RxIndication_SecurityVlanCheck(
  uint16 VlanId);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

#if (STD_ON == ETHIF_GLOBALTIME_SUPPORT)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static Std_ReturnType  EthIf_globalTime_Inputvalidation(
  uint8 CtrlIdx, uint8 ClkIdx, uint8 GlobalTime_API_SID);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static Std_ReturnType  EthIf_globalTime_GetDrvIds(
  uint8 CtrlUnitIdx, uint8 ClkUnitIdx, uint8* CtrlIdx, uint8* ClkIdx);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

#ifdef ETHIF_IMMEDIATE_TRANSMIT_SUPPORT
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static uint8 * EthIf_CreateHeaderList(
  uint8 * DestMacAddress, uint8 * SrcMacAddress,
  #if (STD_ON == ETHIF_VLAN_SUPPORT)
  uint8 * LucVlanInfo,
  #endif
  Eth_FrameType FrameType);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

#if (ETHIF_NO_OF_TXPDUS > ETHIF_ZERO)
#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
EthIf_TxPduStatusType EthIf_GaaTxPduStat[ETHIF_NO_OF_TXPDUS];
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif

#if (ETHIF_NO_OF_RXPDUS > ETHIF_ZERO)
#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
EthIf_RxPduStatusType EthIf_GaaRxPduStat[ETHIF_NO_OF_RXPDUS];
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
**                      Function Definitions                                  **
*******************************************************************************/

/*******************************************************************************
** Function Name        : EthIf_Init                                          **
**                                                                            **
** Service ID           : 0x01                                                **
**                                                                            **
** Description          : Performs initialization of the ETHIF Module.        **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : ConfigPtr                                           **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)    :                             **
**                        EthIf_GddInit ,EthIf_GucTrcvLinkStateChgMainReload  **
**                        EthIf_GpFirstSwtApis,                               **
**                        EthIf_GpDrvToCtrlMapArray,EthIf_GpFirstDriverApis,  **
**                        EthIf_GpDrvToSwtMapArray, EthIf_GpFirstTrcvApis,    **
**                        EthIf_GpDrvToTrcvMapArray,                          **
**                        EthIf_GpFirstLinkStateChangeFunc,                   **
**                        EthIf_GpConfigPtr                                   **
**                        Function(s) invoked   : Det_ReportError             **
**                                                                            **
** Design ID            : ETHIF_SDD_0095                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/*
ETHIF_SRS_007: The EthIf.c file shall include EthIf_MemMap.h
*/
/* PRQA S 3227 2 */
void EthIf_Init(const EthIf_ConfigType*  ConfigPtr)
{
  uint8 LucIndex;
  #if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
  uint8 LucSAIndex;
  #endif

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
     the configuration is Precompile and ConfigPtr is equal to NULL.
     Module Test Case Reference ETHIF_ETC_110" */
  if (EthIf_GddInit != (EthIf_StateType)ETHIF_CTRL_STATE_INIT)
  {

    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    Std_ReturnType LddStatusReturn;
    LddStatusReturn = (Std_ReturnType)E_OK;
    #endif

    /* SWS_EthIf_00116
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    #if (STD_OFF == ETHIF_DEV_ERROR_DETECT)
    /*Violates MISRA 2012 Required Rule 2.2, #ref EthIf_c_QAC_REF_2 */

    ETHIF_UNUSED(ConfigPtr);
    #else
    #if (STD_OFF == ETHIF_PRE_COMPILE_SINGLE)
    if ((NULL_PTR == ConfigPtr) ||
        ((uint32)ETHIF_DBTOC_VALUE != ConfigPtr->ulStartOfDbToc))
    #else
    /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
    the configuration is Precompile and ConfigPtr is equal to NULL.
    Module Test Case Reference ETHIF_ETC_110, SWS ID SWS_EthIf_00116"
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if (NULL_PTR != ConfigPtr)
    #endif
    {
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_INIT_SID, ETHIF_E_INIT_FAILED));
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
    /* polyspace +5 RTE:UNR [Justified:Low] "This condition shall be True when
    the configuration is Precompile and ConfigPtr is equal to NULL.
    Module Test Case Reference ETHIF_ETC_110, SWS ID SWS_EthIf_00116"
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if ((Std_ReturnType)E_OK == LddStatusReturn)
    #endif
    {
      #if (STD_OFF == ETHIF_PRE_COMPILE_SINGLE)
      /* polyspace +5 RTE:UNR [Justified:Low] "This condition shall be True when
         the configuration is Precompile and ConfigPtr is equal to NULL.
         Module Test Case Reference ETHIF_ETC_110, SWS ID SWS_EthIf_00116"
         Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      EthIf_GpConfigPtr = ConfigPtr;
      #endif
      /* SWS_EthIf_00114
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      EthIf_GddInit = (EthIf_StateType)ETHIF_CTRL_STATE_INIT;

      /*Initialize the EthIf_GucTrcvLinkStateChgMainReload with configured
               value*/
      EthIf_GucTrcvLinkStateChgMainReload =
      (uint8)ETHIF_TRCVLINKSTATECHGMAINRELOAD;

      for (LucIndex = ETHIF_ZERO; LucIndex < ETHIF_NUM_OF_CONTROLLERS; LucIndex++)
      {
        EthIf_GaaStatusController[LucIndex] = ETH_MODE_DOWN;
      }

      for (LucIndex = ETHIF_ZERO; LucIndex < ETHIF_NUM_OF_PHYSICAL_CONTROLLERS; LucIndex++)
      {
        EthIf_GaaStatusPhysController[LucIndex] = ETH_MODE_DOWN;
      }

      for (LucIndex = ETHIF_ZERO; LucIndex < ETHIF_NUM_OF_TRANSCEIVERS; LucIndex++)
      {
        EthIf_GaaStatusTrcv[LucIndex] = ETH_MODE_DOWN;
      }

      #if (ETHIF_NO_OF_TXPDUS > ETHIF_ZERO)
      for (LucIndex = ETHIF_ZERO; LucIndex < ETHIF_NO_OF_TXPDUS; LucIndex++)
      {
        EthIf_GaaTxPduStat[LucIndex].ddConfPduId = ETHIF_INVALID_PDUID;
        EthIf_GaaTxPduStat[LucIndex].BuffIdx = ETHIF_INVALID_32;
        EthIf_GaaTxPduStat[LucIndex].ucPhyCtrlIndex = ETHIF_INVALID;
        EthIf_GaaTxPduStat[LucIndex].PduState = ETHIF_PDU_AVAILABLE;
        EthIf_GaaTxPduStat[LucIndex].TxnState = ETHIF_STATE_DEFAULT;
      }
      #endif

      #if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
      for (LucIndex = ETHIF_ZERO; LucIndex < ETHIF_SW_MACSEC_CTRL_COUNT; LucIndex++)
      {
        EthIf_GaaMacsecController[LucIndex].IsProtectFramesEnabled = ETHIF_FALSE;
        EthIf_GaaMacsecController[LucIndex].IsMacSecOperational = ETHIF_FALSE;
        EthIf_GaaMacsecController[LucIndex].ActiveTxAnIndex = ETHIF_INVALID;
        for (LucSAIndex = ETHIF_ZERO; LucSAIndex < ETHIF_MAX_TX_SA; LucSAIndex++)
        {
          EthIf_GaaMacsecController[LucIndex].TxSa[LucSAIndex].Active = ETHIF_FALSE;
          EthIf_GaaMacsecController[LucIndex].TxSa[LucSAIndex].NextPn = ETHIF_ZERO;
        }
      }
      #endif
    }
  }
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_SetControllerMode_DetCheck                    **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : Det check for EthIf_SetControllerMode               **
**                                                                            **
** Sync/Async           : NA                                                  **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx                                             **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s): None                            **
**                                                                            **
**                        Function(s) invoked:                                **
**                        Det_ReportError                                     **
**                                                                            **
** Design ID            : ETHIF_SDD_0158                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 2 */
static Std_ReturnType EthIf_SetControllerMode_DetCheck(
  uint8 CtrlIdx, Eth_ModeType CtrlMode)
{
  Std_ReturnType LddStatusReturn;

  LddStatusReturn = (Std_ReturnType)E_OK;

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf module is in INITIALIZED state.
  Module Test Case Reference ETHIF_ETC_133, SWS ID SWS_EthIf_00036"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_SETCNTRLMODE_SID, ETHIF_E_UNINIT));
  #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_SETCNTRLMODE_SID, ETHIF_E_INV_CTRL_IDX));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  /* polyspace +5 RTE:UNR [Justified:Low] "This condition shall be True when
  CtrlMode will not be equal to ETH_MODE_ACTIVE and ETH_MODE_DOWN.
  Module Test Case Reference ETHIF_ETC_490" */
  if ((ETH_MODE_ACTIVE != CtrlMode) && (ETH_MODE_DOWN != CtrlMode) &&
  (ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST != CtrlMode))
  {
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_SetTrcvMode_ActiveDown                        **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : Switchs Trcv Mode as per EthIf_SetControllerMode    **
**                                                                            **
** Sync/Async           : NA                                                  **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : TrcvIdx, TrcvMode                                   **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s): EthIf_GaaStatusTrcv             **
**                                                                            **
** Design ID            : ETHIF_SDD_0159                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* polyspace-begin RTE:UNR [Justified:Low] "Refer Ethif_c_Poly_REF_1"*/
/* PRQA S 3227 2 */
static Std_ReturnType EthIf_SetTrcvMode_ActiveDown(
  uint8 CtrlIdx, Eth_ModeType TrcvMode)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController_PB* LpStaticController_PB;
  const EthIf_TrcvApis*  LpTrcvApis;
  const EthIf_StaticTransceiver_PB* LpStaticTransceiver_PB;
  #else
  const EthIf_TrcvApis*  LpTrcvApis;
  #endif

  Std_ReturnType LddStatusReturn;
  uint8 LucTrcvIdx;

  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  LpStaticController_PB = &EthIf_GaaStaticController_PB[CtrlIdx];
  #endif

  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  if ((LpStaticController_PB->pTrcv_PB) != NULL_PTR)
  #else
  if ((EthIf_GpConfigPtr->pController[CtrlIdx].pTrancvr) != NULL_PTR)
  #endif
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LucTrcvIdx = (LpStaticController_PB->pTrcv_PB)->ucTrcvIdx;
    #else
    LucTrcvIdx = (EthIf_GpConfigPtr->pController[CtrlIdx].pTrancvr)->ucTrcvIdx;
    #endif
  }
  else
  {
    LucTrcvIdx = ETHIF_ZERO;
  }
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  LpStaticTransceiver_PB = &EthIf_GaaStaticTransceiver_PB[LucTrcvIdx];
  LpTrcvApis = LpStaticTransceiver_PB->pTrcvDriver;
  #else
  LpTrcvApis = EthIf_GpConfigPtr->pTransceiver[LucTrcvIdx].pTrcvDriver;
  #endif

  LddStatusReturn = (Std_ReturnType)E_OK;
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  if (LpStaticController_PB->pTrcv_PB != NULL_PTR)
  #else
  if (LpTrcvApis != NULL_PTR)
  #endif
  {
    /* SWS_EthIf_00266
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    /*polyspace +4 RTE:OBAI [Justified:Low] "LucTrcvIdx point to only
    index 0. hence it will not exceed beyond its boundary. LucTrcvIdx,
    has config for only 1 EthIf. so its within bounds" */
    if ((TrcvMode == ETH_MODE_ACTIVE) &&
      (EthIf_GaaStatusTrcv[LucTrcvIdx] == ETH_MODE_DOWN))
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LddStatusReturn = (LpTrcvApis->pEthTrcvSetTransceiverMode)
        (LpStaticTransceiver_PB->ucTrcvIdx, TrcvMode);
      #else
      LddStatusReturn = (LpTrcvApis->pEthTrcvSetTransceiverMode)
        (EthIf_GpConfigPtr->pTransceiver[LucTrcvIdx].ucTrcvIdx, TrcvMode);
      #endif
      if ((Std_ReturnType)E_OK == LddStatusReturn)
      {
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        LddStatusReturn = (LpTrcvApis->pEthTrcvTransceiverLinkStateRequest)
            (LpStaticTransceiver_PB->ucTrcvIdx, ETHTRCV_LINK_STATE_ACTIVE);
        #else
        LddStatusReturn = (LpTrcvApis->pEthTrcvTransceiverLinkStateRequest)
            (EthIf_GpConfigPtr->pTransceiver[LucTrcvIdx].ucTrcvIdx, ETHTRCV_LINK_STATE_ACTIVE);
        #endif
      }
      else
      {
        /* Do Nothing */
      }
      if ((Std_ReturnType)E_OK == LddStatusReturn)
       {
        /* Update mode status of requested EthIf Transceiver */
        EthIf_GaaStatusTrcv[LucTrcvIdx] = ETH_MODE_ACTIVE;
      }
    }
    /* SWS_EthIf_00479
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    else if (TrcvMode == ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST)
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LddStatusReturn = (LpTrcvApis->pEthTrcvSetTransceiverMode)
        (LpStaticTransceiver_PB->ucTrcvIdx, TrcvMode);
      #else
      LddStatusReturn = (LpTrcvApis->pEthTrcvSetTransceiverMode)
        (EthIf_GpConfigPtr->pTransceiver[LucTrcvIdx].ucTrcvIdx, TrcvMode);
      #endif
      if ((Std_ReturnType)E_OK == LddStatusReturn)
      {
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        LddStatusReturn = (LpTrcvApis->pEthTrcvTransceiverLinkStateRequest)
          (LpStaticTransceiver_PB->ucTrcvIdx, ETHTRCV_LINK_STATE_ACTIVE);
        #else
        LddStatusReturn = (LpTrcvApis->pEthTrcvTransceiverLinkStateRequest)
          (EthIf_GpConfigPtr->pTransceiver[LucTrcvIdx].ucTrcvIdx, ETHTRCV_LINK_STATE_ACTIVE);
        #endif
      }
      else
      {
        /* Do Nothing */
      }
      if ((Std_ReturnType)E_OK == LddStatusReturn)
      {
        /*polyspace +4 RTE:OBAI [Justified:Low] "LucTrcvIdx point to only
        index 0. hence it will not exceed beyond its boundary. LucTrcvIdx,
        has config for only 1 EthIf. so its within bounds" */
        /* Update mode status of requested EthIf Transceiver */
        EthIf_GaaStatusTrcv[LucTrcvIdx] = ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST;
      }
    }
    /* SWS_EthIf_00484
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    /*polyspace +4 RTE:OBAI [Justified:Low] "LucTrcvIdx point to only
    index 0. hence it will not exceed beyond its boundary. LucTrcvIdx,
    has config for only 1 EthIf. so its within bounds" */
    else if ((TrcvMode == ETH_MODE_DOWN) &&
      (EthIf_GaaStatusTrcv[LucTrcvIdx] != ETH_MODE_DOWN))
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LddStatusReturn = (LpTrcvApis->pEthTrcvSetTransceiverMode)
      (LpStaticTransceiver_PB->ucTrcvIdx, ETH_MODE_DOWN);
      #else
      LddStatusReturn = (LpTrcvApis->pEthTrcvSetTransceiverMode)
      (EthIf_GpConfigPtr->pTransceiver[LucTrcvIdx].ucTrcvIdx, ETH_MODE_DOWN);
      #endif
      if ((Std_ReturnType)E_OK == LddStatusReturn)
      {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        LddStatusReturn = (LpTrcvApis->pEthTrcvTransceiverLinkStateRequest)
      (LpStaticTransceiver_PB->ucTrcvIdx, ETHTRCV_LINK_STATE_DOWN);
      #else
        LddStatusReturn = (LpTrcvApis->pEthTrcvTransceiverLinkStateRequest)
      (EthIf_GpConfigPtr->pTransceiver[LucTrcvIdx].ucTrcvIdx, ETHTRCV_LINK_STATE_DOWN);
      #endif
      }
      else
      {
        /* Do Nothing */
      }

      if ((Std_ReturnType)E_OK == LddStatusReturn)
      {
        /* Update mode status of requested EthIf Transceiver */
        EthIf_GaaStatusTrcv[LucTrcvIdx] = ETH_MODE_DOWN;
      }
    }
    else
    {
      /* Do Nothing */
    }
  }
  else
  {
    /* Do Nothing */
  }
  return(LddStatusReturn);
}
 /* polyspace-end RTE:UNR [Justified:Low] "Refer Ethif_c_Poly_REF_1"*/
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_SetControllerMode                             **
**                                                                            **
** Service ID           : 0x03                                                **
**                                                                            **
** Description          : This API service of the Ethernet Interface sets the **
**                        Eth Controller identified by CtrlIdx into           **
**                        the mode given by CtrlMode.                         **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx,CtrlMode                                    **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstDriverApis        **
**                                                                            **
**                        Function(s) invoked: Eth_SetControllerMode          **
**                                                                            **
** Design ID            : ETHIF_SDD_0091                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* Violates certccm-2.1.0-5001 */

/* PRQA S 3227 2 */
Std_ReturnType EthIf_SetControllerMode (uint8 CtrlIdx, Eth_ModeType CtrlMode)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController_PB* LpStaticController_PB;
  const EthIf_StaticController_PB* LpStaticController_Tmp;
  const EthIf_StaticPhyController_PB* LpPhysicalController_Tmp;
  const EthIf_StaticPhyController_PB* LpPhysicalController_PB;
  #endif
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  boolean LblLoopFlagCheck;
  Std_ReturnType LddStatusReturn;
  uint8 LucCount;
  LucCount = ETHIF_ZERO;

  LddStatusReturn = EthIf_SetControllerMode_DetCheck(CtrlIdx, CtrlMode);

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be True when
  EthIf module is in INITIALIZED state and No DET error is present.
  Module Test Case Reference ETHIF_ETC_133, SWS ID SWS_EthIf_00036"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Get the pointer to the requested Controller's structure */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpStaticController_PB = &EthIf_GaaStaticController_PB[CtrlIdx];

    LpPhysicalController_PB = &EthIf_GaaStaticPhyController_PB[
      (LpStaticController_PB->pPhysCtrl)->ucPhysCtrlIdx];
    #endif
    #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpDriverApis = LpPhysicalController_PB->pEthDriver;
    #else
    LpDriverApis = (EthIf_GpConfigPtr->pStaticPhyController[(EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucPhysController].pEthDriver);
    #endif
    /* polyspace +6 RTE:UNR [Justified:Low] "This condition will be False when
    LpDriverApis will be NULL.
    Module Test Case Reference SWS_EthIf_00034h, SWS_EthIf_00035"
     Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if (LpDriverApis != NULL_PTR)
    #endif
    {
      /*SWS_EthIf_00034h, SWS_EthIf_00035
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      /* Update global variable for EthIf_CtrlModeIndication*/
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      EthIf_GucEthEthIfCtrlIdx = LpPhysicalController_PB->ucCtrlIdx;
      #else
    /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
  EthIf_GpConfigPtr is not a NULL PTR.
  Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_00035"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      EthIf_GucEthEthIfCtrlIdx = EthIf_GpConfigPtr->pStaticPhyController
        [(EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucPhysController].ucCtrlIdx;
      #endif
      if ((CtrlMode == ETH_MODE_ACTIVE) && (EthIf_GaaStatusController[CtrlIdx]
        == ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST))
      {
        /* Discard the request */
      }
      /* Check requested mode and previous status of requested EthIf controller */
      else if ((CtrlMode == ETH_MODE_ACTIVE) || (CtrlMode == ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST))
      {
        LblLoopFlagCheck = FALSE;
        #if (ETHIF_ONE < ETHIF_NUM_OF_PHYSICAL_CONTROLLERS)
        do
        {
        #endif
          #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
          LpPhysicalController_Tmp = &EthIf_GaaStaticPhyController_PB[LucCount];
          #endif
          /* polyspace +5 RTE:IDP [Justified:Low] "This Pointer will not be out
          of bound as it will have the controller index configured with respect
          to number of configured controllers  */
          #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
          if ((EthIf_GaaStatusPhysController[LucCount] == ETH_MODE_ACTIVE) &&
           (LpPhysicalController_Tmp->ucCtrlIdx == LpPhysicalController_PB->ucCtrlIdx))
          #else
          if ((EthIf_GaaStatusPhysController[LucCount] == ETH_MODE_ACTIVE) &&
                (EthIf_GpConfigPtr->pStaticPhyController[LucCount].ucCtrlIdx ==
                    (EthIf_GpConfigPtr->pStaticPhyController
                        [(EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucPhysController].ucCtrlIdx)))
          #endif
          {
            LblLoopFlagCheck = TRUE;
            #if (ETHIF_ONE < ETHIF_NUM_OF_PHYSICAL_CONTROLLERS)
            LucCount = ETHIF_NUM_OF_PHYSICAL_CONTROLLERS;
            #endif
            /* Update mode status of requested EthIf Controller */
            SchM_Enter_EthIf_MAIN_PERIOD_PROTECT();
            EthIf_GaaStatusController[CtrlIdx] = CtrlMode;
            SchM_Exit_EthIf_MAIN_PERIOD_PROTECT();
          }
          #if (ETHIF_ONE < ETHIF_NUM_OF_PHYSICAL_CONTROLLERS)
          /*Violates ref EthIf_c_QAC_REF_2, INT30 (Cert-C) */

          else
          {
            LucCount++;
          }

        }while (LucCount < (uint8)ETHIF_NUM_OF_PHYSICAL_CONTROLLERS);
        #endif

        /* Check the flag status and invoke Eth_SetControllerMode API*/
        if (!(LblLoopFlagCheck))
        {
          /* SWS_EthIf_00035
          Once EA tool is ready, SWS_ID will be updated with Design_ID*/
          #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)

          #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
          LddStatusReturn = (LpDriverApis->pEthSetControllerMode)
            (LpPhysicalController_PB->ucCtrlIdx, ETH_MODE_ACTIVE);
          #else
          LddStatusReturn = (LpDriverApis->pEthSetControllerMode)
            ((EthIf_GpConfigPtr->pStaticPhyController[(EthIf_GpConfigPtr->
               pController[CtrlIdx].pPhysCtrl)->ucPhysController].ucCtrlIdx), ETH_MODE_ACTIVE);
          #endif

          #else
          /* polyspace +5 RTE:IDP [Justified:Low] "This Pointer will not be
          out of bound as it will have the controller index configured with
          respect to number of configured controllers  */
          #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
          LddStatusReturn = Eth_SetControllerMode
            (LpPhysicalController_PB->ucCtrlIdx, ETH_MODE_ACTIVE);
          #else
          LddStatusReturn = Eth_SetControllerMode
            ((EthIf_GpConfigPtr->pStaticPhyController[(EthIf_GpConfigPtr->
              pController[CtrlIdx].pPhysCtrl)->ucPhysController].ucCtrlIdx), ETH_MODE_ACTIVE);
          #endif
          #endif
          if ((Std_ReturnType)E_OK == LddStatusReturn)
          {
            /* Update mode status of requested EthIf Controller */
            EthIf_GaaStatusController[CtrlIdx] = CtrlMode;
            #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
            EthIf_GaaStatusPhysController[LpPhysicalController_PB->ucCtrlIdx] = ETH_MODE_ACTIVE;
            #else
            /*polyspace +4 RTE:OBAI [Justified:Low] "ucCtrlIdx point to only
            index 0. hence it will not exceed beyond its boundary. ucCtrlIdx,
            has config for only 1 EthIf. so its within bounds" */
            EthIf_GaaStatusPhysController[(EthIf_GpConfigPtr->pStaticPhyController[(EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucPhysController].ucCtrlIdx)] = ETH_MODE_ACTIVE;
            #endif
          }

          LddStatusReturn = EthIf_SetTrcvMode_ActiveDown (CtrlIdx, CtrlMode);
        }
        else
        {
          /* EthSM_CtrlModeIndication is invoked as Eth Driver for particular
          EthIf Controller is alredy ACTIVE, No need to active another Ethif
          controller having same Eth index  */
          EthSM_CtrlModeIndication(CtrlIdx, CtrlMode);
        }
      }
      /* Check requested mode(ETH_MODE_DOWN) & previous status of requested
         EthIf controller */
      else
      {
        if ((CtrlMode == ETH_MODE_DOWN) && (EthIf_GaaStatusController[CtrlIdx] != ETH_MODE_DOWN))
        {
          EthIf_GaaStatusController[CtrlIdx] = ETH_MODE_DOWN;
          LblLoopFlagCheck = FALSE;
          /* PRQA S 2100 3 */
          for (LucCount = (uint8)ETHIF_ZERO; ((LucCount <
          (uint8)ETHIF_NUM_OF_CONTROLLERS) && (!(LblLoopFlagCheck))); LucCount++)
          {
            #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
            LpStaticController_Tmp = &EthIf_GaaStaticController_PB[LucCount];
            LpPhysicalController_Tmp = &EthIf_GaaStaticPhyController_PB[
              (LpStaticController_Tmp->pPhysCtrl)->ucPhysCtrlIdx];
            #endif
            /* polyspace +5 RTE:IDP [Justified:Low] "This Pointer will not be
            out of bound as it will have the controller index configured with
            respect to number of configured controllers  */
            #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
            if ((EthIf_GaaStatusController[LucCount] == ETH_MODE_ACTIVE) &&
           (LpPhysicalController_Tmp->ucCtrlIdx == LpPhysicalController_PB->ucCtrlIdx))
            #else
            if ((EthIf_GaaStatusController[LucCount] == ETH_MODE_ACTIVE) &&
           ((EthIf_GpConfigPtr->pStaticPhyController
                [(EthIf_GpConfigPtr->pController[LucCount].pPhysCtrl)->ucPhysController].ucCtrlIdx) ==
            (EthIf_GpConfigPtr->pStaticPhyController[(EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucPhysController].ucCtrlIdx)))
            #endif
            {
              LblLoopFlagCheck = TRUE;

            }
            /*Violates ref EthIf_c_QAC_REF_2, INT30 (Cert-C) */

            else
            {
               /* Do Nothing */
            }

          }

          /* Check the flag status and invoke Eth_SetControllerMode API*/
          if (!(LblLoopFlagCheck))
          {
            /* SWS_EthIf_00263
            Once EA tool is ready, SWS_ID will be updated with Design_ID*/
            #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
            #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
            LddStatusReturn = (LpDriverApis->pEthSetControllerMode)
              (LpPhysicalController_PB->ucCtrlIdx, CtrlMode);
            #else
            LddStatusReturn = (LpDriverApis->pEthSetControllerMode)((EthIf_GpConfigPtr->pStaticPhyController[(EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucPhysController].ucCtrlIdx), CtrlMode);
            #endif
            #else
            /* polyspace +5 RTE:IDP [Justified:Low] "This Pointer will not be
            out of bound as it will have the controller index configured with
            respect to number of configured controllers  */
            #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
             LddStatusReturn = Eth_SetControllerMode(
                          LpPhysicalController_PB->ucCtrlIdx, CtrlMode);
            #else
             LddStatusReturn = Eth_SetControllerMode(
                          (EthIf_GpConfigPtr->pStaticPhyController[(EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucPhysController].ucCtrlIdx), CtrlMode);
            #endif
            #endif
            if ((Std_ReturnType)E_OK == LddStatusReturn)
            {
              /* Update mode status of requested EthIf Phys Controller */
              #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
              EthIf_GaaStatusPhysController[LpPhysicalController_PB->ucCtrlIdx] = ETH_MODE_DOWN;
              #else
              /*polyspace +4 RTE:OBAI [Justified:Low] "ucCtrlIdx point to only
              index 0. hence it will not exceed beyond its boundary. ucCtrlIdx,
              has config for only 1 EthIf. so its within bounds" */
              EthIf_GaaStatusPhysController[(EthIf_GpConfigPtr->pStaticPhyController[(EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucPhysController].ucCtrlIdx)] = ETH_MODE_DOWN;
              #endif
            }
          }
          else
          {
            /* EthSM_CtrlModeIndication is invoked as Eth Driver for perticular
            EthIf Controller is alredy ACTIVE, No need to active another Ethif
            controller having same Eth index  */

            EthSM_CtrlModeIndication(CtrlIdx, CtrlMode);
          }
          /* SWS_EthIf_00484
           Once EA tool is ready, SWS_ID will be updated with Design_ID*/
          LddStatusReturn = EthIf_SetTrcvMode_ActiveDown (CtrlIdx, CtrlMode);
        }
      }
    }
  }/* End of if ((Std_ReturnType)E_OK == LddStatusReturn) */
  /* SWS_EthIf_00034f
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_GetControllerMode                             **
**                                                                            **
** Service ID           : 0x04                                                **
**                                                                            **
** Description          : This API service of the Ethernet Interface gets the **
**                        Eth Controller status identified by CtrlIdx into    **
**                        the CtrlModePtr.                                    **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx                                             **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : CtrlModePtr                                         **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstDriverApis        **
**                                                                            **
**                        Function(s) invoked:Eth_GetControllerMode           **
**                                                                            **
** Design ID            : ETHIF_SDD_0089                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/*SWS_EthIf_00039c, SWS_EthIf_00039e
Once EA tool is ready, SWS_ID will be updated with Design_ID*/
/* PRQA S 3227 3 */
Std_ReturnType EthIf_GetControllerMode(uint8 CtrlIdx, Eth_ModeType* CtrlModePtr)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController_PB* LpStaticController_PB;
  const EthIf_StaticPhyController_PB*  LpPhysicalController_PB;
  #endif
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  Std_ReturnType LddStatusReturn;
  LddStatusReturn = (Std_ReturnType)E_OK;

  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETCNTRLMODE_SID, ETHIF_E_INV_CTRL_IDX));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_137, SWS ID SWS_EthIf_00041"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETCNTRLMODE_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  #if ((STD_ON == ETHIF_DEV_ERROR_DETECT) || (STD_ON == ETHIF_SAFE_KSAR))
  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
  CtrlModePtr is NULL PTR.
  Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_00043"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == CtrlModePtr)
  {
    #if (STD_ON == ETHIF_SAFE_KSAR)
    (void)(Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETCNTRLMODE_SID, ETHIF_E_PARAM_POINTER));
    #else
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_GETCNTRLMODE_SID, ETHIF_E_PARAM_POINTER));
      #endif
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif
  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
  EthIf module is in INITIALIZED state and no DET error is present.
  Module Test Case Reference ETHIF_ETC_137, SWS ID SWS_EthIf_00041"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Get the pointer to the requested Controller's structure */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpStaticController_PB = &EthIf_GaaStaticController_PB[CtrlIdx];

    LpPhysicalController_PB = &EthIf_GaaStaticPhyController_PB[
      (LpStaticController_PB->pPhysCtrl)->ucPhysCtrlIdx];
    #endif
    #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpDriverApis = LpPhysicalController_PB->pEthDriver;
    #else
    LpDriverApis = (EthIf_GpConfigPtr->pStaticPhyController[(EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucPhysController].pEthDriver);
    #endif
    /* polyspace +6 RTE:UNR [Justified:Low] "This condition will be False when
    LpDriverApis will be NULL.
    Module Test Case Reference ETHIF_ETC_137, SWS ID SWS_EthIf_00041"
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if (LpDriverApis != NULL_PTR)
    {
       /*SWS_EthIf_00039h, SWS_EthIf_00040a
       Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LddStatusReturn = (LpDriverApis->pEthGetControllerMode)
        (LpPhysicalController_PB->ucCtrlIdx, CtrlModePtr);
      #else
      LddStatusReturn = (LpDriverApis->pEthGetControllerMode)
        ((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx, CtrlModePtr);
      #endif
    }
    #else
    /*SWS_EthIf_00039h, SWS_EthIf_00040a
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    /* polyspace +4 RTE:IDP [Justified:Low] "This Pointer will not be out of
    bound as it will have the controller index configured with respect to
    number of configured controllers  */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LddStatusReturn = Eth_GetControllerMode(LpPhysicalController_PB->ucCtrlIdx,
      CtrlModePtr);
    #else
    /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
    EthIf_GpConfigPtr is not a NULL PTR.
    Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_00040a"
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    LddStatusReturn = Eth_GetControllerMode(
    (EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx, CtrlModePtr);
    #endif
    #endif

  }
  /* SWS_EthIf_00039f
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_GetTransceiverMode                            **
**                                                                            **
** Service ID           : 0x07                                                **
**                                                                            **
** Description          : This API service of the Ethernet Interface gets the **
**                        Eth Trcv status identified by CtrlIdx into          **
**                        the TrcvModePtr.                                    **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx                                             **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : TrcvModePtr                                         **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                                                            **
**                        Function(s) invoked: EthTrcv_GetTransceiverMode     **
**                                                                            **
** Design ID            : ETHIF_SDD_0113                                      **
*******************************************************************************/
#if (ETHIF_ZERO < ETHIF_NUM_OF_TRANSCEIVERS)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 3 */
Std_ReturnType EthIf_GetTransceiverMode(
  uint8 TrcvIdx, Eth_ModeType * TrcvModePtr)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticTransceiver_PB*  LpStaticTransceiver_PB;
  #endif
  const EthIf_TrcvApis*  LpTrcvApis;
  Std_ReturnType LddStatusReturn;

  LddStatusReturn = (Std_ReturnType)E_OK;

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_116, SWS ID SWS_EthIf_00057"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETTRCVMODE_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  else
  {
    #if ((STD_ON == ETHIF_DEV_ERROR_DETECT) || (STD_ON == ETHIF_SAFE_KSAR))
    /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
    TrcvModePtr is NULL_PTR .
    Module Test Case Reference ETHIF_ETC_031, SWS ID SWS_EthIf_00059 "
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if (NULL_PTR == TrcvModePtr)
    {
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_GETTRCVMODE_SID, ETHIF_E_PARAM_POINTER));
      #endif
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
    #endif

    if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= TrcvIdx)
    {
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_GETTRCVMODE_SID, ETHIF_E_INV_TRCV_IDX));
      #endif
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
    /* polyspace +10 RTE:IDP [Justified:Low] "This condition will be False when
    transceiver index will be greater than total number of transceivers configured.
    Module Test Case Reference ETHIF_ETC_116, SWS ID SWS_EthIf_00057"
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      /* polyspace +6 RTE:UNR [Justified:Low] "This condition will be True when
        Trcv Index will be Invalid.
        Module Test Case Reference test_38, SWS ID SWS_EthIf_00058"
        Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      /* Get the pointer to the requested Controller's structure */
      LpStaticTransceiver_PB = &EthIf_GaaStaticTransceiver_PB[TrcvIdx];

      if ((uint8)ETHIF_INVALID == LpStaticTransceiver_PB->ucTrcvIdx)
      #else
      /* polyspace +6 RTE:UNR [Justified:Low] "This condition will be True when
        Trcv Index will be Invalid.
        Module Test Case Reference test_38, SWS ID SWS_EthIf_00058"
        Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      if ((uint8)ETHIF_INVALID == EthIf_GpConfigPtr->pTransceiver[TrcvIdx].ucTrcvIdx)
      #endif
      {
        #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
        (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
          ETHIF_GETTRCVMODE_SID, ETHIF_E_INV_TRCV_IDX));
        #endif
        LddStatusReturn = (Std_ReturnType)E_NOT_OK;
      }
    }
  }
  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
  EthIf module is in INITIALIZED state and no DET reported.
  Module Test Case Reference ETHIF_ETC_116, SWS ID SWS_EthIf_00057"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpTrcvApis = LpStaticTransceiver_PB->pTrcvDriver;
    #else
    LpTrcvApis = EthIf_GpConfigPtr->pTransceiver[TrcvIdx].pTrcvDriver;
    #endif
    /* polyspace +7 RTE:UNR [Justified:Low] "This condition will be False when
    LpTrcvApis will be NULL.
    Module Test Case Reference SWS_EthIf_00055h, SWS_EthIf_00056a" */
    /*SWS_EthIf_00055h, SWS_EthIf_00056a
     Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if (LpTrcvApis != NULL_PTR)
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LddStatusReturn = (LpTrcvApis->pEthTrcvGetTransceiverMode)
        (LpStaticTransceiver_PB->ucTrcvIdx, TrcvModePtr);
      #else
      LddStatusReturn = (LpTrcvApis->pEthTrcvGetTransceiverMode)
        (EthIf_GpConfigPtr->pTransceiver[TrcvIdx].ucTrcvIdx, TrcvModePtr);
      #endif
    }
  }
  /* SWS_EthIf_00055f
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_GetPhysAddr                                   **
**                                                                            **
** Service ID           : 0x08                                                **
**                                                                            **
** Description          : This API service of the Ethernet Interface Obtains  **
**                        the physical source address used by the indexed     **
**                        controller.                                         **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx                                             **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : PhysAddrPtr                                         **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstDriverApis        **
**                                                                            **
**                        Function(s) invoked: Eth_GetPhysAddr                **
**                                                                            **
** Design ID            : ETHIF_SDD_0090                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"

/* PRQA S 3227 2 */
void EthIf_GetPhysAddr(uint8 CtrlIdx, uint8 * PhysAddrPtr)
{
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  Std_ReturnType LddStatusReturn;
  LddStatusReturn = (Std_ReturnType)E_OK;

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be true when
  CtrlIdx is invalid.
  Module Test Case Reference ETHIF_ETC_58" */
  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETPHYADDR_SID, ETHIF_E_INV_CTRL_IDX));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_140, SWS ID SWS_EthIf_00063 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETPHYADDR_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  #if ((STD_ON == ETHIF_DEV_ERROR_DETECT) || (STD_ON == ETHIF_SAFE_KSAR))
  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
  PhysAddrPtr is NULL_PTR .
  Module Test Case Reference ETHIF_ETC_033, SWS ID SWS_EthIf_00065 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == PhysAddrPtr)
  {
    #if (STD_ON == ETHIF_SAFE_KSAR)
    (void)(Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETPHYADDR_SID, ETHIF_E_PARAM_POINTER));
    #else
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_GETPHYADDR_SID, ETHIF_E_PARAM_POINTER));
      #endif
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif
  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
  EthIf module is in INITIALIZED state and no DET are present.
  Module Test Case Reference ETHIF_ETC_140, SWS ID SWS_EthIf_00063 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Get the pointer to the requested Controller's structure */
    #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)

    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpDriverApis = EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].pEthDriver;
    #else
    LpDriverApis = (EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->pEthDriver;
    #endif
    /* polyspace +6 RTE:UNR [Justified:Low] "This condition will be False when
    LpDriverApis will be NULL.
    Module Test Case Reference SWS_EthIf_00061f, SWS_EthIf_00062a"
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if (LpDriverApis != NULL_PTR)
    {
      /*SWS_EthIf_00061f, SWS_EthIf_00062a
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      (LpDriverApis->pEthGetPhysAddr)
        (EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].ucCtrlIdx, PhysAddrPtr);
      #else
      (LpDriverApis->pEthGetPhysAddr)
        (((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx), PhysAddrPtr);
      #endif
    }
    #else
    /*SWS_EthIf_00061f, SWS_EthIf_00062a
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    /* polyspace +4 RTE:IDP [Justified:Low] "This Pointer will not be out of
    bound as it will have the controller index configured with respect to
    number of configured controllers  */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    Eth_GetPhysAddr(EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].ucCtrlIdx, PhysAddrPtr);
    #else
      /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
      EthIf_GpConfigPtr is not a NULL PTR.
      Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_00062a"
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    Eth_GetPhysAddr(((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx), PhysAddrPtr);
    #endif
    #endif
  }
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_SetPhysAddr                                   **
**                                                                            **
** Service ID           : 0x0d                                                **
**                                                                            **
** Description          : This API service of the Ethernet Interface sets the **
**                        physical source address used by the indexed         **
**                        controller.                                         **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant for the same CtrlIdx,                 **
**                        reentrant for different                             **
**                                                                            **
** Input Parameters     : CtrlIdx,PhysAddrPtr                                 **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstDriverApis        **
**                                                                            **
**                        Function(s) invoked:  Eth_SetPhysAddr               **
**                                                                            **
** Design ID            : ETHIF_SDD_0092                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 2 */
void EthIf_SetPhysAddr(uint8 CtrlIdx, const uint8 * PhysAddrPtr)
{
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  Std_ReturnType LddStatusReturn;
  LddStatusReturn = (Std_ReturnType)E_OK;

  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_SETPHYADDR_SID, ETHIF_E_INV_CTRL_IDX));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_142, SWS ID SWS_EthIf_00135 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_SETPHYADDR_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  #if ((STD_ON == ETHIF_DEV_ERROR_DETECT) || (STD_ON == ETHIF_SAFE_KSAR))
  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
  PhysAddrPtr is NULL_PTR.
  Module Test Case Reference ETHIF_ETC_034, SWS ID SWS_EthIf_00137 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == PhysAddrPtr)
  {
    #if (STD_ON == ETHIF_SAFE_KSAR)
    (void)(Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_SETPHYADDR_SID, ETHIF_E_PARAM_POINTER));
    #else
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_SETPHYADDR_SID, ETHIF_E_PARAM_POINTER));
      #endif
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif
  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
  EthIf module is in INITIALIZED state and no DET errors are present.
  Module Test Case Reference ETHIF_ETC_142, SWS ID SWS_EthIf_00135 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Get the pointer to the requested Controller's structure */
    #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpDriverApis = EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].pEthDriver;
    #else
    LpDriverApis = (EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->pEthDriver;
    #endif
    /* polyspace +6 RTE:UNR [Justified:Low] "This condition will be False when
    LpDriverApis will be NULL.
    Module Test Case Reference SWS_EthIf_00132f, SWS_EthIf_00134a"
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if (LpDriverApis != NULL_PTR)
    {
      /* SWS_EthIf_00132f, SWS_EthIf_00134a
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      (LpDriverApis->pEthSetPhysAddr)
        (EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].ucCtrlIdx, PhysAddrPtr);
      #else
      (LpDriverApis->pEthSetPhysAddr)
        (((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx), PhysAddrPtr);
      #endif
    }
    #else
    /* SWS_EthIf_00132f, SWS_EthIf_00134a
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    /* polyspace +4 RTE:IDP [Justified:Low] "This Pointer will not be out of
    bound as it will have the controller index configured with respect to
    number of configured controllers  */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    Eth_SetPhysAddr(EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].ucCtrlIdx, PhysAddrPtr);
    #else
        /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
    EthIf_GpConfigPtr is not a NULL PTR.
    Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_000134a"
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    Eth_SetPhysAddr(((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx), PhysAddrPtr);
    #endif
    #endif
  }
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_UpdatePhysAddrFilter                          **
**                                                                            **
** Service ID           : 0x0c                                                **
**                                                                            **
** Description          : This API service of the Ethernet Interface updates  **
**                        the physical source address to/from the             **
**                        indexed controller filter.                          **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant for the same CtrlIdx,                 **
**                        reentrant for different                             **
**                                                                            **
** Input Parameters     : CtrlIdx, PhysAddrPtr, Action                        **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstDriverApis        **
**                                                                            **
**                        Function(s) invoked: Eth_UpdatePhysAddrFilter       **
**                                                                            **
** Design ID            : ETHIF_SDD_0093                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 3 */
Std_ReturnType EthIf_UpdatePhysAddrFilter(
  uint8 CtrlIdx, const uint8 * PhysAddrPtr, Eth_FilterActionType Action)
{
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  Std_ReturnType LddStatusReturn;
  LddStatusReturn = (Std_ReturnType)E_OK;

  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_UPDATEPHYSADDRFILTER_SID, ETHIF_E_INV_CTRL_IDX));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_143, SWS ID SWS_EthIf_00141 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_UPDATEPHYSADDRFILTER_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  #if ((STD_ON == ETHIF_DEV_ERROR_DETECT) || (STD_ON == ETHIF_SAFE_KSAR))
  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
   PhysAddrPtr is NULL_PTR .
  Module Test Case Reference ETHIF_ETC_034, SWS ID SWS_EthIf_00143 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == PhysAddrPtr)
  {
    #if (STD_ON == ETHIF_SAFE_KSAR)
    (void)(Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_UPDATEPHYSADDRFILTER_SID, ETHIF_E_PARAM_POINTER));
    #else
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_UPDATEPHYSADDRFILTER_SID, ETHIF_E_PARAM_POINTER));
      #endif
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif
  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
  EthIf module is in INITIALIZED state and No DET errors are present.
  Module Test Case Reference ETHIF_ETC_143, SWS ID SWS_EthIf_00141 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Get the pointer to the requested Controller's structure */
    #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpDriverApis = EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].pEthDriver;
    #else
    LpDriverApis = (EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->pEthDriver;
    #endif
    /* polyspace +7 RTE:UNR [Justified:Low] "This condition will be False when
    LpDriverApis will be NULL.
    Module Test Case Reference SWS_EthIf_00139h, SWS_EthIf_00140a"
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if (LpDriverApis != NULL_PTR)
    {
      /*SWS_EthIf_00139h, SWS_EthIf_00140a
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LddStatusReturn = (LpDriverApis->pEthUpdatePhysAddrFilter)
        (EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].ucCtrlIdx, PhysAddrPtr, Action);
      #else
      LddStatusReturn = (LpDriverApis->pEthUpdatePhysAddrFilter)
        (((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx), PhysAddrPtr, Action);
      #endif
    }
    #else
    /*SWS_EthIf_00139h, SWS_EthIf_00140a
     Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    /* polyspace +4 RTE:IDP [Justified:Low] "This Pointer will not be out of
    bound as it will have the controller index configured with respect to
    number of configured controllers  */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LddStatusReturn = Eth_UpdatePhysAddrFilter(EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].ucCtrlIdx,
    PhysAddrPtr, Action);
    #else
        /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
     EthIf_GpConfigPtr is not a NULL PTR.
     Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_000140a"
     Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    LddStatusReturn = Eth_UpdatePhysAddrFilter(((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx),
         PhysAddrPtr, Action);
    #endif
    #endif
  }
  /* SWS_EthIf_00139f
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_ProvideTxBuffer_DetCheck                      **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : Checks DET errors in  EthIf_ProvideTxBuffer         **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx,BufIdxPtr, BufPtr, LenBytePtr               **
**                                                                            **
** InOut parameter      : NA                                                  **
**                                                                            **
** Output Parameters    : NA                                                  **
**                                                                            **
** Return parameter     : BufReq_ReturnType                                   **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstDriverApis        **
**                                                                            **
**                        Function(s) invoked:                                **
**                                   External: Eth_ProvideTxBuffer            **
**                                   Internal: None                           **
**                                                                            **
** Design ID            : ETHIF_SDD_0800                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 2 */
static BufReq_ReturnType EthIf_ProvideTxBuffer_DetCheck(
  uint8 CtrlIdx, const Eth_BufIdxType * BufIdxPtr, const uint16 * LenBytePtr)
{
  BufReq_ReturnType LenBufReturn;

  LenBufReturn = BUFREQ_OK;

  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_PROVIDETXBUF_SID, ETHIF_E_INV_CTRL_IDX));
    #endif
    #if (STD_ON == ETHIF_SECURITY_EVENT_ENABLED)
      /* SWS_EthIf_00503:EthIf module won't provide Context data and TimeStamp
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
     IdsM_ReportSecurityEvent(ETHIF_SEV_ETH_DROP_INV_VLAN, NULL_PTR,
      ETHIF_DEFAULT_CONTEXT_DATA_SIZE, ETHIF_DEFAULT_CONTEXT_DATA_VERSION,
      ETHIF_DEFAULT_COUNT, NULL_PTR);
    #endif
    LenBufReturn = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
  }
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_166, SWS ID SWS_EthIf_00074 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_PROVIDETXBUF_SID, ETHIF_E_UNINIT));
    #endif
    LenBufReturn = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
  }
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be True when
  BufIdxPtr is NULL_PTR .
  Module Test Case Reference ETHIF_ETC_045, SWS ID SWS_EthIf_00071 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == BufIdxPtr)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_PROVIDETXBUF_SID, ETHIF_E_PARAM_POINTER));
    #endif
    LenBufReturn = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
  }
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be True when
  LenBytePtr is NULL_PTR .
  Module Test Case Reference ETHIF_ETC_046, SWS ID SWS_EthIf_00073 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == LenBytePtr)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_PROVIDETXBUF_SID, ETHIF_E_PARAM_POINTER));
    #endif
    LenBufReturn = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
  }
  return(LenBufReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_ProvideTxBuffer                               **
**                                                                            **
** Service ID           : 0x09                                                **
**                                                                            **
** Description          : This API service of the Ethernet Interface Provides **
**                        access to a transmit buffer of the specified        **
**                        Ethernet controller                                 **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx,FrameType,Priority                          **
**                                                                            **
** InOut parameter      : LenBytePtr                                          **
**                                                                            **
** Output Parameters    : BufIdxPtr,BufPtr                                    **
**                                                                            **
** Return parameter     : BufReq_ReturnType                                   **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstDriverApis        **
**                                                                            **
**                        Function(s) invoked:                                **
**                                   External: Eth_ProvideTxBuffer            **
**                                   Internal: None                           **
**                                                                            **
** Design ID            : ETHIF_SDD_0117                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 6 */
BufReq_ReturnType EthIf_ProvideTxBuffer(
  uint8 CtrlIdx, Eth_FrameType FrameType, uint8 Priority,
  Eth_BufIdxType * BufIdxPtr, uint8 ** BufPtr, uint16 * LenBytePtr)
{
  const Eth_BufIdxType*  LpBufIdxPtr;
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController* LpStaticController;
  #endif
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController_PB* LpStaticController_PB;
  #endif
  #if (STD_ON == ETHIF_VLAN_SUPPORT)
  uint16 LusVlanFlag;
  uint16 LusVlanId;
  #endif
  #if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
  EthIf_ControllerMacsecContextType * LpCtrlCtx;
  EthIf_MacsecSaContextType *LpTxSa;
  uint8 LucTxStateOffset;
  uint16 LusSecureDataLen;
  boolean LblMacSecEn;
  #endif
  uint8 LusFramePriority;
  BufReq_ReturnType LenBufReturn;
  LpBufIdxPtr = BufIdxPtr;

  LenBufReturn = EthIf_ProvideTxBuffer_DetCheck(CtrlIdx, LpBufIdxPtr,
    LenBytePtr);

  #if ((STD_ON == ETHIF_DEV_ERROR_DETECT) || (STD_ON == ETHIF_SAFE_KSAR))
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be True when
     BufPtr is NULL_PTR .
     Module Test Case Reference ETHIF_ETC_046, SWS ID SWS_EthIf_00072 "
     Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == BufPtr)
  {
    #if (STD_ON == ETHIF_SAFE_KSAR)
    (void)(Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_PROVIDETXBUF_SID, ETHIF_E_PARAM_POINTER));
    #else
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_PROVIDETXBUF_SID, ETHIF_E_PARAM_POINTER));
    #endif
    #endif
    LenBufReturn = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
  }
  #endif
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be True when
     EthIf module is in INITIALIZED state and No DET errors.
     Module Test Case Reference ETHIF_ETC_166, SWS ID SWS_EthIf_00067 "
     Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((BufReq_ReturnType)BUFREQ_OK == LenBufReturn)
  {
    /* Get the pointer to the requested Controller's structure */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpStaticController = &EthIf_GaaStaticController[CtrlIdx];
    #endif
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpStaticController_PB = &EthIf_GaaStaticController_PB[CtrlIdx];
    #endif

    #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpDriverApis = EthIf_GaaStaticPhyController_PB[(LpStaticController_PB->pPhysCtrl)->ucPhysCtrlIdx].pEthDriver;
    #else
    LpDriverApis = (EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->pEthDriver;
    #endif
    /* polyspace +7 RTE:UNR [Justified:Low] "This condition will be False when
    LpDriverApis will be NULL.
    Reference SWS ID SWS_EthIf_00067"
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if (LpDriverApis != NULL_PTR)
    #endif
    {
      #if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
      LusSecureDataLen = *LenBytePtr;
      #endif
      #if (STD_ON == ETHIF_VLAN_SUPPORT)
      /* Calculate the index to fetch the vlan index value */
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LusVlanId = LpStaticController_PB->usEthIfVlanTagLsb;
      #else
      /* polyspace +10 RTE:UNR [Justified:Low] "This condition will be True when
         EthIf_GpConfigPtr is not a NULL PTR.
         Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_00067"
         Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      LusVlanId = EthIf_GpConfigPtr->pController[CtrlIdx].ucVlanId;
      #endif

      /* check the corresponding Controller VLAN status */
      if (LusVlanId != (uint16)ETHIF_ZERO)
      {
        LusVlanFlag = (uint16)(LusVlanId >> ETHIF_TWELVE);

        LusVlanFlag = LusVlanFlag & (uint16)ETHIF_EIGHT;

        LusVlanId = LusVlanId & (uint16)ETHIF_VLANTAG_MASK;

        LusVlanId = (LusVlanId |
          (uint16)(((uint16)Priority & (uint16)ETHIF_SEVEN) << ETHIF_THIRTEEN));

        /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False
           when LusVlanId is Zero. */
        if (LusVlanFlag == (uint16)ETHIF_EIGHT)
        {
          /* Increase the size of the buffer required by 4 bytes for vlan tag */
          *LenBytePtr = *LenBytePtr + (uint16)ETHIF_FOUR ;
        }
        LusFramePriority = Priority;
      }
      else
      {
        LusFramePriority = ETHIF_ZERO;
        LusVlanFlag = ETHIF_ZERO;
      }
      #else
      /* SWS_EthIf_00146
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      /*Violates MISRA 2012 Required Rule 2.2, #ref EthIf_c_QAC_REF_2 */

      ETHIF_UNUSED(Priority);
      LusFramePriority = ETHIF_ZERO;
      #endif

      #if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
      LblMacSecEn = ETHIF_FALSE;
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      if (ETHIF_SW_MACSEC == LpStaticController_PB->usmacsec)
      #else
      if (ETHIF_SW_MACSEC == EthIf_GpConfigPtr->pController[CtrlIdx].usmacsec)
      #endif
      {
        LpCtrlCtx =
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        &EthIf_GaaMacsecController[LpStaticController_PB->ucSWMacSecIndex];
        #else
        &EthIf_GaaMacsecController[
          EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex];
        #endif
        if (ETHIF_TRUE == LpCtrlCtx->IsMacSecOperational)
        {
          LpTxSa = &LpCtrlCtx->TxSa[LpCtrlCtx->ActiveTxAnIndex];
          if ((ETHIF_TRUE == LpCtrlCtx->IsProtectFramesEnabled) &&
              (ETHIF_TRUE == LpTxSa->Active))
          {
            LblMacSecEn = ETHIF_TRUE;
            /* Add the SecTAG and EtherType length */
            *LenBytePtr += ETHIF_MACSEC_SECTAG_LENGTH;
            /* Add the ICV length */
            *LenBytePtr += ETHIF_MACSEC_ICV_LENGTH;
          }
        }
      }
      #endif

      #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
      /*SWS_EthIf_00067i, SWS_EthIf_00068a
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LenBufReturn = (LpDriverApis->pEthProvideTxBuffer)
        (EthIf_GaaStaticPhyController_PB[(LpStaticController_PB->pPhysCtrl)->ucPhysCtrlIdx].ucCtrlIdx, LusFramePriority, BufIdxPtr, BufPtr,  LenBytePtr);
      #else
      LenBufReturn = (LpDriverApis->pEthProvideTxBuffer)
        (((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx), LusFramePriority, BufIdxPtr, BufPtr,  LenBytePtr);
      #endif
      #else
      /*SWS_EthIf_00067i, SWS_EthIf_00068a
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      /* polyspace +4 RTE:IDP [Justified:Low] "This Pointer will not be out of
         bound as it will have the controller index configured with respect to
         number of configured controllers */
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LenBufReturn = Eth_ProvideTxBuffer(EthIf_GaaStaticPhyController_PB[(LpStaticController_PB->pPhysCtrl)->ucPhysCtrlIdx].ucCtrlIdx, LusFramePriority, BufIdxPtr, BufPtr, LenBytePtr);
      #else
      /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
         EthIf_GpConfigPtr is not a NULL PTR.
         Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_00068a"
         Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      LenBufReturn = Eth_ProvideTxBuffer(((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx), LusFramePriority, BufIdxPtr, BufPtr, LenBytePtr);
      #endif
      #endif

      /*SWS_EthIf_00067f, SWS_EthIf_00067h, SWS_EthIf_00147
        Once EA tool is ready, SWS_ID will be updated with Design_ID*/

      /* Check if max transmit count has reached */
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      if (EthIf_GSaaController[CtrlIdx].ucProvideTxBufferCount <
          LpStaticController->ucEthIfMaxTxBufsTotal)
      #else
      if (EthIf_GSaaController[CtrlIdx].ucProvideTxBufferCount <
          EthIf_GpConfigPtr->pController[CtrlIdx].ucEthIfMaxTxBufsTotal)
      #endif
      {
        /* SW_EthIf_00081 */
        SchM_Enter_EthIf_MAIN_PERIOD_PROTECT();
        EthIf_GSaaController[CtrlIdx].ucProvideTxBufferCount++;
        SchM_Exit_EthIf_MAIN_PERIOD_PROTECT();
      }

      #if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
      if ((BUFREQ_OK == LenBufReturn) &&
          (ETHIF_TRUE == LblMacSecEn))
      {
        #if (STD_ON == ETHIF_VLAN_SUPPORT)
        if (LusVlanFlag == (uint16)ETHIF_EIGHT)
        {
          LusSecureDataLen += ETHIF_FOUR;
        }
        #endif
        /* Add EtherType length */
        LusSecureDataLen += ETHIF_TWO;
        /* Remove the SecTAG and EtherType length */
        *LenBytePtr -= ETHIF_MACSEC_SECTAG_LENGTH;

        EthIf_GenerateSecTAG(LpCtrlCtx, LpTxSa, BufPtr, LusSecureDataLen);

        LucTxStateOffset =
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
          LpStaticController_PB->ucSWMacSecIndex;
        #else
          EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex;
        #endif
        /* Remove the ICV length */
        *LenBytePtr -= ETHIF_MACSEC_ICV_LENGTH;
        #if (STD_ON == ETHIF_VLAN_SUPPORT)
        if (LusVlanFlag == (uint16)ETHIF_EIGHT)
        {
          EthIf_GSaaTxState[LucTxStateOffset].pDataBufferPtr =
            &((*BufPtr)[ETHIF_FOUR]);
          /* Populate Requested EtherType */
          **BufPtr = (uint8)(ETHIF_VLAN_FRAMEID >> ETHIF_EIGHT);
          *(*BufPtr + ETHIF_ONE) = (uint8)ETHIF_VLAN_FRAMEID;
        }
        else
        #endif
        {
          EthIf_GSaaTxState[LucTxStateOffset].pDataBufferPtr = *BufPtr;
        }
        EthIf_GSaaTxState[LucTxStateOffset].ucSecTagLen = ETHIF_SIX;
        /* Leaving two bytes for the requested Ethernet Type */
        *BufPtr = &((*BufPtr)[ETHIF_TWO]);
      }
      #endif

      #if (STD_ON == ETHIF_VLAN_SUPPORT)
      if ((BUFREQ_OK == LenBufReturn) && (LusVlanFlag == (uint16)ETHIF_EIGHT))
      {
        /* To identify it is a VLAN frame */
        **BufPtr = (uint8)((LusVlanId & ETHIF_FRAME_MSB_MASK) >> ETHIF_EIGHT);
        *(*BufPtr + ETHIF_ONE) =  (uint8)(LusVlanId & (uint16)ETHIF_FRAME_LSB_MASK);
        *(*BufPtr + ETHIF_TWO) = (uint8)((FrameType & ETHIF_FRAME_MSB_MASK) >> ETHIF_EIGHT);
        *(*BufPtr + ETHIF_THREE) = (uint8)(FrameType & (Eth_FrameType)ETHIF_FRAME_LSB_MASK);
        *BufPtr = &((*BufPtr)[ETHIF_FOUR]);

        /* Increment the double pointer */
        *LenBytePtr = *LenBytePtr - (uint16)ETHIF_FOUR;
      }
      #else
      /* SWS_EthIf_00146
         Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      /*Violates MISRA 2012 Required Rule 2.2, #ref EthIf_c_QAC_REF_2 */

      ETHIF_UNUSED(FrameType);
      #endif
    }
  } /* End of if ((Std_ReturnType)E_OK == LenBufReturn) */
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  ETHIF_UNUSED(LpStaticController_PB);
  #endif

  return(LenBufReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_Transmit                                      **
**                                                                            **
** Service ID           : 0x0A                                                **
**                                                                            **
** Description          : This API service of the Ethernet Interface Provides **
**                        access to a transmit buffer of the specified        **
**                        Ethernet controller                                 **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx,FrameType,TxConfirmation,PhysAddrPtr        **
**                                                                            **
** InOut parameter      : LenByte                                             **
**                                                                            **
** Output Parameters    : BufIdx                                              **
**                                                                            **
** Return parameter     : BufReq_ReturnType                                   **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstDriverApis        **
**                                                                            **
**                        Function(s) invoked:                                **
**                                   External: Eth_Transmit                   **
**                                   Internal: None                           **
**                                                                            **
** Design ID             : ETHIF_SDD_0118                                     **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 5 */
Std_ReturnType EthIf_Transmit(
  uint8 CtrlIdx, Eth_BufIdxType BufIdx, Eth_FrameType FrameType,
  boolean TxConfirmation, uint16 LenByte, const uint8 * PhysAddrPtr)
{
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  Std_ReturnType LddStatusReturn;
  Eth_FrameType LddFrameType;
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController_PB* LpStaticController_PB;
  #endif
  #if (STD_ON == ETHIF_VLAN_SUPPORT)
  uint16 LusVlanId;
  uint8 LusVlanFlag;
  #endif
  #if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
  uint8 LucTxStateOffset;
  uint8 * LpBuffPtr;
  #endif
  uint16 LucByte;
  LucByte = (uint16)LenByte;
  LddStatusReturn = (Std_ReturnType)E_OK;

  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_TRANSMIT_SID, ETHIF_E_INV_CTRL_IDX));
    #endif
    #if (STD_ON == ETHIF_SECURITY_EVENT_ENABLED)
    /* SWS_EthIf_00503:EthIf module won't provide Context data and TimeStamp
       Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    IdsM_ReportSecurityEvent(ETHIF_SEV_ETH_DROP_INV_VLAN, NULL_PTR,
      ETHIF_DEFAULT_CONTEXT_DATA_SIZE, ETHIF_DEFAULT_CONTEXT_DATA_VERSION,
      ETHIF_DEFAULT_COUNT, NULL_PTR);
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
     EthIf module is in INITIALIZED state .
     Module Test Case Reference ETHIF_ETC_168, SWS ID SWS_EthIf_00077 "
     Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_TRANSMIT_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  if ((Eth_BufIdxType)ETHIF_INVALID_32 <= BufIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_TRANSMIT_SID, ETHIF_E_INV_PARAM));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  #if ((STD_ON == ETHIF_DEV_ERROR_DETECT) || (STD_ON == ETHIF_SAFE_KSAR))
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be True when
     PhysAddrPtr is NULL_PTR .
     Module Test Case Reference ETHIF_ETC_048, SWS ID SWS_EthIf_00080 "
     Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == PhysAddrPtr)
  {
    #if (STD_ON == ETHIF_SAFE_KSAR)
    (void)(Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_TRANSMIT_SID, ETHIF_E_PARAM_POINTER));
    #else
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_TRANSMIT_SID, ETHIF_E_PARAM_POINTER));
    #endif
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be True when
     EthIf module is in INITIALIZED state and there are no DET errors.
     Module Test Case Reference ETHIF_ETC_168, SWS ID SWS_EthIf_00075 "
     Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Get the pointer to the requested Controller's structure */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpStaticController_PB = &EthIf_GaaStaticController_PB[CtrlIdx];
    #endif

    #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpDriverApis = EthIf_GaaStaticPhyController_PB[
    (LpStaticController_PB->pPhysCtrl)->ucPhysCtrlIdx].pEthDriver;
    #else
    LpDriverApis = (EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->pEthDriver;
    #endif
    /* polyspace +7 RTE:UNR [Justified:Low] "This condition will be False when
       LpDriverApis will be NULL.
       Reference SWS ID SWS_EthIf_00075h, SWS_EthIf_00081 "
       Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if (LpDriverApis != NULL_PTR)
    #endif
    {
      /* SWS_EthIf_00075h, SWS_EthIf_00081
         Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      if ((uint8)ETHIF_ZERO !=
        EthIf_GSaaController[CtrlIdx].ucProvideTxBufferCount)
      {
        #if (STD_ON == ETHIF_VLAN_SUPPORT)
        /* Calculate the index to fetch the vlan index value */
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        LusVlanId = LpStaticController_PB->usEthIfVlanTagLsb;
        #else
        LusVlanId = EthIf_GpConfigPtr->pController[CtrlIdx].ucVlanId;
        #endif

        LusVlanFlag = (uint8)(LusVlanId >> ETHIF_TWELVE);
        LusVlanFlag = LusVlanFlag & (uint8)ETHIF_EIGHT;

        /* SWS_EthIf_00148
           Once EA tool is ready, SWS_ID will be updated with Design_ID*/
        if (LusVlanFlag == (uint8)ETHIF_EIGHT)
        {
          LddFrameType = (Eth_FrameType)ETHIF_VLAN_FRAMEID;
        }
        /* polyspace +3 RTE:UNR [Justified:Low] "This condition will be False
           when VLAN Tag value is not same as expected. */
        else
        {
          LddFrameType = FrameType;
        }
        #else
        LddFrameType = FrameType;
        #endif

        #if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        if (ETHIF_SW_MACSEC == LpStaticController_PB->usmacsec)
        #else
        if (ETHIF_SW_MACSEC == EthIf_GpConfigPtr->pController[CtrlIdx].usmacsec)
        #endif
        {
          LucTxStateOffset =
          #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
            LpStaticController_PB->ucSWMacSecIndex;
          #else
            EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex;
          #endif
          /* Populate Requested EtherType */
          LpBuffPtr = EthIf_GSaaTxState[LucTxStateOffset].pDataBufferPtr;

          if (NULL_PTR != LpBuffPtr)
          {
            /* Add the Ethere Type only if VLAN tag is not enabled */
            #if (STD_ON == ETHIF_VLAN_SUPPORT)
            if (!(LddFrameType == ETHIF_VLAN_FRAMEID))
            #endif
            {
              *LpBuffPtr = (uint8)(LddFrameType >> ETHIF_EIGHT);
              *(LpBuffPtr + ETHIF_ONE) = (uint8)LddFrameType;
            }
            /* Add the length of Ethere Type */
            LucByte += ETHIF_TWO;
            /* Update the EtherType to MacSec EtherType */
            LddFrameType = ETHIF_MACSECETHERTYPE;
            if (E_OK == EthIf_GenerateICV(CtrlIdx, LpBuffPtr, LucByte))
            {
              /* Add the length of ICV */
              LucByte += ETHIF_MACSEC_ICV_LENGTH;
            }
          }

          /* Add the length of Sectag */
          LucByte += EthIf_GSaaTxState[LucTxStateOffset].ucSecTagLen;

          /* Reset Tx forwarded State */
          EthIf_GSaaTxState[LucTxStateOffset].pDataBufferPtr = NULL_PTR;
          EthIf_GSaaTxState[LucTxStateOffset].ucSecTagLen = ETHIF_ZERO;
        }
        #endif

        #if (STD_ON == ETHIF_VLAN_SUPPORT)
        /* Calculate the index to fetch the vlan index value */
        if (LusVlanFlag == (uint8)ETHIF_EIGHT)
        {
          LucByte += (uint16)ETHIF_FOUR;
        }
        #endif

        #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
        /* SWS_EthIf_00076a
           Once EA tool is ready, SWS_ID will be updated with Design_ID*/
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        LddStatusReturn = (LpDriverApis->pEthTransmit)
          (EthIf_GaaStaticPhyController_PB[(LpStaticController_PB->pPhysCtrl)->ucPhysCtrlIdx].ucCtrlIdx, BufIdx, LddFrameType,
            TxConfirmation, LucByte, PhysAddrPtr);
        #else
        LddStatusReturn = (LpDriverApis->pEthTransmit)
          (((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx), BufIdx, LddFrameType,
            TxConfirmation, LucByte, PhysAddrPtr);
        #endif
        #else
        /* SWS_EthIf_00076a
           Once EA tool is ready, SWS_ID will be updated with Design_ID*/
        /* polyspace +4 RTE:IDP [Justified:Low] "This Pointer will not be out of
           bound as it will have the controller index configured with respect to
           number of configured controllers */
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        LddStatusReturn = Eth_Transmit(EthIf_GaaStaticPhyController_PB[(LpStaticController_PB->pPhysCtrl)->ucPhysCtrlIdx].ucCtrlIdx,
          BufIdx,  LddFrameType, TxConfirmation, LucByte, PhysAddrPtr);
        #else
        LddStatusReturn = Eth_Transmit(((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx), BufIdx,
          LddFrameType, TxConfirmation, LucByte, PhysAddrPtr);
        #endif
        #endif
        /* SWS_EthIf_00081
           Once EA tool is ready, SWS_ID will be updated with Design_ID*/
        SchM_Enter_EthIf_MAIN_PERIOD_PROTECT();
        EthIf_GSaaController[CtrlIdx].ucProvideTxBufferCount--;
        SchM_Exit_EthIf_MAIN_PERIOD_PROTECT();

        #if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
        EthIf_MacSecUpdateTxStats(CtrlIdx, LenByte, LucByte);
        #endif
      }
      else
      {
        LddStatusReturn = (Std_ReturnType)E_NOT_OK;
      }
    }
  }  /* End of if ((Std_ReturnType)E_OK == LddStatusReturn) */
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  ETHIF_UNUSED(LpStaticController_PB);
  #endif

  /* SWS_EthIf_00075f
     Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_TransmitInputValidation                       **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : This API service of the Ethernet Interface          **
**                        validates input data                                **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different PduIds. Non reentrant for   **
**                        the same PduId.                                     **
**                                                                            **
** Input Parameters     : TxPduId, PduInfoPtr                                 **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GddInit,                 **
**                                                                            **
**                        Function(s) invoked:                                **
**                                   External: Det_ReportError,               **
**                                   Internal: None                           **
**                                                                            **
** Design ID            : ETHIF_SDD_0733                                      **
*******************************************************************************/
#if (ETHIF_NO_OF_TXPDUS > ETHIF_ZERO)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static Std_ReturnType EthIf_TransmitInputValidation(
  PduIdType TxPduId, const PduInfoType * PduInfoPtr)
{
  Std_ReturnType LddReturnValue  = (Std_ReturnType)E_OK;
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  (void) Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
    ETHIF_TRANSMIT_SID, ETHIF_E_UNINIT);
  #endif
  LddReturnValue = (Std_ReturnType)E_NOT_OK;
  }
  /* Validate input parameter TxPduId */
  if ((uint16)TxPduId >= ETHIF_NO_OF_TXPDUS)
  {
  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  (void) Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
    ETHIF_TRANSMIT_SID, ETHIF_E_INV_PARAM);
  #endif
  LddReturnValue = (Std_ReturnType)E_NOT_OK;
  }
  if (NULL_PTR == PduInfoPtr)
  {
  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  (void) Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
    ETHIF_TRANSMIT_SID, ETHIF_E_PARAM_POINTER);
  #endif
  LddReturnValue = (Std_ReturnType)E_NOT_OK;
  }
  return LddReturnValue;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_TransmitVLANSupport                           **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : VLAN Support check for Transmission                 **
**                                                                            **
** Sync/Async           : NA                                                  **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : LucCtrlIdx, LucPriority, LddFrameType               **
**                                                                            **
** Output Parameters    : VlanInfoPtr                                         **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s): None                            **
**                                                                            **
** Function(s) invoked  : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0734                                      **
*******************************************************************************/
#if (ETHIF_NO_OF_TXPDUS > ETHIF_ZERO) && (STD_ON == ETHIF_VLAN_SUPPORT)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static Std_ReturnType EthIf_TransmitVLANSupport(
  uint8 * VlanInfoPtr, uint8 LucCtrlIdx, uint8 LucPriority,
  const Eth_FrameType LddFrameType)
{
  Std_ReturnType LddReturnValue  = (Std_ReturnType)E_OK;
  uint16 LusVlanFlag;
  uint16 LusVlanId;

  if (VlanInfoPtr != NULL)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LusVlanId = EthIf_GaaStaticController_PB[LucCtrlIdx].usEthIfVlanTagLsb;
    #else
    LusVlanId = EthIf_GpConfigPtr->pController[LucCtrlIdx].ucVlanId;
    #endif

    if (LusVlanId != (uint16)ETHIF_ZERO)
    {
      /* Fetching CFI bit(bit 12) */
      LusVlanFlag = (uint16)(LusVlanId >> ETHIF_TWELVE);
      LusVlanFlag = LusVlanFlag & (uint16)ETHIF_EIGHT;
      LusVlanId = LusVlanId & (uint16)ETHIF_VLANTAG_MASK;
      /* Inserting priority into VLAN tag (bit 13-15) */
      LusVlanId =  LusVlanId | (uint16)(((uint16)(LucPriority &
      (uint16)ETHIF_SEVEN)) << ETHIF_THIRTEEN);

      /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be
        False when LusVlanId is Zero. */
      if (LusVlanFlag == (uint16)ETHIF_EIGHT)
      {
        /* To identify it is a VLAN frame */
        /* Write VLAN tag MSB, advance pointer */
        VlanInfoPtr[ETHIF_ZERO] = (uint8)((LusVlanId & ETHIF_FRAME_MSB_MASK) >>
          ETHIF_EIGHT);
        /* Write VLAN tag LSB, advance pointer */
        VlanInfoPtr[ETHIF_ONE] = (uint8)(LusVlanId & (uint16)ETHIF_FRAME_LSB_MASK);
        /* Write FrameType MSB, advance pointer */
        VlanInfoPtr[ETHIF_TWO] = (uint8)((LddFrameType & ETHIF_FRAME_MSB_MASK) >>
          ETHIF_EIGHT);
        /* Write FrameType LSB, advance pointer */
        VlanInfoPtr[ETHIF_THREE] = (uint8)(LddFrameType &
          (Eth_FrameType)ETHIF_FRAME_LSB_MASK);
      }
      else
      {
        LddReturnValue = (Std_ReturnType)E_NOT_OK;
      }
    }
    else
    {
      LddReturnValue = (Std_ReturnType)E_NOT_OK;
    }
  }
  return LddReturnValue;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_GetListElemLength                             **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : Get the total length of the List Elements           **
**                                                                            **
** Sync/Async           : NA                                                  **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : ListElemAddressPtr                                  **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : uint32                                              **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s): None                            **
**                                                                            **
** Function(s) invoked  : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0735                                      **
*******************************************************************************/
#if ((ETHIF_NO_OF_TXPDUS > ETHIF_ZERO) && \
     (ETHIF_METADATA_SUPPORT == STD_ON) && \
     (ETHIF_NO_OF_TXMETADATAITEMS > ETHIF_ZERO))
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static uint32 EthIf_GetListElemLength(ListElemStructType * ListElemAddressPtr)
{
  uint32 LulTotalHeaderLen = ETHIF_ZERO;

  if (ListElemAddressPtr != NULL_PTR)
  {
    do
    {
      if ((ListElemAddressPtr->DataPtr != NULL_PTR) &&
        (ListElemAddressPtr->DataLength > (uint16)ETHIF_ZERO))
      {
        LulTotalHeaderLen += ListElemAddressPtr->DataLength;
      }
      ListElemAddressPtr = ListElemAddressPtr->NextListElemPtr;
    } while (ListElemAddressPtr != NULL_PTR);
  }
  return LulTotalHeaderLen;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_ListElemTransmitSupport                       **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : Get the total length of the List Elements           **
**                                                                            **
** Sync/Async           : NA                                                  **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : ListElemAddressPtr                                  **
**                                                                            **
** Output Parameters    : DataBufPtr                                          **
**                                                                            **
** Return parameter     : uint32                                              **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s): None                            **
**                                                                            **
** Function(s) invoked  : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0736                                      **
*******************************************************************************/
#if ((ETHIF_NO_OF_TXPDUS > ETHIF_ZERO) && \
     (ETHIF_METADATA_SUPPORT == STD_ON) && \
     (ETHIF_NO_OF_TXMETADATAITEMS > ETHIF_ZERO))
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static uint32 EthIf_ListElemTransmitSupport(
  uint8 * DataBufPtr, ListElemStructType * ListElemAddressPtr)
{
  uint32 LulBufIndex = ETHIF_ZERO;

  if ((ListElemAddressPtr != NULL_PTR) && (DataBufPtr != NULL_PTR))
  {
    do
    {
      if ((ListElemAddressPtr->DataPtr != NULL_PTR) &&
        (ListElemAddressPtr->DataLength > (uint16)ETHIF_ZERO))
      {
        ETHIF_MEMCPY(DataBufPtr, ListElemAddressPtr->DataPtr,
          ListElemAddressPtr->DataLength);
        DataBufPtr += ListElemAddressPtr->DataLength;
        LulBufIndex += ListElemAddressPtr->DataLength;
      }
      ListElemAddressPtr = ListElemAddressPtr->NextListElemPtr;

    } while (ListElemAddressPtr != NULL_PTR);
  }
  return LulBufIndex;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

#ifdef ETHIF_IMMEDIATE_TRANSMIT_SUPPORT
/*******************************************************************************
** Function Name        : EthIf_CreateHeaderList                              **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : This function creates Etherent Header for           **
**                        HardwareDataTransfer                                **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non-Reentrant                                       **
**                                                                            **
** Input Parameters     : DestMacAddress, SrcMacAddress, LucVlanInfo,         **
**                        FrameType                                           **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : uint8 *                                             **
**                                                                            **
** Preconditions        : EthIf_Init should be invoked.                       **
**                                                                            **
** Remarks              : Global Variable(s): None                            **
**                                                                            **
**                        Function(s) invoked:                                **
**                                   External: Bsw_Memcpy                     **
**                                   Internal: None                           **
**                                                                            **
** Design ID            : ETHIF_SDD_0737                                      **
*******************************************************************************/
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static uint8 * EthIf_CreateHeaderList(
  uint8 * DestMacAddress, uint8 * SrcMacAddress,
  #if (STD_ON == ETHIF_VLAN_SUPPORT)
  uint8 * LucVlanInfo,
  #endif
  Eth_FrameType FrameType)
{
  static uint8 LddHeaderBuffer[ETHIF_ETH_HEADER_LENGTH];
  /* Copy Destination MAC Address */
  ETHIF_MEMCPY(&LddHeaderBuffer[ETHIF_ZERO], DestMacAddress, ETHIF_SIX);
  /* Copy Source MAC Address */
  ETHIF_MEMCPY(&LddHeaderBuffer[ETHIF_SIX], SrcMacAddress, ETHIF_SIX);
  #if (STD_ON == ETHIF_VLAN_SUPPORT)
  /* Copy TP Id */
  LddHeaderBuffer[ETHIF_TWELVE] = ETHIF_ZERO;
  LddHeaderBuffer[ETHIF_THIRTEEN] = ETHIF_ZERO;
  /* Copy TC Id */
  LddHeaderBuffer[ETHIF_FOURTEEN] = LucVlanInfo[ETHIF_ZERO];
  LddHeaderBuffer[ETHIF_FIFTEEN] = LucVlanInfo[ETHIF_ONE];
  #endif
  /* Copy Frame Type */
  ETHIF_MEMCPY(&LddHeaderBuffer[ETHIF_SIXTEEN], &FrameType, ETHIF_TWO);

  return (uint8 *)&LddHeaderBuffer;
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_TransmitPdu                                   **
**                                                                            **
** Service ID           : 0x1D                                                **
**                                                                            **
** Description          : This API service of the Ethernet Interface Transmits**
**                        Pdu Based Data                                      **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different PduIds. Non reentrant for   **
**                        the same PduId.                                     **
**                                                                            **
** Input Parameters     : TxPduId, PduInfoPtr                                 **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s): EthIf_GaaTxPdus,                **
**                                            EthIf_GSaaController,           **
**                                            EthIf_GaaStaticController,      **
**                                            EthIf_GaaStaticPhyController_PB,**
**                                            EthIf_GddInit                   **
**                                                                            **
**                        Function(s) invoked:                                **
**                                   External: Eth_ImmediateTransmit,         **
**                                             Eth_ProvideTxBuffer,           **
**                                             Eth_Transmit,                  **
**                                             LSduR_EthIfTriggerTransmit,    **
**                                             LSduR_EthIfTxConfirmation,     **
**                                             Det_ReportError,               **
**                                             Det_ReportRuntimeError,        **
**                                             IdsM_ReportSecurityEvent       **
**                                   Internal: EthIf_TransmitInputValidation, **
**                                             EthIf_GetMetaDataInfo,         **
**                                             EthIf_TransmitVLANSupport,     **
**                                             EthIf_GetListElemLength,       **
**                                             EthIf_ListElemTransmitSupport  **
**                                                                            **
** Design ID            : ETHIF_SDD_0731                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_TransmitPdu(
  PduIdType TxPduId, const PduInfoType * PduInfoPtr)
{
  Std_ReturnType LddReturnValue;
  #if (ETHIF_NO_OF_TXPDUS > ETHIF_ZERO)
  uint8 LucCtrlIdx;
  uint8 LucEthCtrlIdx;
  uint8 LucPriority;
  Eth_BufIdxType LddBufIdxPtr;
  uint8 * LpBufPtr;
  Eth_FrameType LddFrameType;
  uint32 LulLenByte;

  PduInfoType LddPduInfo;
  boolean LblHWDataTransfer;

  #if (STD_ON == ETHIF_VLAN_SUPPORT)
  uint8 LucVlanInfo[ETHIF_VLAN_BYTE_LENGTH];
  boolean LblVLANEn;
  #endif
  #ifdef ETHIF_IMMEDIATE_TRANSMIT_SUPPORT
  ListElemStructType LddHeaderListPtr;
  #endif

  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController_PB* LpStaticController_PB;
  #endif

  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis *  LpDriverApis;
  #endif
  const EthIf_TxPduType * LpTxPdu;
  const EthIf_TxPoolType * LpTxPool;

  #if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
  EthIf_ControllerMacsecContextType * LpCtrlCtx;
  EthIf_MacsecSaContextType *LpTxSa;
  PduLengthType SecureDataLen;
  boolean LblMacSecEn;
  #endif

  #if ((ETHIF_METADATA_SUPPORT == STD_ON) && \
       (ETHIF_NO_OF_TXMETADATAITEMS > ETHIF_ZERO))
  EthIf_MetaDataInfo MetaData;
  uint32 LulListElemBufIndex = (uint32)ETHIF_ZERO;
  uint32 LulListElemLen = (uint32)ETHIF_ZERO;
  #endif

  BufReq_ReturnType LddBufRetVal = (BufReq_ReturnType)BUFREQ_E_NOT_OK;

  /* Validate the input parameters and send DET if applicable */
  LddReturnValue = EthIf_TransmitInputValidation(TxPduId, PduInfoPtr);

  if ((Std_ReturnType)E_OK == LddReturnValue)
  {
    LulLenByte = PduInfoPtr->SduLength;
    LpTxPdu = &EthIf_GaaTxPdus[TxPduId];
    LpTxPool = LpTxPdu->pTxPool;

    /* Get the corresponding Contro Index, Priority */
    LucCtrlIdx = LpTxPool->ucCtrlIdx;

    if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= LucCtrlIdx)
    {
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_TRANSMIT_SID, ETHIF_E_INV_CTRL_IDX));
      #endif
      #if (STD_ON == ETHIF_SECURITY_EVENT_ENABLED)
      /* SWS_EthIf_00503:EthIf module won't provide Context data and TimeStamp
         Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      IdsM_ReportSecurityEvent(ETHIF_SEV_ETH_DROP_INV_VLAN, NULL_PTR,
        ETHIF_DEFAULT_CONTEXT_DATA_SIZE, ETHIF_DEFAULT_CONTEXT_DATA_VERSION,
        ETHIF_DEFAULT_COUNT, NULL_PTR);
      #endif
      LddReturnValue = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LpStaticController_PB = &EthIf_GaaStaticController_PB[LucCtrlIdx];
      #endif

      LucEthCtrlIdx = ETHIF_PHY_CTRL_IDX(LucCtrlIdx);
      #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LpDriverApis = EthIf_GaaStaticPhyController_PB
        [(LpStaticController_PB->pPhysCtrl->
        ucPhysCtrlIdx)].pEthDriver;
      #else
      LpDriverApis = (EthIf_GpConfigPtr->pController[LucCtrlIdx].
        pPhysCtrl)->pEthDriver;
      #endif

      if (LpDriverApis != NULL_PTR)
      #endif
      {
        /* Common check for all : indirect, direct(deferred forwarding,
           immediate forwarding) */
        /* SWS_EthIf_00671 */
        /* TxPduId achieved as det error is bypassed already */
        if (ETHIF_PDU_AVAILABLE == EthIf_GaaTxPduStat[TxPduId].PduState)
        {
          /* Fetch Metadata Elements */
          #if ((ETHIF_METADATA_SUPPORT == STD_ON) && \
               (ETHIF_NO_OF_TXMETADATAITEMS > ETHIF_ZERO))
          EthIf_GetMetaDataInfo(&MetaData, PduInfoPtr->MetaDataPtr,
            LpTxPdu->ucMetaDataMask);
          #endif

          LddFrameType = LpTxPool->ddFrameType;
          /* Get Priority value */
          if (LpTxPool->ucPriorityId != ETHIF_INVALID)
          {
            /* If Priority is configured, use that */
            LucPriority = LpTxPool->ucPriorityId;
          }
          else
          {
            #if ((ETHIF_METADATA_SUPPORT == STD_ON) && \
                 (ETHIF_NO_OF_TXMETADATAITEMS > ETHIF_ZERO))
            /* Fetch data from MetaDataItem PRIORITY_8 */
            if (ETHIF_INVALID != MetaData.Priority)
            {
              LucPriority = MetaData.Priority;
            }
            else
            #endif
            {
              LucPriority = ETHIF_ZERO; /* Fallback to 0 value */
            }
          }

          #if (STD_ON == ETHIF_VLAN_SUPPORT)
          /* Support VLAN */
          LddReturnValue = EthIf_TransmitVLANSupport(&LucVlanInfo[ETHIF_ZERO],
            LucCtrlIdx, LucPriority, LddFrameType);
          if ((Std_ReturnType)E_OK == LddReturnValue)
          {
            LblVLANEn = ETHIF_TRUE;
            LddFrameType = (Eth_FrameType)ETHIF_VLAN_FRAMEID;
            LulLenByte = LulLenByte + (uint16)ETHIF_VLAN_BYTE_LENGTH;
          }
          else
          {
            LblVLANEn = ETHIF_FALSE;
            LucPriority = ETHIF_ZERO;
          }
          #else
          /* SWS_EthIf_00146
             Once EA tool is ready, SWS_ID will be updated with Design_ID*/
          LucPriority = ETHIF_ZERO;
          #endif

          /* Initial condition check for deferred forwarding */
          #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
          LblHWDataTransfer = EthIf_GaaStaticPhyController_PB[
            (LpStaticController_PB->pPhysCtrl->ucPhysCtrlIdx)].
            blCtrlHWDataTransfer;
          #else
          LblHWDataTransfer = (EthIf_GpConfigPtr->pController[LucCtrlIdx].
            pPhysCtrl)->blCtrlHWDataTransfer;
          #endif

          /* SWS_EthIf_00670 */
          /* JIRA ID : */
          /* TxPduId achieved as det error is bypassed already */
          if ((ETHIF_TRUE == LpTxPdu->blKeepTxBuffer) &&
            (ETHIF_TRUE == LblHWDataTransfer))
          {
            #ifdef ETHIF_IMMEDIATE_TRANSMIT_SUPPORT
            if (NULL_PTR == PduInfoPtr->SduDataPtr)
            {
              /* Return with failure status */
              LddReturnValue = (Std_ReturnType)E_NOT_OK;
            }
            else
            {
              /* Check for immediate forwarding */
              /* SWS_EthIf_00666 */
              /* To be removed when HeaderListPtr is properly generated */
              LddHeaderListPtr.DataPtr = EthIf_CreateHeaderList(
                MetaData.MacAddress, MetaData.MacAddress,
                #if (STD_ON == ETHIF_VLAN_SUPPORT)
                &LucVlanInfo[0],
                #endif
                LpTxPool->ddFrameType);
              LddHeaderListPtr.DataLength = ETHIF_ETH_HEADER_LENGTH;
              if (MetaData.ListElemTypePtr != NULL_PTR)
              {
                LddHeaderListPtr.NextListElemPtr = MetaData.ListElemTypePtr;
              }
              else
              {
                LddHeaderListPtr.NextListElemPtr = NULL_PTR;
              }

              /* Store The status of PDU */
              EthIf_GaaTxPduStat[TxPduId].BuffIdx = LpTxPdu->ddTxHandleId;
              /* Check for immediate forwarding satisfied */
              /* SWS_EthIf_00667 : Generate LpHeaderListPtr */
              LddReturnValue = Eth_ImmediateTransmit(LucEthCtrlIdx,
                LpTxPdu->ddTxHandleId, LucPriority, &LddHeaderListPtr,
                PduInfoPtr->SduDataPtr, (uint16)PduInfoPtr->SduLength);
            }
            #endif
          }
          else
          {
            /* deferred forwarding */
            /* Get the total length of the List Elem Data from metadata */
            #if ((ETHIF_METADATA_SUPPORT == STD_ON) && \
                 (ETHIF_NO_OF_TXMETADATAITEMS > ETHIF_ZERO))
            if (MetaData.ListElemTypePtr != NULL_PTR)
            {
              LulListElemLen = EthIf_GetListElemLength(MetaData.ListElemTypePtr);
              /* Add the total header length calculated to requested buffer
                 length */
              LulLenByte += LulListElemLen;
            }
            #endif

            #if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
            LblMacSecEn = ETHIF_FALSE;
            #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
            if (ETHIF_SW_MACSEC == LpStaticController_PB->usmacsec)
            #else
            if (ETHIF_SW_MACSEC ==
                EthIf_GpConfigPtr->pController[LucCtrlIdx].usmacsec)
            #endif
            {
              LpCtrlCtx =
              #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
                &EthIf_GaaMacsecController[
                LpStaticController_PB->ucSWMacSecIndex];
              #else
                &EthIf_GaaMacsecController[
                EthIf_GpConfigPtr->pController[LucCtrlIdx].ucSWMacSecIndex];
              #endif
              if (ETHIF_TRUE == LpCtrlCtx->IsMacSecOperational)
              {
                LpTxSa = &LpCtrlCtx->TxSa[LpCtrlCtx->ActiveTxAnIndex];
                if ((ETHIF_TRUE == LpCtrlCtx->IsProtectFramesEnabled) &&
                    (ETHIF_TRUE == LpTxSa->Active))
                {
                  LblMacSecEn = ETHIF_TRUE;
                  /* Add the STAG and EtherType length */
                  LulLenByte += ETHIF_EIGHT;
                  /* Add the ICV length */
                  LulLenByte += ETHIF_MACSEC_ICV_LENGTH;
                }
              }
            }
            #endif

            /* SWS_EthIf_00672 : Proceed for Eth_ProvideTxBuffer */
            LddBufRetVal = ETHIF_ETH_PROVIDETXBUFFER(LucEthCtrlIdx,
              LucPriority, &LddBufIdxPtr, &LpBufPtr, (uint16*)&LulLenByte);

            if (BUFREQ_OK != LddBufRetVal)
            {
              /* Return with failure status */
              LddReturnValue = (Std_ReturnType)E_NOT_OK;
            }
            else
            {
              LddReturnValue = (Std_ReturnType)E_OK;

              #if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
              if (ETHIF_TRUE == LblMacSecEn)
              {
                SecureDataLen = PduInfoPtr->SduLength;
                #if (STD_ON == ETHIF_VLAN_SUPPORT)
                if (ETHIF_TRUE == LblVLANEn)
                {
                  SecureDataLen += ETHIF_FOUR;
                }
                #endif
                #if ((ETHIF_METADATA_SUPPORT == STD_ON) && \
                     (ETHIF_NO_OF_TXMETADATAITEMS > ETHIF_ZERO))
                SecureDataLen += LulListElemLen;
                #endif
                /* add EtherType length */
                SecureDataLen += ETHIF_TWO;
                EthIf_GenerateSecTAG(LpCtrlCtx,
                                     LpTxSa, &LpBufPtr, (uint16)SecureDataLen);
                /* Populate Requested EtherType */
                *LpBufPtr = (uint8)(LddFrameType >> ETHIF_EIGHT);
                *(LpBufPtr + ETHIF_ONE) = (uint8)LddFrameType;
                LpBufPtr += ETHIF_TWO;
                /* Update the EtherType to MacSec EtherType */
                LddFrameType = ETHIF_MACSECETHERTYPE;
              }
              #endif

              #if (STD_ON == ETHIF_VLAN_SUPPORT)
              if (ETHIF_TRUE == LblVLANEn)
              {
                ETHIF_MEMCPY(LpBufPtr, LucVlanInfo, ETHIF_VLAN_BYTE_LENGTH);
                LpBufPtr += ETHIF_VLAN_BYTE_LENGTH; /* Increment the pointer 4
                bytes(0-3) to start adding SduData from 4th byte */
              }
              #endif

              #if ((ETHIF_METADATA_SUPPORT == STD_ON) && \
                   (ETHIF_NO_OF_TXMETADATAITEMS > ETHIF_ZERO))
              if ((uint32)ETHIF_ZERO != LulListElemLen)
              {
                /* Jira : AR-130636 */
                /* Support for extracting List Elem Data from metadata and
                copy to Buffer */
                LulListElemBufIndex = EthIf_ListElemTransmitSupport(LpBufPtr,
                  MetaData.ListElemTypePtr);

                LpBufPtr += LulListElemBufIndex; /* Increment the pointer by
                the list elem data length */
              }
              #endif

              if (NULL_PTR == PduInfoPtr->SduDataPtr)
              {
                LddPduInfo.SduLength = PduInfoPtr->SduLength;
                /* Call LSduR_EthIfTriggerTransmit if SduDataPtr is Null */
                /* DataPtr and length already updated based on VLAN support*/
                LddPduInfo.SduDataPtr = LpBufPtr;
                LddReturnValue = LSduR_EthIfTriggerTransmit(LpTxPdu->ddConfPduId,
                                                            &LddPduInfo);
              }
              else
              {
                /* Copy Data into Buffer if Provided */
                ETHIF_MEMCPY(LpBufPtr, PduInfoPtr->SduDataPtr,
                             (uint16)PduInfoPtr->SduLength);
              }

              #if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
              if (ETHIF_TRUE == LblMacSecEn)
              {
                LpBufPtr -= ETHIF_TWO;
                (void) EthIf_GenerateICV(LucCtrlIdx, LpBufPtr,
                     PduInfoPtr->SduLength + ETHIF_TWO);
              }
              #endif

              if ((Std_ReturnType)E_OK == LddReturnValue)
              {
                EthIf_GaaTxPduStat[TxPduId].BuffIdx = LddBufIdxPtr;
                /* SWS_EthIf_00674 */
                /* Call Eth_Transmit to Trigger Transmission */
                /* SWS_EthIf_00076a
                  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
                LddReturnValue = ETHIF_ETH_TRANSMIT(LucEthCtrlIdx, LddBufIdxPtr,
                  LddFrameType, (uint16)LulLenByte,
                  #if ((ETHIF_METADATA_SUPPORT == STD_ON) && \
                       (ETHIF_NO_OF_TXMETADATAITEMS > ETHIF_ZERO))
                  MetaData.MacAddress);
                  #else
                  NULL_PTR);
                  #endif
              }
            }
          }
          if ((Std_ReturnType)E_OK == LddReturnValue)
          {

            #if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
            EthIf_MacSecUpdateTxStats(LucCtrlIdx, (uint16)PduInfoPtr->SduLength,
                                      (uint16)LulLenByte);
            #endif
            /* Store The status of PDU */
            EthIf_GaaTxPduStat[TxPduId].ddConfPduId = LpTxPdu->ddConfPduId;
            EthIf_GaaTxPduStat[TxPduId].ucPhyCtrlIndex = LucEthCtrlIdx;
            EthIf_GaaTxPduStat[TxPduId].PduState = ETHIF_PDU_IN_USE;
          }
          else
          {
            /* SWS_EthIf_00674 : Negative case for LSduR_EthIfTriggerTransmit */
            /* SWS_EthIf_00675 : Negative case for Eth_Transmit */
            /* Notify upper layer of failure */
            LSduR_EthIfTxConfirmation(LpTxPdu->ddConfPduId, E_NOT_OK);
          }
        }
        else
        {
          /* SWS_EthIf_00676 : Negative scenario */
          /* SWS_EthIf_00646 */
          #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
          (void)Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
            ETHIF_TRANSMIT_SID, ETHIF_E_PDU_STATE_TRANSITION_FAILED);
          #endif
          /* Return with failure status */
          LddReturnValue = (Std_ReturnType)E_NOT_OK;
        }
      }
    }
  }
  else
  {
    /* Return with failure status */
    LddReturnValue = (Std_ReturnType)E_NOT_OK;
  }
  #else /* (ETHIF_NO_OF_TXPDUS > ETHIF_ZERO) ends*/
  UNUSED(TxPduId);
  UNUSED(PduInfoPtr);
  LddReturnValue = (Std_ReturnType)E_NOT_OK;
  #endif
  return LddReturnValue;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_GetVersionInfo                                **
**                                                                            **
** Service ID           : 0x0B                                                **
**                                                                            **
** Description          : This service returns the version information of     **
**                        this module.                                        **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant                                           **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : VersionInfoPtr                                      **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)  : None                          **
**                                                                            **
**                        Function(s) invoked : Det_ReportError               **
**                                                                            **
** Design ID            : ETHIF_SDD_0160                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_VERSIONINFO_API)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 2 */
void EthIf_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr)
{
  Std_ReturnType LddStatusReturn;

  LddStatusReturn = (Std_ReturnType)E_OK;
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be True when
  VersionInfoPtr is  NULL_PTR .
  Module Test Case Reference ETHIF_ETC_049, SWS ID  SWS_EthIf_00127"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == VersionInfoPtr)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETVERSIONINFO_SID, ETHIF_E_PARAM_POINTER));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
   DET error is present.
  Module Test Case Reference ETHIF_ETC_049, SWS ID  SWS_EthIf_00127"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* SWS_EthIf_00082h
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    /* Copy the vendor Id */
    VersionInfoPtr->vendorID = ETHIF_VENDOR_ID;
    /* Copy the module Id */
    VersionInfoPtr->moduleID = ETHIF_MODULE_ID;
    /* Copy Software Major Version */
    VersionInfoPtr->sw_major_version = (uint8)ETHIF_SW_MAJOR_VERSION;
    /* Copy Software Minor Version */
    VersionInfoPtr->sw_minor_version = (uint8)ETHIF_SW_MINOR_VERSION;
    /* Copy Software Patch Version */
    VersionInfoPtr->sw_patch_version = (uint8)ETHIF_SW_PATCH_VERSION;
  } /* End of if ((Std_ReturnType)E_OK == LddStatusReturn) */
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_CheckWakeup                                   **
**                                                                            **
** Service ID           : 0x30                                                **
**                                                                            **
** Description          : Service is called by integration code to check a    **
**                        wakeup source.                                      **
**                                                                            **
** Sync/Async           : Asynchronous                                        **
**                                                                            **
** Re-entrancy          : Reentrant                                           **
**                                                                            **
** Input Parameters     : WakeupSource                                        **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GaaWakeupSource,         **
**                                                                            **
**                        Function(s) invoked: EthTrcv_CheckWakeup            **
**                                                                            **
** Design ID            : ETHIF_SDD_0112                                      **
*******************************************************************************/
#if ((ETHIF_ZERO < ETHIF_NUM_OF_TRANSCEIVERS) &&\
(STD_ON == ETHIF_WAKEUP_SUPPORT))
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 2 */
Std_ReturnType EthIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource)
{
  const EthIf_WakeupSourceType*  LpWakeupSource;
  const EthIf_TrcvApis*  LpTrcvApis;
  Std_ReturnType LddStatusReturn;

  #if (ETHIF_TOTAL_NUM_OF_WAKEUPSOURCE > ETHIF_ONE)
  uint8 LucIdx;
  LucIdx = ETHIF_ZERO;
  #endif

  LddStatusReturn = (Std_ReturnType)E_OK;

  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_126, SWS ID SWS_EthIf_00246 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_CHECKWAKEUP_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  if ((EcuM_WakeupSourceType)ETHIF_INVALID_32 <= WakeupSource)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_CHECKWAKEUP_SID, ETHIF_E_INV_PARAM));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be True when
  EthIf module is in INITIALIZED state and NO DET errors are prsent.
  Module Test Case Reference ETHIF_ETC_126, SWS ID SWS_EthIf_00246 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    #if (ETHIF_TOTAL_NUM_OF_WAKEUPSOURCE > ETHIF_ONE)
    while (LucIdx < ETHIF_TOTAL_NUM_OF_WAKEUPSOURCE)
    {
      LpWakeupSource = &EthIf_GaaWakeupSource[LucIdx];
    #else
      LpWakeupSource = &EthIf_GaaWakeupSource[ETHIF_ZERO];
    #endif
      /* Get the pointer to the requested Controller's structure */
      LpTrcvApis = LpWakeupSource->pTrcvDriver;

      /* SWS_EthIf_00245
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      if (LpWakeupSource->ulWakeupSource == WakeupSource)
      {
    /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
    LpTrcvApis will be NULL.
    Module SWS Reference SWS ID SWS_EthIf_00245 "
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
        if (LpTrcvApis != NULL_PTR)
        {
          LddStatusReturn = (LpTrcvApis->pEthTrcvCheckWakeup)
            (LpWakeupSource->ucTrcvIdx);
        }
      }
    #if (ETHIF_TOTAL_NUM_OF_WAKEUPSOURCE > ETHIF_ONE)
      LucIdx++;
    }
    #endif
  }
  /* SWS_EthIf_00244f
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_GetPortMacAddr                                **
**                                                                            **
** Service ID           : 0x28                                                **
**                                                                            **
** Description          : This Service Obtains the port over which this       **
**                        MAC-address can be reached.                         **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : MacAddrPtr                                          **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : SwitchIdxPtr, PortIdxPtr                            **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init should invoked.                          **
**                                                                            **
** Remarks              : Global Variable(s)  : None                          **
**                                                                            **
**                        Function(s) invoked : EthSwt_GetPortMacAddr         **
**                                                                            **
** Design ID            : ETHIF_SDD_0103                                      **
*******************************************************************************/
/* PRQA S 1251 2 */
#if ((ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES) &&\
(STD_ON == ETHIF_GET_PORT_MAC_ADDR_API))
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 3 */
Std_ReturnType EthIf_GetPortMacAddr(
  const uint8 * MacAddrPtr, uint8 * SwitchIdxPtr, uint8 * PortIdxPtr)
{
  Std_ReturnType LddStatusReturn;

  LddStatusReturn = (Std_ReturnType)E_OK;
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_197, SWS ID SWS_EthIf_00193 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETPORTMACADDR_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  #if ((STD_ON == ETHIF_DEV_ERROR_DETECT) || (STD_ON == ETHIF_SAFE_KSAR))
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be true when
  MacAddrPtr is NULL_PTR .
  Module Test Case Reference ETHIF_ETC_035, SWS ID SWS_EthIf_00194 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == MacAddrPtr)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETPORTMACADDR_SID, ETHIF_E_PARAM_POINTER));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be true when
  SwitchIdxPtr is NULL_PTR .
  Module Test Case Reference ETHIF_ETC_035, SWS ID SWS_EthIf_00194 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == SwitchIdxPtr)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETPORTMACADDR_SID, ETHIF_E_PARAM_POINTER));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be true when
  PortIdxPtr is NULL_PTR .
  Module Test Case Reference ETHIF_ETC_035, SWS ID SWS_EthIf_00194 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == PortIdxPtr)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETPORTMACADDR_SID, ETHIF_E_PARAM_POINTER));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be true when
  EthIf module is in INITIALIZED state and NO det errors are present.
  Module Test Case Reference ETHIF_ETC_197, SWS ID SWS_EthIf_00193 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* SWS_EthIf_00191
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    LddStatusReturn = EthSwt_GetPortMacAddr(MacAddrPtr, SwitchIdxPtr,
       PortIdxPtr);
  }
  /* SWS_EthIf_00190f
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_GetArlTable                                   **
**                                                                            **
** Service ID           : 0x29                                                **
**                                                                            **
** Description          : This Service Obtains the address resolution table of**
**                        a switch.                                           **
**                                                                            **
** Sync/Async           : Synchronous /Asynchronous                           **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : SwitchIdx                                           **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : ArlTable                                            **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init should invoked.                          **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstSwtApis           **
**                                                                            **
**                        Function(s) invoked : EthSwt_GetArlTable            **
**                                                                            **
** Design ID            : ETHIF_SDD_0100                                      **
*******************************************************************************/
/* PRQA S 1251 2 */
#if ((ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES) && (STD_ON == ETHIF_GET_ARL_TABLE))
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 2 */
Std_ReturnType EthIf_GetArlTable(uint8 SwitchIdx, EthSwt_MacVlanType * ArlTable)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticSwitch_PB*  LpStaticSwitch_PB;
  #if (ETHIF_ONE < ETHIF_SWT_DRIVER_COUNT)
  const EthIf_SwtApis*  LpSwtApis;
  #endif
  #endif

  Std_ReturnType LddStatusReturn;

  LddStatusReturn = (Std_ReturnType)E_OK;
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_201, SWS ID SWS_EthIf_00200 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETARLTABLE_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  #if ((STD_ON == ETHIF_DEV_ERROR_DETECT) || (STD_ON == ETHIF_SAFE_KSAR))
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be true when
  ArlTable is NULL_PTR .
  Module Test Case Reference ETHIF_ETC_037, SWS ID SWS_EthIf_00193 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == ArlTable)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETARLTABLE_SID, ETHIF_E_PARAM_POINTER));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be true when
  EthIf module is in INITIALIZED state and no DET errors are present.
  Module Test Case Reference ETHIF_ETC_201, SWS ID SWS_EthIf_00199 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {

    if ((uint8)ETHIF_SWT_DRIVER_COUNT > SwitchIdx)
    {
      /* Get the pointer to the requested Controller's structure */
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LpStaticSwitch_PB = &EthIf_GaaStaticSwitch_PB[SwitchIdx];
      #endif
      #if (ETHIF_ONE < ETHIF_SWT_DRIVER_COUNT)
      #if (STD_ON == ETHIF_PRE_COMPILE_SINGLE)
      LpSwtApis = ETHIF_GPFIRSTSWTAPIS;
      #endif

      /* SWS_EthIf_00196h
       Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LddStatusReturn = LpSwtApis[LpStaticSwitch_PB->ucSwtDriverIdx].
        pEthSwtGetArlTable(LpStaticSwitch_PB->ucSwtIdx, (uint32)ETHIF_ZERO,
          (uint32)ETHIF_ZERO, ArlTable);
      #else
      LddStatusReturn = EthIf_GaaSwtApis[EthIf_GpConfigPtr->pStaticSwitch[SwitchIdx].ucSwtDriverIdx].
        pEthSwtGetArlTable(EthIf_GpConfigPtr->pStaticSwitch[SwitchIdx].ucSwtIdx, (uint32)ETHIF_ZERO,
          (uint32)ETHIF_ZERO, ArlTable);
      #endif
      #else
      /* SWS_EthIf_00196h
       Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LddStatusReturn = EthSwt_GetArlTable(LpStaticSwitch_PB->ucSwtIdx,
        (uint32)ETHIF_ZERO, (uint32)ETHIF_ZERO, ArlTable);
      #else
          /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
        EthIf_GpConfigPtr is not a NULL PTR.
        Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_00040a"
        Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      LddStatusReturn = EthSwt_GetArlTable(EthIf_GpConfigPtr->pStaticSwitch[SwitchIdx].ucSwtIdx,
        (uint32)ETHIF_ZERO, (uint16)ETHIF_ZERO, ArlTable);
      #endif
      #endif
    }
    else
    {
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
  }
  /* SWS_EthIf_00196f
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_StoreConfiguration                            **
**                                                                            **
** Service ID           : 0x2c                                                **
**                                                                            **
** Description          : Stores the configuration of the learned MAC/Port    **
**                        tables of a switch in a persistent manner.          **
**                                                                            **
** Sync/Async           : Synchronous /Asynchronous                           **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : SwitchIdx                                           **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init should invoked.                          **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstSwtApis           **
**                                                                            **
**                        Function(s) invoked : EthSwt_StoreConfiguration     **
**                                                                            **
**  Design ID           : ETHIF_SDD_0105                                      **
*******************************************************************************/
/* PRQA S 1251 2 */
#if ((ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES) &&\
(STD_ON == ETHIF_STORE_CONFIGURATION_API))
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 2 */
Std_ReturnType EthIf_StoreConfiguration(uint8 SwitchIdx)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticSwitch_PB*  LpStaticSwitch_PB;
  #if (ETHIF_ONE < ETHIF_SWT_DRIVER_COUNT)
  const EthIf_SwtApis*  LpSwtApis;
  #endif
  #endif

  Std_ReturnType LddStatusReturn;

  LddStatusReturn = (Std_ReturnType)E_OK;
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_213, SWS ID SWS_EthIf_00217"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_STORECONFIGURATION_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be True when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_213, SWS ID SWS_EthIf_00217 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {

    if ((uint8)ETHIF_SWT_DRIVER_COUNT > SwitchIdx)
    {
      /* Get the pointer to the requested Controller's structure */
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LpStaticSwitch_PB = &EthIf_GaaStaticSwitch_PB[SwitchIdx];
      #endif
      #if (ETHIF_ONE < ETHIF_SWT_DRIVER_COUNT)
      #if (STD_ON == ETHIF_PRE_COMPILE_SINGLE)
      LpSwtApis = ETHIF_GPFIRSTSWTAPIS;
      #endif
      #endif

      #if (ETHIF_ONE < ETHIF_SWT_DRIVER_COUNT)
      /* SWS_EthIf_00214h
       Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LddStatusReturn = LpSwtApis[LpStaticSwitch_PB->ucSwtDriverIdx].
        pEthSwtStoreConfiguration(LpStaticSwitch_PB->ucSwtIdx);
      #else
      LddStatusReturn = EthIf_GaaSwtApis[EthIf_GpConfigPtr->pStaticSwitch[SwitchIdx].ucSwtDriverIdx].
        pEthSwtStoreConfiguration(EthIf_GpConfigPtr->pStaticSwitch[SwitchIdx].ucSwtIdx);
      #endif
      #else
      /* SWS_EthIf_00214h
       Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LddStatusReturn = EthSwt_StoreConfiguration(LpStaticSwitch_PB->ucSwtIdx);
      #else
          /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
        EthIf_GpConfigPtr is not a NULL PTR.
        Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_00040a"
        Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      LddStatusReturn = EthSwt_StoreConfiguration(EthIf_GpConfigPtr->pStaticSwitch[SwitchIdx].ucSwtIdx);
      #endif
      #endif
    }
    else
    {
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
  }
  /* SWS_EthIf_00214f
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_ResetConfiguration                            **
**                                                                            **
** Service ID           : 0x2d                                                **
**                                                                            **
** Description          : Reset the configuration of the learned MAC/Port     **
**                        tables of a switch in a persistent manner.          **
**                                                                            **
** Sync/Async           : Synchronous /Asynchronous                           **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : SwitchIdx                                           **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init should invoked.                          **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstSwtApis           **
**                                                                            **
**                        Function(s) invoked : EthSwt_ResetConfiguration     **
**                                                                            **
**  Design ID           : ETHIF_SDD_0104                                      **
*******************************************************************************/
/* PRQA S 1251 2 */
#if ((ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES) &&\
(STD_ON == ETHIF_RESET_CONFIGURATION_API))
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 2 */
Std_ReturnType EthIf_ResetConfiguration(uint8 SwitchIdx)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticSwitch_PB*  LpStaticSwitch_PB;
  #if (ETHIF_ONE < ETHIF_SWT_DRIVER_COUNT)
  const EthIf_SwtApis*  LpSwtApis;
  #endif
  #endif
  Std_ReturnType LddStatusReturn;

  LddStatusReturn = (Std_ReturnType)E_OK;
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_217, SWS ID SWS_EthIf_00219"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_RESETCONFIGURATION_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be True when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_217, SWS ID SWS_EthIf_00219"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {

    if ((uint8)ETHIF_SWT_DRIVER_COUNT > SwitchIdx)
    {
      /* Get the pointer to the requested Controller's structure */
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LpStaticSwitch_PB = &EthIf_GaaStaticSwitch_PB[SwitchIdx];
      #endif
      #if (ETHIF_ONE < ETHIF_SWT_DRIVER_COUNT)
      #if (STD_ON == ETHIF_PRE_COMPILE_SINGLE)
      LpSwtApis = ETHIF_GPFIRSTSWTAPIS;
      #endif
      #endif

      #if (ETHIF_ONE < ETHIF_SWT_DRIVER_COUNT)
      /* SWS_EthIf_00219h
       Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LddStatusReturn = LpSwtApis[LpStaticSwitch_PB->ucSwtDriverIdx].
        pEthSwtResetConfiguration(LpStaticSwitch_PB->ucSwtIdx);
      #else
      LddStatusReturn = EthIf_GaaSwtApis[EthIf_GpConfigPtr->pStaticSwitch[SwitchIdx].ucSwtDriverIdx].
        pEthSwtResetConfiguration(EthIf_GpConfigPtr->pStaticSwitch[SwitchIdx].ucSwtIdx);
      #endif
      #else
      /* SWS_EthIf_00219h
       Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LddStatusReturn = EthSwt_ResetConfiguration(LpStaticSwitch_PB->ucSwtIdx);
      #else
          /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
        EthIf_GpConfigPtr is not a NULL PTR.
        Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_00040a"
        Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      LddStatusReturn = EthSwt_ResetConfiguration(EthIf_GpConfigPtr->pStaticSwitch[SwitchIdx].ucSwtIdx);
      #endif
      #endif
    }
    else
    {
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
  }
  /* SWS_EthIf_00219f
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_globalTime_Inputvalidation                    **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : CtrlIdx, ClkIdx check for PhcTime APIs              **
**                                                                            **
** Sync/Async           : NA                                                  **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : ClkIdx , ClkIdx                                     **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s): None                            **
**                                                                            **
**  Design ID           : ETHIF_SDD_0601                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_GLOBALTIME_SUPPORT)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static Std_ReturnType EthIf_globalTime_Inputvalidation(
  uint8 CtrlIdx, uint8 ClkUnitIdx, uint8 GlobalTime_API_SID)
{
  uint8 LusClksize = ETHIF_ZERO;
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;

  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    /*SWS_EthIf_00651 :
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      GlobalTime_API_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  else
  {
    /* SWS_EthIf_00653 : validates the received ctrlIdx
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
    {
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        GlobalTime_API_SID, ETHIF_E_INV_CTRL_IDX));
      #endif
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
      #if (STD_ON == ETHIF_PRE_COMPILE_SINGLE)
      if ((EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl)
           ->pEthClk != NULL_PTR)
      {
        LusClksize = ((EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl)
         ->pEthClk)->ucclksize;
      }
      else
      {
        LusClksize = ETHIF_ZERO;
      }
      #else
      if ((((EthIf_GpConfigPtr->pController[CtrlIdx]).pPhysCtrl)
          ->pEthClk) != NULL_PTR)
      {
        LusClksize = (((EthIf_GpConfigPtr->pController[CtrlIdx]).pPhysCtrl)
        ->pEthClk->ucclksize);
      }
      else
      {
        LusClksize = ETHIF_ZERO;
      }
      #endif
    }

    if (LusClksize <= ClkUnitIdx)
    {
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        GlobalTime_API_SID, ETHIF_E_INV_CLKUNIT_IDX));
      #endif
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
  }

  #if (STD_OFF == ETHIF_DEV_ERROR_DETECT)
  ETHIF_UNUSED(GlobalTime_API_SID);
  #endif
  return(LddStatusReturn);
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_globalTime_GetDrvIds                          **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : Get Drv CtrlIdx, ClkIdx check for PhcTime APIs      **
**                                                                            **
** Sync/Async           : NA                                                  **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlUnitIdx , ClkUnitIdx                            **
**                                                                            **
** Output Parameters    : CtrlIdx, ClkIdx                                     **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s): None                            **
**                                                                            **
** Design ID            : ETHIF_SDD_0110                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_GLOBALTIME_SUPPORT)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static Std_ReturnType EthIf_globalTime_GetDrvIds(
  uint8 CtrlUnitIdx, uint8 ClkUnitIdx, uint8 * CtrlIdx, uint8 * ClkIdx)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;

  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  if ((EthIf_GaaStaticController_PB[CtrlUnitIdx].pPhysCtrl)
       ->pEthClk != NULL_PTR)
  {
    *ClkIdx = ((EthIf_GaaStaticController_PB[CtrlUnitIdx].pPhysCtrl)
     ->pEthClk)->pclkindx[ClkUnitIdx];

    *CtrlIdx = EthIf_GaaStaticPhyController_PB[((EthIf_GaaStaticController_PB
       [CtrlUnitIdx].pPhysCtrl)->ucPhysCtrlIdx)].ucCtrlIdx;
  }
  else
  {
    *CtrlIdx = ETHIF_INVALID;
    *ClkIdx = ETHIF_INVALID;
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #else
  if ((EthIf_GpConfigPtr->pController[CtrlUnitIdx].pPhysCtrl)
       ->pEthClk != NULL_PTR)
  {
    *CtrlIdx = EthIf_GpConfigPtr->pStaticPhyController
    [(EthIf_GpConfigPtr->pController[CtrlUnitIdx].pPhysCtrl)
     ->ucPhysController].ucCtrlIdx;

    *ClkIdx = (EthIf_GpConfigPtr->pController[CtrlUnitIdx].pPhysCtrl)->pEthClk
       ->pclkindx[ClkUnitIdx];
  }
  else
  {
    *ClkIdx = ETHIF_INVALID;
    *CtrlIdx = ETHIF_INVALID;
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif

  return(LddStatusReturn);
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_SetPhcTime                                    **
**                                                                            **
** Service ID           : 0x96                                                **
**                                                                            **
** Description          : This API service of the Ethernet Interface Sets the **
**                        the absolute time of the PHC                        **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx, ClkUnitIdx,                                **
**                        timeStampPtr                                        **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GaaClkUnitIndx           **
**                                                                            **
**                        Function(s) invoked:Eth_SetPhcTime                  **
**                                                                            **
** Design ID            : ETHIF_SDD_0111                                      **
*******************************************************************************/
#if (ETHIF_HW_DISPLINCE_CLK_DISABLED == STD_OFF)
#if (ETHIF_PHC_SUPPORT_ENABLED == STD_ON)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_SetPhcTime(
  uint8 CtrlIdx, uint8 ClkUnitIdx, const TimeStampType * timeStampPtr)
{
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  uint8 LusCtrlIdx;
  uint8 LusClkUnitIdx;
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;

  /* SWS_EthIf_00609 : validates the Input ClkUnitIdx and CtrlIdx
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  LddStatusReturn = EthIf_globalTime_Inputvalidation(CtrlIdx, ClkUnitIdx,
   ETHIF_SETPHCTIME_SID);

  /* SWS_EthIf_00652 : validates the input timeStampPtr pointer
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (timeStampPtr == NULL_PTR)
  {
    #if (STD_ON == ETHIF_SAFE_KSAR)
    (void)(Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_SETPHCTIME_SID, ETHIF_E_PARAM_POINTER));
    #else
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_SETPHCTIME_SID, ETHIF_E_PARAM_POINTER));
      #endif
    #endif

    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    LddStatusReturn = EthIf_globalTime_GetDrvIds(CtrlIdx, ClkUnitIdx,
      &LusCtrlIdx, &LusClkUnitIdx);
  }

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpDriverApis = (&EthIf_GaaStaticPhyController_PB[
        (EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl)->ucPhysCtrlIdx]->pEthDriver);
    #else
    LpDriverApis = ((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl).pEthDriver);
    #endif
    /* Invoke respective Eth Drivers' API */
    if (NULL_PTR != LpDriverApis)
    {
      LddStatusReturn = (LpDriverApis->pEthSetPhcTime)
      (LusCtrlIdx, LusClkUnitIdx, timeStampPtr);
    }
    #else
    /* SWS_EthIf_00586 : Ethernet Driver API call
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    LddStatusReturn = Eth_SetPhcTime(LusCtrlIdx, LusClkUnitIdx, timeStampPtr);
    #endif
  }

  return(LddStatusReturn);
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif
#endif

/*******************************************************************************
** Function Name        : EthIf_GetPhcTime                                    **
**                                                                            **
** Service ID           : 0x98                                                **
**                                                                            **
** Description          : This API service of the Ethernet Interface Gets the **
**                        the Current time  value of the PHC                  **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx, ClkUnitIdx,                                **
**                        timeStampPtr and timeQualPtr                        **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GaaClkUnitIndx           **
**                                                                            **
**                        Function(s) invoked:Eth_GetPhcTime                  **
**                                                                            **
** Design ID            : ETHIF_SDD_0114                                      **
*******************************************************************************/
#if (ETHIF_HW_DISPLINCE_CLK_DISABLED == STD_OFF)
#if (ETHIF_PHC_SUPPORT_ENABLED == STD_ON)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_GetPhcTime(
  uint8 CtrlIdx, uint8 ClkUnitIdx, TimeStampQualType * timeQualPtr,
  TimeStampType * timeStampPtr)
{
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  uint8 LusCtrlIdx = ETHIF_ZERO;
  uint8 LusClkUnitIdx = ETHIF_ZERO;
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;

  /* SWS_EthIf_00627 : validates the received ClkUnitIdx
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  LddStatusReturn = EthIf_globalTime_Inputvalidation(CtrlIdx, ClkUnitIdx,
    ETHIF_GETPHCTIME_SID);

  /* SWS_EthIf_00652 : validates the input timeStampPtr & timeQualPtr pointer
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((timeStampPtr == NULL_PTR) || (timeQualPtr == NULL_PTR))
  {
    #if (STD_ON == ETHIF_SAFE_KSAR)
    (void)(Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETPHCTIME_SID, ETHIF_E_PARAM_POINTER));
    #else
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_GETPHCTIME_SID, ETHIF_E_PARAM_POINTER));
      #endif
    #endif

    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    LddStatusReturn = EthIf_globalTime_GetDrvIds(CtrlIdx, ClkUnitIdx,
      &LusCtrlIdx, &LusClkUnitIdx);
  }

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpDriverApis = (&EthIf_GaaStaticPhyController_PB[
        (EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl)->ucPhysCtrlIdx]->pEthDriver);
    #else
    LpDriverApis = ((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl).pEthDriver);
    #endif
    /* Invoke respective Eth Drivers' API */
    if (NULL_PTR != LpDriverApis)
    {
      LddStatusReturn = (LpDriverApis->pEthGetPhcTime)
        (LusCtrlIdx, LusClkUnitIdx, timeQualPtr, timeStampPtr);
    }
    #else
    /* SWS_EthIf_00631 : Ethernet Driver API call
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    LddStatusReturn = Eth_GetPhcTime(LusCtrlIdx, LusClkUnitIdx,
        timeQualPtr, timeStampPtr);
    #endif
  }
  return(LddStatusReturn);
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif
#endif

/*******************************************************************************
** Function Name        : EthIf_SetPhcCorrection                              **
**                                                                            **
** Service ID           : 0x97                                                **
**                                                                            **
** Description          : This API service of the Ethernet Interface Sets the **
**                        PHC parameters to adapt rate and offset of the PHC  **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx, ClkUnitIdx,                                **
**                        rateDeviation and offset                            **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GaaClkUnitIndx           **
**                                                                            **
**                        Function(s) invoked: Eth_SetPhcCorrection           **
**                                                                            **
** Design ID            : ETHIF_SDD_0115                                      **
*******************************************************************************/
#if (ETHIF_HW_DISPLINCE_CLK_DISABLED == STD_OFF)
#if (ETHIF_PHC_SUPPORT_ENABLED == STD_ON)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_SetPhcCorrection(
  uint8 CtrlIdx, uint8 ClkUnitIdx, sint32 rateDeviation, sint32 offset)
{
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  uint8 LusCtrlIdx = ETHIF_ZERO;
  uint8 LusClkUnitIdx = ETHIF_ZERO;
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;

  /* SWS_EthIf_00652 : validates the input timeStampPtr pointer
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  LddStatusReturn = EthIf_globalTime_Inputvalidation(CtrlIdx, ClkUnitIdx,
    ETHIF_SETPHCCORRECTION_SID);

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    LddStatusReturn = EthIf_globalTime_GetDrvIds(CtrlIdx, ClkUnitIdx,
      &LusCtrlIdx, &LusClkUnitIdx);
  }

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpDriverApis = (&EthIf_GaaStaticPhyController_PB[CtrlIdx])->pEthDriver;
    #else
    LpDriverApis = EthIf_GpConfigPtr->pStaticPhyController[CtrlIdx].pEthDriver;
    #endif
    /* Invoke respective Eth Drivers' API */
    if (NULL_PTR != LpDriverApis)
    {
      LddStatusReturn = (LpDriverApis->pEthSetPhcCorrection)
        (LusCtrlIdx, LusClkUnitIdx, rateDeviation, offset);
    }
    #else
    /* SWS_EthIf_00624 : Ethernet Driver API call
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    LddStatusReturn = Eth_SetPhcCorrection(LusCtrlIdx, LusClkUnitIdx,
        rateDeviation, offset);
    #endif
  }

  return(LddStatusReturn);
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif
#endif

/*******************************************************************************
** Function Name        : EthIf_SetPpsSignalMode                              **
**                                                                            **
** Service ID           : 0x99                                                **
**                                                                            **
** Description          : This API service of the Ethernet Interface          **
**                        Enables/disables the generation of a PPS signal     **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx, ClkUnitIdx,                                **
**                        signalMode                                          **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GaaClkUnitIndx           **
**                                                                            **
**                        Function(s) invoked: Eth_SetPpsSignalMode           **
**                                                                            **
** Design ID            : ETHIF_SDD_0116                                      **
*******************************************************************************/
#if (ETHIF_HW_DISPLINCE_CLK_DISABLED == STD_OFF)
#if (ETHIF_PHC_SUPPORT_ENABLED == STD_ON)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_SetPpsSignalMode(
  uint8 CtrlIdx, uint8 ClkUnitIdx, boolean signalMode)
{
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  uint8 LusCtrlIdx = ETHIF_ZERO;
  uint8 LusClkUnitIdx = ETHIF_ZERO;
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;

  /* SWS_EthIf_00622 : validates the received ClkUnitIdx
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  LddStatusReturn = EthIf_globalTime_Inputvalidation(CtrlIdx, ClkUnitIdx,
    ETHIF_SETPPSSIGNALMODE_SID);

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    LddStatusReturn = EthIf_globalTime_GetDrvIds(CtrlIdx, ClkUnitIdx,
      &LusCtrlIdx, &LusClkUnitIdx);
  }

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpDriverApis = (&EthIf_GaaStaticPhyController_PB[
        (EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl)->ucPhysCtrlIdx]->pEthDriver);
    #else
    LpDriverApis = ((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl).pEthDriver);
    #endif
    /* Invoke respective Eth Drivers' API */
    if (NULL_PTR != LpDriverApis)
    {
      LddStatusReturn = (LpDriverApis->pEthSetPpsSignalMode)
        (LusCtrlIdx, LusClkUnitIdx,  signalMode);
    }
    #else
    /* SWS_EthIf_00636 : Ethernet Driver API call
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    LddStatusReturn = Eth_SetPpsSignalMode(LusCtrlIdx,
       LusClkUnitIdx, signalMode);
    #endif
  }

  return(LddStatusReturn);
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif
#endif

/*******************************************************************************
** Function Name        : EthIf_GetCurrentTimeTuple                           **
**                                                                            **
** Service ID           : 0x95                                                **
**                                                                            **
** Description          : This service Reads the current time of the timestamp**
**                        clock and the current time of the PHC.              **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx, ClkUnitIdx                                 **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : currentTimeTuplePtr                                 **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : EthIf_Init should invoked.                          **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstDriverApis        **
**                                                                            **
**                        Function(s) invoked: Eth_GetCurrentTimeTuple        **
**                                                                            **
** Design ID            : ETHIF_SDD_0107                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_GLOBALTIME_SUPPORT)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_GetCurrentTimeTuple(
  uint8 CtrlIdx, uint8 ClkUnitIdx, TimeTupleType *  currentTimeTuplePtr)
{
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  uint8 LusCtrlIdx = ETHIF_ZERO;
  uint8 LusClkUnitIdx = ETHIF_ZERO;
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;

  /* SWS_EthIf_00603 : validates the received ClkUnitIdx
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  LddStatusReturn = EthIf_globalTime_Inputvalidation(CtrlIdx, ClkUnitIdx,
    ETHIF_GETCURRENTTIMETUPLE_SID);

  /* Checks currentTimeTuplePtr validation */
  /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be true when
  currentTimeTuplePtr is NULL_PTR .
  Module Test Case Reference ETHIF_ETC_040, SWS ID SWS_EthIf_00157 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == currentTimeTuplePtr)
  {
    #if (STD_ON == ETHIF_SAFE_KSAR)
    (void)(Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETCURRENTTIMETUPLE_SID, ETHIF_E_PARAM_POINTER));
    #else
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_GETCURRENTTIMETUPLE_SID, ETHIF_E_PARAM_POINTER));
      #endif
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    LddStatusReturn = EthIf_globalTime_GetDrvIds(CtrlIdx, ClkUnitIdx,
      &LusCtrlIdx, &LusClkUnitIdx);
  }

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpDriverApis = (EthIf_GaaStaticPhyController_PB[
        (EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl)->ucPhysCtrlIdx].pEthDriver);
    #else
    LpDriverApis = ((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->pEthDriver);
    #endif
    /* Invoke respective Eth Drivers' API */
    if (NULL_PTR != LpDriverApis)
    {
      LddStatusReturn = (LpDriverApis->pEthGetCurrentTimeTuple)
      (LusCtrlIdx, LusClkUnitIdx, currentTimeTuplePtr);
    }
    #else
    /* Invoke Eth Driver API */
    LddStatusReturn = Eth_GetCurrentTimeTuple(LusCtrlIdx, LusClkUnitIdx,
       currentTimeTuplePtr);
    #endif
  }

  return(LddStatusReturn);
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_EnableEgressTimeStamp                         **
**                                                                            **
** Service ID           : 0x23                                                **
**                                                                            **
** Description          : This service Activates egress time stamping on a    **
**                        dedicated message object.                           **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx, BufIdx                                     **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : EthIf_Init should invoked.                          **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstDriverApis        **
**                                                                            **
**                        Function(s) invoked: Eth_EnableEgressTimeStamp      **
**                                                                            **
**  Design ID           : ETHIF_SDD_0106                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_GLOBALTIME_SUPPORT)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 2 */
void EthIf_EnableEgressTimeStamp(uint8 CtrlIdx, Eth_BufIdxType BufIdx)
{
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  Std_ReturnType LddStatusReturn;
  LddStatusReturn = (Std_ReturnType)E_OK;

  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_ENABLEEGRESSTIMESTAMP_SID, ETHIF_E_INV_CTRL_IDX));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_151, SWS ID  SWS_EthIf_00161"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_ENABLEEGRESSTIMESTAMP_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_151, SWS ID  SWS_EthIf_00161"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Get the pointer to the requested Controller's structure */
      #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LpDriverApis = EthIf_GaaStaticPhyController_PB[
      (EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].pEthDriver;
      #else
      LpDriverApis = (EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->pEthDriver;
      #endif
      /* polyspace +7 RTE:UNR [Justified:Low] "This condition will be False when
      LpDriverApis will be NULL.
      Reference SWS ID SWS_EthIf_00161"
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if (LpDriverApis != NULL_PTR)
    {
      /* Invoke respective Eth Drivers' API */
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      (LpDriverApis->pEthEnableEgressTimeStamp)
        (EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].ucCtrlIdx, BufIdx);
      #else
      (LpDriverApis->pEthEnableEgressTimeStamp)
        (((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx), BufIdx);
      #endif
    }
      #else
      /* Invoke respective Eth Drivers' API */
      /* polyspace +4 RTE:IDP [Justified:Low] "This Pointer will not be out of
      bound as it will have the controller index configured with respect to
      number of configured controllers */
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      Eth_EnableEgressTimeStamp(EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].ucCtrlIdx, BufIdx);
      #else
          /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
        EthIf_GpConfigPtr is not a NULL PTR.
        Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_00040a"
        Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      Eth_EnableEgressTimeStamp(((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx), BufIdx);
      #endif
      #endif
  }
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_GetEgressTimeStamp                            **
**                                                                            **
** Service ID           : 0x24                                                **
**                                                                            **
** Description          : This service Reads back the egress time stamp on a  **
**                        dedicated message object.                           **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx, BufIdx                                     **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : timeQualPtr, timeStampPtr                           **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : EthIf_Init should invoked.                          **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstDriverApis        **
**                                                                            **
**                        Function(s) invoked: Eth_GetEgressTimeStamp         **
**                                                                            **
**  Design ID           : ETHIF_SDD_0108                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_GLOBALTIME_SUPPORT)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 5 */
Std_ReturnType  EthIf_GetEgressTimeStamp(
  uint8 CtrlIdx, Eth_BufIdxType BufIdx, TimeStampQualType * timeQualPtr,
  TimeStampType * timeStampPtr)
{
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  Std_ReturnType LddStatusReturn;
  LddStatusReturn = (Std_ReturnType)E_OK;

  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETEGRESSTIMESTAMP_SID, ETHIF_E_INV_CTRL_IDX));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_153, SWS ID SWS_EthIf_00167 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETEGRESSTIMESTAMP_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  #if ((STD_ON == ETHIF_DEV_ERROR_DETECT) || (STD_ON == ETHIF_SAFE_KSAR))
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
  timeQualPtr is NULL_PTR .
  Module Test Case Reference ETHIF_ETC_041, SWS ID SWS_EthIf_00169 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == timeQualPtr)
  {
    #if (STD_ON == ETHIF_SAFE_KSAR)
    (void)(Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETEGRESSTIMESTAMP_SID, ETHIF_E_PARAM_POINTER));
    #else
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_GETEGRESSTIMESTAMP_SID, ETHIF_E_PARAM_POINTER));
      #endif
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
  timeStampPtr is NULL_PTR .
  Module Test Case Reference ETHIF_ETC_041, SWS ID SWS_EthIf_00169 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == timeStampPtr)
  {
    #if (STD_ON == ETHIF_SAFE_KSAR)
    (void)(Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETEGRESSTIMESTAMP_SID, ETHIF_E_PARAM_POINTER));
    #else
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_GETEGRESSTIMESTAMP_SID, ETHIF_E_PARAM_POINTER));
      #endif
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif
  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_153, SWS ID SWS_EthIf_00167 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Get the pointer to the requested Controller's structure */
      #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LpDriverApis = EthIf_GaaStaticPhyController_PB[
      (EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].pEthDriver;
      #else
      LpDriverApis = (EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->pEthDriver;
      #endif
      /* polyspace +7 RTE:UNR [Justified:Low] "This condition will be False when
      LpDriverApis will be NULL.
      Reference SWS ID SWS_EthIf_00167"
       Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      if (LpDriverApis != NULL_PTR)
      {
        /* SWS_EthIf_00166h
         Once EA tool is ready, SWS_ID will be updated with Design_ID*/
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        LddStatusReturn = (LpDriverApis->pEthGetEgressTimeStamp)
          (EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[
            CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].ucCtrlIdx,
             BufIdx, timeQualPtr,
            timeStampPtr);
        #else
        LddStatusReturn = (LpDriverApis->pEthGetEgressTimeStamp)
          (((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx), BufIdx, timeQualPtr, timeStampPtr);
        #endif
      }
        #else
      /* polyspace +7 RTE:IDP [Justified:Low] "Pointer will hold the address
    with respect to number of physical controllers configured" */
        /* SWS_EthIf_00166h
         Once EA tool is ready, SWS_ID will be updated with Design_ID*/
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        LddStatusReturn = Eth_GetEgressTimeStamp
         (EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[CtrlIdx
           ].pPhysCtrl->ucPhysCtrlIdx)].ucCtrlIdx,
               BufIdx, timeQualPtr, timeStampPtr);
        #else
          /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
        EthIf_GpConfigPtr is not a NULL PTR.
        Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_00040a"
        Once EA tool is ready, SWS_ID will be updated with Design_ID*/
        LddStatusReturn = Eth_GetEgressTimeStamp
        (((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx), BufIdx, timeQualPtr, timeStampPtr);
    #endif
    #endif

  }
  return (LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_GetIngressTimeStamp                           **
**                                                                            **
** Service ID           : 0x25                                                **
**                                                                            **
** Description          : This service Reads back the Ingress time stamp on a **
**                        dedicated message object.                           **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx, DataPtr                                    **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : timeQualPtr, timeStampPtr                           **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : EthIf_Init should invoked.                          **
**                                                                            **
** Remarks              : Global Variable(s):  EthIf_GaaStaticController,     **
**                                             EthIf_GpFirstDriverApis        **
**                                                                            **
**                        Function(s) invoked: Eth_GetIngressTimeStamp        **
**                                                                            **
**  Design ID           : ETHIF_SDD_0119                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_GLOBALTIME_SUPPORT)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 5 */
Std_ReturnType EthIf_GetIngressTimeStamp(
  uint8 CtrlIdx, const Eth_DataType * DataPtr,
  TimeStampQualType * timeQualPtr, TimeStampType * timeStampPtr)
{
  #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
  const EthIf_DrvApis*  LpDriverApis;
  #endif
  Std_ReturnType LddStatusReturn;
  LddStatusReturn = (Std_ReturnType)E_OK;

  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETINGRESSTIMESTAMP_SID, ETHIF_E_INV_CTRL_IDX));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  /* polyspace +6 RTE:UNR [Justified:Low] "This condition will be False when
  EthIf module is in INITIALIZED state .
  Module Test Case Reference ETHIF_ETC_155, SWS ID SWS_EthIf_00173 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETINGRESSTIMESTAMP_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  #if ((STD_ON == ETHIF_DEV_ERROR_DETECT) || (STD_ON == ETHIF_SAFE_KSAR))
  /* polyspace +6 RTE:UNR [Justified:Low] "This condition will be true when
  DataPtr is NULL_PTR .
  Module Test Case Reference ETHIF_ETC_042, SWS ID SWS_EthIf_00175 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == DataPtr)
  {
    #if (STD_ON == ETHIF_SAFE_KSAR)
    (void)(Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETINGRESSTIMESTAMP_SID, ETHIF_E_PARAM_POINTER));
    #else
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_GETINGRESSTIMESTAMP_SID, ETHIF_E_PARAM_POINTER));
      #endif
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  /* polyspace +6 RTE:UNR [Justified:Low] "This condition will be true when
  timeQualPtr is NULL_PTR .
  Module Test Case Reference ETHIF_ETC_042, SWS ID SWS_EthIf_00175 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == timeQualPtr)
  {
    #if (STD_ON == ETHIF_SAFE_KSAR)
    (void)(Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETINGRESSTIMESTAMP_SID, ETHIF_E_PARAM_POINTER));
    #else
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_GETINGRESSTIMESTAMP_SID, ETHIF_E_PARAM_POINTER));
      #endif
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  /* polyspace +6 RTE:UNR [Justified:Low] "This condition will be true when
  timeStampPtr is NULL_PTR .
  Module Test Case Reference ETHIF_ETC_042, SWS ID SWS_EthIf_00175 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == timeStampPtr)
  {
    #if (STD_ON == ETHIF_SAFE_KSAR)
    (void)(Det_ReportRuntimeError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETINGRESSTIMESTAMP_SID, ETHIF_E_PARAM_POINTER));
    #else
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_GETINGRESSTIMESTAMP_SID, ETHIF_E_PARAM_POINTER));
      #endif
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif
  /* polyspace +6 RTE:UNR [Justified:Low] "This condition will be true when
  EthIf module is in INITIALIZED state and No DET errors are present.
  Module Test Case Reference ETHIF_ETC_155, SWS ID SWS_EthIf_00173 "
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Get the pointer to the requested Controller's structure */
    /* #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpPhysicalController_PB = &EthIf_GaaStaticPhyController_PB[CtrlIdx];
    #endif */
      #if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LpDriverApis = EthIf_GaaStaticPhyController_PB[
      (EthIf_GaaStaticController_PB[CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].pEthDriver;
      #else
      LpDriverApis = (EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->pEthDriver;
      #endif

      /* polyspace +7 RTE:UNR [Justified:Low] "This condition will be False when
      LpDriverApis will be NULL.
      Reference SWS ID SWS_EthIf_00172h"
       Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      if (LpDriverApis != NULL_PTR)
      {
        /* SWS_EthIf_00172h
        Once EA tool is ready, SWS_ID will be updated with Design_ID*/
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        LddStatusReturn = (LpDriverApis->pEthGetIngressTimeStamp)
          ((EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB
           [CtrlIdx].pPhysCtrl->ucPhysCtrlIdx)].ucCtrlIdx), DataPtr,
            timeQualPtr, timeStampPtr);
        #else
        LddStatusReturn = (LpDriverApis->pEthGetIngressTimeStamp)
          (((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx),
            DataPtr,  timeQualPtr, timeStampPtr);
        #endif
      }
      #else
    /* polyspace +7 RTE:IDP [Justified:Low] "Pointer will hold the address with
    respect to number of physical controllers configured" */
        /* SWS_EthIf_00172h
         Once EA tool is ready, SWS_ID will be updated with Design_ID*/
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        LddStatusReturn = Eth_GetIngressTimeStamp
        ((EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[CtrlIdx
          ].pPhysCtrl->ucPhysCtrlIdx)].ucCtrlIdx), DataPtr, timeQualPtr,
          timeStampPtr);
        #else
          /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be True when
        EthIf_GpConfigPtr is not a NULL PTR.
        Module Test Case Reference ETHIF_ETC_030, SWS ID SWS_EthIf_00040a" */
        LddStatusReturn = Eth_GetIngressTimeStamp
        (((EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx),
          DataPtr, timeQualPtr, timeStampPtr);
    #endif
       #endif

  }
  return (LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_GetBufWRxParams                               **
** Description          : Read out values related to the receive direction of **
**                        the transceiver for a received packet. For example, **
**                        this could be RSSI or Channel belonging to one      **
**                        single packet.                                      **
** Input Parameters     : CtrlIdx , RxParamIds, NumParams                     **
** InOut parameter      : None                                                **
** Output Parameters    : ParamValues                                         **
** Return parameter     : Std_ReturnType                                      **
** Preconditions        : EthIf_Init should invoked.                          **
** Global Variable(s)   : EthIf_GaaStaticController,                          **
**                        EthIf_GpStaticController                            **
** Function(s) invoked  : EthIf_GetBufWRxParams,                              **
**                        WEth_GetBufWRxParams                                **
** Remarks              : None                                                **
** Design ID            : ETHIF_SDD_0710                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_ENABLE_WETH_API)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType  EthIf_GetBufWRxParams(
  uint8 CtrlIdx, const WEth_BufWRxParamIdType * RxParamIds,
  uint32 * ParamValues, uint8 NumParams)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticPhyController_PB * LpPhysicalController_PB;
  #endif
  Std_ReturnType LddStatusReturn;

  LddStatusReturn = (Std_ReturnType)E_OK;
  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETBUFWRXPARAMS_SID, ETHIF_E_UNINIT);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif

  if (ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETBUFWRXPARAMS_SID, ETHIF_E_INV_CTRL_IDX);
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  if (NULL_PTR == RxParamIds)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETBUFWRXPARAMS_SID, ETHIF_E_PARAM_POINTER);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  if (NULL_PTR == ParamValues)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETBUFWRXPARAMS_SID, ETHIF_E_PARAM_POINTER);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Get the pointer to the requested Controller's structure */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpPhysicalController_PB = &EthIf_GaaStaticPhyController_PB[CtrlIdx];
    #endif
    /* Invoke Eth transceiver API.
    The function EthIf_GetBufWRxParams shall forward the
    call to function WEth_GetBufWRxParams of the respective Ethernet
    Transceiver Driver */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (LpPhysicalController_PB->ucWEthCtrlIdx != ETHIF_INVALID)
    {
      LddStatusReturn = WEth_GetBufWRxParams
        (LpPhysicalController_PB->ucWEthCtrlIdx, RxParamIds,
          ParamValues, NumParams);
    }
    #else
    if (EthIf_GpConfigPtr->pStaticPhyController[CtrlIdx].ucWEthCtrlIdx != ETHIF_INVALID)
    {
      LddStatusReturn = WEth_GetBufWRxParams
        (EthIf_GpConfigPtr->pStaticPhyController[CtrlIdx].ucWEthCtrlIdx, RxParamIds,
          ParamValues, NumParams);
    }
    #endif
    else
    {
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
  }

  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_GetBufWTxParams                               **
** Description          : Read out values related to the transmit direction of**
**                        the transceiver for a transmitted packet.For example**
**                        this could be transaction ID belonging to one single**
**                        packet.                                             **
** Input Parameters     : CtrlIdx , TxParamIds, NumParams                     **
** InOut parameter      : None                                                **
** Output Parameters    : ParamValues                                         **
** Return parameter     : Std_ReturnType                                      **
** Preconditions        : EthIf_Init should invoked.                          **
** Global Variable(s)   : EthIf_GaaStaticController,                          **
**                        EthIf_GpStaticController                            **
** Function(s) invoked  : EthIf_GetBufWRxParams                               **
** Remarks              : None                                                **
** Design ID            : ETHIF_SDD_0711                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_ENABLE_WETH_API)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType  EthIf_GetBufWTxParams(
  uint8 CtrlIdx, const WEth_BufWTxParamIdType * TxParamIds,
  uint32 * ParamValues, uint8 NumParams)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticPhyController_PB*
    LpPhysicalController_PB;
  #endif
  Std_ReturnType LddStatusReturn;

  LddStatusReturn = (Std_ReturnType)E_OK;
  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETBUFWTXPARAMS_SID, ETHIF_E_UNINIT);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif

  if (ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETBUFWTXPARAMS_SID, ETHIF_E_INV_CTRL_IDX);
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  if (NULL_PTR == TxParamIds)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETBUFWTXPARAMS_SID, ETHIF_E_PARAM_POINTER);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if (NULL_PTR == ParamValues)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETBUFWTXPARAMS_SID, ETHIF_E_PARAM_POINTER);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Get the pointer to the requested Controller's structure */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpPhysicalController_PB = &EthIf_GaaStaticPhyController_PB[CtrlIdx];
    #endif
    /*Invoke Eth transceiver API.
    The function EthIf_GetBufWTxParams shall forward the
    call to function WEth_GetBufWTxParams of the respective Ethernet
    Transceiver Driver */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (LpPhysicalController_PB->ucWEthCtrlIdx != ETHIF_INVALID)
    {
      LddStatusReturn = WEth_GetBufWTxParams
        (LpPhysicalController_PB->ucWEthCtrlIdx, TxParamIds, ParamValues,
          NumParams);
    }
    #else
    if (EthIf_GpConfigPtr->pStaticPhyController[CtrlIdx].ucWEthCtrlIdx != ETHIF_INVALID)
    {
      LddStatusReturn = WEth_GetBufWTxParams
        (EthIf_GpConfigPtr->pStaticPhyController[CtrlIdx].ucWEthCtrlIdx, TxParamIds, ParamValues,
          NumParams);
    }
    #endif
    else
    {
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }


  }
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_SetBufWTxParams                               **
** Description          : Set values related to the transmit direction of the **
**                        transceiver for a specific buffer(packet to be sent)**
**                        For example, this can be the desired transmit power **
**                        or the channel belonging to one single packet.      **
** Input Parameters     : CtrlIdx , TxParamIds, BufIdx , ParamValues,         **
**                        NumParams                                           **
** InOut parameter      : None                                                **
** Output Parameters    : ParamValues                                         **
** Return parameter     : Std_ReturnType                                      **
** Preconditions        : EthIf_Init should invoked.                          **
** Global Variable(s)   : EthIf_GaaStaticController,                          **
**                        EthIf_GpStaticController                            **
** Function(s) invoked  : EthIf_SetBufWTxParams                               **
**                        WEth_SetBufWTxParams                                **
** Remarks              : None                                                **
** Design ID            :ETHIF_SDD_0712                                       **
*******************************************************************************/
#if (STD_ON == ETHIF_ENABLE_WETH_API)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType  EthIf_SetBufWTxParams(
  uint8 CtrlIdx, Eth_BufIdxType BufIdx,
  const WEth_BufWTxParamIdType * TxParamIds,
  const uint32 * ParamValues, uint8 NumParams)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticPhyController_PB*
    LpPhysicalController_PB;
  #endif

  Std_ReturnType LddStatusReturn;

  LddStatusReturn = (Std_ReturnType)E_OK;
  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_SETBUFWTXPARAMS_SID, ETHIF_E_UNINIT);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif

  if (ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_SETBUFWTXPARAMS_SID, ETHIF_E_INV_CTRL_IDX);
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if (ETHIF_INVALID_32 == BufIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_SETBUFWTXPARAMS_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  if (NULL_PTR == TxParamIds)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_SETBUFWTXPARAMS_SID, ETHIF_E_PARAM_POINTER);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if (NULL_PTR == ParamValues)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_SETBUFWTXPARAMS_SID, ETHIF_E_PARAM_POINTER);

    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Get the pointer to the requested Controller's structure */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpPhysicalController_PB = &EthIf_GaaStaticPhyController_PB[CtrlIdx];
    #endif
    /*
    Invoke Eth transceiver API.
    The function EthIf_SetBufWTxParams shall forward the
    call to function WEth_SetBufWTxParams of the respective Ethernet
    Transceiver Driver.
    */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (LpPhysicalController_PB->ucWEthCtrlIdx != ETHIF_INVALID)
    {
      LddStatusReturn = WEth_SetBufWTxParams
        (LpPhysicalController_PB->ucWEthCtrlIdx, BufIdx, TxParamIds,
           ParamValues, NumParams);
    }
    #else
    if (EthIf_GpConfigPtr->pStaticPhyController[CtrlIdx].ucWEthCtrlIdx != ETHIF_INVALID)
    {
      LddStatusReturn = WEth_SetBufWTxParams
        (EthIf_GpConfigPtr->pStaticPhyController[CtrlIdx].ucWEthCtrlIdx, BufIdx, TxParamIds,
           ParamValues, NumParams);
    }
    #endif
    else
    {
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
  }
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_SetRadioParams                                **
** Description          : Set values related to a transceiver's wireless      **
**                        radio For example, this could be the selection of   **
**                        the radio settings (channel, ...).                  **
** Input Parameters     : TrcvId , ParamIds, ParamValue , NumParams           **
** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType                                      **
** Preconditions        : EthIf_Init should invoked.                          **
** Global Variable(s)   : EthIf_GaaStaticTransceiver,                         **
**                        EthIf_GpStaticTransceiver                           **
** Function(s) invoked  : EthIf_SetRadioParams                                **
**                        WEthTrcv_SetRadioParams                             **
** Remarks              : None                                                **
** Design ID            : ETHIF_SDD_0713                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_ENABLE_WETH_API)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_SetRadioParams(
  uint8 TrcvIdx, const WEthTrcv_SetRadioParamIdType * ParamIds,
  const uint32 * ParamValues, uint8 NumParams)

{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticTransceiver_PB*  LpStaticTransceiver_PB;
  #endif
  Std_ReturnType LddStatusReturn;

  LddStatusReturn = (Std_ReturnType)E_OK;

  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                  ETHIF_SETRADIOPARAMS_SID, ETHIF_E_UNINIT);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif

  if (ETHIF_NUM_OF_TRANSCEIVERS  <= TrcvIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_SETRADIOPARAMS_SID, ETHIF_E_INV_TRCV_IDX);
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  if (NULL_PTR == ParamIds)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_SETRADIOPARAMS_SID, ETHIF_E_PARAM_POINTER);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if (NULL_PTR == ParamValues)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_SETRADIOPARAMS_SID, ETHIF_E_PARAM_POINTER);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /*Get the pointer to the requested Controller's structure */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpStaticTransceiver_PB = &EthIf_GaaStaticTransceiver_PB[TrcvIdx];
    #endif
    /*
    Invoke Eth transceiver API.
    The function EthIf_SetRadioParams shall forward the
    call to function WEthTrcv_SetRadioParams of the respective Ethernet
    Transceiver Driver.
    */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (LpStaticTransceiver_PB->ucWEthTrcvIdx != ETHIF_INVALID)
    {
      LddStatusReturn = WEthTrcv_SetRadioParams
        (LpStaticTransceiver_PB->ucWEthTrcvIdx, ParamIds, ParamValues, NumParams);
    }
    #else
    if (EthIf_GpConfigPtr->pTransceiver[TrcvIdx].ucWEthTrcvIdx != ETHIF_INVALID)
    {
      LddStatusReturn = WEthTrcv_SetRadioParams
        (EthIf_GpConfigPtr->pTransceiver[TrcvIdx].ucWEthTrcvIdx, ParamIds, ParamValues, NumParams);
    }
    #endif
    else
    {
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
  } /* End of if ((Std_ReturnType)E_OK == LddStatusReturn) */
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif
/*******************************************************************************
** Function Name        : EthIf_SetChanRxParams                               **
** Description          : Set values related to the receive direction of a    **
**                        transceiver's wireless channel. For example, this   **
**                        could be a channel parameter like the frequency.    **
** Input Parameters     : TrcvId ,RadioId, ParamIds, ParamValue , NumParams   **
** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType                                      **
** Preconditions        : EthIf_Init should invoked.                          **
** Global Variable(s)   : EthIf_GaaStaticTransceiver,                         **
**                        EthIf_GpStaticTransceiver                           **
** Function(s) invoked  : EthIf_SetChanRxParams                               **
**                        WEthTrcv_SetChanRxParams                            **
** Remarks              : None                                                **
** Design ID            : ETHIF_SDD_0714                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_ENABLE_WETH_API)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType  EthIf_SetChanRxParams(
 uint8 TrcvIdx, uint8 RadioId, const WEthTrcv_SetChanRxParamIdType * RxParamIds,
 const uint32 * ParamValues, uint8 NumParams)

{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticTransceiver_PB * LpStaticTransceiver_PB;
  #endif
  Std_ReturnType LddStatusReturn;

  LddStatusReturn = (Std_ReturnType)E_OK;
  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    /* If DET error check is ON */
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                  ETHIF_SETCHANRXPARAMS_SID, ETHIF_E_UNINIT);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif

  if (ETHIF_NUM_OF_TRANSCEIVERS  <= TrcvIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_SETCHANRXPARAMS_SID, ETHIF_E_INV_TRCV_IDX);
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if (ETHIF_INVALID == RadioId)
  {
  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_SETCHANRXPARAMS_SID, ETHIF_E_INV_PARAM);
  #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  if (NULL_PTR == RxParamIds)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_SETCHANRXPARAMS_SID, ETHIF_E_PARAM_POINTER);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  if (NULL_PTR == ParamValues)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_SETCHANRXPARAMS_SID, ETHIF_E_PARAM_POINTER);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif


  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Get the pointer to the requested Controller's structure */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpStaticTransceiver_PB = &EthIf_GaaStaticTransceiver_PB[TrcvIdx];
    #endif
    /* Invoke Eth transceiver API.
    The function EthIf_SetChanRxParams shall forward the
    call to function WEthTrcv_SetChanRxParams of the respective Ethernet
    Transceiver Driver */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (LpStaticTransceiver_PB->ucWEthTrcvIdx != ETHIF_INVALID)
    {
      LddStatusReturn = WEthTrcv_SetChanRxParams(
        LpStaticTransceiver_PB->ucWEthTrcvIdx, RadioId,
          RxParamIds, ParamValues, NumParams);
    }
    #else
    if (EthIf_GpConfigPtr->pTransceiver[TrcvIdx].ucWEthTrcvIdx != ETHIF_INVALID)
    {
      LddStatusReturn = WEthTrcv_SetChanRxParams(
        EthIf_GpConfigPtr->pTransceiver[TrcvIdx].ucWEthTrcvIdx, RadioId,
          RxParamIds, ParamValues, NumParams);
    }
    #endif
    else
    {
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
  }
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif
/*******************************************************************************
** Function Name        : EthIf_SetChanTxParams                               **
** Description          : Set values related to the transmit direction of a   **
**                        transceiver's wireless channel. For example, this   **
**                        could be bitrate of a channel.                      **
** Input Parameters     : TrcvIdx ,RadioIdx, ParamIds, ParamValue , NumParams **
** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType                                      **
** Preconditions        : EthIf_Init should invoked.                          **
** Global Variable(s)   : EthIf_GaaStaticTransceiver,                         **
**                        EthIf_GpStaticTransceiver                           **
** Function(s) invoked  : EthIf_SetChanTxParams                               **
**                        WEthTrcv_SetChanTxParams                            **
** Remarks              : None                                                **
** Design ID            : ETHIF_SDD_0715                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_ENABLE_WETH_API)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType  EthIf_SetChanTxParams(
  uint8 TrcvIdx, uint8 RadioId,
  const WEthTrcv_SetChanTxParamIdType * TxParamIds,
  const uint32 *  ParamValues, uint8 NumParams)

{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticTransceiver_PB*
    LpStaticTransceiver_PB;
  #endif
  Std_ReturnType LddStatusReturn;

  LddStatusReturn = (Std_ReturnType)E_OK;
  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                  ETHIF_SETCHANTXPARAMS_SID, ETHIF_E_UNINIT);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif

  if (ETHIF_NUM_OF_TRANSCEIVERS  <= TrcvIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_SETCHANTXPARAMS_SID, ETHIF_E_INV_TRCV_IDX);
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if (ETHIF_INVALID == RadioId)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_SETCHANTXPARAMS_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  if (NULL_PTR == TxParamIds)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_SETCHANTXPARAMS_SID, ETHIF_E_PARAM_POINTER);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if (NULL_PTR == ParamValues)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_SETCHANTXPARAMS_SID, ETHIF_E_PARAM_POINTER);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif


  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Get the pointer to the requested Controller's structure */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpStaticTransceiver_PB = &EthIf_GaaStaticTransceiver_PB[TrcvIdx];
    #endif
    /* Invoke Eth transceiver API.
    The function EthIf_SetChanTxParams shall forward the
    call to function WEthTrcv_SetChanTxParams of the respective Ethernet
    Transceiver Driver */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (LpStaticTransceiver_PB->ucWEthTrcvIdx != ETHIF_INVALID)
    {
      LddStatusReturn = WEthTrcv_SetChanTxParams
        (LpStaticTransceiver_PB->ucWEthTrcvIdx, RadioId,
          TxParamIds, ParamValues, NumParams);
    }
    #else
    if (EthIf_GpConfigPtr->pTransceiver[TrcvIdx].ucWEthTrcvIdx != ETHIF_INVALID)
    {
      LddStatusReturn = WEthTrcv_SetChanTxParams
        (EthIf_GpConfigPtr->pTransceiver[TrcvIdx].ucWEthTrcvIdx, RadioId,
          TxParamIds, ParamValues, NumParams);
    }
    #endif
    else
    {
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
  }
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_GetChanRxParams                               **
** Description          : Read values related to the receive direction of the **
**                        transceiver. For example, this could be a Channel   **
**                       Busy Ratio(CBR) or the average Channel Idle Time(CIT)**
** Input Parameters     : TrcvId ,RadioId, ParamIds, ParamValue , NumParams   **
** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType                                      **
** Preconditions        : EthIf_Init should invoked.                          **
** Global Variable(s)   : EthIf_GaaStaticTransceiver,                         **
**                        EthIf_GpStaticTransceiver                           **
** Function(s) invoked  : EthIf_SetChanTxParams                               **
**                        WEthTrcv_SetChanTxParams                            **
** Remarks              : None                                                **
** Design ID            : ETHIF_SDD_0716                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_ENABLE_WETH_API)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType  EthIf_GetChanRxParams(
  uint8 TrcvIdx, uint8 RadioId,
  const WEthTrcv_GetChanRxParamIdType * RxParamIds,
  uint32 * ParamValues, uint8 NumParams)

{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticTransceiver_PB*
    LpStaticTransceiver_PB;
  #endif
  Std_ReturnType LddStatusReturn;
  uint8 LucWEthTrcvIdx;

  LddStatusReturn = (Std_ReturnType)E_OK;
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                  ETHIF_GETCHANRXPARAMS_SID, ETHIF_E_UNINIT);
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if (ETHIF_NUM_OF_TRANSCEIVERS  <= TrcvIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_GETCHANRXPARAMS_SID, ETHIF_E_INV_TRCV_IDX);
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if (ETHIF_INVALID == RadioId)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_GETCHANRXPARAMS_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  if (NULL_PTR == RxParamIds)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_GETCHANRXPARAMS_SID, ETHIF_E_PARAM_POINTER);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if (NULL_PTR == ParamValues)
  {
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                     ETHIF_GETCHANRXPARAMS_SID, ETHIF_E_PARAM_POINTER);
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  #endif


  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpStaticTransceiver_PB = &EthIf_GaaStaticTransceiver_PB[TrcvIdx];
    LucWEthTrcvIdx = LpStaticTransceiver_PB->ucWEthTrcvIdx;
    #else
    LucWEthTrcvIdx = EthIf_GpConfigPtr->pTransceiver[TrcvIdx].ucWEthTrcvIdx;
    #endif
    /* Invoke Eth transceiver API.
    The function EthIf_GetChanRxParams shall forward the
    call to function WEthTrcv_GetChanRxParams of the respective Ethernet
    Transceiver Driver */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (LpStaticTransceiver_PB->ucWEthTrcvIdx != ETHIF_INVALID)
    #else
    if (EthIf_GpConfigPtr->pTransceiver[TrcvIdx].ucWEthTrcvIdx != ETHIF_INVALID)
    #endif
    {
      LddStatusReturn = WEthTrcv_GetChanRxParams(&LucWEthTrcvIdx, RadioId,
        RxParamIds, ParamValues, NumParams);
    }
    else
    {
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
  }
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_RxIndication_SecurityVlanCheck                **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : VLan secuirty check for EthIf_RxIndication          **
**                                                                            **
** Sync/Async           : NA                                                  **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : VLANId , DataPtr                                    **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s): None                            **
**                                                                            **
**  Design ID           : ETHIF_SDD_0156                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_VLAN_SUPPORT)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static void  EthIf_RxIndication_SecurityVlanCheck(uint16 VlanId)
{
  boolean LblValidVlanId;
  uint8 LucLoopCount;

  LblValidVlanId = FALSE;

  for (LucLoopCount = (uint8)ETHIF_ZERO; ((LucLoopCount <
  (uint8)ETHIF_NUM_OF_CONTROLLERS) && (!(LblValidVlanId))); LucLoopCount++)
  {
    /* checking controller index with respect to configured Ethernet controllers */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (VlanId == (uint16)(EthIf_GaaStaticController_PB[LucLoopCount].
              usEthIfVlanTagLsb & (uint16)ETHIF_VLANID_CHECK))
    #else
    if (VlanId == (uint16)(EthIf_GpConfigPtr->pController[LucLoopCount].ucVlanId
        & (uint16)ETHIF_VLANID_CHECK))
    #endif
    {
      LblValidVlanId = TRUE;
    }
  }

  /* This condition shall be True when  controller index is invalid. */
  if (!(LblValidVlanId))
  {
    /* SWS_EthIf_00502, SWS_EthIf_00503 */
    #if (STD_ON == ETHIF_SECURITY_EVENT_ENABLED)
      /* SWS_EthIf_00503:EthIf module won't provide Context data and TimeStamp
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
     IdsM_ReportSecurityEvent(ETHIF_SEV_ETH_DROP_INV_VLAN, NULL_PTR,
      ETHIF_DEFAULT_CONTEXT_DATA_SIZE, ETHIF_DEFAULT_CONTEXT_DATA_VERSION,
      ETHIF_DEFAULT_COUNT, NULL_PTR);
    #endif
  }
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif
/*******************************************************************************
** Function Name        : EthIf_RxIndication_DetCheck                         **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : Det check for EthIf_RxIndication                    **
**                                                                            **
** Sync/Async           : NA                                                  **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx , DataPtr                                   **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s): None                            **
**                                                                            **
**                        Function(s) invoked:                                **
**                        Det_ReportError                                     **
**                                                                            **
** Design ID            : ETHIF_SDD_0157                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 3 */
static Std_ReturnType EthIf_RxIndication_DetCheck(
  uint8 CtrlIdx, const Eth_DataType *  DataPtr)
{
  boolean LblValidCtrlIdx;
  Std_ReturnType LddStatusReturn;
  uint8 LucLoopCount;

  LddStatusReturn = E_OK;
  LblValidCtrlIdx = FALSE;

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf module is in INITIALIZED state.
  Module Test Case Reference ETHIF_ETC_027, SWS ID SWS_EthIf_00086"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_RXINDICATION_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  else
  {
    for (LucLoopCount = (uint8)ETHIF_ZERO; ((LucLoopCount <
      (uint8)ETHIF_NUM_OF_PHYSICAL_CONTROLLERS) && (!(LblValidCtrlIdx)));
      LucLoopCount++)
    {
      /* polyspace +4 RTE:OBAI [Justified:Low] "This Pointer will not be out of
      bound as it will have the controller index configured with respect to
      number of configured controllers */
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      if (EthIf_GaaStaticPhyController_PB[LucLoopCount].ucCtrlIdx == CtrlIdx)
      #else
      if (EthIf_GpConfigPtr->pStaticPhyController[LucLoopCount].ucCtrlIdx ==
        CtrlIdx)
      #endif
      {
        LblValidCtrlIdx = TRUE;
      }
      /*Violates ref EthIf_c_QAC_REF_2, INT30 (Cert-C) */
    }
  }

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be True when
  controller index is valid. */
  if (!(LblValidCtrlIdx))
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_RXINDICATION_SID, ETHIF_E_INV_CTRL_IDX));
    #endif

    /* SWS_EthIf_00502, SWS_EthIf_00503
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    #if (STD_ON == ETHIF_SECURITY_EVENT_ENABLED)
      /* SWS_EthIf_00503:EthIf module won't provide Context data and TimeStamp
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
     IdsM_ReportSecurityEvent(ETHIF_SEV_ETH_DROP_INV_VLAN, NULL_PTR,
      ETHIF_DEFAULT_CONTEXT_DATA_SIZE, ETHIF_DEFAULT_CONTEXT_DATA_VERSION,
      ETHIF_DEFAULT_COUNT, NULL_PTR);
    #endif

    #if (STD_ON == ETHIF_MEASUREMENT_DATA_API)
    /* SWS_EthIf_00313
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/

    if (ETHIF_INVALID_32 != EthIf_GulVlanDiagMeasData)
    {
      EthIf_GulVlanDiagMeasData += (uint32)ETHIF_ONE;
    }
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
  DataPtr will not be Null.
  Module Test Case Reference ETHIF_ETC_050, SWS ID SWS_EthIf_00088"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (NULL_PTR == DataPtr)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_RXINDICATION_SID, ETHIF_E_PARAM_POINTER));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
/*******************************************************************************
** Function Name        : EthIf_RxIndicationLinearSearch                      **
**                                                                            **
** Service ID           : 0x10                                                **
**                                                                            **
** Description          : This API searches for the frame type to identify    **
**                        the upper layer Rx Indication function              **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : FrameType,LpRxIndex                                 **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)    :                             **
**                                               EthIf_GpFrameTypeToRxIndFunc **
**                                               EthIf_GaaFrameTypeToRxIndFunc**
**                        Function(s) invoked   :                             **
**                                  External    :                             **
**                                  Internal    :                             **
**                                                                            **
**  Design ID           : ETHIF_SDD_0155                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 2 */
static boolean EthIf_RxIndicationLinearSearch(
  Eth_FrameType FrameType,
  #if ((ETHIF_NO_OF_RXPDUS > ETHIF_ZERO) && \
       (ETHIF_PRE_COMPILE_SINGLE == STD_ON))
  uint8 PhyCtrlIdx, uint8 * LpRxPduIndex,
  #endif
  uint8 * LpRxIndex)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_FrameTypeToRxIndFunc_PB*
    LpFrameTypeToRxIndFunc_PB;
  #endif
  boolean LblRetVal;
  uint8 LucCount = ETHIF_ZERO;

  #if ((ETHIF_NO_OF_RXPDUS > ETHIF_ZERO) && \
       (ETHIF_PRE_COMPILE_SINGLE == STD_ON))
  const EthIf_RxPoolType * LpRxPool;
  EthIf_RxPduStatusType * LpRxPduStat;
  uint8 LucRxPduIndex;
  boolean LblRxPoolFound;
  uint8 LucRxPoolCount;
  *LpRxPduIndex = ETHIF_INVALID;
  #endif
  LblRetVal = ETHIF_FALSE;

  /* Loop for all the frmetypes until the frametype matches */
  do
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    LpFrameTypeToRxIndFunc_PB = &EthIf_GaaFrameTypeToRxIndFunc_PB[LucCount];
    #endif

    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (FrameType == LpFrameTypeToRxIndFunc_PB->usEthIfFrameType)

    #else
    if (FrameType == EthIf_GpConfigPtr->pFrameTypeToRxIndFunc[LucCount].usEthIfFrameType)
    #endif
    {
      /* Copy the Owner value once the FrameType Matches */
      #if (ETHIF_NO_OF_RXPDUS > ETHIF_ZERO)
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      /* Fixed RxPools Handelling*/
      LblRxPoolFound = ETHIF_FALSE;
      LucRxPoolCount = ETHIF_ZERO;
      LpRxPool = &EthIf_GaaRxPools[LpFrameTypeToRxIndFunc_PB->ucRxFixedPoolIndex];
      while ((LblRxPoolFound == ETHIF_FALSE) &&
             (LucRxPoolCount < LpFrameTypeToRxIndFunc_PB->ucRxFixedPoolSize))
      {
        LpRxPool += LucRxPoolCount;
        if (LpRxPool->ucPhyCtrlIdx == PhyCtrlIdx)
        {
          for (LucRxPduIndex = ETHIF_ZERO; LucRxPduIndex < LpRxPool->ucPdusize;
               LucRxPduIndex++)
          {
            LpRxPduStat = &EthIf_GaaRxPduStat[LpRxPool->ucPduIDsIndex] +
              LucRxPduIndex;
            if (ETHIF_PDU_AVAILABLE ==  LpRxPduStat->PduState)
            {
              *LpRxPduIndex = LpRxPool->ucPduIDsIndex + LucRxPduIndex;
              LucRxPduIndex = LpRxPool->ucPdusize;
              LblRxPoolFound = ETHIF_TRUE;
            }
          }
        }
        LucRxPoolCount++;
      }
      /* Floating RxPools Handelling*/
      LucRxPoolCount = ETHIF_ZERO;
      LpRxPool = &EthIf_GaaRxPools[LpFrameTypeToRxIndFunc_PB->ucRxFloatPoolIndex];
      while ((LblRxPoolFound == ETHIF_FALSE) &&
             (LucRxPoolCount < LpFrameTypeToRxIndFunc_PB->ucRxFloatPoolSize))
      {
        LpRxPool += LucRxPoolCount;
        for (LucRxPduIndex = ETHIF_ZERO; LucRxPduIndex < LpRxPool->ucPdusize;
             LucRxPduIndex++)
        {
          LpRxPduStat = &EthIf_GaaRxPduStat[LpRxPool->ucPduIDsIndex] +
            LucRxPduIndex;
          if (ETHIF_PDU_AVAILABLE ==  LpRxPduStat->PduState)
          {
            *LpRxPduIndex = LpRxPool->ucPduIDsIndex + LucRxPduIndex;
            LucRxPduIndex = LpRxPool->ucPdusize;
            LblRxPoolFound = ETHIF_TRUE;
          }
        }
        LucRxPoolCount++;
      }
      #endif
      #endif
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      *LpRxIndex = LpFrameTypeToRxIndFunc_PB->ucRxIndex;
      #else
      *LpRxIndex = EthIf_GpConfigPtr->pFrameTypeToRxIndFunc[LucCount].ucRxIndex;
      #endif
      LucCount = ETHIF_NUM_FRAME_OWNER;
      LblRetVal = (boolean)ETHIF_ONE;
    }
    /*Violates ref EthIf_c_QAC_REF_2, INT30 (Cert-C) */

    else
    {
      LucCount++;
    }
  }while (LucCount < ETHIF_NUM_FRAME_OWNER);
  return (LblRetVal);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_RxIndication                                  **
**                                                                            **
** Service ID           : 0x10                                                **
**                                                                            **
** Description          : This API of the EthIf module handles a received     **
**                        frame received by the indexed controller            **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx,DataPtr,LenByte                             **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)    :                             **
**                                               EthIf_GddInit                **
**                                               EthIf_GpRxIndicationFunc     **
**                                               EthIf_GaaRxIndicationFunc_PB **
**                                                                            **
**                        Function(s) invoked   :                             **
**                                  External    : EthIf_LinearSearch          **
**                                                <UL>_RxIndication           **
**                                  Internal    : None                        **
**                                                                            **
** Design ID            : ETHIF_SDD_0086                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
void EthIf_RxIndication (uint8 CtrlIdx,
  Eth_FrameType FrameType, boolean IsBroadcast,
  const uint8 * PhysAddrPtr, const Eth_DataType * DataPtr, uint16 LenByte
  #ifdef ETHIF_RXINDICATION_INGRESSTIME_SUPPORT
  , const TimeTupleType* IngressTimeTuplePtr, Eth_BufIdxType RxHandleId
  #endif
  )
{
  const uint8* LpDataPtr;
  #if ((ETHIF_NO_OF_RXPDUS > ETHIF_ZERO) && \
       (ETHIF_PRE_COMPILE_SINGLE == STD_ON))
  EthIf_RxPduStatusType * LpRxPduStat;
  const EthIf_RxPduType * LpRxPdu;
  PduInfoType LddPduInfo;
  uint8 LucRxPduIndex;
  #ifdef ETHIF_IMMEDIATE_TRANSMIT_SUPPORT
  const EthIf_RxPoolType * LpRxPool;
  uint8 LucRxPoolIndex;
  #endif
  #endif
  const EthIf_RxIndFuncPtr * LpRxIndicationFunc;
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController_PB* LpController;
  #else
  const EthIf_StaticPhyController* LpPhyscontroller;
  #endif
  uint8 LucPhysCtrlIdx;

  boolean LblRetVal;
  boolean LblCtrlFound;
  Std_ReturnType LddStatusReturn;
  Eth_FrameType LddFrameType;
  uint8 LucRxIndex;
  uint8 LucEthIfCtrlIdx;
  uint8 LucLoopCount;
  uint16 LusLength;
  #if (STD_ON == ETHIF_SECURITY_EVENT_ENABLED)

  uint8 LaaPhysAddrPtr[(uint8)ETHIF_SIX] = {ETHIF_ZERO,ETHIF_ZERO,ETHIF_ZERO,
  ETHIF_ZERO,ETHIF_ZERO,ETHIF_ZERO};
  uint8 LucMACAddrLoopCount;
  boolean LblMACAddr;
  #endif
  /* Added Vlan Tag Implementation */
  #if (STD_ON == ETHIF_VLAN_SUPPORT)
  uint8 LucVlanFlag;
  uint16 LusVlanId;
  #endif

  LucRxIndex = (uint8)ETHIF_ZERO;
  LblCtrlFound = FALSE;
  LblRetVal = FALSE;
  LucEthIfCtrlIdx = (uint8)ETHIF_ZERO;

  LddStatusReturn = EthIf_RxIndication_DetCheck(CtrlIdx, DataPtr);

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf_RxIndication_DetCheck is not being invoked.
  Module Test Case Reference test_9" */
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* Added Vlan Tag Implementation as per CR10 */
    LusLength = LenByte;
    LddFrameType = FrameType;
    /* Updated for Redmine #53417 */
    /* PRQA S 0751 1 */
    LpDataPtr = (const uint8*)DataPtr;
    LucLoopCount = ETHIF_ZERO;
    while (LucLoopCount < (uint8)ETHIF_NUM_OF_CONTROLLERS)
    {
      /* polyspace +4 RTE:OBAI [Justified:Low] "This Pointer will not be out of
      bound as it will have the controller index configured with respect to
      number of configured controllers */
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LpController = &EthIf_GaaStaticController_PB[LucLoopCount];
      LucPhysCtrlIdx = LpController->pPhysCtrl->ucPhysCtrlIdx;

      if (EthIf_GaaStaticPhyController_PB[LucPhysCtrlIdx].ucCtrlIdx == CtrlIdx)
      #else
      {
        LpPhyscontroller = EthIf_GpConfigPtr->pController[LucLoopCount].pPhysCtrl;
        LucPhysCtrlIdx = LpPhyscontroller->ucPhysController;
      }
      if (EthIf_GpConfigPtr->pStaticPhyController[LucPhysCtrlIdx].ucCtrlIdx
        == CtrlIdx)
      #endif
      {
        #if (STD_ON == ETHIF_SECURITY_EVENT_ENABLED)
        LblMACAddr = (boolean)ETHIF_FALSE;

        EthIf_GetPhysAddr(CtrlIdx, &LaaPhysAddrPtr[0]);

        for (LucMACAddrLoopCount = (uint8)ETHIF_ZERO; LucMACAddrLoopCount <
          (uint8)ETHIF_SIX; LucMACAddrLoopCount++)
        {
          if (PhysAddrPtr[LucMACAddrLoopCount] !=
            LaaPhysAddrPtr[LucMACAddrLoopCount])
          {
            LblMACAddr = (boolean)ETHIF_TRUE;
          }
        }
        /* SWS_EthIf_00502, SWS_EthIf_00503
        Once EA tool is ready, SWS_ID will be updated with Design_ID*/
        if (!(LblMACAddr))
        {
          /* SWS_EthIf_00503:EthIf module won't provide Context data and TimeStamp
          Once EA tool is ready, SWS_ID will be updated with Design_ID*/
          IdsM_ReportSecurityEvent(ETHIF_SEV_ETH_DROP_MAC_COLLISION,  NULL_PTR,
            ETHIF_DEFAULT_CONTEXT_DATA_SIZE, ETHIF_DEFAULT_CONTEXT_DATA_VERSION,
            ETHIF_DEFAULT_COUNT, NULL_PTR);

        }
        #endif

        #if (STD_ON == ETHIF_VLAN_SUPPORT)
        if (FrameType == (Eth_FrameType)ETHIF_VLAN_FRAMEID)
        {

          LusVlanId = (uint16)(((uint16)LpDataPtr[ETHIF_ZERO]
            & (uint16)ETHIF_VLANID_MASK) << ETHIF_EIGHT);

          LusVlanId = LusVlanId | (uint16)(LpDataPtr[ETHIF_ONE]);

          #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
          LucVlanFlag = (uint8)(EthIf_GaaStaticController_PB[LucLoopCount].
            usEthIfVlanTagLsb >> ETHIF_TWELVE);

          #else
          LucVlanFlag = (uint8)(EthIf_GpConfigPtr->pController[LucLoopCount].
            ucVlanId >> ETHIF_TWELVE);
          #endif

          /* Validating  VLANId */
          EthIf_RxIndication_SecurityVlanCheck(LusVlanId);

          /*Check if vlan id is present or not*/
          if ((uint8)ETHIF_EIGHT == (LucVlanFlag & (uint8)ETHIF_EIGHT))
          {

            #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
            if (LusVlanId == (uint16)(EthIf_GaaStaticController_PB[LucLoopCount].
              usEthIfVlanTagLsb & (uint16)ETHIF_VLANID_CHECK))
            #else

            if (LusVlanId == (uint16)(EthIf_GpConfigPtr->pController[LucLoopCount].
              ucVlanId & (uint16)ETHIF_VLANID_CHECK))
            #endif
            {

              LddFrameType = (uint16)((uint16)LpDataPtr[ETHIF_TWO]
                << ETHIF_EIGHT);

              LddFrameType = LddFrameType | (uint16)(LpDataPtr[ETHIF_THREE]);

              LusLength = LenByte - (uint16)ETHIF_FOUR;
              LblCtrlFound = TRUE;
              /*Found the required vlanid */
              LucEthIfCtrlIdx = LucLoopCount;
              /* #violates #ref EthIf_Cbk_c_QAC_REF_3 Violates MISRA 2012
                Required Rule 11.3*/

              LpDataPtr++;
              LpDataPtr++;
              LpDataPtr++;
              LpDataPtr++;
              LucLoopCount = ETHIF_NUM_OF_CONTROLLERS;
            }
            /*Violates ref EthIf_c_QAC_REF_2, INT30 (Cert-C) */

            else
            {
              LucLoopCount++;
            }
          }
          /* polyspace +7 RTE:UNR [Justified:Low] "This condition shall be True
          when vlan id is not present.
          Module Test Case Reference test_9" */
          /*Violates ref EthIf_c_QAC_REF_2, INT30 (Cert-C) */

          else
          {
            LucLoopCount++;
          }
        }/*end of if (VLAN FrameType)*/
        else
        {
          /*Check if untag vlanid is configured or not*/
          /* polyspace +7 RTE:UNR [Justified:Low] "This condition shall be True
          when vlan id is configured.
          Module Test Case Reference test_9" */

          #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
          if ((EthIf_GaaStaticController_PB[LucLoopCount].usEthIfVlanTagLsb)
            == (uint16)ETHIF_ZERO)
          #else

          if ((EthIf_GpConfigPtr->pController[LucLoopCount].ucVlanId)
            == (uint16)ETHIF_ZERO)
          #endif
          {
            #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
            LucEthIfCtrlIdx = EthIf_GaaStaticController[LucLoopCount].
              ucEthIfCtrlIdx;
            #else
            LucEthIfCtrlIdx =
              EthIf_GpConfigPtr->pController[LucLoopCount].ucEthIfCtrlIdx;
            #endif

            LblCtrlFound = TRUE;
            LddFrameType = FrameType;
            LucLoopCount = ETHIF_NUM_OF_CONTROLLERS;
          }
          /*Violates ref EthIf_c_QAC_REF_2, INT30 (Cert-C) */

          else
          {
            LucLoopCount++;
          }
        }
        #else
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        if ((EthIf_GaaStaticController_PB[LucLoopCount].usEthIfVlanTagLsb)
          == (uint16)ETHIF_ZERO)
        {
          LucEthIfCtrlIdx = EthIf_GaaStaticController[LucLoopCount].
            ucEthIfCtrlIdx;
        #else
        if ((EthIf_GpConfigPtr->pController[LucLoopCount].ucVlanId)
          == ETHIF_ZERO)
        {
          LucEthIfCtrlIdx = EthIf_GpConfigPtr->pController[LucLoopCount].
            ucEthIfCtrlIdx;
        #endif
          LblCtrlFound = ETHIF_TRUE;
          LddFrameType = FrameType;
          LucLoopCount = ETHIF_NUM_OF_CONTROLLERS;
        }
        /* polyspace +5 RTE:UNR [Justified:Low] "This condition shall be False
        when usEthIfVlanTagLsb equal to ETHIF_ZERO.
        Module Test Case Reference EthIf_Cfg_3_test_11" */

        else
        {
          LucLoopCount++;
        }
        #endif
      }/*end of if (CtrlIdx)*/
      /* polyspace +9 RTE:UNR [Justified:Low] "This condition shall be True
      when control indes is invalid.
      Module Test Case Reference test_9" */
      /*Violates ref EthIf_c_QAC_REF_2, INT30 (Cert-C) */

      else
      {
        LucLoopCount++;
      }

    }
    /* SWS_EthIf_00085f
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    /* Search for the frame type to identify the upper layer Rx Indication
    function  */
    /* polyspace +3 RTE:UNR [Justified:Low] "This condition shall be False when
    CtrlFound status will not be equal to ETHIF_TRUE. */
    if (LblCtrlFound)
    {
      LblRetVal = EthIf_RxIndicationLinearSearch(LddFrameType,
      #if ((ETHIF_NO_OF_RXPDUS > ETHIF_ZERO) && \
           (ETHIF_PRE_COMPILE_SINGLE == STD_ON))
      CtrlIdx, &LucRxPduIndex,
      #endif
      &LucRxIndex);
    }
    /* SWS_EthIf_00104b, SWS_EthIf_00104c
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if (LblRetVal)
    {
      #if ((ETHIF_NO_OF_RXPDUS > ETHIF_ZERO) && \
           (ETHIF_PRE_COMPILE_SINGLE == STD_ON))
      if (ETHIF_INVALID != LucRxPduIndex)
      {
        LpRxPduStat = &EthIf_GaaRxPduStat[LucRxPduIndex];
        LpRxPdu = &EthIf_GaaRxPdus[LucRxPduIndex];
        #if (ETHIF_METADATA_SUPPORT == STD_ON)
        #if (ETHIF_NO_OF_RXMETADATAITEMS > ETHIF_ZERO)
        EthIf_PutMetaDataInfo(LpRxPdu->pMetadataBuffer,
          LpRxPdu->ucMetaDataMask, PhysAddrPtr,
          #ifdef ETHIF_RXINDICATION_INGRESSTIME_SUPPORT
          IngressTimeTuplePtr,
          #endif
          IsBroadcast);
        LddPduInfo.MetaDataPtr = LpRxPdu->pMetadataBuffer;
        #endif
        #endif
        LddPduInfo.SduLength = LusLength;
        LddPduInfo.SduDataPtr = (uint8 * )LpDataPtr;

        LpRxPduStat->PduState = ETHIF_PDU_IN_USE;
        LSduR_EthIfRxIndication(LpRxPdu->ucRxPduId, &LddPduInfo);
        if (ETHIF_FALSE == LpRxPdu->blKeepTxBuffer)
        {
          LpRxPduStat->PduState = ETHIF_PDU_AVAILABLE;
          #ifdef ETHIF_IMMEDIATE_TRANSMIT_SUPPORT
          LucRxPoolIndex = EthIf_RxPoolIdxMappingArray[LucRxPduIndex];
          LpRxPool = &EthIf_GaaRxPools[LucRxPoolIndex];
          Eth_ReleaseRxBuffer(LpRxPool->ucPhyCtrlIdx
          #ifdef ETHIF_RXINDICATION_INGRESSTIME_SUPPORT
          , RxHandleId
          #endif
          );
          #endif
        }
        #ifdef ETHIF_RXINDICATION_INGRESSTIME_SUPPORT
        else
        {
          LpRxPduStat->RxHandleId = RxHandleId;
        }
        #endif
      }
      else
      #endif
      {
        LpRxIndicationFunc = ETHIF_GPFIRSTRXINDICATIONFUNC;
        if (NULL_PTR != LpRxIndicationFunc[LucRxIndex])
        {
          (LpRxIndicationFunc[LucRxIndex])(LucEthIfCtrlIdx, LddFrameType,
            IsBroadcast, PhysAddrPtr, LpDataPtr, LusLength);
        }
      }
    }
    else
    {
      /* Unknown EtherType SWS_EthIf_00502, SWS_EthIf_00503
       Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #if (STD_ON == ETHIF_SECURITY_EVENT_ENABLED)
      /* SWS_EthIf_00503:EthIf module won't provide Context data and TimeStamp
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      IdsM_ReportSecurityEvent(ETHIF_SEV_ETH_DROP_UNKNOWN_ETHERTYPE, NULL_PTR,
        ETHIF_DEFAULT_CONTEXT_DATA_SIZE, ETHIF_DEFAULT_CONTEXT_DATA_VERSION,
        ETHIF_DEFAULT_COUNT, NULL_PTR);
      #endif
    }
    #ifdef ETHIF_RXINDICATION_INGRESSTIME_SUPPORT
    ETHIF_UNUSED(IngressTimeTuplePtr);
    ETHIF_UNUSED(RxHandleId);
    #endif
  }/* End of if ((Std_ReturnType)E_OK == LddStatusReturn) */
  /* SWS_EthIf_00172g
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_ReleaseRxBuffer                               **
**                                                                            **
** Service ID           : 0x9B                                                **
**                                                                            **
** Description          : This API of the EthIf module Releases Rx buffer     **
**                        which was held during reception                     **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different PduIds. Non reentrant for   **
**                        the same PduId                                      **
**                                                                            **
** Input Parameters     : RxPduId                                             **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)    : EthIf_RxPoolIdxMappingArray,**
**                                                EthIf_GaaRxPools,           **
**                                                EthIf_GaaRxPdus,            **
**                                                EthIf_GaaRxPduStat          **
**                                                                            **
**                        Function(s) invoked   : Eth_ReleaseRxBuffer         **
**                                                                            **
** Design ID            : ETHIF_SDD_0732                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 3 */
void EthIf_ReleaseRxBuffer(PduIdType RxPduId)
{
  #if (ETHIF_RELEASERXBUFF_SUPPORT == STD_ON)
  const EthIf_RxPoolType * LpRxPool;
  const EthIf_RxPduType * LpRxPdu;
  EthIf_RxPduStatusType * LpRxPduStat;
  uint8 LucRxPoolIndex;
  /* Validate input parameter RxPduId */
  if (RxPduId <= ETHIF_RXPOOLIDX_MAP_SIZE)
  {
    /* Fetch TxPool Index */
    LucRxPoolIndex = EthIf_RxPoolIdxMappingArray[RxPduId];
    LpRxPool = &EthIf_GaaRxPools[LucRxPoolIndex];
    LpRxPduStat = &EthIf_GaaRxPduStat[RxPduId];
    LpRxPdu = &EthIf_GaaRxPdus[LpRxPool->ucPduIDsIndex];
    if ((ETHIF_PDU_IN_USE ==  LpRxPduStat->PduState) && (ETHIF_TRUE == LpRxPdu->blKeepTxBuffer))
    {
      LpRxPduStat->PduState = ETHIF_PDU_AVAILABLE;
      #ifdef ETHIF_IMMEDIATE_TRANSMIT_SUPPORT
      Eth_ReleaseRxBuffer(LpRxPool->ucPhyCtrlIdx
      #ifdef ETHIF_RXINDICATION_INGRESSTIME_SUPPORT
      , LpRxPduStat->RxHandleId
      #endif
      );
      #endif
    }
  }
  #else
  UNUSED(RxPduId);
  #endif
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_TxConfirmation                                **
**                                                                            **
** Service ID           : 0x11                                                **
**                                                                            **
** Description          : This API of the EthIf module Confirms frame         **
**                        transmission by the indexed controller              **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx, BufIdx, Result                             **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)    :                             **
**                                                EthIf_GddInit               **
**                                                EthIf_GpTxConfirmationFunc  **
**                                                EthIf_GaaTxConfirmationFunc **
**                                                EthIf_GaaTxPduStat          **
**                                                                            **
**                        Function(s) invoked   : <UL>_TxConfirmation,        **
**                                                LSduR_EthIfTxConfirmation   **
**                                                                            **
** Design ID            : ETHIF_SDD_0088                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 3 */
void EthIf_TxConfirmation(
  uint8 CtrlIdx, Eth_BufIdxType BufIdx, Std_ReturnType Result)
{
  #if ((ETHIF_PRE_COMPILE_SINGLE == STD_ON) && \
       (ETHIF_ENABLE_TX_INTERRUPT == STD_OFF))
  const EthIf_StaticPhyController_PB* LpPhysicalController_PB;
  #endif
  Std_ReturnType LddStatusReturn;
  #if (ETHIF_ZERO < ETHIF_UL_TXCONFIRMATION_NUM)
  const EthIf_TxConFuncPtr*  LpTxConfirmationFunc;
  uint8 LucCtrlIdx;
  #endif
  #if (ETHIF_NO_OF_TXPDUS > ETHIF_ZERO)
  EthIf_TxPduStatusType * LpPduStatus;
  boolean LblLsduRConf;
  #endif
  #if ((ETHIF_ONE < ETHIF_UL_TXCONFIRMATION_NUM) || \
       (ETHIF_NO_OF_TXPDUS > ETHIF_ZERO))
  /* PRQA S 3204 1 */
  uint8 LucCount = (uint8)ETHIF_ZERO;
  #endif
  LddStatusReturn = E_OK;

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf module is in INITIALIZED state.
  Module Test Case Reference ETHIF_ETC_026, SWS ID SWS_EthIf_00092"
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_TXCONFIRMATION_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  else
  {
    /* polyspace +5 RTE:IDP [Justified:Low] "This Pointer will not be out of bound
    as it will have the controller index configured with respect to number of
    configured controllers  */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)

    #if (ETHIF_ENABLE_TX_INTERRUPT == STD_OFF)
    SchM_Enter_EthIf_MAIN_PERIOD_PROTECT();
    LpPhysicalController_PB = &EthIf_GaaStaticPhyController_PB[EthIf_GucEthIfCtrlIdx_Tx];
    SchM_Exit_EthIf_MAIN_PERIOD_PROTECT();

    if (CtrlIdx != LpPhysicalController_PB->ucCtrlIdx)
    #else
    if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
    #endif

    #else
    #if (ETHIF_ENABLE_TX_INTERRUPT == STD_OFF)
    if (CtrlIdx != (EthIf_GpConfigPtr->pStaticPhyController[EthIf_GucEthIfCtrlIdx_Tx].ucCtrlIdx))
    #else
    if (EthIf_GpConfigPtr->ucEthIfControllercount <= CtrlIdx)
    #endif

    #endif
    {
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_TXCONFIRMATION_SID, ETHIF_E_INV_CTRL_IDX));
      #endif
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }

    if ((Eth_BufIdxType)ETHIF_INVALID_32 <= BufIdx)
    {
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_TXCONFIRMATION_SID, ETHIF_E_INV_PARAM));
      #endif
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
  }

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
  any of the DET error is being reported.
  Module Test Case Reference test_16, test_17, test_18" */
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    #if (ETHIF_NO_OF_TXPDUS > ETHIF_ZERO)
    LucCount = ETHIF_ZERO;
    LblLsduRConf = ETHIF_FALSE;
    LpPduStatus = &EthIf_GaaTxPduStat[ETHIF_ZERO];
    while (LucCount < ETHIF_NO_OF_TXPDUS)
    {
      if (LpPduStatus->PduState == ETHIF_PDU_IN_USE)
      {
        if ((LpPduStatus->BuffIdx == BufIdx) &&
            (LpPduStatus->ucPhyCtrlIndex == CtrlIdx))
        {
          /* Call LSduR_EthIfTxConfirmation */
          LSduR_EthIfTxConfirmation(LpPduStatus->ddConfPduId, Result);
          /* Reset Pdu Status Variable */
          LpPduStatus->ddConfPduId = ETHIF_INVALID_PDUID;
          LpPduStatus->BuffIdx = ETHIF_INVALID_32;
          LpPduStatus->ucPhyCtrlIndex = ETHIF_INVALID;
          LpPduStatus->PduState = ETHIF_PDU_AVAILABLE;
          LpPduStatus->TxnState = ETHIF_STATE_DEFAULT;
          LblLsduRConf = ETHIF_TRUE;
        }
      }
      LpPduStatus++;
      LucCount++;
    }
    if (ETHIF_FALSE == LblLsduRConf)
    #endif
    {
      #if (ETHIF_ZERO < ETHIF_UL_TXCONFIRMATION_NUM)
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LpTxConfirmationFunc = &EthIf_GaaTxConfirmationFunc_PB[ETHIF_ZERO];
      #else
      LpTxConfirmationFunc = EthIf_GpConfigPtr->pTxConfirmationFunc;
      #endif
      /* SWS_EthIf_00091f, SWS_EthIf_00106b, SWS_EthIf_00106c, SWS_EthIf_00125
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #if (ETHIF_ENABLE_TX_INTERRUPT == STD_OFF)
      LucCtrlIdx = EthIf_GucEthIfCtrlIdx_Tx;
      #else
      LucCtrlIdx = CtrlIdx;
      #endif

      #if (ETHIF_ONE < ETHIF_UL_TXCONFIRMATION_NUM)
      LucCount = ETHIF_ZERO;
      do
      {
        (LpTxConfirmationFunc[LucCount])(LucCtrlIdx, BufIdx, Result);
      #else
        (LpTxConfirmationFunc[0])(LucCtrlIdx, BufIdx, Result);
      #endif

      /* PRQA S 3383 2 */
      #if (ETHIF_ONE < ETHIF_UL_TXCONFIRMATION_NUM)
        LucCount++;
      /* PRQA S 1251 1 */
      } while (LucCount < ETHIF_UL_TXCONFIRMATION_NUM);
      #endif
      /* SWS_EthIf_00166g
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      #endif

      #if (STD_OFF == ETHIF_DEV_ERROR_DETECT)
      /*Violates MISRA 2012 Required Rule 2.2, #ref EthIf_Cbk_c_QAC_REF_2 */
      /*PRQA S 3112 2*/
      ETHIF_UNUSED(CtrlIdx);
      #endif
    }
  }
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_CtrlModeIndication                            **
**                                                                            **
** Service ID           : 0x0e                                                **
**                                                                            **
** Description          : This API of the EthIf module Confirms the change in **
**                        the controller mode requested for the controller    **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx,CtrlMode                                    **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)    :  EthIf_GddInit              **
**                                                                            **
**                        Function(s) invoked   : EthSm_CtrlModeIndication    **
**                                                                            **
** Design ID            : ETHIF_SDD_0085                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 2 */
void  EthIf_CtrlModeIndication(uint8 CtrlIdx, Eth_ModeType CtrlMode)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController_PB*  LpStaticController_PB;
  const EthIf_StaticPhyController_PB*  LpPhysicalController_PB;
  #endif
  Std_ReturnType LddStatusReturn;
  uint8 LucCtrlIdx;
  LddStatusReturn = E_OK;

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf module is in UNINITIALIZED state.
  Module Test Case Reference test_20 " */
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf module is in UNINITIALIZED state.
  Module Test Case Reference test_18" */
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {

    if (EthIf_GucEthEthIfCtrlIdx < (uint8)ETHIF_NUM_OF_PHYSICAL_CONTROLLERS)
    {
    /* SWS_EthIf_00252
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    SchM_Enter_EthIf_MAIN_PERIOD_PROTECT();
    EthIf_GaaStatusPhysController[EthIf_GucEthEthIfCtrlIdx] = CtrlMode;
    SchM_Exit_EthIf_MAIN_PERIOD_PROTECT();
    }
    for (LucCtrlIdx = (uint8)ETHIF_ZERO; LucCtrlIdx <
        (uint8)ETHIF_NUM_OF_CONTROLLERS; LucCtrlIdx++)
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LpStaticController_PB = &EthIf_GaaStaticController_PB[LucCtrlIdx];

      LpPhysicalController_PB = &EthIf_GaaStaticPhyController_PB[
        (LpStaticController_PB->pPhysCtrl)->ucPhysCtrlIdx];

      if (CtrlIdx == LpPhysicalController_PB->ucCtrlIdx)
      {
        EthSM_CtrlModeIndication(EthIf_GaaStaticController[LucCtrlIdx].ucEthIfCtrlIdx, CtrlMode);
      }
      #else
      if (CtrlIdx == (EthIf_GpConfigPtr->pController[LucCtrlIdx].pPhysCtrl)->ucCtrlIdx)
      {
        EthSM_CtrlModeIndication((EthIf_GpConfigPtr->pController[LucCtrlIdx].
           ucEthIfCtrlIdx), CtrlMode);
      }
      #endif
    }
  }
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_TrcvModeIndication                            **
**                                                                            **
** Service ID           : 0x0f                                                **
**                                                                            **
** Description          : This API of the EthIf module Confirms the change in **
**                        the transceiver mode requested for the transceiver  **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : TrcvIdx,TrcvMode                                    **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)    : EthIf_GddInit               **
**                                                                            **
**                        Function(s) invoked   :                             **
**                                                                            **
** Design ID            : ETHIF_SDD_0087                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 2 */
void  EthIf_TrcvModeIndication(uint8 TrcvIdx, Eth_ModeType TrcvMode)
{
  Std_ReturnType LddStatusReturn;
  LddStatusReturn = E_OK;

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf module is in INITIALIZED state.
  Module Test Case Reference test_23" */
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf module is in UNINITIALIZED state.
  Module Test Case Reference test_23" */
  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {

    if (EthIf_GucTrcvEthIfCtrlIdx < (uint8)ETHIF_NUM_OF_TRANSCEIVERS)
    {
      /* SWS_EthIf_00253
      Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      SchM_Enter_EthIf_MAIN_PERIOD_PROTECT();
      EthIf_GaaStatusTrcv[EthIf_GucTrcvEthIfCtrlIdx] = TrcvMode;
      SchM_Exit_EthIf_MAIN_PERIOD_PROTECT();
    }

  }
    /*Violates MISRA 2012 Required Rule 2.2, #ref EthIf_Cbk_c_QAC_REF_2 */

    ETHIF_UNUSED(TrcvIdx);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_SleepIndication                               **
**                                                                            **
** Service ID           : 0x0e                                                **
**                                                                            **
** Description          : This API is called by the corresponding EthTrcv,    **
**                        if a sleep indication was detected on the network.  **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : TrcvIdx                                             **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)    :  EthIf_GddInit              **
**                                                                            **
**                        Function(s) invoked   :  EthSM_SleepIndication      **
**                                                                            **
** Design ID            : ETHIF_SDD_0096                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 2 */
void EthIf_SleepIndication(uint8 TrcvIdx)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticTransceiver_PB*  LpStaticTransceiver_PB;
  #endif
  Std_ReturnType LddStatusReturn;

  LddStatusReturn = (Std_ReturnType)E_OK;


  /* polyspace +4 RTE:UNR [Justified:Low] "This condition shall be False when
  EthIf module is in UNINITIALIZED state.
  Module Test Case Reference test_20 " */
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    /* polyspace +8 RTE:IDP [Justified:Low] "This Pointer will not be out of
    bound as it will have the controller index configured with respect to number
    of configured controllers  */
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  LpStaticTransceiver_PB = &EthIf_GaaStaticTransceiver_PB[EthIf_GucEthEthIfCtrlIdx];
  if (TrcvIdx == LpStaticTransceiver_PB->ucTrcvIdx)
  #else
  if (TrcvIdx == (EthIf_GpConfigPtr->pTransceiver[EthIf_GucEthEthIfCtrlIdx].ucTrcvIdx))
  #endif
    {
      EthSM_SleepIndication(EthIf_GucEthEthIfCtrlIdx);
    }
  }
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_GetAndResetMeasurementData                    **
**                                                                            **
** Service ID           : 0x45                                                **
**                                                                            **
** Description          : This API Allows to read and reset detailed          **
**                        measurement data for diagnostic purposes            **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant                                           **
**                                                                            **
** Input Parameters     : MeasurementIdx, MeasurementResetNeeded              **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : MeasurementDataPtr                                  **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0020                                      **
*******************************************************************************/
/* SWS_EthIf_00316
Once EA tool is ready, SWS_ID will be updated with Design_ID*/
#if (STD_ON == ETHIF_MEASUREMENT_DATA_API)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 4 */
Std_ReturnType EthIf_GetAndResetMeasurementData(
  EthIf_MeasurementIdxType MeasurementIdx, boolean MeasurementResetNeeded,
  uint32 * MeasurementDataPtr)
{
  Std_ReturnType LddStatusReturn;
  uint32 LulVlanDiagMeasData;

  LddStatusReturn = (Std_ReturnType)E_OK;

  (void)SchM_Enter_EthIf_MAIN_PERIOD_PROTECT();
  LulVlanDiagMeasData = EthIf_GulVlanDiagMeasData;
  (void)SchM_Exit_EthIf_MAIN_PERIOD_PROTECT();

  /* SWS_EthIf_00319
  Once EA tool is ready, SWS_ID will be updated with Design_ID*/
  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETANDRESETMEASDATA_SID, ETHIF_E_UNINIT));

    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  #endif
  {
    /* SWS_EthIf_00308
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    if ((uint32*)NULL_PTR != MeasurementDataPtr)
    {
      /* SWS_EthIf_00309
       Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      if (ETHIF_MEAS_DROP_CRTLIDX == MeasurementIdx)
      {
        *MeasurementDataPtr = LulVlanDiagMeasData;
        if (MeasurementResetNeeded)
        {
          (void)SchM_Enter_EthIf_MAIN_PERIOD_PROTECT();
          EthIf_GulVlanDiagMeasData = ETHIF_ZERO;
          (void)SchM_Exit_EthIf_MAIN_PERIOD_PROTECT();
        }
        else
        {
          /* Do Nothing */
        }
      }
      /* SWS_EthIf_00310
       Once EA tool is ready, SWS_ID will be updated with Design_ID*/
      else
      {
        LddStatusReturn = (Std_ReturnType)E_NOT_OK;
      }
    }

  /* polyspace +5 RTE:UNR [Justified:Low] "This condition will be true when
  MeasurementDataPtr is NULL and MeasurementIdx is ETHIF_MEAS_ALL.
  Module Test Case Reference ETHIF_ETC_600" */
    /* SWS_EthIf_00314
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    else if (ETHIF_MEAS_ALL == MeasurementIdx)
    {
      if (MeasurementResetNeeded)
      {
        /* SWS_EthIf_00312
        Once EA tool is ready, SWS_ID will be updated with Design_ID*/
        (void)SchM_Enter_EthIf_MAIN_PERIOD_PROTECT();
        EthIf_GulVlanDiagMeasData = ETHIF_ZERO;
        (void)SchM_Exit_EthIf_MAIN_PERIOD_PROTECT();
      }
      else
      {
        LddStatusReturn = (Std_ReturnType)E_NOT_OK;
      }
    }
    else
    {
      /* Do nothing */
    }
  }
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_GetPhyWakeupReason                            **
**                                                                            **
** Service ID           : 0x69                                                **
**                                                                            **
** Description          : None                                                **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant                                           **
**                                                                            **
** Input Parameters     : TrcvIdx                                             **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : WakeupReasonPtr                                     **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0024                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_WAKEUP_SUPPORT)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 3 */
Std_ReturnType  EthIf_GetPhyWakeupReason(
  uint8 TrcvIdx, EthTrcv_WakeupReasonType * WakeupReasonPtr)
{
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_TrcvApis*  LpTrcvApis;
  const EthIf_StaticTransceiver_PB* LpStaticTransceiver_PB;
  #endif
  Std_ReturnType LddStatusReturn;

  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    /* SWS_EthIf_00487
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETPHYWUREASON_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  else if ((uint8)ETHIF_NUM_OF_TRANSCEIVERS  <= TrcvIdx)
  {
    /* SWS_EthIf_00488
    Once EA tool is ready, SWS_ID will be updated with Design_ID*/
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETPHYWUREASON_SID, ETHIF_E_INV_TRCV_IDX));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
    /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be true when
    WakeupReasonPtr is NULL.
    Module Test Case Reference SWS_EthIf_00489"
    Once EA tool is ready, SWS_ID will be updated with Design_ID */
  else if (NULL_PTR == WakeupReasonPtr)
  {
    /* SWS_EthIf_00489
     Once EA tool is ready, SWS_ID will be updated with Design_ID */
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETPHYWUREASON_SID, ETHIF_E_PARAM_POINTER));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  else
  {
    LddStatusReturn = (Std_ReturnType)E_OK;

    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    /* Get the pointer to the requested Transceiver's structure */
    LpStaticTransceiver_PB = &EthIf_GaaStaticTransceiver_PB[TrcvIdx];
    LpTrcvApis = LpStaticTransceiver_PB->pTrcvDriver;
    if (LpTrcvApis != NULL_PTR)
    #else
    /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be true when
    pTrcvDriver is NOT_NULL.
    Module Test Case Reference SWS_EthIf_00486"
    Once EA tool is ready, SWS_ID will be updated with Design_ID */
    if ((EthIf_GpConfigPtr->pTransceiver[TrcvIdx].pTrcvDriver) != NULL_PTR)
    #endif
    {
      /* SWS_EthIf_00486
       Once EA tool is ready, SWS_ID will be updated with Design_ID */
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      LddStatusReturn = (LpTrcvApis->pEthTrcvGetBusWuReason)
        (LpStaticTransceiver_PB->ucTrcvIdx, WakeupReasonPtr);
      #else
      LddStatusReturn = ((EthIf_GpConfigPtr->pTransceiver[TrcvIdx].pTrcvDriver)->pEthTrcvGetBusWuReason)
        (EthIf_GpConfigPtr->pTransceiver[TrcvIdx].ucTrcvIdx, WakeupReasonPtr);
      #endif
    }
  }
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_GetCtrlIdxList                                **
**                                                                            **
** Service ID           : 0x44                                                **
**                                                                            **
** Description          : This API returns the number and index of all active **
**                        Ethernet controllers.                               **
**                                                                            **
** Sync/Async           : Asynchronous                                        **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut parameter      : NumberOfCtrlIdx                                     **
**                                                                            **
** Output Parameters    : CtrlIdxListPtr                                      **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0025                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_GET_CTRLIDXLIST_API)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 3 */
Std_ReturnType EthIf_GetCtrlIdxList(
  uint8 * NumberOfCtrlIdx, uint8 * CtrlIdxListPtr)
{
  Std_ReturnType LddStatusReturn;
  uint8 LucNumOfActiveCtrls;
  uint8 LucLoopCount;
  LucNumOfActiveCtrls = (uint8)ETHIF_ZERO;

  /* SWS_EthIf_00299
  Once EA tool is ready, SWS_ID will be updated with Design_ID */
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETCTRLIDLIST_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  /* SWS_EthIf_00300
   Once EA tool is ready, SWS_ID will be updated with Design_ID */
  else if (*NumberOfCtrlIdx > (uint8)ETHIF_ZERO)
  {
    /* polyspace +4 RTE:UNR [Justified:Low] "This condition will be true when
    CtrlIdxListPtr is NULL and NumberOfCtrlIdx is greater than 0.
    Module Test Case Reference ETHIF_ETC_118" */
    if (NULL_PTR == CtrlIdxListPtr)
    {
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
        ETHIF_GETCTRLIDLIST_SID, ETHIF_E_PARAM_POINTER));
      #endif
      LddStatusReturn = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
      LddStatusReturn = (Std_ReturnType)E_OK;
    }
  }

  else
  {
    LddStatusReturn = (Std_ReturnType)E_OK;
  }

  if ((Std_ReturnType)E_OK == LddStatusReturn)
  {
    for (LucLoopCount = (uint8)ETHIF_ZERO; (LucLoopCount <
        (uint8)ETHIF_NUM_OF_CONTROLLERS); LucLoopCount++)
    {
      /* Check if Controller is active */
      if ((EthIf_GaaStatusController[LucLoopCount] == ETH_MODE_ACTIVE) ||
        (EthIf_GaaStatusController[LucLoopCount] == ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST))
      {
        if ((*NumberOfCtrlIdx != (uint8)ETHIF_ZERO) &&
          (LucNumOfActiveCtrls < *NumberOfCtrlIdx))
        {
          #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
          CtrlIdxListPtr[LucNumOfActiveCtrls] =
            EthIf_GaaStaticController[LucLoopCount].ucEthIfCtrlIdx;
          #else
          CtrlIdxListPtr[LucNumOfActiveCtrls] =
            EthIf_GpConfigPtr->pController[LucLoopCount].ucEthIfCtrlIdx;
          #endif
        }

        LucNumOfActiveCtrls++;
      }


    }
    /* SWS_EthIf_00298
     Once EA tool is ready, SWS_ID will be updated with Design_ID */
    *NumberOfCtrlIdx = LucNumOfActiveCtrls;
  }
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_GetVlanId                                     **
**                                                                            **
** Service ID           : 0x43                                                **
**                                                                            **
** Description          : Returns the VLAN identifier of the requested        **
**                        Ethernet controller.                                **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : CtrlIdx                                             **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : VlanIdPtr                                           **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0026                                      **
*******************************************************************************/
#if (STD_ON == ETHIF_GET_VLANID_SUPPORT)
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 3 */
Std_ReturnType EthIf_GetVlanId(uint8 CtrlIdx, uint16 * VlanIdPtr)
{
  Std_ReturnType LddStatusReturn;
  #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
  const EthIf_StaticController_PB * LpStaticController_PB;
  #endif
  /* SWS_EthIf_00302
  Once EA tool is ready, SWS_ID will be updated with Design_ID  */
  if (ETHIF_CTRL_STATE_INIT != EthIf_GddInit)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETVLANID_SID, ETHIF_E_UNINIT));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  else if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETVLANID_SID, ETHIF_E_INV_CTRL_IDX));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  /* SWS_EthIf_00303 Once EA tool is ready, SWS_ID will be updated with Design_ID
   polyspace +3 RTE:UNR [Justified:Low] "This condition will be true when
  VlanIdPtr is NULL. Module Test Case Reference ETHIF_ETC_76" */
  else if (NULL_PTR == VlanIdPtr)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)(Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
      ETHIF_GETVLANID_SID, ETHIF_E_PARAM_POINTER));
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }

  else
  {
    LddStatusReturn = (Std_ReturnType)E_OK;
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    /* Get the pointer to the requested Controller's structure */
    LpStaticController_PB = &EthIf_GaaStaticController_PB[CtrlIdx];
    /* SWS_EthIf_00301
     Once EA tool is ready, SWS_ID will be updated with Design_ID */

    *VlanIdPtr = (uint16)((LpStaticController_PB->usEthIfVlanTagLsb)
                  & (uint16)ETHIF_VLANID_CHECK);
    #else

    *VlanIdPtr = (uint16)((EthIf_GpConfigPtr->pController[CtrlIdx].ucVlanId)
                  & (uint16)ETHIF_VLANID_CHECK);
    #endif
  }
  return(LddStatusReturn);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
** Function Name        : EthIf_StartAllPorts                                 **
**                                                                            **
** Service ID           : 0x07                                                **
**                                                                            **
** Description          : This API Request to set all configured and affected **
**                        EthSwtPorts to ETH_MODE_ACTIVE                      **
**                                                                            **
** Sync/Async           : Asynchronous                                        **
**                                                                            **
** Re-entrancy          : Reentrant                                           **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0027                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_StartAllPorts(void)
{
  return(E_NOT_OK);
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_SwitchPortGroupRequestMode                    **
**                                                                            **
** Service ID           : 0x06                                                **
**                                                                            **
** Description          : This API Request a mode for the EthIfSwtPortGroup   **
**                                                                            **
** Sync/Async           : Asynchronous                                        **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : PortGroupIdx, PortMode                              **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0028                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
/* PRQA S 3227 3 */
Std_ReturnType  EthIf_SwitchPortGroupRequestMode(
  EthIf_SwitchPortGroupIdxType PortGroupIdx, Eth_ModeType PortMode)
{
  ETHIF_UNUSED(PortGroupIdx);
  ETHIF_UNUSED(PortMode);
  return(E_NOT_OK);
}

/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)

/*******************************************************************************
** Function Name        : EthIf_MacSecInitRxSc                                **
**                                                                            **
** Service ID           : 0x87                                                **
**                                                                            **
** Description          : RequeststheEthernetInferface(MACsecperSW)or         **
**                        EthernetTransceiverDriver to configure a Reception  **
**                        SecureChannel forthegivenSecureChannel Identifier   **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        the same CtrlIdx                                    **
**                                                                            **
** Input Parameters     : CtrlIdx, Sci                                        **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0781                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecInitRxSc(uint8 CtrlIdx, uint64 Sci)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;
  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
  #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                      ETHIF_MACSEC_INITRXSC_SID, ETHIF_E_INV_PARAM);
  #endif
    LddStatusReturn = E_NOT_OK;
  }
  if (LddStatusReturn != E_NOT_OK)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (EthIf_GaaStaticController_PB[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #else
    if (EthIf_GpConfigPtr->pController[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #endif
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      uint8 LucMacsecindex = EthIf_GaaStaticController_PB[CtrlIdx].ucSWMacSecIndex;
      #else
      uint8 LucMacsecindex = EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex;
      #endif
      EthIf_ControllerMacsecContextType *LpCtrlCtx = &EthIf_GaaMacsecController[LucMacsecindex];
      LpCtrlCtx->RxSci = Sci;
      EthIf_GaaMacSecSeqBits[LucMacsecindex] |= ETHIF_SEQBIT_INIT_RXSC;
    }
    else
    {
      /* The DET Error to be send Service not supported , Need to change the ID */
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                      ETHIF_MACSEC_INITRXSC_SID, ETHIF_E_INV_CTRL_IDX);
      #endif
      LddStatusReturn = E_NOT_OK;
    }
  }
  return LddStatusReturn;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MacSecUpdateSecY                              **
**                                                                            **
** Service ID           : 0x88                                                **
**                                                                            **
** Description          : Requests the Ethernet Inferface to update the       **
**                        SecY/PAC of the PHY with the provided parameters.   **
**                                                                            **
** Sync/Async           : Asynchronous                                        **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        the same CtrlIdx                                    **
**                                                                            **
** Input Parameters     : CtrlIdx, MACsecCfgPtr, TxSci                        **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0782                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecUpdateSecY(
  uint8 CtrlIdx, const Mka_MacSecConfigType * MACsecCfgPtr, uint64 TxSci)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;
  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                      ETHIF_MACSEC_UPDATESECY_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = E_NOT_OK;
  }
  else if (MACsecCfgPtr == NULL_PTR)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                      ETHIF_MACSEC_UPDATESECY_SID, ETHIF_E_PARAM_POINTER);
    #endif
    LddStatusReturn = E_NOT_OK;
  }

  if (LddStatusReturn != E_NOT_OK)
  {
    /* 1. Update global SecY configuration */
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (EthIf_GaaStaticController_PB[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #else
    if (EthIf_GpConfigPtr->pController[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #endif
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      uint8 LucMacsecindex = EthIf_GaaStaticController_PB[CtrlIdx].ucSWMacSecIndex;
      #else
      uint8 LucMacsecindex = EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex;
      #endif
      EthIf_ControllerMacsecContextType *LpCtrlCtx = &EthIf_GaaMacsecController[LucMacsecindex];
      EthIf_MacSecSeqBitsType Seqbitflag = EthIf_GaaMacSecSeqBits[LucMacsecindex];

      /* Quick mask check: all required bits must be set */
      if ((Seqbitflag & ETHIF_SEQBITS_UPDATE_SECY_REQ) == ETHIF_SEQBITS_UPDATE_SECY_REQ)
      {

        LpCtrlCtx->ValidateFrames           = MACsecCfgPtr->ValidateFrames;
        LpCtrlCtx->ConfidentialityOffset    = MACsecCfgPtr->ConfidentialityOffset;
        LpCtrlCtx->CurrentCipherSuite       = MACsecCfgPtr->CurrentCipherSuite;
        LpCtrlCtx->ReplayWindow             = MACsecCfgPtr->ReplayWindow;
        LpCtrlCtx->BypassedVlanPtrs         = MACsecCfgPtr->BypassedVlanPtrs;
        LpCtrlCtx->BypassedEtherTypesPtr    = MACsecCfgPtr->BypassedEtherTypesPtr;
        LpCtrlCtx->BypassedVlansLength      = MACsecCfgPtr->BypassedVlansLength;
        LpCtrlCtx->BypassedEtherTypesLength = MACsecCfgPtr->BypassedEtherTypesLength;
        LpCtrlCtx->IsControlledPortEnabled  = MACsecCfgPtr->ControlledPortEnabled;
        LpCtrlCtx->IsProtectFramesEnabled   = MACsecCfgPtr->ProtectFrames;
        LpCtrlCtx->IsReplayProtectEnabled   = MACsecCfgPtr->ReplayProtect;
        LpCtrlCtx->TxSci                    = TxSci;

        /*  Call MKA module notification API */
        Mka_MacSecUpdateSecYNotification
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        (EthIf_GaaStaticController_PB[CtrlIdx].pPaeInst->ucPaeIdx, LddStatusReturn);
        #else
        (EthIf_GpConfigPtr->pController[CtrlIdx].pPaeInst->ucPaeIdx, LddStatusReturn);
        #endif
        EthIf_GaaMacSecSeqBits[CtrlIdx] |= ETHIF_SEQBIT_UPDATE_SECY;
      }
      else
      {
        #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
        (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                           ETHIF_MACSEC_UPDATESECY_SID, ETHIF_E_INV_CTRL_IDX);
        #endif
        LddStatusReturn = E_NOT_OK;
      }
    }
    else
    {
      /* The DET Error to be send Service not supported , Need to change the ID */
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                        ETHIF_MACSEC_UPDATESECY_SID, ETHIF_E_INV_CTRL_IDX);
      #endif
      LddStatusReturn = E_NOT_OK;
    }
  }
  return LddStatusReturn;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MacSecUpdateSecYNotification                  **
**                                                                            **
** Service ID           : 0x6c                                                **
**                                                                            **
** Description          : Callback to notify that EthTrcv_MacSecUpdateSecY    **
**                        finished                                            **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        the same CtrlIdx                                    **
**                                                                            **
** Input Parameters     : CtrlIdx, Result                                     **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0792                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
void EthIf_MacSecUpdateSecYNotification(uint8 CtrlIdx, Std_ReturnType Result)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;
  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                      ETHIF_MACSEC_UPDATESECY_NOTIFICATION_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = E_NOT_OK;
  }
  if (LddStatusReturn != E_NOT_OK)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (NULL_PTR != EthIf_GaaStaticController_PB[CtrlIdx].pPaeInst)
    #else
    if (NULL_PTR != EthIf_GpConfigPtr->pController[CtrlIdx].pPaeInst)
    #endif
    {
      Mka_MacSecUpdateSecYNotification
     #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
     (EthIf_GaaStaticController_PB[CtrlIdx].pPaeInst->ucPaeIdx, Result);
     #else
     (EthIf_GpConfigPtr->pController[CtrlIdx].pPaeInst->ucPaeIdx, Result);
     #endif
    }
  }
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MacSecAddRxSa                                 **
**                                                                            **
** Service ID           : 0x83                                                **
**                                                                            **
** Description          : RequesttheEthernetInferface(MACsecperSW)            **
**                        finished                                            **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        The Short Secure Channel Identifier is included     **
**                        to support XPN configurations.                      **
**                                                                            **
** Input Parameters     : CtrlIdx, An,  LowestPn, Ssci, KeysPtr, Active       **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0783                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecAddRxSa(
  uint8 CtrlIdx, uint8 An, uint64 LowestPn, uint32 Ssci,
  const Mka_SakKeyPtrType* KeysPtr,  boolean Active)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;

  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                      ETHIF_MACSEC_ADDRXSA_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = E_NOT_OK;
  }
  else if (NULL == KeysPtr)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                        ETHIF_MACSEC_ADDRXSA_SID, ETHIF_E_PARAM_POINTER);
    #endif
    LddStatusReturn = E_NOT_OK;
  }

  if (LddStatusReturn != E_NOT_OK)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (EthIf_GaaStaticController_PB[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #else
    if (EthIf_GpConfigPtr->pController[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #endif
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      uint8 LucMacsecindex = EthIf_GaaStaticController_PB[CtrlIdx].ucSWMacSecIndex;
      #else
      uint8 LucMacsecindex = EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex;
      #endif
      EthIf_MacSecSeqBitsType Seqbitflag = EthIf_GaaMacSecSeqBits[LucMacsecindex];
      /* Quick mask check: all required bits must be set */
      if ((Seqbitflag & ETHIF_SEQBITS_ADDRXSA_REQ) == ETHIF_SEQBITS_ADDRXSA_REQ)
      {
        EthIf_MacsecSaContextType *LpReceivedRxSa = &EthIf_GaaMacsecController[LucMacsecindex].RxSa[An];
        EthIf_ControllerMacsecContextType *LpCtrlCtx = &EthIf_GaaMacsecController[LucMacsecindex];
        uint64 Lullsuite = LpCtrlCtx->CurrentCipherSuite;
        switch (Lullsuite)
          {
            case ETHIF_GCM_AES_128:
              LpReceivedRxSa->KeyLength = ETHIF_SIXTEEN;
              break;
            case ETHIF_GCM_AES_256:
              LpReceivedRxSa->KeyLength = ETHIF_THIRTY_TWO;
              break;
            case ETHIF_GCM_AES_XPN_128:
              LpReceivedRxSa->KeyLength = ETHIF_SIXTEEN;
              break;
            case ETHIF_GCM_AES_XPN_256:
              LpReceivedRxSa->KeyLength = ETHIF_THIRTY_TWO;
              break;
            default:
              LpReceivedRxSa->KeyLength = ETHIF_SIXTEEN;
              break;
          }
        /* 1. Update Tx SA context */
        LpReceivedRxSa->Active = Active;
        LpReceivedRxSa->An = An;
        LpReceivedRxSa->LowestPn = LowestPn;

        ETHIF_MEMCPY(LpReceivedRxSa->Sak, KeysPtr->SakKeyPtr, LpReceivedRxSa->KeyLength);
        ETHIF_MEMCPY(LpReceivedRxSa->Salt, KeysPtr->SaltKeyPtr, ETHIF_SALT_KEY_LENGTH);
        ETHIF_MEMCPY(LpReceivedRxSa->HashKey, KeysPtr->HashKeyPtr, LpReceivedRxSa->KeyLength);

        /* Convert SSCI (uint32) to byte array */
        ETHIF_32BWORDTOBYTEARR(LpReceivedRxSa->Ssci, (uint32 *)(&Ssci));


        Mka_MacSecAddRxSaNotification
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        (EthIf_GaaStaticController_PB[CtrlIdx].pPaeInst->ucPaeIdx, LddStatusReturn);
        #else
        (EthIf_GpConfigPtr->pController[CtrlIdx].pPaeInst->ucPaeIdx, LddStatusReturn);
        #endif
        EthIf_GaaMacSecSeqBits[LucMacsecindex] |= ETHIF_SEQBIT_ADD_RXSA;
      }
      else
      {
        #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
        (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                           ETHIF_MACSEC_ADDRXSA_SID, ETHIF_E_INV_CTRL_IDX);
        #endif
        LddStatusReturn = E_NOT_OK;
      }
    }
    else
    {
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      /* The DET Error to be send Service not supported , Need to change the ID */
      (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                        ETHIF_MACSEC_ADDRXSA_SID, ETHIF_E_INV_CTRL_IDX);
      #endif
      LddStatusReturn = E_NOT_OK;
    }
  }
  return LddStatusReturn;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MacSecAddRxSaNotification                     **
**                                                                            **
** Service ID           : 0x6c                                                **
**                                                                            **
** Description          : Callback to notify that EthTrcv_MacSecUpdateSecY    **
**                        finished                                            **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        the same CtrlIdx                                    **
**                                                                            **
** Input Parameters     : CtrlIdx, Result                                     **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0794                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
void EthIf_MacSecAddRxSaNotification(uint8 CtrlIdx, Std_ReturnType Result)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;
  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                      ETHIF_MACSEC_ADDRXSANOTIFICATION_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = E_NOT_OK;
  }
  if (LddStatusReturn != E_NOT_OK)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (NULL_PTR != EthIf_GaaStaticController_PB[CtrlIdx].pPaeInst)
    #else
    if (NULL_PTR != EthIf_GpConfigPtr->pController[CtrlIdx].pPaeInst)
    #endif
    {
      Mka_MacSecAddRxSaNotification
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      (EthIf_GaaStaticController_PB[CtrlIdx].pPaeInst->ucPaeIdx, Result);
      #else
      (EthIf_GpConfigPtr->pController[CtrlIdx].pPaeInst->ucPaeIdx, Result);
      #endif
    }
  }
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MacSecAddTxSa                                 **
**                                                                            **
** Service ID           : 0x85                                                **
**                                                                            **
** Description          : Requests the Ethernet Inferface to create           **
**                        a Transmission Secure Association in the Transceiver**
**                        The Short Secure Channel Identifier is included     **
**                        to support XPN configurations                       **
**                                                                            **
** Sync/Async           : Asynchronous                                        **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        the same CtrlIdx                                    **
**                                                                            **
** Input Parameters     : CtrlIdx, An, NextPn, Ssci, KeysPtr, Active          **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0784                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecAddTxSa(
  uint8 CtrlIdx, uint8 An, uint64 NextPn, uint32 Ssci,
  const Mka_SakKeyPtrType * KeysPtr, boolean Active)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;

  /* Validate parameters */
  if (((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx) || (An >= ETHIF_MAX_TX_SA))
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                      ETHIF_MACSEC_ADDTXSA_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  else if (KeysPtr == NULL_PTR)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                      ETHIF_MACSEC_ADDTXSA_SID, ETHIF_E_PARAM_POINTER);
    #endif
    LddStatusReturn = (Std_ReturnType)E_NOT_OK;
  }
  if (LddStatusReturn != E_NOT_OK)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (EthIf_GaaStaticController_PB[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #else
    if (EthIf_GpConfigPtr->pController[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #endif
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      uint8 LucMacsecindex = EthIf_GaaStaticController_PB[CtrlIdx].ucSWMacSecIndex;
      #else
      uint8 LucMacsecindex = EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex;
      #endif
      EthIf_MacSecSeqBitsType Seqbitflag = EthIf_GaaMacSecSeqBits[LucMacsecindex];
      /* Quick mask check: all required bits must be set */
      if ((Seqbitflag & ETHIF_SEQBITS_ADDTXSA_REQ) == ETHIF_SEQBITS_ADDTXSA_REQ)
      {
        EthIf_ControllerMacsecContextType *LpCtrlCtx = &EthIf_GaaMacsecController[LucMacsecindex];
        EthIf_MacsecSaContextType *LptxSa = &LpCtrlCtx->TxSa[An];
        uint64 Lullsuite = LpCtrlCtx->CurrentCipherSuite;
        switch (Lullsuite)
        {
          case ETHIF_GCM_AES_128:
            LptxSa->KeyLength = ETHIF_SIXTEEN;
            break;
          case ETHIF_GCM_AES_256:
            LptxSa->KeyLength = ETHIF_THIRTY_TWO;
            break;
          case ETHIF_GCM_AES_XPN_128:
            LptxSa->KeyLength = ETHIF_SIXTEEN;
            break;
          case ETHIF_GCM_AES_XPN_256:
            LptxSa->KeyLength = ETHIF_THIRTY_TWO;
            break;
          default:
            LptxSa->KeyLength = ETHIF_SIXTEEN;
            break;
        }
        /* Updating active An index  */
        LpCtrlCtx->ActiveTxAnIndex = An;
        /* Update Tx SA context */
        LptxSa->Active = Active;
        LptxSa->An = An;
        LptxSa->NextPn = NextPn;
        LptxSa->MaxPn = LpCtrlCtx->IsXpnEnabled ? ETHIF_UINT64_MAX : ETHIF_UINT32_MAX;
        LptxSa->CipherSuite = LpCtrlCtx->CurrentCipherSuite;


        ETHIF_MEMCPY(LptxSa->Sak, KeysPtr->SakKeyPtr, LptxSa->KeyLength);
        ETHIF_MEMCPY(LptxSa->Salt, KeysPtr->SaltKeyPtr, ETHIF_SALT_KEY_LENGTH);
        ETHIF_MEMCPY(LptxSa->HashKey, KeysPtr->HashKeyPtr, LptxSa->KeyLength);

        /* Convert SSCI (uint32) to byte array */
        ETHIF_32BWORDTOBYTEARR(LptxSa->Ssci, (uint32 *)(&Ssci));

        /* Call MKA module notification API */
        Mka_MacSecAddTxSaNotification
        #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
        (EthIf_GaaStaticController_PB[CtrlIdx].pPaeInst->ucPaeIdx, LddStatusReturn);
        #else
        (EthIf_GpConfigPtr->pController[CtrlIdx].pPaeInst->ucPaeIdx, LddStatusReturn);
        #endif
        EthIf_GaaMacSecSeqBits[LucMacsecindex] |= ETHIF_SEQBIT_ADD_TXSA;
      }
      else
      {
        #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
        (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                           ETHIF_MACSEC_ADDTXSA_SID, ETHIF_E_INV_CTRL_IDX);
        #endif
        LddStatusReturn = E_NOT_OK;
      }
    }
    else
    {
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      /* The DET Error to be send Service not supported , Need to change the ID */
      (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                       ETHIF_MACSEC_ADDTXSA_SID, ETHIF_E_INV_CTRL_IDX);
      #endif
      LddStatusReturn = E_NOT_OK;
    }
  }
  return LddStatusReturn;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MacSecAddTxSaNotification                     **
**                                                                            **
** Service ID           : 0x72                                                **
**                                                                            **
** Description          : Callback to notify that EthTrcv_MacSecAddTxSa       **
**                        finished                                            **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        the same CtrlIdx                                    **
**                                                                            **
** Input Parameters     : CtrlIdx, Result                                     **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0793                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
void EthIf_MacSecAddTxSaNotification(uint8 CtrlIdx, Std_ReturnType Result)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;
  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                      ETHIF_MACSEC_ADDTXSANOTIFICATION_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = E_NOT_OK;
  }
  if (LddStatusReturn != E_NOT_OK)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (NULL_PTR != EthIf_GaaStaticController_PB[CtrlIdx].pPaeInst)
    #else
    if (NULL_PTR != EthIf_GpConfigPtr->pController[CtrlIdx].pPaeInst)
    #endif
    {
      Mka_MacSecAddTxSaNotification
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      (EthIf_GaaStaticController_PB[CtrlIdx].pPaeInst->ucPaeIdx, Result);
      #else
      (EthIf_GpConfigPtr->pController[CtrlIdx].pPaeInst->ucPaeIdx, Result);
      #endif
    }
  }
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MacSecSetControlledPortEnabled                **
**                                                                            **
** Service ID           : 0x80                                                **
**                                                                            **
** Description          : Requests to set the Controlled Port enabled         **
**                        parameter of a PAE.                                 **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        the same CtrlIdx                                    **
**                                                                            **
** Input Parameters     : CtrlIdx, ControlledPortEnabled                      **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0805                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecSetControlledPortEnabled(
  uint8 CtrlIdx, boolean ControlledPortEnabled)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;

  /* Validate parameters */
  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                      ETHIF_MACSEC_CONTROLLEDPORTENABLED_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = E_NOT_OK;
  }
  if (LddStatusReturn != E_NOT_OK)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (EthIf_GaaStaticController_PB[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #else
    if (EthIf_GpConfigPtr->pController[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #endif
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      uint8 LucMacsecindex = EthIf_GaaStaticController_PB[CtrlIdx].ucSWMacSecIndex;
      #else
      uint8 LucMacsecindex = EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex;
      #endif
      EthIf_ControllerMacsecContextType *LpCtrlCtx = &EthIf_GaaMacsecController[LucMacsecindex];

      LpCtrlCtx->IsControlledPortEnabled = ControlledPortEnabled;
      if (ETHIF_TRUE == ControlledPortEnabled)
      {
        EthIf_GaaMacSecSeqBits[LucMacsecindex] |= ETHIF_SEQBIT_CTRL_PORT_ENABLED;
      }
    }
    else
    {
      /* The DET Error to be send Service not supported , Need to change the ID */
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                      ETHIF_MACSEC_CONTROLLEDPORTENABLED_SID, ETHIF_E_INV_CTRL_IDX);
      #endif
      LddStatusReturn = E_NOT_OK;
    }
  }
  return LddStatusReturn;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MacSecUpdateTxSa                              **
**                                                                            **
** Service ID           : 0x84                                                **
**                                                                            **
** Description          : Requests the Ethernet Inferface to to update        **
**                        the Transmission Secure Association with the given  **
**                        Packet Number. The Active parameter is included to  **
**                        change the specified AN status                      **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        the same CtrlIdx                                    **
**                                                                            **
** Input Parameters     : CtrlIdx, An, NextPn, Active                         **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0785                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecUpdateTxSa(
  uint8 CtrlIdx, uint8 An, uint64 NextPn, boolean Active)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;
  /* Validate parameters */
  if (((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx) || (An >= ETHIF_MAX_TX_SA))
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                        ETHIF_MACSEC_UPDATETXSA_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn =  E_NOT_OK;
  }
  /* Check if SA exists */
  if (LddStatusReturn != E_NOT_OK)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (EthIf_GaaStaticController_PB[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #else
    if (EthIf_GpConfigPtr->pController[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #endif
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      uint8 LucMacsecindex = EthIf_GaaStaticController_PB[CtrlIdx].ucSWMacSecIndex;
      #else
      uint8 LucMacsecindex = EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex;
      #endif
      EthIf_ControllerMacsecContextType *LpCtrlCtx = &EthIf_GaaMacsecController[LucMacsecindex];
      EthIf_MacsecSaContextType *LptxSa = &LpCtrlCtx->TxSa[An];
      /* Updating active An index  */
      LpCtrlCtx->ActiveTxAnIndex = An;
      /* Update PN and Active state */
      LptxSa->NextPn = NextPn;
      LptxSa->Active = Active;
      EthIf_GaaMacSecSeqBits[LucMacsecindex] |= ETHIF_SEQBIT_TXSA_ACTIVE;
    }
    else
    {
      /* The DET Error to be send Service not supported , Need to change the ID */
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
            ETHIF_MACSEC_UPDATETXSA_SID, ETHIF_E_INV_CTRL_IDX);
      #endif
      LddStatusReturn = E_NOT_OK;
    }
  }
  return LddStatusReturn;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MacSecGetTxSaNextPn                           **
**                                                                            **
** Service ID           : 0x90                                                **
**                                                                            **
** Description          : Request the Ethernet Inferface (MACsec per SW) or   **
**                        the Ethernet Transceiver Driver to return           **
**                        the Packet Number that is used for the next packet  **
**                        in the given Transmission Secure Association        **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        the same CtrlIdx                                    **
**                                                                            **
** Input Parameters     : CtrlIdx, An                                         **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : NextPnPtr                                           **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0786                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecGetTxSaNextPn(
  uint8 CtrlIdx, uint8 An, uint64* NextPnPtr)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;
  if (((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx) || (An >= ETHIF_MAX_TX_SA))
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                        ETHIF_MACSEC_GETTXSANEXTPN_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = E_NOT_OK;
  }
  if (LddStatusReturn != E_NOT_OK)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (EthIf_GaaStaticController_PB[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #else
    if (EthIf_GpConfigPtr->pController[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #endif
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      uint8 LucMacsecindex = EthIf_GaaStaticController_PB[CtrlIdx].ucSWMacSecIndex;
      #else
      uint8 LucMacsecindex = EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex;
      #endif
      EthIf_ControllerMacsecContextType *LpCtrlCtx = &EthIf_GaaMacsecController[LucMacsecindex];
      /* Return PN without increment (increment happens in UpdateTxSa) */
      *NextPnPtr = LpCtrlCtx->TxSa[An].NextPn;
      EthIf_GaaMacSecSeqBits[LucMacsecindex] |= ETHIF_SEQBIT_GET_TXSA_NEXTPN;
    }
    else
    {
      /* The DET Error to be send Service not supported , Need to change the ID */
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                        ETHIF_MACSEC_GETTXSANEXTPN_SID, ETHIF_E_INV_CTRL_IDX);
      #endif
      LddStatusReturn = E_NOT_OK;
    }
  }
  return LddStatusReturn;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MacSecUpdateRxSa                              **
**                                                                            **
** Service ID           : 0x82                                                **
**                                                                            **
** Description          : RequesttheEthernetInferface(MACsecperSW)           **
**                        The Active parameter is included to change         **
**                        the specified AN status.                           **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        The Short Secure Channel Identifier is included     **
**                        to support XPN configurations.                      **
**                                                                            **
** Input Parameters     : CtrlIdx, An,  LowestPn,  Active                     **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0787                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecUpdateRxSa(
  uint8 CtrlIdx, uint8 An, uint64 LowestPn, boolean Active )
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;
  if (((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx) || (An >= ETHIF_MAX_RX_SA))
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                        ETHIF_MACSEC_UPDATERXSA_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = E_NOT_OK;
  }
  /* Check if SA exists */
  if (LddStatusReturn != E_NOT_OK)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (EthIf_GaaStaticController_PB[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #else
    if (EthIf_GpConfigPtr->pController[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #endif
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      uint8 LucMacsecindex = EthIf_GaaStaticController_PB[CtrlIdx].ucSWMacSecIndex;
      #else
      uint8 LucMacsecindex = EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex;
      #endif

      EthIf_ControllerMacsecContextType *LpCtrlCtx = &EthIf_GaaMacsecController[LucMacsecindex];
      EthIf_MacsecSaContextType *LprxSa = &LpCtrlCtx->RxSa[An];
      /* Update PN and Active state */
      LprxSa->LowestPn = LowestPn;
      LprxSa->Active = Active;
      EthIf_GaaMacSecSeqBits[LucMacsecindex] |= ETHIF_SEQBIT_UPDATE_RXSA_LAPN;
    }
    else
    {
      /* The DET Error to be send Service not supported , Need to change the ID */
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                   ETHIF_MACSEC_UPDATERXSA_SID, ETHIF_E_INV_CTRL_IDX);
      #endif
      LddStatusReturn = E_NOT_OK;
    }
  }
  return LddStatusReturn;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MacSecOperational                             **
**                                                                            **
** Service ID           : 0x1C                                                **
**                                                                            **
** Description          : To inform EthIf that MacSec is operational and that **
**                        EthSM can be informed.                              **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        the same CtrlIdx                                    **
**                                                                            **
** Input Parameters     : CtrlIdx,MacSecOperational                           **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0788                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecOperational (
  uint8 CtrlIdx, boolean MacSecOperational)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;
  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                        ETHIF_MACSEC_OPERATIONAL_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = E_NOT_OK;
  }
  if (LddStatusReturn != E_NOT_OK)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (EthIf_GaaStaticController_PB[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #else
    if (EthIf_GpConfigPtr->pController[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #endif
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      uint8 LucMacsecindex = EthIf_GaaStaticController_PB[CtrlIdx].ucSWMacSecIndex;
      #else
      uint8 LucMacsecindex = EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex;
      #endif
      EthIf_ControllerMacsecContextType  *LpCtrlCtx = &EthIf_GaaMacsecController[LucMacsecindex];
      EthIf_MacSecSeqBitsType LddSeqbitflag = EthIf_GaaMacSecSeqBits[LucMacsecindex];

      /* Quick mask check: all required bits must be set */
      if ((LddSeqbitflag & ETHIF_SEQBITS_REQUIRED) == ETHIF_SEQBITS_REQUIRED)
      {
          LpCtrlCtx->IsMacSecOperational = MacSecOperational;
      }
      else
      {
        #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
        (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                         ETHIF_MACSEC_OPERATIONAL_SID, ETHIF_E_INV_CTRL_IDX);
        #endif
        LddStatusReturn = E_NOT_OK;
      }
    }
    else
    {
      /* The DET Error to be send Service not supported , Need to change the ID */
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                        ETHIF_MACSEC_OPERATIONAL_SID, ETHIF_E_INV_CTRL_IDX);
      #endif
      LddStatusReturn = E_NOT_OK;
    }
  }
  return LddStatusReturn;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MacSecDeleteTxSa                              **
**                                                                            **
** Service ID           : 0x16                                                **
**                                                                            **
** Description          : Requests the Ethernet Inferface to  to remove       **
**                        the Transmission Secure Association identified by   **
**                        the provided Association Number.                    **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        the same CtrlIdx                                    **
**                                                                            **
** Input Parameters     : CtrlIdx, An                                         **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0789                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecDeleteTxSa(uint8 CtrlIdx, uint8 An)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;

  /* Validate parameters */
  if (((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx) || (An >= ETHIF_MAX_TX_SA))
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                      ETHIF_MACSEC_DELETETXSA_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = E_NOT_OK;
  }
  if (LddStatusReturn != E_NOT_OK)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (EthIf_GaaStaticController_PB[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #else
    if (EthIf_GpConfigPtr->pController[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #endif
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      uint8 LucMacsecindex = EthIf_GaaStaticController_PB[CtrlIdx].ucSWMacSecIndex;
      #else
      uint8 LucMacsecindex = EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex;
      #endif
      EthIf_ControllerMacsecContextType *LpCtrlCtx = &EthIf_GaaMacsecController[LucMacsecindex];
      /* Clear SCI */
      LpCtrlCtx->TxSci = ETHIF_ZERO;

      /* Clear Key */
      ETHIF_MEMSET(LpCtrlCtx->TxSa[An].Sak, ETHIF_ZERO, sizeof(LpCtrlCtx->TxSa[An].Sak));

      /* Reset Next Packet Number */
      LpCtrlCtx->TxSa[An].NextPn = ETHIF_ZERO;

      /* Resetting active An index  */
      LpCtrlCtx->ActiveTxAnIndex = ETHIF_INVALID;

      /* Mark SA as unused/inactive */
      LpCtrlCtx->TxSa[An].Active = FALSE;
    }
    else
    {
      /* The DET Error to be send Service not supported , Need to change the ID */
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                         ETHIF_MACSEC_DELETETXSA_SID, ETHIF_E_INV_CTRL_IDX);
      #endif
      LddStatusReturn = E_NOT_OK;
    }
  }
  return LddStatusReturn;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MacSecResetRxSc                               **
**                                                                            **
** Service ID           : 0x86                                                **
**                                                                            **
** Description          : RequeststheEthernetInferface(MACsecperSW)or         **
**                        the Ethernet Transceiver Driver to reset to default **
**                        MACsec values of the Reception Secure Channel for   **
**                        the given Secure Channel Identifier                 **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        the same CtrlIdx                                    **
**                                                                            **
** Input Parameters     : CtrlIdx, Sci                                        **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0790                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecResetRxSc(uint8 CtrlIdx, uint64 Sci)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;
  if ((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx)
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                        ETHIF_MACSEC_RESETRXSC_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = E_NOT_OK;
  }
  if (LddStatusReturn != E_NOT_OK)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (EthIf_GaaStaticController_PB[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #else
    if (EthIf_GpConfigPtr->pController[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #endif
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      uint8 LucMacsecindex = EthIf_GaaStaticController_PB[CtrlIdx].ucSWMacSecIndex;
      #else
      uint8 LucMacsecindex = EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex;
      #endif

      EthIf_GaaMacsecController[LucMacsecindex].RxSci = ETHIF_ZERO;
    }
    else
    {
      /* The DET Error to be send Service not supported , Need to change the ID */
      #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
      (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                          ETHIF_MACSEC_RESETRXSC_SID, ETHIF_E_INV_CTRL_IDX);
      #endif
      LddStatusReturn = E_NOT_OK;
    }
  }
  UNUSED(Sci);
  return LddStatusReturn;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_MacSecDeleteRxSa                              **
**                                                                            **
** Service ID           : 0x81                                                **
**                                                                            **
** Description          : Requests the Ethernet Inferface to  to remove       **
**                        the Transmission Secure Association identified by   **
**                        the provided Association Number.                    **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant for different CtrlIdx, Non reentrant for  **
**                        the same CtrlIdx                                    **
**                                                                            **
** Input Parameters     : CtrlIdx, An                                         **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : Std_ReturnType                                      **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0791                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecDeleteRxSa(uint8 CtrlIdx, uint8 An)
{
  Std_ReturnType LddStatusReturn = (Std_ReturnType)E_OK;

  /* Validate parameters */
  if (((uint8)ETHIF_NUM_OF_CONTROLLERS <= CtrlIdx) || (An >= ETHIF_MAX_TX_SA))
  {
    #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
    (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                      ETHIF_MACSEC_DELETERXSA_SID, ETHIF_E_INV_PARAM);
    #endif
    LddStatusReturn = E_NOT_OK;
  }
  if (LddStatusReturn != E_NOT_OK)
  {
    #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
    if (EthIf_GaaStaticController_PB[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #else
    if (EthIf_GpConfigPtr->pController[CtrlIdx].usmacsec == ETHIF_SW_MACSEC)
    #endif
    {
      #if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
      uint8 LucMacsecindex = EthIf_GaaStaticController_PB[CtrlIdx].ucSWMacSecIndex;
      #else
      uint8 LucMacsecindex = EthIf_GpConfigPtr->pController[CtrlIdx].ucSWMacSecIndex;
      #endif
      EthIf_ControllerMacsecContextType *LpCtrlCtx = &EthIf_GaaMacsecController[LucMacsecindex];

      /* Clear SCI */
      LpCtrlCtx->RxSci = ETHIF_ZERO;

      /* Clear Key */
      ETHIF_MEMSET(LpCtrlCtx->RxSa[An].Sak, ETHIF_ZERO, sizeof(LpCtrlCtx->RxSa[An].Sak));

      /* Reset Next Packet Number */
      LpCtrlCtx->RxSa[An].NextPn = ETHIF_ZERO;

      /* Mark SA as unused/inactive */
      LpCtrlCtx->RxSa[An].Active = FALSE;
    }
    else
    {
     /* The DET Error to be send Service not supported , Need to change the ID */
       #if (STD_ON == ETHIF_DEV_ERROR_DETECT)
        (void)Det_ReportError(ETHIF_MODULE_ID, ETHIF_MODULE_INSTANCE_ID,
                        ETHIF_MACSEC_DELETERXSA_SID, ETHIF_E_INV_CTRL_IDX);
        #endif
      LddStatusReturn = E_NOT_OK;
    }
  }
  return LddStatusReturn;
}
/* PRQA S 4640 1 */
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/


