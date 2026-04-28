/*******************************************************************************
**                        KPIT Technologies Limited                           **
**                                                                            **
** KPIT Technologies Limited owns all the rights to this work. This work      **
** shall not be copied, reproduced, used, modified or its information         **
** disclosed without the prior written authorization of KPIT Technologies     **
** Limited.                                                                   **
**                                                                            **
** SRC-MODULE: EthIf_IntTypes.h                                               **
**                                                                            **
** TARGET    : All                                                            **
**                                                                            **
** PRODUCT   : AUTOSAR EthIf                                                  **
**                                                                            **
** PURPOSE   : This file provides internal data structures of EthIf module.   **
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
** Design ID : ETHIF_SDD_0013                                                 **
*******************************************************************************/

/*******************************************************************************
**                      Revision History                                      **
********************************************************************************
** Date          Changed By      Description                                  **
********************************************************************************
** 27-Feb-2026   Atreya          As per feature #CP-21187 and #CP-1450, added **
**               Chiplunkar,     structures and functions for MacSec support  **
**               Vini Kadam                                                   **
** 20-Dec-2025   Atreya          As per feature #CP-10982, added structures   **
**               Chiplunkar      and functions for PDU based Reception        **
**                               support(LsduR)                               **
** 07-Nov-2025   Devika Ramesh   As per feature #CP-10982, added structures   **
**                               and functions for PDU based transmission     **
**                               support(LsduR)                               **
** 17-Sep-2025   A Naresh        As per US  #CP-7847, Hardware TimeStamp      **
**                               updated code for EthIfClkUnit                **
** 12-Sep-2025   A Naresh        As per US  #CP-6076, SecurityEvents updated  **
**                               code for SecurityEventRef Internal Macros    **
** 30-Jul-2025   A Naresh        As per US #CP-1018, Updated static code      **
**                               as per Compiler Abstraction                  **
** 11-Jul-2025   A Naresh        As per US #198921, Initial version work-     **
**                               -product is carry over from R23-11 V1.0.0    **
*******************************************************************************/

/*******************************************************************************
**                   MISRA-C:2012 violations Section                          **
*******************************************************************************/
/**
* #section EthIf_IntTypes_h_REF_1
* Violates MISRA 2012 Advisory Dir 4.9, A function should be used in
* preference to a function-like macro where they are interchangeable.
* JUSTIFICATION: Small function-like macros are used to tradeoff between
* the speed and readability of the firmware.
**/

/*******************************************************************************
**                      CERT-C Violations Section                             **
*******************************************************************************/

#ifndef ETHIF_INTTYPES_H
#define ETHIF_INTTYPES_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "EthIf.h"
/*******************************************************************************
**                        Version Information                                 **
*******************************************************************************/

/*******************************************************************************
**                             Macro Definitions                              **
*******************************************************************************/
/* ETHIF_ONE                  */
#define ETHIF_ONE                               (0x01u)
/* ETHIF_TWO                  */                
#define ETHIF_TWO                               (0x02u)
/* ETHIF_THREE                */                
#define ETHIF_THREE                             (0x03u)
/* ETHIF_FOUR                 */                
#define ETHIF_FOUR                              (0x04u)
/* ETHIF_FIVE                 */                
#define ETHIF_FIVE                              (0x05u)
/* ETHIF_SIX                  */                
#define ETHIF_SIX                               (0x06u)
/* ETHIF_SEVEN                */                
#define ETHIF_SEVEN                             (0x07u)
/* ETHIF_EIGHT                */                
#define ETHIF_EIGHT                             (0x08u)
/* ETHIF_TWELVE               */                
#define ETHIF_TWELVE                            (0x0Cu)
/* ETHIF_THIRTEEN             */                
#define ETHIF_THIRTEEN                          (0x0Du)
/* ETHIF_FOURTEEN             */                
#define ETHIF_FOURTEEN                          (0x0Eu)
/* ETHIF_FIFTEEN              */                
#define ETHIF_FIFTEEN                           (0x0Fu)
/* ETHIF_SIXTEEN              */                
#define ETHIF_SIXTEEN                           (0x10u)
/* ETHIF_THIRTY               */                
#define ETHIF_THIRTY                            (0x1Eu)
/* ETHIF_THIRTY_TWO           */                
#define ETHIF_THIRTY_TWO                        (0x20u)
/* ETHIF_FIFTY                */                
#define ETHIF_FIFTY                             (0x32u)
/* ETHIF_INVALID              */                
#define ETHIF_INVALID                           (0xFFu)
/* ETHIF_FRAME_LSB_MASK       */                
#define ETHIF_FRAME_LSB_MASK                    (0x00FFu)
/*ETHIF_FRAME_MSB_MASK        */                
#define ETHIF_FRAME_MSB_MASK                    (uint16)(0xFF00u)
/* ETHIF_VLAN_FRAMEID         */                
#define ETHIF_VLAN_FRAMEID                      (0x8100u)
/* ETHIF_VLANID_MASK          */                
#define ETHIF_VLANID_MASK                       (0x0Fu)
/* ETHIF_VLANID_CHECK         */                
#define ETHIF_VLANID_CHECK                      (0x0FFFu)
/* ETHIF_VLANTAG_MASK         */                
#define ETHIF_VLANTAG_MASK                      (0x1FFFu)
/* ETHIF_TRUE                 */                
#define ETHIF_TRUE                              (boolean)(0x01u)
/* ETHIF_FALSE                */                
#define ETHIF_FALSE                             (boolean)(0x00u)
/* ETHIF_INVALID              */                       
#define ETHIF_INVALID_32                        (0xFFFFFFFFUL)
/* ETHIF_INVALID_PDUID */                       
#define ETHIF_INVALID_PDUID                     (PduIdType)0xFFFFu
/* ETHIF IDSM DEFAULT CONTEXT DATA SIZE     */
#define ETHIF_DEFAULT_CONTEXT_DATA_SIZE         (uint16)0x00
/* ETHIF IDSM DEFAULT CONTEXT DATA VERSION  */
#define ETHIF_DEFAULT_CONTEXT_DATA_VERSION      (uint16)0x01
/* ETHIF IDSM DEFAULT COUNT                 */
#define ETHIF_DEFAULT_COUNT                     (uint16)0x01
/* ETHIF PHC SET TIME FEATURE               */
#define ETHIF_HW_DISPLINCE_CLK_DISABLED          STD_ON
/* ETHIF VLAN SUPPORT LENGTH                */
#define ETHIF_VLAN_BYTE_LENGTH                  (0x04u)

#if (STD_ON == ETHIF_VLAN_SUPPORT)
#define ETHIF_ETH_HEADER_LENGTH                 0x12u
#else                                           
#define ETHIF_ETH_HEADER_LENGTH                 0x0Eu
#endif                                          
                                                
#define ETHIF_MAC_ADDRESS_LENGTH                0x06u
                                                
#define ETHIF_MAX_TX_SA                         0x02u
                                                
#define ETHIF_MAX_RX_SA                         0x02u
                                                
#define ETHIF_UINT64_MAX                        0xFFFFFFFFFFFFFFFFu
                                                
#define ETHIF_UINT32_MAX                        0xFFFFFFFFu
                                                
#define ETHIF_GCM_AES_128                       (uint64)0x0080C20001000001
#define ETHIF_GCM_AES_256                       (uint64)0x0080C20001000002
#define ETHIF_GCM_AES_XPN_128                   (uint64)0x0080C20001000003
#define ETHIF_GCM_AES_XPN_256                   (uint64)0x0080C20001000004
#define ETHIF_SALT_KEY_LENGTH                   0x0Bu
/*----------------------MACRO'S for tool generated structures-----------------*/

#if (STD_ON == ETHIF_PRE_COMPILE_SINGLE)
#define ETHIF_GPFIRSTRXINDICATIONFUNC \
        &EthIf_GaaRxIndicationFunc_PB[ETHIF_ZERO]
#define ETHIF_GPFIRSTLINKSTATECHANGEFUNC      &EthIf_GaaLinkStateChangeFunc_PB[0]
#define ETHIF_GPMKALINKSTATECHANGEFUNC        &EthIf_GaaMkaLinkStateChangeFunc_PB[0]
#define ETHIF_GPFIRSTSWTAPIS                  &EthIf_GaaSwtApis[0]
#else
#define ETHIF_GPFIRSTRXINDICATIONFUNC \
        EthIf_GpConfigPtr->pRxIndicationFunc
#endif

/*#ref EthIf_IntTypes_h_REF_1 Violates MISRA 2012 Advisory Dir 4.9 */
/*PRQA S 3453 5 */
#ifndef ETHIF_UNUSED                  /* Check for unused parameter warnings*/
  #define ETHIF_UNUSED(variable) ((void) (variable))
#endif

#if (ETHIF_PRE_COMPILE_SINGLE == STD_ON)
#define ETHIF_PHY_CTRL_IDX(CtrlIdx) \
  EthIf_GaaStaticPhyController_PB[(EthIf_GaaStaticController_PB[CtrlIdx]. \
  pPhysCtrl->ucPhysCtrlIdx)].ucCtrlIdx
#else
#define ETHIF_PHY_CTRL_IDX(CtrlIdx) \
  (EthIf_GpConfigPtr->pController[CtrlIdx].pPhysCtrl)->ucCtrlIdx
#endif

#if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
#define ETHIF_ETH_PROVIDETXBUFFER(PhyCtrlIndex, Priority, BufIdxPtr, BufPtr, LenBytePtr) \
  (LpDriverApis->pEthProvideTxBuffer) \
  (PhyCtrlIndex, Priority, BufIdxPtr, BufPtr, LenBytePtr)
#else
#define ETHIF_ETH_PROVIDETXBUFFER(PhyCtrlIndex, Priority, BufIdxPtr, BufPtr, LenBytePtr) \
  Eth_ProvideTxBuffer(PhyCtrlIndex, Priority, BufIdxPtr, BufPtr, LenBytePtr)
#endif

#if (ETHIF_ONE < ETHIF_ETH_DRIVER_COUNT)
#define ETHIF_ETH_TRANSMIT(PhyCtrlIndex, BufIdxPtr, FrameType, LenBytePtr, PhyAddress) \
  (LpDriverApis->pEthTransmit)(PhyCtrlIndex, BufIdxPtr, FrameType, ETHIF_TRUE, LenBytePtr, PhyAddress)
#else
#define ETHIF_ETH_TRANSMIT(PhyCtrlIndex, BufIdxPtr, FrameType, LenBytePtr, PhyAddress) \
  Eth_Transmit(PhyCtrlIndex, BufIdxPtr, FrameType, ETHIF_TRUE, LenBytePtr, PhyAddress)
#endif

#define ETHIF_BITSET_CHECK(Mask, Position) \
        (boolean) (Mask & (ETHIF_ONE << Position))

#define ETHIF_BITUPDATE(Value, Position) \
        (((Value) & 0x01u) << (Position))

#if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
#define ETHIF_MACSECETHERTYPE           0x88E5u

#define ETHIF_SCI_ENABLED(LblSCIEn, LblEndStation, LblBroadcast) \
    (LblSCIEn) = ((ETHIF_FALSE == (LblEndStation)) && \
                  (ETHIF_FALSE == (LblBroadcast))) ? ETHIF_TRUE : ETHIF_FALSE;
#endif

#define ETHIF_MEMCPY      Bsw_Memcpy

#define ETHIF_MEMSET      memset

#if (CPU_TYPE == CPU_TYPE_64)
#define ETHIF_PTR_TYPE   uint64
#elif (CPU_TYPE == CPU_TYPE_32)
#define ETHIF_PTR_TYPE   uint32
#elif (CPU_TYPE == CPU_TYPE_16)
#define ETHIF_PTR_TYPE   uint16
#endif

#define ETHIF_MACSEC_ICV_LENGTH      0x10
#define ETHIF_MACSEC_SECTAG_LENGTH   0x08

#define ETHIF_32BWORDTOBYTEARR(LpDest, LpSrc) \
        EthIf_UintNToUnit8Arr(LpDest, LpSrc, ETHIF_ONE, ETHIF_FOUR)

#define ETHIF_64BWORDTOBYTEARR(LpDest, LpSrc) \
        EthIf_UintNToUnit8Arr(LpDest, LpSrc, ETHIF_ONE, ETHIF_EIGHT)

/*******************************************************************************
*                               Type Definitions                               *
*******************************************************************************/


/* Design ID: ETHIF_SDD_0519,ETHIF_SDD_0048*/
typedef struct STag_EthIf_Controller
{
  /* Count for ProvideTxbuffer */
  uint8 ucProvideTxBufferCount;
}EthIf_Controller;

#if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
typedef struct STag_EthIf_TxStateType
{
  uint8 * pDataBufferPtr;
  uint16 ucSecTagLen;
}EthIf_TxStateType;
#endif

typedef enum ETag_EthIf_StateType
{
  ETHIF_CTRL_STATE_UNINIT = 0,
  ETHIF_CTRL_STATE_INIT
} EthIf_StateType;

typedef enum ETag_EthIf_PduStateType
{
  ETHIF_PDU_AVAILABLE = (uint8)0u,
  ETHIF_PDU_IN_USE
}EthIf_PduStateType;

typedef enum ETag_EthIf_TxnStateType
{
  ETHIF_STATE_DEFAULT = (uint8)0u,
  ETHIF_STATE_DEFFERED,
  ETHIF_STATE_IMMEDIATE
}EthIf_TxnStateType;

typedef struct STag_EthIf_TxPduStatusType
{
  Eth_BufIdxType BuffIdx;
  EthIf_PduStateType PduState;
  EthIf_TxnStateType TxnState;
  PduIdType ddConfPduId;
  uint8 ucPhyCtrlIndex;
}EthIf_TxPduStatusType;

typedef struct STag_EthIf_RxPduStatusType
{
  EthIf_PduStateType PduState;
  #ifdef ETHIF_RXINDICATION_INGRESSTIME_SUPPORT
  Eth_BufIdxType RxHandleId;
  #endif
}EthIf_RxPduStatusType;

/* MetaDataInfo */
typedef struct STag_EthIf_MetaDataInfo
{
  TimeTupleType* TimeTuplePtr;
  ListElemStructType* ListElemTypePtr;
  uint16 Vlan16;
  uint8 MacAddress[ETHIF_MAC_ADDRESS_LENGTH];
  uint8 Priority;
}EthIf_MetaDataInfo;

#if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)

/* Per-controller bitfield that tracks "have we done this step successfully?" */
typedef uint32 EthIf_MacSecSeqBitsType;

/* Bit positions (adjust to your exact flow) */
#define ETHIF_SEQBIT_INIT_RXSC            (1u << 0)  /* EthIf_MacSecInitRxSc() */
#define ETHIF_SEQBIT_UPDATE_SECY          (1u << 1)  /* EthIf_MacSecUpdateSecY() */
#define ETHIF_SEQBIT_ADD_RXSA             (1u << 2)  /* EthIf_MacSecAddRxSa() */
#define ETHIF_SEQBIT_ADD_TXSA             (1u << 3)  /* EthIf_MacSecAddTxSa() */
#define ETHIF_SEQBIT_CTRL_PORT_ENABLED    (1u << 4)  /* EthIf_MacSecSetControlledPortEnabled(TRUE) */
#define ETHIF_SEQBIT_TXSA_ACTIVE          (1u << 5)  /* EthIf_MacSecUpdateTxSa(ACTIVE) */
#define ETHIF_SEQBIT_GET_TXSA_NEXTPN      (1u << 6)  /* EthIf_MacSecGetTxSaNextPn() */
#define ETHIF_SEQBIT_UPDATE_RXSA_LAPN     (1u << 7)  /* EthIf_MacSecUpdateRxSa() */

/* All bits that must be set before Operational(TRUE) */
#define ETHIF_SEQBITS_REQUIRED ( \
        ETHIF_SEQBIT_INIT_RXSC          | \
        ETHIF_SEQBIT_UPDATE_SECY        | \
        ETHIF_SEQBIT_ADD_RXSA           | \
        ETHIF_SEQBIT_ADD_TXSA           | \
        ETHIF_SEQBIT_CTRL_PORT_ENABLED  | \
        ETHIF_SEQBIT_TXSA_ACTIVE        | \
        ETHIF_SEQBIT_GET_TXSA_NEXTPN    | \
        ETHIF_SEQBIT_UPDATE_RXSA_LAPN )

/* All bits that must be set before Updatesecy(TRUE) */
#define ETHIF_SEQBITS_UPDATE_SECY_REQ  ETHIF_SEQBIT_INIT_RXSC

/* All bits that must be set before Updatesecy(TRUE) */
#define ETHIF_SEQBITS_ADDRXSA_REQ  ( \
        ETHIF_SEQBIT_INIT_RXSC       | \
        ETHIF_SEQBIT_UPDATE_SECY )

#define ETHIF_SEQBITS_ADDTXSA_REQ  ( \
        ETHIF_SEQBIT_INIT_RXSC       | \
        ETHIF_SEQBIT_UPDATE_SECY )
/* ===================== SecY (Entity-Level) Tx Statistics ===================== */
typedef struct EthIf_StatsTxSecYType
{
    uint64 OutPktsUntagged;     /* Packets transmitted without a SecTAG */
    uint64 OutPktsTooLong;      /* Packets discarded due to exceeding max length */
    uint64 OutOctetsProtected;  /* Octets integrity protected but not encrypted */
    uint64 OutOctetsEncrypted;  /* Octets integrity protected and encrypted */
} EthIf_StatsTxSecYType;

/* ===================== SecY (Entity-Level) Rx Statistics ===================== */
typedef struct EthIf_StatsRxSecYType
{
    uint64 InPktsUntagged;      /* Packets without SecTAG received if ValidateFrames != STRICT */
    uint64 InPktsNoTag;         /* Packets without SecTAG discarded when ValidateFrames == STRICT */
    uint64 InPktsBadTag;        /* Packets discarded due to invalid SecTAG, PN=0, or bad ICV */
    uint64 InPktsNoSa;          /* Packets received with unknown SCI or unused SA */
    uint64 InPktsNoSaError;     /* Packets discarded due to unknown SCI or inactive SA */
    uint64 InPktsOverrun;       /* Packets discarded due to crypto performance overrun */
    uint64 InOctetsValidated;   /* Octets recovered (integrity protected, not encrypted) */
    uint64 InOctetsDecrypted;   /* Octets recovered (integrity protected and encrypted) */
} EthIf_StatsRxSecYType;


/* ===================== Secure Channel Tx Statistics ===================== */
typedef struct EthIf_StatsTxScType
{
    uint64 OutPktsProtected;    /* Integrity protected but not encrypted packets for this Tx SC */
    uint64 OutPktsEncrypted;    /* Integrity protected and encrypted packets for this Tx SC */
} EthIf_StatsTxScType;


/* ===================== Secure Channel Rx Statistics ===================== */
typedef struct EthIf_StatsRxScType
{
    uint64 InPktsOk;            /* Packets validated successfully within replay window */
    uint64 InPktsUnchecked;     /* Packets received if ValidateFrames == DISABLED */
    uint64 InPktsDelayed;       /* Packets received with PN lower than LowestPn when ReplayProtect == false */
    uint64 InPktsLate;          /* Packets discarded due to PN lower than LowestPn when ReplayProtect == true */
    uint64 InPktsInvalid;       /* Packets failed validation but accepted if ValidateFrames == CHECKED and not encrypted */
    uint64 InPktsNotValid;      /* Packets discarded due to failed validation when ValidateFrames == STRICT or encrypted */
} EthIf_StatsRxScType;


/* ===================== Aggregated MACsec Statistics ===================== */
typedef struct EthIf_MacsecStatisticsType
{
    EthIf_StatsTxSecYType TxSecYStats;  /* SecY transmission statistics */
    EthIf_StatsRxSecYType RxSecYStats;  /* SecY reception statistics */
    EthIf_StatsTxScType   TxScStats;    /* Secure Channel transmission statistics */
    EthIf_StatsRxScType   RxScStats;    /* Secure Channel reception statistics */
} EthIf_MacsecStatisticsType;


/* ===================== Secure Association Context ===================== */
typedef struct EthIf_MacsecSaContextType
{
    uint64     LowestPn;            /* Lowest acceptable Packet Number */
    uint64     MaxPn;               /* PN rollover threshold */
    uint64     NextPn;
    uint64     CipherSuite;         /* Cipher suite (AES-GCM-128 or AES-GCM-256) */
    uint32     ReplayWindow;        /* Replay protection window size */
    uint8      Sak[32];             /* Secure Association Key */
    uint8      HashKey[32];         /* Optional integrity key */
    uint8      Salt[12];            /* Salt for XPN nonce */
    uint8      Ssci[4];             /* Short SCI for XPN mode */
    uint8      KeyLength;           /* Length of SAK (16 or 32 bytes) */
    uint8      An;                  /* Association Number (0-3) */
    boolean    Active;              /* Is this SA active? */
    uint8      Padding;             /* Pdding */
} EthIf_MacsecSaContextType;


/* ===================== Controller MACsec Context ===================== */
typedef struct EthIf_ControllerMacsecContextType
{
  uint64                        CurrentCipherSuite;      /* Cipher suite negotiated by MKA */
  uint64                        RxSci;                   /* Secure Channel Identifier at Reception (SCI) */
  uint64                        TxSci;                   /* Secure Channel Identifier at transmission (SCI) */
  Mka_ValidateFramesType        ValidateFrames;
  Mka_ConfidentialityOffsetType ConfidentialityOffset;
  EthIf_MacsecStatisticsType    MacsecStats;             /* Aggregated Tx/Rx statistics for SecY and SC */
  EthIf_MacsecSaContextType     TxSa[ETHIF_MAX_TX_SA];   /* Transmit Secure Associations */
  EthIf_MacsecSaContextType     RxSa[ETHIF_MAX_RX_SA];   /* Receive Secure Associations */
  uint32                        ReplayWindow;            /* Replay protection window size (from MKA) */
  const uint16 *                BypassedVlanPtrs;
  const uint16 *                BypassedEtherTypesPtr;
  uint8                         BypassedVlansLength;
  uint8                         BypassedEtherTypesLength;
  uint8                         ActiveTxAnIndex;         /* Index of the currently active Tx SA */
  uint8                         ActiveRxAnIndex;         /* Index of the currently active Rx SA */
  boolean                       IsControlledPortEnabled; /* Controlled port state (runtime: open/closed) */
  boolean                       IsProtectFramesEnabled;  /* Enable MACsec protection for Tx frames */
  boolean                       IsReplayProtectEnabled;  /* Enable replay protection */
  boolean                       IsXpnEnabled;            /* Extended Packet Number (XPN) mode enabled */
  boolean                       IsMacSecOperational;     /* MacSec Operational status*/
    
} EthIf_ControllerMacsecContextType;

typedef struct STag_EthIf_SecTAGType
{
  uint8 ucTCIAN;
  uint8 ucShortLen;
  uint8 ucPacketNum[4];
  uint8 ucSecChanId[8];
} EthIf_SecTAGType;

#endif


/*******************************************************************************
**                         Global Data                                        **
*******************************************************************************/
#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
extern EthIf_StateType EthIf_GddInit;
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"
extern uint8 EthIf_GucTrcvEthIfCtrlIdx;
#define ETHIF_STOP_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"
extern uint8 EthIf_GucEthEthIfCtrlIdx;
#define ETHIF_STOP_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"
extern uint8 EthIf_GucTrcvLinkStateChgMainReload;
#define ETHIF_STOP_SEC_VAR_CLEARED_8
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
extern Eth_ModeType EthIf_GaaStatusController
  [ETHIF_NUM_OF_CONTROLLERS];
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
extern Eth_ModeType EthIf_GaaStatusPhysController
  [ETHIF_NUM_OF_PHYSICAL_CONTROLLERS];
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
extern Eth_ModeType EthIf_GaaStatusTrcv
  [ETHIF_NUM_OF_TRANSCEIVERS];
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

#if (ETHIF_NO_OF_TXPDUS > ETHIF_ZERO)
#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
extern EthIf_TxPduStatusType EthIf_GaaTxPduStat[ETHIF_NO_OF_TXPDUS];
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONST_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_TxPoolType EthIf_GaaTxPools[ETHIF_NO_OF_TXPOOLS];
#define ETHIF_STOP_SEC_CONST_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONST_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_TxPduType EthIf_GaaTxPdus[ETHIF_NO_OF_TXPDUS];
#define ETHIF_STOP_SEC_CONST_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif

#if (ETHIF_NO_OF_RXPDUS > ETHIF_ZERO)
#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
extern EthIf_RxPduStatusType EthIf_GaaRxPduStat[ETHIF_NO_OF_RXPDUS];
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONST_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_RxPoolType EthIf_GaaRxPools[ETHIF_NO_OF_RXPOOLS];
#define ETHIF_STOP_SEC_CONST_UNSPECIFIED
#include "EthIf_MemMap.h"

#if (ETHIF_RELEASERXBUFF_SUPPORT == STD_ON)
#define ETHIF_START_SEC_CONST_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const uint8 EthIf_RxPoolIdxMappingArray[ETHIF_RXPOOLIDX_MAP_SIZE];
#define ETHIF_STOP_SEC_CONST_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif

#define ETHIF_START_SEC_CONST_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_RxPduType EthIf_GaaRxPdus[ETHIF_NO_OF_RXPDUS];
#define ETHIF_STOP_SEC_CONST_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif

#if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
extern EthIf_ControllerMacsecContextType EthIf_GaaMacsecController
  [ETHIF_SW_MACSEC_CTRL_COUNT];
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"
extern EthIf_MacSecSeqBitsType EthIf_GaaMacSecSeqBits[ETHIF_SW_MACSEC_CTRL_COUNT];
#define ETHIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CONST_UNSPECIFIED
#include "EthIf_MemMap.h"
extern const EthIf_ICVHandlerType EthIf_ICVHandlers[ETHIF_SW_MACSEC_CTRL_COUNT];
#define ETHIF_STOP_SEC_CONST_UNSPECIFIED
#include "EthIf_MemMap.h"
#endif
/*******************************************************************************
**                              In-line Functions                             **
*******************************************************************************/

/*******************************************************************************
** Function Name        : EthIf_ReverseCopy                                   **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : This API Copies the bytes from Source to Destination**
**                        in Reverse Order                                    **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : LpSrc, LucDatalength                                **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : LpDest                                              **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):                                 **
**                                                                            **
**                        Function(s) invoked:                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0803                                      **
*******************************************************************************/
/* PRQA S 4640 1 */
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
static inline void EthIf_ReverseCopy(uint8 * LpDest, const uint8 * LpSrc, uint8 LucDatalength)
{
  uint8 LucIndex;
  uint8 * LpDstByte = LpDest + LucDatalength - 1;
  for (LucIndex = ETHIF_ZERO; LucIndex < LucDatalength; LucIndex++)
  {
    *LpDstByte-- = *LpSrc++;
  }
}
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

/*******************************************************************************
** Function Name        : EthIf_UintNToUnit8Arr                               **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : This API Copies the data from UintN type to Uint8   **
**                        array (Big Endian)                                  **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : LpSrc, LucLength, UintNSize                         **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : LpDest                                              **
**                                                                            **
** Return parameter     : void                                                **
**                                                                            **
** Preconditions        : EthIf_Init() should be invoked.                     **
**                                                                            **
** Remarks              : Global Variable(s):                                 **
**                                                                            **
**                        Function(s) invoked:                                **
**                                                                            **
** Design ID            : ETHIF_SDD_0804                                      **
*******************************************************************************/
static inline void EthIf_UintNToUnit8Arr(uint8 *LpDest, const void *LpSrc, uint16 LucLength, uint8 UintNSize)
{
  const uint8 * LpSrcByte;
  uint8 * LpDstByte;
  uint16 LucDataIdx;
  uint16 LucByteIdx;

  if (!((LpSrc == NULL_PTR) || (LpDest == NULL_PTR)))
  {
    LpSrcByte = (const uint8 *)LpSrc;
    for (LucDataIdx = 0; LucDataIdx < LucLength; LucDataIdx++)
    {
      #if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
      LpDstByte = LpDest;
      for (LucByteIdx = 0; LucByteIdx < UintNSize; LucByteIdx++)
      {
        *LpDstByte++ = *LpSrcByte++;
      }
      #else
      LpDstByte = LpDest + UintNSize - 1;
      for (LucByteIdx = 0; LucByteIdx < UintNSize; LucByteIdx++)
      {
        *LpDstByte-- = *LpSrcByte++;
      }
      #endif
      LpSrcByte += UintNSize;
      LpDest += UintNSize;
    }
  }
  ETHIF_UNUSED(LpSrcByte);
}

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

#if (ETHIF_HW_DISPLINCE_CLK_DISABLED == STD_OFF)
#if (STD_ON == ETHIF_PHC_SUPPORT_ENABLED)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType EthIf_SetPhcTime (uint8 CtrlIdx,
uint8 ClkUnitIdx, const TimeStampType* timeStampPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType EthIf_GetPhcTime (uint8 CtrlIdx,
uint8 ClkUnitIdx, TimeStampQualType* timeQualPtr,
TimeStampType* timeStampPtr);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType EthIf_SetPhcCorrection (uint8 CtrlIdx,
uint8 ClkUnitIdx, sint32 rateDeviation, sint32 offset);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType EthIf_SetPpsSignalMode (
uint8 CtrlIdx, uint8 ClkUnitIdx, boolean signalMode);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif
#endif

#if (ETHIF_METADATA_SUPPORT == STD_ON)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType EthIf_GetMetaDataInfo(EthIf_MetaDataInfo * MetaData,
  uint8 * MetaDataPtr, const uint8 LucMetaDataMask);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern void EthIf_PutMetaDataInfo(uint8 * MetaDatPtr,
  const uint8 LucMetaDataMask, const uint8* PhysAddrPtr,
  #ifdef ETHIF_RXINDICATION_INGRESSTIME_SUPPORT
  const TimeTupleType* IngressTimeTuplePtr,
  #endif
  boolean IsBroadcast);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

#if (ETHIF_MACSEC_SUPPORT_ENABLED == STD_ON)
#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern void EthIf_MacSecUpdateTxStats(uint8 LucCtrlIdx, uint16 LusDataLen,
                                      uint16 LusPacketLength);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern void EthIf_GenerateSecTAG(
  const EthIf_ControllerMacsecContextType * LpCtrlCtx,
  EthIf_MacsecSaContextType *LpTxSa, uint8 ** LpDataPtr, uint16 LusDataLen);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"

#define ETHIF_START_SEC_CODE
#include "EthIf_MemMap.h"
extern Std_ReturnType EthIf_GenerateICV(uint8 LucCtrlIdx,
                                 uint8 * LpDataPtr, uint32 LuldataLen);
#define ETHIF_STOP_SEC_CODE
#include "EthIf_MemMap.h"
#endif

#endif /* ETHIF_TYPES_H */
/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
