/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_Handshake.c                                                **
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
**  Design ID  : TLS_SDD_0307                                                 **
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
* #section Tls_Handshake_c_REF_1
* Violates MISRAC2012-RULE_5_4-c-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Handshake_c_REF_2
* Violates MISRAC2012-RULE_5_4-d-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Handshake_c_REF_3
* Violates MISRAC2012-RULE_20_1-a-4: Do not precede the #include directive with
* executable code
* Justification: Macros are defined by AUTOSAR standards and internal
* specifications.
*
* #section Tls_Handshake_c_REF_4
* Violates CERT_C-DCL19-a-3: Variable "" should be declared as locally as
* possible
* Justification: Coding guidelines suggest the variables are declared on the
* start of the function.
*
* #section Tls_Handshake_c_REF_5
* Violates CERT_C-API00-a-3: Parameter '' is not validated before use.
* Justification: This parameter either originates from the configuration or is
* validated in the main API before being passed as an argument to the local API.
* Hence, additional validation within the local API is not required and has been
* manually verified.
*
* #section Tls_Handshake_c_REF_6
* Violates CERT_C-DCL00-a-3:  Declare local variable '' as const
* Justification: The tool analysis does not detect indirect modifications through
* dereferencing or pointer arithmetic, so applying the 'const' qualifier would 
* be incorrect in this context.
*/

/* parasoft-begin-suppress MISRAC2012-RULE_5_4-c-2 
"Reason: Tls_Handshake_c_REF_1" */
/* parasoft-begin-suppress MISRAC2012-RULE_5_4-d-2 
"Reason: Tls_Handshake_c_REF_2" */
/* parasoft-begin-suppress MISRAC2012-RULE_20_1-a-4 
"Reason: Tls_Handshake_c_REF_3" */
/* parasoft-begin-suppress CERT_C-DCL19-a-3 
"Reason: Tls_Handshake_c_REF_4" */
/* parasoft-begin-suppress CERT_C-API00-a-3 
"Reason: Tls_Handshake_c_REF_5" */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Tls_Internal.h"
#include "Tls_Inline.h"
#include "Tls_External.h"

#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
#include "Tls_Certificate.h"
#endif


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
static void  Tls_HandleClientFinReception(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PerformServerFinished(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PrepareEEMessage(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PerformHandShakeKeyscalc(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_HandleClientHelloReception(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PrepareServerHelloMessage(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PerformClientFinished(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_HandleServerFinReception(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_HandleEEReception(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PrepareCertHSMessage(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_HandleCertHSMessage(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_HandleServerHelloReception(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PerformApplicationKeyscalc(const uint16 LusActiveConIdxx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PrepareCHMessage(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_ProcessDummyHandshake(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_ProcessServerHShakeProto(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_ProcessClientHShakeProto(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PrepareCVHSMessage(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_HandleCVHSMessage(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PerformCertVerification(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PrepareServerCertReqHSMsg(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_HandleServerCRHSMessage(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
#define TLS_START_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"
/* Constant variable to hold Sequence of element of handshake protocol to 
 follow for TLS Handshake*/
static const Tls_ElmntsOfHShakeProto Tls_GSaaHShakeElmntList[TLS_FOUR][TLS_THIRTEEN] =
{
    {
        /* Client -PSK */   
        TLS_PERFORM_CLIENT_HELLO_HANDSHAKE,
        TLS_HANDLE_SERVER_HELLO_RECEPTION,
        TLS_PERFORM_CLIENT_HANDSHAKE_KEYS_CALCULATION,
        TLS_HANDLE_ENCRYPTED_EXTENSIONS_RECEPTION,    
        TLS_HANDLE_SERVER_FINISHED_HANDSHAKE,    
        TLS_PERFORM_CLIENT_FINISHED_HANDSHAKE,
        TLS_PERFORM_CLIENT_APPLICATION_KEYS_CALCULATION,
		TLS_DUMMY_HANDSHAKE,
		TLS_DUMMY_HANDSHAKE,
		TLS_DUMMY_HANDSHAKE,
		TLS_DUMMY_HANDSHAKE,
		TLS_DUMMY_HANDSHAKE,
        TLS_DUMMY_HANDSHAKE        
    },
    { 
        /* Server -PSK */
        TLS_HANDLE_CLIENT_HELLO_RECEPTION,    
        TLS_PERFORM_SERVER_HELLO_HANDSHAKE,
        TLS_PERFORM_SERVER_HANDSHAKE_KEYS_CALCULATION,
        TLS_PERFORM_ENCRYPTED_EXTENSIONS_HANDSHAKE,
        TLS_PERFORM_SERVER_FINISHED_HANDSHAKE,
        TLS_PERFORM_SERVER_APPLICATION_KEYS_CALCULATION,      
        TLS_HANDLE_CLIENT_FINISHED_HANDSHAKE,
		TLS_DUMMY_HANDSHAKE,
		TLS_DUMMY_HANDSHAKE,
		TLS_DUMMY_HANDSHAKE,
		TLS_DUMMY_HANDSHAKE,
		TLS_DUMMY_HANDSHAKE,
        TLS_DUMMY_HANDSHAKE   		
    },
    {
        /* Client - Certificate*/   
        TLS_PERFORM_CLIENT_HELLO_HANDSHAKE,
        TLS_HANDLE_SERVER_HELLO_RECEPTION,
        TLS_PERFORM_CLIENT_HANDSHAKE_KEYS_CALCULATION,
        TLS_HANDLE_ENCRYPTED_EXTENSIONS_RECEPTION,  
        TLS_HANDLE_CERTIFICATE_REQUEST_HANDSHAKE,  
        TLS_HANDLE_CERTIFICATE_HANDSHAKE,  
		TLS_PERFORM_CERTIFICATE_VERIFICATION,
        TLS_HANDLE_CERTIFICATE_VERIFY,    
        TLS_HANDLE_SERVER_FINISHED_HANDSHAKE,
		TLS_PERFORM_CERTIFICATE_HANDSHAKE,
		TLS_PERFORM_CERTIFICATE_VERIFY,		
        TLS_PERFORM_CLIENT_FINISHED_HANDSHAKE,
        TLS_PERFORM_CLIENT_APPLICATION_KEYS_CALCULATION        
    },
    { 
        /* Server - Certificate*/
        TLS_HANDLE_CLIENT_HELLO_RECEPTION,    
        TLS_PERFORM_SERVER_HELLO_HANDSHAKE,
        TLS_PERFORM_SERVER_HANDSHAKE_KEYS_CALCULATION,
        TLS_PERFORM_ENCRYPTED_EXTENSIONS_HANDSHAKE,
        TLS_PERFORM_CERTIFICATE_REQUEST_HANDSHAKE,
        TLS_PERFORM_CERTIFICATE_HANDSHAKE,
        TLS_PERFORM_CERTIFICATE_VERIFY,
        TLS_PERFORM_SERVER_FINISHED_HANDSHAKE,
        TLS_PERFORM_SERVER_APPLICATION_KEYS_CALCULATION,    
        TLS_HANDLE_CERTIFICATE_HANDSHAKE,
		TLS_PERFORM_CERTIFICATE_VERIFICATION,
		TLS_HANDLE_CERTIFICATE_VERIFY,
		TLS_HANDLE_CLIENT_FINISHED_HANDSHAKE
    }    
};
#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"


#define TLS_START_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"
static const Tls_HShakeProtoFuncPtrType Tls_GSaaHShakeProtoFuncPtr[TLS_TWENTYFOUR] =
{
    &Tls_PrepareCHMessage,    
    &Tls_HandleClientHelloReception,    
    &Tls_PrepareServerHelloMessage,     
    &Tls_HandleServerHelloReception,    
    &Tls_PerformHandShakeKeyscalc,      
    &Tls_ProcessDummyHandshake,
    &Tls_ProcessDummyHandshake,         
    &Tls_PerformHandShakeKeyscalc,      
    &Tls_PrepareEEMessage,              
    &Tls_HandleEEReception,   
    &Tls_PrepareServerCertReqHSMsg,
    &Tls_HandleServerCRHSMessage,           
    &Tls_PrepareCertHSMessage,            
    &Tls_HandleCertHSMessage,           
    &Tls_PerformCertVerification,         
    &Tls_PrepareCVHSMessage,
    &Tls_HandleCVHSMessage,       
    &Tls_PerformServerFinished,         
    &Tls_HandleServerFinReception,      
    &Tls_PerformClientFinished,         
    &Tls_HandleClientFinReception,      
    &Tls_PerformApplicationKeyscalc,    
    &Tls_PerformApplicationKeyscalc,
    &Tls_ProcessDummyHandshake
};
#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

/*******************************************************************************
** Function Name        : Tls_ProcessHShakeProtocol                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function processes handshake.                  **
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
**  Design ID           : TLS_SDD_0193                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
void  Tls_ProcessHShakeProtocol(const uint16 LusActiveConnIdx)
{
    TcpIp_TlsConnectionIdType LddConnectionId;
    
    LddConnectionId = 
    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
            
    if (Tls_GaaConnection[LddConnectionId].ddTlsConnectionType == TLS_CLIENT)
    {
        Tls_ProcessClientHShakeProto(LusActiveConnIdx);
    }
    else
    {
        Tls_ProcessServerHShakeProto(LusActiveConnIdx);
    }  
 
}/* end of Tls_ProcessHShakeProtocol */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_ProcessClientHShakeProto                        **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function processes handshake.                  **
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
**  Design ID           : TLS_SDD_0204                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_ProcessClientHShakeProto(const uint16 LusActiveConnIdx)
{
    Tls_ElmntsOfHShakeProto  LddHShakeElement;
    
    LddHShakeElement = Tls_GaaFetchHShakeElmnts[LusActiveConnIdx];
    
    /* functionn gets called , depenidng on which stage was previosly completed */
    Tls_GSaaHShakeProtoFuncPtr[LddHShakeElement] (LusActiveConnIdx);
  
}/* end of Tls_ProcessClientHShakeProto*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_ProcessServerHShakeProto                  **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function processes handshake.                  **
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
**  Design ID           : TLS_SDD_0215                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_ProcessServerHShakeProto(const uint16  LusActiveConnIdx)
{
    Tls_ElmntsOfHShakeProto  LddHShakeElement;
    
    LddHShakeElement = Tls_GaaFetchHShakeElmnts[LusActiveConnIdx];
    
    /* functionn gets called , depenidng on which stage was previosly completed */
    Tls_GSaaHShakeProtoFuncPtr[LddHShakeElement] (LusActiveConnIdx); 
 
}/* end of Tls_ProcessServerHShakeProto*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_ProcessDummyHandshake                           **
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
**  Design ID           : TLS_SDD_0767                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_ProcessDummyHandshake(const uint16  LusActiveConnIdx)
{
    /* Dummy Handshake*/ 
    TLS_UNUSED(LusActiveConnIdx);
}/* end of Tls_ProcessServerHShakeProto*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*****************************CLIENT-HANDSHAKES********************************/

/*******************************************************************************
** Function Name        : Tls_PrepareCHMessage                                **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares the Client hello message.    **
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
**  Design ID           : TLS_SDD_0226                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_PrepareCHMessage(const uint16 LusActiveConnIdx)
{
    Std_ReturnType  LddReturnValue;
    uint8    LucLoopCnt;
    uint8*              LpHShakeBuf; 
    uint16              LusCHBufIdx;
    uint16              LusLengthProcessed;  // parasoft-suppress CERT_C-DCL00-a-3 "Reason: Tls_Handshake_c_REF_1."
    TcpIp_TlsConnectionIdType LddConnectionId;
	uint8 LucIdx;


    const Tls_CHProtoFuncPtrType Tls_GSaaCHProtoFuncPtr[TLS_SIX] =
    {
        &Tls_PrepareChLegacyVersion, 
        &Tls_PrepareChRandomNum,        
        &Tls_PrepareChSessionId,  
        &Tls_PrepareChCipherSuites,   
        &Tls_PrepareChCompressMethd,
        &Tls_PrepareChExtensions
    };

  
    LucLoopCnt = (uint8)TLS_ZERO;
    
    LpHShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO]; 
    LddConnectionId = 
    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
    
    /* 4 bytes handshake header */
    LusCHBufIdx = (uint16)TLS_FOUR;    
    do
    {    
        LddReturnValue = Tls_GSaaCHProtoFuncPtr[LucLoopCnt]
                                                    (LddConnectionId,
                                                    &LpHShakeBuf[LusCHBufIdx],
                                                    &LusLengthProcessed);
                                                    
        /* Increment loop count to point to next element */
        LucLoopCnt++;
        LusCHBufIdx += LusLengthProcessed;
    
        /* Loop end to check if any issue occured during preparation or
        all elements are prepared.*/
    }while ((LucLoopCnt < (uint8)TLS_SIX) && (E_OK == LddReturnValue));


    if (E_OK == LddReturnValue)
    {   
        /* set this to use later , during Key serive */
        Tls_HandshakeFrame[LusActiveConnIdx].usProcessedHSBytes = LusCHBufIdx;
        
        Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition += 
        LusCHBufIdx;
        
        /* add record protocol and transmit */                                                    
        LddReturnValue = Tls_TransmitRecord(
                                                LusActiveConnIdx,
                                                TLS_HANDSHAKE,
                                                &LpHShakeBuf[TLS_ZERO],
                                                LusCHBufIdx);                                                      
                    
        if (E_OK == LddReturnValue)
        {
            /* update the state to waiting for SH */
            Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_WAIT_FOR_SH;
            
            /* Increment to point to next Element */
            ++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
            
			LucIdx = (uint8)TLS_CLIENT +
			(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
			
			/* Set the next element of handhsake to be processed by client */
			Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
			Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
				
            #if (STD_ON == TLS_TIMER_SUPPORT)
                Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
                    Tls_GaaConnection[LddConnectionId].usTlsWaitingTimeout;
            #endif                
        }
        else
        {
            Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
            Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);
        }         
    }
    else
    {
        Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
        Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);
    } 
}/* end of Tls_PrepareCHMessage*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_HandleServerHelloReception                      **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function handles the Server hello message.     **
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
**  Design ID           : TLS_SDD_0195                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_HandleServerHelloReception(const uint16 LusActiveConnIdx)
{
    Std_ReturnType      LddReturnValue; 
    uint8*              LpHShakeBuf;    
    uint16              LusHShakeBufferLen;
	uint8 LucIdx;
  
    if (TLS_CLIENT_WAIT_FOR_SH == Tls_GaaClientState[LusActiveConnIdx])
    {
        /* Timeout mechanism to detect and handle scenarios where the Server Hello
        message is not received within the expected timeframe.*/
        #if (STD_ON == TLS_TIMER_SUPPORT)                   
            if ((uint16)TLS_ONE < 
                Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout)        
            {   
                Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout -= 
                (uint16)TLS_ONE;            
            }
            else
            {
                Tls_GddConnHSStatus[Tls_GddActiveConnManager[LusActiveConnIdx].
                                ddTlsConnectionId] = TLS_CONNECTION_FAILED;
            
                Tls_ConnStatsNotification(
                Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
                TLS_CONNECTION_FAILED);                
            }
        #endif
    }
    else if (TLS_CLIENT_RECVD_SH == Tls_GaaClientState[LusActiveConnIdx])
    {
		/* this will tell us where the CH msg ends and SH starts */
        LusHShakeBufferLen =
        Tls_HandshakeFrame[LusActiveConnIdx].usProcessedHSBytes;
    
        LpHShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusHShakeBufferLen];
    
        LddReturnValue = Tls_ValidateServerHello(LusActiveConnIdx,
                                        &LpHShakeBuf[TLS_ZERO]);
                                              
        if (E_OK == LddReturnValue)
        {
            
            Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_WAIT_FOR_EE;
            
            /* If No Issue */
            /* Increment to point to next Handshake Element */
            ++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
     						   
			LucIdx = (uint8)TLS_CLIENT +
			(uint8)Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
											ddTlsConnectionId].ddTlsAuthType;
			
			/* Set the next element of handhsake to be processed by client */
			Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
			Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]]; 
			
            #if (STD_ON == TLS_TIMER_SUPPORT)
                Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
                Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
                                        ddTlsConnectionId].usTlsWaitingTimeout;
            #endif 
        }   
    }
    else
    {
        /* Not expected - Internal Error */
    }    
  
}/* end of Tls_HandleServerHelloReception*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_HandleEEReception                               **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function handle reception of Encrypted         **
**                        Extension.                                          **
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
**  Design ID           : TLS_SDD_0196                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_HandleEEReception(const uint16 LusActiveConnIdx)
{
    Std_ReturnType      LddReturnValue; 
    Tls_AuthType LddAuthType;
    Tls_FrameType*      LpTlsFrame;
    uint8*              LpHShakeBuf;    
    uint16              LusEELen;
	uint8 LucIdx;

    LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];
    
    LpHShakeBuf = 
		&Tls_GaaHShakeBuffer[LusActiveConnIdx][LpTlsFrame->usProcessedHSBytes];

	if (TLS_CLIENT_WAIT_FOR_EE == Tls_GaaClientState[LusActiveConnIdx])
    {    
        if (LpTlsFrame->usUnProcessedData > LpTlsFrame->usProcessedData)
        {
            Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_RECVD_EE;
        }
        else
        {
            /* Do Nothing */
            #if (STD_ON == TLS_TIMER_SUPPORT)
                Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout -= 
                (uint16)TLS_ONE;
                    
                if ((uint16)TLS_ZERO == 
                    Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout)
                {
                    Tls_GddConnHSStatus[Tls_GddActiveConnManager
                                    [LusActiveConnIdx].ddTlsConnectionId] = 
                                                        TLS_CONNECTION_FAILED;
                
                    Tls_ConnStatsNotification(
                    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
                    TLS_CONNECTION_FAILED);                
                }
            #endif
        }
    }
    else if (TLS_CLIENT_RECVD_EE == Tls_GaaClientState[LusActiveConnIdx])
    {
        LddReturnValue = Tls_ValidateEE(LusActiveConnIdx, 
                                            &LpHShakeBuf[TLS_ZERO], &LusEELen);
        
        if (E_OK == LddReturnValue)
        {
            LpTlsFrame->usProcessedData += LusEELen;
			/* now it point to end of EE */
			LpTlsFrame->usProcessedHSBytes += LusEELen;
            
            LddAuthType = Tls_GaaConnection[Tls_GddActiveConnManager[
                            LusActiveConnIdx].ddTlsConnectionId].ddTlsAuthType;
            
            /* this will point to the next byte , After the EE HS frame ends*/  
            if ((LpTlsFrame->usUnProcessedData) > (LpTlsFrame->usProcessedData))
            {
                if (TLS_AUTH_PSK == LddAuthType)
                {
                    Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_RECVD_FINSHED;
                }
                else
                {
                    Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_RECVD_CR;
                }
            }
            else
            {
                if (TLS_AUTH_PSK == LddAuthType)
                {
                    Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_WAIT_FOR_FINSHED;
                }
                else
                {
                    Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_WAIT_FOR_CR;
                }
            }         
                  
            /* If No Issue */
            /* Increment to point to next Element */
            ++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
            
			LucIdx = (uint8)TLS_CLIENT +
			(uint8)Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
											ddTlsConnectionId].ddTlsAuthType;
			
			/* Set the next element of handhsake to be processed by client */
			Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
			Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]]; 
				
            #if (STD_ON == TLS_TIMER_SUPPORT)
                Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
                Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
                                        ddTlsConnectionId].usTlsWaitingTimeout;
            #endif
		}										
        else
        {
            Tls_GddConnHSStatus[Tls_GddActiveConnManager[LusActiveConnIdx].
                                ddTlsConnectionId] = TLS_CONNECTION_FAILED;
                
            Tls_ConnStatsNotification(
                Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
                TLS_CONNECTION_FAILED);
        }                                
    }
    else
    {
        /* Not expected - Internal Error */
    }
  
}/* end of Tls_HandleEEReception*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_PrepareServerHelloMessage                       **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares the Server hello message.    **
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
**  Design ID           : TLS_SDD_0328                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PrepareServerHelloMessage(const uint16 LusActiveConnIdx)
{
    Std_ReturnType  LddReturnValue;
    uint8    LucLoopCnt;
    uint8*    LpHShakeBuf;
    uint16    LusSHIdx;
    uint32    LulSHBufIdx;
    uint16    LusLengthProcessed; // parasoft-suppress CERT_C-DCL00-a-3 "Reason: Tls_Handshake_c_REF_1."   
    uint16    LusSHLen;   
    TcpIp_TlsConnectionIdType LddConnectionId;
	uint8 LucIdx;


    const Tls_SHProtocolFuncPtrType Tls_GSaaSHProtocolFuncPtr[TLS_SIX] =
    {
        &Tls_PrepareShLegacyVersion, 
        &Tls_PrepareShRandomNumber,        
        &Tls_PrepareShSessionId,  
        &Tls_PrepareShCipherSuite,   
        &Tls_PrepareShCompressionMethod,
        &Tls_PrepareShExtensions
    };

  
    LddConnectionId = 
    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
 
    LucLoopCnt = (uint8)TLS_ZERO; 
    LusSHIdx = Tls_HandshakeFrame[LusActiveConnIdx].usProcessedHSBytes; 
    LpHShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusSHIdx];
    /* 4 bytes handshake header */
    LulSHBufIdx = (uint32)TLS_FOUR;   
 
    do
    {
        /* Invoke Function to prepare specific element data */
        LddReturnValue = Tls_GSaaSHProtocolFuncPtr[LucLoopCnt]
                                            (LddConnectionId,
                                            &LpHShakeBuf[LulSHBufIdx],
                                            &LusLengthProcessed);
           
        /* Increment loop count to point to next element */
        LucLoopCnt++;
        LulSHBufIdx += (uint32)LusLengthProcessed;
        /* Loop end to check if any issue occured during preparation or
        all elements are prepared.*/
    }while ((LucLoopCnt < (uint8)TLS_SIX) && (E_OK == LddReturnValue));
 

    if (E_OK == LddReturnValue)
    {

        /* update this here , to use during Key Derive */
        Tls_HandshakeFrame[LusActiveConnIdx].usProcessedHSBytes += 
        (uint16)(LulSHBufIdx & (uint32)TLS_LSB_MASK_U16);

        Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition += 
        (uint16)(LulSHBufIdx & (uint32)TLS_LSB_MASK_U16);
        
        LusSHLen = (uint16)(LulSHBufIdx & (uint32)TLS_LSB_MASK_U16);

        LulSHBufIdx -= (uint32)TLS_FOUR;
  
        /* update for handshake header */ 
        LpHShakeBuf[TLS_ZERO] = (uint8)TLS_SERVER_HELLO;
  
        LpHShakeBuf[TLS_ONE] =
        (uint8)((LulSHBufIdx >> (uint32)TLS_SIXTEEN) & (uint32)TLS_LSB_MASK);
        
        LpHShakeBuf[TLS_TWO] = 
        (uint8)((LulSHBufIdx >> (uint32)TLS_EIGHT) & (uint32)TLS_LSB_MASK);
        
        LpHShakeBuf[TLS_THREE] = (uint8)(LulSHBufIdx & (uint32)TLS_LSB_MASK);


        /* add record protocol and transmit */                                               
        LddReturnValue = Tls_TransmitRecord(
                                                LusActiveConnIdx,
                                                TLS_HANDSHAKE,
                                                &LpHShakeBuf[TLS_ZERO],
                                                LusSHLen);                                                  
                                                    
        if (E_OK == LddReturnValue)
        {
            /* update the state to waiting for SH */
            Tls_GaaServerState[LusActiveConnIdx] = TLS_SERVER_NEGOTIATED; 
                
            /* Increment to point to next Element */
            ++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
            						
			LucIdx = (uint8)TLS_SERVER +
			(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
			
			/* Set the next element of handhsake to be processed by Server */
			Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
			Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
				
				
            #if (STD_ON == TLS_TIMER_SUPPORT)
                Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
                    Tls_GaaConnection[LddConnectionId].usTlsWaitingTimeout;
            #endif
        }
        else
        {
            /* Need more to done TBD */
            Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
            Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);
        }    
    }
    else
    {
        Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
        Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);        
    }
 
}/* end of Tls_PrepareServerHelloMessage */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_HandleServerFinReception                        **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function handle reception of Server            **
**                        Finished Message.                                   **
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
**  Design ID           : TLS_SDD_0197                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_HandleServerFinReception(const uint16 LusActiveConnIdx)
{
    Std_ReturnType LddReturnValue;
    TcpIp_TlsConnectionIdType LddConnectionId;
    Tls_FrameType*      LpTlsFrame;
	uint8 LucIdx;
    
	LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];
	
	if (TLS_CLIENT_WAIT_FOR_FINSHED == Tls_GaaClientState[LusActiveConnIdx])
    {
        if (LpTlsFrame->usUnProcessedData > LpTlsFrame->usProcessedData)
        {
            Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_RECVD_FINSHED;
        }
        else
        {
            /* Do Nothing */
            #if (STD_ON == TLS_TIMER_SUPPORT)
                Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout -= 
                (uint16)TLS_ONE;
                    
                if ((uint16)TLS_ZERO == 
                    Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout)
                {
                    Tls_GddConnHSStatus[Tls_GddActiveConnManager
                                    [LusActiveConnIdx].ddTlsConnectionId] = 
                                                        TLS_CONNECTION_FAILED;
                
                    Tls_ConnStatsNotification(
                    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
                    TLS_CONNECTION_FAILED);                
                }
            #endif
        }
    }
    else if (TLS_CLIENT_RECVD_FINSHED == Tls_GaaClientState[LusActiveConnIdx])
    {
        /* Validate Server Finished */
        LddConnectionId = 
        Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
        
        LddReturnValue = Tls_VerifyFinishMessage(LusActiveConnIdx);
        
        if (E_OK == LddReturnValue)
        {
			/* flow changes from here for Mutual TLS Client side 
			it depends on wether client recived Server Certificte Req HS msg 
			and wether it was validated succfulley 
			if YES: Prepare CERT and CERT VERIFY Msg 
			if NO : PREPARE CLIENT FIN Msg */
			/* this will be only true , when Cert Req was validated succesfully */
			#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)	
			if (LpTlsFrame->blIsCertReqReceived)
			{
				++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];				
			}
			else
			{
				/* Skip Cert HS and Cert Cerify HS msg */
				Tls_GaaHandshakeElementIndex[LusActiveConnIdx] += TLS_THREE;
			}
			#else
				++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];	
			#endif
		    							   
			LucIdx = (uint8)TLS_CLIENT +
			(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
			
			/* Set the next element of handhsake to be processed by client */
			Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
			Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  			
            
            #if (STD_ON == TLS_TIMER_SUPPORT)
                Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
                Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
                                            ddTlsConnectionId].usTlsWaitingTimeout;
            #endif           
        }
        else
        {
            Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
            Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);  
        }                                 
    }
    else
    {
        /* Not expected - Internal Error */
    }
}/* end of Tls_HandleServerFinReception*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PerformClientFinished                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares client finished message.     **
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
**  Design ID           : TLS_SDD_0237                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_PerformClientFinished(const uint16 LusActiveConnIdx)
{
    Std_ReturnType LddReturnValue;
    TcpIp_TlsConnectionIdType LddConnectionId;
	uint8 LucIdx;
    
    LddConnectionId = 
    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;

    LddReturnValue = Tls_ProcessFinishedMessage(LusActiveConnIdx);                                
                                    
    if (E_OK == LddReturnValue)
    {
        /* If No Issue */
        /* Increment to point to next Element */
        ++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
        							
		LucIdx = (uint8)TLS_CLIENT +
		(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
		
		/* Set the next element of handhsake to be processed by Server */
		Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
		Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
				
    }
    else
    {
        Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
        Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);      
    }    
}/* end of Tls_PerformClientFinished*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_PrepareServerCertReqHSMsg                       **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares the Certificate message.    **
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
**  Design ID           : TLS_SDD_0385                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PrepareServerCertReqHSMsg(const uint16 LusActiveConnIdx)
{
    #if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
		Std_ReturnType  LddReturnValue;
		TcpIp_TlsConnectionIdType LddConnectionId;
		uint8 LucIdx;
		
		LddConnectionId = 
		Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
		
		LddReturnValue = Tls_ProcessCertRequestMsg(LusActiveConnIdx);
		
		if (E_OK == LddReturnValue)
		{            
			/* Increment to point to next Element */
			++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
		
			LucIdx = (uint8)TLS_SERVER +
			(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
			
			/* Set the next element of handhsake to be processed by Server */
			Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
			Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
					
			#if (STD_ON == TLS_TIMER_SUPPORT)
				Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
				Tls_GaaConnection[LddConnectionId].usTlsWaitingTimeout;
			#endif
		}
		else
		{
			/* Need more to done TBD */
			Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
			Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);
		}    
    #else
		TLS_UNUSED(LusActiveConnIdx);
    #endif
}/* end of Tls_PrepareServerCertReqHSMsg */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_PrepareCertHSMessage                       **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares the Certificate message.    **
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
**  Design ID           : TLS_SDD_0385                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_PrepareCertHSMessage(const uint16 LusActiveConnIdx)
{
    /* Asummption , there will always be oonly one certificate configured in 
    server or client , since need more anlaysis on SNI and Trusted CA keys */
    #if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
    Std_ReturnType  LddReturnValue;
    TcpIp_TlsConnectionIdType LddConnectionId;
	Tls_ConnectionType LddConType;
	uint8 LucIdx;
	boolean LblClientAuthReq;

    LddConnectionId = 
    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
	
    /*creating a single function , so that it might work during clinet sending
    its certificate also */
    LddReturnValue = Tls_ProcessCertMessage(LusActiveConnIdx);

    if (E_OK == LddReturnValue)
    {
		/* flow changes from here for Mutual TLS Client side 
		if Empty Certificate was sent by client , then dont send Cert Verify Hs msg
		move to HandShake Msg */
		
		LddConType = Tls_GaaConnection[LddConnectionId].ddTlsConnectionType;
		
		if (TLS_CLIENT == LddConType)
		{
			LblClientAuthReq = Tls_GaaConnection[LddConnectionId].blTlsUseClintAuthReq;
			
			if (LblClientAuthReq)
			{
				++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];				
			}
			else
			{
				/* Skip CERT Verify and move to Send Client Finished */
				Tls_GaaHandshakeElementIndex[LusActiveConnIdx] += TLS_TWO;
			}			
		}
		else
		{
			++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];	
		}
		
		LucIdx = (uint8)LddConType +
		(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
		
		/* Set the next element of handhsake to be processed by Server */
		Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
		Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
				
        #if (STD_ON == TLS_TIMER_SUPPORT)
            Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
            Tls_GaaConnection[LddConnectionId].usTlsWaitingTimeout;
        #endif
    }
    else
    {
        /* Need more to done TBD */
        Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
        Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);
    }    
    #else
    TLS_UNUSED(LusActiveConnIdx);
    #endif
}/* end of Tls_PrepareCertMessage */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_HandleCertHSMessage                       **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares the Certificate message.    **
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
**  Design ID           : TLS_SDD_0386                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void  Tls_HandleCertHSMessage(const uint16 LusActiveConnIdx)
{
	#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
		Std_ReturnType LddReturnValue;
		Tls_FrameType*      LpTlsFrame;
		Tls_ConnectionType LddConType;
		uint8* LpHShakeBuf;
		uint8 LucIdx;
		
		LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];
		
		LpHShakeBuf = 
			&Tls_GaaHShakeBuffer[LusActiveConnIdx][LpTlsFrame->usProcessedHSBytes];
		
		LddConType = Tls_GaaConnection[Tls_GddActiveConnManager
						[LusActiveConnIdx].ddTlsConnectionId].ddTlsConnectionType;
	
		if (TLS_CLIENT == LddConType)
		{
			if (TLS_CLIENT_WAIT_FOR_CERT == Tls_GaaClientState[LusActiveConnIdx])
			{
				if (LpTlsFrame->usUnProcessedData > LpTlsFrame->usProcessedData)
				{
					Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_RECVD_CERT;
				}
				else
				{
					/* Do Nothing */
					#if (STD_ON == TLS_TIMER_SUPPORT)
						Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout -= 
						(uint16)TLS_ONE;
							
						if ((uint16)TLS_ZERO == 
							Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout)
						{
							Tls_GddConnHSStatus[Tls_GddActiveConnManager
											[LusActiveConnIdx].ddTlsConnectionId] = 
																TLS_CONNECTION_FAILED;
						
							Tls_ConnStatsNotification(
							Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
							TLS_CONNECTION_FAILED);                
						}
					#endif
				}
			}
			else if (TLS_CLIENT_RECVD_CERT == Tls_GaaClientState[LusActiveConnIdx])
			{
				LddReturnValue = Tls_ValidateCertMsg(LusActiveConnIdx, LpHShakeBuf);
						
				if (E_OK == LddReturnValue)
				{		      
					/* If No Issue */
					/* Increment to point to next Element */
					++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
		
					LucIdx = (uint8)TLS_CLIENT +
					(uint8)Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
													ddTlsConnectionId].ddTlsAuthType;
					
					/* Set the next element of handhsake to be processed by client */
					Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
					Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
					
					
					#if (STD_ON == TLS_TIMER_SUPPORT)
						Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
						Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
												ddTlsConnectionId].usTlsWaitingTimeout;
					#endif
				}										
				else
				{
					Tls_GddConnHSStatus[Tls_GddActiveConnManager[LusActiveConnIdx].
										ddTlsConnectionId] = TLS_CONNECTION_FAILED;
						
					Tls_ConnStatsNotification(
						Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
						TLS_CONNECTION_FAILED);
				}
			}
			else
			{
				/* do Nothing */
			}	
		}
		else
		{
			/* when Certificate is sent by Client */
			if (TLS_SERVER_WAIT_FOR_CERT == Tls_GaaServerState[LusActiveConnIdx])
			{
				if (LpTlsFrame->usUnProcessedData > LpTlsFrame->usProcessedData)
				{
					Tls_GaaServerState[LusActiveConnIdx] = TLS_SERVER_RECVD_CERT;
				}
				else
				{
					/* Do Nothing */
					#if (STD_ON == TLS_TIMER_SUPPORT)
						Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout -= 
						(uint16)TLS_ONE;
							
						if ((uint16)TLS_ZERO == 
							Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout)
						{
							Tls_GddConnHSStatus[Tls_GddActiveConnManager
											[LusActiveConnIdx].ddTlsConnectionId] = 
																TLS_CONNECTION_FAILED;
						
							Tls_ConnStatsNotification(
							Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
							TLS_CONNECTION_FAILED);                
						}
					#endif
				}
			}
			else if (TLS_SERVER_RECVD_CERT == Tls_GaaServerState[LusActiveConnIdx])
			{
				LddReturnValue = Tls_ValidateCertMsg(LusActiveConnIdx, LpHShakeBuf);
						
				if (E_OK == LddReturnValue)
				{		      
					/* If No Issue */
					/* Increment to point to next Element */
					++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
		
					LucIdx = (uint8)TLS_SERVER +
					(uint8)Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
													ddTlsConnectionId].ddTlsAuthType;
					
					/* Set the next element of handhsake to be processed by client */
					Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
					Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
					
					
					#if (STD_ON == TLS_TIMER_SUPPORT)
						Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
						Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
												ddTlsConnectionId].usTlsWaitingTimeout;
					#endif
				}										
				else
				{
					Tls_GddConnHSStatus[Tls_GddActiveConnManager[LusActiveConnIdx].
										ddTlsConnectionId] = TLS_CONNECTION_FAILED;
						
					Tls_ConnStatsNotification(
						Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
						TLS_CONNECTION_FAILED);
				}
			}
			else
			{
				/* do Nothing */
			}	
		}
	#else
		TLS_UNUSED(LusActiveConnIdx);
	#endif	
}/* end of Tls_HandleCertHSMessage */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_HandleClientHelloReception                      **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function handles the Client hello message.     **
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
**  Design ID           : TLS_SDD_0198                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_HandleClientHelloReception(const uint16 LusActiveConnIdx)
{
    Std_ReturnType      LddReturnValue; 
    uint8*              LpCHShakeBuf;    
    uint16              LusCHBufIdx;
	uint8 LucIdx;
    
    if (TLS_SERVER_WAIT_FOR_CH == Tls_GaaServerState[LusActiveConnIdx])
    {
        /* Timeout mechanism to detect and handle scenarios where the Client Hello
        message is not received within the expected timeframe after opening the
        Tls Connection.*/
        #if (STD_ON == TLS_TIMER_SUPPORT)                   
            if ((uint16)TLS_ONE < 
                  Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout)        
            {   
                Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout -= 
                (uint16)TLS_ONE;            
            }
            else
            {
                Tls_GddConnHSStatus[Tls_GddActiveConnManager[LusActiveConnIdx].
                                ddTlsConnectionId] = TLS_CONNECTION_FAILED;
                
                    Tls_ConnStatsNotification(
                    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
                    TLS_CONNECTION_FAILED);                
            }
        #endif
    }
    else if (TLS_SERVER_RECVD_CH == Tls_GaaServerState[LusActiveConnIdx])
    {
        /* Initiate validation of the Client Hello message to ensure protocol
        compliance. */        
        LpCHShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO];
        
        LusCHBufIdx = (uint16)TLS_ZERO;
        
        LddReturnValue = Tls_ValidateClientHello(LusActiveConnIdx,
                                                &LpCHShakeBuf[LusCHBufIdx]);
        /* If No Issue */                                    
        if (E_OK == LddReturnValue)
        {                                   
            /* Increment to point to next Handshake Element */
            ++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
							
			LucIdx = (uint8)TLS_SERVER +
			(uint8)Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
											ddTlsConnectionId].ddTlsAuthType;
			
			/* Set the next element of handhsake to be processed by Server */
			Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
			Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
				
            #if (STD_ON == TLS_TIMER_SUPPORT)
                Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
            Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
                                        ddTlsConnectionId].usTlsWaitingTimeout;
            #endif                             
        }
    }
    else
    {
        /* Not expected - Internal Error */
    }
  
}/* end of Tls_HandleClientHelloReception*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PerformHandShakeKeyscalc                         **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function derives server side handshake keys.   **
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
**  Design ID           : TLS_SDD_0194                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_PerformHandShakeKeyscalc(const uint16 LusActiveConnIdx)
{

    Std_ReturnType LddReturnValue;  
    TcpIp_TlsConnectionIdType LddConnectionId;     
	uint8 LucIdx;	

    if ((TLS_SERVER_NEGOTIATED == Tls_GaaServerState[LusActiveConnIdx]) ||
        (TLS_CLIENT_WAIT_FOR_EE == Tls_GaaClientState[LusActiveConnIdx]))
    {

        LddReturnValue = Tls_DeriveESMaterial(LusActiveConnIdx);
        
        if (E_OK == LddReturnValue)
        {
            LddReturnValue = Tls_DeriveHSSecretMaterial(LusActiveConnIdx);
        }
		
        if (E_OK == LddReturnValue)
        {
            LddConnectionId = 
            Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
    
            /* Increment to point to next Element */
            ++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
            
            /* since this function can be calle by server and client also
            so updating here based on client or server */
            if (Tls_GaaConnection[LddConnectionId].ddTlsConnectionType == 
                                                                    TLS_CLIENT)
            {
				LucIdx = (uint8)TLS_CLIENT +
				(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;              
            }
            else
            {
				LucIdx = (uint8)TLS_SERVER +
				(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;            
            }

			/* Set the next element of handhsake to be processed by Server */
			Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
			Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
				
            Tls_HandshakeFrame[LusActiveConnIdx].blIsHSKeysDerived = TLS_TRUE;
                                
            #if (STD_ON == TLS_TIMER_SUPPORT)
                Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
                        Tls_GaaConnection[LddConnectionId].usTlsWaitingTimeout;
            #endif        
        }
        else
        {
            Tls_GddConnHSStatus[Tls_GddActiveConnManager[LusActiveConnIdx].
                                    ddTlsConnectionId] = TLS_CONNECTION_FAILED;
   
            Tls_ConnStatsNotification(
                Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
                TLS_CONNECTION_FAILED);
        }    
    }
    else
    {
        /* Do nothing */
    }
      
}/* end of Tls_PerformHandShakeKeyscalc*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PrepareEEMessage                                **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares Encrypted Extension message. **
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
**  Design ID           : TLS_SDD_0280                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_PrepareEEMessage(const uint16 LusActiveConnIdx)
{
    Std_ReturnType  LddReturnValue;
    TcpIp_TlsConnectionIdType LddConnectionId;
	Tls_AuthType LddAuthType;
    uint8* LpHShakeBuf;
    uint16 LusEEIdx;
    uint8  LaaInnerPlainText[TLS_SEVEN];  
	uint8 LucIdx;
    boolean LblClientAuthReq;
    
    LddConnectionId = 
    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;

    /* from where to store the EE message */
    LusEEIdx = Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition; 
    LpHShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusEEIdx];
    
    /* update for handshake header
    0x08, 0x00, 0x00, 0x02, 0x00, 0x00 */ 
    LpHShakeBuf[TLS_ZERO] = (uint8)TLS_ENCRYPTED_EXTENSIONS;     
    LpHShakeBuf[TLS_ONE] = (uint8)TLS_ZERO; 
    LpHShakeBuf[TLS_TWO] = (uint8)TLS_ZERO; 
    LpHShakeBuf[TLS_THREE] = (uint8)TLS_TWO;
    LpHShakeBuf[TLS_FOUR] = (uint8)TLS_ZERO;    
    LpHShakeBuf[TLS_FIVE] = (uint8)TLS_ZERO;


    Tls_MemCpy(&LaaInnerPlainText[TLS_ZERO], &LpHShakeBuf[TLS_ZERO],
                                                    (uint16)TLS_SIX);
                                                    
    LaaInnerPlainText[TLS_SIX] = (uint8)TLS_HANDSHAKE;  
    
    LddReturnValue = Tls_TransmitRecord(
                                        LusActiveConnIdx,
                                        TLS_APPLICATION_DATA,
                                        &LaaInnerPlainText[TLS_ZERO],
                                        (uint16)TLS_SEVEN); 
                                         
    if (E_OK == LddReturnValue)
    {   
        /* Increment to point to next Element */
        ++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];

		LddAuthType = Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
							
		LucIdx = (uint8)TLS_SERVER + (uint8)LddAuthType;
		
		if (TLS_AUTH_CERT == LddAuthType)
		{
			/* check if the connection is configured or Client Authentication Required */  
			LblClientAuthReq = Tls_GaaConnection[LddConnectionId].blTlsUseClintAuthReq;  
			
			if (!LblClientAuthReq)
			{
				/* if not configured, then move to prepare of Certificte HS msg */
				++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
			}			
		}
		
		/* Set the next element of handhsake to be processed by Server */
		Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
		Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
        
        /* 0x08, 0x00, 0x00, 0x01, 0x00*/
        Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition += 
        (uint16)TLS_SIX;
        Tls_HandshakeFrame[LusActiveConnIdx].usProcessedHSBytes += 
        (uint16)TLS_SIX;   
        #if (STD_ON == TLS_TIMER_SUPPORT)
            Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
                Tls_GaaConnection[LddConnectionId].usTlsWaitingTimeout;
        #endif
    }
    else
    {
        Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
        Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);
    } 
    
}/* end of Tls_PrepareEEMessage*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/* Assisted by GitHub Copilot */
/*******************************************************************************
** Function Name        : Tls_PrepareCVHSMessage                        **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares certificate verify message.  **
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
**  Design ID           : TLS_SDD_0388                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_PrepareCVHSMessage(const uint16 LusActiveConnIdx)
{
	#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
    Std_ReturnType LddRetrnVal;
    TcpIp_TlsConnectionIdType LddConnectionId;
	Tls_ConnectionType LddConType;
	uint8 LucIdx;

    LddConnectionId = Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;

    /* prepares CV verify and Transmits*/
    LddRetrnVal = Tls_PrepCVHSMessage(LusActiveConnIdx);

    /* Step 5: Advance handshake state or handle error */
    if (E_OK == LddRetrnVal)
    {
        ++Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
		
		LddConType = Tls_GaaConnection[LddConnectionId].ddTlsConnectionType;
		
		LucIdx = (uint8)LddConType +
		(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
		
		/* Set the next element of handhsake to be processed by Server */
		Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
		Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
				
        #if (STD_ON == TLS_TIMER_SUPPORT)
        Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout =
            Tls_GaaConnection[LddConnectionId].usTlsWaitingTimeout;
        #endif
    }
    else
    {
        Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
        Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);
    }
	#else
		TLS_UNUSED(LusActiveConnIdx);
	#endif
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_HandleCVHSMessage                  **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares certificate verify message.  **
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
**  Design ID           : TLS_SDD_0389                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_HandleCVHSMessage(const uint16 LusActiveConnIdx)
{
	/* redsign later to make comman code */
    #if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
		Std_ReturnType LddReturnValue;
		TcpIp_TlsConnectionIdType LddConnectionId;
        Tls_FrameType*      LpTlsFrame;
		Tls_ConnectionType LddConType;
        uint16 LusCVHSLen;
		uint8 LucIdx;

        LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];

		LddConnectionId = 
		Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
		
		LddConType = Tls_GaaConnection[LddConnectionId].ddTlsConnectionType;
								
		if (LddConType == TLS_SERVER)
		{
			if (TLS_SERVER_WAIT_FOR_CV == Tls_GaaServerState[LusActiveConnIdx])
			{
				if (LpTlsFrame->usUnProcessedData > LpTlsFrame->usProcessedData)
				{
					Tls_GaaServerState[LusActiveConnIdx] = TLS_SERVER_RECVD_CV;
				}
				else
				{
					/* Do Nothing */
					#if (STD_ON == TLS_TIMER_SUPPORT)
						Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout -= 
						(uint16)TLS_ONE;
						
						if ((uint16)TLS_ZERO == 
						Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout)
						{
							Tls_GddConnHSStatus[Tls_GddActiveConnManager
										[LusActiveConnIdx].ddTlsConnectionId] = 
															TLS_CONNECTION_FAILED;
					
							Tls_ConnStatsNotification(
							Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
							TLS_CONNECTION_FAILED);                
						}
					#endif
				}
			}
			else if (TLS_SERVER_RECVD_CV == Tls_GaaServerState[LusActiveConnIdx])
			{
				LddReturnValue = Tls_ValidateCVMSg(LusActiveConnIdx, &LusCVHSLen);
	
				if (E_OK == LddReturnValue)
				{
					LpTlsFrame->usProcessedData += LusCVHSLen;
					LpTlsFrame->usProcessedHSBytes += LusCVHSLen;
	
					/* If No Issue */
					/* Increment to point to next Element */
					++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
	
					if ((LpTlsFrame->usUnProcessedData) > (LpTlsFrame->usProcessedData))
					{
						Tls_GaaServerState[LusActiveConnIdx] = 
														TLS_SERVER_RECVD_FINISHED;
					}
					else
					{
						Tls_GaaServerState[LusActiveConnIdx] = 
														TLS_SERVER_WAIT_FOR_FINISHED;
					}
	
					LucIdx = (uint8)TLS_SERVER +
					(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
		
					/* Set the next element of handhsake to be processed by client */
					Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
					Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
					
					#if (STD_ON == TLS_TIMER_SUPPORT)
						Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
						Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
												ddTlsConnectionId].usTlsWaitingTimeout;
					#endif
				}
				else
				{
					Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
					Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);     
				}
			}
			else
			{
				/* Not expected - Internal Error */
			}			
		}
		else
		{
			if (TLS_CLIENT_WAIT_FOR_CV == Tls_GaaClientState[LusActiveConnIdx])
			{
				if (LpTlsFrame->usUnProcessedData > LpTlsFrame->usProcessedData)
				{
					Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_RECVD_CV;
				}
				else
				{
					/* Do Nothing */
					#if (STD_ON == TLS_TIMER_SUPPORT)
						Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout -= 
						(uint16)TLS_ONE;
						
						if ((uint16)TLS_ZERO == 
						Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout)
						{
							Tls_GddConnHSStatus[Tls_GddActiveConnManager
										[LusActiveConnIdx].ddTlsConnectionId] = 
															TLS_CONNECTION_FAILED;
					
							Tls_ConnStatsNotification(
							Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
							TLS_CONNECTION_FAILED);                
						}
					#endif
				}
			}
			else if (TLS_CLIENT_RECVD_CV == Tls_GaaClientState[LusActiveConnIdx])
			{
				LddReturnValue = Tls_ValidateCVMSg(LusActiveConnIdx, &LusCVHSLen);
	
				if (E_OK == LddReturnValue)
				{
					LpTlsFrame->usProcessedData += LusCVHSLen;
					LpTlsFrame->usProcessedHSBytes += LusCVHSLen;
	
					/* If No Issue */
					/* Increment to point to next Element */
					++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
	
					if ((LpTlsFrame->usUnProcessedData) > (LpTlsFrame->usProcessedData))
					{
						Tls_GaaClientState[LusActiveConnIdx] = 
														TLS_CLIENT_RECVD_FINSHED;
					}
					else
					{
						Tls_GaaClientState[LusActiveConnIdx] = 
														TLS_CLIENT_WAIT_FOR_FINSHED;
					}
	
					LucIdx = (uint8)TLS_CLIENT +
					(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
		
					/* Set the next element of handhsake to be processed by client */
					Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
					Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
					
					#if (STD_ON == TLS_TIMER_SUPPORT)
						Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
						Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
												ddTlsConnectionId].usTlsWaitingTimeout;
					#endif
				}
				else
				{
					Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
					Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);     
				}
			}
			else
			{
				/* Not expected - Internal Error */
			}		
		}
	#else
		TLS_UNUSED(LusActiveConnIdx);
	#endif
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PerformServerFinished                           **
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
**  Design ID           : TLS_SDD_0248                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_PerformServerFinished(const uint16 LusActiveConnIdx)
{
    Std_ReturnType LddReturnValue;
    TcpIp_TlsConnectionIdType LddConnectionId;
	uint8 LucIdx;
    
    LddConnectionId = 
    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;

    LddReturnValue = Tls_ProcessFinishedMessage(LusActiveConnIdx);                                
                                    
    if (E_OK == LddReturnValue)
    {
        /* If No Issue */
        /* Increment to point to next Element */
        ++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
        
		LucIdx = (uint8)TLS_SERVER +
		(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
		
		/* Set the next element of handhsake to be processed by Server */
		Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
		Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
				
        #if (STD_ON == TLS_TIMER_SUPPORT)
            Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
                Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
                                        ddTlsConnectionId].usTlsWaitingTimeout;
        #endif 
    }
    else
    {
        Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
        Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);      
    }    
}/* end of Tls_PerformServerFinished*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_HandleClientFinReception                        **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function handle reception of Client            **
**                        Finished Message.                                   **
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
**  Design ID           : TLS_SDD_0269                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_HandleClientFinReception(const uint16 LusActiveConnIdx)
{
    Std_ReturnType LddReturnValue;
    TcpIp_TlsConnectionIdType LddConnectionId;
    Tls_FrameType*      LpTlsFrame;
	uint8 LucIdx;
 
    if (TLS_SERVER_WAIT_FOR_FINISHED == Tls_GaaServerState[LusActiveConnIdx])
    {
        LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];
        
        if (LpTlsFrame->usUnProcessedData > LpTlsFrame->usProcessedData)
        {
            Tls_GaaServerState[LusActiveConnIdx] = TLS_SERVER_RECVD_FINISHED;
        }
        else
        {
            /* Do Nothing */
            #if (STD_ON == TLS_TIMER_SUPPORT)
                Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout -= 
                (uint16)TLS_ONE;
                        
                if ((uint16)TLS_ZERO == 
                        Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout)
                {
    
                    Tls_GddConnHSStatus[Tls_GddActiveConnManager[LusActiveConnIdx].
                                    ddTlsConnectionId] = TLS_CONNECTION_FAILED;
                                    
                    Tls_ConnStatsNotification(
                        Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
                        TLS_CONNECTION_FAILED);
                }
            #endif
        }
    }
    else if (TLS_SERVER_RECVD_FINISHED == Tls_GaaServerState[LusActiveConnIdx])
    {
        LddConnectionId = 
        Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
        /* Validate Client Finished */
        LddReturnValue = Tls_VerifyFinishMessage(LusActiveConnIdx);
        
        if (E_OK == LddReturnValue)
        {
            /* If No Issue */
            /* Increment to point to next Element */
            ++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];

			LucIdx = (uint8)TLS_SERVER +
			(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
			
			/* Set the next element of handhsake to be processed by Server */
			Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
			Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
				
            Tls_GaaServerState[LusActiveConnIdx] = TLS_SERVER_CONNECTED; 
			
            #if (STD_ON == TLS_TIMER_SUPPORT)
                Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
                Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
                                        ddTlsConnectionId].usTlsWaitingTimeout;
            #endif 

			Tls_HandshakeFrame[LusActiveConnIdx].ulWriteSeqNum = (uint64)TLS_ZERO;    
			Tls_HandshakeFrame[LusActiveConnIdx].ulReadSeqNum = (uint64)TLS_ZERO;
			
			Tls_HandshakeFrame[LusActiveConnIdx].blIsAppKeysDerived = TLS_TRUE;
			Tls_HandshakeFrame[LusActiveConnIdx].blIsHSKeysDerived = TLS_FALSE;   
			
			Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_READY;
			
			/* clear */
			Tls_MemSet(&Tls_GaaHShakeBuffer[LusActiveConnIdx][TLS_ZERO], 
									(uint8)TLS_ZERO, (uint16)TLS_MAX_BUFFER_SIZE);
			
			Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition = 
																(uint16)TLS_ZERO;   
			
			Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_READY);            
        }
        else
        {
            Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
            Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);  
        }    
    }
  else
  {
    /* Not expected - Internal Error */
  }
  
}/* end of Tls_HandleClientFinReception*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_PerformApplicationKeyscalc                      **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function derives Application keys.             **
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
**  Design ID           : TLS_SDD_0259                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_PerformApplicationKeyscalc(const uint16 LusActiveConIdxx)
{

    Std_ReturnType LddReturnValue;
    TcpIp_TlsConnectionIdType LddConnectionId;
	
	uint8* LpHSShakeBuf;
	uint16 LusNegotitdCiprSutId;
    uint8 LucChoosenHashLen;
	uint16 LusTemp;
	uint16 LusTotlInLen;
    uint8 LaaInnerPlainText[TLS_FIFTYTHREE]; // 48(max hash) + 4(HShdr) + 1
	uint8 LucIdx;
	boolean LblClientAuthReq;
	
    LddConnectionId = 
    Tls_GddActiveConnManager[LusActiveConIdxx].ddTlsConnectionId;                     

    LddReturnValue = Tls_DeriveMasterSecretMaterial(LusActiveConIdxx);
        
    if (E_OK == LddReturnValue)
    {
		/* once applicato keys dericatiov is succesfull then only transmit */			
		/* get the negotaited cipher suit from server side */
		LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
		Tls_HandshakeFrame[LusActiveConIdxx].usNegotiatedCiprSutIdx].
		ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId;
		
		LucChoosenHashLen = 
		(LusNegotitdCiprSutId == (uint16)TLS_AES_256_GCM_SHA384) ? 
		(uint8)TLS_FOURTYEIGHT : (uint8)TLS_THIRTYTWO;
								
		LusTotlInLen = 
					Tls_HandshakeFrame[LusActiveConIdxx].usHShakeBufferPosition;
		
		/* SERVER : now this will point till SF first byte */  
		/* CLIENT : now this will point till CF first byte */  
		LusTotlInLen -= (uint16)((uint16)TLS_FOUR + (uint16)LucChoosenHashLen);   
		
		LpHSShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConIdxx][LusTotlInLen];
		
		LusTemp = (uint16)TLS_FOUR + (uint16)LucChoosenHashLen;
		
		Tls_MemCpy(&LaaInnerPlainText[TLS_ZERO], &LpHSShakeBuf[TLS_ZERO],
																	LusTemp);
													
		LaaInnerPlainText[LusTemp] = (uint8)TLS_HANDSHAKE; 
		
		LddReturnValue = Tls_TransmitRecord(
											LusActiveConIdxx,
											TLS_APPLICATION_DATA,
											&LaaInnerPlainText[TLS_ZERO],
											LusTemp + (uint16)TLS_ONE);  		
		if (E_OK == LddReturnValue)
		{	
			if (Tls_GaaConnection[LddConnectionId].ddTlsConnectionType == 
																		TLS_CLIENT)
			{
				/* If No Issue */
				/* Increment to point to next Element */
				++ Tls_GaaHandshakeElementIndex[LusActiveConIdxx];
			
				/* Set the next element of handhsake to be processed by client */
				LucIdx = (uint8)TLS_CLIENT +
				(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
	  
				/* Set the next element of handhsake to be processed by client */
				Tls_GaaFetchHShakeElmnts[LusActiveConIdxx] = 
				Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConIdxx]];    
				
				Tls_GaaClientState[LusActiveConIdxx] = TLS_CLIENT_CONNECTED;             
			
			
			
				Tls_HandshakeFrame[LusActiveConIdxx].ulWriteSeqNum = (uint64)TLS_ZERO;    
				Tls_HandshakeFrame[LusActiveConIdxx].ulReadSeqNum = (uint64)TLS_ZERO;
			
				Tls_HandshakeFrame[LusActiveConIdxx].blIsAppKeysDerived = TLS_TRUE;
				Tls_HandshakeFrame[LusActiveConIdxx].blIsHSKeysDerived = TLS_FALSE;   
			
				Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_READY;
			
				/* clear */
				Tls_MemSet(&Tls_GaaHShakeBuffer[LusActiveConIdxx][TLS_ZERO], 
										(uint8)TLS_ZERO, (uint16)TLS_MAX_BUFFER_SIZE);
				
				Tls_HandshakeFrame[LusActiveConIdxx].usHShakeBufferPosition = 
																	(uint16)TLS_ZERO;   
				
				Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_READY);  
			}
			else
			{
				if (TLS_AUTH_CERT == Tls_GaaConnection[LddConnectionId].ddTlsAuthType)
				{
					/* Combination 1 : if Client Auth is true , the for sure it would have 
					sent a cert req msg to Client, and now we shall wait for a Empty Cert or Full cert 
					depending on peer */ 
					LblClientAuthReq = Tls_GaaConnection[LddConnectionId].blTlsUseClintAuthReq;  
					
					if (LblClientAuthReq)
					{
						/* move to recving and validating a cert HS msg */
						++ Tls_GaaHandshakeElementIndex[LusActiveConIdxx];
						
						Tls_GaaServerState[LusActiveConIdxx] = 
												TLS_SERVER_WAIT_FOR_CERT; 
					}
					else
					{
						/* wait for Client to send FInished msg */
						Tls_GaaHandshakeElementIndex[LusActiveConIdxx] += 
																	(uint8)TLS_FOUR;
																	
						Tls_GaaServerState[LusActiveConIdxx] = 
												TLS_SERVER_WAIT_FOR_FINISHED; 
					}					
				}
				else
				{
					/* move to recive client Finished Msg */
					++ Tls_GaaHandshakeElementIndex[LusActiveConIdxx];
					
					Tls_GaaServerState[LusActiveConIdxx] = 
												TLS_SERVER_WAIT_FOR_FINISHED; 
				}

				
				LucIdx = (uint8)TLS_SERVER +
				(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
	  
				/* Set the next element of handhsake to be processed by Server */
				Tls_GaaFetchHShakeElmnts[LusActiveConIdxx] = 
				Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConIdxx]];
			}	
		}
		else
		{
			Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
			Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);     
		}
    }
    else
    {
        Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
        Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);      
    }
    
}/* end of Tls_PerformApplicationKeyscalc*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_PerformCertVerification          **
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
**  Design ID           : TLS_SDD_0387                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_PerformCertVerification(const uint16 LusActiveConnIdx)
{
	#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
    Tls_AsyncEventType LddAsyncEvent;
    TcpIp_TlsConnectionIdType LddConnectionId;
    Tls_FrameType*      LpTlsFrame;
	Tls_ConnectionType LddConType;
    uint32 LulHSHdrLen;
    uint8* LpHShakeBuf;
    uint16 LusCERTMsgLen;
	uint8 LucIdx;
    
    LddConnectionId = 
    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
        
    LddAsyncEvent = Tls_VerifyCertificate(LusActiveConnIdx);                                
                                    
    if (TLS_E_OK == LddAsyncEvent)
    {
        LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];
		
		/* since we know at what byte index the Certificate msg starts 
        get the location*/
        LpHShakeBuf = 
		&Tls_GaaHShakeBuffer[LusActiveConnIdx][LpTlsFrame->usProcessedHSBytes];

        LulHSHdrLen =
		((uint32)TLS_ZERO  << (uint8)TLS_TWENTYFOUR) |
		((uint32)LpHShakeBuf[TLS_ONE]  << (uint8)TLS_SIXTEEN) |
		((uint32)LpHShakeBuf[TLS_TWO]  << (uint8)TLS_EIGHT)  |
		((uint32)LpHShakeBuf[TLS_THREE]); 
		
		LusCERTMsgLen = (uint16)((uint16)LulHSHdrLen + (uint16)TLS_FOUR);

        /* if Certificate Msg was Succfully verify from start to end
        only then updated Processed bytes */
        LpTlsFrame->usProcessedData += LusCERTMsgLen;
        LpTlsFrame->usProcessedHSBytes += LusCERTMsgLen;
		
        /* If No Issue */
        /* Increment to point to next Element */
        ++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
		
		LddConType = Tls_GaaConnection[LddConnectionId].ddTlsConnectionType;
		
		if (TLS_CLIENT == LddConType)
		{
			if ((LpTlsFrame->usUnProcessedData) > (LpTlsFrame->usProcessedData))
			{
				Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_RECVD_CV;
			}
			else
			{
				Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_WAIT_FOR_CV;
			} 			
		}
		else
		{
			if ((LpTlsFrame->usUnProcessedData) > (LpTlsFrame->usProcessedData))
			{
				Tls_GaaServerState[LusActiveConnIdx] = TLS_SERVER_RECVD_CV;
			}
			else
			{
				Tls_GaaServerState[LusActiveConnIdx] = TLS_SERVER_WAIT_FOR_CV;
			}		
		}


		LucIdx = (uint8)LddConType +
		(uint8)Tls_GaaConnection[LddConnectionId].ddTlsAuthType;
		
		/* Set the next element of handhsake to be processed by client */
		Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
		Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
				
        #if (STD_ON == TLS_TIMER_SUPPORT)
            Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
                Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
                                        ddTlsConnectionId].usTlsWaitingTimeout;
        #endif 
    }
    else if (TLS_E_NOT_OK == LddAsyncEvent)
    {
        Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
        Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);      
    }
    else
    {
        /* Still Verification is in progress, so once u call main func again
        it shd come in this function again */
    }
	#else
		TLS_UNUSED(LusActiveConnIdx);
	#endif
	
}/* end of Tls_PerformServerFinished*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"




/*******************************************************************************
** Function Name        : Tls_HandleServerCRHSMessage                               **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function handle reception of Encrypted         **
**                        Extension.                                          **
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
**  Design ID           : TLS_SDD_0196                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_HandleServerCRHSMessage(const uint16 LusActiveConnIdx)
{
	#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
		Std_ReturnType      LddReturnValue; 
		Tls_FrameType*      LpTlsFrame;
		uint8*              LpHShakeBuf;    
		uint16              LusCRLen;
		uint8 LucIdx;
		
		LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];
		
		LpHShakeBuf = 
			&Tls_GaaHShakeBuffer[LusActiveConnIdx][LpTlsFrame->usProcessedHSBytes];
		
		
		/* check wether its a CERT REQ or CERT HS msg , cuz both can come depending
		on pear */
		if (TLS_CLIENT_WAIT_FOR_CR == Tls_GaaClientState[LusActiveConnIdx])
		{    
			if (LpTlsFrame->usUnProcessedData > LpTlsFrame->usProcessedData)
			{
				/* here i am not checking if its a different msg, just checking
				if its CERT REQ or CERT */
				if ((uint8)TLS_CERTIFICATE_REQUEST == (*LpHShakeBuf))
				{
					Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_RECVD_CR;
				}
				else
				{
					Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_RECVD_CERT;
				
					/* here if it was cert msg, then when we call next main func
					it shd jump to handle Server Cert func */
				
					/* Increment to point to next Element */
					++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
				
					LucIdx = (uint8)TLS_CLIENT +
					(uint8)Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
											ddTlsConnectionId].ddTlsAuthType;
				
					/* Set the next element of handhsake to be processed by client */
					Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
					Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
				}
			}
			else
			{
				/* Do Nothing */
				#if (STD_ON == TLS_TIMER_SUPPORT)
					Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout -= 
					(uint16)TLS_ONE;
						
					if ((uint16)TLS_ZERO == 
						Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout)
					{
						Tls_GddConnHSStatus[Tls_GddActiveConnManager
										[LusActiveConnIdx].ddTlsConnectionId] = 
															TLS_CONNECTION_FAILED;
					
						Tls_ConnStatsNotification(
						Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
						TLS_CONNECTION_FAILED);                
					}
				#endif
			}
		}
		/* problem is it come here also , if msgs like (EE  + CERT REQ + Someother msg 
		comes in together at once , with one header kind of , so extra conditions are
		also to be added here )*/
		else if (TLS_CLIENT_RECVD_CR == Tls_GaaClientState[LusActiveConnIdx])
		{
			/* here i am not checking if its a different msg, just checking
			if its CERT REQ or CERT */
			if ((uint8)TLS_CERTIFICATE_REQUEST == (*LpHShakeBuf))
			{
				LddReturnValue = Tls_ValidateCertReqMSg(LusActiveConnIdx, &LusCRLen);
		
				if (E_OK == LddReturnValue)
				{
					LpTlsFrame->usProcessedData += LusCRLen;
					/* now it point to end of Cert req */
					LpTlsFrame->usProcessedHSBytes += LusCRLen;
                    /* set it here, to use it later during Client Certificate transit */
                    LpTlsFrame->blIsCertReqReceived = TLS_TRUE;
					
					/* this will point to the next byte , After the Cert req HS frame ends*/  
					if ((LpTlsFrame->usUnProcessedData) > (LpTlsFrame->usProcessedData))
					{
						Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_RECVD_CERT;
					}
					else
					{
						Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_WAIT_FOR_CERT;
					}         
						
					/* If No Issue */
					/* Increment to point to next Element */
					++ Tls_GaaHandshakeElementIndex[LusActiveConnIdx];
					
					LucIdx = (uint8)TLS_CLIENT +
					(uint8)Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
													ddTlsConnectionId].ddTlsAuthType;
					
					/* Set the next element of handhsake to be processed by client */
					Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
					Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]]; 
						
					#if (STD_ON == TLS_TIMER_SUPPORT)
						Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
						Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
												ddTlsConnectionId].usTlsWaitingTimeout;
					#endif
				}										
				else
				{
					Tls_GddConnHSStatus[Tls_GddActiveConnManager[LusActiveConnIdx].
									ddTlsConnectionId] = TLS_CONNECTION_FAILED;
					
					Tls_ConnStatsNotification(
					Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
					TLS_CONNECTION_FAILED);
				} 
			}
			else
			{
				/* since its CERT HS msg, so update the state */
				Tls_GaaClientState[LusActiveConnIdx] = TLS_CLIENT_RECVD_CERT;
				/* Call Server Cert Handler , instead of wasting a main function*/
				LddReturnValue = Tls_ValidateCertMsg(LusActiveConnIdx, LpHShakeBuf);
		
				if (E_OK == LddReturnValue)
				{		      
					/* If No Issue */
					/* Increment to point to next Element , since we have to jump , Certificate Verify */
					Tls_GaaHandshakeElementIndex[LusActiveConnIdx] += TLS_TWO;
					
					LucIdx = (uint8)TLS_CLIENT +
					(uint8)Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
													ddTlsConnectionId].ddTlsAuthType;
					
					/* Set the next element of handhsake to be processed by client */
					Tls_GaaFetchHShakeElmnts[LusActiveConnIdx] = 
					Tls_GSaaHShakeElmntList[LucIdx][Tls_GaaHandshakeElementIndex[LusActiveConnIdx]];  
					
					
					#if (STD_ON == TLS_TIMER_SUPPORT)
						Tls_GddActiveConnManager[LusActiveConnIdx].usWaitingTimeout = 
						Tls_GaaConnection[Tls_GddActiveConnManager[LusActiveConnIdx].
												ddTlsConnectionId].usTlsWaitingTimeout;
					#endif
				}										
				else
				{
					Tls_GddConnHSStatus[Tls_GddActiveConnManager[LusActiveConnIdx].
									ddTlsConnectionId] = TLS_CONNECTION_FAILED;
					
					Tls_ConnStatsNotification(
					Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId,
					TLS_CONNECTION_FAILED);
				}            
			}                         
		}
		else
		{
			/* Not expected - Internal Error */
		}
	#else
		TLS_UNUSED(LusActiveConnIdx);
	#endif
}/* end of Tls_HandleServerCRHSMessage*/
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/* parasoft-end-suppress CERT_C-API00-a-3 */
/* parasoft-end-suppress CERT_C-DCL19-a-3 */
/* parasoft-end-suppress MISRAC2012-RULE_20_1-a-4 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-d-2 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 */