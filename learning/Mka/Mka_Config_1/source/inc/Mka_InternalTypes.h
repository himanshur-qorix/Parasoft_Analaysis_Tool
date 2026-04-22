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
**  12-Sep-25     Samarth G           Initial Version R24-11	                **

*******************************************************************************/
#ifndef MKA_INTERNALTYPES_H
#define MKA_INTERNALTYPES_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

#include "Mka_Cfg.h"

/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/
#define NUMBER_OF_CIPHERSUITES_CONFIGURED    MKA_TOTAL_NUM_CIPHER_SUITES

#define MACSEC_MAX_SIZE (0x0A * NUMBER_OF_CIPHERSUITES_CONFIGURED)
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/* Design_ID          : MKA_SDD_XXXX  */

/*#define MKA_THREE 0x03
#define MKA_CKN_LENGTH (16U)*/

/*******************************************************************************
**                       Function Prototypes                                  **
*******************************************************************************/

/* Structure to hold MKA PAE Configuration */
// typedef struct STag_Mka_PaeConfigurationType {
//   boolean blMkaAutoStart;
//   uint8 ucMkaPaeConfigurationIdx;
//   uint16 usMkaRetryBaseDelay;                    /* type chnage derive data
//   type to uint16 */ uint16 usMkaRetryCyclicDelay;                  /* type
//   chnage derive data type to uint16 */ uint16 usMkaSakRekeyTimeSpan; /* type
//   chnage derive data type to uint16 */
// }Mka_PaeConfigurationType;

/* Design_ID          : MKA_SDD_XXXX */
/* Structure to hold MKA KayParticipant reference */
// typedef struct STag_Mka_KayParticipants {
//
//   /* ref of the MkaCryptoAlgoConfigIdx from MkaCryptoAlgoConfig  */
//  	uint8 ucMkaCryptoAlgoRefConfigIdx;
//
//   /* ref of the CsmKeyId  from CsmKeys  */
//  	uint32 usMkaCryptoCknCakKeyRefCsmKeyId;
//
//   /* ref of the CsmJobId from CsmJobs  */
//  	uint32 usMkaCryptoIckDeriveJobRefCsmJobId;
//
//   /* ref of the CsmJobId from CsmJobs  */
//  	uint32 usMkaCryptoIcvGenerateJobRefCsmJobId;
//
//   /* ref of the CsmJobId from CsmJobs  */
//  	uint32 usMkaCryptoIcvVerifyJobRefCsmJobId;
//
//   /* ref of the CsmJobId from CsmJobs  */
//  	uint32 usMkaCryptoKekDeriveJobRefCsmJobId;
//
//   /* ref of the CsmJobId from CsmJobs  */
//   uint32 usMkaCryptoKeyUnwrapJobRefCsmJobId;
//
//   /* ref of the CsmJobId from CsmJobs  */
//  	uint32 usMkaCryptoKeyWrapJobRefCsmJobId;
//
//   /* ref of the CsmJobId from CsmJobs  */
//  	uint32 usMkaCryptoRandomJobRefCsmJobId;
//
//   /* ref of the CsmKeyId  from CsmKeys  */
//  	uint32 usMkaCryptoSakKeyRefCsmKeyId;
//
//  	boolean blMkaParticipantActivate;
// }Mka_KayParticipants;

/* Design_ID          : MKA_SDD_XXXX */
/* Enum to hold Mka_RoleType */
// typedef enum ETag_Mka_RoleType
//{
//	MKA_KEY_SERVER = 0,
//	MKA_PEER = 255
// }Mka_RoleType;

/* Design_ID          : MKA_SDD_XXXX */
/* Structure to hold MKA PAE Instances */
// typedef struct STag_Mka_PaeInstanceType {
//   uint8 ucMkaEthIfControllerIndex;
//  	const uint16 *pMkaBypassEtherType;               /* this parameter under
//  MkaKay container */ 	const uint16 *pMkaBypassVlan; /* this parameter
//  under MkaKay container */ 	uint32 usMkaDstMacAddress; /* this parameter
//  under MkaKay container */ 	uint16 usMkaEAlogMismatchInstance; /* this
//  parameter under MkaKay --> MkaKayDemEventParameterRefs  container */
//  uint16 usMkaEKeyMismatchInstance;                /* this parameter under
//  MkaKay --> MkaKayDemEventParameterRefs  container */ 	uint16
//  usMkaEKeyNotPresentInstance;              /* this parameter under MkaKay -->
//  MkaKayDemEventParameterRefs  container */ 	uint16 usMkaETimeoutInstance; /*
//  this parameter under MkaKay --> MkaKayDemEventParameterRefs  container */
//  	const Mka_KayParticipants *pMkaKayParticipants;
//  	uint8 ucMkaKeyServerPriority;                    /* this parameter under
//  MkaKay container */ 	Mka_RoleType ddMkaRole; /* this parameter under
//  MkaKay container */ 	uint32 usMkaSrcMacAddress; /* this parameter
//  under MkaKay container */ 	uint16 usMkaOnFailPermissiveMode; /* type chnage
//  derive data type to uint16 */ 	uint8 ucMkaOnFailPermissiveModeTimeout;
//  uint8 ucMkaPaeConfRefIndex; 	uint8 ucMkaSwitchPortIndex;
//   /* Mka_PaeConfigurationType PaeConfiguration; */
//  	uint8 ucMkaPaeIdx;
//   /* MkaSwitchPortRef */
//
// }Mka_PaeInstanceType;

/* Design_ID          : MKA_SDD_XXXX */
/* Enum to hold Mka_MacSecCapablity */
// typedef enum ETag_Mka_MacSecCapablity
//{
//   INTEGRITY_AND_CONFIDENTIALITY = 0,
//   INTEGRITY_WITHOUT_CONFIDENTIALITY
// }Mka_MacSecCapablity;
//
//  /* Design_ID          : MKA_SDD_XXXX */
//  /* Enum to hold Mka_MacSecConfidentialityOffset */
// typedef enum ETag_Mka_MacSecConfidentialityOffset
//{
//   CONFIDENTIALITY_OFFSET_0 = 0,
//   CONFIDENTIALITY_OFFSET_30,
//   CONFIDENTIALITY_OFFSET_50
// }Mka_MacSecConfidentialityOffset;
//
///* Design_ID          : MKA_SDD_XXXX */
///* Enum to hold Mka_MacSecCipherSuiteType */
// typedef enum ETag_Mka_MacSecCipherSuiteType
//{
//   GCM_AES_128 = 0,
//   GCM_AES_256,
//   GCM_AES_XPN_128,
//   GCM_AES_XPN_256
// }Mka_MacSecCipherSuiteType;
//
// typedef struct STag_Mka_CipherSuitesType
//{
//   Mka_MacSecCipherSuiteType ddMkaMacSecCipherSuite;
//   uint8 ucMkaMacSecCipherSuitePrio;
// }Mka_CipherSuitesType;

/* Design_ID          : MKA_SDD_XXXX */
/* Structure to hold MKA Crypto Algo Configuration */
// typedef struct STag_Mka_CryptoAlgoConfigType {
//   const Mka_CipherSuitesType *pMkaCipherSuites;
//   uint8 ucMkaCryptoAlgoConfigIdx;
//   Mka_MacSecCapablity ddMkaMacSecCapability;
//   Mka_MacSecConfidentialityOffset ddMkaMacSecConfidentialityOffset;
//   boolean blMkaMacSecReplayProtection;
//   uint8 usMkaMacSecReplayProtectionWindow;
// }Mka_CryptoAlgoConfigType;

typedef struct STag_Mka_StatusFlag_Type {
  uint8 ddkeyserver : 1;
  uint8 ddmacdesired : 1;
  uint8 ddmacsecCapability : 2;
} Mka_StatusFlag_Type;

typedef struct STag_Mka_BasicParameterSet_Type {
  uint8 Mka_VersionIdentifier;
  uint8 Mka_KeyServerPriority;
  uint8 Mka_Lifetime;
  Mka_StatusFlag_Type Mka_flag;
  uint64 Mka_Sci;
  uint8 Mka_AcMi[12];
  uint32 Mka_AMn;
  uint32 Mka_AlgoAgility;
  uint16 Mka_Ckn_Length;
  uint8 Mka_Ckn[16];
  uint16 Mka_parameterSetLength;
} Mka_BasicParameterSet_Type;

typedef struct STag_Mka_AnouncementSet_Type {

  uint8 Mka_AnouncementparameterType;
  uint8 Mka_CipherSuiteImplementationCapabilty[2];
  uint8 Mka_CipherSuiteImplementationCipherSuiteReferenceNumber[8];

  uint16 Mka_AnouncementPacketLength;
  uint16 Mka_TLV_Type : 7;
  uint16 Mka_Info_String : 9;

} Mka_AnouncementSet_Type;

typedef struct STag_Mka_LiveandPotentialPeerList_Type {
  uint8 Mka_parameterSetType;
  uint8 Mka_KeyServerSSCI;
  uint16 Mka_parameterBodySetlength;
} Mka_LiveandPotentialPeerList_Type;

typedef struct STag_Mka_PeerList_type {
  uint8 mka_parameterSetType;
  uint8 Mka_keySSCI;
  uint16 Mka_parameterBodySetlength;
  uint8 Mka_LivePotentialMId[12];
  uint8 Mka_LivePotentialMNum[4];
} Mka_PeerList_type;

typedef struct STag_Mka_TLV_Type {
  uint8 Mka_TLV_FType ;
  uint8 Mka_Tlv_String_len;

  uint8 Mka_Infor_String[MACSEC_MAX_SIZE];
} Mka_TLV_Type;

typedef struct STag_Mka_Distributed_SAK_type{
uint8 Mka_ParameterSetType;
uint8 Mka_Distributed_An:2;
uint8 Mka_ConfidentialityOffset:2;
uint16 parametersetBodylength;
uint32 Mka_Keynumber;
uint8 Mka_AES_KEY[24];
}Mka_Distributed_SAK_type;
typedef struct STag_Mka_Use_ParamterSet_Type{
uint8 Mka_ParamterSetType;
uint8 Mka_Latest_Key_AN:2;
uint8 Mka_Latest_Key_Tx:1;
uint8 Mka_Latest_Key_Rx:1;
uint8 Mka_OLD_Key_AN:1;
uint8 Mka_OLD_Key_Tx:1;
uint8 Mka_OLD_Key_Rx:1;
uint8 Mka_PlainText:1;
uint8 Mka_Plainrx:1;
uint8 reserved:1;
uint8 Mka_Delay_Protect:1;
uint16 ParamterSetBodyLength:12;
uint8  Mka_MI_Latest_key_Server_Member[12];
uint32 Mka_Latest_Key_Number;
uint32 Mka_Latest_Key_Lowest_Acceptable_PN;
uint8  Mka_MI_Old_key_Server_Member[12];
uint32  Mka_Old_Key_Key_number;
uint32 Mka_Old_Key_Lowest_Acceptable_PN;

}Mka_Use_ParamterSet_Type;

typedef struct STag_Mka_Pdu_Type {
  uint8 Mka_Protocol_version;
  uint16 Mka_packetType;
  uint32 Mka_packetBodylength;
  Mka_BasicParameterSet_Type Mka_BasicParameterSet;
  Mka_LiveandPotentialPeerList_Type Mka_liveandPoterntial;
  Mka_PeerList_type Mka_PeerList;
  Mka_TLV_Type Mka_TLV;
} Mka_Pdu_Type;

typedef struct STag_EAPOL_MKA_TYPE {

  uint8 usEapol_version;
  uint8 usEapol_PSetType;
  uint16 usEapo_pdu_Length;
} Mka_Eapol_Type;

typedef struct STag_Mka_MemberIdentifierStorageType{

  uint8 MI[12];
  uint32 MN;
}Mka_MemberIdentifierStorageType;

typedef enum ETag_MkaFrameTye{
/* BASIC PARAMTER SET TYPE frame*/
MKA_BASIC_FRAME_TYPE = 0,
/*POTENTIAL List PDU creation frame*/
MKA_LIVE_TYPE,

/*LIVE and DISTibuted Parameter set Type creation frame*/
MKA_POTENTIAL_TYPE,

/*USE case parameter setype creation frame*/
MKA_USE_LIVE_TYPE,

MKA_DISTRIBUTED_TYPE,

MKA_MACSEC_TLV_TYPE =0x70,

MKA_ICV_INDICATOR = 0xFF

}Mka_FrameType;

typedef struct STag_Mka_Selected_KayParticipantType{
 boolean blkaypartselect;
 uint8 index;


}Mka_Selected_KayParticipantType;

typedef struct STag_Mka_JobRegistryType{
uint8 PaeId;
uint8 JobId;
uint8 KayIndex;
uint8 JobCounter;
boolean status;

}Mka_JobRegistryType;


#endif /* MKA_INTERNALTYPES_H */

/*******************************************************************************
**                          End of File                                       **
*******************************************************************************/
