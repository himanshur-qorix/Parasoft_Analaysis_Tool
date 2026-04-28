/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_External.h                                                 **
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
** Design ID  : TLS_SDD_0299, TLS_SDD_0350                                    **
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
#ifndef TLS_EXTERNAL_H
#define TLS_EXTERNAL_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "TcpIp.h"
#include "Tls_Generated_Callouts.h"

#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType Tls_TcpTransmit(
								const TcpIp_TlsConnectionIdType LddConnectionId,
								const uint8* LpDataPtr,
								const uint16 LusTotalDataLength);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern void Tls_ConnStatsNotification(
						const TcpIp_TlsConnectionIdType LddConnectionId,
                        const Tls_ConnectionStatusType  LddConnectionResult);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType Tls_TcpReception(
								const TcpIp_TlsConnectionIdType LddConnectionId,
								const uint8* LpDataPtr,
								const uint16 LusTotalDataLength);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType Tls_TcpCopyTxData(
								const TcpIp_TlsConnectionIdType LddConnectionId,
								uint8* LpDataPtr,
								const uint16 LusTotalDataLength);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PerformHKDFExtract(                                       
    /* JobId to perform HKDF extract */
                                            const uint32 LulExtractJobId, 
    /* KeyId to be Used with the Extract Job Id perform HKDF extract */                                            
                                            const uint32 LulExtractKeyId,
    /* specifies if Salt is available or not for HKDF extract */                                       
                                            const boolean LblIsSaltAvailable,
    /* specifies if IKM is available or not for HKDF extract */                                              
                                            const boolean LblIsIKMAvailable,
    /*  specifies which CipherSuite to be used for HKDF extract */ 
                                            const uint16 LusNegotitdCiprSutId,
    /* Salt KeyId to be used to perform HKDF extract */ 
                                            const uint32 LulSaltKey,
    /* IKM KeyId to be used to perform HKDF extract */                                             
                                            const uint32 LulIKMKey,
    /* PRK Id KeyId to store the derived Key */
                                            const uint32 LulPRKKey); // PRK
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PerformMacGenerate(
/* JobId to perform Mac Generate */
                                        uint32 LulJobId,
/* KeyId to be used in pair the Mac Job id to perform Mac Generate */                                        
                                        uint32 LulKeyId,
/* KeyId to be used to copy into Mac Key Id */                                        
                                        uint32 LulBaseKeyId,
/* Contains the pointer to the data for which the MAC shall be computed */                                         
                                        const uint8* LpdataPtr,
/* Contains the number of bytes to be Hmaced */                                        
                                        uint32 LuldataLength,
/* Contains the pointer to the data where the MAC shall be stored */                                        
                                        uint8* LpresultPtr,
/* Holds a pointer to the memory location in which the output 
length in bytes is stored */                                          
                                        uint32* LpresultLengthPtr);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PerformHKDFExpand(
/* JobId to perform HKDF Expand */
                                        const uint32 LulExpandJobId,
/* KeyId to be Used with the Expand Job Id perform HKDF Expand */                                        
                                        const uint32 LulExpandKeyId,
/* PRK key Id to be copied into Exapnd key Id to perform HKDF Expand */                                         
                                        const uint32 LulPRKKey,
/* specifies which CipherSuite to be used for HKDF Expand */                                        
                                        const uint16 LusNegotitdCiprSutId,
/* specifies the output Key length to be generated */                                        
                                        const uint16 LusLKeylength,
/* specifies the Input Info to be used in HKDF expand */                                       
                                        const uint8* LpInputLabel,
/* specifies the length of Input Info */                                        
                                        const uint8 LucInpLabelSize,
/* KeyId to be used to store the Final Key */                                       
                                        const uint32 LulOkmKey);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PerformHash(
/* JobId to perform Hash */
                                uint32 LuljobId,
/* Contains the pointer to the data for which the hash shall be computed */                                
                                const uint8* LpdataPtr,
/* Contains the number of bytes to be hashed */                                 
                                uint32 LuldataLength,
/* Contains the pointer to the data where the hash value shall be stored */                                
                                uint8* LpresultPtr,
/* Holds a pointer to the memory location in which the output length in bytes
is stored */                          
                                uint32* LpresultLengthPtr);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PerformRandomGenerate(
/* JobId to perform Random Generate */
                                uint32 LuljobId,
/* Holds a pointer to the memory location which will hold the result 
of the random number generation */                                  
                                uint8* LpresultPtr,
/* Holds a pointer to the memory location in which the result length in 
bytes is stored */                                 
                                uint32* LpresultLengthPtr);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PerformXor(                                        
/* KeyId where the Write IV is stored */
                                        const uint32 LulWriteIvKeyId,
/* specifies the input Padded Sequence Number */                                       
                                        const uint8* LpPaddedSequence,
/* Out pointer to store the nounce which is generated */                                        
                                        uint8* LpNounce); 
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PerformAEADEncrypt(                                        
/* JobID to perform Aead Encrypt */
                                        const uint32 LulAEADEncryptJobId,
/* Key id which is paired with Job id for AEAD encrypt */
                                        const uint32 LulAEADEncryptKeyId,
/* secret Key to Perform AEAD encrypt */                                       
                                        const uint32 LulSHSWriteKey,
/*  specifies which CipherSuite to be used for AEAD encrypt */ 
                                        const uint16 LusNegotitdCiprSutId,
/* Contains the pointer to the data to be encrypted */
                                        const uint8* LpPlaintextPtr,
/* Contains the number of bytes to encrypt */
                                        const uint32 LulPlaintextLen,
/* Contains the pointer to the associated data */
                                        const uint8* LpAssociatedDataPtr,
/* Contains the number of bytes of the associated data */
                                        const uint32 LulassociatedDataLen,
/* Contains the pointer to the Noune */
                                        const uint8* LpNounce,                                        
/* Contains the number of bytes of the Nounce data */
                                        const uint32 LulNounceLen,                                       
/* Contains the pointer to the Encrypted Record */
                                        uint8* LpEncryptedRecord,
/* Contains the number of bytes of the EncryptedRecord */                                      
                                        uint16* LpEncryptdRecordLen);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PerformAEADDecrypt(                                        
/* JobID to perform Aead Decrypt */
                                        const uint32 LulAEADDDecryptJobId,
/* Key id which is paired with Job id for AEAD Decrypt */
                                        const uint32 LulAEADDecryptKeyId,
/* secret Key to Perform AEAD Decrypt */                                       
                                        const uint32 LulWriteKey,
/*  specifies which CipherSuite to be used for AEAD Decrypt */ 
                                        const uint16 LusNegotitdCiprSutId,
/* Contains the pointer to the data to be Decrypted */
                                        const uint8* LpCiphertextPtr,
/* Contains the number of bytes to Decrypt */
                                        const uint32 LulCiphertextLen,
/* Contains the pointer to the associated data */
                                        const uint8* LpAssociatedDataPtr,
/* Contains the number of bytes of the associated data */
                                        const uint32 LulassociatedDataLen,
/* Contains the pointer to the Noune */
                                        const uint8* LpNounce,                                        
/* Contains the number of bytes of the Nounce data */
                                        const uint32 LulNounceLen,                                       
/* Contains the pointer to the Decrypted Record */
                                        uint8* LpDecryptedRecord,
/* Contains the number of bytes of the DecryptedRecord */                                      
                                        uint16* LpDecryptdRecordLen);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType Tls_PerformMacVerify(
/* JobId to perform Mac Generate */
                                        uint32 LulVerifyJobId,
/* KeyId to be used in pair the Mac Job id to perform Mac Generate */                                        
                                        uint32 LulVerifyKeyId,
/* KeyId to be used to copy into Mac Key Id */                                        
                                        uint32 LulBaseKeyId,
/* Contains the pointer to the data for which the MAC shall be verifed */                                         
                                        const uint8* LpdataPtr,
/* Contains the number of bytes to be Hmaced */                                        
                                        uint32 LuldataLength,
/* Holds a pointer to the MAC to be verified */                                         
                                        const uint8* LpmacPtr,
/* Contains the MAC length in BITS to be verified */                                        
                                        uint32 LulmacLength);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
 

#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern void Tls_PerformDestroyKeys(                                        
                                    /* array which is encoded  */
                                    const uint8* LpDestroyKeyMask,
                                    /* array Size */
                                    const uint8 LucMaskSize);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType Tls_PerformKeypairGeneration(                                        
                            /* JobId to perform Key Exchange*/
                                    uint32 LulJobId,
                            /* KeyId to perform Exchange operation */                                        
                                    uint32 LulExchangeKeyId,                                    
                            /* pointer to the data of public value */
                                    uint8* LucpublicValuePtr,
                            /* pointer to the public value length */        
                                    uint32* LulpublicValueLengthPtr);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h" 


#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType Tls_PerformSecretKeypairGeneration(                                        
                            /* JobId to perform Key Exchange*/
                                    uint32 LulJobId,
                            /* KeyId to perform Exchange operation */                                        
                                    uint32 LulExchangeKeyId,                                    
                            /* pointer to the data of public value */
                                    const uint8* LucPatnerpubValPtr,
                            /* pointer to the public value length */        
                                    uint32 LulPatnerpubValLen);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h" 

#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"

extern Std_ReturnType Tls_PerformSignatureGeneration(                                        
                            /* JobId to perform Signature Generation*/
                                    const uint32 LulJobId,
                            /* KeyId to perform Signature Generation */                                        
                                    const uint32 LulKeyId,
                            /* pointer to the data to be signed*/        
                                    const uint8* dataPtr,
                            /* Contains number of bytes to sign. */
                                    const uint32 dataLength,
                            /* pointer to the signature data*/        
                                    uint8* signaturePtr,
                            /* pointer to the signature value length */         
                                    uint32* signatureLengthPtr);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"



#define TLS_START_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PerformSignatureVerify(
/* JobId to perform Signature Verify*/
					const uint32 LulJobId,
/* KeyId to be used to set the public key */                                        
                                        const uint32 LulPubKeyId,
/* Contains the pointer to Publick Key */                                         
                                        const uint8* LpdPublicKeyPtr,
/* Contains the len of Public Key */                                         
                                        const uint16 LusPublicKeyLen,                                       
/* Contains the pointer to the data to be verified */                                         
                                        const uint8* LpdataPtr,
/* Contains the number of data bytes */                                        
                                        const uint32 LuldataLength,
/* Holds a pointer to the signature to be verified */                                        
                                        const uint8* LpSignaturePtr,
/* Contains the signature length in bytes */                                        
                                        const uint32 LulSignatureLength);
#define TLS_STOP_SEC_CALLOUT_CODE
#include "Tls_MemMap.h"
 
#endif