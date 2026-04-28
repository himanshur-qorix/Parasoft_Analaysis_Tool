/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_Server.c                                                   **
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
**  Design ID  : TLS_SDD_0313                                                 **
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
     

/*
* #section Tls_Server_c_REF_1
* Violates MISRAC2012-RULE_8_13-a-4: Pass parameter '' with const specifier
* Justification: The parameter '' is part of a common function
* pointer interface used for ServerHello message
* construction. All functions referenced by the function pointer table
* share the same prototype and write extension
* or protocol data into the provided handshake buffer.
* Although this specific function does not directly modify the buffer,
* it forwards the pointer to other functions which populate the handshake
* structure. Therefore the parameter cannot be declared as const without
* breaking the uniform function pointer interface and preventing valid
* buffer updates in other implementations.
*
* #section Tls_Server_c_REF_2
* Violates CERT_C-DCL13-a-3: Pass parameter '' with const specifier
* Justification: The parameter '' is part of a common function
* pointer interface used for ServerHello message
* construction. All functions referenced by the function pointer table
* share the same prototype and write extension
* or protocol data into the provided handshake buffer.
* Although this specific function does not directly modify the buffer,
* it forwards the pointer to other functions which populate the handshake
* structure. Therefore the parameter cannot be declared as const without
* breaking the uniform function pointer interface and preventing valid
* buffer updates in other implementations.
*
* #section Tls_Server_c_REF_3
* Violates MISRAC2012-RULE_5_4-c-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Server_c_REF_4
* Violates MISRAC2012-RULE_5_4-d-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Server_c_REF_5
* Violates MISRAC2012-RULE_20_1-a-4: Do not precede the #include directive with
* executable code
* Justification: Macros are defined by AUTOSAR standards and internal
* specifications.
*
* #section Tls_Server_c_REF_6
* Violates CERT_C-DCL19-a-3: Variable "" should be declared as locally as
* possible
* Justification: Coding guidelines suggest the variables are declared on the
* start of the function.
*
* #section Tls_Server_c_REF_7
* Violates CERT_C-API00-a-3: Parameter '' is not validated before use.
* Justification: This parameter either originates from the configuration or is
* validated in the main API before being passed as an argument to the local API.
* Hence, additional validation within the local API is not required and has been
* manually verified.
*/


/* parasoft-begin-suppress MISRAC2012-RULE_5_4-c-2 
"Reason: Tls_Server_c_REF_1" */
/* parasoft-begin-suppress MISRAC2012-RULE_5_4-d-2 
"Reason: Tls_Server_c_REF_2" */
/* parasoft-begin-suppress MISRAC2012-RULE_20_1-a-4 
"Reason: Tls_Server_c_REF_3" */
/* parasoft-begin-suppress CERT_C-DCL19-a-3 
"Reason: Tls_Server_c_REF_4" */
/* parasoft-begin-suppress CERT_C-API00-a-3 
"Reason: Tls_Server_c_REF_5" */
     
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Tls_Internal.h"

//------------------------REMOVE LATER and check
#include "Tls_External.h"
#include "Tls_Inline.h"

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
static Std_ReturnType  Tls_ValidateChExtensions(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChCompressMethd(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChCipherSuites(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8*const  LpCHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChSessionId(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChLegacyVersion(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            uint16* const LpLengthProcessed);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_GetSHSupportedVerExt(
                            uint8* const LpHShakeBuf, uint16* const LpExtLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetShKeyShareExt(
                                const TcpIp_TlsConnectionIdType  LddConnectionId,
                                uint8* const LpHShakeBuf,
                                uint16* const LpExtLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"
                            
                            
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidatePskKeyExModesExt(
                                            const uint16 LusActiveConnIdx,
                                            const uint8* const LpCHShakeBuf);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateCHSupportedVersExt(
                                            const uint16 LusActiveConnIdx,
                                            const uint8* const LpCHShakeBuf);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChSuprtdGrp(
                                                const uint16 LusActiveConnIdx,
                                                const uint8* const LpCHShakeBuf);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChKShare(
                                                const uint16 LusActiveConnIdx,
                                                const uint8* const LpCHShakeBuf);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ParseDuplicatKShar(
                                            const uint16 LusActiveConnIdx,
                                            uint16* const LpParsdGrpsKShare,
                                            uint16* const LpParsdLen,
                                            const uint8* const LpCHShakeBuf);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateSGKSCombo(
                                            const uint16 LusActiveConnIdx,
                                            const uint8* const LpCHShakeBuf,
                                            /* KS */
                                            const uint16* const LpParsdGrpsKShare,
                                            const uint16 LusParsdLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"                                        


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChSign(
                                                const uint16 LusActiveConnIdx,
                                                const uint8* const LpCHShakeBuf);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

                                            
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChPskExt(
                                                const uint16 LusActiveConnIdx,
                                                const uint8* const LpCHShakeBuf);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ParseCHExtensions(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            Tls_ParsedExt* const LpParsedExtns,
                            uint8* const LpParsedCount,
                            uint8* const LpSeenMask);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_CHExtCheckDeps(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            Tls_ParsedExt* const LpParsedExtns,
                            uint8* const LpParsedCount,
                            uint8* const LpSeenMask);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_CHExtValidations(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            Tls_ParsedExt* const LpParsedExtns,
                            uint8* const LpParsedCount,
                            uint8* const LpSeenMask);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static boolean Tls_FindBestPubKS(
								const uint16 LusActiveConnIdx,
								const uint8* const LpCHShakeBuf,
								/* KS */
								const uint16* const LpParsdGrpsKShare,
								const uint16 LusParsdLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

/*******************************************************************************
** Function Name        : Tls_PrepareShLegacyVersion                          **
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
**  Design ID           : TLS_SDD_0232                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_PrepareShLegacyVersion(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed)
{
    uint16 Lusversion12;
    
    Lusversion12 =  Tls_Htons((uint16)TLS_VERSION_V12);
    Tls_MemCpy(LpHShakeBuf, &Lusversion12, (uint16)TLS_TWO);

    /* 2 bytes of SH hello Leagcy Verison */
    *LpLengthProcessed = (uint16)TLS_TWO; 
    TLS_UNUSED(LddConnectionId);
    return E_OK;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PrepareShRandomNumber                           **
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
**  Design ID           : TLS_SDD_0233                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_PrepareShRandomNumber(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed)
{
 
    Std_ReturnType LddRetrnValCsm;
    uint32 LulresultLenPtr; 
    uint16 LusActiveConnIdx;
    uint8 LaaRandom[TLS_THIRTYTWO];
     
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
        LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);
        
        /* COAs */
        Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
        LddRetrnValCsm = E_NOT_OK;
    }
    return LddRetrnValCsm;   
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PrepareShSessionId                              **
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
**  Design ID           : TLS_SDD_0234                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_PrepareShSessionId(
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
** Function Name        : Tls_PrepareShCipherSuite                            **
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
**  Design ID           : TLS_SDD_0235                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_PrepareShCipherSuite(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* const LpHShakeBuf, // parasoft-suppress MISRAC2012-RULE_8_13-a-4 CERT_C-DCL13-a-3 "Reason: Tls_Server_c_REF_7 Tls_Server_c_REF_8.
                            uint16* const LpLengthProcessed)
{
    *LpLengthProcessed = (uint16)TLS_TWO; 
    TLS_UNUSED(LddConnectionId);
    TLS_UNUSED(LpHShakeBuf);
    return E_OK; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PrepareShCompressionMethod                      **
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
**  Design ID           : TLS_SDD_0236                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_PrepareShCompressionMethod(
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
** Function Name        : Tls_PrepareShExtensions                             **
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
**  Design ID           : TLS_SDD_0238                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_PrepareShExtensions(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed)
{
    Tls_AuthType LddAuthType;
    Std_ReturnType LddReturnValue;
    uint16 LusExtLen;
    uint16 LusIndex;
    
    LddReturnValue = E_OK;
    LusExtLen = (uint16)TLS_ZERO;
    *LpLengthProcessed = (uint16)TLS_TWO;
    
    /* +2 bytes of Total extension Length */
    LusIndex = (uint16)TLS_TWO;
    
    LddAuthType = Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
    
    /* Create SupportedVersionsExtension for Server */
    Tls_GetSHSupportedVerExt(&LpHShakeBuf[LusIndex], &LusExtLen);
 
    /* 6 bytes of Supported ext */
    LusIndex += LusExtLen;


    if (TLS_AUTH_PSK == LddAuthType)
    {
        #if (STD_ON == TLS_PSK_IDENTITY_SUPPORT_ENABLED)    
            /* Create pre shared keys Extension for Server */
            Tls_GetServerPSKExt(&LpHShakeBuf[LusIndex], &LusExtLen);
        #endif
    }
    else
    {
        LddReturnValue = 
        Tls_GetShKeyShareExt(LddConnectionId, &LpHShakeBuf[LusIndex], &LusExtLen);
    }
 
    if (E_OK == LddReturnValue)
    {
        *LpLengthProcessed += LusExtLen;
        
        LusExtLen = Tls_Htons(LusExtLen);
        Tls_MemCpy(LpHShakeBuf, &LusExtLen, (uint16)TLS_TWO);      
    }

    return LddReturnValue; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_GetSHSupportedVerExt                   **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares Supported version Extension data.**
**                        For Server Hello                                    **
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
**  Design ID           : TLS_SDD_0332                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_GetSHSupportedVerExt(
                            uint8* const LpHShakeBuf, uint16* const LpExtLen)
{
    uint16 LusTemp;
    uint16 LusIndex;
    
    /* Tag */
    LusTemp = Tls_Htons((uint16)TLS_SUPPORTED_VERSIONS);
    Tls_MemCpy(LpHShakeBuf, &LusTemp, (uint16)TLS_TWO);

                                                
    LusIndex = (uint16)TLS_TWO;
    
    /* Length */
    LusTemp = Tls_Htons((uint16)TLS_TWO);
    Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO);
    LusIndex += (uint16)TLS_TWO;
    
    LpHShakeBuf[LusIndex] = (uint8)TLS_THREE;
    LusIndex += (uint16)TLS_ONE;  
    LpHShakeBuf[LusIndex] = (uint8)TLS_FOUR;
    
    *LpExtLen = (*LpExtLen) + (uint16)TLS_SIX;
    
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"




/*******************************************************************************
** Function Name        : Tls_GetShKeyShareExt                                **
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
**  Design ID           : TLS_SDD_0346                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"                            
static Std_ReturnType  Tls_GetShKeyShareExt(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* const LpHShakeBuf,
                            uint16* const LpExtLen)
{
    Std_ReturnType  LddReturnValue;
    Tls_FrameType* LpTlsShFrame;
    uint32 LulpublicValueLength;
    uint32 LulKeyExchangeJobId;
    uint32 LulKeyExchangeKeyId;
    uint16 LusGrp;
    uint16 LusIndex;
    uint16 LusTemp;
    uint16 LusExpKShareLen;
    uint16 LusActiveConnIdx;
    uint8 LucpublicValue[TLS_MAX_KEY_SHARE_SIZE];
    
    LusIndex = (uint16)TLS_ZERO;
    LulpublicValueLength = (uint32)TLS_MAX_KEY_SHARE_SIZE;
    
    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);
    
    LpTlsShFrame = &Tls_HandshakeFrame[LusActiveConnIdx];
    
    /* Convert data into network byte order : TAG field */
    LusTemp = Tls_Htons((uint16)TLS_CH_KEY_SHARE);   
    /* Encode the Tag field for Key Share extension.*/
    Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO); 
                                                    

    /* Fecth the Key Id to trigger Key pair generation */
    LulKeyExchangeKeyId = Tls_GaaHandshake[LpTlsShFrame->usNegotiatedSupGrpIdx]
                                                .ulTlsCsmKeyExchangeKeyRefIdx;
    
    /* Fecth the Job Id to trigger Key pair generation */
    LulKeyExchangeJobId = Tls_GaaHandshake[LpTlsShFrame->usNegotiatedSupGrpIdx]
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

        LusGrp = 
        Tls_GaaHandshake[LpTlsShFrame->usNegotiatedSupGrpIdx].usTlsSupportedGrpId;
            
        Tls_GetKShareExpLen(LusGrp, &LusExpKShareLen);
                    
        if ((uint16)(LulpublicValueLength & (uint32)TLS_LSB_MASK_U16) == 
                                                                LusExpKShareLen)
        {
            /* Key Share Tag field */
            LusIndex += (uint16)TLS_TWO; 
            /* Encode the Main len  */
            LusTemp = (uint16)((LulpublicValueLength + (uint32)TLS_FOUR) & 
                                                    (uint32)TLS_LSB_MASK_U16);   
                                                    
            LusTemp = Tls_Htons(LusTemp);
            Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO);
                                                                                    
            
            /* Value Field , Grpup Id */
            LusIndex += (uint16)TLS_TWO;
            /* Convert data into network byte order */
            LusTemp = 
                Tls_Htons(Tls_GaaHandshake[LpTlsShFrame->usNegotiatedSupGrpIdx].usTlsSupportedGrpId);
            /* Encode the Named group ID */
            Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO);
                                                
            
            /* Value Field ,  Key Exchange Length*/
            LusIndex += (uint16)TLS_TWO;
            LusTemp = (uint16)(LulpublicValueLength & (uint32)TLS_LSB_MASK_U16);        
            LusTemp = Tls_Htons(LusTemp); 
            /* Encode the Named group ID */
            Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO);                                          
            
              
            /* Value Field ,  Key Exchange */
            LusIndex += (uint16)TLS_TWO;
            LusTemp = (uint16)(LulpublicValueLength & (uint32)TLS_LSB_MASK_U16);        
            /* Encode the Key Exchange */
            Tls_MemCpy(&LpHShakeBuf[LusIndex], &LucpublicValue[TLS_ZERO], LusTemp);
            
            
            /* Return : Tag and Key Share Extension Length */
            *LpExtLen = (*LpExtLen) + LusTemp + (uint16)TLS_EIGHT;           
        }
        else
        {
            LddReturnValue = E_NOT_OK;
        }
    }
    
    return LddReturnValue; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_ValidateCHSupportedVersExt                   **
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
**  Design ID           : TLS_SDD_0333                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateCHSupportedVersExt(
                                                const uint16 LusActiveConnIdx,
                                                const uint8* const LpCHShakeBuf)
{
    Std_ReturnType  LddReturnValue;
    uint16 LusTlsVersion;
    uint16 LusExtLen;
    uint16 LusIndex;
    uint8 LucSupprtdExtLen;
    boolean  LblVer13Found;
    
    LblVer13Found = TLS_FALSE;
    /* move to len field */
    LusIndex = (uint16)TLS_TWO;
    
    /* Get the Ext length */
    LusExtLen = Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]);

    /* if supported ext is presemyt , then minimun 3 bytes shd be the length */
    if (LusExtLen >= (uint16)TLS_THREE)
    {
        /* move to Supported Version Length field */
        LusIndex += (uint16)TLS_TWO;
        LucSupprtdExtLen = LpCHShakeBuf[LusIndex];
        
        /* Just to verify that Supported Version Length field will always be 
        one byte less then Supported Version main Tag length */
        if ((uint16)((uint16)LucSupprtdExtLen + (uint16)TLS_ONE) == LusExtLen)
        {
            /* check if its a even byte always , since all 
            version are 2 bytes in rfc */
            if ((LucSupprtdExtLen & ((uint8)TLS_ONE)) == ((uint8)TLS_ZERO))
            {
                /* move to Data field */
                LusIndex += (uint16)TLS_ONE;
                do
                {
                    LusTlsVersion = 
                                Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]);
                                
                    if ((uint16)TLS_VERSION_V13 == LusTlsVersion)
                    {
                        LblVer13Found = TLS_TRUE;
                        break;
                    }  
                    LucSupprtdExtLen -= (uint8)TLS_TWO;
                    LusIndex += (uint16)TLS_TWO;
            
                }while(LucSupprtdExtLen > (uint8)TLS_ZERO); 

                if (TLS_TRUE == LblVer13Found)
                {
                    LddReturnValue = E_OK;
                }
                else
                {
                    Tls_GaaAlert[LusActiveConnIdx] = TLS_PROTOCOL_VERSION;
                    LddReturnValue = E_NOT_OK;  
                }                
            }
            else
            {
                Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
                LddReturnValue = E_NOT_OK;               
            }
        }
        else
        {
            Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
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
** Function Name        : Tls_ValidatePskKeyExModesExt                   **
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
**  Design ID           : TLS_SDD_0334                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidatePskKeyExModesExt(
                                                const uint16 LusActiveConnIdx,
                                                const uint8* const LpCHShakeBuf)
{
    Std_ReturnType  LddReturnValue;
    uint16 LusExtLen;
    uint16 LusIndex;
    uint8 LucPskKeExtLen;
    boolean  LblPskExFound;
   
    
    LblPskExFound = TLS_FALSE;
    /* move to Tag len field */
    LusIndex = (uint16)TLS_TWO;
            
    /* Get the Ext length */
    LusExtLen = Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]);

    /* if PSK key exchange mode ext is present , 
    then minimun 2 bytes shd be the length */
    if (LusExtLen >= (uint16)TLS_TWO)
    {
        /* move to PSK ke Ext len field */
        LusIndex += (uint16)TLS_TWO;
        LucPskKeExtLen = LpCHShakeBuf[LusIndex];
        
        /* Just to verify that Psk ext Len  field will always be 
        one byte less  main Tag length */
        if ((uint16)((uint16)LucPskKeExtLen + (uint16)TLS_ONE) == LusExtLen)
        {
            /* move to PSK exchange mode Length : field */
            LusIndex += (uint16)TLS_ONE;

            do
            {         
                if ((uint8)TLS_PSK_KE == LpCHShakeBuf[LusIndex])
                {
                    LblPskExFound = TLS_TRUE;
                    break;
                }  
                LucPskKeExtLen -= (uint8)TLS_ONE;
                LusIndex += (uint16)TLS_ONE;
            }while(LucPskKeExtLen > (uint8)TLS_ZERO);
            
            if (TLS_TRUE == LblPskExFound)
            {
                LddReturnValue = E_OK;
            }
            else
            {
                Tls_GaaAlert[LusActiveConnIdx] = TLS_HANDSHAKE_FAILURE;
                LddReturnValue = E_NOT_OK;
            }
        }
        else
        {
            Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
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
** Function Name        : Tls_ValidateClientHello                          **
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
**  Design ID           : TLS_SDD_0335                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ValidateClientHello(
                                        const uint16 LusActiveConnIdx,
                                        const uint8* const LpHShakeBuf)
{
    Std_ReturnType  LddReturnValue;
    TcpIp_TlsConnectionIdType LddConnectionId;  
    Tls_FrameType*  LpTlsFrame;  
    uint16          LusHShakeLen;
    uint16          LusHBufIdx;
    uint16          LusLengthProcessed; 
    uint8           LucLoopCnt;   


    const Tls_CHValidationFuncPtrType LaaCHValidationFuncPtr[TLS_FIVE] =
    {
        &Tls_ValidateChLegacyVersion,       
        &Tls_ValidateChSessionId,  
        &Tls_ValidateChCipherSuites,   
        &Tls_ValidateChCompressMethd,
        &Tls_ValidateChExtensions
    };

    LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];

    LddConnectionId = 
                Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;				

    /* Validate the client hello handshake header Tag. */
    if ((uint8)TLS_CLIENT_HELLO == (*LpHShakeBuf))
    { 
        LusHBufIdx = (uint16)TLS_TWO;
        LusHShakeLen = Tls_GetExtensionLength(&LpHShakeBuf[LusHBufIdx]);
        
        if ((LusHShakeLen < TLS_MINMUM_SIZE_OF_CH) || 
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
                LddReturnValue = LaaCHValidationFuncPtr[LucLoopCnt]
                                                    (LddConnectionId,
                                                    &LpHShakeBuf[LusHBufIdx],
                                                    &LusLengthProcessed);
            
                /* Increment loop count to point to next element */
                LucLoopCnt++;
            
                LusHBufIdx += LusLengthProcessed;
                /* At the end of the loop, verify whether any errors were 
                encountered during the validation of the Client Hello message */
            } while ((LusHBufIdx < (uint16)(LusHShakeLen - (uint16)TLS_FOUR)) && 
                    (LucLoopCnt < (uint8)TLS_FIVE) && (E_OK == LddReturnValue));

            /* update this here , to use during Key Derive */
            LpTlsFrame->usProcessedHSBytes = LusHBufIdx; 
            
            /* Update total processed length */
            LusLengthProcessed = LusHBufIdx - (uint16)TLS_FOUR; 
            
            /* Verify that all parsed elements were successfully processed as
               per the length specified in the handshake header. */
            if ((E_OK == LddReturnValue) && 
                                        (LusLengthProcessed != LusHShakeLen))
            {
                Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
                LddReturnValue = E_NOT_OK;
            }                             
        }
    }
    else
    {
        Tls_GaaAlert[LusActiveConnIdx] = TLS_UNEXPECTED_MESSAGE;
        LddReturnValue = E_NOT_OK;    
    } 
    
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
** Function Name        : Tls_ValidateChLegacyVersion                          **
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
**  Design ID           : TLS_SDD_0239                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChLegacyVersion(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            uint16* const LpLengthProcessed)
{
    Std_ReturnType  LddReturnValue;
    uint16 LusVersion;
    uint16 LusActiveConnIdx;
    
    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);
    
    /* Get the CH version */
    LusVersion = Tls_GetExtensionLength(LpCHShakeBuf);

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
        /* Leagcy version ( 2 bytes) + 32 bytes random num bytes */
        *LpLengthProcessed = (uint16)TLS_THIRTYFOUR;
    }

    return LddReturnValue; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_ValidateChSessionId                          **
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
**  Design ID           : TLS_SDD_0240                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChSessionId(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            uint16* const LpLengthProcessed)
{
    Std_ReturnType  LddReturnValue;
    uint16 LusActiveConnIdx;

    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);
    
    if ((uint8)TLS_ZERO != *LpCHShakeBuf)
    {
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
** Function Name        : Tls_ValidateChCipherSuites                          **
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
**  Design ID           : TLS_SDD_0242                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_ValidateChCipherSuites(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            uint16* const LpLengthProcessed)
{
    Std_ReturnType  LddReturnValue;
    Tls_FrameType*  LpTlsFrame;
    uint8* LpSHShakeBuf;
    uint16 LusChCiprLen;
    uint16 LusSHCiphr;
    uint16 LusCHCiphr;
    uint16 LusNumOfCipherWrkrs;
    uint16 LusCiphrWrkrCtr;
    uint16 LusCiphrWrkrStartIdx;
    uint16 LusCiphrWrkrLookUpIdx;
    uint16 LusActiveConnIdx;
    uint16 LusSHIdx;
    uint16 LusIndex;
 
    LusIndex = (uint16)TLS_ZERO;
    LddReturnValue = E_NOT_OK;

    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);

    LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];
    
    /* Get the cipher suite size*/
    LusChCiprLen = Tls_GetExtensionLength(LpCHShakeBuf);
    

    *LpLengthProcessed = (uint16)TLS_TWO + LusChCiprLen;

    if ((LusChCiprLen >= (uint16)TLS_TWO) && 
        ((LusChCiprLen & ((uint16)TLS_ONE)) == ((uint16)TLS_ZERO)))
    {
        LusCiphrWrkrStartIdx = 
                        Tls_GaaConnection[LddConnectionId].usCiphrWrkrConnIdx;
        LusNumOfCipherWrkrs = 
                        Tls_GaaConnection[LddConnectionId].usNumOfCiphrWrkrs;
        
        do
        {
            LusIndex += (uint16)TLS_TWO;
            LusCHCiphr = Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]);
            
            for (LusCiphrWrkrCtr = LusNumOfCipherWrkrs; 
                                            LusCiphrWrkrCtr > (uint16)TLS_ZERO;
                                                            LusCiphrWrkrCtr--)
            { 
                LusCiphrWrkrLookUpIdx = 
                Tls_GaaCiphrWrkrLookUp[LusCiphrWrkrStartIdx + LusCiphrWrkrCtr -
                                                            (uint16)TLS_ONE];
   
                LusSHCiphr = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
                                LusCiphrWrkrLookUpIdx].ulTlsCiphrSutDefRefIdx
                                                        ].usTlsCiphersuiteId;
         
                if (LusSHCiphr == LusCHCiphr)
                {
                    LusSHIdx = LpTlsFrame->usHShakeBufferPosition;
                    
                    LpTlsFrame->usNegotiatedCiprSutIdx = LusCiphrWrkrLookUpIdx;
                    
                    LpSHShakeBuf = 
                            &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusSHIdx];
                    
                    /* Concvert and copy */
                    LusCHCiphr = Tls_Htons(LusCHCiphr);
                    
                  
                    Tls_MemCpy(&LpSHShakeBuf[TLS_THIRTYNINE], &LusCHCiphr,
                                                            (uint16)TLS_TWO);  
                                                           
                    LddReturnValue = E_OK;
                    break;
                }     
            }
            LusChCiprLen -= (uint8)TLS_TWO;

        }while((LusChCiprLen > (uint16)TLS_ZERO) && 
                                                (E_NOT_OK == LddReturnValue));
        
        /* if no cipher matched then insufficient_security alert */
        if (E_NOT_OK == LddReturnValue)
        {
            /* If the server is unable to negotiate a supported set of parameters
            (i.e., there is no overlap between the client and server parameters),
            it MUST abort the handshake with either a "handshake_failure" or
            "insufficient_security" fatal alert */
            
            Tls_GaaAlert[LusActiveConnIdx] = TLS_HANDSHAKE_FAILURE;
        }
    }
    else
    {  
        Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;             
    }

    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_ValidateChCompressMethd                      **
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
**  Design ID           : TLS_SDD_0241                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChCompressMethd(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            uint16* const LpLengthProcessed)
{
 
    Std_ReturnType  LddReturnValue;
    uint16 LusActiveConnIdx;
    
    
    if ((*LpCHShakeBuf == (uint8)TLS_ONE) && 
                                    (LpCHShakeBuf[TLS_ONE] == (uint8)TLS_ZERO))
    {  
        LddReturnValue = E_OK;
        *LpLengthProcessed = (uint16)TLS_TWO;
    }
    else
    {

        /* legacy_compression_methods:  Versions of TLS before 1.3 supported
        compression with the list of supported compression methods being
        sent in this field.  For every TLS 1.3 ClientHello, this vector
        MUST contain exactly one byte, set to zero, which corresponds to
        the "null" compression method in prior versions of TLS.  If a
        TLS 1.3 ClientHello is received with any other value in this
        field, the server MUST abort the handshake with an
        "illegal_parameter" alert. */     
        
        LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);
        
        Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
        LddReturnValue = E_NOT_OK;
    }
    
    return LddReturnValue;  
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"
static const Tls_ExtHandler Tls_ExtTable[TLS_SIX] =
{
    {
        TLS_SUPPORTED_VERSIONS,
        &Tls_ValidateCHSupportedVersExt,
        0,
        TLS_SUPPORTED_VERSIONS_BIT
    },
    {
        TLS_CH_PSK_KEY_EXCHANGE_MODES,
        &Tls_ValidatePskKeyExModesExt,
        0,
        TLS_PSK_KEY_EXCHANGE_MODES_BIT
    },   
    {
        TLS_PRE_SHARED_KEY,
        &Tls_ValidateChPskExt,
        TLS_PSK_KEY_EXCHANGE_MODES_BIT,
        TLS_PRE_SHARED_KEY_BIT
    },
    {
        TLS_CH_SUPPORTED_GROUPS,
        &Tls_ValidateChSuprtdGrp,
        0,
        TLS_CH_SUPPORTED_GROUPS_BIT
    },  
    {
        TLS_CH_KEY_SHARE,
        &Tls_ValidateChKShare,
        TLS_CH_SUPPORTED_GROUPS_BIT,
        TLS_CH_KEY_SHARE_BIT
    },  
    {
        TLS_CH_SIGNATURE_ALGORITHMS,
        &Tls_ValidateChSign,
        0,
        TLS_SIGNATURE_ALGORITHMS_BIT
    }
};
#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_ValidateChExtensions                             **
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
**  Design ID           : TLS_SDD_0243                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChExtensions(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            uint16* const LpLengthProcessed)
{
    /* code which i saw in wolfssl , table driver thing 
    1)Parse
    2)store
    3) look for dependecies 
    4) validate at the end 
    i think with our old switch case, this was getting complex , so shifted to 
    look up tabel kind of */
    Std_ReturnType  LddReturnValue;
    Tls_ParsedExt LddParsedExtns[TLS_SIX];
    uint16 LusTemp;
    uint16 LusActiveConnIdx;
    uint8 LucParsedCount;
    uint8 LucSeenMask;
    uint8 LucLoopCnt;

    const Tls_CHExtHandlrFnPtrType LaaCHExtvaliFnPtr[TLS_THREE] =
    {
        &Tls_ParseCHExtensions,       
        &Tls_CHExtCheckDeps,  
        &Tls_CHExtValidations
    };

    /* get the total extension length */
    LucLoopCnt = (uint8)TLS_ZERO;
    LucParsedCount = (uint8)TLS_ZERO;
    LucSeenMask = (uint8)TLS_ZERO;
    LusTemp = Tls_GetExtensionLength(LpCHShakeBuf);
    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId); 

    if (LusTemp > (uint16)TLS_EIGHT)
    {
        
        do
        {
            LddReturnValue = LaaCHExtvaliFnPtr[LucLoopCnt]
                                                (LddConnectionId,
                                                LpCHShakeBuf,
                                                &LddParsedExtns[TLS_ZERO],
                                                &LucParsedCount,
                                                &LucSeenMask);

            LucLoopCnt++;
        }while ((LucLoopCnt < (uint8)TLS_THREE) && (E_OK == LddReturnValue));
        
        if (E_OK == LddReturnValue)
        {
            *LpLengthProcessed = LusTemp + (uint16)TLS_TWO;
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
** Function Name        : Tls_ParseCHExtensions                             **
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
**  Design ID           : TLS_SDD_0354                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ParseCHExtensions(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            Tls_ParsedExt* const LpParsedExtns,
                            uint8* const LpParsedCount,
                            uint8* const LpSeenMask)
{
    Std_ReturnType  LddReturnValue;
    uint16 LusTag;
    const Tls_ExtHandler* LpExtHandler;
    uint16 LusTotalExtLen;
    uint16 LusIndex;
    uint16 LusSummedExtLen;
    uint16 LusTemp;
    uint16 LusActiveConnIdx;
    uint8 LucExtCtr;
    boolean LblFound;
    
    /* i am just parsing here , without validating  */
    LblFound = TLS_FALSE;
	LpExtHandler = NULL_PTR;
    //LusSkippedExtLen = (uint16)TLS_ZERO;
    LusSummedExtLen = (uint16)TLS_ZERO;
    LusTotalExtLen = Tls_GetExtensionLength(LpCHShakeBuf);
    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId); 

    /* Move to First Tag len value extension */
    LusIndex = (uint16)TLS_TWO;
    
    do
    {            
        LddReturnValue = 
            Tls_CheckSupportedTag(TLS_CLIENT_HELLO, &LpCHShakeBuf[LusIndex]);  
    
        if (E_OK == LddReturnValue)
        {   
            LusTag = 
                ((uint16)((uint16)LpCHShakeBuf[LusIndex] << (uint16)TLS_EIGHT) | 
                            (uint16)LpCHShakeBuf[LusIndex + (uint16)TLS_ONE]); 
                            
            /* this could be made into a new function in future may be
            like FindExtHandler Table kind of */        
            for (LucExtCtr = (uint8)TLS_ZERO; LucExtCtr < (uint8)TLS_SIX;
                                                                LucExtCtr++)
            {
                /* for now since less extension in Qorix tls
                , will focus on liner search */
                if (LusTag == (uint16)(Tls_ExtTable[LucExtCtr].ddExtType))
                {
                    LpExtHandler = &Tls_ExtTable[LucExtCtr];
                    LblFound = TLS_TRUE;
                    break;
                }
            }
            
            /* why this here : get the len of the extension, even if its 
            suported or not by us */
            LusTemp = Tls_GetExtensionLength(
                                &LpCHShakeBuf[LusIndex + (uint16)TLS_TWO]); 
                                
            /* to check if an valid extension is recived , but not currently
            supported by Qorix TLS */
            if (TLS_TRUE == LblFound)
            {
                /* reset it back for the next extension which will loop again */
                LblFound = TLS_FALSE;
                
                /* start storing the captured extension details locally
                since this itself was a local variable passed by addr */
                
                LpParsedExtns[*LpParsedCount] = 
                                    (Tls_ParsedExt){LpExtHandler,
                                                    /* starts from tag  */
                                                    &LpCHShakeBuf[LusIndex]};
                
                (*LpParsedCount)++;
                
                /* since each Extension has been a given a dedicated bit slot
                so this checks what all extension is alreay parsed by us */                     
                (*LpSeenMask) = (*LpSeenMask) | (LpExtHandler->ucExtbit);
            }            
            
            /* even if it is supported or not supported , we have move to 
            next Tag */          
            LusIndex += (uint16)(TLS_FOUR + LusTemp);
    
            /* Keep Tracking it, so that All extension summed up should be 
            equal to Total extension size */
            LusSummedExtLen += (uint16)(TLS_FOUR + LusTemp);
            
            /* here two scenarios can happen,
            1)PSK ext might not be the last extension, along with that all 
            other extensions , length field is correct
            2)once u found out its a PSK ext , but during that if any of the
            other extension length field was incorrect , which is curently not
            validated during parsing, so adding a check for that */
            /* scenario two */
            if (LusSummedExtLen > LusTotalExtLen)
            {
                Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
                LddReturnValue = E_NOT_OK;     
            }
            else
            {
                /* scenario one */
                if ((uint16)TLS_PRE_SHARED_KEY == LusTag)
                {
                    if ((LusTotalExtLen - LusSummedExtLen) != (uint16)TLS_ZERO)
                    {
                        /* The "pre_shared_key" extension MUST be the last extension in the
                        ClientHello (this facilitates implementation as described below).
                        Servers MUST check that it is the last extension and otherwise fail
                        the handshake with an "illegal_parameter" alert. */
                        
                        Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER; 
                        LddReturnValue = E_NOT_OK;  
                    }
                }
            }
        }
        else
        {
            Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
            LddReturnValue = E_NOT_OK;                 
        }
        
    }while((LusSummedExtLen < LusTotalExtLen) && (E_OK == LddReturnValue));

    return LddReturnValue; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_CHExtCheckDeps                             **
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
**  Design ID           : TLS_SDD_0352                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_CHExtCheckDeps(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            Tls_ParsedExt* const LpParsedExtns, // parasoft-suppress MISRAC2012-RULE_8_13-a-4 CERT_C-DCL13-a-3 "Reason: Tls_Server_c_REF_7 Tls_Server_c_REF_8.
                            uint8* const LpParsedCount, // parasoft-suppress MISRAC2012-RULE_8_13-a-4 CERT_C-DCL13-a-3 "Reason: Tls_Server_c_REF_7 Tls_Server_c_REF_8.
                            uint8* const LpSeenMask) // parasoft-suppress MISRAC2012-RULE_8_13-a-4 CERT_C-DCL13-a-3 "Reason: Tls_Server_c_REF_7 Tls_Server_c_REF_8.
{
    Std_ReturnType  LddReturnValue;
    Tls_AuthType LddAuthType;
    uint16 LusActiveConnIdx;
    uint8 LucExtCtr;

    LddReturnValue = E_OK;
    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);
    
    /* Check Dependencies here only  */
    for (LucExtCtr = (uint8)TLS_ZERO; LucExtCtr < (*LpParsedCount);
                                                            LucExtCtr++)
    {
        if ((uint8)(LpParsedExtns[LucExtCtr].pExtHandler->ucRequiresMask & 
            (*LpSeenMask)) != 
            LpParsedExtns[LucExtCtr].pExtHandler->ucRequiresMask)
        {
            LddReturnValue = E_NOT_OK;
            break;
        }
    }
    
    /* after this we also have to check if madnatory extensions are 
    recived or not, depending on AUth Type */
    /* My internal Look Up tabel , no other way*/						
    /*		            7	6	5	4	 3	  2	   1	    0
                        NA	NA	NA	PSK  KS   SG  PSK_KE   SV
    AUTHPSK	 0x13	=   0	0	0	1	 0	  0	   1	   1
    AUTHCERT 0xD    =   0	0	0	0	 1	  1	   0	   1 */
    
    if (E_OK == LddReturnValue)
    {
        LddAuthType = Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
        
        if (TLS_AUTH_PSK == LddAuthType)
        {
            if (((uint8)TLS_AUTH_PSK_MASK & (*LpSeenMask)) != (uint8)TLS_AUTH_PSK_MASK)
            {
                LddReturnValue = E_NOT_OK;  
            }
        }
        else
        {
            if (((uint8)TLS_AUTH_CERT_MASK & (*LpSeenMask)) != (uint8)TLS_AUTH_CERT_MASK)
            {
                LddReturnValue = E_NOT_OK;  
            }                   
        }
    }
    
    if (E_NOT_OK == LddReturnValue)
    {
        /* Servers receiving a ClientHello which does not conform to these
        requirements MUST abort the handshake with a "missing_extension" alert */
    
        Tls_GaaAlert[LusActiveConnIdx] = TLS_MISSING_EXTENSION; 
    }

    
    TLS_UNUSED(LpCHShakeBuf);
    return LddReturnValue; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_CHExtValidations                             **
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
**  Design ID           : TLS_SDD_0353                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_CHExtValidations(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            Tls_ParsedExt* const LpParsedExtns, // parasoft-suppress MISRAC2012-RULE_8_13-a-4 CERT_C-DCL13-a-3 "Reason: Tls_Server_c_REF_7 Tls_Server_c_REF_8.
                            uint8* const LpParsedCount, // parasoft-suppress MISRAC2012-RULE_8_13-a-4 CERT_C-DCL13-a-3 "Reason: Tls_Server_c_REF_7 Tls_Server_c_REF_8.
                            uint8* const LpSeenMask) // parasoft-suppress MISRAC2012-RULE_8_13-a-4 CERT_C-DCL13-a-3 "Reason: Tls_Server_c_REF_7 Tls_Server_c_REF_8.
{
    Std_ReturnType  LddReturnValue;
    Tls_AuthType LddAuthType;
    uint16 LusActiveConnIdx;
    uint8 LucExtTable;
    uint8 LucParsed;
    uint8 LucShifting;
    
    LddAuthType = Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);
	LddReturnValue = E_OK;

    /* My internal Look Up tabel , no other way*/						
    /*		            7	6	5	4	 3	  2	   1	    0
                        NA	NA	NA	PSK  KS   SG  PSK_KE   SV
    AUTHPSK	 0x13	=   0	0	0	1	 0	  0	   1	   1
    AUTHCERT 0xD    =   0	0	0	0	 1	  1	   0	   1 */
    
    if (TLS_AUTH_PSK == LddAuthType)
    {
        //LucTotalExts = (uint8)TLS_THREE;
        LucShifting = (uint8)TLS_AUTH_PSK_MASK;
        // ask abhijith if this becomes to complex to read, just attach a uint8 array {0,1,4}, instead of bit shifting
    }
    else
    {
        //LucTotalExts = (uint8)TLS_THREE;   // since signature still not implemenetd , so later do it  
        LucShifting = (uint8)TLS_AUTH_CERT_MASK;   
        // ask abhijith if this becomes to complex to read, just attach a uint8 array {0,2,3}, instead of bit shifting        
    }
    
    for (LucExtTable = (uint8)TLS_ZERO; LucExtTable < (uint8)TLS_SIX; 
                                                                LucExtTable++)
    {
        if (((uint8)((uint8)((uint8)TLS_ONE << LucExtTable) & LucShifting)) == 
                                        (uint8)((uint8)TLS_ONE << LucExtTable))
        {
            for (LucParsed = (uint8)TLS_ZERO; LucParsed < (*LpParsedCount); 
                                                                LucParsed++)
            {
                if (LpParsedExtns[LucParsed].pExtHandler == 
                                                (&Tls_ExtTable[LucExtTable]))
                {
                    LddReturnValue = 
                    Tls_ExtTable[LucExtTable].aaValidateFn(LusActiveConnIdx,
                                            LpParsedExtns[LucParsed].pExtData);                    
                }
                
                if (E_OK != LddReturnValue)
                {
                    break;
                }
            }            
        }
        if (E_OK != LddReturnValue)
        {
            break;
        }        
    }
    
    TLS_UNUSED(LpSeenMask);
    TLS_UNUSED(LpCHShakeBuf);
    return LddReturnValue; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_ValidateChSuprtdGrp                          **
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
**  Design ID           : TLS_SDD_0391                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChSuprtdGrp(
                                                const uint16 LusActiveConnIdx,
                                                const uint8* const LpCHShakeBuf)
{
    Std_ReturnType  LddReturnValue;
    uint16 LusSHSupGrp;
    uint16 LusCHSupGrp;
    uint16 LusExtLen;
    uint16 LusIndex;
    uint16 LusTempIdx;
    uint16 LusSecondryLen;
    uint16 LusHSWrkrStartIdx;
    uint16 LusNumOfHSWrkrs;
    uint16 LusHSWrkrCtr;
    uint16 LusHSWrkrLookUpIdx;
    boolean Lblstatus;
    boolean LblSGFound;
    
    
    LusIndex = (uint16)TLS_TWO;
    LblSGFound = TLS_FALSE;
    LddReturnValue = E_NOT_OK;
    
    LusExtLen = Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]);   

    /* why :12 : since there are 5 supported grps from Qorix, 5*2 = 10
    plsu 2 byte secondary length */
    if ((LusExtLen >= (uint16)TLS_FOUR) && (LusExtLen <= (uint16)TLS_MAX_SG_SH_KS_LEN)
        && ((LusExtLen & ((uint16)TLS_ONE)) == ((uint16)TLS_ZERO)))
    {

        LusIndex += (uint16)TLS_TWO;
        /* store this cuz: need the addres to know where is the suport grps 
        during Key share combo validation */
        LusTempIdx = LusIndex;
        LusSecondryLen = Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]); 
        
        if (
            ((LusSecondryLen >= (uint16)TLS_TWO) 
            && 
            (LusSecondryLen <= ((uint16)TLS_MAX_SG_SH_KS_LEN - (uint16)TLS_TWO)))
            &&
            ((LusSecondryLen & (uint16)TLS_ONE) == (uint16)TLS_ZERO)
            &&
            (LusExtLen == (uint16)(LusSecondryLen + (uint16)TLS_TWO))
            )
        {
            LusHSWrkrStartIdx = Tls_GaaConnection[Tls_GddActiveConnManager
                            [LusActiveConnIdx].ddTlsConnectionId].usHSWrkrIdx;
 
            LusNumOfHSWrkrs = Tls_GaaConnection[Tls_GddActiveConnManager[
                        LusActiveConnIdx].ddTlsConnectionId].usNumOfHSWorkers;   
            
            do
            {
                /* move to each suported grp */
                LusIndex += (uint16)TLS_TWO;
                
				LusCHSupGrp = 
                ((uint16)((uint16)LpCHShakeBuf[LusIndex] << (uint16)TLS_EIGHT) | 
                            (uint16)LpCHShakeBuf[LusIndex + (uint16)TLS_ONE]);	
                                                    
                Lblstatus = Tls_CheckSupportedGrpValue(LusCHSupGrp);  
                
                if (Lblstatus)
                {
                    for (LusHSWrkrCtr = (uint16)TLS_ZERO; LusHSWrkrCtr < 
                                                LusNumOfHSWrkrs; LusHSWrkrCtr++)
                    { 
                        LusHSWrkrLookUpIdx = 
                            Tls_GaaHSLookUp[LusHSWrkrStartIdx + LusHSWrkrCtr];
                        
                        LusSHSupGrp = 
                        Tls_GaaHandshake[LusHSWrkrLookUpIdx].usTlsSupportedGrpId;
                        
	    	            if (LusSHSupGrp == LusCHSupGrp)
                        {
                            /* using the eixisting member completlty for a different
                            reason to store the addres where the suported grp starts
                            avoiding creating global variable */
                            Tls_HandshakeFrame[LusActiveConnIdx].pPatnerKeyShare
                            = &LpCHShakeBuf[LusTempIdx];
                            
	    		            LblSGFound = TLS_TRUE;
                            LddReturnValue = E_OK;
                            break;						
						}
                    }                    
                }
                LusSecondryLen -= (uint16)TLS_TWO;
            }while ((LusSecondryLen > (uint16)TLS_ZERO) && (!LblSGFound));

            if (!LblSGFound)
            {
                /* If the server is unable to negotiate a supported set of parameters
                (i.e., there is no overlap between the client and server parameters),
                it MUST abort the handshake with either a "handshake_failure" or
                "insufficient_security" fatal alert */      
                
                Tls_GaaAlert[LusActiveConnIdx] = TLS_HANDSHAKE_FAILURE;
                LddReturnValue = E_NOT_OK;                 
            }
        }
        else
        {
            Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
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
** Function Name        : Tls_ValidateChKShare                          **
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
**  Design ID           : TLS_SDD_0392                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChKShare(
                                            const uint16 LusActiveConnIdx,
                                            const uint8* const LpCHShakeBuf)
{
    /* main point is before coming to key share , some how Suported grp shd be
    validated, or shd have address of the Suuported grp*/
    Std_ReturnType  LddReturnValue;
    uint16 LusExtLen;
    uint16 LusIndex;
    uint16 LusSecondryLen;
    uint16 LusParsdLen;
    uint16 LaaParsdGrpsKShare[TLS_TOTAL_SG_RFC];
    
    LusIndex = (uint16)TLS_TWO;
    LusParsdLen = (uint16)TLS_ZERO;
    
    LusExtLen = Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]);    

    if ((LusExtLen > TLS_ZERO) && (LusExtLen <= (uint16)TLS_MAX_SG_SH_KS_LEN))
    {

        LusIndex += (uint16)TLS_TWO;
        LusSecondryLen = Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]);
	
        if ((LusSecondryLen > TLS_ZERO) && 
        (LusSecondryLen <= ((uint16)((uint16)TLS_MAX_SG_SH_KS_LEN - 
                                                            (uint16)TLS_TWO))))
        {
            /* do the folowing 
            1)create a array of uint8 which has all the grps in keys share  */
            
            LddReturnValue = Tls_ParseDuplicatKShar(
                                                    LusActiveConnIdx,
                                                    &LaaParsdGrpsKShare[TLS_ZERO],
                                                    &LusParsdLen,
                                                    &LpCHShakeBuf[LusIndex]);
                                                    
            if (E_OK == LddReturnValue)
            {
                LddReturnValue = Tls_ValidateSGKSCombo(
                                                LusActiveConnIdx,
                                                &LpCHShakeBuf[LusIndex],
                                                &LaaParsdGrpsKShare[TLS_ZERO],
                                                LusParsdLen);               
            }                                                                                                
        }
        else
        {
            Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
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
** Function Name        : Tls_ParseDuplicatKShar                          **
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
**  Design ID           : TLS_SDD_0393                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ParseDuplicatKShar(
                                            const uint16 LusActiveConnIdx,
                                            uint16* const LpParsdGrpsKShare,
                                            uint16* const LpParsdLen,
                                            const uint8* const LpCHShakeBuf)
{
    Std_ReturnType  LddReturnValue;
    uint16 LusGrpId;
    uint16 LusIndex;
    uint16 LusSecondryLen;
    uint16 LusCount;
    uint16 LusKeyExLen;
    
    LddReturnValue = E_OK;
    LusIndex = (uint16)TLS_ZERO;
    LusSecondryLen = Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]);
    LusIndex += (uint16)TLS_TWO;
    
    do
    {
        /* get the grp Id */
        LusGrpId = Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]);
        
        /* Clients MUST NOT offer multiple KeyShareEntry values
        for the same group.  Clients MUST NOT offer any KeyShareEntry values
        for groups not listed in the client's "supported_groups" extension.
        Servers MAY check for violations of these rules and abort the
        handshake with an "illegal_parameter" alert if one is violated */
        
        for (LusCount = (uint16)TLS_ZERO; LusCount < (*LpParsdLen); LusCount++)
        {
            if (LpParsdGrpsKShare[LusCount] == LusGrpId)
            {
                Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
                LddReturnValue = E_NOT_OK;                 
                break;
            }
        }

        if (E_OK == LddReturnValue)
        {
            LpParsdGrpsKShare[*LpParsdLen] = LusGrpId;
            (*LpParsdLen) += (uint16)TLS_ONE;
            
            
            /* get the Key exchange len field */
            LusIndex += (uint16)TLS_TWO;
            LusKeyExLen = Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]); 
            
            /* move to actual key share */
            LusIndex += (uint16)TLS_TWO; 
            LusIndex += LusKeyExLen;  
            
            if (LusSecondryLen < (uint16)(LusKeyExLen + (uint16)TLS_FOUR))
            {
                Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
                LddReturnValue = E_NOT_OK;            
            }
            else
            {
                LusSecondryLen -= (uint16)(LusKeyExLen + (uint16)TLS_FOUR);
            }            
        }
    }while ((LusSecondryLen > (uint16)TLS_ZERO) && (E_OK == LddReturnValue));

    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_ValidateSGKSCombo                          **
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
**  Design ID           : TLS_SDD_0355                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static boolean Tls_FindBestPubKS(
								const uint16 LusActiveConnIdx,
								const uint8* const LpCHShakeBuf,
								/* KS */
								const uint16* const LpParsdGrpsKShare,
								const uint16 LusParsdLen)
{
    const uint8* LpSupGrpExtPtr;
    uint16 LusSHSupGrp;
    uint16 LusSGGrpId;
    uint16 LusSupGpTotalLen;
    uint16 LusSGExtIdx;
    uint16 LusKSTempIdx;
    uint16 LusIndex;
    uint16 LusKeyExLen;
    uint16 LusExpKShareLen;
    uint16 LusActulGrp;
    uint16 LusSecondryLen;
    
    uint16 LusHSWrkrStartIdx;
    uint16 LusNumOfHSWrkrs;
    uint16 LusHSWrkrLookUpIdx;
    uint16 LusHSWrkrCtr;
    
    boolean Lblstatus;
    boolean LblQorixSuprtd;  
    boolean LblFound;    


    LblFound = TLS_FALSE;
    LpSupGrpExtPtr = Tls_HandshakeFrame[LusActiveConnIdx].pPatnerKeyShare;
                                                
     LusSupGpTotalLen =
        (uint16)(
            (uint16)((uint16)LpSupGrpExtPtr[TLS_ZERO] << (uint16)TLS_EIGHT) |
            (uint16)LpSupGrpExtPtr[TLS_ONE]);
                                                

	/* now choose the highesh pri from CH suported grp and 
	check if its there in Key share ext */
	LusSGExtIdx = (uint16)TLS_TWO;   
	
	do
	{
		Lblstatus = TLS_FALSE;
		
		/* take a grp id from CH SG ext  */
		LusSGGrpId = 
			((uint16)((uint16)LpSupGrpExtPtr[LusSGExtIdx] << (uint16)TLS_EIGHT) | 
						(uint16)LpSupGrpExtPtr[LusSGExtIdx + (uint16)TLS_ONE]);	
		
		/* before checking this in Keyshare , check if grp is supported by qorix */
		LblQorixSuprtd = Tls_CheckSupportedGrpValue(LusSGGrpId); 
		
		if (LblQorixSuprtd)
		{
	
			/* check if this grp is there in Server configuration */
			LusHSWrkrStartIdx = Tls_GaaConnection[Tls_GddActiveConnManager
						[LusActiveConnIdx].ddTlsConnectionId].usHSWrkrIdx;
	
			LusNumOfHSWrkrs = Tls_GaaConnection[Tls_GddActiveConnManager[
					LusActiveConnIdx].ddTlsConnectionId].usNumOfHSWorkers;                
			
			for (LusHSWrkrCtr = (uint16)TLS_ZERO; LusHSWrkrCtr < 
											LusNumOfHSWrkrs; LusHSWrkrCtr++)
			{
				LusHSWrkrLookUpIdx = 
						Tls_GaaHSLookUp[LusHSWrkrStartIdx + LusHSWrkrCtr];
					
				LusSHSupGrp = 
				Tls_GaaHandshake[LusHSWrkrLookUpIdx].usTlsSupportedGrpId; 
	
				if (LusSGGrpId == LusSHSupGrp)
				{
					Lblstatus = TLS_TRUE;
					Tls_HandshakeFrame[LusActiveConnIdx].
								usNegotiatedSupGrpIdx = LusHSWrkrLookUpIdx;                       
					break;
				}                        
			}
			
			
			if (Lblstatus)
			{
				/* resetting */
				Lblstatus = TLS_FALSE;
	
				/* now check if the grp from SG is there in Key share Grp */
				/* also one note :
				at this point of time , we know there is no two same grps in the 
				key share ext */
				/* its ok to not find some particlar grp which is there in SG 
				ext amd not there in Key share ext, but the reverse shd not be the case */
				for (LusKSTempIdx = (uint16)TLS_ZERO; LusKSTempIdx < LusParsdLen; 
																LusKSTempIdx++)
				{
					if (LusSGGrpId == LpParsdGrpsKShare[LusKSTempIdx])
					{
						Lblstatus = TLS_TRUE;
						break;
					}                        
				}
				
				/* now after finding the Grp in Key share, check if its length
				is also correct */      
				if (TLS_TRUE == Lblstatus)
				{        
					/* resetting */
					Lblstatus =  TLS_FALSE;  
					
					LusIndex = (uint16)TLS_ZERO;
					LusSecondryLen = 
							Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]);
					
					/* point to the first grp id */
					LusIndex += (uint16)TLS_TWO;
				
					do
					{
						LusActulGrp = 
						Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]);
						
						/* get the Key exchange len field */
						LusIndex += (uint16)TLS_TWO;
						
						LusKeyExLen = 
						Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]); 
							
						if (LusSGGrpId == LusActulGrp)
						{
							Lblstatus = TLS_TRUE;
							
							Tls_GetKShareExpLen(LusSGGrpId,
														&LusExpKShareLen);
							
							if (LusKeyExLen == LusExpKShareLen)
							{
								/* found the correct Key share */
								LblFound = TLS_TRUE;
								/* due to TASKING , kept it out*/
								/* like how we stored the addres of public key share
								in a pointer in clinet side , same do for server 
								for now storing locally , once all validatioon are sucsfull
								will replace that */
								Tls_HandshakeFrame[LusActiveConnIdx].pPatnerKeyShare = &LpCHShakeBuf[LusIndex];
							}
							else
							{
								/* this here i am optimzing 
								reason : since before coming to this function
								i have validated already that there is no same grps 
								in the key share , so once u found ur SG grp in this key share
								but its length was not expcted , then for sure, there is no 
								reason to go ahead and check agin to find one more , since we have 
								already validated parsing , that there is no two same grps ,
								so just skip the whole Key share in the CH, and choose the next 
								grp in the SG ext and start checking that */
							}    
						}
						/* move to actual key share */
						LusIndex += (uint16)TLS_TWO; 
						LusIndex += LusKeyExLen;
						
						/* thsi will not be in negative vale , since its already 
						validated before coming here */
						LusSecondryLen -= 
								(uint16)(LusKeyExLen + (uint16)TLS_FOUR);
						
						/* if a proper SG was found without proper length , then also 
						come out and search for new grp */
					}while ((LusSecondryLen > (uint16)TLS_ZERO) && (!Lblstatus));
				}
			} 
		}
		
		/* move to next grp in SG ext */
		LusSGExtIdx += (uint16)TLS_TWO;
		LusSupGpTotalLen -= (uint16)TLS_TWO;
		
	}while ((LusSupGpTotalLen > (uint16)TLS_ZERO) && (!LblFound));
    
    return LblFound;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"




/*******************************************************************************
** Function Name        : Tls_ValidateSGKSCombo                          **
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
**  Design ID           : TLS_SDD_0355                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateSGKSCombo(
                                            const uint16 LusActiveConnIdx,
                                            const uint8* const LpCHShakeBuf,
                                            /* KS */
                                            const uint16* const LpParsdGrpsKShare,
                                            const uint16 LusParsdLen)
{
    Std_ReturnType  LddReturnValue;
    const uint8* LpSupGrpExtPtr;
    uint16 LusSGGrpId;
    uint16 LusSupGpTotalLen;
    uint16 LusSGExtIdx;
    uint16 LusKSTempIdx;
    uint16 LusChoosenIdx;
    
    boolean Lblstatus;
    boolean LblFound;    

	LddReturnValue = E_OK;
	LusChoosenIdx = (uint16)TLS_ZERO;
    LpSupGrpExtPtr = Tls_HandshakeFrame[LusActiveConnIdx].pPatnerKeyShare;
                                                
     LusSupGpTotalLen =
        (uint16)(
            (uint16)((uint16)LpSupGrpExtPtr[TLS_ZERO] << (uint16)TLS_EIGHT) |
            (uint16)LpSupGrpExtPtr[TLS_ONE]);
                                                

    /* verify the negative sceiaro 
    supported_groups:    
    [secp256r1, secp384r1 ]
    key_share:
    [ secp256r1, secp384r1, x25519 ]*/

    /* why *2 : since LusParsdLen indicated how many grps are there
    in Kshare, but each grp is 2 bytes */                                      
    if (LusSupGpTotalLen >= ((uint16)(LusParsdLen * (uint16)TLS_TWO)))
    {
		LblFound = Tls_FindBestPubKS(
									LusActiveConnIdx,
									LpCHShakeBuf,
									LpParsdGrpsKShare,
									LusParsdLen);


        /* this will be true only if there was proper Key share found
        with its length and qorix supported, but as per RFC, there as to be done 
        more validations */
        if (LblFound)    
        {                     
            for (LusKSTempIdx = (uint16)TLS_ZERO; LusKSTempIdx < LusParsdLen; 
                                                                    LusKSTempIdx++)
            {
                /* resetting */
                Lblstatus = TLS_FALSE;
                                                            
                LusSupGpTotalLen =
                (uint16)(
                (uint16)((uint16)LpSupGrpExtPtr[TLS_ZERO] << (uint16)TLS_EIGHT) |
                (uint16)LpSupGrpExtPtr[TLS_ONE]);
            
                /* move to the actual grps is suported grps extension */
                LusSGExtIdx = (uint16)TLS_TWO;
                do
                {
                    LusSGGrpId = Tls_GetExtensionLength(&LpSupGrpExtPtr[LusSGExtIdx]);
                    
                    /* check if each grp in key share , shd be there in Suprted grp */
                    if (LusSGGrpId == LpParsdGrpsKShare[LusKSTempIdx])
                    {
                        /* to check for the negative scenario */
                        /* supported_groups:         
                        [secp256r1, secp384r1 ]
                        key_share:
                        [ secp384r1, secp256r1 ] */  
                        if (LusKSTempIdx > (uint16)TLS_ZERO)
                        {
                            if ((uint16)(LusSGExtIdx - (uint16)TLS_TWO) 
                                                                < LusChoosenIdx)
                            {
                                Lblstatus = TLS_FALSE;
                                LusSupGpTotalLen = (uint16)TLS_ZERO;   /* force loop exit */
                            }
                            else
                            {
                                /* why -2 , cuz: LusSGExtIdx i am starting from +2 
                                so to maintain proper counters */
                                LusChoosenIdx = LusSGExtIdx - (uint16)TLS_TWO;
                                Lblstatus = TLS_TRUE;
                                LusSupGpTotalLen = (uint16)TLS_ZERO;   /* force loop exit */
                            }    
                        }
                        else
                        {
                            /* why -2 , cuz: LusSGExtIdx i am starting from +2 
                            so to maintain proper counters */
                            LusChoosenIdx = LusSGExtIdx - (uint16)TLS_TWO;
                            Lblstatus = TLS_TRUE;
                            LusSupGpTotalLen = (uint16)TLS_ZERO;   /* force loop exit */                            
                        }                       
                    }
                    else
                    {
                        /* move to next grp in SG ext */
                        LusSGExtIdx += (uint16)TLS_TWO;
                        LusSupGpTotalLen -= (uint16)TLS_TWO;                      
                    }
                }while ((LusSupGpTotalLen > (uint16)TLS_ZERO) && (!Lblstatus));
                
                /* to check for the negative scenario */
                /* supported_groups:      
                [secp256r1, secp384r1 ]
                key_share:
                [ secp256r1, x25519 ] */
                
                if (Lblstatus == TLS_FALSE)
                {
					Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
                    LddReturnValue = E_NOT_OK;  
                    break;
                }
            }
        }
        else
        {
			Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
            LddReturnValue = E_NOT_OK; 
        }
    }
    else
    {
		Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
        LddReturnValue = E_NOT_OK; 
    }
    
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_ValidateChSign                          **
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
**  Design ID           : TLS_SDD_0394                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChSign(
                                                const uint16 LusActiveConnIdx,
                                                const uint8* const LpCHShakeBuf)
{
    Std_ReturnType  LddReturnValue;
    uint16 LusCHSign;
    uint16 LusSHSign;
    uint16 LusExtLen;
    uint16 LusIndex;
    uint16 LusSecondryLen;
    uint16 LusHSWrkrStartIdx;
    uint16 LusNumOfHSWrkrs;
    uint16 LusHSWrkrCtr;
    uint16 LusHSWrkrLookUpIdx;
    boolean Lblstatus;
    boolean LblSignFound;
    
    
    LusIndex = (uint16)TLS_TWO;
    LblSignFound = TLS_FALSE;
    LddReturnValue = E_NOT_OK;
    
    LusExtLen = Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]);   

    if ((LusExtLen >= (uint16)TLS_FOUR) && (LusExtLen <= (uint16)TLS_MAX_SG_SH_KS_LEN)
        && ((LusExtLen & ((uint16)TLS_ONE)) == ((uint16)TLS_ZERO)))
    {

        LusIndex += (uint16)TLS_TWO;
        LusSecondryLen = Tls_GetExtensionLength(&LpCHShakeBuf[LusIndex]); 
        
        if (
            ((LusSecondryLen >= (uint16)TLS_TWO) 
            && 
            (LusSecondryLen <= ((uint16)TLS_MAX_SG_SH_KS_LEN - (uint16)TLS_TWO)))
            &&
            ((LusSecondryLen & (uint16)TLS_ONE) == (uint16)TLS_ZERO)
            &&
            (LusExtLen == (uint16)(LusSecondryLen + (uint16)TLS_TWO))
            )
        {
            LusHSWrkrStartIdx = Tls_GaaConnection[Tls_GddActiveConnManager
                            [LusActiveConnIdx].ddTlsConnectionId].usHSWrkrIdx;
 
            LusNumOfHSWrkrs = Tls_GaaConnection[Tls_GddActiveConnManager[
                        LusActiveConnIdx].ddTlsConnectionId].usNumOfHSWorkers;   
            
            do
            {
                /* move to each signture */
                LusIndex += (uint16)TLS_TWO;
                
				LusCHSign = 
                ((uint16)((uint16)LpCHShakeBuf[LusIndex] << (uint16)TLS_EIGHT) | 
                            (uint16)LpCHShakeBuf[LusIndex + (uint16)TLS_ONE]); 
							
                                                    
                Lblstatus = Tls_CheckSignatureAlgoValue(LusCHSign);  
                
                if (Lblstatus)
                {
                    for (LusHSWrkrCtr = (uint16)TLS_ZERO; LusHSWrkrCtr < 
                                                LusNumOfHSWrkrs; LusHSWrkrCtr++)
                    { 
                        LusHSWrkrLookUpIdx = 
                            Tls_GaaHSLookUp[LusHSWrkrStartIdx + LusHSWrkrCtr];
                        
                        LusSHSign = 
                        Tls_GaaHandshake[LusHSWrkrLookUpIdx].usTlsSignatureId;
                        
	    	            if (LusSHSign == LusCHSign)
                        {                           
	    		            LblSignFound = TLS_TRUE;
                            LddReturnValue = E_OK;
                            Tls_HandshakeFrame[LusActiveConnIdx].
                                    usNegotiatedSigAlgoIdx = LusHSWrkrLookUpIdx;
                            break;						
						}
                    }                    
                }
                LusSecondryLen -= (uint16)TLS_TWO;
            }while ((LusSecondryLen > (uint16)TLS_ZERO) && (!LblSignFound));

            if (!LblSignFound)
            {
                /* If the server is unable to negotiate a supported set of parameters
                (i.e., there is no overlap between the client and server parameters),
                it MUST abort the handshake with either a "handshake_failure" or
                "insufficient_security" fatal alert */      
                
                Tls_GaaAlert[LusActiveConnIdx] = TLS_HANDSHAKE_FAILURE;
                LddReturnValue = E_NOT_OK;                 
            }
        }
        else
        {
            Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
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
** Function Name        : Tls_ValidateChPskExt                          **
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
**  Design ID           : TLS_SDD_0351                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChPskExt(
                                            const uint16 LusActiveConnIdx,
                                            const uint8* const LpCHShakeBuf)
{
    Std_ReturnType  LddReturnValue;
    
    #if (STD_ON == TLS_PSK_IDENTITY_SUPPORT_ENABLED)
        Tls_FrameType*  LpTlsFrame;
        uint8* LpSHShakeBuf;
        uint32 LulPskCsmKeyId; 
        uint16 LusPskIndex;
        uint16 LusSHIdx;
               
        
        LusPskIndex = (uint16)TLS_ZERO;
                    
        LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];
        
        LddReturnValue = Tls_ValidateClientPskExt(LusActiveConnIdx,
                                                    LpCHShakeBuf,
                                                    &LusPskIndex,
                                                    &LulPskCsmKeyId);         
    
        if (E_OK == LddReturnValue)                                                           
        {                                              
            /* this will point till last of CH msg */               
            LusSHIdx = LpTlsFrame->usHShakeBufferPosition;
            
            LpTlsFrame->ulNegotiatedPskCsmKeyId = 
                                        LulPskCsmKeyId;
            
            LpSHShakeBuf = 
            &Tls_GaaHShakeBuffer[LusActiveConnIdx]
                                            [LusSHIdx];
            
            /* Concvert and copy */
            LusPskIndex = Tls_Htons(LusPskIndex);
            
            /* MOve to Psk data field */
            Tls_MemCpy(&LpSHShakeBuf[TLS_FIFTYFOUR], &LusPskIndex, 
                                                            (uint16)TLS_TWO);   
        }
    #else
        LddReturnValue = E_OK;
        TLS_UNUSED(LusActiveConnIdx);
        TLS_UNUSED(LpCHShakeBuf);
    #endif
                                     
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/* parasoft-end-suppress CERT_C-API00-a-3 */
/* parasoft-end-suppress CERT_C-DCL19-a-3 */
/* parasoft-end-suppress MISRAC2012-RULE_20_1-a-4 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-d-2 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 */
