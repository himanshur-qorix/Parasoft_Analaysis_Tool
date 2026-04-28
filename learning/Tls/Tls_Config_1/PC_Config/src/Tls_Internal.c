/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_Internal.c                                                 **
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
**  Design ID  : TLS_SDD_0308, TLS_SDD_0347, TLS_SDD_0348                     **
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
* #section Tls_Internal_c_REF_1
* Violates MISRAC2012-RULE_10_5-a-4:  Do not cast from the essential '' type to
* the essentially enum '' type
* Justification: The cast from '' to '' is intentional and safe in this context.
*
* #section Tls_Internal_c_REF_2
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
* #section Tls_Internal_c_REF_3
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
*
* #section Tls_Internal_c_REF_4
* Violates CERT_C-DCL00-b-3: Pass parameter '' with const specifier
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
* #section Tls_Internal_c_REF_5
* Violates MISRAC2012-RULE_5_4-c-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Internal_c_REF_6
* Violates MISRAC2012-RULE_5_4-d-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Internal_c_REF_7
* Violates MISRAC2012-RULE_20_1-a-4: Do not precede the #include directive with
* executable code
* Justification: Macros are defined by AUTOSAR standards and internal
* specifications.
*
* #section Tls_Internal_c_REF_8
* Violates CERT_C-DCL19-a-3: Variable "" should be declared as locally as
* possible
* Justification: Coding guidelines suggest the variables are declared on the
* start of the function.
*
* #section Tls_Internal_c_REF_9
* Violates CERT_C-API00-a-3: Parameter '' is not validated before use.
* Justification: This parameter either originates from the configuration or is
* validated in the main API before being passed as an argument to the local API.
* Hence, additional validation within the local API is not required and has been
* manually verified.
*/


/* parasoft-begin-suppress MISRAC2012-RULE_5_4-c-2 
"Reason: Tls_Internal_c_REF_5" */
/* parasoft-begin-suppress MISRAC2012-RULE_5_4-d-2 
"Reason: Tls_Internal_c_REF_6" */
/* parasoft-begin-suppress MISRAC2012-RULE_20_1-a-4 
"Reason: Tls_Internal_c_REF_7" */
/* parasoft-begin-suppress CERT_C-DCL19-a-3 
"Reason: Tls_Internal_c_REF_8" */
/* parasoft-begin-suppress CERT_C-API00-a-3 
"Reason: Tls_Internal_c_REF_9" */


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
 
/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_PrepareFinishVerifyData(const uint16 LusActiveConnIdx,
                                            const uint32 LulSrcHSTrficSecrt);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_ProcessCloseConn(const TcpIp_TlsConnectionIdType LddConnectionId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_DestroyKeys(const TcpIp_TlsConnectionIdType LddConnectionId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_HandleRxIndication (const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_HandleTxIndication (const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PerformCloseConnAction(
                            const TcpIp_TlsConnectionIdType  LddConnectionId);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_CheckCHTag(const uint16 LusTag);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_CheckSHTag(const uint16 LusTag);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_CheckCRTag(const uint16 LusTag);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_MainFunction                                    **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Performs periodic  operation                        **
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
**  Design ID           : TLS_SDD_0245, TLS_SDD_0341                          **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
void Tls_MainFunction(const TcpIp_TlsConnectionIdType TlsConnectionId)
{
    Tls_ConnectionStatusType LddHSStatus;
    Tls_ActiveConnType* LpActiveConn;
    uint16 LusActiveConnIdx;
    
    if (!(Tls_GblInitStatus))
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                                        TLS_MAINFUNCTION_SID, TLS_E_UNINIT);
        #endif 
    }
    else if (TlsConnectionId >= TLS_TOTAL_CONNECTIONS)
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                       TLS_MAINFUNCTION_SID, TLS_E_INVALID_PARAMETER);
        #endif   
    }
    else
    {
        LddHSStatus = Tls_GddConnHSStatus[TlsConnectionId];
        
        if (TLS_CONNECTION_NOT_INITIATED != LddHSStatus)
        {
            LusActiveConnIdx = Tls_GetActIdx(TlsConnectionId); 
            
            LpActiveConn = &Tls_GddActiveConnManager[LusActiveConnIdx];
            
            /* dont process any application when Tx is in progress */
            if (
            (!(Tls_HandshakeFrame[LusActiveConnIdx].blIsTxInProgress)) &&
            ((Tls_HandshakeFrame[LusActiveConnIdx].blIsHSKeysDerived) || 
            (Tls_HandshakeFrame[LusActiveConnIdx].blIsAppKeysDerived)) &&
            /* if any alert is logged , dont */
            (TLS_UNKNOWN_ALERT_DESCRIPTION == Tls_GaaAlert[LusActiveConnIdx]) &&
            /* if closeconnection was called in between Rx indications */
            (TLS_CONNECTION_CLOSED != LddHSStatus) &&
            (TLS_CONNECTION_FAILED != LddHSStatus)
            )
            {
                Tls_HandleRxIndication(LusActiveConnIdx);
            }
             
            LddHSStatus = Tls_GddConnHSStatus[TlsConnectionId];

            /* Process HandShake Activities for each accepted connection */          
            if ((LpActiveConn->ddConnectionState == TLS_CONNECTION_ACTIVE) &&
                                (TLS_CONNECTION_IN_PROGRESS == LddHSStatus))
            {
                Tls_ProcessHShakeProtocol(LusActiveConnIdx);                             
            }
            /* Process HandShake Activities for each accepted connection */          
            else if ((LpActiveConn->ddConnectionState == TLS_CONNECTION_ACTIVE) &&
                                        (TLS_CONNECTION_READY == LddHSStatus) &&
                    (Tls_HandshakeFrame[LusActiveConnIdx].blIsTxInProgress))
            {
                /* Application Data Handling by Record Protocol */
                Tls_HandleTxIndication(LusActiveConnIdx);           
            }
            /* if failed , or aborted during Application data transfer,
            for now just reset to idle state , coming next will add alert for this 
            also , like we did for Closed */
            else if ((TLS_CONNECTION_FAILED == LddHSStatus) || 
                                        (TLS_CONNECTION_ABORTED == LddHSStatus))
            {
                Tls_PerformCloseConnAction(TlsConnectionId); 
            }
            /* Close connection was called, previosly , 
            now process the close connection request  */
            else if (TLS_CONNECTION_CLOSED == LddHSStatus)
            {
                Tls_ProcessCloseConn(TlsConnectionId);
            }
            else
            {
                /* Do nothing */
            }         
        }
        else
        {
            /* Do nothing */
        }        
    } 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_ConvertTextToHex                  **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Converts Text to string with a lookup table **
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
**  Design ID           : TLS_SDD_0246                                         **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
void  Tls_ConvertTextToHex(
                            const uint8* const LpInptPskIdentyTxt,
                            uint8* const LpHexDataResult,
                            const uint16  LusPskIdentityLen)
{

    const uint8 LaaTxtToHexlookupTble[TLS_SIZEOF_TEXT_TO_HEX_ARR] =
    {
        TLS_HEX_00, TLS_HEX_01, TLS_HEX_02, TLS_HEX_03,
        TLS_HEX_04, TLS_HEX_05, TLS_HEX_06, TLS_HEX_07,
        TLS_HEX_08, TLS_HEX_09, TLS_HEX_0A, TLS_HEX_0B,
        TLS_HEX_0C, TLS_HEX_0D, TLS_HEX_0E, TLS_HEX_0F,
    
        TLS_HEX_10, TLS_HEX_11, TLS_HEX_12, TLS_HEX_13,
        TLS_HEX_14, TLS_HEX_15, TLS_HEX_16, TLS_HEX_17,
        TLS_HEX_18, TLS_HEX_19, TLS_HEX_1A, TLS_HEX_1B,
        TLS_HEX_1C, TLS_HEX_1D, TLS_HEX_1E, TLS_HEX_1F,
    
        TLS_HEX_20, TLS_HEX_21, TLS_HEX_22, TLS_HEX_23,
        TLS_HEX_24, TLS_HEX_25, TLS_HEX_26, TLS_HEX_27,
        TLS_HEX_28, TLS_HEX_29, TLS_HEX_2A, TLS_HEX_2B,
        TLS_HEX_2C, TLS_HEX_2D, TLS_HEX_2E, TLS_HEX_2F,
    
        TLS_HEX_30, TLS_HEX_31, TLS_HEX_32, TLS_HEX_33,
        TLS_HEX_34, TLS_HEX_35, TLS_HEX_36, TLS_HEX_37,
        TLS_HEX_38, TLS_HEX_39, TLS_HEX_3A, TLS_HEX_3B,
        TLS_HEX_3C, TLS_HEX_3D, TLS_HEX_3E, TLS_HEX_3F,
    
        TLS_HEX_40, TLS_HEX_41, TLS_HEX_42, TLS_HEX_43,
        TLS_HEX_44, TLS_HEX_45, TLS_HEX_46, TLS_HEX_47,
        TLS_HEX_48, TLS_HEX_49, TLS_HEX_4A, TLS_HEX_4B,
        TLS_HEX_4C, TLS_HEX_4D, TLS_HEX_4E, TLS_HEX_4F,
    
        TLS_HEX_50, TLS_HEX_51, TLS_HEX_52, TLS_HEX_53,
        TLS_HEX_54, TLS_HEX_55, TLS_HEX_56, TLS_HEX_57,
        TLS_HEX_58, TLS_HEX_59, TLS_HEX_5A, TLS_HEX_5B,
        TLS_HEX_5C, TLS_HEX_5D, TLS_HEX_5E, TLS_HEX_5F,
    
        TLS_HEX_60, TLS_HEX_61, TLS_HEX_62, TLS_HEX_63,
        TLS_HEX_64, TLS_HEX_65, TLS_HEX_66, TLS_HEX_67,
        TLS_HEX_68, TLS_HEX_69, TLS_HEX_6A, TLS_HEX_6B,
        TLS_HEX_6C, TLS_HEX_6D, TLS_HEX_6E, TLS_HEX_6F,
    
        TLS_HEX_70, TLS_HEX_71, TLS_HEX_72, TLS_HEX_73,
        TLS_HEX_74, TLS_HEX_75, TLS_HEX_76, TLS_HEX_77,
        TLS_HEX_78, TLS_HEX_79, TLS_HEX_7A, TLS_HEX_7B,
        TLS_HEX_7C, TLS_HEX_7D, TLS_HEX_7E, TLS_HEX_7F
    };

    uint16  LusLenOfPsk;
    
    LusLenOfPsk = (uint16)TLS_ZERO;
    
    while (LusLenOfPsk < LusPskIdentityLen) 
    {
        LpHexDataResult[LusLenOfPsk] = 
                        LaaTxtToHexlookupTble[LpInptPskIdentyTxt[LusLenOfPsk]];
        LusLenOfPsk++;
    } 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_GetInActiveConnIdx                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : loops through  Tls_GddActiveConnManager and fetches  **
**                        a slot which is not used, and also verifies group connections  **
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
**  Design ID           : TLS_SDD_0254                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_GetInActiveConnIdx(
                            const TcpIp_TlsConnectionIdType  LddConnectionId,
                            const Tls_ConnectionType  LddConnectionType,
                            uint16* const LpActiveConIdx)
{
    Std_ReturnType  LddReturnValue;
    uint16    LusActiveConnIdx;
    #if (STD_ON == TLS_GROUP_CONFIGURED)
        uint16  LusGroupId;
    #endif
 
 
    LddReturnValue = E_NOT_OK;
    
    for (LusActiveConnIdx = (uint16)TLS_ZERO; 
        LusActiveConnIdx < TLS_MAX_ACTIVE_CONNECTIONS; LusActiveConnIdx++)
    {
        if (Tls_GddActiveConnManager[LusActiveConnIdx].ddConnectionState ==
                                                    TLS_CONNECTION_INACTIVE)
        {
            LddReturnValue = E_OK;
            *LpActiveConIdx = LusActiveConnIdx;
            break;
        }
    }
 

    if (E_OK == LddReturnValue)
    {
        if (LddConnectionType == 
                Tls_GaaConnection[LddConnectionId].ddTlsConnectionType)
        {
            if (TLS_CONNECTION_NOT_INITIATED ==
                                    Tls_GddConnHSStatus[LddConnectionId])
            {
                #if (STD_ON == TLS_GROUP_CONFIGURED)
                    if (TLS_TRUE == 
                        Tls_GaaConnection[LddConnectionId].blTlsIsGrpEnbld)
                    {
                        LusGroupId = Tls_GaaGroupArray[LddConnectionId];
                        if (Tls_GddGroups[LusGroupId] == 
                                            TLS_GROUP_CONNECTION_INACTIVE)
                        {     
                            Tls_GddGroups[LusGroupId] = 
                                                TLS_GROUP_CONNECTION_ACTIVE;
                
                            LddReturnValue = E_OK;
                        }
                        else
                        {
                            LddReturnValue = E_NOT_OK;
                        }
                    }
                    /* if connection is not configured with group */
                    else
                    {
                        LddReturnValue = E_OK;
                    }
                #else
                    LddReturnValue = E_OK;
                #endif    
            }
            else
            {
                LddReturnValue = E_NOT_OK;
            }
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
** Function Name        : Tls_GetActIdx                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : the function gets the index value of the Tls conn ID   **
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
**  Design ID           : TLS_SDD_0249                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
uint16  Tls_GetActIdx(const TcpIp_TlsConnectionIdType LddConnectionId)
{
    uint16 LusActiveConnIdx;
    
    for (LusActiveConnIdx = (uint16)TLS_ZERO;
            LusActiveConnIdx < TLS_MAX_ACTIVE_CONNECTIONS; LusActiveConnIdx++)
    {
        if (Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId == 
                                                                LddConnectionId)
        {
            break;
        }
    }
    return LusActiveConnIdx;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_CheckCHTag                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : the function checks if a TAG of an extension is a know Tag   **
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
**  Design ID           : TLS_SDD_XXXX                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_CheckCHTag(const uint16 LusTag)
{     
    Std_ReturnType  LddReturnValue;                                               
		
	/* as per tabel 4.2 in Rfc 8446 */
	switch (LusTag)
	{
		case (uint16)TLS_SUPPORTED_VERSIONS:
		case (uint16)TLS_CH_SUPPORTED_GROUPS:
		case (uint16)TLS_CH_SIGNATURE_ALGORITHMS:
		case (uint16)TLS_CH_KEY_SHARE:
		case (uint16)TLS_CH_PSK_KEY_EXCHANGE_MODES:
		case (uint16)TLS_PRE_SHARED_KEY:   
		case (uint16)TLS_CH_SERVER_NAME:
			LddReturnValue = E_OK;
			break;
		default:
			LddReturnValue = E_NOT_OK;
			break;
	}
	
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_CheckSHTag                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : the function checks if a TAG of an extension is a know Tag   **
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
**  Design ID           : TLS_SDD_XXXX                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_CheckSHTag(const uint16 LusTag)
{     
    Std_ReturnType  LddReturnValue;                                               
		
	/* as per tabel 4.2 in Rfc 8446 */
	switch (LusTag)
	{
		case (uint16)TLS_SUPPORTED_VERSIONS:
		case (uint16)TLS_CH_KEY_SHARE:
		case (uint16)TLS_PRE_SHARED_KEY:   
			LddReturnValue = E_OK;
			break;
		default:
			LddReturnValue = E_NOT_OK;
			break;
	}
	
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_CheckCRTag                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : the function checks if a TAG of an extension is a know Tag   **
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
**  Design ID           : TLS_SDD_XXXX                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_CheckCRTag(const uint16 LusTag)
{     
    Std_ReturnType  LddReturnValue;                                               
		
	/* as per tabel 4.2 in Rfc 8446 */
	switch (LusTag)
	{
		case (uint16)TLS_STATUS_REQUEST:
		case (uint16)TLS_CH_SIGNATURE_ALGORITHMS: 
		case (uint16)TLS_SIGNED_CERT_TIMESTAMP:
		case (uint16)TLS_CERTIFICATE_AUTHORITIES: 
		case (uint16)TLS_OID_FILTERS:
		case (uint16)TLS_SIGNATURE_ALGORITHMS_CERT:
			LddReturnValue = E_OK;
			break;
		default:
			LddReturnValue = E_NOT_OK;
			break;
	}
	
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_CheckSupportedTag                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : the function checks if a TAG of an extension is a know Tag   **
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
**  Design ID           : TLS_SDD_0250                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_CheckSupportedTag(
                                const Tls_HandShakeProtocolType LddHSMsgType,
                                const uint8* const LpHShakeBuf)
{
  
    /* The table below indicates the messages where a given extension may
    appear, using the following notation: CH (ClientHello),
    SH (ServerHello), EE (EncryptedExtensions), CT (Certificate),
    CR (CertificateRequest), NST (NewSessionTicket), and
    HRR (HelloRetryRequest).  If an implementation receives an extension
    which it recognizes and which is not specified for the message in
    which it appears, it MUST abort the handshake with an
    "illegal_parameter" alert. */
    
    Std_ReturnType  LddReturnValue;
    uint16 LusTag;
    
    LusTag = ((uint16)((uint16)LpHShakeBuf[TLS_ZERO] << (uint16)TLS_EIGHT) | 
                                                (uint16)LpHShakeBuf[TLS_ONE]); 
                                                
    if (TLS_CLIENT_HELLO == LddHSMsgType)
    {
		LddReturnValue = Tls_CheckCHTag(LusTag);
    }
    else if (TLS_SERVER_HELLO == LddHSMsgType)
    {
		LddReturnValue = Tls_CheckSHTag(LusTag);
    }
    else if (TLS_CERTIFICATE_REQUEST == LddHSMsgType)
    {
		LddReturnValue = Tls_CheckCRTag(LusTag);
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
** Function Name        : Tls_GetLValue                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : the function gets the L value, Based on the negotiated cipher suite   **
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
**  Design ID           : TLS_SDD_0251                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
uint16 Tls_GetLValue(const uint16 LusNegotitdCiprSutId)
{
    uint16  LusLValue;
    
    switch (LusNegotitdCiprSutId)
    {
        case (uint16)TLS_AES_128_GCM_SHA256:
        case (uint16)TLS_AES_128_CCM_SHA256:
        case (uint16)TLS_AES_128_CCM_8_SHA256:
            LusLValue = (uint16)TLS_SIXTEEN;
            break;
        
        case (uint16)TLS_AES_256_GCM_SHA384:
        case (uint16)TLS_CHACHA20_POLY1305_SHA256:
            LusLValue = (uint16)TLS_THIRTYTWO;
            break;
        default:
            LusLValue = (uint16)TLS_ZERO;
            break;
    }
    return LusLValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_ConstructNonce                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : the function gets the L value, Based on the negotiated cipher suite   **
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
**  Design ID           : TLS_SDD_0253                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType Tls_ConstructNonce(const uint64 LullSeqNum,
                                    const uint32 LulWriteIvKeyId,
                                    uint8* const LpNounce)
{
    Std_ReturnType  LddReturnValue;
    uint8 LucCount;
    
    
    /* padd sequence number to 12 bytes (IV length ) , 
    Before hand taken care here */
    uint8 LaaPaddedSequence[TLS_TWELVE];
    
    /* RFC 8446: "Section 5.2" */
    if (LullSeqNum == TLS_RECORD_SEQ_NUM_MAX)
    {
        /* Must rekey or terminate before overflow */
        LddReturnValue = E_NOT_OK; 
    }
    else
    {
        Tls_MemSet(&LaaPaddedSequence[TLS_ZERO], (uint8)TLS_ZERO, 
                                                            (uint16)TLS_TWELVE);

        for (LucCount = (uint8)TLS_ZERO; LucCount < (uint8)TLS_EIGHT; LucCount++)
        {
            LaaPaddedSequence[TLS_FOUR + LucCount] = 
                (uint8)(LullSeqNum >> (TLS_FIFTYSIX - (TLS_EIGHT * LucCount)));
        }
        
        /* XOR with (write_iv) */       
        LddReturnValue = Tls_PerformXor(LulWriteIvKeyId,
                                        &LaaPaddedSequence[TLS_ZERO],
                                        LpNounce);
                                        
    }
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_GetExtensionLength                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : the function checks if a TAG of an extension is a know Tag   **
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
**  Design ID           : TLS_SDD_0252                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
uint16  Tls_GetExtensionLength(const uint8* const LpHShakeBuf)
{
    return ((uint16)((uint16)LpHShakeBuf[TLS_ZERO] << (uint16)TLS_EIGHT) | 
                                                (uint16)LpHShakeBuf[TLS_ONE]); 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PerformCloseConnAction                                       **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : To update the states during internal close action   **
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
**  Design ID           : TLS_SDD_0255                                       **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_PerformCloseConnAction(const TcpIp_TlsConnectionIdType LddConnectionId)
{
    Std_ReturnType  LddReturnValue;
    uint16 LusActiveConnIdx;
    #if (STD_ON == TLS_GROUP_CONFIGURED)
    uint16  LusGroupId;
    #endif
  
    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);

    if (TLS_UNKNOWN_ALERT_DESCRIPTION != Tls_GaaAlert[LusActiveConnIdx]) // parasoft-suppress CERT_C-ARR30-a-2 CERT_C-ARR39-a-2 CERT_C-EXP08-b-2 CERT_C-FIO37-a-1 CERT_C-MSC19-a-3 CERT_C-STR31-a-1 MISRAC2012-DIR_4_1-a-2 MISRAC2012-RULE_18_1-a-2 "Reason: Boundary values are validated before use and verified manually.
    {
        LddReturnValue = 
            Tls_ProcessAlert(LddConnectionId, Tls_GaaAlert[LusActiveConnIdx]);
        
        if (E_OK == LddReturnValue)
        {
            Tls_GaaAlert[LusActiveConnIdx] = TLS_UNKNOWN_ALERT_DESCRIPTION;
        }
    }
    else
    {

        /* call function to destry keys */
        Tls_DestroyKeys(LddConnectionId);

        Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_NOT_INITIATED;
    
        if (Tls_GaaConnection[LddConnectionId].ddTlsConnectionType == TLS_CLIENT)
        {
            Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_START;
        }
        else
        {
            Tls_GaaServerState[LusActiveConnIdx] = TLS_SERVER_WAIT_FOR_CH;
        }
    
        Tls_GaaHandshakeElementIndex[LusActiveConnIdx] = TLS_ZERO;
        Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId = 
                                                        TLS_INVALID_CONNECTION;
        Tls_GddActiveConnManager[LusActiveConnIdx].ddConnectionState = 
                                                        TLS_CONNECTION_INACTIVE;
        
        Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition = TLS_ZERO; 
        Tls_HandshakeFrame[LusActiveConnIdx].ulWriteSeqNum = (uint64)TLS_ZERO;    
        Tls_HandshakeFrame[LusActiveConnIdx].ulReadSeqNum = (uint64)TLS_ZERO;
        Tls_HandshakeFrame[LusActiveConnIdx].blIsHSKeysDerived = TLS_FALSE;
        Tls_HandshakeFrame[LusActiveConnIdx].blIsAppKeysDerived = TLS_FALSE;
        Tls_HandshakeFrame[LusActiveConnIdx].blIsTxInProgress = TLS_FALSE;
        Tls_HandshakeFrame[LusActiveConnIdx].usTotlAppData = (uint16)TLS_ZERO;
        Tls_HandshakeFrame[LusActiveConnIdx].usProcessedData = (uint16)TLS_ZERO;
        Tls_HandshakeFrame[LusActiveConnIdx].usUnProcessedData = (uint16)TLS_ZERO;
		Tls_HandshakeFrame[LusActiveConnIdx].blIsCloseConnAfterHS = TLS_FALSE;
		#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
		Tls_HandshakeFrame[LusActiveConnIdx].ddVerifyCertState = TLS_IDLE;
		Tls_HandshakeFrame[LusActiveConnIdx].blIsCertReqReceived = TLS_FALSE;
		#endif
        Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedSupGrpIdx = 
                                                    (uint16)TLS_DEFAULT_VALUE;
        Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedSigAlgoIdx = 
                                                    (uint16)TLS_DEFAULT_VALUE;
    
        /* Clear Queue */
        Tls_HandshakeFrame[LusActiveConnIdx].ddQueue.usQHead = (uint16)TLS_ZERO;
        Tls_HandshakeFrame[LusActiveConnIdx].ddQueue.usQTail = (uint16)TLS_ZERO;
        Tls_HandshakeFrame[LusActiveConnIdx].ddQueue.usTotlQData = (uint16)TLS_ZERO;
        
        Tls_GaaAlert[LusActiveConnIdx] = TLS_UNKNOWN_ALERT_DESCRIPTION;
    
        /* clear */
        Tls_MemSet(&Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO], 
                                    (uint8)TLS_ZERO, (uint16)TLS_MAX_BUFFER_SIZE);
                                    
        Tls_MemSet(&Tls_GaaAppBuffer[LusActiveConnIdx][TLS_ZERO], 
                                    (uint8)TLS_ZERO, (uint16)TLS_MAX_FRAGMENT_SIZE);
    
        Tls_MemSet(&Tls_GaaBuffer[LusActiveConnIdx][TLS_ZERO], 
                                    (uint8)TLS_ZERO, (uint16)TLS_MAX_BUFFER_SIZE);
                                    
        Tls_MemSet(&Tls_GaaRecAppBuffer[LusActiveConnIdx][TLS_ZERO], 
                                    (uint8)TLS_ZERO, (uint16)TLS_MAX_FRAGMENT_SIZE);
    
        #if (STD_ON == TLS_TIMER_SUPPORT)
            Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
                                                            (uint16)TLS_ZERO;
        #endif
    
        #if (STD_ON == TLS_GROUP_CONFIGURED)
            if (TLS_TRUE == Tls_GaaConnection[LddConnectionId].blTlsIsGrpEnbld)
            {
                LusGroupId = Tls_GaaGroupArray[LddConnectionId];
                if (Tls_GddGroups[LusGroupId] == TLS_GROUP_CONNECTION_ACTIVE)
                {     
                    Tls_GddGroups[LusGroupId] = TLS_GROUP_CONNECTION_INACTIVE;
                }
            }
        #endif
    }
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_ProcessHandshakeRecord                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          :                                                     **
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
**  Design ID           : TLS_SDD_0256                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ProcessHandshakeRecord (
                                const uint16 LusActiveConnIdx,
                                const Tls_ConnectionType LddTlsConnectionType,
                                const uint8* const LpDataPtr,
                                const uint16  LusTotalDataLength)
{
    Std_ReturnType  LddReturnValue;
    Tls_FrameType*  LpTlsFrame;
    uint8  LucState;
    uint16 LusHShakeBufferPosition; 

    
        /* Transition matrix: [Connection Type][Current State] to Next State */
    static const uint8 Tls_SubstateMatrix[TLS_TWO][TLS_EIGHT] = 
    {
        /* CLIENT transitions */
        {
            /* Current State : TLS_CLIENT_START */ (uint8)TLS_INVALID,
            /* Current State : TLS_CLIENT_WAIT_FOR_SH */ (uint8)TLS_CLIENT_RECVD_SH,
            /* Current State : TLS_CLIENT_RECVD_SH */ (uint8)TLS_INVALID,
            /* Current State : TLS_CLIENT_WAIT_FOR_EE */ (uint8)TLS_CLIENT_RECVD_EE,
            /* Current State : TLS_CLIENT_RECVD_EE */ (uint8)TLS_INVALID,
            /* Current State : TLS_CLIENT_WAIT_FOR_FINSHED */ (uint8)TLS_CLIENT_RECVD_FINSHED,
            /* Current State : TLS_CLIENT_RECVD_FINSHED */ (uint8)TLS_INVALID,
            /* Current State : TLS_CLIENT_CONNECTED */ (uint8)TLS_INVALID        
    
        },
        /* SERVER transitions */
        {
            /* Current State : TLS_SERVER_WAIT_FOR_CH */ (uint8)TLS_SERVER_RECVD_CH,
            /* Current State : TLS_SERVER_RECVD_CH */ (uint8)TLS_INVALID,
            /* Current State : TLS_SERVER_NEGOTIATED */ (uint8)TLS_INVALID,
            /* Current State : TLS_SERVER_WAIT_FOR_FINISHED */ (uint8)TLS_SERVER_RECVD_FINISHED,
            /* Current State : TLS_SERVER_RECVD_FINISHED */ (uint8)TLS_INVALID,
            /* Current State : TLS_SERVER_CONNECTED */ (uint8)TLS_INVALID,
            /* Current State : TLS_INVALID */ (uint8)TLS_INVALID,
            /* Current State : TLS_INVALID */ (uint8)TLS_INVALID
        }
    }; 

    LddReturnValue = E_OK;

    LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];            
    
    if (TLS_CLIENT == LddTlsConnectionType)
    {
      LucState =  (uint8)Tls_GaaClientState[LusActiveConnIdx];
    }
    else
    {
      LucState =  (uint8)Tls_GaaServerState[LusActiveConnIdx]; 
    }
    /* bascially this tells us, where every HS msg ends */
    LusHShakeBufferPosition = LpTlsFrame->usHShakeBufferPosition;
    
    if (((LusHShakeBufferPosition + LusTotalDataLength) < TLS_MAX_BUFFER_SIZE)
        && ((uint8)TLS_INVALID != Tls_SubstateMatrix[LddTlsConnectionType]
                                                                    [LucState]))
    {  
        /* Store the handshake data in the buffer */
        Tls_MemCpy(
        &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusHShakeBufferPosition],
        &LpDataPtr[TLS_ZERO],
        LusTotalDataLength);
        
        /* Update the position in the handshake buffer */
        /* store without Record Hdr bytes */
        LpTlsFrame->usHShakeBufferPosition += LusTotalDataLength;
    
        if (TLS_CLIENT == LddTlsConnectionType)
        {
            Tls_GaaClientState[LusActiveConnIdx] = 
                    (Tls_ClientStateType)Tls_SubstateMatrix[ // parasoft-suppress MISRAC2012-RULE_10_5-a-4 "Reason:  Tls_Internal_c_REF_1"
                                                LddTlsConnectionType][LucState];
        }
        else
        {
            Tls_GaaServerState[LusActiveConnIdx] = 
            (Tls_ServerStateType)Tls_SubstateMatrix[ // parasoft-suppress MISRAC2012-RULE_10_5-a-4 "Reason:  Tls_Internal_c_REF_1"
                                        LddTlsConnectionType][LucState];                   
        }
    }
    else
    {
        /* Because TLS 1.3 forbids renegotiation, if a server has negotiated
        TLS 1.3 and receives a ClientHello at any other time, it MUST
        terminate the connection with an "unexpected_message" alert. */
        
        Tls_GaaAlert[LusActiveConnIdx] = TLS_UNEXPECTED_MESSAGE;
        LddReturnValue = E_NOT_OK;  
    }           
            
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_ProcessAlertRecord                              **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          :                                                     **
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
**  Design ID           : TLS_SDD_0258                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ProcessAlertRecord (
                                const TcpIp_TlsConnectionIdType LddConnectionId,
                                const uint8* const LucDataPtr,
                                const uint16  LusTotalDataLength)
{
    Std_ReturnType  LddReturnValue;

	/* The length after removing that Record type at the 
                    end post decryption*/
    if (LusTotalDataLength == (uint8)TLS_TWO)
	{
		/* Check for Alert Level */
		if (LucDataPtr[TLS_ZERO] == (uint8)TLS_WARNING)
        {
           /* Check for Alert Description */
		   if (LucDataPtr[TLS_ONE] == (uint8)TLS_USER_CANCELED)
			{
		      /* based on our design , If TLS_USER_CANCELED
			  is received it can be being ignored */
              
              LddReturnValue = E_OK;  
			}
			
			else if (LucDataPtr[TLS_ONE] == (uint8)TLS_CLOSE_NOTIFY)
			{
		      /* move the connection status to close state */
              Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_CLOSED;
              LddReturnValue = E_OK;  
			}
			
            else
            {
                LddReturnValue = E_NOT_OK;  
            }			
            
        }
		/* Check for Alert Level */
	    else if (LucDataPtr[TLS_ZERO] == (uint8)TLS_FATAL)
	    {
            if (TLS_CONNECTION_IN_PROGRESS == 
                                        Tls_GddConnHSStatus[LddConnectionId])
            {
                /* our design */
                Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
                Tls_ConnStatsNotification(LddConnectionId,
                                                        TLS_CONNECTION_FAILED);                    
            }
            else
            {
                Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_ABORTED;
                Tls_ConnStatsNotification(LddConnectionId,
                                                        TLS_CONNECTION_ABORTED);                     
            } 
            LddReturnValue = E_OK;
	    }
        else
        {
            LddReturnValue = E_NOT_OK;  
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
** Function Name        : Tls_PrepareFinishVerifyData                         **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares server finished message.     **
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
**  Design ID           : TLS_SDD_0260                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_PrepareFinishVerifyData(const uint16 LusActiveConnIdx,
                                            const uint32 LulSrcHSTrficSecrt)
{
    
    Std_ReturnType  LddReturnValue;
    uint32 LulHashingJobId;
    uint32 LulMacJobId;
    uint32 LulMacKeyId;
    uint32 LulFinishedKey;
    uint32 LulOutHashLen;
    uint32 LulOutHMacLen;
    uint16 LusTotlInLen;
	uint16 LusTemp;
    uint8* LpHSShakeBuf;
    uint16 LusNegotitdCiprSutId;
    uint8 LucChoosenHashLen;
    uint8 LaaOutHash[TLS_MAX_HASH_SIZE];
    uint8 LaaOutHMAC[TLS_MAX_HASH_SIZE];
       

    /* from here we will get to know , how many bytes in total,
    Server : CH + SH + EE OR
    client : CH + SH + EE + SF
    depending on wether this was called by server or client 
    that many all till now recived messages to be hased */
    LusTotlInLen = 
                Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition;
    LpHSShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO];   

    LulHashingJobId = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHashVerifyJobRefIdx; 
    
    /* get the negotaited cipher suit from server side */
    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
    Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
    ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId; 
    
    LucChoosenHashLen = 
    (LusNegotitdCiprSutId == (uint16)TLS_AES_256_GCM_SHA384) ? 
    (uint8)TLS_FOURTYEIGHT : (uint8)TLS_THIRTYTWO;
    
    LulOutHashLen = (uint32)LucChoosenHashLen;
    
        
    LddReturnValue = Tls_PerformHash(                                        
    /* JobId to perform Hash */
                                    LulHashingJobId,
    /* Contains the pointer to the data for which the hash shall be computed */                                    
                                    LpHSShakeBuf,
    /* Contains the number of bytes to be hashed */                                 
                                    (uint32)LusTotlInLen,
    /* Contains the pointer to the data where the hash value shall be stored */                                    
                                    &LaaOutHash[TLS_ZERO],
    /* Holds a pointer to the memory location in which the output length in bytes
    is stored */                                
                                    &LulOutHashLen);

    if ((E_OK == LddReturnValue) && 
        /* Csm Hash shd return the choosen cipher amount of bytes */
        (LulOutHashLen == (uint32)LucChoosenHashLen))
        
    {
        LddReturnValue = Tls_DeriveFinishedKey(LusActiveConnIdx, 
                                                            LulSrcHSTrficSecrt);
                                                           
        if (E_OK == LddReturnValue)
        {
            LulMacJobId = 
            Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[LusActiveConnIdx].
                    usNegotiatedCiprSutIdx].ulTlsCipherCsmMacGenerateJobRefIdx;
        
            LulMacKeyId = 
            Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[LusActiveConnIdx].
                    usNegotiatedCiprSutIdx].ulTlsCipherCsmMacGenerateKeyRefIdx;

            /* Currenlty in this Key slot only early secret was stored at the start of derive ES 
            but then after that , the slot becomes uselss, so reusing the slot for to store finish key    */
            LulFinishedKey = 
            Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
            LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
            ].ulTlsCsmEarlySecretKeyRefIdx; 
    
            LulOutHMacLen = (uint32)LucChoosenHashLen;
        
            LddReturnValue = Tls_PerformMacGenerate(
            /* JobId to perform Mac Generate */
                                            LulMacJobId,
            /* KeyId to be used in pair the Mac Job id to perform Mac Generate */
                                            LulMacKeyId,
            /* KeyId to be used to copy into Mac Key Id */
                                            LulFinishedKey,
            /* Contains the pointer to the data for which the MAC shall be computed */                                       
                                            &LaaOutHash[TLS_ZERO],
            /* Contains the number of bytes to be Hmaced */                                        
                                            LulOutHashLen,
            /* Contains the pointer to the data where the MAC shall be stored */                                
                                            &LaaOutHMAC[TLS_ZERO],
            /* Holds a pointer to the memory location in which the output 
            length in bytes is stored */                                        
                                            &LulOutHMacLen);
                                        
            if ((E_OK == LddReturnValue) && 
                /* Csm Hash shd return the choosen cipher amount of bytes */
                (LulOutHMacLen == (uint32)LucChoosenHashLen))
            
            {
                /* updated HandShake Header */
                /* get the location where to store the Finished Message 
                resuing the old local varibales itself, if required , can intro new
                local variables for readablitiy */
                LpHSShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusTotlInLen];   
        
                LpHSShakeBuf[TLS_ZERO] = (uint8)TLS_FINISHED;  
                LpHSShakeBuf[TLS_ONE] = (uint8)TLS_ZERO;
                LpHSShakeBuf[TLS_TWO] = (uint8)TLS_ZERO;
                LpHSShakeBuf[TLS_THREE] = LucChoosenHashLen;
                /* copy the hmac into HS buffer (Verify Data) */
                Tls_MemCpy(&LpHSShakeBuf[TLS_FOUR], &LaaOutHMAC[TLS_ZERO],
                                                    (uint16)LucChoosenHashLen);
                    
                /* no matter client or server , Verify data will always either be 
                32 or 48 bytes , so 
                now that CF or SF is stored from usHShakeBufferPosition indexth 
                so this can be genralized in next operation */
				/* updating this here only , since in handshake.c again
				we have fetch the choosen Hash */
		
				LusTemp = (uint16)TLS_FOUR + (uint16)LucChoosenHashLen;
		
				Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition += 
																		LusTemp;
            }
            else
            {
                LddReturnValue = E_NOT_OK;
            }             
        }                   
    }
    else
    {
        LddReturnValue = E_NOT_OK;
    }

    /* if any crypto operataions failed */
    if (E_NOT_OK == LddReturnValue)
    {
        Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
    }
                                        
    return LddReturnValue;
}/* end of Tls_PrepareFinishVerifyData */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_ProcessFinishedMessage                         **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares server or Client finished message.     **
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
**  Design ID           : TLS_SDD_0295                                         **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType Tls_ProcessFinishedMessage(const uint16 LusActiveConnIdx)
{
    /*
        finished_key =
        HKDF-Expand-Label(BaseKey, "finished", "", Hash.length)
       
        verify_data =
          HMAC(finished_key,
               Transcript-Hash(Handshake Context,
                               Certificate*, CertificateVerify*))
    */
    Std_ReturnType  LddReturnValue;
    TcpIp_TlsConnectionIdType LddConnectionId;
    
    uint32 LulSrcHSTrficSecrt;
    
    LddConnectionId = 
    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;

    if (Tls_GaaConnection[LddConnectionId].ddTlsConnectionType == TLS_CLIENT)
    {
        LulSrcHSTrficSecrt = 
        Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
        LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
        ].ulTlsCsmClientHSTrafficSecretKeyRefIdx; 
    }
    else
    {
        LulSrcHSTrficSecrt = 
        Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
        LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
        ].ulTlsCsmServerHSTrafficSecretKeyRefIdx;       
    }

    
    LddReturnValue = 
        Tls_PrepareFinishVerifyData(LusActiveConnIdx, LulSrcHSTrficSecrt); 
    
    return LddReturnValue;
}/* end of Tls_ProcessFinishedMessage */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_VerifyFinishMessage                         **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares server or Client finished message.     **
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
**  Design ID           : TLS_SDD_0329                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType Tls_VerifyFinishMessage(const uint16 LusActiveConnIdx)
{
    /*
        finished_key =
        HKDF-Expand-Label(BaseKey, "finished", "", Hash.length)
       
        verify_data =
          HMAC(finished_key,
               Transcript-Hash(Handshake Context,
                               Certificate*, CertificateVerify*))
    */
    Std_ReturnType  LddReturnValue;
    TcpIp_TlsConnectionIdType LddConnectionId;
    uint8* LpActualFinMacPtr;
    uint8* LpHSShakeBuf;
    uint32 LulSrcHSTrficSecrt;
    uint32 LulHashingJobId;
    uint32 LulOutHashLen;
    uint32 LulVerifyJobId;
    uint32 LulVerifyKeyId;
    uint32 LulFinishedKey;
    uint16 LusNegotitdCiprSutId;
    uint16 LusLenToHash;
    uint16 LusFinishLen;
    uint8 LucChoosenHashLen;   
    uint8 LaaOutHash[TLS_MAX_HASH_SIZE];

    /* get the negotaited cipher suit from server side */
    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
    Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
    ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId; 
    
    LucChoosenHashLen = 
    (LusNegotitdCiprSutId == (uint16)TLS_AES_256_GCM_SHA384) ? 
    (uint8)TLS_FOURTYEIGHT : (uint8)TLS_THIRTYTWO;
    
    /* Verification from here we will get to know , how many bytes in total,
    server = CH + SH + EE + SF + CF OR
    client = CH + SH + EE + SF 
    depending on wether this was called by server or client 
    that many all till now recived messages to be hased */

    LusLenToHash = 
                Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition;
 
    /* now this will point till EE last byte if Client */
    /* now this will point till SF last byte if Server */    
    LusLenToHash -= (uint16)((uint16)TLS_FOUR + (uint16)LucChoosenHashLen);            
    
    /* should point to the HSHdr of CF or SF msg */
    LpHSShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusLenToHash];

    /* verify HandShake Headr of Finished Msg */
    if ((uint8)TLS_FINISHED == (*LpHSShakeBuf))
    {
        LusFinishLen = Tls_GetExtensionLength(&LpHSShakeBuf[TLS_ONE]);
        if ((uint16)TLS_ZERO == LusFinishLen)
        {
            if (LucChoosenHashLen == LpHSShakeBuf[TLS_THREE])
            {
                LpHSShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO];
                
                LulHashingJobId = 
                Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
                LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
                ].ulTlsCsmHashVerifyJobRefIdx;    

                LulOutHashLen = (uint32)LucChoosenHashLen;     

                LddReturnValue = Tls_PerformHash(                                        
                /* JobId to perform Hash */
                                                LulHashingJobId,
                /* Contains the pointer to the data for which the hash shall be computed */                                    
                                                LpHSShakeBuf,
                /* Contains the number of bytes to be hashed */                                 
                                                (uint32)LusLenToHash,
                /* Contains the pointer to the data where the hash value shall be stored */                                    
                                                &LaaOutHash[TLS_ZERO],
                /* Holds a pointer to the memory location in which the output length in bytes
                is stored */                                
                                                &LulOutHashLen);     

                if ((E_OK == LddReturnValue) && 
                    /* Csm Hash shd return the choosen cipher amount of bytes */
                    (LulOutHashLen == (uint32)LucChoosenHashLen))
                    
                {
                    LddConnectionId = 
                    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
    
                    if (Tls_GaaConnection[LddConnectionId].ddTlsConnectionType 
                                                                == TLS_CLIENT)
                    {
                        LulSrcHSTrficSecrt = 
                        Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
                        LusActiveConnIdx].usNegotiatedCiprSutIdx].
                        usTlsConnHandshakeRefBaseIndx].
                        ulTlsCsmServerHSTrafficSecretKeyRefIdx;
                        
                    }
                    else
                    {
                        LulSrcHSTrficSecrt = 
                        Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
                        LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
                        ].ulTlsCsmClientHSTrafficSecretKeyRefIdx;         
                    }
                    
                    LddReturnValue = Tls_DeriveFinishedKey(LusActiveConnIdx, 
                                                            LulSrcHSTrficSecrt);
                                                            
                    if (E_OK == LddReturnValue)
                    {
                        /* to make it point to exactly the verify data (HMAC) part of the CF or SF msg */
                        LpActualFinMacPtr = 
                        &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusLenToHash + (uint16)TLS_FOUR];
                        
                        LulVerifyJobId = 
                        Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[LusActiveConnIdx].
                                usNegotiatedCiprSutIdx].ulTlsCipherCsmMacVerifyJobRefIdx;
                    
                        LulVerifyKeyId = 
                        Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[LusActiveConnIdx].
                                usNegotiatedCiprSutIdx].ulTlsCipherCsmMacVerifyKeyRefIdx;
                        
                        /* Currenlty in this Key slot only early secret was stored at the start of derive ES 
                        but then after that , the slot becomes uselss, so reusing the slot for to store finish key    */
                        LulFinishedKey = 
                        Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
                        LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
                        ].ulTlsCsmEarlySecretKeyRefIdx; 
    
                        LddReturnValue = Tls_PerformMacVerify(
                        /* JobId to perform csm_macverify */
                                                        LulVerifyJobId,
                        /* KeyId to be used in pair the Verify Job id to perform Mac verify */
                                                        LulVerifyKeyId,
                        /* KeyId to be used to copy into Csm Verify Key Id */
                                                        LulFinishedKey,
                        /* Holds a pointer to the data for which the MAC shall be verified */                                       
                                                        &LaaOutHash[TLS_ZERO],
                        /*Contains the number of data bytes for which the MAC shall beverified*/                                        
                                                        LulOutHashLen,
                        /* Holds a pointer to the MAC to be verified */                                       
                                                        &LpActualFinMacPtr[TLS_ZERO],
                        /* Contains the MAC length in BITS to be verified */                                        
                                                        LulOutHashLen);   
                                    
                        if (E_NOT_OK == LddReturnValue)
                        {
                            /* Recipients of Finished messages MUST verify that the contents are
                            correct and if incorrect MUST terminate the connection with a
                            "decrypt_error" alert. */
                            Tls_GaaAlert[LusActiveConnIdx] = TLS_DECRYPT_ERROR;
                        }
                    }
                    else
                    {
                        Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
                    }
                }
                else
                {
                    Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
                    LddReturnValue = E_NOT_OK;
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
}/* end of Tls_VerifyFinishMessage */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_ProcessAlert                         **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares server or Client finished message.     **
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
**  Design ID           : TLS_SDD_0257                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType Tls_ProcessAlert(const TcpIp_TlsConnectionIdType  LddConnectionId, 
                                        const Tls_AlertDescription LddAlert)
{
    Std_ReturnType  LddReturnValue;
    Tls_ContentType LddContentType;
    uint16 LusActiveConnIdx;
    uint16 LusPlainTxtLen;
    /* Alert 2 bytes + 1 byte ContentType */
    uint8 LaaInnerPlainAlert[TLS_THREE];
    boolean LblHSKeysDerived;
    boolean LblAppKeysDerived;
    
    
    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);    
    LblHSKeysDerived = Tls_HandshakeFrame[LusActiveConnIdx].blIsHSKeysDerived;
    LblAppKeysDerived = Tls_HandshakeFrame[LusActiveConnIdx].blIsAppKeysDerived;
    
    if ((TLS_CLOSE_NOTIFY == LddAlert) || (TLS_USER_CANCELED == LddAlert))
    {
        /* prepare close notify or user cancled as per conn in progress or ready */
        LaaInnerPlainAlert[TLS_ZERO] = (uint8)TLS_WARNING;          
    }
    else
    {
        /* prepare alert message and tranmsit */
        LaaInnerPlainAlert[TLS_ZERO] = (uint8)TLS_FATAL;       
    }
    

    LaaInnerPlainAlert[TLS_ONE] = (uint8)LddAlert;    
    LaaInnerPlainAlert[TLS_TWO] = (uint8)TLS_ALERT;
    
    /* just checking wether HS keys are derived are not is enough , since
    App keys wont be geenrated if HS keys itself are not generated */
    if (LblHSKeysDerived || LblAppKeysDerived)
    {
        LddContentType = TLS_APPLICATION_DATA;  
        LusPlainTxtLen =   (uint16)TLS_THREE;   
    }
    else
    {
        LddContentType = TLS_ALERT; 
        LusPlainTxtLen =   (uint16)TLS_TWO;
    } 

    LddReturnValue = Tls_TransmitRecord(
                                            LusActiveConnIdx,
                                            LddContentType,
                                            &LaaInnerPlainAlert[TLS_ZERO],
                                            LusPlainTxtLen);  
                         
    return LddReturnValue;
}/* end of Tls_ProcessAlert */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_ProcessCloseConn                         **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares server or Client finished message.     **
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
**  Design ID           : TLS_SDD_0261                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_ProcessCloseConn(const TcpIp_TlsConnectionIdType LddConnectionId)
{
    Std_ReturnType  LddReturnValue;

    LddReturnValue = Tls_ProcessAlert(LddConnectionId, TLS_CLOSE_NOTIFY);
    
    if (E_OK == LddReturnValue)
    {
        /* send Notification to tctpip back as per design */
        Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_CLOSED);
        
        /* call function to destry keys */
        
        Tls_DestroyKeys(LddConnectionId);
        /* call function to destry keys */
        
        Tls_PerformCloseConnAction(LddConnectionId);
    }                        
}/* end of Tls_ProcessCloseConn */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_DestroyKeys                         **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares server or Client finished message.     **
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
**  Design ID           : TLS_SDD_0262                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_DestroyKeys(const TcpIp_TlsConnectionIdType LddConnectionId)
{
    uint16 LusActiveConnIdx;
    uint8  LaaDestroyKeyMask[TLS_DESTROY_ARR_SIZE];
    uint32  LaaAllKeys[TLS_USED_KEYS] = {(uint32)TLS_ZERO};
    uint8  LucTotlNumKeys;
    uint8  LucNumOfPsks;
    uint8  LucCount;
    uint8  LucbyteIndex;
    uint8  LucbitIndex;
    uint8 LucBitMaskSize;
    
    LucBitMaskSize = (uint8)TLS_ZERO;
    
    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);
    
    #if (STD_ON == TLS_PSK_IDENTITY_SUPPORT_ENABLED)
        LucNumOfPsks = Tls_GaaConnection[LddConnectionId].ucNumOfPreSharedKeys;
    #else
        LucNumOfPsks = (uint8)TLS_ZERO;
    #endif

    
    LucTotlNumKeys = (uint8)TLS_EIGHTEEN + LucNumOfPsks;
    
    Tls_MemSet(&LaaDestroyKeyMask[TLS_ZERO], (uint8)TLS_ZERO, 
                                                (uint16)TLS_DESTROY_ARR_SIZE);
                                               
/* fetch all keys */
    for (LucCount = (uint8)TLS_ZERO; LucCount < LucNumOfPsks; LucCount++)
    {
        #if (STD_ON == TLS_PSK_IDENTITY_SUPPORT_ENABLED)
        LaaAllKeys[LucCount] = 
                            Tls_GaaPskIdentity[LucCount].ulTlsCsmBinderKeyRefId;
        #endif
    }
    
    LaaAllKeys[LucCount] = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmEarlySecretKeyRefIdx;
    LucCount++;
    
    LaaAllKeys[LucCount] = 
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExpandKeyRefIdx; 
    LucCount++;

    LaaAllKeys[LucCount] =  
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmDerivedKeyRefIdx; 
    LucCount++;

    LaaAllKeys[LucCount] =  
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHKDFExtractKeyRefIdx; 
    LucCount++;

    LaaAllKeys[LucCount] =  
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHandshakeSecretKeyRefIdx;
    LucCount++;

    LaaAllKeys[LucCount] =  
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmClientHSTrafficSecretKeyRefIdx;  
    LucCount++;

    LaaAllKeys[LucCount] =  
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmServerHSTrafficSecretKeyRefIdx;  
    LucCount++;

    LaaAllKeys[LucCount] =  
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmClientHandshakeWritekeyRefIdx; 
    LucCount++;
    
    LaaAllKeys[LucCount] =  
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmServerHandshakeWritekeyRefIdx; 
    LucCount++;
    
    LaaAllKeys[LucCount] =  
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmClientHandshakeWriteIVKeyRefIdx; 
    LucCount++;
    
    LaaAllKeys[LucCount] =  
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmServerHandshakeWriteIVKeyRefIdx;   
    LucCount++;

    LaaAllKeys[LucCount] =  
    Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmMasterSecretKeyRefIdx;  
    LucCount++;

    LaaAllKeys[LucCount] =  
    Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].ulTlsCsmClientApplTrafficSecretKeyRefIdx;  
    LucCount++;

    LaaAllKeys[LucCount] =  
    Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].ulTlsCsmServerApplTrafficSecretKeyRefIdx;
    LucCount++;

    LaaAllKeys[LucCount] =  
    Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].ulTlsCsmClientApplWriteKeyRefIdx; 
    LucCount++;

    LaaAllKeys[LucCount] =  
    Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].ulTlsCsmServerApplWriteKeyRefIdx; 
    LucCount++;

    LaaAllKeys[LucCount] =  
    Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].ulTlsCsmClientApplWriteIVRefIdx; 
    LucCount++;

    LaaAllKeys[LucCount] =  
    Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
    LusActiveConnIdx].usNegotiatedCiprSutIdx].ulTlsCsmServerApplWriteIVRefIdx;
    
    /* encoding */
    for (LucCount = (uint8)TLS_ZERO; LucCount < LucTotlNumKeys; LucCount++)
    {
        LucbyteIndex = (uint8)(LaaAllKeys[LucCount] >> TLS_THREE);
        /* needed to type caset , before parasoft just does not understand it
        although tasking understood it, since here value will never go beyond 7
        even though its a 32 bit value */
        LucbitIndex = (uint8)(LaaAllKeys[LucCount] & TLS_SEVEN);
        LaaDestroyKeyMask[LucbyteIndex] |= (uint8)((uint8)TLS_ONE << LucbitIndex);
        
        /* this is needed becuz , we should know , what is the last byte index 
        of the LaaDestroyKeyMask, since we need to send the size of array also 
        to Perform_DestroyKeys */
        if (LucbyteIndex > LucBitMaskSize)
        {
            LucBitMaskSize = LucbyteIndex;
        }
    }
    
    Tls_PerformDestroyKeys(&LaaDestroyKeyMask[TLS_ZERO], 
                                            LucBitMaskSize + (uint8)TLS_ONE);
            
}/* end of Tls_DestroyKeys */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_HandleTxIndication                              **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          :                                                     **
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
**  Design ID           : TLS_SDD_0330, TLS_SDD_0343                          **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_HandleTxIndication (const uint16 LusActiveConnIdx)
{
    TcpIp_TlsConnectionIdType LddConnectionId;
    Std_ReturnType LddReturnValue;
    uint16 LusMaxFragmntLen;
    uint16 LusHShakeBufferPosition;
    uint16 LusTotalAppData;
    uint16 LusPlaintextLen;
    uint8* LpDataPtr;
    /* why max frag size + 1  ?
    1St scene : if 1000 bytes of data is recived , it will be fragmented based 
    on max frag of that connection 
    2nd : if recived data is less then max frag of the connnections, then also 
    the below array can accomodate it 
    3rd , also 1 byte of content byte should also be there */
    uint8 LaaInnerPlainText[TLS_MAX_FRAGMENT_SIZE + TLS_ONE];
 
    LddConnectionId = 
                Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
    /* Updated for the Bug since
		Application Data recived by TxIndication ,
		Return value was E_OK,
		but immediatly close connection is called for the same connection 
		since it was not going Handle RX idnication , and state was not changed 
		to CLOSED so updating here */
	if (Tls_HandshakeFrame[LusActiveConnIdx].blIsCloseConnAfterHS)
	{
		/* why set this even if directly we are calleding proces close conn
		chances are the proces close conn might fail , so again once main
		function is called , it will automatically go to proces close conn */
		Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_CLOSED;   
		Tls_ProcessCloseConn(LddConnectionId);
	}
	else
	{
    /* gather all the required data */
    LusHShakeBufferPosition = 
         Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition;
         
    LusMaxFragmntLen = Tls_GaaConnection[LddConnectionId].usTlsMaxFragmentLen;
    
    LusTotalAppData = Tls_HandshakeFrame[LusActiveConnIdx].usTotlAppData;

    LpDataPtr = &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusHShakeBufferPosition];
    
    if ((uint16)(LusTotalAppData - LusHShakeBufferPosition) > LusMaxFragmntLen)
    {
        Tls_MemCpy(&LaaInnerPlainText[TLS_ZERO], LpDataPtr, LusMaxFragmntLen);
 
        LusPlaintextLen = LusMaxFragmntLen;
        LaaInnerPlainText[LusPlaintextLen] = (uint8)TLS_APPLICATION_DATA; 
         
    }
    /* only copy remaing data, or the recived bytes of data */
    else
    {
        Tls_MemCpy(&LaaInnerPlainText[TLS_ZERO], LpDataPtr, 
                                    LusTotalAppData - LusHShakeBufferPosition);
                                    
        LusPlaintextLen = LusTotalAppData - LusHShakeBufferPosition;   
        LaaInnerPlainText[LusPlaintextLen] = (uint8)TLS_APPLICATION_DATA;      
    }
    
    /* this will help us in quiting the work, 
    so that when next main function is called , nothing shd happen , if all the 
    data is processed */
    LusHShakeBufferPosition += LusPlaintextLen;  
    Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition = 
                                                        LusHShakeBufferPosition;  
                                                        
    LddReturnValue = Tls_TransmitRecord(
                                        LusActiveConnIdx,
                                        TLS_APPLICATION_DATA,
                                        &LaaInnerPlainText[TLS_ZERO],
                                        LusPlaintextLen + (uint16)TLS_ONE); 
    if (E_OK == LddReturnValue)
    {
        /* meaning there is no plain data to be proccessed */
        if (LusHShakeBufferPosition == LusTotalAppData)
        {
            /* reset Indication , such that next time HandleTxIndication shd not be invoked 
            and this connection shd accpet other Txindication 
            clear buffer also */
            /* clear */
            Tls_MemSet(&Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO], 
                                (uint8)TLS_ZERO, (uint16)TLS_MAX_BUFFER_SIZE);  

            Tls_MemSet(&Tls_GaaAppBuffer[LusActiveConnIdx][TLS_ZERO], 
                                (uint8)TLS_ZERO, (uint16)TLS_MAX_FRAGMENT_SIZE);
                                
            Tls_HandshakeFrame[LusActiveConnIdx].blIsTxInProgress = TLS_FALSE;
            Tls_HandshakeFrame[LusActiveConnIdx].usTotlAppData = 
                                                            (uint16)TLS_ZERO;           
                                                          
        }        
    }
    else
    {
        Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_ABORTED;        
    }
  }
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_HandleRxIndication                              **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          :                                                     **
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
**  Design ID           : TLS_SDD_0331, TLS_SDD_0344                          **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_HandleRxIndication (const uint16 LusActiveConnIdx)
{
    /* at any point of time 
    in Gaabuffer , either it can have an encrypted alert or encrypted aApplication
    so to check if any bytes are there in the buffer , the minimum amount of bytes
    which shd be there depends on the choosen cipher suite
    so if the Tag len is 8 , then mininmum amount of data to be present in the 
    buffer to consider it valid, is 5 + 8 + 0 + 1 = 14 
    if Tag is 16 , then it is 5 + 16 + 0 + 1 = 23 */
    Tls_QueueType* LddQue;
    TcpIp_TlsConnectionIdType LddConnectionId;
	Tls_FrameType* LpTlsFrame;
    Std_ReturnType LddReturnValue;
    uint8* LpEncrPtr;
    uint8* LpDecrPtr;
    uint16 LusNegotitdCiprSutId;
    uint16 LusRecordSize;
    uint16 LusDecryptDataLen;
    uint16 LusMaxFragmntLen;
    uint16 LusHShakeBufferPosition;
    uint16 LusComputeSize;
    uint8 LucChoosenTagLen;
    uint8 LaaRecHdr[TLS_FIVE];
	/* for Tasking warning  */
	LpDecrPtr = NULL_PTR;
	LddReturnValue = E_OK;
	
	LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];
	
    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
                LpTlsFrame->usNegotiatedCiprSutIdx].
                ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId;    

    LucChoosenTagLen = 
                (LusNegotitdCiprSutId == (uint16)TLS_AES_128_CCM_8_SHA256) ? 
                        (uint8)TLS_EIGHT : (uint8)TLS_SIXTEEN; 

	LddQue = &LpTlsFrame->ddQueue;
    
    LddConnectionId = 
				Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
					
    /* so instead of simply accesing the buffer, check if valid amount of data
    is present in the buffer */
    if (LddQue->usTotlQData >= 
                        ((uint16)((uint16)LucChoosenTagLen + (uint16)TLS_SIX)))
    {
		/* chances are there CF and Application data are recived at once ,
		so validate CF fully and Application Keys derive fully , then only start
		with decrypting Application data */		
		if (TLS_SERVER == Tls_GaaConnection[LddConnectionId].ddTlsConnectionType)
		{
			if (TLS_SERVER_RECVD_FINISHED == Tls_GaaServerState[LusActiveConnIdx])
			{
				LddReturnValue = E_NOT_OK;
			}
		}
        
		if (E_OK == LddReturnValue)
		{
			Tls_MemSet(&LaaRecHdr[TLS_ZERO], (uint8)TLS_ZERO, (uint16)TLS_FIVE);
	
			/* get the first five bytes of the record header */
			Tls_GetQRecordHdr(LusActiveConnIdx, &LaaRecHdr[TLS_ZERO]);
	
			/* check the 4th and 5th byte , to see how long is the record
			need to validate here, if its in Big indian, and do we need to convert it 
			back to little indian for processing  */
			LusRecordSize = 
					((uint16)((uint16)LaaRecHdr[TLS_THREE] << (uint16)TLS_EIGHT) | 
													(uint16)LaaRecHdr[TLS_FOUR]);
	
			LusRecordSize += (uint16)TLS_FIVE;
	
			/* this way i can make sure , that the required number of bytes are 
			there in the buffer , even though if some garbage bytes occure
			this check will help us in avoiding that, Basically to make sure 
			that many bytes is actually tehre in the buffer or not */
			if (LddQue->usTotlQData >= LusRecordSize)
			{
				LpEncrPtr = &Tls_GaaBuffer[LusActiveConnIdx][LddQue->usQTail];
	
				if (TLS_CONNECTION_IN_PROGRESS == Tls_GddConnHSStatus[LddConnectionId])
				{
					/* since here i need to know , from which index i shd correctly 
					start storing the handshake frame 
					Note : even if the decrypted output turns out to be an alert 
					we will still store in the Tls_GaaHShakeBuffer, since anyway 
					if it was an alert , we will close the connection in next main*/
	
					LusHShakeBufferPosition = LpTlsFrame->usHShakeBufferPosition;
	
                LusComputeSize = LusRecordSize;
                LusComputeSize = (uint16)(LusComputeSize  - (uint16)TLS_FIVE);
                LusComputeSize = 
                        (uint16)(LusComputeSize  - (uint16)LucChoosenTagLen);
					/* this is to pre check if the decrypted HS frame, can be stored
					in the HS buffer */
                if ((uint16)(LusHShakeBufferPosition + LusComputeSize)
                                                < (uint16)TLS_MAX_BUFFER_SIZE)
					{
						/* where to store , depends of what HS state is */
						LpDecrPtr = 
						&Tls_GaaHShakeBuffer[LusActiveConnIdx][LusHShakeBufferPosition];
						LddReturnValue = E_OK;
					}
					else
					{
						LddReturnValue = E_NOT_OK;
					}
				}
				/* if not then for sure, it will be ready , since no other ways 
				it can come till this point*/           
				else
				{
					/* where to store , depends of what HS state is */
					LpDecrPtr =  &Tls_GaaRecAppBuffer[LusActiveConnIdx][TLS_ZERO];
					LddReturnValue = E_OK;
				}
	
				if (E_OK == LddReturnValue)
				{
					LddReturnValue = Tls_DecryptRecord(
									LusActiveConnIdx,
									LpEncrPtr,            
									LpDecrPtr,
									&LusDecryptDataLen);
	
					if (E_OK == LddReturnValue)
					{
						if ((uint8)TLS_ALERT == LpDecrPtr[LusDecryptDataLen])
						{
							LddReturnValue = Tls_ProcessAlertRecord(
												LddConnectionId,
												LpDecrPtr,
												LusDecryptDataLen);
												
							/* TBD : as discussed with abhijith, since we are keeping it 
							strict as per frame , so its this returns enotok, then 
							stop and abort */
							if (E_OK != LddReturnValue)
							{
								if (TLS_CONNECTION_IN_PROGRESS == 
											Tls_GddConnHSStatus[LddConnectionId])
								{
									/* our design */
									Tls_GddConnHSStatus[LddConnectionId] = 
															TLS_CONNECTION_FAILED;
									Tls_ConnStatsNotification(LddConnectionId,
															TLS_CONNECTION_FAILED);                    
								}
								else
								{
									Tls_GddConnHSStatus[LddConnectionId] = 
															TLS_CONNECTION_ABORTED;
									Tls_ConnStatsNotification(LddConnectionId,
															TLS_CONNECTION_ABORTED);                     
								}                          
							}
							/* here if processing alert is succesfull , then no need 
							of dequeing, since the connection is already closed state
							and in next main function , everytging will be reset*/
						}
						else
						{
							if (TLS_CONNECTION_IN_PROGRESS == 
											Tls_GddConnHSStatus[LddConnectionId])
							{
								/* Update the position in the handshake buffer */
								/* store without Record Hdr bytes */
								LpTlsFrame->usHShakeBufferPosition += LusDecryptDataLen;
		
								LpTlsFrame->usUnProcessedData += LusDecryptDataLen;
	
								Tls_DeQueueRecord(LusActiveConnIdx, LusRecordSize);
							}
							/* for sure it will be READY state */
							else
							{
								/* if required, make this a Seprate func, after checking with 
								abhijith */
								/* then for sure , it will be application data , which needs
								to be validated and sent to Tcpip */
								LusMaxFragmntLen = 
								Tls_GaaConnection[LddConnectionId].usTlsMaxFragmentLen;
	
								/* verify this part before only, before doing any work */
								if (LusDecryptDataLen > LusMaxFragmntLen)
								{
									/* Abort ? and Reset everything, Que also  ?*/
									Tls_GddConnHSStatus[LddConnectionId] = 
														TLS_CONNECTION_ABORTED;
														
									Tls_ConnStatsNotification(LddConnectionId,
															TLS_CONNECTION_ABORTED);
								}
								else
								{
									/*call reception function to Tcpip */
									LddReturnValue = Tls_TcpReception(
									LddConnectionId,
									/* Buffer with the whole Message prepared */
									&Tls_GaaRecAppBuffer[LusActiveConnIdx][TLS_ZERO],
									/* Message Length */
									LusDecryptDataLen);
	
									if (E_OK == LddReturnValue)
									{
										Tls_DeQueueRecord(LusActiveConnIdx, 
																	LusRecordSize);
									}
									else
									{
									/* Abort or close ? */
									/* Abort ? and Reset everything, Que also  ?*/
										Tls_GddConnHSStatus[LddConnectionId] = 
														TLS_CONNECTION_ABORTED; 
										
										Tls_ConnStatsNotification(LddConnectionId,
															TLS_CONNECTION_ABORTED);
									}
								}
							}
						}
					}
				}
			}
			else
			{
				Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_ABORTED; 
				Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_ABORTED);
			}			
		}
    }
    else
    {
        /* here check if Close Connection was called when HS was in Completed State
        and there is nothig in the Buffere to be decrypted */
		/* Note : this will be only true at only one plave , when closeconnection
		was called during HS completed iwth abort false */
        if (LpTlsFrame->blIsCloseConnAfterHS)
        {
            Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_CLOSED;   
        }
    }
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_ProcessDummyCHExt                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function is Dummy.                             **
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
**  Design ID           : TLS_SDD_0768                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ProcessDummyCHExt
                            (const TcpIp_TlsConnectionIdType  LddConnectionId,
                            uint8* LpHShakeBuf, // parasoft-suppress MISRAC2012-RULE_8_13-a-4 CERT_C-DCL13-a-3 CERT_C-DCL00-b-3 "Reason: Tls_Internal_c_REF_2 Tls_Internal_c_REF_3 Tls_Internal_c_REF4.
                            uint16* const LpExtLen) // parasoft-suppress MISRAC2012-RULE_8_13-a-4 CERT_C-DCL13-a-3 "Reason: Tls_Internal_c_REF_2 Tls_Internal_c_REF_3.
{
    /* Dummy Handshake*/ 
    TLS_UNUSED(LddConnectionId);
    TLS_UNUSED(LpHShakeBuf);
    TLS_UNUSED(LpExtLen);
    
    return E_OK;
}/* end of Tls_ProcessDummyCHExt*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_GetKShareExpLen                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : the function gets the L value, Based on the negotiated cipher suite   **
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
**  Design ID           : TLS_SDD_0356                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
void Tls_GetKShareExpLen(const uint16 LusSuprtdGrp,
                                                uint16* const LpExpKShareLen)
{
    switch (LusSuprtdGrp)
    {
        case (uint16)TLS_SECP256R1:
            *LpExpKShareLen = (uint16)TLS_SECP256R1_SIZE;
            break;
        case (uint16)TLS_SECP384R1:
            *LpExpKShareLen = (uint16)TLS_SECP384R1_SIZE;
            break;
        case (uint16)TLS_SECP521R1:
            *LpExpKShareLen = (uint16)TLS_SECP521R1_SIZE;
            break;
        case (uint16)TLS_X25519:
            *LpExpKShareLen = (uint16)TLS_X25519_SIZE;
            break;
        case (uint16)TLS_X448:
            *LpExpKShareLen = (uint16)TLS_X448_SIZE;
            break;           
        default:
            /* Do Nothing */
            *LpExpKShareLen = (uint16)TLS_ZERO;
            break;
    }

}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_CheckSupportedGrpValue                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : the function gets the L value, Based on the negotiated cipher suite   **
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
**  Design ID           : TLS_SDD_0357                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
boolean Tls_CheckSupportedGrpValue(const uint16 LusSupportedGrpId)
{
    boolean  LblSupGrpFound;
    
    switch (LusSupportedGrpId) 
    {
        case (uint16)TLS_SECP256R1:
        case (uint16)TLS_SECP384R1:
        case (uint16)TLS_SECP521R1:
        case (uint16)TLS_X25519:
        case (uint16)TLS_X448:
            LblSupGrpFound = TLS_TRUE;
            break;
        default:
            LblSupGrpFound = TLS_FALSE;
            break;
    }
    return LblSupGrpFound;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_CheckSignatureAlgoValue                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : the function gets the L value, Based on the negotiated cipher suite   **
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
**  Design ID           : TLS_SDD_0358                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
boolean Tls_CheckSignatureAlgoValue(const uint16 LusSignatureAlgoId)
{
    boolean  LblSigAlgoFound;
    
    switch (LusSignatureAlgoId)
    {
        case (uint16)TLS_RSA_PKCS1_SHA256:
        case (uint16)TLS_RSA_PKCS1_SHA384:
        case (uint16)TLS_RSA_PKCS1_SHA512:
        case (uint16)TLS_RSA_PSS_RSAE_SHA256:
        case (uint16)TLS_RSA_PSS_RSAE_SHA384:
		case (uint16)TLS_RSA_PSS_RSAE_SHA512:
		case (uint16)TLS_RSA_PSS_PSS_SHA256:
		case (uint16)TLS_RSA_PSS_PSS_SHA384:
		case (uint16)TLS_RSA_PSS_PSS_SHA512:
            LblSigAlgoFound = TLS_TRUE;
            break;
        default:
            LblSigAlgoFound = TLS_FALSE;
            break;
    }
    return LblSigAlgoFound;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_ComputeTranscriptHash                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : the function gets the L value, Based on the negotiated cipher suite   **
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
**  Design ID           : TLS_SDD_0359                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType Tls_ComputeTranscriptHash(const uint16 LusActiveConnIdx, uint8* const LpTranscriptHash, uint16* const LpTranscriptHashLen)
{
    Std_ReturnType LddReturnValue;
    uint32 LulHashingJobId;
    uint16 LusTotlInputLen;
    uint8* LpHSShakeBuf;
    uint16 LusNegotitdCiprSutId;
    uint8 LucChoosenHashLen;
    uint32 LulOutHashLen;

    LusTotlInputLen = Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition;
    LpHSShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO];

    LulHashingJobId = Tls_GaaHandshake[
        Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].usTlsConnHandshakeRefBaseIndx
    ].ulTlsCsmHashVerifyJobRefIdx;

    LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[
        Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].ulTlsCiphrSutDefRefIdx
    ].usTlsCiphersuiteId;

    LucChoosenHashLen = (LusNegotitdCiprSutId == (uint16)TLS_AES_256_GCM_SHA384) ? (uint8)TLS_FOURTYEIGHT : (uint8)TLS_THIRTYTWO;
    LulOutHashLen = (uint32)LucChoosenHashLen;

    LddReturnValue = Tls_PerformHash(
        LulHashingJobId,
        LpHSShakeBuf,
        (uint32)LusTotlInputLen,
        LpTranscriptHash,
        &LulOutHashLen
    );

    if ((E_OK == LddReturnValue) && (LulOutHashLen == (uint32)LucChoosenHashLen))
    {
        *LpTranscriptHashLen = (uint16)LucChoosenHashLen;
    }
    else
    {
        LddReturnValue = E_NOT_OK;
        *LpTranscriptHashLen = (uint16)TLS_ZERO;
    }
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_CheckSupportedSigAlgo                          **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : the function gets the L value, Based on the negotiated cipher suite   **
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
**  Design ID           : TLS_SDD_0360                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
boolean Tls_CheckSupportedSigAlgo(const uint16 LusSignatureAlgoId, 
const uint16 LusActiveConnIdx)
{
    boolean  LblSigAlgoFound;
    uint16 LusNumOfHSWrkrs;
	uint16 LusHSWrkrCtr;
	uint16 LusHSWrkrStartIdx;
    uint16 LusHSWrkrLookUpIdx;
	
	LblSigAlgoFound = TLS_FALSE;
    LusNumOfHSWrkrs = Tls_GaaConnection[Tls_GddActiveConnManager[
                        LusActiveConnIdx].ddTlsConnectionId].usNumOfHSWorkers;
						
	LusHSWrkrStartIdx = Tls_GaaConnection[Tls_GddActiveConnManager[
                        LusActiveConnIdx].ddTlsConnectionId].usHSWrkrIdx;
	
	for (LusHSWrkrCtr = (uint16)TLS_ZERO; LusHSWrkrCtr < 
                                                LusNumOfHSWrkrs; LusHSWrkrCtr++)
    {
		LusHSWrkrLookUpIdx = Tls_GaaHSLookUp[LusHSWrkrStartIdx + LusHSWrkrCtr];
		
		if (Tls_GaaHandshake[LusHSWrkrLookUpIdx].usTlsSignatureId == 
		    LusSignatureAlgoId)
			{
				LblSigAlgoFound = TLS_TRUE;
				Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedSigAlgoIdx = 
															LusHSWrkrLookUpIdx;
                break;
			}
	}		

    
    return LblSigAlgoFound;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/* parasoft-end-suppress CERT_C-API00-a-3 */
/* parasoft-end-suppress CERT_C-DCL19-a-3 */
/* parasoft-end-suppress MISRAC2012-RULE_20_1-a-4 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-d-2 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 */