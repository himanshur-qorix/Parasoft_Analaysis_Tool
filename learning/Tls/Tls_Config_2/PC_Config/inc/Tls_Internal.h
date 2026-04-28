/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_Internal.h                                                 **
**                                                                            **
** TARGET    : All                                                            **
**                                                                            **
** PRODUCT   : AUTOSAR Transport Layer Security                               **
**                                                                            **
** PURPOSE   : Provision of module header                                     **
**                                                                            **
** PLATFORM DEPENDANT [yes/no]: no                                            **
**                                                                            **
** TO BE CHANGED BY USER [yes/no]: no                                         **
**                                                                            **
** Design ID  : TLS_SDD_0301                                                  **
*******************************************************************************/

/*******************************************************************************
**                      Revision History                                      **
********************************************************************************
**  Date          Changed By          Description                             **
********************************************************************************
** 16-Jan-2026    Pramod Ganiga,     Support for TLS Alert Management         **
** 18-Dec-2025   Pramod Ganiga,       Support for Application Data transmission**
** 21-Nov-2025   Pramod Ganiga,       Support for Encrypted Extensions,       **
**               Chirukuri Sherly     Support for Finished Protocol           **
** 30-Sep-2025   Pramod Ganiga        Initial version.                        **
*******************************************************************************/
#ifndef TLS_INTERNAL_H
#define TLS_INTERNAL_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "ComStack_Types.h"
#include "TcpIp.h"
#include "Tls_Ram.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern void  Tls_MainFunction(const TcpIp_TlsConnectionIdType TlsConnectionId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern void  Tls_ConvertTextToHex(
								const uint8* const LpInptPskIdentyTxt,
								uint8* const LpHexDataResult,
								const uint16  LusPskIdentityLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_GetInActiveConnIdx(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							const Tls_ConnectionType  LddConnectionType,
							uint16* const LpActiveConIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern uint16  Tls_GetActIdx(const TcpIp_TlsConnectionIdType LddConnectionId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_CheckSupportedTag(
                                const Tls_HandShakeProtocolType LddHSMsgType,
                                const uint8* const LpHShakeBuf);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern uint16  Tls_GetLValue(const uint16 LusNegotitdCiprSutId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern uint16  Tls_GetExtensionLength(const uint8* const LpHShakeBuf);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern void  Tls_ProcessHShakeProtocol(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PrepareChLegacyVersion(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PrepareChRandomNum(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PrepareChSessionId(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PrepareChCipherSuites(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PrepareChCompressMethd(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PrepareChExtensions(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_PrepareShLegacyVersion(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_PrepareShRandomNumber(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PrepareShSessionId(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PrepareShCipherSuite(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PrepareShCompressionMethod(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PrepareShExtensions(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#if (STD_ON == TLS_PSK_IDENTITY_SUPPORT_ENABLED)
/* PSK related Internal functions */
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType Tls_GetChPSKExt(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* LpHShakeBuf,
							uint16* const LpExtLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/* Server side functions to validate client hello function*/										

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern void Tls_GetServerPSKExt(uint8* const LpHShakeBuf, uint16* const LpExtLen);
									
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

									
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_ValidateClientPskExt(
				const uint16 LusActiveConnIdx,
				const uint8* const LpCHShakeBuf,
				uint16* const LpPskIndex,
                uint32* const LpPskCsmKeyId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_ValidateServerPskExt(
				const TcpIp_TlsConnectionIdType  LddConnectionId,
				const uint8* const LpSHShakeBuf,
                uint16* const LpExtLen,
                uint32* const LpPskCsmKeyId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"
#endif

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern uint32 Tls_SerializeCHBuf(const Tls_FrameType* LpElementData);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/* Record Protocol Internal Functions */
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_TransmitRecord(
                            const uint16 LusActiveConnIdx,
                            const Tls_ContentType LddContentType,
                            const uint8* const LpPlainTxtPtr,
                            const uint16 LusPlainTxtLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_HandleRecordReception (
                                const TcpIp_TlsConnectionIdType LddConnectionId,
                                const uint8* const LucDataPtr,
                                const uint16  LusTotalDataLength);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ProcessHandshakeRecord (
                                const uint16 LusActiveConnIdx,
                                const Tls_ConnectionType  LddTlsConnectionType,
                                const uint8* const LpDataPtr,
                                const uint16  LusTotalDataLength);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ProcessAlertRecord (
                                const TcpIp_TlsConnectionIdType LddConnectionId,
                                const uint8* const LucDataPtr,
                                const uint16  LusTotalDataLength);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_ParseRecord(
									const TcpIp_TlsConnectionIdType  LddConnectionId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType Tls_DecryptRecord(const uint16 LusActiveConnIdx,
                                const uint8* const LpEncryptPtr, 
                                uint8* LpDecryptData,
                                uint16* const LpDecryptDataLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_FragmentRecord(
                            const TcpIp_TlsConnectionIdType  LddConnectionId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_ValidateCHSupportedVersionExt(
									TcpIp_TlsConnectionIdType  LddConnectionId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_ValidateServerHello(
									const uint16 LusActiveConnIdx,
									const uint8* const LpSHShakeBuf);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_ValidateClientHello(
									const uint16 LusActiveConnIdx,
									const uint8* const LpHShakeBuf);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType Tls_ConstructNonce(const uint64 LullSeqNum,
                                    const uint32 LulWriteIvKeyId,
                                    uint8* const LpNounce);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_ValidateEE(const uint16 LusActiveConnIdx,
							const uint8* const LpHShakeBuf, uint16* const LpEELen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_DeriveESMaterial(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_DeriveHSSecretMaterial(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_DeriveMasterSecretMaterial(
                                                const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/******************Binder Key functions ***************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_DeriveBinderMaterials(
                                        const uint32 LulPskCsmKeyId,
                                        const uint32 LulTrgBinderKey,
                                        const Tls_PSKHashType LddPSKHashType);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/******************Binder Key functions ***************/

/******************Finished Key functions ***************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_DeriveFinishedKey(const uint16 LusActiveConnIdx,
                                            const uint32 LulSrcHSTrficSecrt);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_ProcessFinishedMessage(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType Tls_VerifyFinishMessage(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/******************Finished Key functions ***************/

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType Tls_ProcessAlert(
                            const TcpIp_TlsConnectionIdType  LddConnectionId, 
                            const Tls_AlertDescription LddAlert);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_EnqueueRecord(const uint16 LusActiveConnIdx,
                                const uint8* const LpDataPtr,
                                const uint16 LusDataLength);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern void Tls_DeQueueRecord(const uint16 LusActiveConnIdx,
												const uint16 LusDataLength);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern void Tls_GetQRecordHdr(const uint16 LusActiveConnIdx, uint8* const LpDstPtr);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ProcessDummyCHExt
                            (const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* LpHShakeBuf,
                            uint16* const LpExtLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
void Tls_GetKShareExpLen(const uint16 LusSuprtdGrp,
                                                uint16* const LpExpKShareLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern boolean Tls_CheckSupportedGrpValue(const uint16 LusSupportedGrpId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern boolean Tls_CheckSignatureAlgoValue(const uint16 LusSupportedGrpId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType Tls_ComputeTranscriptHash(const uint16 LusActiveConnIdx, 
			uint8* const LpTranscriptHash, uint16* const LpTranscriptHashLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern boolean Tls_CheckSupportedSigAlgo(const uint16 LusSignatureAlgoId, 
const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

                                    
#endif