/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_Client.c                                                   **
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
**  Design ID  : TLS_SDD_0306                                                 **
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
* #section Tls_Client_c_REF_1
* Violates MISRAC2012-RULE_5_4-c-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Client_c_REF_2
* Violates MISRAC2012-RULE_5_4-d-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Client_c_REF_3
* Violates MISRAC2012-RULE_20_1-a-4: Do not precede the #include directive with
* executable code
* Justification: Macros are defined by AUTOSAR standards and internal
* specifications.
*
* #section Tls_Client_c_REF_4
* Violates CERT_C-DCL19-a-3: Variable "" should be declared as locally as
* possible
* Justification: Coding guidelines suggest the variables are declared on the
* start of the function.
*
* #section Tls_Client_c_REF_5
* Violates CERT_C-API00-a-3: Parameter '' is not validated before use.
* Justification: This parameter either originates from the configuration or is
* validated in the main API before being passed as an argument to the local API.
* Hence, additional validation within the local API is not required and has been
* manually verified.
*
* #section Tls_Client_c_REF_6
* Violates MISRAC2012-RULE_8_13-a-4: Pass parameter '' with const specifier
* Justification: The parameter '' is part of a common function
* pointer interface used for ClientHello message
* construction. All functions referenced by the function pointer table
* share the same prototype and write extension
* or protocol data into the provided handshake buffer.
* Although this specific function does not directly modify the buffer,
* it forwards the pointer to other functions which populate the handshake
* structure. Therefore the parameter cannot be declared as const without
* breaking the uniform function pointer interface and preventing valid
* buffer updates in other implementations.
*
* #section Tls_Client_c_REF_7
* Violates CERT_C-DCL13-a-3: Pass parameter '' with const specifier
* Justification: The parameter '' is part of a common function
* pointer interface used for ClientHello message
* construction. All functions referenced by the function pointer table
* share the same prototype and write extension
* or protocol data into the provided handshake buffer.
* Although this specific function does not directly modify the buffer,
* it forwards the pointer to other functions which populate the handshake
* structure. Therefore the parameter cannot be declared as const without
* breaking the uniform function pointer interface and preventing valid
* buffer updates in other implementations.
*/

/* parasoft-begin-suppress MISRAC2012-RULE_5_4-c-2 
"Reason: Tls_Client_c_REF_1" */
/* parasoft-begin-suppress MISRAC2012-RULE_5_4-d-2 
"Reason: Tls_Client_c_REF_2" */
/* parasoft-begin-suppress MISRAC2012-RULE_20_1-a-4 
"Reason: Tls_Client_c_REF_3" */
/* parasoft-begin-suppress CERT_C-DCL19-a-3 
"Reason: Tls_Client_c_REF_4" */
/* parasoft-begin-suppress CERT_C-API00-a-3 
"Reason: Tls_Client_c_REF_5" */


/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Tls_Internal.h"
#include "Tls_Inline.h"
#include "Tls_External.h"


/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/

/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetCHSupportedVersExt(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* LpHShakeBuf,
                            uint16* const LpExtLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetPskKeyExModesExt(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* LpHShakeBuf,
                            uint16* const LpExtLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_UpdateCHHDr(const TcpIp_TlsConnectionIdType  LddConnectionId,
                                                        const uint16 LusExtLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateSHSupportedVersExt(
                                            const uint16 LusActiveConnIdx,
                                            const uint8* const LpCHShakeBuf, 
                                            uint16* const LpExtLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateSHKeySExt(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpSHShakeBuf,
                            uint16* const LpExtLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/* Signature related Internal functions */
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_GetChSignatureExt(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* LpHShakeBuf,
							uint16* const LpExtLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/* Key Share related Internal functions */
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_GetChKeyShareExt(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* LpHShakeBuf,
							uint16* const LpExtLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/* Supported Group related Internal functions */
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_GetChSupportedGrpExt(
							const TcpIp_TlsConnectionIdType  LddConnectionId,
							uint8* LpHShakeBuf,
							uint16* const LpExtLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/



/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/


/*******************************************************************************
** Function Name        : Tls_ValidateSHSupportedVersExt                   **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares Supported version Extension data.**
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
**  Design ID           : TLS_SDD_0216                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateSHSupportedVersExt(
                                            const uint16 LusActiveConnIdx,
                                            const uint8* const LpCHShakeBuf, 
                                            uint16* const LpExtLen)
{
    Std_ReturnType  LddReturnValue;
    uint16 LusTlsVersion;
    uint16 LusIndex;
   
    /* move to len field */
    LusIndex = (uint16)TLS_TWO;
    
    /* Get the Ext length */
    *LpExtLen = Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]);


    if (*LpExtLen == (uint16)TLS_TWO)
    {
        /* move to Data field */
        LusIndex += (uint16)TLS_TWO;
        LusTlsVersion = Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]);  
        
        if ((uint16)TLS_VERSION_V13 == LusTlsVersion)
        {
            LddReturnValue = E_OK; 
        }
        else
        {
            /* If this extension is present, clients MUST ignore the
            ServerHello.legacy_version value and MUST use only the
            "supported_versions" extension to determine the selected version. If
            the "supported_versions" extension in the ServerHello contains a
            version not offered by the client or contains a version prior to
            TLS 1.3, the client MUST abort the handshake with an
            "illegal_parameter" alert. */
            
            Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
            LddReturnValue = E_NOT_OK; 
        }
    }
    else
    {
        Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
        LddReturnValue = E_NOT_OK; 
    }
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_ValidateShLegacyVersion                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function Validates Legacy Version of CH.        **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
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
**  Design ID           : TLS_SDD_0214                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateShLegacyVersion(
       const TcpIp_TlsConnectionIdType  LddConnectionId,
       const uint8* const LpSHShakeBuf,
                            uint16* const LpLengthProcessed)
{
    
    Std_ReturnType  LddReturnValue;
    uint16 LusActiveConnIdx;
    uint16 LusVersion;

    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);

    /* Get the SH version */
    LusVersion = Tls_GetExtensionLength(LpSHShakeBuf);
    
    if ((uint16)TLS_SSL_VERSION_V30 == LusVersion)
    {
        /* Implementations MUST NOT send a ClientHello.legacy_version or
        ServerHello.legacy_version set to 0x0300 or less.  Any endpoint
        receiving a Hello message with ClientHello.legacy_version or
        ServerHello.legacy_version set to 0x0300 MUST abort the handshake
        with a "protocol_version" alert. */
        
        Tls_GaaAlert[LusActiveConnIdx] = TLS_PROTOCOL_VERSION;
        LddReturnValue = E_NOT_OK;
    }
    else
    {
        LddReturnValue = E_OK;
        /* Legacy version (2 bytes) + 32 bytes random num bytes */
        *LpLengthProcessed = (uint16)TLS_THIRTYFOUR;
    }

    return LddReturnValue; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_ValidateShSessionId                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function Validates Session Id of CH.        **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
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
**  Design ID           : TLS_SDD_0213                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateShSessionId(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpSHShakeBuf,
                            uint16* const LpLengthProcessed)
{
    Std_ReturnType  LddReturnValue;
    uint16 LusActiveConnIdx;
    
    if ((uint8)TLS_ZERO != *LpSHShakeBuf)
    {
        /* legacy_session_id_echo:  The contents of the client's
        legacy_session_id field.  Note that this field is echoed even if
        the client's value corresponded to a cached pre-TLS 1.3 session
        which the server has chosen not to resume.  A client which
        receives a legacy_session_id_echo field that does not match what
        it sent in the ClientHello MUST abort the handshake with an
        "illegal_parameter" alert. */
        
        LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);

        Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER; // parasoft-suppress CERT_C-ARR30-a-2 CERT_C-ARR39-a-2 CERT_C-EXP08-b-2 CERT_C-FIO37-a-1 CERT_C-MSC19-a-3 CERT_C-STR31-a-1 MISRAC2012-DIR_4_1-a-2 MISRAC2012-RULE_18_1-a-2 "Reason: Boundary values are validated before use and verified manually.
        LddReturnValue = E_NOT_OK;
    }
    else
    {
        *LpLengthProcessed = (uint16)TLS_ONE;
        LddReturnValue = E_OK;
    }

    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"
/*******************************************************************************
** Function Name        : Tls_ValidateShCompressMethd                         **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares Compression Method data.     **
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
**  Design ID           : TLS_SDD_0212                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateShCompressMethd(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpSHShakeBuf,
                            uint16* const LpLengthProcessed)
{
 
    Std_ReturnType  LddReturnValue;
    uint16 LusActiveConnIdx;

    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);

    if (*(LpSHShakeBuf) == (uint8)TLS_ZERO)
    {  
        LddReturnValue = E_OK;
        *LpLengthProcessed = (uint16)TLS_ONE;
    }
    else
    {
        Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER; // parasoft-suppress CERT_C-ARR30-a-2 CERT_C-ARR39-a-2 CERT_C-EXP08-b-2 CERT_C-FIO37-a-1 CERT_C-MSC19-a-3 CERT_C-STR31-a-1 MISRAC2012-DIR_4_1-a-2 MISRAC2012-RULE_18_1-a-2 "Reason: Boundary values are validated before use and verified manually.
        LddReturnValue = E_NOT_OK;
    }

    return LddReturnValue;  
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_ValidateShCipherSuites                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function Validates Session Id of CH.        **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
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
**  Design ID           : TLS_SDD_0211                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_ValidateShCipherSuites(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpSHShakeBuf,
                            uint16* const LpLengthProcessed)
{
    Std_ReturnType  LddReturnValue;
    Tls_FrameType*  LpTlsFrame;
    uint16 LusSHCiphr;
    uint16 LusCHCiphr;
    uint16 LusNumOfCipherWrkrs;
    uint16 LusCiphrWrkrCtr;
    uint16 LusCiphrWrkrStartIdx;
    uint16 LusCiphrWrkrLookUpIdx;
    uint16 LusActiveConnIdx;   
 
    LddReturnValue = E_NOT_OK;

    LusCiphrWrkrStartIdx = 
    Tls_GaaConnection[LddConnectionId].usCiphrWrkrConnIdx;
     
    LusNumOfCipherWrkrs = 
    Tls_GaaConnection[LddConnectionId].usNumOfCiphrWrkrs;
    
    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);
    LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];

    LusSHCiphr = Tls_GetExtensionLength(LpSHShakeBuf);
  
    for (LusCiphrWrkrCtr = LusNumOfCipherWrkrs; 
        LusCiphrWrkrCtr > (uint16)TLS_ZERO; LusCiphrWrkrCtr--)
    {
        LusCiphrWrkrLookUpIdx = 
        Tls_GaaCiphrWrkrLookUp[LusCiphrWrkrStartIdx + LusCiphrWrkrCtr -
        (uint16)TLS_ONE];
   
        LusCHCiphr = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
        LusCiphrWrkrLookUpIdx].ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId;
        
        if (LusCHCiphr == LusSHCiphr)
        {  
            LddReturnValue = E_OK;
            *LpLengthProcessed = (uint16)TLS_TWO;
            LpTlsFrame->usNegotiatedCiprSutIdx = LusCiphrWrkrLookUpIdx;
            break;
        }     
    }

    /* cipher_suite:  The single cipher suite selected by the server from
    the list in ClientHello.cipher_suites.  A client which receives a
    cipher suite that was not offered MUST abort the handshake with an
    "illegal_parameter" alert. */
    if (E_NOT_OK == LddReturnValue)
    {
        Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
    }
    
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"
/*******************************************************************************
** Function Name        : Tls_ValidateShExtensions                             **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares Extension data.              **
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
**  Design ID           : TLS_SDD_0244                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateShExtensions(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpSHShakeBuf,
                            uint16* const LpLengthProcessed)
{
    Std_ReturnType  LddReturnValue;
    Tls_AuthType LddAuthType;
    uint16 LusTotalExtLen;
    uint16 LusExtLen;
    uint16 LusIndex;
    uint16 LusTag;
    uint16 LusSummedExtLen;
	uint16 LusActiveConnIdx;
    #if (STD_ON == TLS_PSK_IDENTITY_SUPPORT_ENABLED)
    Tls_FrameType*  LpTlsFrame;
    uint32 LulPskCsmKeyId;
    #endif
    boolean LaaMandatoryExt[TLS_TWO] = {TLS_FALSE};

    LusSummedExtLen = (uint16)TLS_ZERO;
    LusIndex = (uint16)TLS_ZERO;

    /* get the Total extension Field value */
    LusTotalExtLen = Tls_GetExtensionLength(&LpSHShakeBuf[LusIndex]);

    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId); 

    #if (STD_ON == TLS_PSK_IDENTITY_SUPPORT_ENABLED)    
    LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx]; 
    #endif
    
    if (LusTotalExtLen < (uint16)TLS_EIGHT)
    {
        Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
        LddReturnValue = E_NOT_OK;
    }
    else
    {
        LddAuthType = Tls_GaaConnection[LddConnectionId].ddTlsAuthType;

        /* Move to First Tag len value extension */
        LusIndex += (uint16)TLS_TWO;
        do
        { 
            LddReturnValue = Tls_CheckSupportedTag(TLS_SERVER_HELLO,
                                                    &LpSHShakeBuf[LusIndex]);
        
            if (E_OK == LddReturnValue)
            {
                LusTag = 
                ((uint16)((uint16)LpSHShakeBuf[LusIndex] << (uint16)TLS_EIGHT) | 
                            (uint16)LpSHShakeBuf[LusIndex + (uint16)TLS_ONE]); 
                            
                switch (LusTag)
                {
                    case (uint16)TLS_SUPPORTED_VERSIONS:
                    
                        LaaMandatoryExt[TLS_ZERO] = TLS_TRUE;      
                        
                        LddReturnValue = 
                        Tls_ValidateSHSupportedVersExt(
                                                    LusActiveConnIdx,
                                                    &LpSHShakeBuf[LusIndex],
                                                    &LusExtLen);
                        break;
              
                    case (uint16)TLS_PRE_SHARED_KEY:  

                        if (TLS_AUTH_PSK == LddAuthType)
                        {
                            LaaMandatoryExt[TLS_ONE] = TLS_TRUE;
                            #if (STD_ON == TLS_PSK_IDENTITY_SUPPORT_ENABLED)
                                LddReturnValue = 
                                Tls_ValidateServerPskExt(LddConnectionId,
                                                &LpSHShakeBuf[LusIndex],
                                                &LusExtLen,
												&LulPskCsmKeyId); 
                                                
                                /* store the NegotiatedPskCsmKeyId , to use later
                                during Key derivation for client */                    
                                LpTlsFrame->ulNegotiatedPskCsmKeyId = 
                                                                LulPskCsmKeyId;
                            #endif
                            
                        }
                        else
                        {
                            Tls_GaaAlert[LusActiveConnIdx] = 
                                                    TLS_UNSUPPORTED_EXTENSION;
                            LddReturnValue = E_NOT_OK;
                        }
                        break;

                    case (uint16)TLS_CH_KEY_SHARE:                       
                        
                        if (TLS_AUTH_CERT == LddAuthType)
                        {
                            LaaMandatoryExt[TLS_ONE] = TLS_TRUE;
                            LddReturnValue = 
                            Tls_ValidateSHKeySExt(LddConnectionId,
                                                &LpSHShakeBuf[LusIndex],
                                                &LusExtLen); 
                        }
                        else
                        {
                            Tls_GaaAlert[LusActiveConnIdx] = 
                                                    TLS_UNSUPPORTED_EXTENSION;                            
                            LddReturnValue = E_NOT_OK;
                        }
                        break;
                        
                    default:
                        /*  LusExtLen = 
                        ((uint16)((uint16)LpSHShakeBuf[LusIndex + 
                                                            (uint16)TLS_TWO] << 
                                            (uint16)TLS_EIGHT) | 
                            (uint16)LpSHShakeBuf[LusIndex + (uint16)TLS_THREE]);*/                         
                        
                        /* Implementations MUST NOT send extension responses if the remote
                        endpoint did not send the corresponding extension requests, with the
                        exception of the "cookie" extension in the HelloRetryRequest.  Upon
                        receiving such an extension, an endpoint MUST abort the handshake
                        with an "unsupported_extension" alert. */
                            
                        /* since our tls will not request any extensions other then supported versions
                        and Preshared key , so sending alert */
                            
                        Tls_GaaAlert[LusActiveConnIdx] = 
                                                    TLS_UNSUPPORTED_EXTENSION;
                        LddReturnValue =  E_NOT_OK;   
                        break;                
                }
            }
            else
            {
                Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;                
            }
        
            if (E_OK == LddReturnValue)
            {
                /* move to next Tag*/
                LusIndex += (uint16)(TLS_FOUR + LusExtLen);

                LusSummedExtLen += (uint16)(TLS_FOUR + LusExtLen);            
            }

        }while((LusSummedExtLen < LusTotalExtLen) && (E_OK == LddReturnValue));
        
        if (E_OK == LddReturnValue)
        {
            if ((LaaMandatoryExt[TLS_ZERO] == TLS_FALSE) ||
            (LaaMandatoryExt[TLS_ONE] == TLS_FALSE))
            {
                Tls_GaaAlert[LusActiveConnIdx] = TLS_MISSING_EXTENSION;
                LddReturnValue = E_NOT_OK;                    
            }
        }
        
        *LpLengthProcessed = LusSummedExtLen + (uint16)TLS_TWO;        
    }

    return LddReturnValue; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

/*******************************************************************************
** Function Name        : Tls_GetPskKeyExModesExt                             **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares data releted to  Extension data.**
**                      : PSK key exchange modes .**
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
**  Design ID           : TLS_SDD_0209                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetPskKeyExModesExt(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* LpHShakeBuf,
                            uint16* const LpExtLen)
{
    uint16 LusSuprtedVer;
    uint16 LusTemp; 
    uint16 LusIndex;

    /* Tag */
    LusSuprtedVer = Tls_Htons((uint16)TLS_CH_PSK_KEY_EXCHANGE_MODES);
    Tls_MemCpy(LpHShakeBuf, &LusSuprtedVer, (uint16)TLS_TWO);     
    LusIndex = (uint16)TLS_TWO;
    
    /* Length */
    LusTemp = Tls_Htons((uint16)TLS_TWO);
    Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO);
    LusIndex += (uint16)TLS_TWO;
 
    /* Value */  
    /* Psk Exchange mode length */
    LpHShakeBuf[LusIndex] = (uint8)TLS_ONE;
    LusIndex += (uint16)TLS_ONE;
    /* Psk only Key establishment  */
    LpHShakeBuf[LusIndex] = (uint8)TLS_ZERO;
                
    *LpExtLen = (*LpExtLen) + (uint16)TLS_SIX;
    TLS_UNUSED(LddConnectionId);

    return E_OK;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_GetCHSupportedVersExt                   **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares Supported version Extension data.**
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
**  Design ID           : TLS_SDD_0208                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetCHSupportedVersExt(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* LpHShakeBuf,
                            uint16* const LpExtLen)
{
    uint16 LusTemp;
    uint16 LusIndex;
    uint16 LusSuprtedVer;
    
    /* Tag */
    LusSuprtedVer = Tls_Htons((uint16)TLS_SUPPORTED_VERSIONS);
    Tls_MemCpy(LpHShakeBuf, &LusSuprtedVer, (uint16)TLS_TWO);
                                                    
    LusIndex = (uint16)TLS_TWO;
            
    /* Length */
    LusTemp = Tls_Htons((uint16)TLS_THREE);
    Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO);
    LusIndex += (uint16)TLS_TWO; 
    

                
    LpHShakeBuf[LusIndex] = (uint8)TLS_TWO;
    LusIndex += (uint16)TLS_ONE;
    LpHShakeBuf[LusIndex] = (uint8)TLS_THREE;
    LusIndex += (uint16)TLS_ONE;  
    LpHShakeBuf[LusIndex] = (uint8)TLS_FOUR;
    
    *LpExtLen = (*LpExtLen) + (uint16)TLS_SEVEN; 
    TLS_UNUSED(LddConnectionId); 

    return E_OK;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PrepareChLegacyVersion                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function provides Legacy Version.              **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
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
**  Design ID           : TLS_SDD_0207                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_PrepareChLegacyVersion(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed)
{
    uint16 Lusversion12;
    
    Lusversion12 = Tls_Htons((uint16)TLS_VERSION_V12);
    
    Tls_MemCpy(LpHShakeBuf, &Lusversion12, (uint16)TLS_TWO);   
    
    /* 2 bytes of CH hello Leagcy Verison */
    *LpLengthProcessed = (uint16)TLS_TWO;  
    TLS_UNUSED(LddConnectionId);
    return E_OK; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PrepareChRandomNum                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function provides 32 bytes of Random data.     **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
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
**  Design ID           : TLS_SDD_0206                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_PrepareChRandomNum(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed)
{
    
    Std_ReturnType LddRetrnValCsm;
    uint32 LulresultLenPtr; 
    uint8 LaaRandom[TLS_THIRTYTWO] = {0};
    

    LddRetrnValCsm = Tls_PerformRandomGenerate(
        /* JobId to perform Random Generate */
        Tls_GaaConnection[LddConnectionId].ulTlsCsmRndmGenJobRefId,
        /* Holds a pointer to the memory location which will hold the result 
        of the random number generation */         
        &LaaRandom[TLS_ZERO],
        /* Holds a pointer to the memory location in which the result length in 
        bytes is stored */        
        &LulresultLenPtr);  
    
    if ((E_OK == LddRetrnValCsm) && ((uint32)TLS_THIRTYTWO == LulresultLenPtr))
    {
        Tls_MemCpy(LpHShakeBuf, &LaaRandom[TLS_ZERO], (uint16)TLS_THIRTYTWO);
        *LpLengthProcessed = (uint16)TLS_THIRTYTWO;
    }
    else
    {
        LddRetrnValCsm = E_NOT_OK;
    }
    return LddRetrnValCsm;  
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PrepareChSessionId                              **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function provides Session Id.                  **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
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
**  Design ID           : TLS_SDD_0205                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_PrepareChSessionId(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed)
{
 
    *LpHShakeBuf = (uint8)TLS_ZERO;
    *LpLengthProcessed = (uint16)TLS_ONE;
    TLS_UNUSED(LddConnectionId);
    return E_OK;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PrepareChCipherSuites                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares Cipher suites data from the  **
**                        configuration.                                      **
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
**  Design ID           : TLS_SDD_0203                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_PrepareChCipherSuites(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed)
{
    uint16 LusNumOfCipherWrkrs;
    uint16 LusCiphrWrkrStartIdx;
    uint16 LusCiphrWrkrLookUpIdx;
    uint16 LusCiphrWrkrCtr;
    uint16  LusTemp;
    uint16  LusIndex;
 
    LusCiphrWrkrStartIdx = 
                        Tls_GaaConnection[LddConnectionId].usCiphrWrkrConnIdx;
 
 
    LusNumOfCipherWrkrs = Tls_GaaConnection[LddConnectionId].usNumOfCiphrWrkrs;
    
    /* convert and copy */
    LusTemp = Tls_Htons(LusNumOfCipherWrkrs * ((uint16)TLS_TWO));
    Tls_MemCpy(LpHShakeBuf, &LusTemp, (uint16)TLS_TWO); 
    
    /* update by 2 bytes to move to cipher data */
    LusIndex = (uint16)TLS_TWO;
 

 
    for (LusCiphrWrkrCtr = LusNumOfCipherWrkrs; 
            LusCiphrWrkrCtr > (uint16)TLS_ZERO;
            LusCiphrWrkrCtr--)
    { 
        LusCiphrWrkrLookUpIdx = Tls_GaaCiphrWrkrLookUp[LusCiphrWrkrStartIdx
        + LusCiphrWrkrCtr - (uint16)TLS_ONE];
    
        /* convert and copy */
        LusTemp = Tls_Htons(Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
                LusCiphrWrkrLookUpIdx].ulTlsCiphrSutDefRefIdx
                ].usTlsCiphersuiteId);
                
        Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO);          
            LusIndex += (uint16)TLS_TWO;
    }
    
    *LpLengthProcessed = (uint16)TLS_TWO + 
                                    (LusNumOfCipherWrkrs * ((uint16)TLS_TWO));
                                                                   
    return E_OK; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PrepareChCompressMethd                      **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares Compression Method data.     **
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
**  Design ID           : TLS_SDD_0202                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_PrepareChCompressMethd(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed)
{
 
    *LpHShakeBuf = (uint8)TLS_ONE;
    LpHShakeBuf[TLS_ONE] = (uint8)TLS_ZERO;
    *LpLengthProcessed = (uint16)TLS_TWO;
    TLS_UNUSED(LddConnectionId);
    return E_OK;  
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PrepareChExtensions                             **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares Extension data.              **
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
**  Design ID           : TLS_SDD_0201                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_PrepareChExtensions(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* const LpHShakeBuf, // parasoft-suppress MISRAC2012-RULE_8_13-a-4 CERT_C-DCL13-a-3 "Reason: Tls_Client_c_REF_5 Tls_Client_c_REF_7.
                            uint16* const LpLengthProcessed)
{
    Std_ReturnType  LddReturnValue;
    Tls_AuthType LddAuthType;
    uint16 LusExtLen;
    uint16 LusIndex;
    uint8 LucTotlExt;
    uint8 LucLoopCnt;
	uint8 LucAuthIdx;


    const Tls_CHExtFuncPtrType Tls_GSaaCHExtFuncPtr[TLS_TWO][TLS_FOUR] =
    {
        { 
            /* PSK */   
            &Tls_GetCHSupportedVersExt, 
            &Tls_GetPskKeyExModesExt,
            #if (STD_ON == TLS_PSK_IDENTITY_SUPPORT_ENABLED)        
            &Tls_GetChPSKExt,
            #else
            &Tls_ProcessDummyCHExt,
            #endif    
            &Tls_ProcessDummyCHExt
        },
        /* as new extension comes , we can add here */
        {
            /* AUTH */
            &Tls_GetCHSupportedVersExt, 
            &Tls_GetChSupportedGrpExt,          
            &Tls_GetChKeyShareExt,
            &Tls_GetChSignatureExt        
        }		
    };


    LucLoopCnt = (uint8)TLS_ZERO;
    LusExtLen = (uint16)TLS_ZERO;
    *LpLengthProcessed = (uint16)TLS_ZERO;
    /* +2 bytes of Total extension Length */
    LusIndex = (uint16)TLS_TWO;



    LddAuthType = Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
    
    if (TLS_AUTH_PSK == LddAuthType)
    {
        LucTotlExt = (uint8)TLS_THREE;
		LucAuthIdx = (uint8)TLS_ZERO;
    }
    else
    {
        LucTotlExt = (uint8)TLS_FOUR;
		LucAuthIdx = (uint8)TLS_ONE;
    }

    do
    {    
        LddReturnValue = Tls_GSaaCHExtFuncPtr[LucAuthIdx][LucLoopCnt]
                                                    (LddConnectionId,
                                                    &LpHShakeBuf[LusIndex],
                                                    &LusExtLen);
                                                    
        /* Increment loop count to point to next element */
        LucLoopCnt++;
        LusIndex = LusExtLen + (uint16)TLS_TWO;

    }while ((LucLoopCnt < LucTotlExt) && (E_OK == LddReturnValue));

    /* two bytes Total Extension Length */
    *LpLengthProcessed = LusExtLen + (uint16)TLS_TWO;
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_ValidateServerHello                             **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Validates the structure and content of a            **
**                        Server Hello message..                              **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
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
**  Design ID           : TLS_SDD_0200                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ValidateServerHello(
                                        const uint16 LusActiveConnIdx,
                                        const uint8* const LpHShakeBuf)
{
    TcpIp_TlsConnectionIdType LddConnectionId;
    Std_ReturnType  LddReturnValue;
    uint16          LusHShakeLen;
    uint16          LusHBufIdx;
    uint16          LusLengthProcessed;  
    uint8           LucLoopCnt;


    const Tls_SHValidationFuncPtrType LaaSHValidationFuncPtr[TLS_FIVE] =
    {
        &Tls_ValidateShLegacyVersion,       
        &Tls_ValidateShSessionId,  
        &Tls_ValidateShCipherSuites,   
        &Tls_ValidateShCompressMethd,
        &Tls_ValidateShExtensions
    };


    LddConnectionId = 
            Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;

    /* Validate the server hello handshake header Tag. */
    if ((uint8)TLS_SERVER_HELLO == (*LpHShakeBuf))
    {
        LusHBufIdx = (uint16)TLS_TWO;
        
        LusHShakeLen = Tls_GetExtensionLength(&LpHShakeBuf[LusHBufIdx]);
        
        if ((LusHShakeLen < TLS_MINIMUM_SIZE_OF_SH) || 
                                    ((uint8)TLS_ZERO != LpHShakeBuf[TLS_ONE]))
        {            
            Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
            LddReturnValue = E_NOT_OK; 
        }
        else
        {
            LusHBufIdx += (uint16)TLS_TWO;             
            LucLoopCnt = (uint8)TLS_ZERO; 
            
            do
            {
                LddReturnValue = LaaSHValidationFuncPtr[LucLoopCnt]
                                                    (LddConnectionId,
                                                    &LpHShakeBuf[LusHBufIdx],
                                                    &LusLengthProcessed);
            
                /* Increment loop count to point to next element */
                LucLoopCnt++;
            
                LusHBufIdx += LusLengthProcessed;
            }while ((LusHBufIdx < (uint16)(LusHShakeLen - (uint16)TLS_FOUR)) && 
                    (LucLoopCnt < (uint8)TLS_FIVE) && (E_OK == LddReturnValue));
	
			/* this tells us Where the SH msg ends */
            Tls_HandshakeFrame[LusActiveConnIdx].usProcessedHSBytes += LusHBufIdx; 
         
            LusLengthProcessed = LusHBufIdx - (uint16)TLS_FOUR;
            
            /* Verify that all parsed elements were successfully processed as
               per the length specified in the handshake header. */
            if ((E_OK == LddReturnValue) && (LusLengthProcessed != 
                                                                LusHShakeLen))
            {
                /* LddAlert.ddDescription = TLS_DECODE_ERROR */   
                LddReturnValue = E_NOT_OK;
            }                              
        }
    }
    else
    {
        Tls_GaaAlert[LusActiveConnIdx] = TLS_UNEXPECTED_MESSAGE;
        LddReturnValue = E_NOT_OK;     
    }
    
    /* if any alert is logged, comman place to close everthing */
    if (E_OK != LddReturnValue)
    {
        Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
        Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);        
    }
    
    return LddReturnValue; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_ValidateEE                                      **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Validates the structure and content of a            **
**                        Server Hello message..                              **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
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
**  Design ID           : TLS_SDD_0199                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ValidateEE(const uint16 LusActiveConnIdx,
                                const uint8* const LpHShakeBuf, uint16* const LpEELen)
{
    Std_ReturnType  LddReturnValue;
    uint32 LulEELen;
    uint32 LulLoopCnt;

    /* 0x08,
        0x00, 0x00, 0x02, 0x00, 0x00 
        Validate the server hello handshake header Tag. */
    if ((uint8)TLS_ENCRYPTED_EXTENSIONS == (*LpHShakeBuf))
    {
        /* 0x00, 0x00, 0x01 */
        
        LulEELen =
                ((uint32)TLS_ZERO  << (uint8)TLS_TWENTYFOUR) |
                ((uint32)LpHShakeBuf[TLS_ONE]  << (uint8)TLS_SIXTEEN) |
                ((uint32)LpHShakeBuf[TLS_TWO]  << (uint8)TLS_EIGHT)  |
                ((uint32)LpHShakeBuf[TLS_THREE]);    
                
        /* since a valid EE msg is  0x08, 0x00, 0x00, 0x02, 0x00, 0x00 */
        if (LulEELen >= (uint32)TLS_TWO)
        {
            LulLoopCnt = (uint32)TLS_FOUR;
            /* check if all bytes should be zero */
            do
            {
                if ((uint32)TLS_ZERO == LpHShakeBuf[LulLoopCnt])
                {
                    LddReturnValue = E_OK;
                }
                else
                {
                    /* The EncryptedExtensions message contains extensions that can be
                    protected, i.e., any which are not needed to establish the
                    cryptographic context but which are not associated with individual
                    certificates.  The client MUST check EncryptedExtensions for the
                    presence of any forbidden extensions and if any are found MUST abort
                    the handshake with an "illegal_parameter" alert. */

                    Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;

                    LddReturnValue = E_NOT_OK;
                    break;
                }
                LulLoopCnt++;
            }while ((LulLoopCnt - (uint32)TLS_FOUR) < LulEELen);

            /* return what is the total length of EE, along with HS header */            
            *LpEELen =
                (uint16)(
                ((uint16)((uint16)LpHShakeBuf[TLS_TWO] << (uint16)TLS_EIGHT)) |
                (uint16)LpHShakeBuf[TLS_THREE]);
    
            *LpEELen += (uint16)TLS_FOUR;
        }
        else
        {
            Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
            LddReturnValue = E_NOT_OK;
        }                            
    }
    else
    {
        Tls_GaaAlert[LusActiveConnIdx] = TLS_UNEXPECTED_MESSAGE;
        LddReturnValue = E_NOT_OK;     
    }    
    return LddReturnValue; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"





/*******************************************************************************
** Function Name        : Tls_GetChSupportedGrpExt                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares Cipher suites data from the  **
**                        configuration.                                      **
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
**  Design ID           : TLS_SDD_0336                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"                            
static Std_ReturnType  Tls_GetChSupportedGrpExt(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* LpHShakeBuf,
                            uint16* const LpExtLen)
{
    uint16 LusNumOfHSWrkrs;
    uint16 LusHSWrkrStartIdx;
    uint16 LusHSWrkrLookUpIdx;
    uint16 LusHSWrkrCtr;
    uint16  LusTemp;
    uint16  LusIndex1;
    uint16 LusSuprtedGrp;
     
    LusHSWrkrStartIdx = Tls_GaaConnection[LddConnectionId].usHSWrkrIdx;
 
    LusNumOfHSWrkrs = Tls_GaaConnection[LddConnectionId].usNumOfHSWorkers;
    
    /* Tag */
    LusIndex1 = (uint16)TLS_ZERO;
    LusSuprtedGrp = Tls_Htons((uint16)TLS_CH_SUPPORTED_GROUPS);
    Tls_MemCpy(&LpHShakeBuf[LusIndex1], &LusSuprtedGrp, (uint16)TLS_TWO);
        

    /* Length */
    LusIndex1 += (uint16)TLS_TWO;    
    LusTemp = LusNumOfHSWrkrs * (uint16)TLS_TWO;
    
    /* why +2 : its the len inside the value field */
    LusTemp = Tls_Htons(LusTemp + (uint16)TLS_TWO);
    Tls_MemCpy(&LpHShakeBuf[LusIndex1], &LusTemp, (uint16)TLS_TWO);   
 

    /* VALUE : LEN */
    LusIndex1 += (uint16)TLS_TWO; 

    LusTemp = Tls_Htons(LusNumOfHSWrkrs * (uint16)TLS_TWO);
    
    Tls_MemCpy(&LpHShakeBuf[LusIndex1], &LusTemp, (uint16)TLS_TWO); 


    /* VALUE : Actual */
    LusIndex1 += (uint16)TLS_TWO; 
    
    for (LusHSWrkrCtr = (uint16)TLS_ZERO; LusHSWrkrCtr < LusNumOfHSWrkrs;
                                                            LusHSWrkrCtr++)
    { 
        LusHSWrkrLookUpIdx = Tls_GaaHSLookUp[LusHSWrkrStartIdx + LusHSWrkrCtr];
        
        /* convert and copy */
        LusTemp = 
        Tls_Htons(Tls_GaaHandshake[LusHSWrkrLookUpIdx].usTlsSupportedGrpId);
                
        Tls_MemCpy(&LpHShakeBuf[LusIndex1], &LusTemp, (uint16)TLS_TWO);
        
        LusIndex1 += (uint16)TLS_TWO;
    }
                          
    /* why +6: Tag + :Len + Value Len */
    *LpExtLen = 
    (uint16)((*LpExtLen) + ((uint16)(LusNumOfHSWrkrs * (uint16)TLS_TWO)) + 
                                                            (uint16)TLS_SIX);
    
    return E_OK; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_GetChKeyShareExt                                **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares Key Share data.              **
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
**  Design ID           : TLS_SDD_0337                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"                            
static Std_ReturnType  Tls_GetChKeyShareExt(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* LpHShakeBuf,
                            uint16* const LpExtLen)
{
    uint16 LusNumOfHSWrkrs;
    uint16 LusHSWrkrStartIdx;
    uint16 LusHSWrkrLookUpIdx;
    uint16 LusHSWrkrCtr;
    uint16 LusTemp;
    uint16 LusIndex;
    uint16 LusKeyShare;
    uint32 LulKeyExchangeJobId;
    uint32 LulKeyExchangeKeyId;
    Std_ReturnType  LddReturnValue;
    uint8 LucpublicValue[TLS_MAX_KEY_SHARE_SIZE];
    uint32 LulpublicValueLength;
    uint16 LusKeyShareLength;
    uint16 LusKeyShareListLength;
    uint16 LusActualKShareLen;
    uint16 LusExpKShareLen;
     
    LddReturnValue = E_NOT_OK;
    LusHSWrkrStartIdx = Tls_GaaConnection[LddConnectionId].usHSWrkrIdx;
 
    LusNumOfHSWrkrs = Tls_GaaConnection[LddConnectionId].usNumOfHSWorkers;
    
    /* Convert data into network byte order */
    LusKeyShare = Tls_Htons((uint16)TLS_CH_KEY_SHARE);
    
    /* Encode the Tag field for Key Share extension.*/
    Tls_MemCpy(&LpHShakeBuf[TLS_ZERO], &LusKeyShare, (uint16)TLS_TWO);
        
    /* Update processed bytes of data : Key Share Tag*/
    LusIndex = (uint16)TLS_SIX;       
    
    LusKeyShareListLength = (uint16)TLS_ZERO;
     
    
    /* Iterate and prepare Key share for all respective named groups */
    for (LusHSWrkrCtr = (uint16)TLS_ZERO; LusHSWrkrCtr < LusNumOfHSWrkrs;
                                                            LusHSWrkrCtr++)
    {          
        /* Fetch the Handshake to get named group */
        LusHSWrkrLookUpIdx = Tls_GaaHSLookUp[LusHSWrkrStartIdx + LusHSWrkrCtr];
        
        /* Fecth the Key Id to trigger Key pair generation */
        LulKeyExchangeKeyId = Tls_GaaHandshake[LusHSWrkrLookUpIdx]
                              .ulTlsCsmKeyExchangeKeyRefIdx;
        
        /* Fecth the Job Id to trigger Key pair generation */
        LulKeyExchangeJobId = Tls_GaaHandshake[LusHSWrkrLookUpIdx]
                              .ulTlsCsmKeyExchangeCalcPubValJobRefIdx;
        
        /* Initiate Key Pair Generation for a Named group */
        LddReturnValue = Tls_PerformKeypairGeneration(                                        
                                    LulKeyExchangeJobId,
                                    LulKeyExchangeKeyId,
                                    &LucpublicValue[TLS_ZERO],      
                                    &LulpublicValueLength);
                                    
        /* Encode of Named Group, Key Exchange length and Key for a group */
        if (LddReturnValue == E_OK)
        {      
            /* things to check 
            1)check if the LulpublicValueLength is proper for the grpId */

            LusTemp = Tls_GaaHandshake[LusHSWrkrLookUpIdx].usTlsSupportedGrpId;

            Tls_GetKShareExpLen(LusTemp, &LusExpKShareLen); 

            LusActualKShareLen = 
                    (uint16)(LulpublicValueLength & (uint32)TLS_LSB_MASK_U16);

            if (LusActualKShareLen == LusExpKShareLen)
            {            
                /* Convert data into network byte order */
                LusTemp = Tls_Htons(LusTemp);
                
                /* Encode the Named Group ID */
                Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO);
                
                /* Update processed bytes : Named Group Tag /Id */
                LusIndex += (uint16)TLS_TWO;
                
                /* Convert data into network byte order */
                LusTemp = Tls_Htons(LusActualKShareLen);  
                
                /* Encode the Key Length */
                Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO); 
                
                /* Update processed bytes : Length Bytes of Key data*/
                LusIndex += (uint16)TLS_TWO;
                
                /* Encode the Public Key Value */
                Tls_MemCpy(&LpHShakeBuf[LusIndex], &LucpublicValue[TLS_ZERO],
                                                            LusActualKShareLen); 
                
                /* Update processed bytes: Public Key data*/
                LusIndex += LusActualKShareLen;
                
                LusKeyShareListLength += 
                                (uint16)(LusActualKShareLen + (uint16)TLS_FOUR);               
            }
            else
            {
                LddReturnValue = E_NOT_OK;
            }    
        }
        
        if (E_NOT_OK == LddReturnValue)
        {
            break;
        }
    }
    
    if (E_OK == LddReturnValue)
    {
        /* Convert data into network byte order */
        LusTemp = Tls_Htons(LusKeyShareListLength);    
        
        /* Encode the Key Share list length*/
        Tls_MemCpy(&LpHShakeBuf[LusIndex - LusKeyShareListLength - TLS_TWO], 
                                                    &LusTemp, (uint16)TLS_TWO);
        
        /* Calculate total extension length */
        LusKeyShareLength = (uint16)(LusKeyShareListLength + (uint16)TLS_TWO);
        
        /* Convert data into network byte order */
        LusTemp = Tls_Htons(LusKeyShareLength);    
        
        /* Encode the total Key Share extension length*/
        Tls_MemCpy(&LpHShakeBuf[LusIndex - LusKeyShareLength - TLS_TWO], 
                                                    &LusTemp, (uint16)TLS_TWO);
                                                
        /* Return : Tag and Key Share Extension Length */
        *LpExtLen = (*LpExtLen) + LusKeyShareLength + (uint16)TLS_FOUR;       
    }

    return LddReturnValue; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_GetChSignatureExt                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares Cipher suites data from the  **
**                        configuration.                                      **
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
**  Design ID           : TLS_SDD_0338                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"                            
static Std_ReturnType  Tls_GetChSignatureExt(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* LpHShakeBuf,
                            uint16* const LpExtLen)
{
    uint16 LusNumOfHSWrkrs;
    uint16 LusHSWrkrStartIdx;
    uint16 LusHSWrkrLookUpIdx;
    uint16 LusHSWrkrCtr;
    uint16  LusTempp;
    uint16  LusIndexx;
    uint16 LusSign;
     
    LusHSWrkrStartIdx = Tls_GaaConnection[LddConnectionId].usHSWrkrIdx;
 
    LusNumOfHSWrkrs = Tls_GaaConnection[LddConnectionId].usNumOfHSWorkers;
    
    /* Tag */
    LusIndexx = (uint16)TLS_ZERO;
    LusSign = Tls_Htons((uint16)TLS_CH_SIGNATURE_ALGORITHMS);
    Tls_MemCpy(&LpHShakeBuf[TLS_ZERO], &LusSign, (uint16)TLS_TWO);
        

    /* Length */
    LusIndexx += (uint16)TLS_TWO;    
    LusTempp = LusNumOfHSWrkrs * (uint16)TLS_TWO;
    
    /* why +2 : its the len inside the value field */
    LusTempp = Tls_Htons(LusTempp + (uint16)TLS_TWO);
    Tls_MemCpy(&LpHShakeBuf[LusIndexx], &LusTempp, (uint16)TLS_TWO);   
 

    /* VALUE : LEN */
    LusIndexx += (uint16)TLS_TWO; 

    LusTempp = Tls_Htons(LusNumOfHSWrkrs * (uint16)TLS_TWO);
    
    Tls_MemCpy(&LpHShakeBuf[LusIndexx], &LusTempp, (uint16)TLS_TWO); 


    /* VALUE : Actual */
    LusIndexx += (uint16)TLS_TWO; 
    
    for (LusHSWrkrCtr = (uint16)TLS_ZERO; LusHSWrkrCtr < LusNumOfHSWrkrs;
                                                            LusHSWrkrCtr++)
    { 
        LusHSWrkrLookUpIdx = Tls_GaaHSLookUp[LusHSWrkrStartIdx + LusHSWrkrCtr];

		/* convert and copy */
        LusTempp = 
        Tls_Htons(Tls_GaaHandshake[LusHSWrkrLookUpIdx].usTlsSignatureId);
		
		Tls_MemCpy(&LpHShakeBuf[LusIndexx], &LusTempp, (uint16)TLS_TWO);
        
        LusIndexx += (uint16)TLS_TWO;
    }
                          
    /* why +6: Tag + :Len + Value Len */
    *LpExtLen = 
    (uint16)((*LpExtLen) + ((uint16)(LusNumOfHSWrkrs * (uint16)TLS_TWO)) + 
                                                            (uint16)TLS_SIX);
     
    /* Just call this , when all the extension are done prepared */
    Tls_UpdateCHHDr(LddConnectionId, *LpExtLen);
    
    return E_OK; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_UpdateCHHDr                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function Validates Session Id of CH.        **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
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
**  Design ID           : TLS_SDD_0339                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_UpdateCHHDr(const TcpIp_TlsConnectionIdType  LddConnectionId,
                                                    const uint16 LusExtLen)
{
    uint8* LpHShakeHdrPtr;
    uint16 LusActiveConnIdx;    
    uint16 LusNumOfCipherWrkrs;   
    uint16 LusHSPos; 
    uint16 LusTemp;   
    uint16 LusTempLen;   
 
    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);     
    LpHShakeHdrPtr = &Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO];  // parasoft-suppress CERT_C-ARR30-a-2 CERT_C-ARR39-a-2 CERT_C-EXP08-b-2 CERT_C-FIO37-a-1 CERT_C-MSC19-a-3 CERT_C-STR31-a-1 MISRAC2012-DIR_4_1-a-2 MISRAC2012-RULE_18_1-a-2 "Reason: Boundary values are validated before use and verified manually.
    
    LusNumOfCipherWrkrs = Tls_GaaConnection[LddConnectionId].usNumOfCiphrWrkrs;
    
    /* this will point to the Total extension Length Position */
    LusHSPos = (uint16)TLS_FOURTYTHREE + 
                                ((uint16)TLS_TWO * (uint16)LusNumOfCipherWrkrs);
    
    /* Convert and store */
    LusTemp = Tls_Htons(LusExtLen);
    Tls_MemCpy(&LpHShakeHdrPtr[LusHSPos], &LusTemp, (uint16)TLS_TWO);


    /* update for handshake header */ 
    *LpHShakeHdrPtr = (uint8)TLS_CLIENT_HELLO;
    
                            
    LpHShakeHdrPtr[TLS_ONE] = (uint8)TLS_ZERO;
    
    LusTempLen = LusExtLen;
     
    /* the fixed fields + 2 bytes extension length  */
    LusTempLen += (uint16)(LusHSPos + (uint16)TLS_TWO);
    
    /* why -4 : since HS Header shd not be included */
    LusTempLen -= (uint16)TLS_FOUR;

    /* Shifts operate on the numeric value inside the CPU, not on memory bytes 
    so no endiannes issue */
    LpHShakeHdrPtr[TLS_TWO] = (uint8)((uint16)LusTempLen >> (uint16)TLS_EIGHT);
    
    LpHShakeHdrPtr[TLS_THREE] = 
                            (uint8)(LusTempLen & (uint16)TLS_LSB_MASK);    
    
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_ValidateSHKeySExt                      **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Once this internal API is called , it start looping **
**       through each PSK conatiner associated with the server**
      connection for the each PSK it checks wether the same **
      PSK is present in CH message and if its there it      **
      validats the respective binder  **
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
**  Design ID           : TLS_SDD_0340                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateSHKeySExt(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpSHShakeBuf,
                            uint16* const LpExtLen)
{ 
    Std_ReturnType  LddSrvrRtrnVal;
    uint16  LusSuprtdGrp;
    Tls_FrameType*  LpTlsFrame;
    uint16  LusExtLen;
    uint16 LusActualKShareLen;
    uint16 LusExpKShareLen;
    uint16 LusNumOfHSWrkrs;
    uint16 LusHSWrkrStartIdx;
    uint16 LusHSWrkrLookUpIdx;
    uint16 LusHSWrkrCtr;
    uint16  LusTemp;
    uint16 LusIndex;
    uint16 LusActiveConnIdx;
    boolean  Lblstatus;
    
 
    /* move to len field */
    LusIndex = (uint16)TLS_TWO;
    
    /* Get the Ext length */
    LusExtLen = Tls_GetExtensionLength(&LpSHShakeBuf[LusIndex]);
    *LpExtLen = LusExtLen;

    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);
 
    if ((LusExtLen >= (uint16)TLS_MIN_SH_KEYSHARE_LEN) && 
                                (LusExtLen <= (uint16)TLS_MAX_SH_KEYSHARE_LEN))
    {
        /* move to Group field */
        LusIndex += (uint16)TLS_TWO;
		LusSuprtdGrp = 
                ((uint16)((uint16)LpSHShakeBuf[LusIndex] << (uint16)TLS_EIGHT) | 
                            (uint16)LpSHShakeBuf[LusIndex + (uint16)TLS_ONE]);	

        Lblstatus = Tls_CheckSupportedGrpValue(LusSuprtdGrp);

        /* check if the Supoted grp is tshere in the CH key share */
        if (Lblstatus)
        {
            /* resseting it back, since using the same variable  */
            Lblstatus = TLS_FALSE;
            
            LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];
            
            /* could be made into a new shared function */
            /* fetch the handshake conatiner Index */  
            LusHSWrkrStartIdx = 
                            Tls_GaaConnection[LddConnectionId].usHSWrkrIdx;
 
            LusNumOfHSWrkrs = 
                        Tls_GaaConnection[LddConnectionId].usNumOfHSWorkers;

            for (LusHSWrkrCtr = (uint16)TLS_ZERO; 
                                LusHSWrkrCtr < LusNumOfHSWrkrs; LusHSWrkrCtr++)
            { 
                LusHSWrkrLookUpIdx = 
                            Tls_GaaHSLookUp[LusHSWrkrStartIdx + LusHSWrkrCtr];

                LusTemp = 
                    Tls_GaaHandshake[LusHSWrkrLookUpIdx].usTlsSupportedGrpId;

                if (LusSuprtdGrp == LusTemp)
                {
                    Lblstatus = TLS_TRUE;
                    /* store it for further key derivation */
                    LpTlsFrame->usNegotiatedSupGrpIdx = LusHSWrkrLookUpIdx;
                    break;
                }
            }
            
            if (Lblstatus == TLS_TRUE)
            {
                /* move to Key Exchange Length Field */
                LusIndex += (uint16)TLS_TWO;
                LusActualKShareLen = 
                                Tls_GetExtensionLength(&LpSHShakeBuf[LusIndex]);
                                
                Tls_GetKShareExpLen(LusSuprtdGrp, &LusExpKShareLen);

                if (LusActualKShareLen == LusExpKShareLen)
                {  
                    /* store the address where the Key share is there in our global struct */
                    LpTlsFrame->pPatnerKeyShare = &LpSHShakeBuf[LusIndex];
                    LddSrvrRtrnVal = E_OK;
                }
                else
                {
                    Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
                    LddSrvrRtrnVal = E_NOT_OK;
                }    
            }
            else
            {
                Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
                LddSrvrRtrnVal = E_NOT_OK;
            }
        }
        else
        {
            Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
            LddSrvrRtrnVal = E_NOT_OK;
        }
    }
    else
    {
        Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
        LddSrvrRtrnVal = E_NOT_OK;
    }   
    
    return LddSrvrRtrnVal;  
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/* parasoft-end-suppress CERT_C-API00-a-3 */
/* parasoft-end-suppress CERT_C-DCL19-a-3 */
/* parasoft-end-suppress MISRAC2012-RULE_20_1-a-4 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-d-2 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 */