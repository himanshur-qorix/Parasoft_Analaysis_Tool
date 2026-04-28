/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_PreSharedKey.c                                             **
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
**  Design ID  : TLS_SDD_0310                                                 **
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
* #section Tls_PreSharedKey_c_REF_1
* Violates MISRAC2012-RULE_5_4-c-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_PreSharedKey_c_REF_2
* Violates MISRAC2012-RULE_5_4-d-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_PreSharedKey_c_REF_3
* Violates MISRAC2012-RULE_20_1-a-4: Do not precede the #include directive with
* executable code
* Justification: Macros are defined by AUTOSAR standards and internal
* specifications.
*
* #section Tls_PreSharedKey_c_REF_4
* Violates CERT_C-DCL19-a-3: Variable "" should be declared as locally as
* possible
* Justification: Coding guidelines suggest the variables are declared on the
* start of the function.
*
* #section Tls_PreSharedKey_c_REF_5
* Violates CERT_C-API00-a-3: Parameter '' is not validated before use.
* Justification: This parameter either originates from the configuration or is
* validated in the main API before being passed as an argument to the local API.
* Hence, additional validation within the local API is not required and has been
* manually verified.

*/

/* parasoft-begin-suppress MISRAC2012-RULE_5_4-c-2 
"Reason: Tls_PreSharedKey_c_REF_1" */
/* parasoft-begin-suppress MISRAC2012-RULE_5_4-d-2 
"Reason: Tls_PreSharedKey_c_REF_2" */
/* parasoft-begin-suppress MISRAC2012-RULE_20_1-a-4 
"Reason: Tls_PreSharedKey_c_REF_3" */
/* parasoft-begin-suppress CERT_C-DCL19-a-3 
"Reason: Tls_PreSharedKey_c_REF_4" */
/* parasoft-begin-suppress CERT_C-API00-a-3 
"Reason: Tls_PreSharedKey_c_REF_5" */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

#include "Tls_InternalTypes.h"
#include "Tls_Internal.h"
#include "Tls_Ram.h"
#include "Tls_Inline.h"
#include "Tls_External.h"
#if (STD_ON == TLS_PSK_IDENTITY_SUPPORT_ENABLED)


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
static uint32 GetLenOfPskIdsInCh(const uint8* const LpCHPtr);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static uint32 GetCHLenToBeHMaced(const uint8* const LpClientHelloMsgPtr);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetServerPreSharedKey(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8  LucPskCounter,
                            uint8* const LpIdentityHintPtr,
                            uint16* const LpLenOfServerPsk);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChShBinder(
                                        const uint8* const LpClientBinder,
                                        const uint16  LusLenOfClientBindr,
                                        const uint8* const LpServerBinder,
                                        const uint16  LusLenOfSrvrBindr);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetServerBinderValue(
                            const uint8  LucPskCounter,
                            const uint8* const LpClientHelloMsgPtr,
                            const uint32  LulCsmKeyId,
                            uint8* const LpServerBinder,
                            uint32* const LpHmacedOutLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetCsmKeyForServerPsk(   
                                const TcpIp_TlsConnectionIdType  LddConnectionId,
                                const uint8  LucPskCounter,
                                const uint16  LusLenOfServerPsk,
                                const uint8* const LpIdentityPtr,
                                uint32* const LpCsmKeyId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_GetCHMatchngPskBindrVal(
                                    const uint8* const LpClientHelloMsgPtr,
                                    const uint32 LulLenTillPskExt,
                                    const uint16 LusPskIndex,
                                    uint8* const LpClientBinder,
                                    uint16* const LpLenOfClientBinder);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_IsServrPSKPresentInCH(
                                    const uint8* const LpClientHelloMsgPtr,
                                    const uint32 LulLenTillPskExt,
                                    const uint8* const LpIdentityHintPtr,
                                    const uint16 LusLenOfServerPsk,
                                    uint16* const LpPskIndex);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetServerDynPSK(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8 LucPskCounter,
                            uint8* const LpIdentityHintPtr,
                            uint16* const LpLenOfPskIdntyHint);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_GetServerStaticPSK(const uint8 LucPskCounter,
                                            uint8* const LpIdentityHintPtr);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetInputPskKeys(
                                const TcpIp_TlsConnectionIdType LddConnectionId,
                                const uint8 LucPskIndex,
                                uint32* const LpPskCsmKeyId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_CalculateBinders(
                            uint8*  LpExtensionData,
                            const uint8* const LpPartialCHHash,
                            const uint32 LulTrgBinderKey,
                            const uint32 LulInputPskCsmKeyId,
                            const Tls_PSKHashType LddPSKHashType);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_GetAllClientBindrVal(
                                const TcpIp_TlsConnectionIdType LddConnectionId,
                                uint8* LpExtensionData,
                                const uint8* const LpCHBuff,
                                const uint32  LullLenToBeHMaced);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetClientDynPSK(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8  LucPskIdx,
                            uint8*  LpExtensionData,
                            uint8* const LpLenOfPskData);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_GetClientStaticPSK(
                                const uint8  LucPskCounter,
                                uint8*  LpExtensionData);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/


/*******************************************************************************
** Function Name        : Tls_GetChPSKExt                  **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Prepares and formats the PSK,s referred to a connection **
**                        and calls required internal functions**
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
**  Design ID           : TLS_SDD_0263                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_GetChPSKExt(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* LpHShakeBuf,
                            uint16* const LpExtLen)
{
    Std_ReturnType  LddReturnValue;
    uint8* LpHShakeHdrPtr;
    uint32 LulLenToBeHMaced;
    uint32 LulHShakeLen;
    uint16  LusLenOfDynIdntsData;
    uint16  LusLenOfstatIdntsData;
    uint16  LusNumOfCipherWrkrs;
    uint16  LusTotlBindrLen;
    uint16 LusTemp;
    uint16 LusIndex;
    uint16 LusActiveConnIdx;
    uint16 LusTotlIDx;
    uint16 LusCiprSuitSize;
    uint8  LucPskCounter;
    uint8  LucNumOfPsks;
    uint8  LucPskStartIdx;
    uint8  LucPskLookUpIndex;
    uint8  LucLenOfPskData;

        
    LusLenOfDynIdntsData = (uint16)TLS_ZERO;
    LusLenOfstatIdntsData = (uint16)TLS_ZERO;
    LucPskCounter = (uint8)TLS_ZERO;
    LusTotlBindrLen = (uint16)TLS_ZERO;
	LddReturnValue = E_NOT_OK;
    
    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);
    LucPskStartIdx = Tls_GaaConnection[LddConnectionId].ucPSKConnLookUpIdx;    
    LucNumOfPsks = Tls_GaaConnection[LddConnectionId].ucNumOfPreSharedKeys;
    LusNumOfCipherWrkrs = Tls_GaaConnection[LddConnectionId].usNumOfCiphrWrkrs;
    /* Record_changed Update */
    LpHShakeHdrPtr = &Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO];

    ///////////////////////////////////
    /* going back so that i can get the total extension field  location*/
    LusTotlIDx = (uint16)TLS_THIRTYNINE;
    
    /* take the 40th and 41th byte */
    LusCiprSuitSize = 
                ((uint16)((uint16)LpHShakeHdrPtr[LusTotlIDx] << (uint16)TLS_EIGHT) | 
                            (uint16)LpHShakeHdrPtr[LusTotlIDx + (uint16)TLS_ONE]);
    
    LusTotlIDx += (uint16)((uint16)TLS_FOUR + LusCiprSuitSize);   
    
    ///////////////////////////////////
        
    LulLenToBeHMaced = ((uint32)TLS_FOURTYFIVE +
                        (((uint32)TLS_TWO) * ((uint32)LusNumOfCipherWrkrs)) +
                        (uint32)(*LpExtLen)); 
                        
    /* Tag */
    LusTemp = Tls_Htons((uint16)TLS_PRE_SHARED_KEY);
    Tls_MemCpy(LpHShakeBuf, &LusTemp, (uint16)TLS_TWO);

                                                    
    LusIndex = (uint16)TLS_SIX;
 
    do
    {
        LucPskLookUpIndex = Tls_GaaPSKLookUp[LucPskStartIdx + LucPskCounter];
        
        /* Check if the PSk conatiner is Static or not*/
        if (TLS_TRUE == Tls_GaaPskIdentity[LucPskLookUpIndex].blIsDynPskAvail)
        {
            LddReturnValue = Tls_GetClientDynPSK(
                                                LddConnectionId,
                                                LucPskLookUpIndex,
                                                &LpHShakeBuf[LusIndex],
                                                &LucLenOfPskData);
   
            if (E_OK == LddReturnValue)
            {
             
                LucLenOfPskData += (uint8)TLS_SIX;
                
                LusLenOfDynIdntsData += (uint16)LucLenOfPskData;
            }
            else
            {
                /* if Fetching dynamic psk wont work , then try to fetch 
                static PSK */
                /* SWS_TcpIp_00325 */
                if (TLS_INVALID_SHORT != 
                    Tls_GaaPskIdentity[LucPskLookUpIndex].usTlsPskIdentityLen)
                {
                    Tls_GetClientStaticPSK(LucPskLookUpIndex, 
                                                        &LpHShakeBuf[LusIndex]);
 
                    LucLenOfPskData = (uint8)(Tls_GaaPskIdentity[
                                    LucPskLookUpIndex].usTlsPskIdentityLen &
                                    (uint16)TLS_LSB_MASK) + (uint8)TLS_SIX;
      
                    LusLenOfstatIdntsData += (uint16)LucLenOfPskData;
                }
                /* if there is no static also configured , then skip the psk */    
                else
                {
                    LucLenOfPskData = (uint8)TLS_ZERO;     
                }    
            }
        }
        else
        {
            Tls_GetClientStaticPSK(LucPskLookUpIndex, &LpHShakeBuf[LusIndex]);
            
            LucLenOfPskData = (uint8)(Tls_GaaPskIdentity[
                                LucPskLookUpIndex].usTlsPskIdentityLen &
                                (uint16)TLS_LSB_MASK) + (uint8)TLS_SIX;
                
            LusLenOfstatIdntsData += (uint16)LucLenOfPskData;
        }
        
        /* when this scenario occurs ?
        if there is dynamic PSK availabel , but when called , its return value
        was ENOTOK and there is no static configured , in that skip doing calculations */
        if (LucLenOfPskData != (uint8)TLS_ZERO)
        {
            /* precalculate All the binder length */
            /* why 49 ? : 48 + 1 (binder len filed )
            why 33 ? : 32 + 1 (binder len filed) */

            if (TLS_SHA_384 == 
                        Tls_GaaPskIdentity[LucPskLookUpIndex].ddTlsPSKHashType)
            {
                LusTotlBindrLen += (uint16)TLS_FOURTYNINE;
            }
            else
            {
                LusTotlBindrLen += (uint16)TLS_THIRTYTHREE;
            }    
        
            LusIndex += (uint16)LucLenOfPskData;           
        }

        LucPskCounter++;
    }while (LucPskCounter < LucNumOfPsks);
 
    /* why checking this condition ?
    assumen no PSK was able to arranged in the frame, only in that case
    this corner case will come to picture */
    if (LusTotlBindrLen > (uint16)TLS_ZERO)
    {
        /* saved extesnion point */
        LusTemp = Tls_Htons(LusLenOfstatIdntsData + LusLenOfDynIdntsData);
        Tls_MemCpy(&LpHShakeBuf[TLS_FOUR], &LusTemp, (uint16)TLS_TWO);
        
        
        (*LpExtLen) += (uint16)(
                        /* 2 bytes of psk_key_exchange_modes(41) +
                        2 bytes of Size of Psk extension Data field +
                        2 bytes field of ( Length of all identities data) +
                        2 bytes field of (len of all binders data field) */        
                        (uint32)TLS_EIGHT +
                        /*len of static + dyn data */
                        (uint32)LusLenOfstatIdntsData +
                        (uint32)LusLenOfDynIdntsData +
                        /*precalculated len of all binders  */ 
                        (uint32)LusTotlBindrLen);
        
        /* update the precaluclted length , since hash needs to done before 
        hand */ 
        LusTemp = Tls_Htons(*LpExtLen);
        Tls_MemCpy(&LpHShakeHdrPtr[LusTotlIDx], &LusTemp, (uint16)TLS_TWO);
  
        /* update the precaluclted length , since hash needs to done before 
        hand */ 
        /* Length */     
        LusTemp = Tls_Htons((uint16)(
                            (uint32)TLS_TWO + 
                            /*len of static + dyn data */
                            (uint32)LusLenOfstatIdntsData + 
                            (uint32)LusLenOfDynIdntsData +
                            /* 2 bytes field of (len of all binders data field) */
                            (uint32)TLS_TWO +
                            /*precalculated len of all binders  */
                            (uint32)LusTotlBindrLen
                            ));

        Tls_MemCpy(&LpHShakeBuf[TLS_TWO], &LusTemp, (uint16)TLS_TWO);
  
        /* Tag : 2 bytes
        Len 2 bytes 
        value : static + dync identites + 2 bytes identites field */
        LulLenToBeHMaced += ((uint32)(uint16)TLS_SIX +
                                            (uint32)LusLenOfstatIdntsData +
                                            (uint32)LusLenOfDynIdntsData);
                                                            
        LulHShakeLen = 
                    ((uint32)TLS_FOURTYONE +
                    (uint32)(((uint32)TLS_TWO) * (uint32)LusNumOfCipherWrkrs) +
                    (uint32)(*LpExtLen));                    

        /* update for handshake header */ 
        *LpHShakeHdrPtr = (uint8)TLS_CLIENT_HELLO;
        
                                
        LpHShakeHdrPtr[TLS_ONE] =
        (uint8)((LulHShakeLen >> (uint32)TLS_SIXTEEN) & (uint32)TLS_LSB_MASK);
        
        LpHShakeHdrPtr[TLS_TWO] = 
        (uint8)((LulHShakeLen >> (uint32)TLS_EIGHT) & (uint32)TLS_LSB_MASK);
        
        LpHShakeHdrPtr[TLS_THREE] = 
                                (uint8)(LulHShakeLen & (uint32)TLS_LSB_MASK); 
        
        /* Get all the binder values */
        LddReturnValue = Tls_GetAllClientBindrVal(LddConnectionId,
                                                &LpHShakeBuf[LusIndex],
                                                LpHShakeHdrPtr,
                                                LulLenToBeHMaced);                                            
    }   
 
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_GetClientStaticPSK                     **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Prepares the referred PSK,s to connection in the buffer **
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
**  Design ID           : TLS_SDD_0264                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_GetClientStaticPSK(const uint8 LucPskCounter,
                                                    uint8* LpExtensionData)
{
    const uint8*  LpPskIdentity;
    uint16  LusPskIdentityLen;
    uint16  LusTemp;
    uint16  LusIndex; 
    /* Get the Length of the Static PSK Identity */
    LusPskIdentityLen = Tls_GaaPskIdentity[LucPskCounter].usTlsPskIdentityLen;
        
    LusTemp = Tls_Htons(LusPskIdentityLen);
 
    Tls_MemCpy(LpExtensionData, &LusTemp, (uint16)TLS_TWO);
 
    /* Move to (Psk identity string encoded in ASCII) */
    LusIndex = (uint16)TLS_TWO;
    
    /* get the addrees of the PSK indentity in a local pointer */
    LpPskIdentity = &Tls_GaaPskIdentity[LucPskCounter].aaPskIdentity[TLS_ZERO];
    
    Tls_ConvertTextToHex(LpPskIdentity, &LpExtensionData[LusIndex],
                                                            LusPskIdentityLen);
                                                            
    LusIndex += LusPskIdentityLen;
    
    Tls_MemSet(&LpExtensionData[LusIndex], (uint8)TLS_ZERO, (uint16)TLS_FOUR);
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_GetClientDynPSK                    **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Prepares the referred PSK,s to connection in the buffer **
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
**  Design ID           : TLS_SDD_0265                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetClientDynPSK(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8  LucPskIdx,
                            uint8*  LpExtensionData,
                            uint8* const LpLenOfPskData)
{
    uint32  LulCsmKeyId;
    const uint8*  LpPskIdentyHintPtr;
    Std_ReturnType  LddReturnValue;
    uint16  LusPskIdentyHintLen;
    uint16  LusPskIdentyLenPtr;
    uint16  LusTemp;
    uint16  LusIndex;
    uint8  LaaPskIdentyarr[TLS_MAX_LEN_OF_PSK_IDEN];
    
    LddReturnValue =  E_NOT_OK;
    LusPskIdentyHintLen = (uint16)TLS_ZERO;
    LpPskIdentyHintPtr = NULL_PTR;
    LusPskIdentyLenPtr = (uint16)TLS_ZERO;
 

    if ((Tls_GaaPskIdentity[LucPskIdx].TlsPskGetClientKeyIdentityFunc)
                                                                != NULL_PTR)
    {
        /* Get the return value of the dynamic psk */
        LddReturnValue = 
        Tls_GaaPskIdentity[LucPskIdx].TlsPskGetClientKeyIdentityFunc(
                                                LddConnectionId,
                                                /* no use for current TLS */
                                                LusPskIdentyHintLen, 
                                                /* no use for current TLS */
                                                LpPskIdentyHintPtr, 
                                                &LusPskIdentyLenPtr,
                                                /* values will be there here */
                                                &LaaPskIdentyarr[TLS_ZERO],
                                                &LulCsmKeyId);  
    }
 
    if (E_OK == LddReturnValue)
    {
        
        if ((LusPskIdentyLenPtr > (uint16)TLS_THIRTYTWO) || (LusPskIdentyLenPtr < (uint16)TLS_EIGHT))
        {
            LddReturnValue = E_NOT_OK;
        }
        else
        {
            /*since the len of psk id is limited to 32 chars, so acceesing the byte*/
            *LpLenOfPskData = (uint8)(LusPskIdentyLenPtr & (uint16)TLS_LSB_MASK);
            
        	/*  size of field(Length of first identity string) */
        	LusTemp = Tls_Htons(LusPskIdentyLenPtr);
        
            Tls_MemCpy(LpExtensionData, &LusTemp, (uint16)TLS_TWO);
        
        	/* Move to (Psk identity string encoded in ASCII) */
        	LusIndex = (uint16)TLS_TWO;        
 
            Tls_ConvertTextToHex(&LaaPskIdentyarr[TLS_ZERO], 
                                                    &LpExtensionData[LusIndex], 
                                                    LusPskIdentyLenPtr);
 
            LusIndex += LusPskIdentyLenPtr;
  
            Tls_MemSet(&LpExtensionData[LusIndex], (uint8)TLS_ZERO, 
                                                            (uint16)TLS_FOUR);           
        } 
    }  
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_GetAllClientBindrVal                  **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Prepares the referred PSK,s Binder Values **
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
**  Design ID           : TLS_SDD_0266                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetAllClientBindrVal(
                            const TcpIp_TlsConnectionIdType LddConnectionId,
                            uint8* LpExtensionData,
                            const uint8* const LpCHBuff,
                            const uint32  LullLenToBeHMaced)
{
    Std_ReturnType  LddReturnValue;
    Tls_PSKHashType LddPSKHashType;
    uint32 LulInputPskCsmKeyId;
    uint32 LulTrgBinderKey;
    uint32 LulHashingJobId;
    uint32 LulPartialCHHashLen;
    uint16 LusLenOfDynIdntsData;
    uint16 LusIndex;
    uint16 LusBinderLen;
    uint8 LucPskCounter;
    uint8 LucNumOfPsks;
    uint8 LucPskStartIdx;
    uint8 LucPskLookUpIndex;
    boolean LblBreak;
    
    uint8 LaaPartialCHHash[TLS_THIRTYTWO] = {0};
    LulPartialCHHashLen = (uint32)TLS_THIRTYTWO;
    LblBreak = TLS_FALSE;
    LusLenOfDynIdntsData = (uint16)TLS_ZERO;
    
    LucPskStartIdx =  Tls_GaaConnection[LddConnectionId].ucPSKConnLookUpIdx;
    
    LucNumOfPsks = Tls_GaaConnection[LddConnectionId].ucNumOfPreSharedKeys;
    
    LulHashingJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[TLS_ZERO].
    usTlsConnHandshakeRefBaseIndx].ulTlsCsmHashVerifyJobRefIdx;
            
    LusIndex = (uint16)TLS_TWO;
    
    /* Prepare the Hash Transciprt only once, and use that only every time */
    LddReturnValue = Tls_PerformHash(                                   
    /* JobId to perform Hash */
                                    LulHashingJobId,
    /* Contains the pointer to the data for which the hash shall be computed */                                    
                                    LpCHBuff,
    /* Contains the number of bytes to be hashed */                                 
                                    LullLenToBeHMaced,
    /* Contains the pointer to the data where the hash value shall be stored */                                    
                                    &LaaPartialCHHash[TLS_ZERO],
    /* Holds a pointer to the memory location in which the output length in bytes
    is stored */                                
                                    &LulPartialCHHashLen);
                                    
    if ((E_OK == LddReturnValue) && 
                                (LulPartialCHHashLen == (uint32)TLS_THIRTYTWO))
    {
        for (LucPskCounter = (uint8)TLS_ZERO; LucPskCounter < LucNumOfPsks; 
                                                                LucPskCounter++)
        {
            LucPskLookUpIndex = 
            Tls_GaaPSKLookUp[LucPskStartIdx + LucPskCounter];
            
            LddReturnValue = Tls_GetInputPskKeys(
                                                LddConnectionId,
                                                LucPskLookUpIndex,
                                                &LulInputPskCsmKeyId);
             
            /* if this fails , just iterate to next PSK , instead of stoping the CH */
            if (E_OK == LddReturnValue)
            {
                /* get where to store the binder key */
                LulTrgBinderKey = 
                Tls_GaaPskIdentity[LucPskLookUpIndex].ulTlsCsmBinderKeyRefId;
                
                LddPSKHashType = 
                        Tls_GaaPskIdentity[LucPskLookUpIndex].ddTlsPSKHashType;
                        
                LddReturnValue = Tls_CalculateBinders(
                                                &LpExtensionData[LusIndex],
                                                &LaaPartialCHHash[TLS_ZERO],
                                                LulTrgBinderKey,
                                                LulInputPskCsmKeyId,
                                                LddPSKHashType);
                                                
                if (E_OK == LddReturnValue)
                {
                    LusBinderLen = (TLS_SHA_384 == LddPSKHashType) ? 
                    (uint16)TLS_FOURTYEIGHT : (uint16)TLS_THIRTYTWO;                   
                    /* adding 1 byte + 32 or 48 bytes bytes to move to next 
                    binder value  */
                    LusLenOfDynIdntsData += (uint16)((uint16)TLS_ONE + LusBinderLen);
                    
                    /* move to next binder value */
                    LusIndex += (uint16)((uint16)TLS_ONE + LusBinderLen);                   
                }
                else
                {
                    /* Tls_CalculateBinders Failed */
                    LblBreak = TLS_TRUE;
                }
            }
                
            if (TLS_TRUE == LblBreak)
            {
                break;
            }
        }
        
        if (E_OK == LddReturnValue)
        {
            /* Final length upate for Total binders field */
            LusLenOfDynIdntsData = Tls_Htons(LusLenOfDynIdntsData);
            Tls_MemCpy(&LpExtensionData[TLS_ZERO], &LusLenOfDynIdntsData, 
                                                            (uint16)TLS_TWO);            
        }    
    }
    else
    {
        LddReturnValue = E_NOT_OK;
    }
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_GetInputPskKeys                  **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function once called , based on wether the PSK 
                            conatiners is Dynamic or static , give out the 
                            CSM Input PSK key and Binder key slots **
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
**  Design ID           : TLS_SDD_0267                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetInputPskKeys(
                                const TcpIp_TlsConnectionIdType LddConnectionId,
                                const uint8 LucPskIndex,
                                uint32* const LpPskCsmKeyId)
{
    Std_ReturnType LddReturnValue;
    const uint8*  LpPskIdentyHintPtr;
    uint16 LusPskIdentyHintLen;
    uint16 LusPskIdentyLenPtr;
    uint8  LaaPskIdentyarr[TLS_MAX_LEN_OF_PSK_IDEN];
    
    /* initial values */
    LpPskIdentyHintPtr = NULL_PTR;
    LusPskIdentyHintLen = (uint16)TLS_ZERO;
    
    if (TLS_TRUE == Tls_GaaPskIdentity[LucPskIndex].blIsDynPskAvail)
    {
        /* Get the return value of the dynamic psk */
        LddReturnValue = 
        Tls_GaaPskIdentity[LucPskIndex].TlsPskGetClientKeyIdentityFunc(
                                                LddConnectionId,
                                                /* no use for current TLS */
                                                LusPskIdentyHintLen, 
                                                /* no use for current TLS */
                                                LpPskIdentyHintPtr, 
                                                &LusPskIdentyLenPtr,
                                                // values will be there here
                                                &LaaPskIdentyarr[TLS_ZERO], 
                                                LpPskCsmKeyId);

        if (E_OK == LddReturnValue)
        {
            if ((LusPskIdentyLenPtr > (uint16)TLS_THIRTYTWO) || 
                                (LusPskIdentyLenPtr < (uint16)TLS_EIGHT))
            {
                /* if Fetching dynamic psk wont work , then try to fetch 
                static PSK */
                /* SWS_TcpIp_00325 */
                if (TLS_INVALID_SHORT != 
                    Tls_GaaPskIdentity[LucPskIndex].usTlsPskIdentityLen)
                {  
                    *LpPskCsmKeyId = 
                        Tls_GaaPskIdentity[LucPskIndex].ulTlsPSKCsmKeyRefId;
                    LddReturnValue = E_OK;
                }
                else
                {
                    LddReturnValue = E_NOT_OK;
                }
            }               
        }
        else
        {
            /* if Fetching dynamic psk wont work , then try to fetch 
            static PSK */
            /* SWS_TcpIp_00325 */
            if (TLS_INVALID_SHORT != 
                    Tls_GaaPskIdentity[LucPskIndex].usTlsPskIdentityLen)
            {  
                *LpPskCsmKeyId = 
                    Tls_GaaPskIdentity[LucPskIndex].ulTlsPSKCsmKeyRefId;
                LddReturnValue = E_OK;
            }
            else
            {
                LddReturnValue = E_NOT_OK;
            }
        }   
    }
    else
    {
        *LpPskCsmKeyId = Tls_GaaPskIdentity[LucPskIndex].ulTlsPSKCsmKeyRefId;
        LddReturnValue = E_OK;
    }
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_CalculateBinders                  **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Once this function called , it will generate 
                          binder key and Binder **
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
**  Design ID           : TLS_SDD_0268                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_CalculateBinders(
                            uint8*  LpExtensionData,
                            const uint8* const LpPartialCHHash,
                            const uint32 LulTrgBinderKey,
                            const uint32 LulInputPskCsmKeyId,
                            const Tls_PSKHashType LddPSKHashType)
{
    Std_ReturnType LddReturnValue;
    uint32 LullHmacedOutLen;
    uint32 LulMacJobId;
    uint32 LulMacKeyId;
    uint32 LulPartialCHHashLen;
    uint8 LucTemp;
 
    LulMacJobId = 
    Tls_GaaCiphrSuiteWrkr[TLS_ZERO].ulTlsCipherCsmMacGenerateJobRefIdx;
    LulMacKeyId = 
    Tls_GaaCiphrSuiteWrkr[TLS_ZERO].ulTlsCipherCsmMacGenerateKeyRefIdx;

    LulPartialCHHashLen = (uint32)TLS_THIRTYTWO;
                                              
  
    LddReturnValue = 
    Tls_DeriveBinderMaterials(
                            LulInputPskCsmKeyId, 
                            LulTrgBinderKey,
                            LddPSKHashType);        

    if (E_OK == LddReturnValue)
    {
        /* based on Hash Type of the PSK, Output Mac length Changes */
        if (TLS_SHA_384 == LddPSKHashType)
        {
            LucTemp = (uint8)TLS_FOURTYEIGHT;
            LullHmacedOutLen = (uint32)TLS_FOURTYEIGHT;
        }
        else
        {
            LucTemp = (uint8)TLS_THIRTYTWO;
            LullHmacedOutLen = (uint32)TLS_THIRTYTWO;
        }

        *LpExtensionData = LucTemp;        
    
        LddReturnValue = Tls_PerformMacGenerate(
        /* JobId to perform Mac Generate */
                                        LulMacJobId,
        /* KeyId to be used in pair the Mac Job id to perform Mac Generate */
                                        LulMacKeyId,
        /* KeyId to be used to copy into Mac Key Id */
                                        LulTrgBinderKey,
        /* Contains the pointer to the data for which the MAC shall be computed */                                       
                                        LpPartialCHHash,
        /* Contains the number of bytes to be Hmaced */                                        
                                        LulPartialCHHashLen,
        /* Contains the pointer to the data where the MAC shall be stored */                                
                                        &LpExtensionData[TLS_ONE],
        /* Holds a pointer to the memory location in which the output 
        length in bytes is stored */                                        
                                        &LullHmacedOutLen);
        
        if (E_OK == LddReturnValue)
        {
            
            if (TLS_SHA_384 == LddPSKHashType)
            {
                if ((uint32)TLS_FOURTYEIGHT != LullHmacedOutLen)
                {
                    LddReturnValue = E_NOT_OK;
                }
            }
            else
            {
                if ((uint32)TLS_THIRTYTWO != LullHmacedOutLen)
                {
                    LddReturnValue = E_NOT_OK;
                }               
            }
        }
    }        
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_GetServerPSKExt                      **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Once this internal API is called , it calles some more  **
**                      : internal function to validate the CH message and preapres  **
**                      : and formats the Server hello PSK extension  **
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
**  Design ID           : TLS_SDD_0270                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
void  Tls_GetServerPSKExt(uint8* const LpHShakeBuf, uint16* const LpExtLen)
{ 
    uint16 LusTemp;
    uint16 LusIndex;
    
    /* Tag */
    LusTemp = Tls_Htons((uint16)TLS_PRE_SHARED_KEY);
    Tls_MemCpy(LpHShakeBuf, &LusTemp, (uint16)TLS_TWO);
    
    LusIndex = (uint16)TLS_TWO;
    
    /* Length */
    LusTemp = Tls_Htons((uint16)TLS_TWO);
    Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO);
    
    *LpExtLen = (*LpExtLen) + (uint16)TLS_SIX;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_ValidateClientPskExt                      **
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
**  Design ID           : TLS_SDD_0271                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ValidateClientPskExt(
                    const uint16 LusActiveConnIdx,
                    const uint8* const LpCHShakeBuf,
                    uint16* const LpPskIndex,
                    uint32* const LpPskCsmKeyId)
{
    TcpIp_TlsConnectionIdType LddConnectionId;
    Std_ReturnType  LddSrvrRtrnVal;
    Tls_PSKHashType LddPSKHashType;
    uint8* LpClientHelloMsgPtr;
    uint32 LulLenTillPskExt;
    uint32  LullLenOfSrvrBindr;
    uint16  LusLenOfServerPsk;
    uint16  LusLenOfClientBinder;
    uint16  LusExtLen;
    uint16 LusNegotitdCiprSutId;
    uint8  LucPskCounter;
    uint8  LucPskStartIdx;
    uint8  LucNumOfPsks;
    uint8  LaaIdentyHintArr[TLS_MAX_LEN_OF_PSK_IDEN];
    uint8  LaaServerBinder[TLS_SIZEOF_BINDER_ARR];
    uint8  LaaClientBinder[TLS_SIZEOF_BINDER_ARR];
    uint8 LucPskLookUpIndex;
    
    /* move to len field */ 
    /* Get the Ext length */
    LusExtLen = Tls_GetExtensionLength(&LpCHShakeBuf[TLS_TWO]);

    /* Moved from GetServerPskExt function to here */
    LddConnectionId = 
                Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;

    if ((LusExtLen >= (uint16)TLS_MIN_LEN_OF_PSK_EXT) && 
                                (LusExtLen <= (uint16)TLS_MAX_LEN_OF_PSK_EXT))
    {
        
        LucPskStartIdx = Tls_GaaConnection[LddConnectionId].ucPSKConnLookUpIdx;
        
        LucNumOfPsks = Tls_GaaConnection[LddConnectionId].ucNumOfPreSharedKeys;
        

        LpClientHelloMsgPtr = &Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO];
        
        
        LulLenTillPskExt = GetLenOfPskIdsInCh(LpClientHelloMsgPtr);

        LucPskCounter = (uint8)TLS_ZERO;
        do
        {
            LucPskLookUpIndex = 
                            Tls_GaaPSKLookUp[LucPskStartIdx + LucPskCounter];
            
            LddSrvrRtrnVal = Tls_GetServerPreSharedKey(
                                                    LddConnectionId,
                                                    LucPskLookUpIndex,
                                                    &LaaIdentyHintArr[TLS_ZERO],
                                                    &LusLenOfServerPsk);
                                                    
            if (E_OK == LddSrvrRtrnVal)
            {
                /* get the negotaited cipher suit from server side */
                LusNegotitdCiprSutId = 
                Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
                Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
                ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId;
                
                LddPSKHashType = 
                Tls_GaaPskIdentity[LucPskLookUpIndex].ddTlsPSKHashType;
                /* NEW CHECK */
                if ((uint16)TLS_AES_256_GCM_SHA384 == LusNegotitdCiprSutId)
                {
                    if (TLS_SHA_384 != LddPSKHashType)
                    {
                        LddSrvrRtrnVal = E_NOT_OK;
                    }
                }
                else
                {
                    if (TLS_SHA_384 == LddPSKHashType)
                    {
                        LddSrvrRtrnVal = E_NOT_OK;
                    }                    
                }
                
                if (E_OK == LddSrvrRtrnVal)
                {
                    LddSrvrRtrnVal = Tls_IsServrPSKPresentInCH(
                                                LpClientHelloMsgPtr,
                                                LulLenTillPskExt,
                                                &LaaIdentyHintArr[TLS_ZERO],
                                                LusLenOfServerPsk,
                                                LpPskIndex);

                    if (E_OK == LddSrvrRtrnVal)
                    {
                        LddSrvrRtrnVal = Tls_GetCsmKeyForServerPsk(   
                                                    LddConnectionId,
                                                    LucPskLookUpIndex,
                                                    LusLenOfServerPsk,
                                                    &LaaIdentyHintArr[TLS_ZERO],
                                                    LpPskCsmKeyId);

                        if (E_OK == LddSrvrRtrnVal)
                        {
                            break;                            
                        }
                        else
                        {
                            *LpPskIndex = (uint16)TLS_ZERO;                            
                        }
                    }
                    else
                    {
                        *LpPskIndex = (uint16)TLS_ZERO;
                    }
                }
            }
            
        LucPskCounter++;
        }while(LucPskCounter < LucNumOfPsks);        
 
        if (E_OK == LddSrvrRtrnVal)
        {
  
            LddSrvrRtrnVal = Tls_GetServerBinderValue(
                                                    LucPskLookUpIndex,
                                                    LpClientHelloMsgPtr,
                                                    *LpPskCsmKeyId,
                                                    &LaaServerBinder[TLS_ZERO],
                                                    &LullLenOfSrvrBindr);
      
            if (E_OK == LddSrvrRtrnVal)
            {

                Tls_GetCHMatchngPskBindrVal(LpClientHelloMsgPtr,
                                            LulLenTillPskExt,
                                            *LpPskIndex,
                                            &LaaClientBinder[TLS_ZERO],
                                            &LusLenOfClientBinder);
      
                LddSrvrRtrnVal = Tls_ValidateChShBinder(
                                                    &LaaClientBinder[TLS_ZERO],
                                                    LusLenOfClientBinder,
                                                    &LaaServerBinder[TLS_ZERO],
                                                    (uint16)LullLenOfSrvrBindr);
            

                if (E_OK == LddSrvrRtrnVal)
                {    
                    /* why its decremented becuz ( from RFC)
                    selected_identity:  The server's chosen identity expressed as a
                    (0-based) index into the identities in the client's list */
                    (*LpPskIndex)--;
                }
                else
                {
                    /* decrypt_error:  A handshake (not record layer) cryptographic
                    operation failed, including being unable to correctly verify a
                    signature or validate a Finished message or a PSK binder. */

                    Tls_GaaAlert[LusActiveConnIdx] = TLS_DECRYPT_ERROR;
                }        
            }
            else
            {
                /* validation failed , as per rfc stop here itself , dont go to
                validate next PSK , what NEXT ??*/

                Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
            } 
        }
        else
        { 
            /* unknown_psk_identity:  Sent by servers when PSK key establishment is
             desired but no acceptable PSK identity is provided by the client.
            Sending this alert is OPTIONAL; servers MAY instead choose to send
            a "decrypt_error" alert to merely indicate an invalid PSK
            identity
            */

            Tls_GaaAlert[LusActiveConnIdx] = TLS_DECRYPT_ERROR;
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

/*******************************************************************************
** Function Name        : Tls_ValidateServerPskExt                      **
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
**  Design ID           : TLS_SDD_0272                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ValidateServerPskExt(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8* const LpSHShakeBuf,
                            uint16* const LpExtLen,
                            uint32* const LpPskCsmKeyId)
{
    
    Std_ReturnType  LddSrvrRtrnVal;
    Tls_PSKHashType LddPSKHashType;
    uint16  LusExtLen;
    uint16  LusSelectedPskIdx;
    uint16  LusNumOfPsks;
    uint16  LusIndex;
    uint16  LusPskIdentyHintLen;
    uint16  LusPskIdentyLenPtr;
    uint16  LusNegotitdCiprSutId;
    uint16  LusActiveConnIdx;
    uint8   LucPskLookUpIdx;
    uint8   LucPskStartIdx;
    uint8   LaaPskIdentyarr[TLS_MAX_LEN_OF_PSK_IDEN];
    uint8   LaaPskIdentyHintPtr[TLS_ONE] = {(uint8)TLS_ZERO};    
    
 
    LusPskIdentyHintLen = (uint16)TLS_ZERO;
    /* move to len field */
    LusIndex = (uint16)TLS_TWO;
    
    /* Get the Ext length */
    LusExtLen = Tls_GetExtensionLength(&LpSHShakeBuf[LusIndex]);
    *LpExtLen = LusExtLen;

    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);
 
    if (LusExtLen == (uint16)TLS_TWO)
    {   
        /* move to value field */
        LusIndex += (uint16)TLS_TWO;
        /* Get the selected PSK index from teh buffer */
        LusSelectedPskIdx = Tls_GetExtensionLength(&LpSHShakeBuf[LusIndex]);
      
        LusNumOfPsks = Tls_GaaConnection[LddConnectionId].ucNumOfPreSharedKeys;
   
        if (LusSelectedPskIdx < LusNumOfPsks)
        {
            LucPskStartIdx = 
                        Tls_GaaConnection[LddConnectionId].ucPSKConnLookUpIdx;
            
            LucPskLookUpIdx = 
                        Tls_GaaPSKLookUp[(uint16)((uint16)LucPskStartIdx + 
                                                            LusSelectedPskIdx)];

            /* get the negotaited cipher suit from server side */
            LusNegotitdCiprSutId = 
            Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
            Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
            ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId;
                
            LddPSKHashType = 
                        Tls_GaaPskIdentity[LucPskLookUpIdx].ddTlsPSKHashType;
            
            /* NEW CHECK */
            if ((uint16)TLS_AES_256_GCM_SHA384 == LusNegotitdCiprSutId)
            {
                if (TLS_SHA_384 == LddPSKHashType)
                {
                    LddSrvrRtrnVal = E_OK;
                }
                else
                {
                    LddSrvrRtrnVal = E_NOT_OK;
                }
            }
            else
            {
                if (TLS_SHA_384 != LddPSKHashType)
                {
                    LddSrvrRtrnVal = E_OK;
                }
                else
                {
                    LddSrvrRtrnVal = E_NOT_OK;
                }    
            }
            
            if (E_OK == LddSrvrRtrnVal)
            {
                /* check wether dyamic is configured */
                if (TLS_TRUE == 
                            Tls_GaaPskIdentity[LucPskLookUpIdx].blIsDynPskAvail)
                {
                    /* Get the return value of the dynamic psk */
                    LddSrvrRtrnVal = 
                    Tls_GaaPskIdentity[LucPskLookUpIdx].
                                                TlsPskGetClientKeyIdentityFunc(
                                                LddConnectionId,
                                                /* no use for current TLS */
                                                LusPskIdentyHintLen, 
                                                /* no use for current TLS */
                                                &LaaPskIdentyHintPtr[TLS_ZERO], 
                                                &LusPskIdentyLenPtr,
                                                /* values will be there here */
                                                &LaaPskIdentyarr[TLS_ZERO],
                                                LpPskCsmKeyId);
                                               
                    if (E_OK != LddSrvrRtrnVal)
                    {
                        if (TLS_INVALID_SHORT != 
                            Tls_GaaPskIdentity[LucPskLookUpIdx].usTlsPskIdentityLen)
                        {
                            *LpPskCsmKeyId = 
                            Tls_GaaPskIdentity[LucPskLookUpIdx].ulTlsPSKCsmKeyRefId; 
                            LddSrvrRtrnVal = E_OK;                          
                        }    
                    }                            
                }
                /* as per our validation rule , there shd be either dynamic or 
                static pSK mandatorly configured, so else if not required  */
                /* SWS_TcpIp_00325 */
                else
                {
                    *LpPskCsmKeyId = 
                    Tls_GaaPskIdentity[LucPskLookUpIdx].ulTlsPSKCsmKeyRefId;
                }           
            }
            else
            {
                /* Clients MUST verify that the server's selected_identity is within the
                range supplied by the client, that the server selected a cipher suite
                indicating a Hash associated with the PSK, and that a server
                "key_share" extension is present if required by the ClientHello
                "psk_key_exchange_modes" extension.  If these values are not
                consistent, the client MUST abort the handshake with an
                "illegal_parameter" alert. */
            
                Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;        
            }
        }
        else
        {
            /* Clients MUST verify that the server's selected_identity is within the
            range supplied by the client, that the server selected a cipher suite
            indicating a Hash associated with the PSK, and that a server
            "key_share" extension is present if required by the ClientHello
            "psk_key_exchange_modes" extension.  If these values are not
            consistent, the client MUST abort the handshake with an
            "illegal_parameter" alert. */
            
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


/*******************************************************************************
** Function Name        : Tls_GetServerStaticPSK                  **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Once this internal API is called , it return the static  **
**        PSK identity asso'to the PSK conatiner              **
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
**  Design ID           : TLS_SDD_0273                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_GetServerStaticPSK(const uint8 LucPskCounter,
                                                uint8* const LpIdentityHintPtr)
{
    const uint8*  LpPskIdentity;
    uint16  LusPskIdentityLen;
    
    LusPskIdentityLen = 
                    Tls_GaaPskIdentity[LucPskCounter].usTlsPskIdentityHintLen;
    
    LpPskIdentity = 
    &Tls_GaaPskIdentity[LucPskCounter].aaPresharedKeyIdentityHint[TLS_ZERO];
    
    Tls_ConvertTextToHex(LpPskIdentity, LpIdentityHintPtr, LusPskIdentityLen);
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_GetServerDynPSK                  **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Once this internal API is called , it call the     **
      required callout function functions associted to the  **
      PSKK conatiner           **
**       to get the PSK identity                              **
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
**  Design ID           : TLS_SDD_0274                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetServerDynPSK(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8 LucPskCounter,
                            uint8* const LpIdentityHintPtr,
                            uint16* const LpLenOfPskIdntyHint)
{
    Std_ReturnType  LddReturnValue;
    uint16  LusIdntyHintLenPtr;
    uint8  LaaIdtyHintHexFrmt[TLS_MAX_LEN_OF_PSK_IDEN] = {(uint8)TLS_ZERO};
    
    LusIdntyHintLenPtr = (uint16)TLS_ZERO;
    
    if ((Tls_GaaPskIdentity[LucPskCounter].TlsPskGetKeyIdentityHintFunc)
                                                                    != NULL_PTR)
    {
        LddReturnValue = 
        Tls_GaaPskIdentity[LucPskCounter].TlsPskGetKeyIdentityHintFunc(
                                                LddConnectionId,
                                                &LusIdntyHintLenPtr,
                                                /* values will be there here */
                                                LpIdentityHintPtr); 
                                                
        if (E_OK == LddReturnValue)
        {
            if ((LusIdntyHintLenPtr > (uint16)TLS_THIRTYTWO) || 
                                    (LusIdntyHintLenPtr < (uint16)TLS_EIGHT))
            {
                LddReturnValue = E_NOT_OK;
            }
            else
            {
                *LpLenOfPskIdntyHint = LusIdntyHintLenPtr;
        
                Tls_ConvertTextToHex(LpIdentityHintPtr, 
                            &LaaIdtyHintHexFrmt[TLS_ZERO], LusIdntyHintLenPtr);
        
                Tls_MemCpy(LpIdentityHintPtr, &LaaIdtyHintHexFrmt[TLS_ZERO],
                                                        LusIdntyHintLenPtr);               
            }    
        }            
    }
    else
    {
        LddReturnValue =  E_NOT_OK;        
    }
 
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_IsServrPSKPresentInCH                  **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Once this internal API is called , its checks wether**
**     the server side PSK is available in the CH PSK extension **
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
**  Design ID           : TLS_SDD_0275                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_IsServrPSKPresentInCH(
                                    const uint8* const LpClientHelloMsgPtr,
                                    const uint32 LulLenTillPskExt,
                                    const uint8* const LpIdentityHintPtr,
                                    const uint16 LusLenOfServerPsk,
                                    uint16* const LpPskIndex)
{
    Std_ReturnType  LddReturnValue; 
    const uint8*  LpClientPskPtr;
    const uint8*  LpServerPskPtr;
    uint16  LusLenOfAllPskIds;
    uint16  LusLenOfAllPskIdCnt;
    uint16  LusCount; 
    uint16  LusLenOfClientPsk; 
    uint32  LulIndex; 
    boolean  LblPskFound; 
    
    LblPskFound = TLS_TRUE;
    LusLenOfAllPskIdCnt = (uint16)TLS_ZERO;

    LulIndex = LulLenTillPskExt;
    
    LusLenOfAllPskIds = 
        ((uint16)((uint16)LpClientHelloMsgPtr[LulIndex] << (uint16)TLS_EIGHT) | 
                    (uint16)LpClientHelloMsgPtr[LulIndex + (uint32)TLS_ONE]);                            
                            
    /* move in position by two bytes to "Length of first identity string"
    Field */
    LulIndex += (uint32)TLS_TWO;

    do
    {
        (*LpPskIndex)++;

        LusLenOfClientPsk = 
        ((uint16)((uint16)LpClientHelloMsgPtr[LulIndex] << (uint16)TLS_EIGHT) | 
                    (uint16)LpClientHelloMsgPtr[LulIndex + (uint32)TLS_ONE]);                             
                            
        /* move in position by two bytes to "Psk identity string encoded in 
        ASCII" Field*/
        LulIndex += (uint32)TLS_TWO;
        
        /* save the address of client PSK , instead of partially updating
        LpClientHelloMsgPtr*/
        LpClientPskPtr = &LpClientHelloMsgPtr[LulIndex];
        
        /* save the address of server PSK , instead of updating the actual
        pointer LpIdentityHintPtr*/
        LpServerPskPtr = LpIdentityHintPtr;
  
        /* move in position by psk len bytes + tkt age ( 4 bytes) 
        to next psk*/  
            LulIndex += (uint32)LusLenOfClientPsk + (uint32)TLS_FOUR;
        
        /* checking if server psk len and client psk length are correct , 
        before checking teh actual PSK identity */
        if (LusLenOfServerPsk == LusLenOfClientPsk)
        {
            /* start comparing the server PSK with Client PSK*/
            for (LusCount = (uint16)TLS_ZERO; LusCount < LusLenOfClientPsk; 
                        LusCount++)
            {   
                if ((LpClientPskPtr[LusCount]) != (LpServerPskPtr[LusCount]))
                {
                    LblPskFound = TLS_FALSE;
                    break;
                }
            }
        }
        else
        {   
            LblPskFound = TLS_FALSE;
        }

        /* to point to next PSK in CH message */
        LusLenOfAllPskIdCnt += (uint16)((uint16)TLS_TWO + LusLenOfClientPsk + 
                                (uint16)TLS_FOUR);
                                
        if (LblPskFound == TLS_TRUE)
        {
            LddReturnValue = E_OK;
            break;
        }
        else
        {
            LblPskFound = TLS_TRUE;
            LddReturnValue = E_NOT_OK;
        }
    }while(LusLenOfAllPskIdCnt < LusLenOfAllPskIds);
    return LddReturnValue; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_GetCHMatchngPskBindrVal                  **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Once this internal API is called,it loops thrugh the**
**        Client hello PSk extension to get the Binder for the**
**       Selected/Matched PSK                                 **
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
**  Design ID           : TLS_SDD_0276                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_GetCHMatchngPskBindrVal(
                                    const uint8* const LpClientHelloMsgPtr,
                                    const uint32 LulLenTillPskExt,
                                    const uint16 LusPskIndex,
                                    uint8* const LpClientBinder,
                                    uint16* const LpLenOfClientBinder)
{
 
    uint16  LusLenOfAllPskIds;
    uint16  LusPskCount;
    uint32  LulIndex; 
    uint8  LucLenOfBinder;
    
    LusPskCount = (uint16)TLS_ZERO;
    
    /* move in position by LulLenTillPskExt bytes to Length of all identities 
    data Field */
    LulIndex = LulLenTillPskExt;
    
    /* used to skip this many bytes , to move to binder field */                          
    LusLenOfAllPskIds = 
    ((uint16)((uint16)LpClientHelloMsgPtr[LulIndex] << (uint16)TLS_EIGHT) | 
                    (uint16)LpClientHelloMsgPtr[LulIndex + (uint32)TLS_ONE]);                             

    /* move in position by tw0 bytes to "Length of first identity data" Field*/
    LulIndex += (uint32)TLS_TWO;
    
    /* move in position by LusLenOfAllPskIds bytes to "Length of all binders 
    data"  field*/
    LulIndex += (uint32)LusLenOfAllPskIds; 
    
    /* move in position by twi bytes to "Length of first binder" Field*/  
    LulIndex += (uint32)TLS_TWO; 
    
    
    while (LusPskCount < (uint16)(LusPskIndex - (uint16)TLS_ONE))
    {
        /* to loop till the respective binder of the psk */
        LusPskCount++;                            

        /* save how many bytes binder value to be read and compared*/                                  
        LucLenOfBinder = LpClientHelloMsgPtr[LulIndex];  
                    
        /* move in position by two bytes to "binder value" Field*/
        LulIndex += (uint32)TLS_ONE;
        
        /* move in position by LucLenOfBinder bytes to next binder data */
        LulIndex += (uint32)LucLenOfBinder;
    }

  
    LucLenOfBinder = LpClientHelloMsgPtr[LulIndex];                                
                            
    *LpLenOfClientBinder = (uint16)LucLenOfBinder;
    
    LulIndex += (uint32)TLS_ONE;
    
    Tls_MemCpy(LpClientBinder, &LpClientHelloMsgPtr[LulIndex], 
                                                    (uint16)LucLenOfBinder);
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_GetCsmKeyForServerPsk                            **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Once this internal API is called , it calls the **
      requried **
**        Csm Apis to get the Key associated to the PSK       **
**       conatiner , by checking if its dynamic or static     **
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
**  Design ID           : TLS_SDD_0277                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetCsmKeyForServerPsk(   
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8  LucPskCounter,
                            const uint16  LusLenOfServerPsk,
                            const uint8* const LpIdentityPtr,
                            uint32* const LpCsmKeyId)
{
    Std_ReturnType  LddReturnValue;
    
    if (TLS_TRUE == Tls_GaaPskIdentity[LucPskCounter].blIsDynPskAvail)
    {
        LddReturnValue = 
        Tls_GaaPskIdentity[LucPskCounter].TlsPskGetServerKeyIdentityFunc(
                                                            LddConnectionId,
                                                            LusLenOfServerPsk,
                                                            LpIdentityPtr,
                                                            LpCsmKeyId);
    
        /* if Fetching csm key for dynamic psk wont work , then try to fetch 
        key for static PSK */
        /* SWS_TcpIp_00325 */   
        if (E_OK != LddReturnValue)
        {
            if (TLS_INVALID_WORD != 
                        Tls_GaaPskIdentity[LucPskCounter].ulTlsPSKCsmKeyRefId)
            {
                *LpCsmKeyId = 
                        Tls_GaaPskIdentity[LucPskCounter].ulTlsPSKCsmKeyRefId;
                LddReturnValue = E_OK;    
            }
        } 
    }
    else
    {
        /* check if static is configured */
        *LpCsmKeyId = Tls_GaaPskIdentity[LucPskCounter].ulTlsPSKCsmKeyRefId;
        LddReturnValue = E_OK;
    }
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_GetServerBinderValue                            **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Once this internal API is called , it calls the     **
      requried Csm Apis to generated the Binder value       **
**       associated to the PSK                                **
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
**  Design ID           : TLS_SDD_0283                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetServerBinderValue(
                            const uint8  LucPskCounter,
                            const uint8* const LpClientHelloMsgPtr,
                            const uint32  LulCsmKeyId,
                            uint8* const LpServerBinder,
                            uint32* const LpHmacedOutLen)
{
 
    Std_ReturnType  LddReturnValue;
    Tls_PSKHashType LddPSKHashType;
    uint32 LullLenToBeHMaced;
    uint32 LulTrgBinderKey;
    uint32 LulMacJobId;
    uint32 LulMacKeyId;
    uint32 LulHashingJobId;
    uint32 LulPartialCHHashLen;
    
    uint8 LaaPartialCHHash[TLS_THIRTYTWO] = {0};
    LulPartialCHHashLen = (uint32)TLS_THIRTYTWO;
    
    LulTrgBinderKey = Tls_GaaPskIdentity[LucPskCounter].ulTlsCsmBinderKeyRefId;
    LddPSKHashType = Tls_GaaPskIdentity[LucPskCounter].ddTlsPSKHashType;

    LulMacJobId = 
    Tls_GaaCiphrSuiteWrkr[TLS_ZERO].ulTlsCipherCsmMacGenerateJobRefIdx;
    LulMacKeyId = 
    Tls_GaaCiphrSuiteWrkr[TLS_ZERO].ulTlsCipherCsmMacGenerateKeyRefIdx;


    LulHashingJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[TLS_ZERO].
    usTlsConnHandshakeRefBaseIndx].ulTlsCsmHashVerifyJobRefIdx;
    
    LullLenToBeHMaced = GetCHLenToBeHMaced(LpClientHelloMsgPtr);
   
    /* Prepare the Hash Transciprt only once */
    LddReturnValue = Tls_PerformHash(
    /* JobId to perform Hash */
                                    LulHashingJobId,
    /* Contains the pointer to the data for which the hash shall be computed */                                    
                                    LpClientHelloMsgPtr,
    /* Contains the number of bytes to be hashed */                                 
                                    LullLenToBeHMaced,
    /* Contains the pointer to the data where the hash value shall be stored */                                    
                                    &LaaPartialCHHash[TLS_ZERO],
    /* Holds a pointer to the memory location in which the output length in bytes
    is stored */                                
                                    &LulPartialCHHashLen);

    if ((E_OK == LddReturnValue) && 
                                (LulPartialCHHashLen == (uint32)TLS_THIRTYTWO))
    {
        LddReturnValue = 
        Tls_DeriveBinderMaterials(LulCsmKeyId, LulTrgBinderKey, LddPSKHashType);

        if (E_OK == LddReturnValue)
        { 
            /* based on Hash Type of the PSK, Output Mac length Changes */
            *LpHmacedOutLen = (TLS_SHA_384 == LddPSKHashType) ? 
            (uint32)TLS_FOURTYEIGHT : (uint32)TLS_THIRTYTWO;   
            
            LddReturnValue = Tls_PerformMacGenerate(
            /* JobId to perform Mac Generate */
                                                    LulMacJobId,
            /* KeyId to be used in pair the Mac Job id to perform Mac Generate */
                                                    LulMacKeyId,
            /* KeyId to be used to copy into Mac Key Id */
                                                    LulTrgBinderKey,
            /* Contains the pointer to the data for which the MAC shall be computed */                                       
                                                    &LaaPartialCHHash[TLS_ZERO],
            /* Contains the number of bytes to be Hmaced */                                        
                                                     LulPartialCHHashLen,
            /* Contains the pointer to the data where the MAC shall be stored */                                
                                                    LpServerBinder,
            /* Holds a pointer to the memory location in which the output 
            length in bytes is stored */                                        
                                                    LpHmacedOutLen);  
                                        

            if (E_OK == LddReturnValue)
            {
                if (TLS_SHA_384 == LddPSKHashType)
                {
                    if ((uint32)TLS_FOURTYEIGHT != (*LpHmacedOutLen))
                    {
                        LddReturnValue = E_NOT_OK;
                    }
                }
                else
                {
                    if ((uint32)TLS_THIRTYTWO != (*LpHmacedOutLen))
                    {
                        LddReturnValue = E_NOT_OK;
                    }               
                }
            }
        }
    }
    else
    {
        LddReturnValue = E_NOT_OK;
    } 
    
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_ValidateChShBinder                    **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Once this internal API is called , its checks wether**
**        the Selected CH binder and Serverhello binder are same**
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
**  Design ID           : TLS_SDD_0282                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateChShBinder(
                                            const uint8* const LpClientBinder,
                                            const uint16  LusLenOfClientBindr,
                                            const uint8* const LpServerBinder,
                                            const uint16  LusLenOfSrvrBindr)
{
    Std_ReturnType  LddReturnValue;
    uint16  LusCount;
    
    LusCount = (uint16)TLS_ZERO;
    LddReturnValue = E_OK;
    
    if (LusLenOfSrvrBindr == LusLenOfClientBindr)
    {
        do
        {
            if ((LpClientBinder[LusCount]) != (LpServerBinder[LusCount]))
            {
                LddReturnValue = E_NOT_OK;
                break;
            }
            LusCount++;
        }while(LusCount < LusLenOfSrvrBindr);  
    }
    else
    {
        LddReturnValue = E_NOT_OK;
    }

 return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_GetServerPreSharedKey                            **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Once this internal API is called , its checks wether**
      the input (sent ) PSK is static or dyamic pskand calls**
**      next internal APIS, to get the PSK Identity of the PSK**
**      container             **
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
**  Design ID           : TLS_SDD_0279                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetServerPreSharedKey(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const uint8  LucPskCounter,
                            uint8* const LpIdentityHintPtr,
                            uint16* const LpLenOfServerPsk)
{
    Std_ReturnType  LddReturnValue;
 
    if (TLS_TRUE == Tls_GaaPskIdentity[LucPskCounter].blIsDynPskAvail)
    {
        LddReturnValue = Tls_GetServerDynPSK(LddConnectionId,
                                            LucPskCounter,
                                            LpIdentityHintPtr, 
                                            LpLenOfServerPsk); 
   
        /* if Fetching dynamic psk wont work , then try to fetch static PSK */
        /* SWS_TcpIp_00325 */   
        if (E_OK != LddReturnValue)
        {
            if (TLS_INVALID_SHORT != 
                    Tls_GaaPskIdentity[LucPskCounter].usTlsPskIdentityHintLen)
            {
                Tls_GetServerStaticPSK(LucPskCounter, LpIdentityHintPtr);
     
                *LpLenOfServerPsk = 
                    Tls_GaaPskIdentity[LucPskCounter].usTlsPskIdentityHintLen;
                LddReturnValue = E_OK;
            }    
        }
    }
    /* either dynamic PSK or Static psk should be configured mandatorly as per 
    validation rule*/
    /* SWS_TcpIp_00325 */
    else
    {
        Tls_GetServerStaticPSK(LucPskCounter, LpIdentityHintPtr);
        
        *LpLenOfServerPsk = 
                    Tls_GaaPskIdentity[LucPskCounter].usTlsPskIdentityHintLen;  
                    
        LddReturnValue = E_OK;                    
    }
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : GetCHLenToBeHMaced                             **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function gets the total length from handhake header
       till the PSK extenion ( indentities filed).**
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
**  Design ID           : TLS_SDD_0281                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static uint32 GetCHLenToBeHMaced(const uint8* const LpClientHelloMsgPtr)
{ 
    uint32 LulLenOfPartialCH;
    const uint8* LpCHPtr;
    uint16 LusExtType;
    uint16 LusCiprSuitSize;
    uint16 LusTemp;
    uint16 LusIndex;
 
 
    LpCHPtr = LpClientHelloMsgPtr;
    
    LusIndex = (uint16)TLS_THIRTYNINE;
    LulLenOfPartialCH = (uint32)TLS_THIRTYNINE;
    
    /* take the 40th and 41th byte */
    LusCiprSuitSize = 
                    ((uint16)((uint16)LpCHPtr[LusIndex] << (uint16)TLS_EIGHT) | 
                                (uint16)LpCHPtr[LusIndex + (uint16)TLS_ONE]);
    
    LusIndex += (uint16)((uint16)TLS_SIX + LusCiprSuitSize);
    
    LulLenOfPartialCH += ((uint32)TLS_SIX + (uint32)LusCiprSuitSize);
 
    do
    {

        LusExtType = Tls_GetExtensionLength(&LpCHPtr[LusIndex]);                                        

        /* Move to Length field */
        LusIndex += (uint16)TLS_TWO;
        LulLenOfPartialCH += (uint32)TLS_TWO; 
        /* get the lenth field of the extension */                                       
        LusTemp = ((uint16)((uint16)LpCHPtr[LusIndex] << (uint16)TLS_EIGHT) | 
                                (uint16)LpCHPtr[LusIndex + (uint16)TLS_ONE]);
                                
        /* Move to Value field */
        LusIndex += (uint16)TLS_TWO;
        LulLenOfPartialCH += (uint32)TLS_TWO; 
        
        if ((uint16)TLS_PRE_SHARED_KEY == LusExtType)
        {
            LusTemp = 
                    ((uint16)((uint16)LpCHPtr[LusIndex] << (uint16)TLS_EIGHT) | 
                                (uint16)LpCHPtr[LusIndex + (uint16)TLS_ONE]);  
                                

            LulLenOfPartialCH += (uint32)LusTemp + (uint32)TLS_TWO;
        }
        else
        {
            LusIndex += LusTemp;
            LulLenOfPartialCH += (uint32)LusTemp;   
        }
  
    }while((uint16)TLS_PRE_SHARED_KEY != LusExtType);
 
    return LulLenOfPartialCH;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : GetLenOfPskIdsInCh                             **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function gets the length of total PSK ids Data  
      in the CH message           **
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
**  Design ID           : TLS_SDD_0278                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static uint32 GetLenOfPskIdsInCh(const uint8* const LpCHPtr)
{ 
    uint32 LulResultLen;
    uint16 LusExtType;
    uint16 LusCiprSuitSize;
    uint16 LusTemp;
    uint16 LusIndex;
 
    LusIndex = (uint16)TLS_THIRTYNINE;
    LulResultLen = (uint32)TLS_THIRTYNINE;
                                
    LusCiprSuitSize = 
                    ((uint16)((uint16)LpCHPtr[LusIndex] << (uint16)TLS_EIGHT) | 
                                (uint16)LpCHPtr[LusIndex + (uint16)TLS_ONE]);                                     

    LusIndex += (uint16)((uint16)TLS_SIX + LusCiprSuitSize);
    
    LulResultLen += ((uint32)TLS_SIX + (uint32)LusCiprSuitSize);
 
    do
    {

        LusExtType = Tls_GetExtensionLength(&LpCHPtr[LusIndex]);

        /* Move to Length field */
        LusIndex += (uint16)TLS_TWO;
        LulResultLen += (uint32)TLS_TWO; 
        /* get the lenth field of the extension */                                  
        LusTemp = ((uint16)((uint16)LpCHPtr[LusIndex] << (uint16)TLS_EIGHT) | 
                                (uint16)LpCHPtr[LusIndex + (uint16)TLS_ONE]);                                       

        /* Move to Value field */
        LusIndex += (uint16)TLS_TWO;
        LulResultLen += (uint32)TLS_TWO; 
   
        if ((uint16)TLS_PRE_SHARED_KEY != LusExtType)
        {
            /* Move LusTemp number of bytes to move to next extension */
            LusIndex += LusTemp;
            LulResultLen += (uint32)LusTemp; 
        }
  
    }while((uint16)TLS_PRE_SHARED_KEY != LusExtType);
 
    return LulResultLen;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#endif

/* parasoft-end-suppress CERT_C-API00-a-3 */
/* parasoft-end-suppress CERT_C-DCL19-a-3 */
/* parasoft-end-suppress MISRAC2012-RULE_20_1-a-4 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-d-2 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 */
