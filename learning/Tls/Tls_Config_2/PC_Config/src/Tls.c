/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls.c                                                          **
**                                                                            **
** TARGET    : All                                                            **
**                                                                            **
** PRODUCT   : AUTOSAR Transport Layer Security                               **
**                                                                            **
** PURPOSE   : This file provides definitions of following Tls APIs:          **
**                                                                            **
**             Tls_Init                                                       **
**                                                                            **
**  PLATFORM DEPENDANT [yes/no]: no                                           **
**                                                                            **
**  TO BE CHANGED BY USER [yes/no]: no                                        **
**                                                                            **
**  Design ID  : TLS_SDD_0305, TLS_SDD_0316, TLS_SDD_0339                     **
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
* #section Tls_c_REF_1
* Violates CERT_C-DCL00-b-3: Declare parameter '' as const
* Justification: Standard Autosar Interface provided cannot be tailored
* according to the violation.
*
*
* #section Tls_c_REF_2
* Violates MISRAC2012-RULE_5_4-c-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_c_REF_3
* Violates MISRAC2012-RULE_5_4-d-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_c_REF_4
* Violates MISRAC2012-RULE_20_1-a-4: Do not precede the #include directive with
* executable code
* Justification: Macros are defined by AUTOSAR standards and internal
* specifications.
*
* #section Tls_c_REF_5
* Violates CERT_C-DCL19-a-3: Variable "" should be declared as locally as
* possible
* Justification: Coding guidelines suggest the variables are declared on the
* start of the function.
*
* #section Tls_c_REF_6
* Violates CERT_C-API00-a-3: Parameter '' is not validated before use.
* Justification: This parameter either originates from the configuration or is
* validated in the main API before being passed as an argument to the local API.
* Hence, additional validation within the local API is not required and has been
* manually verified.
*/


/* parasoft-begin-suppress MISRAC2012-RULE_5_4-c-2 
"Reason: Tls_c_REF_2" */
/* parasoft-begin-suppress MISRAC2012-RULE_5_4-d-2 
"Reason: Tls_c_REF_3" */
/* parasoft-begin-suppress MISRAC2012-RULE_20_1-a-4 
"Reason: Tls_c_REF_4" */
/* parasoft-begin-suppress CERT_C-DCL19-a-3 
"Reason: Tls_c_REF_5" */
/* parasoft-begin-suppress CERT_C-API00-a-3 
"Reason: Tls_c_REF_6" */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Tls.h"
#include "Tls_Internal.h"
#include "Det.h"
#include "Tls_External.h"

/* Later Remove this TO_REMOVE for TLS_unused */
#include "Tls_InternalTypes.h"
#include "Tls_Inline.h"
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


/*******************************************************************************
** Function Name        : Tls_Init                                       **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Initializes the TLS module                          **
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
**  Design ID           : TLS_SDD_0187, TLS_SDD_0337                          **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
void  Tls_Init(void)
{
    uint16 LusStatusCount;
  
    /* initialize the handshake to default */
    for (LusStatusCount = (uint16)TLS_ZERO; 
        LusStatusCount < TLS_TOTAL_CONNECTIONS; LusStatusCount++)
    {
        Tls_GddConnHSStatus[LusStatusCount] = TLS_CONNECTION_NOT_INITIATED;
    }

    /* initialize the states to default */
    for (LusStatusCount = (uint16)TLS_ZERO; 
        LusStatusCount < TLS_MAX_ACTIVE_CONNECTIONS; LusStatusCount++)
    {
        Tls_GaaClientState[LusStatusCount] = TLS_CLIENT_START;
        Tls_GaaServerState[LusStatusCount] = TLS_SERVER_WAIT_FOR_CH;
        Tls_GaaHandshakeElementIndex[LusStatusCount] = TLS_ZERO;
        Tls_GddActiveConnManager[LusStatusCount].ddTlsConnectionId = 
                                                        TLS_INVALID_CONNECTION;
        Tls_GddActiveConnManager[LusStatusCount].ddConnectionState = 
                                                        TLS_CONNECTION_INACTIVE;
                                                        
        Tls_HandshakeFrame[LusStatusCount].ulWriteSeqNum = (uint64)TLS_ZERO;
        Tls_HandshakeFrame[LusStatusCount].ulReadSeqNum = (uint64)TLS_ZERO;
        Tls_HandshakeFrame[LusStatusCount].blIsHSKeysDerived = TLS_FALSE;
        Tls_HandshakeFrame[LusStatusCount].blIsAppKeysDerived = TLS_FALSE;
        Tls_HandshakeFrame[LusStatusCount].blIsTxInProgress = TLS_FALSE;
        Tls_HandshakeFrame[LusStatusCount].usProcessedData = (uint16)TLS_ZERO;
        Tls_HandshakeFrame[LusStatusCount].usUnProcessedData = (uint16)TLS_ZERO;
        Tls_HandshakeFrame[LusStatusCount].usNegotiatedSupGrpIdx = 
                                                    (uint16)TLS_DEFAULT_VALUE;
        Tls_HandshakeFrame[LusStatusCount].usNegotiatedSigAlgoIdx = 
                                                    (uint16)TLS_DEFAULT_VALUE;
		#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)											
		Tls_HandshakeFrame[LusStatusCount].ddVerifyCertState = TLS_IDLE;
		Tls_HandshakeFrame[LusStatusCount].blIsCertReqReceived = TLS_FALSE;
		#endif
        Tls_HandshakeFrame[LusStatusCount].blIsCloseConnAfterHS = TLS_FALSE;    
        Tls_HandshakeFrame[LusStatusCount].usProcessedHSBytes = (uint16)TLS_ZERO;
        Tls_HandshakeFrame[LusStatusCount].usNegotiatedCiprSutIdx = (uint16)TLS_ZERO;
        Tls_HandshakeFrame[LusStatusCount].ulNegotiatedPskCsmKeyId = (uint16)TLS_ZERO;
        Tls_HandshakeFrame[LusStatusCount].usHShakeBufferPosition = (uint16)TLS_ZERO;
        Tls_HandshakeFrame[LusStatusCount].usTotlAppData = (uint16)TLS_ZERO;
        /* Clear Queue */
        Tls_HandshakeFrame[LusStatusCount].ddQueue.usQHead = (uint16)TLS_ZERO;
        Tls_HandshakeFrame[LusStatusCount].ddQueue.usQTail = (uint16)TLS_ZERO;
        Tls_HandshakeFrame[LusStatusCount].ddQueue.usTotlQData = (uint16)TLS_ZERO;

        Tls_GaaAlert[LusStatusCount] = TLS_UNKNOWN_ALERT_DESCRIPTION;
        
        #if (STD_ON == TLS_TIMER_SUPPORT)
            Tls_GddActiveConnManager[LusStatusCount].usWaitingTimeout = 
                                                            (uint16)TLS_ZERO;
        #endif
    }

    /* initialize the Groups to default state */
    #if (STD_ON == TLS_GROUP_CONFIGURED)
        for (LusStatusCount = (uint16)TLS_ZERO; 
                            LusStatusCount < TLS_TOTAL_GROUPS; LusStatusCount++)
        {
            Tls_GddGroups[LusStatusCount] = TLS_GROUP_CONNECTION_INACTIVE;
        }
    #endif
 
    /*Set the global init status to true*/
    Tls_GblInitStatus = TLS_TRUE;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_OpenConnection                             **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Opens a TLS connection and updates the Connection   **
**                        state and handshake state                           **
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
**  Design ID           : TLS_SDD_0188, TLS_SDD_0340                          **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_OpenConnection(
                                TcpIp_TlsConnectionIdType TlsConnectionId, // parasoft-suppress CERT_C-DCL00-b-3 "Reason: Tls_c_REF_1."
                                const Tls_ConnectionType  TlsConnectionType)
{
    Std_ReturnType  LddReturnValue;
    uint16   LusActiveIdx;
 
    if (!(Tls_GblInitStatus))
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                            TLS_OPENCONNECTION_SID, TLS_E_UNINIT);
        #endif
        LddReturnValue = E_NOT_OK;  
    }
    else if (TlsConnectionId >= TLS_TOTAL_CONNECTIONS)
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                            TLS_OPENCONNECTION_SID, TLS_E_INVALID_PARAMETER);
        #endif
        LddReturnValue = E_NOT_OK;  
    }
    else
    {
        LddReturnValue = 
        Tls_GetInActiveConnIdx(
                                TlsConnectionId, 
                                TlsConnectionType, 
                                &LusActiveIdx);
    
        /* if the func returns E_OK , meaning there is an empty SLOT left in 
        the active connections */
        if (E_OK == LddReturnValue)
        {      
            /* update the state to ONLINE/ HANDSHAKE started */
            Tls_GddConnHSStatus[TlsConnectionId] = TLS_CONNECTION_IN_PROGRESS; 
            
            Tls_GddActiveConnManager[LusActiveIdx].ddTlsConnectionId = 
                                                            TlsConnectionId;
            Tls_GddActiveConnManager[LusActiveIdx].ddConnectionState = 
                                                        TLS_CONNECTION_ACTIVE;
            #if (STD_ON == TLS_TIMER_SUPPORT)
                Tls_GddActiveConnManager[LusActiveIdx].usWaitingTimeout = 
                        Tls_GaaConnection[TlsConnectionId].usTlsWaitingTimeout;
            #endif                                             
        
            Tls_GaaHandshakeElementIndex[LusActiveIdx] = TLS_ZERO;
            
            /* Initial index to denote position in the handshake buffer */
            Tls_HandshakeFrame[LusActiveIdx].usHShakeBufferPosition = TLS_ZERO;
            Tls_HandshakeFrame[LusActiveIdx].ulWriteSeqNum = (uint64)TLS_ZERO;
            Tls_HandshakeFrame[LusActiveIdx].ulReadSeqNum = (uint64)TLS_ZERO;
            Tls_HandshakeFrame[LusActiveIdx].blIsHSKeysDerived = TLS_FALSE;
            Tls_HandshakeFrame[LusActiveIdx].blIsAppKeysDerived = TLS_FALSE;
            Tls_HandshakeFrame[LusActiveIdx].blIsTxInProgress = TLS_FALSE;
            Tls_HandshakeFrame[LusActiveIdx].usTotlAppData = (uint16)TLS_ZERO;
            Tls_HandshakeFrame[LusActiveIdx].usProcessedData = (uint16)TLS_ZERO;
            Tls_HandshakeFrame[LusActiveIdx].usUnProcessedData = (uint16)TLS_ZERO;
			#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
			Tls_HandshakeFrame[LusActiveIdx].ddVerifyCertState = TLS_IDLE;
			Tls_HandshakeFrame[LusActiveIdx].blIsCertReqReceived = TLS_FALSE;
			#endif
			Tls_HandshakeFrame[LusActiveIdx].blIsCloseConnAfterHS = TLS_FALSE;
            Tls_HandshakeFrame[LusActiveIdx].usNegotiatedSupGrpIdx = 
                                                    (uint16)TLS_DEFAULT_VALUE;
            Tls_HandshakeFrame[LusActiveIdx].usNegotiatedSigAlgoIdx = 
                                                    (uint16)TLS_DEFAULT_VALUE;

            Tls_HandshakeFrame[LusActiveIdx].usProcessedHSBytes = (uint16)TLS_ZERO;
            Tls_HandshakeFrame[LusActiveIdx].usNegotiatedCiprSutIdx = 
                                                            (uint16)TLS_ZERO;
            Tls_HandshakeFrame[LusActiveIdx].ulNegotiatedPskCsmKeyId = 
                                                            (uint16)TLS_ZERO;
                /* Clear Queue */
            Tls_HandshakeFrame[LusActiveIdx].ddQueue.usQHead = 
                                                            (uint16)TLS_ZERO;
            Tls_HandshakeFrame[LusActiveIdx].ddQueue.usQTail = 
                                                            (uint16)TLS_ZERO;
            Tls_HandshakeFrame[LusActiveIdx].ddQueue.usTotlQData = 
                                                            (uint16)TLS_ZERO;

            Tls_GaaAlert[LusActiveIdx] = TLS_UNKNOWN_ALERT_DESCRIPTION;
        
            if (TLS_CLIENT == TlsConnectionType)
            {
                Tls_GaaFetchHShakeElmnts[LusActiveIdx] = 
                                            TLS_PERFORM_CLIENT_HELLO_HANDSHAKE;
            }
            else
            {
                Tls_GaaFetchHShakeElmnts[LusActiveIdx] =
                                            TLS_HANDLE_CLIENT_HELLO_RECEPTION;      
            }
        }  
    }
    
    return LddReturnValue; 
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"
/*******************************************************************************
** Function Name        : Tls_CloseConnection                                 **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Closes a TLS connection                             **
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
**  Design ID           : TLS_SDD_0189, TLS_SDD_0342                          **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_CloseConnection(
                                    TcpIp_TlsConnectionIdType TlsConnectionId, // parasoft-suppress CERT_C-DCL00-b-3 "Reason: Tls_c_REF_1."
                                    boolean Abort) // parasoft-suppress CERT_C-DCL00-b-3 "Reason: Tls_c_REF_1."
{

    Std_ReturnType  LddReturnValue;
	uint16 LusActiveConnIdx;
	
    if (!(Tls_GblInitStatus))
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                                        TLS_CLOSECONNECTION_SID, TLS_E_UNINIT);
        #endif
        LddReturnValue = E_NOT_OK;  
    }
    else if (TlsConnectionId >= TLS_TOTAL_CONNECTIONS)
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                            TLS_CLOSECONNECTION_SID, TLS_E_INVALID_PARAMETER);
        #endif
        LddReturnValue = E_NOT_OK;  
    }
    else
    {
        switch (Tls_GddConnHSStatus[TlsConnectionId])
        {
            case TLS_CONNECTION_IN_PROGRESS:
            
                if (Abort)
                {
                    Tls_GddConnHSStatus[TlsConnectionId] = 
                                                        TLS_CONNECTION_ABORTED;
                                                        
                    Tls_ConnStatsNotification(TlsConnectionId,
                                                        TLS_CONNECTION_ABORTED);
                                                        
                    LddReturnValue = E_OK;
                }
                else
                {
                    LddReturnValue = 
                        Tls_ProcessAlert(TlsConnectionId, TLS_USER_CANCELED);
                        
                    if (E_OK == LddReturnValue)
                    {
                        Tls_GddConnHSStatus[TlsConnectionId] = 
                                                          TLS_CONNECTION_CLOSED;
                    }                   
                }

                break;  

            case TLS_CONNECTION_READY:
            
                if (Abort)
                {
                    Tls_GddConnHSStatus[TlsConnectionId] = 
                                                        TLS_CONNECTION_ABORTED;
                                                        
                    Tls_ConnStatsNotification(TlsConnectionId,
                                                        TLS_CONNECTION_ABORTED);
                }
                else
                {
                    LusActiveConnIdx = Tls_GetActIdx(TlsConnectionId);
					Tls_HandshakeFrame[LusActiveConnIdx].blIsCloseConnAfterHS = 
																	TLS_TRUE;                   
                }

                LddReturnValue = E_OK;
                break; 

            default:
                LddReturnValue = E_NOT_OK;
                break;                
        }
    }
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"
/*******************************************************************************
** Function Name        : Tls_RxIndication                                    **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Indicates incoming data to the TLS module           **
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
**  Design ID           : TLS_SDD_0190                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_RxIndication(
                                TcpIp_TlsConnectionIdType TlsConnectionId, // parasoft-suppress CERT_C-DCL00-b-3 "Reason: Tls_c_REF_1."
                                const uint8*  DataPtr, // parasoft-suppress CERT_C-DCL00-b-3 "Reason: Tls_c_REF_1."
                                const uint16  TotalDataLength)
{
    Std_ReturnType  LddReturnValue;
    
    /* Check if initialization was successful */
    if (!(Tls_GblInitStatus))
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                                            TLS_RXINDICATION_SID, TLS_E_UNINIT);
        #endif
        LddReturnValue = E_NOT_OK;  
    }
    /* Input parameter validation */
    else if (NULL_PTR == DataPtr)
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                                    TLS_RXINDICATION_SID, TLS_E_PARAM_POINTER);
        #endif
        LddReturnValue = E_NOT_OK;          
    }
    else if (!(TLS_ZERO < TotalDataLength))
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                                TLS_RXINDICATION_SID, TLS_E_INVALID_PARAMETER);
        #endif
        LddReturnValue = E_NOT_OK;        
    }
    else if (TlsConnectionId >= TLS_TOTAL_CONNECTIONS)
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                       TLS_RXINDICATION_SID, TLS_E_INVALID_PARAMETER);
        #endif
        LddReturnValue = E_NOT_OK;    
    }
    else
    {
      if ((TLS_CONNECTION_IN_PROGRESS == Tls_GddConnHSStatus[TlsConnectionId])
          || (TLS_CONNECTION_READY == Tls_GddConnHSStatus[TlsConnectionId]))
      {
        LddReturnValue = Tls_HandleRecordReception (
                                TlsConnectionId,
                                DataPtr,
                                TotalDataLength);  
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
** Function Name        : Tls_GetConnectionStatus                             **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Gets the status of a called Tls Connection          **
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
**  Design ID           : TLS_SDD_0191                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_GetConnectionStatus(
                                TcpIp_TlsConnectionIdType TlsConnectionId, // parasoft-suppress CERT_C-DCL00-b-3 "Reason: Tls_c_REF_1." 
                                Tls_ConnectionStatusType*  ConnectionResultPtr) // parasoft-suppress CERT_C-DCL00-b-3 "Reason: Tls_c_REF_1."
{
    Std_ReturnType  LddReturnValue;
    
    /* Initialize default return value */
    LddReturnValue = E_OK;
    
    if (!(Tls_GblInitStatus))
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                                    TLS_GETCONNECTIONSTATUS_SID, TLS_E_UNINIT);
        #endif
        LddReturnValue = E_NOT_OK;  
    }
    else if (TlsConnectionId >= TLS_TOTAL_CONNECTIONS)
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                       TLS_GETCONNECTIONSTATUS_SID, TLS_E_INVALID_PARAMETER);
        #endif
        LddReturnValue = E_NOT_OK;    
    }
    else if (NULL_PTR == ConnectionResultPtr)
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
        (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                       TLS_GETCONNECTIONSTATUS_SID, TLS_E_PARAM_POINTER);
        #endif
        LddReturnValue = E_NOT_OK;    
    }    
    else
    {
        /* Do Nothing */
    }
  
    if (E_OK == LddReturnValue)
    {
        /* Provide the status for requested TLS connection */
        *ConnectionResultPtr = Tls_GddConnHSStatus[TlsConnectionId];    
    }   

    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_TxIndication                                    **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          :Indicates incoming Application data to the TLS module**
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
**  Design ID           : TLS_SDD_0192                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_TxIndication(
                                TcpIp_TlsConnectionIdType TlsConnectionId, // parasoft-suppress CERT_C-DCL00-b-3 "Reason: Tls_c_REF_1."
                                const uint8*  DataPtr, // parasoft-suppress CERT_C-DCL00-b-3 "Reason: Tls_c_REF_1."
                                const uint16  TotalDataLength)
{
 
    Std_ReturnType  LddReturnValue;
    uint16 LusActiveConnIdx;
	
    if (!(Tls_GblInitStatus))
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                                            TLS_TXINDICATION_SID, TLS_E_UNINIT);
        #endif
        LddReturnValue = E_NOT_OK;  
    }
    else if (TlsConnectionId >= TLS_TOTAL_CONNECTIONS)
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                       TLS_TXINDICATION_SID, TLS_E_INVALID_PARAMETER);
        #endif
        LddReturnValue = E_NOT_OK;    
    }
    else if (!(TLS_ZERO < TotalDataLength))
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
            (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                                TLS_TXINDICATION_SID, TLS_E_INVALID_PARAMETER);
        #endif
        LddReturnValue = E_NOT_OK;        
    }
    else
    {
        LusActiveConnIdx = Tls_GetActIdx(TlsConnectionId);
        
        if ((TLS_CONNECTION_READY == Tls_GddConnHSStatus[TlsConnectionId]) && 
                    (!(Tls_HandshakeFrame[LusActiveConnIdx].blIsTxInProgress)))
        {
            /* if tx indication was called with length greater then Buff capacity*/
            if (TotalDataLength <= (uint16)TLS_MAX_BUFFER_SIZE)
            { 
                if (NULL_PTR == DataPtr)
                {
                    LddReturnValue = Tls_TcpCopyTxData(TlsConnectionId,
                            &Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO],
                            TotalDataLength);
                }
                else
                {
                    Tls_MemCpy(&Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO], 
                                DataPtr, TotalDataLength);

                    LddReturnValue = E_OK;
                }

                if (E_OK == LddReturnValue)
                {
                    Tls_HandshakeFrame[LusActiveConnIdx].blIsTxInProgress 
                                                                = TLS_TRUE;

                    Tls_HandshakeFrame[LusActiveConnIdx].usTotlAppData = 
                                                                TotalDataLength;  
                }
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
** Function Name        : Tls_GetVersionInfo                                       **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This service Returns the version information                          **
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
**  Design ID           : TLS_SDD_0285, TLS_SDD_0338                          **
*******************************************************************************/
#if (STD_ON == TLS_VERSION_INFO_API)
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
void Tls_GetVersionInfo(Std_VersionInfoType*  versioninfo) // parasoft-suppress CERT_C-DCL00-b-3 "Reason: Tls_c_REF_1."
{

    if (NULL_PTR == versioninfo)
    {
        #if (STD_ON == TLS_DEV_ERROR_DETECT)
        (void)Det_ReportError(TLS_MODULE_ID, TLS_INSTANCE_ID,
                    TLS_GETVERSIONINFO_SID, TLS_E_PARAM_POINTER);
        #endif  
    }
    else
    {
        /* Copy the vendor Id */
        versioninfo->vendorID = TLS_VENDOR_ID;
        /* Copy the module Id */
        versioninfo->moduleID = TLS_MODULE_ID;
        /* Copy Software Major Version */
        versioninfo->sw_major_version = (uint8)TLS_SW_MAJOR_VERSION;
        /* Copy Software Minor Version */
        versioninfo->sw_minor_version = (uint8)TLS_SW_MINOR_VERSION;
        /* Copy Software Patch Version */
        versioninfo->sw_patch_version = (uint8)TLS_SW_PATCH_VERSION;     
    }
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"
#endif

/* parasoft-end-suppress CERT_C-API00-a-3 */
/* parasoft-end-suppress CERT_C-DCL19-a-3 */
/* parasoft-end-suppress MISRAC2012-RULE_20_1-a-4 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-d-2 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 */