/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_Keyderivation.c                                            **
**                                                                            **
** TARGET    : All                                                            **
**                                                                            **
** PRODUCT   : AUTOSAR Transport Layer Security                               **
**                                                                            **
** PURPOSE   : This file provides definitions of following Tls APIs:          **
**                                                                            **
**  PLATFORM DEPENDANT [yes/no]: no                                           **
**                                                                            **
**  TO BE CHANGED BY USER [yes/no]: no                                        **
**                                                                            **
**  Design ID  : TLS_SDD_0309, TLS_SDD_0346                                   **
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
/*******************************************************************************
**                      Parasoft violations Section                           **
*******************************************************************************/

/** 
*
* #section Tls_Keyderivation_c_REF_1
* Violates MISRAC2012-RULE_5_4-c-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Keyderivation_c_REF_2
* Violates MISRAC2012-RULE_5_4-d-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Keyderivation_c_REF_3
* Violates MISRAC2012-RULE_20_1-a-4: Do not precede the #include directive with
* executable code
* Justification: Macros are defined by AUTOSAR standards and internal
* specifications.
*
* #section Tls_Keyderivation_c_REF_4
* Violates CERT_C-DCL19-a-3: Variable "" should be declared as locally as
* possible
* Justification: Coding guidelines suggest the variables are declared on the
* start of the function.
*
* #section Tls_Keyderivation_c_REF_5
* Violates CERT_C-API00-a-3: Parameter '' is not validated before use.
* Justification: This parameter either originates from the configuration or is
* validated in the main API before being passed as an argument to the local API.
* Hence, additional validation within the local API is not required and has been
* manually verified.
*
* #section Tls_Keyderivation_c_REF_6
* Violates MISRAC2012-RULE_1_1-a-2: More than 31 initial characters in an
* internal identifier: 34
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted.
*/

/* parasoft-begin-suppress MISRAC2012-RULE_5_4-c-2 
"Reason: Tls_Keyderivation_c_REF_1" */
/* parasoft-begin-suppress MISRAC2012-RULE_5_4-d-2 
"Reason: Tls_Keyderivation_c_REF_2" */
/* parasoft-begin-suppress MISRAC2012-RULE_20_1-a-4 
"Reason: Tls_Keyderivation_c_REF_3" */
/* parasoft-begin-suppress CERT_C-DCL19-a-3 
"Reason: Tls_Keyderivation_c_REF_4" */
/* parasoft-begin-suppress CERT_C-API00-a-3 
"Reason: Tls_Keyderivation_c_REF_5" */
/* parasoft-begin-suppress MISRAC2012-RULE_1_1-a-2 
"Reason: Tls_Keyderivation_c_REF_6" */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Tls_Inline.h"
#include "Tls_Internal.h"

#include "Det.h"
#include "Tls_External.h"
/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/

/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/* finished in Hex to avoid parasoft */
static const uint8 TLS_LABEL_FINISHED[TLS_EIGHT] = 
{
    0x66U, 0x69U, 0x6EU, 0x69U, 0x73U, 0x68U, 0x65U, 0x64U
};

static const uint8 LaaEmptyHash_256[TLS_THIRTYTWO] =
{
    TLS_HEX_E3, TLS_HEX_B0, TLS_HEX_C4, TLS_HEX_42,
    TLS_HEX_98, TLS_HEX_FC, TLS_HEX_1C, TLS_HEX_14,
    TLS_HEX_9A, TLS_HEX_FB, TLS_HEX_F4, TLS_HEX_C8,
    TLS_HEX_99, TLS_HEX_6F, TLS_HEX_B9, TLS_HEX_24,
    TLS_HEX_27, TLS_HEX_AE, TLS_HEX_41, TLS_HEX_E4,
    TLS_HEX_64, TLS_HEX_9B, TLS_HEX_93, TLS_HEX_4C,
    TLS_HEX_A4, TLS_HEX_95, TLS_HEX_99, TLS_HEX_1B,
    TLS_HEX_78, TLS_HEX_52, TLS_HEX_B8, TLS_HEX_55
};


static const uint8 LaaEmptyHash_384[TLS_FOURTYEIGHT] =
{
    TLS_HEX_38, TLS_HEX_B0, TLS_HEX_04, TLS_HEX_75,
    TLS_HEX_30, TLS_HEX_96, TLS_HEX_41, TLS_HEX_04,
    TLS_HEX_44, TLS_HEX_58, TLS_HEX_59, TLS_HEX_75,
    TLS_HEX_87, TLS_HEX_C6, TLS_HEX_47, TLS_HEX_1E,
    TLS_HEX_40, TLS_HEX_55, TLS_HEX_28, TLS_HEX_14,
    TLS_HEX_D4, TLS_HEX_29, TLS_HEX_81, TLS_HEX_55,
    TLS_HEX_9C, TLS_HEX_5F, TLS_HEX_DA, TLS_HEX_E1,
    TLS_HEX_40, TLS_HEX_FC, TLS_HEX_80, TLS_HEX_1A,
    TLS_HEX_61, TLS_HEX_36, TLS_HEX_D8, TLS_HEX_0A,
    TLS_HEX_04, TLS_HEX_58, TLS_HEX_69, TLS_HEX_09,
    TLS_HEX_4E, TLS_HEX_25, TLS_HEX_26, TLS_HEX_14,
    TLS_HEX_83, TLS_HEX_73, TLS_HEX_2C, TLS_HEX_7F
};

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveEarlySecret(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_PrepareHKDFLabel(
                        const uint16 LusNegotitdCiprSutId,
                        uint8* LpHkdfLabel,
                        uint8* const LpHkdfLabelLen,
                        const uint8* const LpLabel,
                        const uint8 LucLabelSize,
                        const uint8* const LpContext,
                        const uint8 LucContextLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveHSSecret(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveMasterSecret(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveAllHSTrafficSecrets(
                                                const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveAllApplicationTrafficSecrets(
                                                const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveEachHSTrfficSecrt(
                                            const uint32 LulTrgHSTrficSecrt, // Where to Store the client or server HandShake Traffic Secret 
                                            const uint32 LulTrgHSWriteKey, // Where to Store the HandShake Write Key
                                            const uint32 LulTrgHSWriteIV,  // Where to Store the HandShake Write IV
                                            const uint16 LusActiveConnIdx,
                                            /* only data ptr is enough , as the 
                                            length i will already know , since 
                                            i know what the choosen cipher is */   
                                            const uint8* const LpCHSHHash,
                                            const uint8* const LpLabel,
                                            const uint8 LucLabelSize);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

                                            
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveEachApplicationTrfficSecrt(
                            /* Where to Store the Master Traffic Secret */
                            const uint32 LulTrgApplTrficSecrtKey,
                            /* Where to Store the Master Write Key */
                            const uint32 LulTrgApplWriteKey,
                            /* Where to Store the Master Write IV */
                            const uint32 LulTrgApplWriteIV, 
                            const uint16 LusActiveConnIdx,
                            /* only data ptr is enough , as the 
                            length i will already know , since 
                            i know what the choosen cipher is */   
                            const uint8* const LpCHSHHash,
                            const uint8* const LpLabel,
                            const uint8 LucLabelSize);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveWriteKey(
                                    /* PRK */
                                    const uint32 LulSrcHSTrficSecrt,
                                    /* Place to store the Write Key */
                                    const uint32 LulTrgHSWriteKey,
                                    const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveWriteIV(
                                    /* PRK */
                                    const uint32 LulSrcHSTrficSecrt,
                                    /* Place to store the Write Key */
                                    const uint32 LulTrgHSWriteIV,
                                    const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveDerivativeKey(
                                    const uint16 LusActiveConnIdx,
                                    /* PRK -> can be used for both 
                                    early derivate key or Handshake derivate key */
                                    const uint32 LulSecret);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveBinderSecret(
                                        const uint32 LulPskCsmKeyId,
                                        const Tls_PSKHashType LddPSKHashType);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveBinderKey(
                                        const uint32 LulTrgBinderKey,
                                        const Tls_PSKHashType LddPSKHashType);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_DeriveESMaterial                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : call this function to get the 
                        "early secret" in TlsCsmEarlySecretKeyRef and 
                        early derivative key in TlsCsmDerivedKeyRef 
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0217                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_DeriveESMaterial(const uint16 LusActiveConnIdx)
{
    Std_ReturnType LddReturnValue;   
    uint32 LulEarlySecret;
    
    LddReturnValue = Tls_DeriveEarlySecret(LusActiveConnIdx);
    
    if (E_OK == LddReturnValue)
    {                  

        LulEarlySecret = 
        Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
        LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
        ].ulTlsCsmEarlySecretKeyRefIdx;            
   
        LddReturnValue = Tls_DeriveDerivativeKey(
                                                LusActiveConnIdx,
                                                LulEarlySecret);
    }
    
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_DeriveHSSecretMaterial                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : call this function to get the 
"HandShake secret" in TlsCsmHandshakeSecretKeyRef
and "HandShake derivative key" in TlsCsmDerivedKeyRef
and "server_handshake_traffic_secret" in TlsCsmServerHandshakeTrafficSecretKeyRef
and "client_handshake_traffic_secret" in TlsCsmClientHandshakeTrafficSecretKeyRef  **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0218                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_DeriveHSSecretMaterial(const uint16 LusActiveConnIdx)
{
    Std_ReturnType LddReturnValue;
    
    LddReturnValue = Tls_DeriveHSSecret(LusActiveConnIdx);
    
    if (E_OK == LddReturnValue)
    {

        LddReturnValue = Tls_DeriveAllHSTrafficSecrets(LusActiveConnIdx);
    }
    
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_DeriveMasterSecretMaterial                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : when this is called u will generate "early secret" 
                          in TlsCsmEarlySecretKeyRef   **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0219                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_DeriveMasterSecretMaterial(const uint16 LusActiveConnIdx)
{
    Std_ReturnType LddReturnValue;
    
    LddReturnValue = Tls_DeriveMasterSecret(LusActiveConnIdx);
    
    if (E_OK == LddReturnValue)
    {
        LddReturnValue = Tls_DeriveAllApplicationTrafficSecrets(LusActiveConnIdx);
    }
    
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_DeriveEarlySecret                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : when this is called u will generate "early secret" 
                          in TlsCsmEarlySecretKeyRef   **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0220                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveEarlySecret(const uint16 LusActiveConnIdx)
{

                // 0
                // |
                // v
    // PSK ->  HKDF-Extract = Early Secret
 
    Std_ReturnType LddReturnValue;   
    Tls_AuthType LddAuthType;
    uint32 LulNegotitdPskCsmKeyId;
    uint32 LulExtractKeyId;
    uint32 LulExtractJobId;
    uint32 LulEarlySecret;
    uint16 LusNegotitdCiprSutId;
    boolean LblIsSaltAvailable;
    boolean LblIsIKMAvailable;
   
    LddAuthType = 
    Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
                                            ddTlsConnectionId].ddTlsAuthType;

    if (TLS_AUTH_PSK == LddAuthType)
    {
        LulNegotitdPskCsmKeyId = 
        Tls_HandshakeFrame[LusActiveConnIdx].ulNegotiatedPskCsmKeyId;
        LblIsSaltAvailable = TLS_FALSE;
        LblIsIKMAvailable = TLS_TRUE;
    }
    else
    {
        LulNegotitdPskCsmKeyId = (uint32)TLS_ZERO;
        LblIsSaltAvailable = TLS_FALSE;
        LblIsIKMAvailable = TLS_FALSE;
    }


    LulExtractKeyId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExtractKeyRefIdx; 

    LulExtractJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExtractJobRefIdx; 
    
    /* get the negotaited cipher suit from server side */
    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
    Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
    ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId;   

    LulEarlySecret = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmEarlySecretKeyRefIdx;
    

    /* Generate Early Secret */
    /* HMAC(Salt,Key) */
    LddReturnValue = Tls_PerformHKDFExtract(
    /* JobId to perform HKDF extract */
                                            LulExtractJobId, 
    /* KeyId to be Used with the Extract Job Id perform HKDF extract */                                            
                                            LulExtractKeyId,
    /* specifies if Salt is available or not for HKDF extract */                                       
                                            LblIsSaltAvailable,
    /* specifies if IKM is available or not for HKDF extract */                                              
                                            LblIsIKMAvailable,
    /*  specifies which CipherSuite to be used for HKDF extract */ 
                                            LusNegotitdCiprSutId,
    /* Salt KeyId to be used to perform HKDF extract */ 
                                            (uint32)TLS_ZERO,
    /* IKM KeyId to be used to perform HKDF extract */                                             
                                            LulNegotitdPskCsmKeyId,
    /* PRK Id KeyId to store the derived Key */
                                            LulEarlySecret); // PRK
                                                    
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_PrepareHKDFLabel                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : once called this will prepare the Hkdf label strcut    **
**                       in the rfc8446 uint8 array     **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0221                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_PrepareHKDFLabel(
                        const uint16 LusNegotitdCiprSutId,
                        uint8* LpHkdfLabel,
                        uint8* const LpHkdfLabelLen,
                        const uint8* const LpLabel,
                        const uint8 LucLabelSize,
                        const uint8* const LpContext,
                        const uint8 LucContextLen)
{    
    uint16 LusOkmLen;   
    uint8 LucHkdfLabelCounter;
    
    /* "tls13 " in Hex to avoid parasoft */
    const uint8 TLS_HKDF_LABEL_PREFIX[TLS_SIX] = 
    {
        0x74U, 0x6CU, 0x73U, 0x31U, 0x33U, 0x20U
    };

    LpHkdfLabel[TLS_ZERO] = (uint8)TLS_ZERO;
    LpHkdfLabel[TLS_TWO] = (uint8)TLS_SIX + LucLabelSize;

    Tls_MemCpy(&LpHkdfLabel[TLS_THREE], TLS_HKDF_LABEL_PREFIX, 
                    (uint16)(sizeof(TLS_HKDF_LABEL_PREFIX)));

    Tls_MemCpy(&LpHkdfLabel[TLS_NINE], LpLabel, (uint16)LucLabelSize);
    
    /* tls13 + input label size*/
    LucHkdfLabelCounter = (uint8)TLS_TWO + (uint8)TLS_ONE + (uint8)TLS_SIX + 
                                                                LucLabelSize;
    /* from the context either zero can be sent or actual hash or hash of 
    empty string */
    LpHkdfLabel[LucHkdfLabelCounter] = LucContextLen;

    if (LpContext != NULL_PTR)
    {
        Tls_MemCpy(&LpHkdfLabel[(uint8)TLS_ONE + LucHkdfLabelCounter], 
                                        LpContext, (uint16)LucContextLen);
        LucHkdfLabelCounter += LucContextLen;
    }

    /* if label was the "Key" , the label size will be different , refer 8448 */
    if ((uint8)TLS_THREE == LucLabelSize)
    {
        LusOkmLen = Tls_GetLValue(LusNegotitdCiprSutId);
        LpHkdfLabel[TLS_ONE] = (uint8)LusOkmLen;
    }
    /* if label was the "Iv" , the label size will be different , refer 8448 */
    else if ((uint8)TLS_TWO == LucLabelSize)
    {
        LpHkdfLabel[TLS_ONE] = (uint8)TLS_TWELVE;
    }
    /* for all other labels , refer 8448 */
    else
    {
        LusOkmLen = 
            (LusNegotitdCiprSutId == (uint16)TLS_AES_256_GCM_SHA384) ? 
            (uint16)TLS_FOURTYEIGHT : (uint16)TLS_THIRTYTWO;

        LpHkdfLabel[TLS_ONE] = (uint8)LusOkmLen;
    }

    *LpHkdfLabelLen = LucHkdfLabelCounter + (uint8)TLS_ONE;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/***************HandShake Key Functions*****************/

/*******************************************************************************
** Function Name        : Tls_DeriveHSSecret                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : when this is called u will generate "HandShake secret" and set in 
                            TlsCsmHandshakeSecretKeyRef   **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0222                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveHSSecret(const uint16 LusActiveConnIdx)
{

            // Derive-Secret(., "derived", "")
                    // |
                    // v
   // (EC)DHE -> HKDF-Extract = Handshake Secret
 
    Std_ReturnType LddReturnValue;    
    Tls_AuthType LddAuthType;
    Tls_FrameType*  LpTlsFrame;
    uint32 LulExtractKeyId;
    uint32 LulExtractJobId;
    uint32 LulErlyDerivKey;
    uint32 LulShardSecrtKey;
    uint32 LulKeyExchangeJobId;
    uint32 LulHSSecret;
    uint32 LulPatnerpubValLen;
    uint16 LusNegotitdCiprSutId;
    boolean LblIsSaltAvailable;
    boolean LblIsIKMAvailable;

    LulErlyDerivKey = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmDerivedKeyRefIdx;   

    LulExtractKeyId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExtractKeyRefIdx; 

    LulExtractJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExtractJobRefIdx;   

    LulHSSecret = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHandshakeSecretKeyRefIdx;     

    /* get the negotaited cipher suit from server side */
    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
    Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
    ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId;

	LblIsSaltAvailable = TLS_TRUE;
    LddAuthType = 
    Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
                                            ddTlsConnectionId].ddTlsAuthType;

    if (TLS_AUTH_PSK == LddAuthType)
    {
        LulShardSecrtKey = (uint32)TLS_ZERO;
        LblIsIKMAvailable = TLS_FALSE;
        LddReturnValue = E_OK;
    }
    else
    {
        LblIsIKMAvailable = TLS_TRUE;  
			
        LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];    

        /* Not sure, directly casting will be safe here ,so i extracting individual 
        bytes and setting it */
        LulPatnerpubValLen = 
        ((uint32)(LpTlsFrame->pPatnerKeyShare[TLS_ZERO]) << (uint8)TLS_EIGHT) |
        (uint32)(LpTlsFrame->pPatnerKeyShare[TLS_ONE]);

        /* Fecth the Key Id to trigger Key pair generation */
        LulShardSecrtKey = 
        Tls_GaaHandshake[LpTlsFrame->usNegotiatedSupGrpIdx]
                                                .ulTlsCsmKeyExchangeKeyRefIdx;
        
        /* Fecth the Job Id to trigger Key pair generation */
        LulKeyExchangeJobId = 
        Tls_GaaHandshake[LpTlsFrame->usNegotiatedSupGrpIdx]
                                        .ulTlsCsmKeyExchangeCalcSecretJobRefIdx;
                                        
        /* Initiate secret Key Pair Generation for a Named group */
        LddReturnValue = Tls_PerformSecretKeypairGeneration(                                        
                                    LulKeyExchangeJobId,
                                    LulShardSecrtKey,
                                    /*Since in the 3rd byte patner key exchange 
                                    value starts */
                                    &LpTlsFrame->pPatnerKeyShare[TLS_TWO],      
                                    LulPatnerpubValLen);

    }

    if (E_OK == LddReturnValue)
    {
        /* Generate HS Secret */
        /* HMAC(Salt,Key) */
        LddReturnValue = Tls_PerformHKDFExtract(
        /* JobId to perform HKDF extract */
                                                LulExtractJobId, 
        /* KeyId to be Used with the Extract Job Id perform HKDF extract */                                              
                                                LulExtractKeyId,
        /* specifies if Salt is available or not for HKDF extract */ 
                                                LblIsSaltAvailable,
        /* specifies if IKM is available or not for HKDF extract */                                           
                                                LblIsIKMAvailable,
        /*  specifies which CipherSuite to be used for HKDF extract */                                            
                                                LusNegotitdCiprSutId,
        /* Salt KeyId to be used to perform HKDF extract */                                            
                                                LulErlyDerivKey,
        /* IKM KeyId to be used to perform HKDF extract */                                            
                                                LulShardSecrtKey,
        /* PRK Id KeyId to store the derived Key */                                            
                                                LulHSSecret);
                                                 
    }
    
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_DeriveMasterSecret                              **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : when this is called u will generate "HandShake secret" and set in 
                            TlsCsmHandshakeSecretKeyRef   **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0223                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveMasterSecret(const uint16 LusActiveConnIdx)
{
    // Derive-Secret(., "derived", "")
                    // |
                    // v
   // 0 -> HKDF-Extract = Master Secret
       
    Std_ReturnType LddReturnValue;    
    uint32 LulExtractKeyId;
    uint32 LulExtractJobId;
    uint32 LulHSDerivKey;
    uint32 LulMasterSecret;
    uint16 LusNegotitdCiprSutId;
 
   /* To store Handshake derived key */ 
    LulHSDerivKey = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmDerivedKeyRefIdx;   
    /* To store Extract key */
    LulExtractKeyId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExtractKeyRefIdx; 
    /* To store Extract Job */
    LulExtractJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExtractJobRefIdx;   
    /* To store Master secret */
    LulMasterSecret = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmMasterSecretKeyRefIdx;     

    /* get the negotaited cipher suit from server side */
    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
    Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
    ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId;          

    /* Generate Master Secret */
    /* HMAC(Salt,Key) */
    LddReturnValue = Tls_PerformHKDFExtract(
    /* JobId to perform HKDF extract */
                                            LulExtractJobId, 
    /* KeyId to be Used with the Extract Job Id perform HKDF extract */                                            
                                            LulExtractKeyId,
    /* specifies if Salt is available or not for HKDF extract */                                       
                                            TLS_TRUE,
    /* specifies if IKM is available or not for HKDF extract */                                              
                                            TLS_FALSE,
    /*  specifies which CipherSuite to be used for HKDF extract */ 
                                            LusNegotitdCiprSutId,
    /* Salt KeyId to be used to perform HKDF extract */ 
                                            LulHSDerivKey,
    /* IKM KeyId to be used to perform HKDF extract */                                             
                                            (uint32)TLS_ZERO,
    /* PRK Id KeyId to store the derived Key */
                                            LulMasterSecret); // PRK
    
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_DeriveAllHSTrafficSecrets                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : when this is called u will generate
1)server_handshake_traffic_secret and store TlsCsmServerHandshakeTrafficSecretKeyRef
2)client_handshake_traffic_secret and store TlsCsmClientHandshakeTrafficSecretKeyRef 
3)and "HandShake derivative key" in TlsCsmDerivedKeyRef   **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0224                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveAllHSTrafficSecrets(const uint16 LusActiveConnIdx)
{                

    Std_ReturnType LddReturnValue;    
    uint8* LpHSShakeBuf;
    uint32 LulHashingJobId;
    uint32 LulCHSTraficSecrt; //ulTlsCsmClientHSTrafficSecretKeyRefIdx
    uint32 LulSHSTraficSecrt; //ulTlsCsmServerHSTrafficSecretKeyRefIdx
    uint32 LulHSSecret; // previosly stored HandShake secret place
    uint32 LulCHSWriteKey; // TlsCsmClientHandshakeWritekey
    uint32 LulSHSWriteKey; // TlsCsmServerHandshakeWritekey
    uint32 LulCHSWriteIV; // TlsCsmClientHandshakeWriteIV
    uint32 LulSHSWriteIV; // TlsCsmServerHandshakeWriteIV
    uint32 LulCHSHHashLen; 
    uint16 LusNegotitdCiprSutId;
    uint16 LusCHSHMsgLen;
    uint8 LucHashLen;
    
    /* "s hs traffic" in Hex to avoid parasoft */
    const uint8 TLS_LABEL_S_HS_TRAFFIC[TLS_TWELVE] = 
    {
        0x73U, 0x20U, 0x68U, 0x73U, 0x20U, 0x74U, 0x72U,
        0x61U, 0x66U, 0x66U, 0x69U, 0x63U
    };  

    /* "c hs traffic" in Hex to avoid parasoft */
    const uint8 TLS_LABEL_C_HS_TRAFFIC[TLS_TWELVE] = 
    {
        0x63U, 0x20U, 0x68U, 0x73U, 0x20U, 0x74U, 0x72U,
        0x61U, 0x66U, 0x66U, 0x69U, 0x63U
    };  

    uint8 LaaCHSHHash[TLS_MAX_HASH_SIZE] = {0};
    
    /* get the negotaited cipher suit from server side */
    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
    Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
    ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId; 

    LulHashingJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHashVerifyJobRefIdx; 

    LulHSSecret = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHandshakeSecretKeyRefIdx; 

    LulCHSTraficSecrt = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmClientHSTrafficSecretKeyRefIdx;  
    
    LulSHSTraficSecrt = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmServerHSTrafficSecretKeyRefIdx;  

    /* Write Key slots for Client */
    LulCHSWriteKey = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmClientHandshakeWritekeyRefIdx; 
    
    /* Write Key slots for Server */
    LulSHSWriteKey = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmServerHandshakeWritekeyRefIdx; 

    /* Write IV slots for Client */
    LulCHSWriteIV = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmClientHandshakeWriteIVKeyRefIdx; 
    
    /* Write IV slots for Server */
    LulSHSWriteIV = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmServerHandshakeWriteIVKeyRefIdx;     
    

    LusCHSHMsgLen = Tls_HandshakeFrame[LusActiveConnIdx].usProcessedHSBytes;             

    LucHashLen = 
    (LusNegotitdCiprSutId == (uint16)TLS_AES_256_GCM_SHA384) ? 
    (uint8)TLS_FOURTYEIGHT : (uint8)TLS_THIRTYTWO;  
      
    LpHSShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO]; 
        
    LulCHSHHashLen = (uint32)LucHashLen;
        
    LddReturnValue = Tls_PerformHash(                                        
    /* JobId to perform Hash */
                                    LulHashingJobId,
    /* Contains the pointer to the data for which the hash shall be computed */                                    
                                    LpHSShakeBuf,
    /* Contains the number of bytes to be hashed */                                 
                                    (uint32)LusCHSHMsgLen,
    /* Contains the pointer to the data where the hash value shall be stored */                                    
                                    &LaaCHSHHash[TLS_ZERO],
    /* Holds a pointer to the memory location in which the output length in bytes
    is stored */                                
                                    &LulCHSHHashLen);                                        

    if ((E_OK == LddReturnValue) && 
            /* Csm Hash shd return the choosen cipher amount of bytes */
            (LulCHSHHashLen == (uint32)LucHashLen))
            
    {
            
        LddReturnValue = Tls_DeriveEachHSTrfficSecrt(
                    /*key slot to store client_handshake_traffic_secret  */
                            LulCHSTraficSecrt,
                    /*key slot to store Client_write_key */                            
                            LulCHSWriteKey, 
                            /* key slot to store Client_WriteIV */ 
                            LulCHSWriteIV,
                            LusActiveConnIdx,
                            &LaaCHSHHash[TLS_ZERO],
                            &TLS_LABEL_C_HS_TRAFFIC[TLS_ZERO],
                            (uint8)(sizeof(TLS_LABEL_C_HS_TRAFFIC)));
                            
                                        
        if (E_OK == LddReturnValue)
        {
                
            LddReturnValue = Tls_DeriveEachHSTrfficSecrt(
                            LulSHSTraficSecrt, // key slot to store server_handshake_traffic_secret
                            LulSHSWriteKey, // key slot to store Server_write_key
                            LulSHSWriteIV, // key slot to store Server_WriteIV
                            LusActiveConnIdx,
                            &LaaCHSHHash[TLS_ZERO],
                            &TLS_LABEL_S_HS_TRAFFIC[TLS_ZERO],
                            (uint8)(sizeof(TLS_LABEL_S_HS_TRAFFIC)));
                            
            if (E_OK == LddReturnValue)
            {
                /* Derive HandShake Derivate Key */
                LddReturnValue = Tls_DeriveDerivativeKey(
                                        LusActiveConnIdx,
                                        /* take handShake Secret and generate
                                        HandShake Derivate Key */
                                        LulHSSecret);                                         
            }              
        }
    }

    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_DeriveAllApplicationTrafficSecrets              **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : when this is called u will generate
1)server_handshake_traffic_secret and store TlsCsmServerHandshakeTrafficSecretKeyRef
2)client_handshake_traffic_secret and store TlsCsmClientHandshakeTrafficSecretKeyRef 
3)and "HandShake derivative key" in TlsCsmDerivedKeyRef   **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0225                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveAllApplicationTrafficSecrets(
                                                const uint16 LusActiveConnIdx)
{                
	TcpIp_TlsConnectionIdType LddConnectionId;
    Std_ReturnType LddReturnValue;    
    uint8* LpHSShakeBuf;
    uint32 LulHashingJobId;
    uint32 LulCApplTraficSecrt; //ulTlsCsmClientApplTrafficSecretKeyRefIdx
    uint32 LulSApplTraficSecrt; //ulTlsCsmServerApplTrafficSecretKeyRefIdx
    uint32 LulCApplWriteKey; // TlsCsmClientApplicationWritekey
    uint32 LulSApplWriteKey; // TlsCsmServerApplicationWritekey
    uint32 LulCApplWriteIV; // TlsCsmClientApplicationWriteIV
    uint32 LulSApplWriteIV; // TlsCsmServerApplicationWriteIV
    uint32 LulCHSHHashLen; 
    uint16 LusNegotitdCiprSutId;
    uint16 LusLenToHash;
    uint8 LucHashLen;

    /* "c ap traffic" in Hex to avoid parasoft */
    const uint8 TLS_LABEL_C_APP_TRAFFIC[TLS_TWELVE] = 
    {
        0x63U, 0x20U, 0x61U, 0x70U, 0x20U, 0x74U, 0x72U,
        0x61U, 0x66U, 0x66U, 0x69U, 0x63U
    };  

    /* "s ap traffic" in Hex to avoid parasoft */
    const uint8 TLS_LABEL_S_APP_TRAFFIC[TLS_TWELVE] = 
    {
        0x73U, 0x20U, 0x61U, 0x70U, 0x20U, 0x74U, 0x72U,
        0x61U, 0x66U, 0x66U, 0x69U, 0x63U
    };  

    uint8 LaaMSHash[TLS_MAX_HASH_SIZE] = {0};
    
    LddConnectionId = 
                Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
				
    /* get the negotaited cipher suit from server side */
    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
    Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
    ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId; 

    LulHashingJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHashVerifyJobRefIdx; 

    /* To store TlsCsmClientApplTrafficSecretKey */
    LulCApplTraficSecrt = 
    Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].ulTlsCsmClientApplTrafficSecretKeyRefIdx; 
    /* To store TlsCsmServerApplTrafficSecretKey */
    LulSApplTraficSecrt = 
    Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].ulTlsCsmServerApplTrafficSecretKeyRefIdx;
    

    /* Write Key slots for TlsCsmClientApplWriteKey */
    LulCApplWriteKey = 
    Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].ulTlsCsmClientApplWriteKeyRefIdx; 
    
    /* Write Key slots for TlsCsmServerApplWriteKey */
    LulSApplWriteKey = 
    Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].ulTlsCsmServerApplWriteKeyRefIdx;
    

    /* Write IV slots for TlsCsmClientApplWriteIV */
    LulCApplWriteIV = 
    Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].ulTlsCsmClientApplWriteIVRefIdx; 
    
    /* Write IV slots for TlsCsmServerApplWriteIV */
    LulSApplWriteIV = 
    Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].ulTlsCsmServerApplWriteIVRefIdx;             

    LucHashLen = 
    (LusNegotitdCiprSutId == (uint16)TLS_AES_256_GCM_SHA384) ? 
    (uint8)TLS_FOURTYEIGHT : (uint8)TLS_THIRTYTWO;

    /* this will point to last byte of CF msg in client 
    or CF in Server */
    LusLenToHash = 
                Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition;
				
	/* why only client ?
	becuz in SERVER, we already made it point to the last byte of SF msg	*/
	if (Tls_GaaConnection[LddConnectionId].ddTlsConnectionType == TLS_CLIENT)
	{
		/* now this will point till SF last byte */    
		LusLenToHash -= (uint16)((uint16)TLS_FOUR + (uint16)LucHashLen); 		
	}
                

    LpHSShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO]; 
    
    LulCHSHHashLen = (uint32)LucHashLen;
    /* Hash should contain client hello to finished  */
    LddReturnValue = Tls_PerformHash(
    /* JobId to perform Hash */
                                    LulHashingJobId,
    /* Contains the pointer to the data for which the hash shall be computed */                                    
                                    LpHSShakeBuf,
    /* Contains the number of bytes to be hashed */                                 
                                    (uint32)LusLenToHash,
    /* Contains the pointer to the data where the hash value shall be stored */                                    
                                    &LaaMSHash[TLS_ZERO],
    /* Holds a pointer to the memory location in which the output length in bytes
    is stored */                                
                                    &LulCHSHHashLen);
 
    if ((E_OK == LddReturnValue) && 
        /* Csm Hash shd return the choosen cipher amount of bytes */
        (LulCHSHHashLen == (uint32)LucHashLen))
        
    {
        
        LddReturnValue = Tls_DeriveEachApplicationTrfficSecrt(
                        LulCApplTraficSecrt, // key slot to store client_handshake_traffic_secret
                        LulCApplWriteKey, // key slot to store Client_write_key
                        LulCApplWriteIV,  // key slot to store Client_WriteIV
                        LusActiveConnIdx,
                        &LaaMSHash[TLS_ZERO],
                        &TLS_LABEL_C_APP_TRAFFIC[TLS_ZERO],
                        (uint8)(sizeof(TLS_LABEL_C_APP_TRAFFIC)));

        if (E_OK == LddReturnValue)
        {
            
            LddReturnValue = Tls_DeriveEachApplicationTrfficSecrt(
                        LulSApplTraficSecrt, // key slot to store server_application_traffic_secret_0
                        LulSApplWriteKey, // key slot to store Server_write_key
                        LulSApplWriteIV, // key slot to store Server_WriteIV
                        LusActiveConnIdx,
                        &LaaMSHash[TLS_ZERO],
                        &TLS_LABEL_S_APP_TRAFFIC[TLS_ZERO],
                        (uint8)(sizeof(TLS_LABEL_S_APP_TRAFFIC)));
        }    
    }
    else
    {
        /* Tls_PerformHash Failed */
        LddReturnValue = E_NOT_OK; 
    }         
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_DeriveEachHSTrfficSecrt                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          :  after calling this u will generate 
client_handshake_traffic_secret and store TlsCsmClientHandshakeTrafficSecretKeyRef
Client_write_key in TlsCsmClientHandshakeWritekey   **
Client_write_IV in TlsCsmClientHandshakeWriteIV   **
Server_handshake_traffic_secret and store TlsCsmServerHandshakeTrafficSecretKeyRef
Server_write_key in TlsCsmServerHandshakeWritekey   **
Server_write_IV in TlsCsmServerHandshakeWriteIV   **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0284                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveEachHSTrfficSecrt(
                                            const uint32 LulTrgHSTrficSecrt, // Where to Store the client or server HandShake Traffic Secret 
                                            const uint32 LulTrgHSWriteKey, // Where to Store the HandShake Write Key
                                            const uint32 LulTrgHSWriteIV,  // Where to Store the HandShake Write IV
                                            const uint16 LusActiveConnIdx,
                                            /* only data ptr is enough , as the 
                                            length i will already know , since 
                                            i know what the choosen cipher is */   
                                            const uint8* const LpCHSHHash,
                                            const uint8* const LpLabel,
                                            const uint8 LucLabelSize)
{    

    Std_ReturnType LddReturnValue;   
    uint32 LulExpandJobId;
    uint32 LulExpandKeyId;
    uint32 LulHSSecret;
    uint16 LusNegotitdCiprSutId;
    uint16 LusOkmLen;
    uint8 LucHashLen;
    uint8 LucHkdfLabelLen;

    uint8 LaaHkdfLabel[TLS_MAX_LABEL_SIZE] = {0};
    
    /* get the negotaited cipher suite*/
    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
    Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
    ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId; 

    LulExpandJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExpandJobRefIdx; 

    LulExpandKeyId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExpandKeyRefIdx;  

    LulHSSecret = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHandshakeSecretKeyRefIdx; 
    
    LucHashLen = 
    (LusNegotitdCiprSutId == (uint16)TLS_AES_256_GCM_SHA384) ? 
    (uint8)TLS_FOURTYEIGHT : (uint8)TLS_THIRTYTWO;

    /* Get the L value , as per analysis, it L value depends on choosen ciphersui*/  
    LusOkmLen = (uint16)LucHashLen;
    

    /* can use this function to prepare label anywhere  */
    Tls_PrepareHKDFLabel(
                        LusNegotitdCiprSutId,
                        &LaaHkdfLabel[TLS_ZERO],
                        &LucHkdfLabelLen,
                        LpLabel,
                        LucLabelSize,
                        /* hash of Client Hello Server hello */
                        &LpCHSHHash[TLS_ZERO],
                        /* here i am sending diffrenrent local var , 
                        since i know it will always be 32 or 48 */
                        LucHashLen); 
                        
    LddReturnValue = Tls_PerformHKDFExpand(
    /* JobId to perform HKDF Expand */
                                        LulExpandJobId,
    /* KeyId to be Used with the Expand Job Id perform HKDF Expand */                                       
                                        LulExpandKeyId,
    /* PRK key Id to be copied into Exapnd key Id to perform HKDF Expand */                                        
                                        LulHSSecret,
    /* specifies which CipherSuite to be used for HKDF Expand */                                    
                                        LusNegotitdCiprSutId,
    /* specifies the output Key length to be generated */                                  
                                        LusOkmLen,
    /* specifies the Input Info to be used in HKDF expand */                                     
                                        &LaaHkdfLabel[TLS_ZERO],
    /* specifies the length of Input Info */                                     
                                        LucHkdfLabelLen,
    /* KeyId to be used to store the Final Key */                                    
                                        LulTrgHSTrficSecrt);
                                        
    if (E_OK == LddReturnValue)
    {        
        LddReturnValue = Tls_DeriveWriteKey(
                            LulTrgHSTrficSecrt, //Secret 
                            LulTrgHSWriteKey, // place to store the Write Key
                            LusActiveConnIdx);
                            
        if (E_OK == LddReturnValue)
        {
        
            LddReturnValue = Tls_DeriveWriteIV(
                                LulTrgHSTrficSecrt, //prk
                                LulTrgHSWriteIV, // place to store the Write IV
                                LusActiveConnIdx);
        }       
    }           
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_DeriveEachApplicationTrfficSecrt                **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          :  after calling this u will generate 
client_handshake_traffic_secret and store TlsCsmClientHandshakeTrafficSecretKeyRef
Client_write_key in TlsCsmClientHandshakeWritekey   **
Client_write_IV in TlsCsmClientHandshakeWriteIV   **
Server_handshake_traffic_secret and store TlsCsmServerHandshakeTrafficSecretKeyRef
Server_write_key in TlsCsmServerHandshakeWritekey   **
Server_write_IV in TlsCsmServerHandshakeWriteIV   **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0286                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveEachApplicationTrfficSecrt(
                                            const uint32 LulTrgApplTrficSecrtKey, // Where to Store the client or server HandShake Traffic Secret 
                                            const uint32 LulTrgApplWriteKey, // Where to Store the HandShake Write Key
                                            const uint32 LulTrgApplWriteIV,  // Where to Store the HandShake Write IV
                                            const uint16 LusActiveConnIdx,
                                            /* only data ptr is enough , as the 
                                            length i will already know , since 
                                            i know what the choosen cipher is */   
                                            const uint8* const LpCHSHHash,
                                            const uint8* const LpLabel,
                                            const uint8 LucLabelSize)
{     

    Std_ReturnType LddReturnValue;   
    uint32 LulExpandJobId;
    uint32 LulExpandKeyId;
    uint32 LulMasterSecret;
    uint16 LusNegotitdCiprSutId;
    uint16 LusOkmLen;
    uint8 LucHashLen;
    uint8 LucHkdfLabelLen;

    uint8 LaaHkdfLabel[TLS_MAX_LABEL_SIZE];
    
    /* get the negotaited cipher suite*/
    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
    Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
    ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId; 

    LulExpandJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExpandJobRefIdx; 

    LulExpandKeyId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExpandKeyRefIdx;  

    LulMasterSecret = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmMasterSecretKeyRefIdx;   

    LucHashLen = 
    (LusNegotitdCiprSutId == (uint16)TLS_AES_256_GCM_SHA384) ? 
    (uint8)TLS_FOURTYEIGHT : (uint8)TLS_THIRTYTWO;

    /* Get the L value */
    LusOkmLen = (uint16)LucHashLen;

    /* can use this function to prepare label anywhere  */
    Tls_PrepareHKDFLabel(
                        LusNegotitdCiprSutId,
                        &LaaHkdfLabel[TLS_ZERO],
                        &LucHkdfLabelLen,
                        LpLabel,
                        LucLabelSize,
                        /* hash of Client Hello Server hello */
                        &LpCHSHHash[TLS_ZERO],
                        /* here i am sending diffrenrent local var , 
                        since i know it will always be 32 or 48 */
                        LucHashLen); 

                        
    LddReturnValue = Tls_PerformHKDFExpand(                       
    /* JobId to perform HKDF Expand */
                                        LulExpandJobId,
    /* KeyId to be Used with the Expand Job Id perform HKDF Expand */                                       
                                        LulExpandKeyId,
    /* PRK key Id to be copied into Exapnd key Id to perform HKDF Expand */                                        
                                        LulMasterSecret,
    /* specifies which CipherSuite to be used for HKDF Expand */                                    
                                        LusNegotitdCiprSutId,
    /* specifies the output Key length to be generated */                                  
                                        LusOkmLen,
    /* specifies the Input Info to be used in HKDF expand */                                     
                                        &LaaHkdfLabel[TLS_ZERO],
    /* specifies the length of Input Info */                                     
                                        LucHkdfLabelLen,
    /* KeyId to be used to store the Final Key */                                    
                                        LulTrgApplTrficSecrtKey); 
                                        
    if (E_OK == LddReturnValue)
    {
        
        LddReturnValue = Tls_DeriveWriteKey(
                            LulTrgApplTrficSecrtKey, //Secret 
                            LulTrgApplWriteKey, // place to store the Write Key
                            LusActiveConnIdx);
                            
        if (E_OK == LddReturnValue)
        {
        
            LddReturnValue = Tls_DeriveWriteIV(
                                LulTrgApplTrficSecrtKey, //prk
                                LulTrgApplWriteIV, // place to store the Write IV
                                LusActiveConnIdx);
        }        
    }         
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_DeriveWriteKey                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : THis function will generate the Sender_Write_Key and 
                         store in required SLot **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0287                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveWriteKey(
                                    const uint32 LulSrcHSTrficSecrt, // PRK
                                    const uint32 LulTrgHSWriteKey, // Place to store the Write Key
                                    const uint16 LusActiveConnIdx)
{     
    Std_ReturnType LddReturnValue;   
    uint32 LulExpandJobId;
    uint32 LulExpandKeyId;
    uint16 LusNegotitdCiprSutId;
    uint16 LusOkmLen;
    uint8 LucHkdfLabelLen;

    /* "key" in Hex to avoid parasoft */
    const uint8 TLS_LABEL_KEY[TLS_THREE] = 
    {
        0x6BU, 0x65U, 0x79U
    };  

    uint8 LaaHkdfLabel[TLS_MAX_LABEL_SIZE] = {0};
       
    /* get the negotaited cipher suit from server side */
    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
    Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
    ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId; 

    LulExpandJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExpandJobRefIdx; 

    /* Get the L value */
    LusOkmLen = Tls_GetLValue(LusNegotitdCiprSutId);

    LulExpandKeyId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExpandKeyRefIdx;         

    Tls_PrepareHKDFLabel(
                        LusNegotitdCiprSutId,
                        &LaaHkdfLabel[TLS_ZERO],
                        &LucHkdfLabelLen,
                        &TLS_LABEL_KEY[TLS_ZERO],
                        (uint8)(sizeof(TLS_LABEL_KEY)),
                        NULL_PTR,
                        (uint8)TLS_ZERO);   
                            
    LddReturnValue = Tls_PerformHKDFExpand(
    /* JobId to perform HKDF Expand */
                                        LulExpandJobId,
    /* KeyId to be Used with the Expand Job Id perform HKDF Expand */                                       
                                        LulExpandKeyId,
    /* PRK key Id to be copied into Exapnd key Id to perform HKDF Expand */                                        
                                        LulSrcHSTrficSecrt,
    /* specifies which CipherSuite to be used for HKDF Expand */                                    
                                        LusNegotitdCiprSutId,
    /* specifies the output Key length to be generated */                                  
                                        LusOkmLen,
    /* specifies the Input Info to be used in HKDF expand */                                     
                                        &LaaHkdfLabel[TLS_ZERO],
    /* specifies the length of Input Info */                                     
                                        LucHkdfLabelLen,
    /* KeyId to be used to store the Final Key */                                    
                                        LulTrgHSWriteKey);                                         
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_DeriveWriteIV                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : THis function will generate the Sender_Write_IV and 
                         store in required SLot **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0288                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveWriteIV(
                                    const uint32 LulSrcHSTrficSecrt, // PRK
                                    const uint32 LulTrgHSWriteIV, // Place to store the Write IV
                                    const uint16 LusActiveConnIdx)
{    
    Std_ReturnType LddReturnValue;    
    uint32 LulExpandJobId;
    uint32 LulExpandKeyId;
    uint16 LusNegotitdCiprSutId;
    uint16 LusOkmLen;
    uint8 LucHkdfLabelLen;

    /* "iv" in Hex to avoid parasoft */
    const uint8 TLS_LABEL_IV[TLS_TWO] = 
    {
        0x69U, 0x76U
    };  

    uint8 LaaHkdfLabel[TLS_MAX_LABEL_SIZE] = {0};

    /* get the negotaited cipher suit from server side */
    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
    Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
    ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId; 

    LulExpandJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExpandJobRefIdx; 

    /* Get the L value, since for all algo its 12 bytes 
    need to get this from any online source why 12 ?    */
    LusOkmLen = (uint16)TLS_TWELVE;

    LulExpandKeyId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExpandKeyRefIdx;

    Tls_PrepareHKDFLabel(
                        LusNegotitdCiprSutId,
                        &LaaHkdfLabel[TLS_ZERO],
                        &LucHkdfLabelLen,
                        &TLS_LABEL_IV[TLS_ZERO],
                        (uint8)(sizeof(TLS_LABEL_IV)),
                        NULL_PTR,
                        (uint8)TLS_ZERO); 
                            
    LddReturnValue = Tls_PerformHKDFExpand(
    /* JobId to perform HKDF Expand */
                                        LulExpandJobId,
    /* KeyId to be Used with the Expand Job Id perform HKDF Expand */                                       
                                        LulExpandKeyId,
    /* PRK key Id to be copied into Exapnd key Id to perform HKDF Expand */                                        
                                        LulSrcHSTrficSecrt,
    /* specifies which CipherSuite to be used for HKDF Expand */                                    
                                        LusNegotitdCiprSutId,
    /* specifies the output Key length to be generated */                                  
                                        LusOkmLen,
    /* specifies the Input Info to be used in HKDF expand */                                     
                                        &LaaHkdfLabel[TLS_ZERO],
    /* specifies the length of Input Info */                                     
                                        LucHkdfLabelLen,
    /* KeyId to be used to store the Final Key */                                    
                                        LulTrgHSWriteIV);                                         
                                        
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_DeriveDerivativeKey                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : when this is called u will generate   **
**                        either early derivative key" and set in TlsCsmDerivedKeyRef **
**                        or HandShake derivative key" and set in TlsCsmDerivedKeyRef **
**                        Depending on where its being called  **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0289                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveDerivativeKey(
                                    const uint16 LusActiveConnIdx,
                                    /* PRK , can be used for both 
                                    early derivate key and Handshake derivate key */
                                    const uint32 LulSecret)
{      
    Std_ReturnType LddReturnValue;    
    uint32 LulExpandJobId;
    uint32 LulExpandKeyId;
    uint32 LulTrgDerivativeKey;
    uint16 LusNegotitdCiprSutId;
    uint16 LusOkmLen;
    uint8 LucHkdfLabelLen1;

    /* "derived" in Hex to avoid parasoft */
    const uint8 TLS_LABEL_DERIVED[TLS_SEVEN] = 
    {
        0x64U, 0x65U, 0x72U, 0x69U, 0x76U, 0x65U, 0x64U
    };    
    
    uint8 LaaHkdfLabel[TLS_MAX_LABEL_SIZE] = {0};

    /* get the negotaited cipher suit from server side */
    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
    Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
    ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId; 

    LulExpandJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExpandJobRefIdx; 
    
    LulExpandKeyId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExpandKeyRefIdx;    
    
    /* either Storing for early Derivative key or HandShake Derivate Key */
    LulTrgDerivativeKey = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmDerivedKeyRefIdx; 

    /* becuz while with derived label , we shd eithe send Hash of empty string
        of 256 or 384 */
    if ((uint16)TLS_AES_256_GCM_SHA384 == LusNegotitdCiprSutId)
    {
        LusOkmLen = (uint16)TLS_FOURTYEIGHT;

        /* preapre content with Derived Label */
        Tls_PrepareHKDFLabel(
                        LusNegotitdCiprSutId,
                        &LaaHkdfLabel[TLS_ZERO],
                        &LucHkdfLabelLen1,
                        &TLS_LABEL_DERIVED[TLS_ZERO],
                        (uint8)(sizeof(TLS_LABEL_DERIVED)),
                        &LaaEmptyHash_384[TLS_ZERO],
                        (uint8)TLS_FOURTYEIGHT);  
    }
    else
    {
        LusOkmLen = (uint16)TLS_THIRTYTWO;

        /* preapre content with Derived Label */
        Tls_PrepareHKDFLabel(
                        LusNegotitdCiprSutId,
                        &LaaHkdfLabel[TLS_ZERO],
                        &LucHkdfLabelLen1,
                        &TLS_LABEL_DERIVED[TLS_ZERO],
                        (uint8)(sizeof(TLS_LABEL_DERIVED)),
                        &LaaEmptyHash_256[TLS_ZERO],
                        (uint8)TLS_THIRTYTWO); 
    }
 
                            
    LddReturnValue = Tls_PerformHKDFExpand(                                        
    /* JobId to perform HKDF Expand */
                                        LulExpandJobId,
    /* KeyId to be Used with the Expand Job Id perform HKDF Expand */                                       
                                        LulExpandKeyId,
    /* PRK key Id to be copied into Exapnd key Id to perform HKDF Expand */                                        
                                        LulSecret,
    /* specifies which CipherSuite to be used for HKDF Expand */                                    
                                        LusNegotitdCiprSutId,
    /* specifies the output Key length to be generated */                                  
                                        LusOkmLen,
    /* specifies the Input Info to be used in HKDF expand */                                     
                                        &LaaHkdfLabel[TLS_ZERO],
    /* specifies the length of Input Info */                                     
                                        LucHkdfLabelLen1,
    /* KeyId to be used to store the Final Key */                                    
                                        LulTrgDerivativeKey);                                         
  
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/***************HandShake Key Functions*****************/


/************BinderKey_GenerationFunctions*********************************/

/*******************************************************************************
** Function Name        : Tls_DeriveBinderMaterials                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : call this function to get the 
                        "early secret" in TlsCsmEarlySecretKeyRef and 
                        Binder key in TlsCsmBinderKeyRef 
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0227                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_DeriveBinderMaterials(const uint32 LulPskCsmKeyId,
                                        const uint32 LulTrgBinderKey,
                                        const Tls_PSKHashType LddPSKHashType)
{
    Std_ReturnType LddReturnValue;   
    
    LddReturnValue = Tls_DeriveBinderSecret(LulPskCsmKeyId, LddPSKHashType);
    
    if (E_OK == LddReturnValue)
    {                              
        LddReturnValue = Tls_DeriveBinderKey(LulTrgBinderKey, LddPSKHashType);
    }
    
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_DeriveBinderSecret                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : when this is called u will generate "early secret" 
                          in TlsCsmEarlySecretKeyRef   **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0228                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveBinderSecret(
                                        const uint32 LulPskCsmKeyId,
                                        const Tls_PSKHashType LddPSKHashType)
{
 
    Std_ReturnType LddReturnValue;   
    uint32 LulExtractKeyId;
    uint32 LulExtractJobId;
    uint32 LulEarlySecret;
    uint16 LusNegotitdCiprSutId;


    LulExtractKeyId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[TLS_ZERO].
    usTlsConnHandshakeRefBaseIndx].ulTlsCsmHKDFExtractKeyRefIdx; 

    LulExtractJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[TLS_ZERO].
    usTlsConnHandshakeRefBaseIndx].ulTlsCsmHKDFExtractJobRefIdx; 
    
    LulEarlySecret = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[TLS_ZERO].
    usTlsConnHandshakeRefBaseIndx].ulTlsCsmEarlySecretKeyRefIdx;

    LusNegotitdCiprSutId = (TLS_SHA_384 == LddPSKHashType) ? 
    (uint16)TLS_AES_256_GCM_SHA384 : (uint16)TLS_AES_128_GCM_SHA256;
    
    /* Generate Early Secret */
    /* HMAC(Salt,Key) */
    LddReturnValue = Tls_PerformHKDFExtract(
    /* JobId to perform HKDF extract */
                                            LulExtractJobId, 
    /* KeyId to be Used with the Extract Job Id perform HKDF extract */                                            
                                            LulExtractKeyId,
    /* specifies if Salt is available or not for HKDF extract */                                       
                                            TLS_FALSE,
    /* specifies if IKM is available or not for HKDF extract */                                              
                                            TLS_TRUE,
    /*  specifies which CipherSuite to be used for HKDF extract */ 
                                            LusNegotitdCiprSutId,
    /* Salt KeyId to be used to perform HKDF extract */ 
                                            (uint32)TLS_ZERO,
    /* IKM KeyId to be used to perform HKDF extract */                                             
                                            LulPskCsmKeyId,
    /* PRK Id KeyId to store the derived Key */
                                            LulEarlySecret);
    
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_DeriveBinderKey                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : when this is called u will generate   **
**                        either early derivative key" and set in TlsCsmDerivedKeyRef **
**                        or HandShake derivative key" and set in TlsCsmDerivedKeyRef **
**                        Depending on where its being called  **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0229                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_DeriveBinderKey(
                                        const uint32 LulTrgBinderKey,
                                        const Tls_PSKHashType LddPSKHashType)
{      
    Std_ReturnType LddReturnValue;    
    uint32 LulExpandJobId;
    uint32 LulExpandKeyId;
    uint32 LulEarlySecret;
    uint16 LusOkmLen;
    uint16 LusNegotitdCiprSutId;
    uint8 LucHkdfLabelLen;

    /* "ext binder" in Hex to avoid parasoft */
    const uint8 TLS_LABEL_EXT_BINDER[TLS_TEN] = 
    {
        0x65U, 0x78U, 0x74U, 0x20U, 0x62U, 0x69U, 0x6EU, 0x64U, 0x65U, 0x72U
    };  

    uint8 LaaHkdfLabel[TLS_MAX_LABEL_SIZE] = {0};
        
    LulExpandJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[TLS_ZERO].
    usTlsConnHandshakeRefBaseIndx].ulTlsCsmHKDFExpandJobRefIdx;

    LulExpandKeyId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[TLS_ZERO].
    usTlsConnHandshakeRefBaseIndx].ulTlsCsmHKDFExpandKeyRefIdx; 

    LulEarlySecret = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[TLS_ZERO].
    usTlsConnHandshakeRefBaseIndx].ulTlsCsmEarlySecretKeyRefIdx;
  
    if (TLS_SHA_384 == LddPSKHashType)
    {
        /* Get the L value, since for its depends of the hash type pf the PSK */
        LusOkmLen = (uint16)TLS_FOURTYEIGHT;
        LusNegotitdCiprSutId = (uint16)TLS_AES_256_GCM_SHA384;

        /* preapre content with Ext binder Label */
        Tls_PrepareHKDFLabel(
                        LusNegotitdCiprSutId,
                        &LaaHkdfLabel[TLS_ZERO],
                        &LucHkdfLabelLen,
                        &TLS_LABEL_EXT_BINDER[TLS_ZERO],
                        (uint8)(sizeof(TLS_LABEL_EXT_BINDER)),
                        &LaaEmptyHash_384[TLS_ZERO],
                        (uint8)TLS_FOURTYEIGHT);   
    }
    else
    {
        LusOkmLen = (uint16)TLS_THIRTYTWO;
        LusNegotitdCiprSutId = (uint16)TLS_AES_128_GCM_SHA256;

         /* preapre content with Ext binder Label */
        Tls_PrepareHKDFLabel(
                        LusNegotitdCiprSutId,
                        &LaaHkdfLabel[TLS_ZERO],
                        &LucHkdfLabelLen,
                        &TLS_LABEL_EXT_BINDER[TLS_ZERO],
                        (uint8)(sizeof(TLS_LABEL_EXT_BINDER)),
                        &LaaEmptyHash_256[TLS_ZERO],
                        (uint8)TLS_THIRTYTWO); 
    }


    LddReturnValue = Tls_PerformHKDFExpand(
    /* JobId to perform HKDF Expand */
                                        LulExpandJobId,
    /* KeyId to be Used with the Expand Job Id perform HKDF Expand */                                       
                                        LulExpandKeyId,
    /* PRK key Id to be copied into Exapnd key Id to perform HKDF Expand */                                        
                                        LulEarlySecret,
    /* specifies which CipherSuite to be used for HKDF Expand */                                    
                                        LusNegotitdCiprSutId,
    /* specifies the output Key length to be generated */                                  
                                        LusOkmLen,
    /* specifies the Input Info to be used in HKDF expand */                                     
                                        &LaaHkdfLabel[TLS_ZERO],
    /* specifies the length of Input Info */                                     
                                        LucHkdfLabelLen,
    /* KeyId to be used to store the Final Key */                                    
                                        LulTrgBinderKey); 
                                        
    if (E_OK == LddReturnValue)
    {
        /* The PskBinderEntry is computed in the same way as the Finished
        message (Section 4.4.4) but with the BaseKey being the binder_key
        derived via the key schedule from the corresponding PSK which is
        being offered (see Section 7.1). */

        /* preapre content with Finish Label , as per RFC 8446 */
        Tls_PrepareHKDFLabel(
                        LusNegotitdCiprSutId,
                        &LaaHkdfLabel[TLS_ZERO],
                        &LucHkdfLabelLen,
                        &TLS_LABEL_FINISHED[TLS_ZERO],
                        (uint8)(sizeof(TLS_LABEL_FINISHED)),
                        NULL_PTR,
                        (uint8)TLS_ZERO); 

        LddReturnValue = Tls_PerformHKDFExpand(
        /* JobId to perform HKDF Expand */
                                            LulExpandJobId,
        /* KeyId to be Used with the Expand Job Id perform HKDF Expand */                                       
                                            LulExpandKeyId,
        /* base key being the binder key as per 8446 */                                         
                                            LulTrgBinderKey,
        /* specifies which CipherSuite to be used for HKDF Expand */                                    
                                            LusNegotitdCiprSutId,
        /* specifies the output Key length to be generated */                                  
                                            LusOkmLen,
        /* specifies the Input Info to be used in HKDF expand */                                     
                                            &LaaHkdfLabel[TLS_ZERO],
        /* specifies the length of Input Info */                                     
                                            LucHkdfLabelLen,
        /* KeyId to be used to store the Final Key, to store the FInish kind of key */                                    
                                            LulTrgBinderKey); 
    }

    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/************BinderKey_GenerationFunctions*********************************/

/************FinishedKey*********************************/

/*******************************************************************************
** Function Name        : Tls_DeriveFinishedKey                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : THis function will generate the Finished Key and 
                         store in required SLot **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0230                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_DeriveFinishedKey(const uint16 LusActiveConnIdx,
                                                const uint32 LulSrcHSTrficSecrt)
{

    /* finished_key =
       HKDF-Expand-Label(BaseKey, "finished", "", Hash.length)
    */    
    Std_ReturnType LddReturnValue;
    uint32 LulExpandJobId;
    uint32 LulExpandKeyId;
    uint32 LulFinishedKey;
    uint16 LusNegotitdCiprSutId;
    uint16 LusOkmLen;
    uint8 LucHkdfLabelLen;

    uint8 LaaHkdfLabel[TLS_MAX_LABEL_SIZE];
    
    /* get the negotaited cipher suit from server side */
    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
    Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
    ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId; 

    LulExpandJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExpandJobRefIdx; 

    /* Get the L value */
    LusOkmLen = 
    (LusNegotitdCiprSutId == (uint16)TLS_AES_256_GCM_SHA384) ? 
    (uint16)TLS_FOURTYEIGHT : (uint16)TLS_THIRTYTWO; 
    
    LulExpandKeyId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExpandKeyRefIdx;     

    /* Currenlty in this Key slot only early secret was stored at the start of derive ES 
    but then after that , the slot becomes uselss, so reusing the slot for to store finish key    */
    LulFinishedKey = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmEarlySecretKeyRefIdx; 
        
    Tls_PrepareHKDFLabel(
                        LusNegotitdCiprSutId,
                        &LaaHkdfLabel[TLS_ZERO],
                        &LucHkdfLabelLen,
                        &TLS_LABEL_FINISHED[TLS_ZERO],
                        (uint8)(sizeof(TLS_LABEL_FINISHED)),
                        NULL_PTR,
                        (uint8)TLS_ZERO);   
                            
    LddReturnValue = Tls_PerformHKDFExpand(
    /* JobId to perform HKDF Expand */
                                        LulExpandJobId,
    /* KeyId to be Used with the Expand Job Id perform HKDF Expand */                                       
                                        LulExpandKeyId,
    /* PRK key Id to be copied into Exapnd key Id to perform HKDF Expand */                                        
                                        LulSrcHSTrficSecrt,
    /* specifies which CipherSuite to be used for HKDF Expand */                                    
                                        LusNegotitdCiprSutId,
    /* specifies the output Key length to be generated */                                  
                                        LusOkmLen,
    /* specifies the Input Info to be used in HKDF expand */                                     
                                        &LaaHkdfLabel[TLS_ZERO],
    /* specifies the length of Input Info */                                     
                                        LucHkdfLabelLen,
    /* KeyId to be used to store the Finished Key */                                    
                                        LulFinishedKey);                                         
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/* parasoft-end-suppress MISRAC2012-RULE_1_1-a-2 */
/* parasoft-end-suppress CERT_C-API00-a-3 */
/* parasoft-end-suppress CERT_C-DCL19-a-3 */
/* parasoft-end-suppress MISRAC2012-RULE_20_1-a-4 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-d-2 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 */