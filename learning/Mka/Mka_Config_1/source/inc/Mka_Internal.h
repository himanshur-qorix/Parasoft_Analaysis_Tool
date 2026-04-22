/*******************************************************************************
**                       Qorix Technologies Limited **
**                                                                            **
** Qorix Technologies Limited owns all the rights to this work. This **
** work shall not be copied,  reproduced,  used,  modified or its information **
** disclosed without the prior written authorization of Qorix Technologies **
** Limited.                                                                   **
**                                                                            **
**  SRC-MODULE:  Mka_Internal.h                                                        **
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
**  Design ID :   MKA_SDD_xxxx                                                **
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
**  15-Nov-25     Samarth G          Limitation section has been updated      **
**                                                                            **
**                                                                            **
**  12-Sep-25     Samarth G           Initial Version R24-11	                **

*******************************************************************************/
#ifndef MKA_INTERNAL_H
#define MKA_INTERNAL_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Mka.h"
#include "Dem.h"
#include "Eth_GeneralTypes.h"
#include "Platform_Types.h"
#include "Std_Types.h"





/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/
#define MKA_VERSION_AR_RELEASE_MAJOR_VERSION 4
#define MKA_VERSION_AR_RELEASE_MINOR_VERSION 10
#define MKA_VERSION_AR_RELEASE_REVISION_VERSION 0

#define MKA_VERSION_SW_MAJOR_VERSION 0
#define MKA_VERSION_SW_MINOR_VERSION 5
#define MKA_VERSION_SW_PATCH_VERSION 1

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
#define MKA_TRUE (0x01U)
#define MKA_FALSE (0x00U)

#define MKA_ZERO (0x00U)
#define MKA_ONE (0x01U)
#define MKA_TWO (0x02U)
#define MKA_THREE (0x3U)
#define MKA_FOUR (0x4U)
#define MKA_FIVE (0x05)
#define MKA_SIX (0x06U)
#define MKA_SEVEN (0x07U)
#define MKA_EIGHT (0x08U)
#define MKA_NINE  (0x09U)
#define MKA_TEN (0x0AU)
#define MKA_TWELEVE (0x0CU)

#define MKA_FIFTEEN (0x0F)
#define MKA_SIXTEEN (0x10)
#define MKA_TWENTY_FOUR (0x18U)
#define MKA_THIRTY_TWO (0x20)

#define MKA_SIXTY_EIGHT (0x44U)

#define MKA_TWO_FIFTY_FIVE (0xFFU)

#define MKA_MAXIMUM_LENGTH_OF_AMN (0xFFFF)
#define MKA_MASK 0x0FFFU

#define MKA_DEM_INVALID (0x00U)

#define MKA_INSTANCE_ID (0x00U)
#define MKA_LINK_STATE_SET_ID (0x1DU)
#define MKA_E_INVALID_PARAMETER (0x04)

#define MKA_MACSEC_FRAME_TYPE (0x88E5)

#define MKA_KEK_LABEL "IEEE8021 KEK"
#define MKA_ICK_LABEL "IEEE8021 ICK"
#define MKA_SAK_LABEL "IEEE8021 SAK"

#define MKA_UNUSED(x) ((void)(x))

#define EAPOL_VERSION_3 (0x03U)
#define EAPOL_TYPE_MKA (0x05U)
#define EAPOL_TYPE_MACSEC_ANNOUNCEMENT (0x70U)
#define MKA_BASIC_PARAMSET_TYPE (0x00U)
#define MKA_VERSION_BASCIC_PARAMETER (0x01U)
#define MKA_KEY_SERVER_PRIORITY (0x01U)
#define MKA_CALLOUT_KEY_LENGTH (0x80)

/* Algorithm Agility (AES-128) — IEEE 802.1X Figure 11-8 */
#define MKA_ALGO_AGILITY_AES128_0 (0x00U)
#define MKA_ALGO_AGILITY_AES128_1 (0x00U)
#define MKA_ALGO_AGILITY_AES128_2 (0x00U)
#define MKA_ALGO_AGILITY_AES128_3 (0x01U)

#define MKA_ALGO_AGILITY_STANDARD (0x0080C201)

#define MKA_CKN_LENGTH (0x10U)

#define MKA_EAPOL_BASIC_PDU_LENGTH   (0x44)
#define MKA_EAPOL_BASIC_Potential_PDU_LENGTH  (0x58)
#define MKA_EAPOL_LIVE_Potential_PDU_LENGTH  (0x5A)
#define MKA_EAPOL_DISTRI_USE_LIVE_PDU_LENGTH (0xA8)
/* Parameter sets */
#define MKA_PARAMSET_LIVE_PEER_LIST (0x01U)
#define MKA_PARAMSET_POTENTIAL_PEER_LIST (0x02U)
/**/

/* ICV length */
#define MKA_ICV_LENGTH (16U)
#define MKA_MEMBER_IDENTIFIER_LENGTH (12U)

#define MKA_E_KEY_MISMATCH_INSTANCE(LpMkaPaeInstance)                          \
  do {                                                                         \
    if (MKA_DEM_INVALID != (LpMkaPaeInstance)->usMkaEKeyMismatchInstance) {    \
      (void)Dem_SetEventStatus((LpMkaPaeInstance)->usMkaEKeyMismatchInstance,  \
                               (Dem_EventStatusType)DEM_EVENT_STATUS_FAILED);  \
    }                                                                          \
  } while (0)

#define MKA_E_ALGO_MISMATCH_INSTANCE(LpMkaPaeInstance)                          \
  do {                                                                         \
    if (MKA_DEM_INVALID != (LpMkaPaeInstance)->usMkaEAlogMismatchInstance) {    \
      (void)Dem_SetEventStatus((LpMkaPaeInstance)->usMkaEAlogMismatchInstance,  \
                               (Dem_EventStatusType)DEM_EVENT_STATUS_FAILED);  \
    }                                                                          \
  } while (0)

/*******************************************************************************
**                       Function Prototypes                                  **
*******************************************************************************/

extern Std_ReturnType Mka_StartSeverElection(void);

extern Std_ReturnType Mka_StartNegotiation(uint8 PaeIndx);

extern void Mka_ProcessMkPdu(uint8 PaeIdx, const uint8 *LpSduDataPtr,
                             uint16 LenBytes,uint8* offset_main);

extern Std_ReturnType
Mka_CknMatching(Mka_BasicParameterSet_Type *Basic_pramtersetType,
                const uint8 *CKN);

extern Std_ReturnType
Mka_IcvMatching(Mka_BasicParameterSet_Type *Basic_pramtersetType,
                const uint8 *CKN, uint8 PaeIdx,
                Mka_PaeInstanceType const *LpMkaPaeInstance, uint8 *mka_icv,
                const uint8 *PhysAddrPtr);

void Mka_PreparePotentialList(uint8 PaeIndx, const uint8 *PhysAddrPtr);
void   Mka_ProcessPotentialPeerList(uint8 PaeIdx, const uint8 *LpSduDataPtr,
                             uint16 LenBytes,uint8* offset_main);
void   Mka_ProcessLivePeerList(uint8 PaeIdx, const uint8 *LpSduDataPtr,
                             uint16 LenBytes,uint8* offset_main);
void   Mka_ProcessDistributedParamter(uint8 PaeIdx, const uint8 *LpSduDataPtr,
                             uint16 LenBytes,uint8* offset_main);
void   Mka_ProcessUseParamterSet(uint8 PaeIdx, const uint8 *LpSduDataPtr,
                             uint16 LenBytes,uint8* offset_main);

void Mka_PeerPduProcess(uint8 PaeIdx, const uint8 *PhysAddrPtr,
                        Eth_FrameType FrameType,
                        Mka_PaeInstanceType const *LpMkaPaeInstance);

void Mka_ServerStateMachine(uint8 Paeidx);
void Mka_ClientStateMachine(
                            uint8 Paeidx);

void Mka_PrepareLivePeerList(Mka_PaeInstanceType const *LpMkaPaeInstance,
                             uint8 Paeidx, uint8 *LpBuffPtr,uint8 *offset_ptr);

void Mka_PreparePotentialPeerList(Mka_PaeInstanceType const *LpMkaPaeInstance,
                                  uint8 Paeidx, uint8 *LpBuffPtr,uint8 *offset_ptr);

void Mka_PrepareDistributedSAKList(Mka_PaeInstanceType const *LpMkaPaeInstance,
                             uint8 Paeidx, uint8 kay,uint8 *LpBuffPtr,uint8 *offset_ptr);
void Mka_PrepareUseParaList(Mka_PaeInstanceType const *LpMkaPaeInstance,
                             uint8 Paeidx, uint8 *LpBuffPtr,uint8 *offset_ptr);

void  Mka_CreateBasic_parameter(uint8 PaeIndx, uint8 *LpBuffptr,uint8 *offset_ptr,uint16 *LusTotalLen);
void Mka_Create_TxPdu(uint8 paeindix,uint8 kay,uint8 mkaframe,const uint8 *PhysAddrPtr);
void Mka_PrepareMacsecTLVPeerList(Mka_PaeInstanceType const *LpMkaPaeInstance,Mka_KayParticipants const *LpMkaKayParticipants,
                                  uint8 Paeidx, uint8 *LpBuffPtr,
                                  uint8 *offset_ptr);
Std_ReturnType   Mka_Transmission(uint8 PaeIdx, const uint8 *LpSduDataPtr,
                             uint16 LenBytes,uint16 bufferId,const uint8* addr);

void Mka_CreateICV(uint8 paeindx, uint8 KayId,uint16 *LusTotalLen,uint8 * LpBuffptr,uint8 * offset_ptr);



 boolean Mka_CheckforConnectivityAssociation(uint8 paeindi,uint8 *Mka_Capability);

void  Mka_ProcessICVIndicatior(uint8 PaeIdx,const uint8 *LpSduDataPtr,uint16 LenByte, uint8 *Offset_Ptr);
void  Mka_ProcessMacSecTLV(uint8 PaeIdx,const uint8 *LpSduDataPtr,uint16 LenByte, uint8 *Offset_Ptr);
                             
void Mka_Intr_be32(const uint8 *data, uint32 *out, uint16 *len, uint8 *offset);
void Mka_Intr_be64(const uint8 *data, uint64 *out, uint16 *len, uint8 *offset);
void Mka_Intr_be7bit(const uint8 *data, uint8 *out, uint16 *len, uint8 *offset);
void Mka_Intr_be16(const uint8 *data, uint16 *out, uint16 *len, uint8 *offset);
void  Mka_CipherSuiteSelection(void);
void Mka_MainHandler(uint8 PaeInst );
void Mka_HandleInternalStateWaitingPeerLink(uint8 PaeInst);
void Mka_HandleInternalStateWaitingPeer(uint8 PaeInst);
void Mka_HandleInternalStateInProgress(uint8 PaeInst);
void Mka_HandleInternalStateAuthFailUnknownPeer(uint8 PaeInst);
void Mka_HandleInternalStateMacsecRunning(uint8 PaeInst);
void  Mka_InternalAdvframesent(uint8 PaeInst);
void   Mka_Internalppstatesent(uint8 PaeInst);
void   Mka_InternallpstateRec(uint8 PaeInst);
void   Mka_InternalSakwaiting(uint8 PaeInst);
void   Mka_InternalSakCompleted(uint8 PaeInst);
void Mka_InternalKayPartSel(uint8 PaeInst);

Std_ReturnType Mka_CheckCAKAvailability(uint8 PaeInst);
Std_ReturnType Mka_ICKCallhandler(uint8 KayId,uint8 paeid);
Std_ReturnType Mka_KEKCallhandler(uint8 KayId,uint8 paeid);
/*consistent name change of paeid*/
Std_ReturnType Mka_GetLinkUpsate(uint8 paeid,boolean *LinkUpstatus);

boolean Mka_SelectCiphersuites(uint8 paeid,uint8 * Selected_Cipher, Mka_KayParticipants const *LpMkaKayParticipants);
void Mka_RandomNumberGeneration(uint8 KayId,uint8 paeId);
void Mka_BuildMkPduWithoutICV(uint8 KayId,uint8 PaeInst);
void Mka_Paramtersethandler(uint8 PaeInst,uint8 KayId);

void Mka_Icvhandler (uint8 PaeInst,uint8 kayId);
void Mka_CknCheckHandler(uint8 paeId, uint8 kayId);
void Mka_IcvVerifyHandler(uint8 paeId, uint8 kayId);
void Mka_FrameCheck(uint8 paeId, uint8 kayId);
void Mka_Potentialhandler(uint8 PaeInst,uint8 kayId);
void Mka_LiveHandler(uint8 PaeInst, uint8 Luscount);
void  Mka_UseHandler(uint8 PaeInst,uint8 Luscount);
 void Mka_DistributedHandler(uint8 PaeInst,uint8 Luscount);
 void  Mka_BasicHandler(uint8 PaeInst, uint8 Luscount);
 void   Mka_BuildMkPduPotentialWithoutICV(uint8 kayId,uint8 PaeInst);
 void   Mka_BuildMkPduLiveWithoutICV(uint8 kayId,uint8 PaeInst);
  void Mka_SakGenerationHandler(uint8 PaeId,uint8  KayId);


#endif /* CRC_H */

/*******************************************************************************
**                          End of File                                       **
*******************************************************************************/
