/*******************************************************************************
**                        KPIT Technologies Limited                           **
**                                                                            **
** KPIT Technologies Limited owns all the rights to this work. This work      **
** shall not be copied, reproduced, used, modified or its information         **
** disclosed without the prior written authorization of KPIT Technologies     **
** Limited.                                                                   **
**                                                                            **
** SRC-MODULE: EthIf.h                                                        **
**                                                                            **
** TARGET    : All                                                            **
**                                                                            **
** PRODUCT   : AUTOSAR EthIf                                                  **
**                                                                            **
** PURPOSE   : Provision of AUTOSAR ETHIF Types and extern datatypes.         **
**                                                                            **
** PLATFORM DEPENDANT [yes/no]: no                                            **
**                                                                            **
** TO BE CHANGED BY USER [yes/no]: no                                         **
**                                                                            **
** Prev. Safety Complaint AUTOSAR Version : R23-11                            **
**                                                                            **
** Prev. Safety Complaint Software Version : 1.0.0                            **
**                                                                            **
**                                                                            **
** Design ID : ETHIF_SDD_0023                                                 **
*******************************************************************************/

/*******************************************************************************
**                      Revision History                                      **
********************************************************************************
** Date          Changed By      Description                                  **
********************************************************************************
** 27-Feb-2026   Atreya          As per feature #CP-21187 and #CP-1450, added **
**               Chiplunkar,     functions declarations for MacSec support    **
**               Vini Kadam                                                   **
** 11-Nov-2025   A Naresh        As per feature #CP-12795, Transmission and   **
**                               Reception for MKPDU added MKA_LinkstateChange**
** 17-Sep-2025   A Naresh        As per US  #CP-7847, Hardware TimeStamp      **
**                               updated code for EthIfClkUnit                **
** 30-Jul-2025   A Naresh        As per US #CP-1018, Updated static code      **
**                               as per Compiler Abstraction                  **
** 11-Jul-2025   A Naresh        As per US #198921, Initial version work-     **
**                               -product is carry over from R23-11 V1.0.0    **
*******************************************************************************/

/*******************************************************************************
**                      MISRA-C:2012 violations Section                       **
*******************************************************************************/

/*******************************************************************************
**                      CERT-C Violations Section                             **
*******************************************************************************/

#ifndef ETHIF_H
#define ETHIF_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
/*Design ID: ETHIF_SDD_0023 */
#include "EthIf_Types.h"
#include "ComStack_Types.h"
#include "WEth_GeneralTypes.h"
#include "EthIf_Cfg.h"
#if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
#include "Mka.h"
#endif
#if (ETHIF_PRE_COMPILE_SINGLE == STD_OFF)
#include  "EthIf_PBcfg.h"
#endif
#if (ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES)
#include "EthSwt_GeneralTypes.h"
#endif
#if ((ETHIF_ZERO < ETHIF_NUM_OF_TRANSCEIVERS) &&\
(STD_ON == ETHIF_WAKEUP_SUPPORT))
#include "EcuM_Types.h"
#include "WEth_GeneralTypes.h"
#endif

/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/

/*******************************************************************************
**                      Macro Definitions                                     **
*******************************************************************************/
/*--------------------------------SERVICE ID  --------------------------------*/
/*Design ID: ETHIF_SDD_0544 */
/*EthIf_Init                 */
#define ETHIF_INIT_SID                    (0x01U)
/*Design ID: ETHIF_SDD_0550 */
/*EthIf_SetControllerMode    */
#define ETHIF_SETCNTRLMODE_SID            (0x03U)
/* Design ID: ETHIF_SDD_0536 */
/*EthIf_GetControllerMode    */
#define ETHIF_GETCNTRLMODE_SID            (0x04U)
/* Design ID: ETHIF_SDD_0542 */
/*EthIf_GetTransceiverMode   */
#define ETHIF_GETTRCVMODE_SID             (0x07U)
/* Design ID: ETHIF_SDD_0540 */
/*EthIf_GetPhysAddr          */
#define ETHIF_GETPHYADDR_SID              (0x08U)
/*Design ID: ETHIF_SDD_0551 */
/*EthIf_SetPhysAddr          */
#define ETHIF_SETPHYADDR_SID              (0x0dU)
/*Design ID: ETHIF_SDD_0555 */
/*EthIf_UpdatePhysAddrFilter */
#define ETHIF_UPDATEPHYSADDRFILTER_SID    (0x0cU)

/* Design ID: ETHIF_SDD_0547 */
/*EthIf_ProvideTxBuffer      */
#define ETHIF_PROVIDETXBUF_SID            (0x09U)
/*Design ID: ETHIF_SDD_0553 */
/*EthIf_Transmit             */
#define ETHIF_TRANSMIT_SID                (0x0AU)
/* Design ID: ETHIF_SDD_0543 */
/*EthIf_GetVersionInfo       */
#define ETHIF_GETVERSIONINFO_SID          (0x0BU)

/* Design ID: ETHIF_SDD_0549 */
/*EthIf_RxIndication         */
#define ETHIF_RXINDICATION_SID            (0x10U)
/* Design ID: ETHIF_SDD_0554 */
/*EthIf_TxConfirmation       */
#define ETHIF_TXCONFIRMATION_SID          (0x11U)

/* Design ID: ETHIF_SDD_0545 */
/*EthIf_MainFunctionRx       */
#define ETHIF_MAINFUNCRX_SID              (0x20U)
/* Design ID: ETHIF_SDD_0546 */
/*EthIf_MainFunctionTx       */
#define ETHIF_MAINFUNCTX_SID              (0x21U)

/* Design ID: ETHIF_SDD_0533 */
/*EthIf_CheckWakeup          */
#define ETHIF_CHECKWAKEUP_SID             (0x30U)
/* Design ID: ETHIF_SDD_0541 */
/*EthIf_GetPortMacAddr       */
#define ETHIF_GETPORTMACADDR_SID          (0x28U)

/* Design ID: ETHIF_SDD_0535 */
/*EthIf_GetArlTable          */
#define ETHIF_GETARLTABLE_SID             (0x29U)

/*Design ID: ETHIF_SDD_0552 */
/*EthIf_StoreConfiguration   */
#define ETHIF_STORECONFIGURATION_SID      (0x2cU)
/* Design ID: ETHIF_SDD_0548 */
/*EthIf_ResetConfiguration   */
#define ETHIF_RESETCONFIGURATION_SID      (0x2dU)
/* Design ID: ETHIF_SDD_0537 */
/*EthIf_GetCurrentTimeTUPLE  */
#define ETHIF_GETCURRENTTIMETUPLE_SID     (0x95U)

/* Design ID: ETHIF_SDD_0534 */
/*EthIf_EnableEgressTimeStamp*/
#define ETHIF_ENABLEEGRESSTIMESTAMP_SID   (0x23U)
/* Design ID: ETHIF_SDD_0538 */
/*EthIf_GetEgressTimeStamp   */
#define ETHIF_GETEGRESSTIMESTAMP_SID      (0x24U)
/* Design ID: ETHIF_SDD_0539 */
/*EthIf_GetIngressTimeStamp  */
#define ETHIF_GETINGRESSTIMESTAMP_SID     (0x25U)

#define ETHIF_GETBUFWRXPARAMS_SID         (0x32U)
#define ETHIF_GETBUFWTXPARAMS_SID         (0x31U)
#define ETHIF_SETBUFWTXPARAMS_SID         (0x33U)
#define ETHIF_SETRADIOPARAMS_SID          (0x34U)
#define ETHIF_SETCHANRXPARAMS_SID         (0x35U)
#define ETHIF_SETCHANTXPARAMS_SID         (0x36U)
#define ETHIF_GETCHANRXPARAMS_SID         (0x37U)
#define ETHIF_GETANDRESETMEASDATA_SID     (0x38U)
#define ETHIF_GETPHYWUREASON_SID          (0x69U)
#define ETHIF_GETCTRLIDLIST_SID           (0x44U)
#define ETHIF_GETVLANID_SID               (0x43U)

/*SWS_EthIf_91062*/
/* EthIf_SetPhcTime
Once EA tool is ready, SWS_ID will be updated with Design_ID*/
#define ETHIF_SETPHCTIME_SID              (0x96U)
/*SWS_EthIf_91063
Once EA tool is ready, SWS_ID will be updated with Design_ID*/
/* EthIf_SetPhcCorrection */
#define ETHIF_SETPHCCORRECTION_SID        (0x97U)
/*SWS_EthIf_91064
Once EA tool is ready, SWS_ID will be updated with Design_ID*/
/* EthIf_GetPhcTime */
#define ETHIF_GETPHCTIME_SID              (0x98U)
/*SWS_EthIf_91065
Once EA tool is ready, SWS_ID will be updated with Design_ID*/
/* EthIf_SetPpsSignalMode */
#define ETHIF_SETPPSSIGNALMODE_SID        (0x99U)

#if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
#define ETHIF_MACSEC_UPDATESECY_SID                (0x88U)
#define ETHIF_MACSEC_UPDATESECY_NOTIFICATION_SID   (0x6CU)
#define ETHIF_MACSEC_ADDTXSA_SID                   (0x85U)
#define ETHIF_MACSEC_UPDATETXSA_SID                (0x84U)
#define ETHIF_MACSEC_DELETETXSA_SID                (0x16U)
#define ETHIF_MACSEC_DELETERXSA_SID                (0x81U)
#define ETHIF_MACSEC_CONTROLLEDPORTENABLED_SID     (0x80U)
#define ETHIF_MACSEC_INITRXSC_SID                  (0x87U)
#define ETHIF_MACSEC_RESETRXSC_SID                 (0x86U)
#define ETHIF_MACSEC_ADDRXSA_SID                   (0x83U)
#define ETHIF_MACSEC_UPDATERXSA_SID                (0x82U)
#define ETHIF_MACSEC_ADDRXSANOTIFICATION_SID       (0x77U)
#define ETHIF_MACSEC_ADDTXSANOTIFICATION_SID       (0x71U)
#define ETHIF_MACSEC_GETTXSANEXTPN_SID             (0x90U)
#define ETHIF_MACSEC_OPERATIONAL_SID               (0x1CU)
#define ETHIF_MACSEC_GETMACSECSTATICTICS_SID       (0x89U)

#endif
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

typedef uint8 EthIf_SwitchPortGroupIdxType;  //EthIf.h

typedef enum ETag_EthIf_MeasurementIdxType  //EthIf.h
{
  ETHIF_MEAS_DROP_CRTLIDX = 0x01U,
  ETHIF_MEAS_RESERVED_1,
  ETHIF_MEAS_RESERVED_2 = 0x80U,
  ETHIF_MEAS_RESERVED_3 = 0xF0U,
  ETHIF_MEAS_ALL = 0xFFU
}EthIf_MeasurementIdxType;

/* Not present in R23-11 */
typedef struct STag_Eth_TimeIntDiffType
{
  TimeStampType diff;
  boolean sign;
} Eth_TimeIntDiffType;

/* Not present in R23-11 */
typedef struct STag_Eth_RateRatioType
{
  Eth_TimeIntDiffType IngressTimeStampDelta;
  Eth_TimeIntDiffType OriginTimeStampDelta;
} Eth_RateRatioType;

#if (ETHIF_PRE_COMPILE_SINGLE == STD_OFF)
/* Design ID : ETHIF_SDD_0520  */
typedef struct STagEthIf_ConfigType
{
  const EthIf_StaticController* pController;

  const EthIf_FrameTypeToRxIndFunc*
  pFrameTypeToRxIndFunc;

  const EthIf_StaticPhyController*
  pStaticPhyController;

  const void * pRxIndicationFunc;

  #if (ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES)
  const EthIf_StaticSwitch* pStaticSwitch;
  #endif

  const EthIf_StaticTransceiver*
  pTransceiver;

  const EthIf_LinkStateChangeFuncPtr*
  pLinkStateChangeFunc;

  #if (ETHIF_MKA_LINKSTATECHANGE_NUM > ETHIF_ZERO)
  const EthIf_MkaLinkStateChangeFuncPtr*
  pMkaLinkStateChangeFunc;
  #endif

  const EthIf_TxConFuncPtr*
  pTxConfirmationFunc;

  uint32 ulStartOfDbToc;

  uint8 ucEthIfControllercount;

  uint8 ucFrameTypeToRxIndFuncount;

  uint8 ucPhyControllercount;

  uint8 ucRxIndicationFuncount;

  #if (ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES)
  uint8 ucStaticSwitchcount;
  #endif

  uint8 ucTransceivercount;

  uint8 ucLinkStateChangeFuncount;

  uint8 ucTxConfirmationFuncount;

}EthIf_ConfigType;
#endif

#if (ETHIF_ETH_CLKUNIT_COUNT > 1)
#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_StaticClkUnit EthIf_GaaStaticClkUnit
[ETHIF_NUM_OF_CLKUNIT];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONFIG_DATA_8
#include "EthIf_MemMap.h"
extern const uint8  EthIf_GaaClkUnitIndx
 [ETHIF_NUM_OF_PHYSICAL_CONTROLLERS][ETHIF_CLKUNIT_INDEX];
#define ETHIF_STOP_SEC_CONFIG_DATA_8
#include "EthIf_MemMap.h"
#endif

#if (ETHIF_PRE_COMPILE_SINGLE == STD_OFF)

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_FrameTypeToRxIndFunc
EthIf_GaaFrameTypeToRxIndFunc[ETHIF_NUM_FRAME_OWNER * ETHIF_CONFIGSET_COUNT];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_StaticController EthIf_GaaStaticController
[ETHIF_NUM_OF_CONTROLLERS * ETHIF_CONFIGSET_COUNT];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_StaticPhyController EthIf_GaaStaticPhyController
[ETHIF_NUM_OF_PHYSICAL_CONTROLLERS * ETHIF_CONFIGSET_COUNT];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_RxIndFuncPtr EthIf_GaaRxIndicationFunc
[ETHIF_UL_RXINDICATION_NUM * ETHIF_CONFIGSET_COUNT];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#if (ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES)
#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_StaticSwitch EthIf_GaaStaticSwitch
[ETHIF_NUM_OF_SWITCHES * ETHIF_CONFIGSET_COUNT];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_StaticTransceiver EthIf_GaaStaticTransceiver
[ETHIF_NUM_OF_TRANSCEIVERS * ETHIF_CONFIGSET_COUNT];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_TxConFuncPtr EthIf_GaaTxConfirmationFunc
[ETHIF_UL_TXCONFIRMATION_NUM * ETHIF_CONFIGSET_COUNT];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_LinkStateChangeFuncPtr EthIf_GaaLinkStateChangeFunc
[ETHIF_UL_LINKSTATECHANGE_NUM * ETHIF_CONFIGSET_COUNT];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#if (ETHIF_MKA_LINKSTATECHANGE_NUM > ETHIF_ZERO)
#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_MkaLinkStateChangeFuncPtr EthIf_GaaMkaLinkStateChangeFunc
[ETHIF_MKA_LINKSTATECHANGE_NUM * ETHIF_CONFIGSET_COUNT];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif

#define ETHIF_START_SEC_CONST_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_ConfigType
  EthIf_GaaConfig[ETHIF_CONFIGSET_COUNT];
#define ETHIF_STOP_SEC_CONST_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
extern const EthIf_ConfigType*
 EthIf_GpConfigPtr;
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"

#endif

#if (STD_ON == ETHIF_MEASUREMENT_DATA_API)
#define ETHIF_START_SEC_VAR_CLEARED_32
#include "EthIf_MemMap.h"
extern uint32 EthIf_GulVlanDiagMeasData;
#define ETHIF_STOP_SEC_VAR_CLEARED_32
#include "EthIf_MemMap.h"
#endif

/* Design ID  : ETHIF_SDD_0604 */
#define ETHIF_START_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"
extern uint8 EthIf_GucEthIfCtrlIdx_Tx;
#define ETHIF_STOP_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"

#if (ETHIF_ZERO < ETHIF_TRCV_DRIVER_COUNT)
#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_TrcvApis
  EthIf_GaaTrcvApis[ETHIF_TRCV_DRIVER_COUNT];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif

#if (ETHIF_PRE_COMPILE_SINGLE == STD_OFF)

#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
extern const EthIf_LinkStateChangeFuncPtr*
EthIf_GpFirstLinkStateChangeFunc;
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"

#if (ETHIF_ETH_DRIVER_COUNT > 1)
#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
extern const EthIf_DrvApis*
                                                        EthIf_GpFirstDriverApis;
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
#endif

#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
extern const EthIf_FrameTypeToRxIndFunc*
  EthIf_GpFirstFrameTypeToRxIndFunc;
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"

#if (ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES)
#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
extern const EthIf_StaticSwitch*  EthIf_GpFirstStaticSwitch;
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
#endif

#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
extern const EthIf_TrcvApis*  EthIf_GpFirstTrcvApis;
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
extern const EthIf_StaticController*
  EthIf_GpFirstStaticController;
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
extern const EthIf_TxConFuncPtr*
  EthIf_GpFirstTxConfirmationFunc;
#define ETHIF_STOP_SEC_VAR_NO_INIT_PTR
#include "EthIf_MemMap.h"
#endif

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_RxIndFuncPtr EthIf_GaaRxIndicationFunc_PB
                    [ETHIF_UL_RXINDICATION_NUM];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#if (ETHIF_ZERO < ETHIF_UL_TXCONFIRMATION_NUM)
#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_TxConFuncPtr
EthIf_GaaTxConfirmationFunc_PB[ETHIF_UL_TXCONFIRMATION_NUM];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif

#if (STD_ON == ETHIF_WAKEUP_SUPPORT)

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_WakeupSourceType
  EthIf_GaaWakeupSource[ETHIF_TOTAL_NUM_OF_WAKEUPSOURCE];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif

#if (ETHIF_SWT_DRIVER_COUNT > 1)
#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
/* Global array for Ethernet Switch structure  */
extern const EthIf_SwtApis  EthIf_GaaSwtApis[];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif

#if (ETHIF_ETH_DRIVER_COUNT > 1)
#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
/* Global array for Ethernet Driver structure  */
extern const EthIf_DrvApis  EthIf_GaaDriverApis[];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif

#if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_StaticController
  EthIf_GaaStaticController[ETHIF_NUM_OF_CONTROLLERS];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_StaticController_PB
  EthIf_GaaStaticController_PB[ETHIF_NUM_OF_CONTROLLERS];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_StaticTransceiver
EthIf_GaaStaticTransceiver[ETHIF_NUM_OF_TRANSCEIVERS];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_StaticTransceiver_PB
EthIf_GaaStaticTransceiver_PB[ETHIF_NUM_OF_TRANSCEIVERS];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_StaticPhyController
 EthIf_GaaStaticPhyController[ETHIF_NUM_OF_PHYSICAL_CONTROLLERS];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_StaticPhyController_PB
 EthIf_GaaStaticPhyController_PB[ETHIF_NUM_OF_PHYSICAL_CONTROLLERS];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_FrameTypeToRxIndFunc_PB
EthIf_GaaFrameTypeToRxIndFunc_PB[ETHIF_NUM_FRAME_OWNER];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#if (ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES)
#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_StaticSwitch_PB
EthIf_GaaStaticSwitch_PB[ETHIF_NUM_OF_SWITCHES];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif
#endif

#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_LinkStateChangeFuncPtr
EthIf_GaaLinkStateChangeFunc_PB[ETHIF_UL_LINKSTATECHANGE_NUM];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"

#if ((STD_ON == ETHIF_PRE_COMPILE_SINGLE) && \
     (ETHIF_MKA_LINKSTATECHANGE_NUM > 0))
#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_MkaLinkStateChangeFuncPtr
EthIf_GaaMkaLinkStateChangeFunc_PB[ETHIF_MKA_LINKSTATECHANGE_NUM];
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif

/*******************************************************************************
**                              In-line Functions                             **
*******************************************************************************/

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
/* SWS_EthIf_00011
Once EA tool is ready, SWS_ID will be updated with Design_ID*/
#define ETHIF_START_SEC_CODE
/* ETHIF_SRS_015 */
#include "EthIf_MemMap.h"
extern void  EthIf_Init(
  const EthIf_ConfigType*  ConfigPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_SetControllerMode(
  uint8 CtrlIdx,
  Eth_ModeType CtrlMode);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_GetControllerMode(
  uint8 CtrlIdx,
  Eth_ModeType*  CtrlModePtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_SetTransceiverMode(
  uint8 TrcvIdx,
  Eth_ModeType TrcvMode);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_GetTransceiverMode(uint8
  TrcvIdx, Eth_ModeType*
    TrcvModePtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern BufReq_ReturnType  EthIf_ProvideTxBuffer(
  uint8 CtrlIdx, Eth_FrameType FrameType, uint8 Priority,
  Eth_BufIdxType*  BufIdxPtr, uint8** BufPtr, uint16*  LenBytePtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_Transmit(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx,
  Eth_FrameType FrameType,
  boolean TxConfirmation,
  uint16 LenByte,
  const uint8*  PhysAddrPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType EthIf_TransmitPdu(
       PduIdType TxPduId,
       const PduInfoType* PduInfoPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern void EthIf_ReleaseRxBuffer(PduIdType RxPduId);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern void  EthIf_GetVersionInfo(
  Std_VersionInfoType* VersionInfoPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern void  EthIf_GetPhysAddr(
  uint8 CtrlIdx,
  uint8*  PhysAddrPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern void  EthIf_SetPhysAddr(
  uint8 CtrlIdx,
  const uint8*  PhysAddrPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_UpdatePhysAddrFilter(
  uint8 CtrlIdx,
  const uint8*  PhysAddrPtr,
  Eth_FilterActionType Action);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#if ((ETHIF_ZERO < ETHIF_NUM_OF_TRANSCEIVERS) &&\
(STD_ON == ETHIF_WAKEUP_SUPPORT))
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_CheckWakeup(
  EcuM_WakeupSourceType WakeupSource);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_GetPortMacAddr(
  const uint8* MacAddrPtr,
  uint8* SwitchIdxPtr,
  uint8* PortIdxPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#if (ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES)
#if (STD_ON == ETHIF_GET_ARL_TABLE)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_GetArlTable(uint8 SwitchIdx,
  EthSwt_MacVlanType*  ArlTable);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif
#endif

#if (ETHIF_ZERO < ETHIF_NUM_OF_SWITCHES)
#if (STD_ON == ETHIF_STORE_CONFIGURATION_API)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_StoreConfiguration(uint8 SwitchIdx);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

#if (STD_ON == ETHIF_RESET_CONFIGURATION_API)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_ResetConfiguration(uint8 SwitchIdx);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif
#endif

#if (STD_ON == ETHIF_GLOBALTIME_SUPPORT)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_GetCurrentTimeTuple(
  uint8 CtrlIdx, uint8 ClkUnitIdx,
  TimeTupleType*  currentTimeTuplePtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern void  EthIf_EnableEgressTimeStamp(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_GetEgressTimeStamp(
  uint8 CtrlIdx, Eth_BufIdxType BufIdx,
  TimeStampQualType*  timeQualPtr,
  TimeStampType*  timeStampPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_GetIngressTimeStamp(
  uint8 CtrlIdx,
  const Eth_DataType*  DataPtr,
  TimeStampQualType*  timeQualPtr,
  TimeStampType*  timeStampPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
#ifndef ETHIF_RXINDICATION_INGRESSTIME_SUPPORT
extern void  EthIf_RxIndication (uint8 CtrlIdx,
  Eth_FrameType FrameType, boolean IsBroadcast,
  const uint8*  PhysAddrPtr, const Eth_DataType*  DataPtr,
  uint16 LenByte);
#else
extern void  EthIf_RxIndication (uint8 CtrlIdx,
  Eth_FrameType FrameType, boolean IsBroadcast,
  const uint8*  PhysAddrPtr, const Eth_DataType*  DataPtr,
  uint16 LenByte, const TimeTupleType*  IngressTimeTuplePtr,
  Eth_BufIdxType RxHandleId);
#endif
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"



#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern void  EthIf_TxConfirmation(
 uint8 CtrlIdx, Eth_BufIdxType BufIdx,
 Std_ReturnType Result);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"


#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern void  EthIf_TrcvModeIndication(
  uint8 TrcvIdx, Eth_ModeType
  TrcvMode);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern void  EthIf_SleepIndication(
                            uint8 TrcvIdx);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern void  EthIf_CtrlModeIndication(
  uint8 CtrlIdx, Eth_ModeType
  CtrlMode);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#if (STD_ON == ETHIF_ENABLE_WETH_API)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_GetBufWRxParams(
  uint8 CtrlIdx,
  const WEth_BufWRxParamIdType*  RxParamIds,
  uint32* ParamValues, uint8 NumParams);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_GetBufWTxParams(
  uint8 CtrlIdx,
  const WEth_BufWTxParamIdType*  TxParamIds,
  uint32* ParamValues, uint8 NumParams);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_SetBufWTxParams(
  uint8 CtrlIdx, Eth_BufIdxType BufIdx,
  const WEth_BufWTxParamIdType*  TxParamIds,
  const uint32*  ParamValues,
  uint8 NumParams);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_SetRadioParams(
  uint8 TrcvIdx,
  const WEthTrcv_SetRadioParamIdType*  ParamIds,
  const uint32*  ParamValues,
  uint8 NumParams);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_SetChanRxParams(
  uint8 TrcvIdx, uint8 RadioId,
  const WEthTrcv_SetChanRxParamIdType*  RxParamIds,
  const uint32*  ParamValues,
  uint8 NumParams);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_SetChanTxParams(
  uint8 TrcvIdx, uint8 RadioId,
  const WEthTrcv_SetChanTxParamIdType* TxParamIds,
  const uint32*  ParamValues,
  uint8 NumParams);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_GetChanRxParams(
  uint8 TrcvIdx, uint8 RadioId,
  const WEthTrcv_GetChanRxParamIdType* RxParamIds,
  uint32* ParamValues, uint8 NumParams);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_StartAllPorts( void );
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_SwitchPortGroupRequestMode(
  EthIf_SwitchPortGroupIdxType PortGroupIdx,
  Eth_ModeType PortMode);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#if (STD_ON == ETHIF_MEASUREMENT_DATA_API)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_GetAndResetMeasurementData(
  EthIf_MeasurementIdxType MeasurementIdx,
    boolean MeasurementResetNeeded,
      uint32*  MeasurementDataPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

#if (STD_ON == ETHIF_WAKEUP_SUPPORT)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_GetPhyWakeupReason(
  uint8 TrcvIdx,
   EthTrcv_WakeupReasonType*  WakeupReasonPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

#if (STD_ON == ETHIF_GET_CTRLIDXLIST_API)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_GetCtrlIdxList(
  uint8*  NumberOfCtrlIdx,
    uint8*  CtrlIdxListPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

#if (STD_ON == ETHIF_GET_VLANID_SUPPORT)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType  EthIf_GetVlanId(
  uint8 CtrlIdx,
    uint16*  VlanIdPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

#if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
	
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecUpdateSecY(
    uint8 CtrlIdx,
    const Mka_MacSecConfigType* MACsecCfgPtr,
    uint64 TxSci);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
void EthIf_MacSecUpdateSecYNotification(
    uint8 CtrlIdx,
    Std_ReturnType Result);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecAddTxSa(
    uint8 CtrlIdx,
    uint8 An,
    uint64 NextPn,
    uint32 Ssci,
    const Mka_SakKeyPtrType* KeysPtr,
    boolean Active);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
void EthIf_MacSecAddTxSaNotification(
    uint8 CtrlIdx,
    Std_ReturnType Result);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecUpdateTxSa(
    uint8 CtrlIdx,
    uint8 An,
    uint64 NextPn,
    boolean Active);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecDeleteTxSa(uint8 CtrlIdx, uint8 An);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"	

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecDeleteRxSa(uint8 CtrlIdx, uint8 An);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecInitRxSc( uint8 CtrlIdx, uint64 Sci);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"	


#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecResetRxSc(uint8 CtrlIdx, uint64 Sci);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"	

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecAddRxSa( uint8 CtrlIdx, 
                                    uint8 An,
                                    uint64 LowestPn,
                                    uint32 Ssci,
                                    const Mka_SakKeyPtrType* KeysPtr,
                                    boolean Active);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
 Std_ReturnType EthIf_MacSecUpdateRxSa( uint8 CtrlIdx,
										uint8 An,
										uint64 LowestPn,
										boolean Active);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"	

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
void EthIf_MacSecAddRxSaNotification (uint8 CtrlIdx,
													Std_ReturnType Result);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"	

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecSetControlledPortEnabled (
uint8 CtrlIdx,
boolean ControlledPortEnabled
);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecOperational (
uint8 CtrlIdx,
boolean MacSecOperational
);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
Std_ReturnType EthIf_MacSecGetTxSaNextPn (
uint8 CtrlIdx,
uint8 An,
uint64* NextPnPtr
);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#endif

#endif /* ETHIF_H */
/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
