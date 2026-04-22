/*******************************************************************************
**                       Qorix Technologies Limited **
**                                                                            **
** Qorix Technologies Limited owns all the rights to this work. This **
** work shall not be copied,  reproduced,  used,  modified or its information **
** disclosed without the prior written authorization of Qorix Technologies **
** Limited.                                                                   **
**                                                                            **
**  SRC-MODULE:  Crc.h                                                        **
**                                                                            **
**  TARGET    :  All                                                          **
**                                                                            **
**  PRODUCT   :  AUTOSAR Crc Library Module                                   **
**                                                                            **
**  PURPOSE   :  Provision of API Prototype Declarations                      **
**                                                                            **
**  PLATFORM DEPENDANT [yes/no]: no                                           **
**                                                                            **
**  TO BE CHANGED BY USER [yes/no]: no                                        **
**                                                                            **
**  Design ID :   CRC_SDD_0062                                                **
*******************************************************************************/

/*******************************************************************************
**                      Revision History                                      **
********************************************************************************
**  Date            By                  Description                           **
********************************************************************************
** 2-Jun-2025    Samarth G         Update for Feature Release                 **
** 24-Dec-2025   Samarth G         Update for Feature Release                 **
** 12-Dec-2025   Samarth G         Update for Feature Release                 **
** 10-Dec-2025   Samarth G         Updated Version for bugfix release         **
** 25-Nov-2025   Samarth G         Updated Version for bugfix release         **
** 21-Nov-2025   Samarth G         Update for Feature Release                 **
** 19-11-2025    Samarth G         Updated Version for bugfix release         **
**                                                                            **
**                                                                            **
**  15-Nov-25     Samarth G         1. Limitation section has been updated    **
**                                                                            **
**                                                                            **
**  12-Sep-25     Samarth G           Initial Version R24-11 **

*******************************************************************************/
#ifndef MKA_TYPES_H
#define MKA_TYPES_H

#include "Mka_Cfg.h"
#include "Mka_InternalTypes.h"
#include "Platform_Types.h"

typedef uint16 EtherType;
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
#define MKA_MEMBER_IDENTIFIER_LENGTH (12U)

#define MKA_ICV_LENGTH (16U)
#define MKA_TWENTY_FOUR (0x18U)
/*******************************************************************************
**                       Function Prototypes                                  **
*******************************************************************************/
/* Controls validation of received frames */
typedef enum ETag_Mka_ValidateFramesType {
  /* Disable validation, remove SecTAGs and ICVs (if present) from received
     frames. */
  MKA_VALIDATE_DISABLED = 0,

  /* Enable validation, do not discard invalid frames */
  MKA_VALIDATE_CHECKED,

  /* Enable validation and discard invalid frames */
  MKA_VALIDATE_STRICT
} Mka_ValidateFramesType;

/* Indicates the offset in case of integrity with confidentiality */
typedef enum ETag_Mka_ConfidentialityOffsetType {
  /* Confidentiality protection disabled */
  MKA_CONFIDENTIALITY_NONE = 0,

  /* Zero initial octets of each user data without confidentiality protection */
  MKA_CONFIDENTIALITY_OFFSET_0,

  /* 30 initial octets of each user data without confidentiality protection */
  MKA_CONFIDENTIALITY_OFFSET_30,

  /* 50 initial octets of each user data without confidentiality protection */
  MKA_CONFIDENTIALITY_OFFSET_50
} Mka_ConfidentialityOffsetType;

/* Structure to configure a referred SecY in EthIf/EthTrcv */
typedef struct STag_Mka_MacSecConfigType {
  /* Indicates status if the MACsec protection of the frames is active or not */
  boolean ProtectFrames;

  /* Indicates status if replay protection is enable or disable */
  boolean ReplayProtect;

  /* If ReplayProtect is enable, indicates the used replay protect window */
  uint32 ReplayWindow;

  /* Status of the validation of the frames. See Mka_ValidateFramesType for
   * possible values */
  Mka_ValidateFramesType ValidateFrames;

  /* Indicates which cipher suite is used in the SecY to update. */
  uint64 CurrentCipherSuite;

  /* Set the Confidentiality Offset. See Mka_ConfidentialityOffsetType for
   * possible values  */
  Mka_ConfidentialityOffsetType ConfidentialityOffset;

  /* Status if the controlled port is enabled or disabled */
  boolean ControlledPortEnabled;

  /* Pointer to the list of bypassed VLANs */
  const uint16 *BypassedVlanPtrs;

  /* Length of the list of bypassed VLANs */
  uint8 BypassedVlansLength;

  /* Pointer to the list of the bypassed Ethernet Types */
  const uint16 *BypassedEtherTypesPtr;

  /* Length of the list of the bypassed Ethernet Types */
  uint8 BypassedEtherTypesLength;
} Mka_MacSecConfigType;

/* Description MACsec Entity (SecY) transmission statistics */
typedef struct STag_Mka_Stats_Tx_SecYType {
  /* The number of packets transmitted without a SecTAG because secy
   * ProtectFramesEnable is configured false */
  uint64 OutPkts_Untagged;

  /* The number of transmitted packets discarded because their length is
  greater than the accepted maximum length (mtu) of the Port */
  uint64 OutPkts_TooLong;

  /* The number of plain text octets integrity protected but not encrypted in
  transmitted frames */
  uint64 OutOctets_Protected;

  /* The number of plain text octets integrity protected and encrypted in
  transmitted frames */
  uint64 OutOctets_Encrypted;
} Mka_Stats_Tx_SecYType;

/* Description MACsec Entity (SecY) reception statistics */
typedef struct STag_Mka_Stats_Rx_SecYType {
  /* The number of packets without the MACsec tag (SecTAG) received if
  Mka_ValidateFrames was not ’MKA_VALIDATE_STRICT’ */
  uint64 InPkts_Untagged;

  /* The number of received packets without a SecTAG discarded because
  Mka_ValidateFrames was ’MKA_VALIDATE_STRICT’ */
  uint64 InPkts_NoTag;

  /* The number of received packets discarded with an invalid SecTAG,
  zero value PN, or invalid ICV */
  uint64 InPkts_BadTag;

  /* The number of received packets with an unknown SCI or for an
  unused SA by the security entity */
  uint64 InPkts_NoSa;

  /* The number of packets discarded because the received SCI is
  unknown or the SA is not in use */
  uint64 InPkts_NoSaError;

  /* The number of packets discarded because they exceeded
  cryptographic performance capabilities */
  uint64 InPkts_Overrun;

  /* The number of plaintext octets recovered from packets that were
  integrity protected but not encrypted */
  uint64 InOctets_Validated;

  /* The number of plaintext octets recovered from packets that were
  integrity protected and encrypted */
  uint64 InOctets_Decrypted;
} Mka_Stats_Rx_SecYType;

/* Secure Channel transmission statistics */
typedef struct STag_Mka_Stats_Tx_ScType {
  /* The number of integrity protected but not encrypted packets for this
   * transmit SC */
  uint64 OutPkts_Protected;

  /* The number of integrity protected and encrypted packets for this transmit
   * SC */
  uint64 OutPkts_Encrypted;
} Mka_Stats_Tx_ScType;

/* Secure Channel reception statistics */
typedef struct STag_Mka_Stats_Rx_ScType {

  /* The number of packets received for this secure channel successfully
  validated and within the replay window */
  uint64 InPkts_Ok;

  /* The number of packets received for this secure channel, if Mka_
  ValidateFrames was ’MKA_VALIDATE_DISABLED’ */
  uint64 InPkts_Unchecked;

  /* The number of received packets, for this secure channel, with packet
  number (PN) lower than the lowest acceptable packet number (Lowest
  Pn) and ReplayProtect was false */
  uint64 InPkts_Delayed;

  /* The number of packets discarded, for this secure channel, because
  the received packet number (PN) was lower than the lowest acceptable
  packet number (LowestPn) and ReplayProtect was true */
  uint64 InPkts_Late;

  /* The number of packets, for this secure channel, that failed validation
  but could be received because ValidateFrames was ’MKA_VALIDATE_
  CHECKED’ and the data was not encrypted (so the original frame
  could be recovered) */
  uint64 InPkts_Invalid;

  /* The number of packets discarded, for this secure channel, because
  validation failed and ValidateFrames was ’MKA_VALIDATE_STRICT’ or
  the data was encrypted (so the original frame could not be recovered) */
  uint64 InPkts_NotValid;
} Mka_Stats_Rx_ScType;

/* SAK key reference */
typedef struct STag_Mka_SakKeyPtrType {
  /* Pointer to the Hash Key */
  const uint8 *HashKeyPtr;

  /* Pointer to the SAK */
  const uint8 *SakKeyPtr;

  /* Pointer to the Salt */
  const uint8 *SaltKeyPtr;
} Mka_SakKeyPtrType;

/* Security Entity statistics */
typedef struct STag_Mka_Stats_SecYType {
  /* Set of statistics in the Security Entity Phy by transmision */
  Mka_Stats_Tx_SecYType StatsTxPhy;

  /* Set of statistics in the Security Entity Phy by reception */
  Mka_Stats_Rx_SecYType StatsRxPhy;

  /* Set of statistics in the Security Entity’s Secure Channel by reception */
  Mka_Stats_Tx_ScType StatsTxSc;

  /* Set of statistics in the Security Entity’s Secure Channel by reception */
  Mka_Stats_Rx_ScType StatsRxSc;
} Mka_Stats_SecYType;

/* Status of the MKA instance. */
typedef enum ETag_Mka_MkaStatus {
  /* MKA session key has been agreed and MACsec link is currently up */
  MKA_STATUS_MACSEC_RUNNING = 0,

  /* MKA is waiting for a link up of the underlying device to begin negotiation
   */
  MKA_STATUS_WAITING_PEER_LINK,

  /* MKA is waiting for a remote peer to transmit MKPDU’s to begin negotiation
   */
  MKA_STATUS_WAITING_PEER,

  /* MKA negotiation is ongoing */
  MKA_STATUS_IN_PROGRESS,

  /* MKA negotiation is not possible because
  ICV’s of remote peer are invalid (ICK and
  therefore CAK keys are different) */
  MKA_STATUS_AUTH_FAIL_UNKNOWN_PEER,

  /* Undefined state, reported when the given bus is disabled */
  MKA_STATUS_UNDEFINED = 0xFF
} Mka_MkaStatus;

typedef enum ETag_Mka_MkaInternalStatusType {

  /*Mka Call to check availablity of CAK */
  /* MKA session key has been agreed and MACsec link is currently up */
  MKA_STATUS_WAITING_PEER_ICK_DERIVATION_INPROGRESS = 0,

  /* MKA is waiting for a link up of the underlying device to begin negotiation
   */
  MKA_STATUS_WAITING_PEER_KEK_DERIVATION_INPROGRESS,

  /* MKA is waiting for a remote peer to transmit MKPDU’s to begin negotiation
   */
  MKA_STATUS_WAITING_PEER_SAK_DERIVATION_INPROGRESS,

  /* MKA negotiation is ongoing */
  MKA_STATUS_WAITING_PEER_ICV_DERIVATION_INPROGRESS,

  /*Between operational and Running*/
  MKA_STATUS_IN_PROGRESS_NOTIFICATION,

  MKA_STATUS_IN_PROGRESS_KAY_SELECTED,

  MKA_INTR_STATE_MKA_INITIALIZED,
  MKA_INTR_STATE_ICK_WAITING,
  MKA_INTR_STATE_ICK_COMPLETED,
  MKA_INTR_STATE_KEK_WAITING,
  MKA_INTR_STATE_KEK_COMPLETED,
  MKA_INTRA_STATE_LINKUP_COMPLETED,
  MKA_INTRA_STATE_TX_ADV_FRAME_SENT,
  MKA_INTR_STATE_TX_ADV_FRAME_COMPLETED,
  MKA_INTR_STATE_RX_ADV_FRAME_RECEIVED,
  MKA_INTR_STATE_RX_ADV_FRAME_VALIDATED,
  MKA_INTR_STATE_KAY_PART_SELECTION_COMPLETED,
  MKA_INTR_STATE_PP_FRAME_SENT,
  MKA_INTR_STATE_LP_FRAME_RECEIVED,
  MKA_INTR_STATE_LP_FRAME_VALIDATED,
  MKA_INTR_STATE_SAK_WAITING,
  MKA_INTR_STATE_SAK_COMPLETED,
  MKA_INTR_STATE_SAK_FRAME_SENT,
  MKA_INTR_STATE_SA_COMPLETED,
  MKA_INTR_STATE_SA_OPERATIONAL,
  MKA_INTR_STATE_SA_STATS_WAITING,
  MKA_INTR_STATE_SA_STATS_COMPLETED,
  MKA_INTR_STATE_SAK_REKYING_WAITING,
  MKA_INTR_STATE_SAK_REKYING_COMPLETED,
  MKA_INTR_STATE_RANDOM_NUMBER_GENERATION_IN_PROGRESS,
  MKA_INTR_STATE_RANDOM_NUMBER_GENERATION_COMPLETED

} Mka_MkaInternalStatusType;

/* Structure with the specific information of a PAE */
typedef struct STag_Mka_PaeStatusType {
  /* Status of the MKA */
  Mka_MkaStatus ConnectionStatus;

  /* SCI includes the peer’s MAC and port */
  uint64 PeerSci;

  /* CKN used for the establishment of the MACsec Secure Channel */
  unsigned char CknInUse[32];
} Mka_PaeStatusType;

// typedef struct STag_Mka_PaeConfigurationType
//{
//    /*Dummy Tyoe*/
//    uint16 type;
//
//
// }Mka_PaeConfigurationType;

// typedef struct STag_Mka_PaeInstanceType
//{
//    /*Dummy Tyoe*/
//    uint16 type;
//
//
// }Mka_PaeInstanceType;

typedef struct STag_Mka_ConfigType {
  /* Implementation specific structure of the post build configuration */
  Mka_PaeConfigurationType Dummy;

  Mka_PaeInstanceType *PaeInstance;

  /* No of PaeInstance */
  uint8 PaeInstanceSize;
} Mka_ConfigType;

typedef struct STag_Mka_TagCI_Type {
  uint8 blversion : 1;
  uint8 blendstation : 1;
  uint8 blsecureChannel : 1;
  uint8 blsinglecpybrdcst : 1;
  uint8 blencryption : 1;
  uint8 blchagetext : 1;
  uint8 association_number : 2;

} Mka_TagCI_Type;
typedef struct STag_Mka_ShortLeanghtType {
  uint8 shortleangth : 6;
  uint8 reserved : 2;
} Mka_ShortLeanghtType;

typedef struct STag_Mka_SecTagType {
  EtherType MacSecEtherTypeOctet;
  Mka_TagCI_Type MacSecTciOctet;
  Mka_ShortLeanghtType MacSecTag_ShortLength;
  uint32 MacSec_Packet_Number;
  uint32 MacSec_GaaSCI[2];

} Mka_SecTagType;

typedef enum ETag_Mka_Peer_Server_StatusType {
  MKA_STATUS_SEVER_ELECTION_ENABLE = 0,
  MKA_STATUS_SERVER_ELECTION_DISABLE

} Mka_Peer_Server_StatusType;

typedef enum ETag_Mka_Kay_StatusType {
MKA_INTR_IN_PROGRESS,
MKA_INTR_WAIT_FOR_RANDOM,
MKA_INTR_RANDOM_COMPLETE,
MKA_INTR_ICV_GEN_COMPLETE,
MKA_INTR_WAIT_FOR_ICV,
MKA_INTR_NEGO_IN_PROGRESS,
MKA_INTR_WAIT_FOR_BP_SET,
MKA_INTR_WAIT_FOR_BP_RECEIVED,
MKA_INTR_WAIT_WRAP,
MAK_INTR_WRAP_COMPLETE,
MKA_INTR_WAIT_UNWRAP,
MKA_INTR_UNWRAP_COMPLETE,

MKA_INTR_WAIT_FOR_SAK,
MKA_INTR_SAK_GEN_COMPLETE,
MKA_INTR_POTENTIAL,
MKA_INTR_LIVE,
MKA_INTR_KEY_SERVER,
MKA_INTR_SECURED,
MKA_INTR_RETIRE,

MKA_INTR_CKN_CHECK,
MKA_INTR_ICV_VERIFY,
MKA_INTR_FRAME_CHECK,
MKA_INTR_FRAME_BASIC,
MKA_INTR_USE_PARA,
MKA_INTR_DISTRIBUTED,
MKA_INTR_PDU_CREATE,

MKA_INTR_INITIAL,
MKA_INTR_ICK_COMPLETE,
MKA_INTR_KEK_COMPLETE,
MKA_INTR_WAIT_FOR_ICK,
MKA_INTR_WAIT_FOR_KEK,
MKA_INTR_DISCARD,

MKA_INTR_KAY_RUNNIG


} Mka_Kay_StatusType;

typedef struct STag_Mka_PaeInstaceStatusType {
  Mka_Peer_Server_StatusType ddPee_Server_status;
  Mka_MkaInternalStatusType ddServerInternalState;

  Mka_MkaStatus ddpaetatus;

} Mka_PaeInstaceStatusType;

typedef struct STag_Mka_KaYParticipatInstanceType {

  char CKN[32];
  boolean isConfigured;
  boolean isEnabled;

} Mka_KaYParticipatInstanceType;
typedef struct STag_Mka_MkPduType{
  uint8 MkPdu[1000];
  uint8 length;
}Mka_MkPduType;
#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_PaeInstaceStatusType
    Mka_GaaPaeServerStatus[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_Kay_StatusType MKa_GaaKayStatus[MKA_TOTAL_NUM_KAY_PARTICIPANT];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_KaYParticipatInstanceType
    Mka_GaaKayParticipant[MKA_TOTAL_NUM_KAY_PARTICIPANT];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_BasicParameterSet_Type
    Mka_GaaBasicparameterset[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_AnouncementSet_Type
    Mka_GaaAnouncementParameterSet[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern uint8 Mka_GaaICV[MKA_TOTAL_NUM_MKA_PAE_INSTANCE][MKA_ICV_LENGTH];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_PeerList_type Mka_GaaPeerPoteList[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern uint32 Mka_BascsicParameterMemberNumber[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_Eapol_Type Mka_Eapol_parameterSet[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_PeerList_type Mka_PotentialPeerList[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_PeerList_type Mka_LivePeerList[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_Distributed_SAK_type
    Mka_DistributedSak[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_Use_ParamterSet_Type
    Mka_UseParameterSetSak[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern uint8 MKa_CipherText[MKA_TWENTY_FOUR];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern boolean Mka_InternalLinkUpState[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern  Mka_Selected_KayParticipantType 
                               Mka_Selectedparticipant;
 #define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_TLV_Type Mka_MacsecTLV[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_JobRegistryType Mka_JobRegistry[MKA_TOTAL_NUM_KAY_PARTICIPANT];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
 extern uint8 Mka_JobRegistyCounter[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
 extern uint8 Mka_MI_Value[MKA_TOTAL_NUM_KAY_PARTICIPANT][12];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"


#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_MkPduType Mka_Pdu[MKA_TOTAL_NUM_KAY_PARTICIPANT] ;
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"



#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern uint8 Mka_IcvGen[MKA_TOTAL_NUM_KAY_PARTICIPANT][MKA_ICV_LENGTH] ;
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"


#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

 extern uint8 MKA_PortPhysicalAddress[MKA_TOTAL_NUM_KAY_PARTICIPANT];

#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

/* */
#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

extern Mka_FrameType Frame_list_type[MKA_TOTAL_NUM_KAY_PARTICIPANT];

#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"


#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
extern Mka_MacSecConfigType Mka_GaaMacSecConfig[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"



#endif /* MKA_TYPES_H */

/*******************************************************************************
**                          End of File                                       **
*******************************************************************************/
