/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_Record.c                                                   **
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
**  Design ID  : TLS_SDD_0312, TLS_SDD_0349                                   **
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
* #section Tls_Record_c_REF_1
* Violates MISRAC2012-RULE_5_4-c-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Record_c_REF_2
* Violates MISRAC2012-RULE_5_4-d-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Record_c_REF_3
* Violates MISRAC2012-RULE_20_1-a-4: Do not precede the #include directive with
* executable code
* Justification: Macros are defined by AUTOSAR standards and internal
* specifications.
*
* #section Tls_Record_c_REF_4
* Violates CERT_C-DCL19-a-3: Variable "" should be declared as locally as
* possible
* Justification: Coding guidelines suggest the variables are declared on the
* start of the function.
*
* #section Tls_Record_c_REF_5
* Violates CERT_C-API00-a-3: Parameter '' is not validated before use.
* Justification: This parameter either originates from the configuration or is
* validated in the main API before being passed as an argument to the local API.
* Hence, additional validation within the local API is not required and has been
* manually verified.
*
* #section Tls_Record_c_REF_6
* Violates MISRAC2012-RULE_2_2-b-2: Value of '' is never used
* Justification: The variables '' are intentionally initialized to a defined 
* safe value (TLS_ZERO) before any functional usage
* The TASKING compiler issues a W507 ("possibly uninitialized") diagnostic due 
* to conservative data-flow analysis. Manual code review confirms that the 
* variable is deterministically initialized and cannot remain uninitialized at runtime.
*
* #section Tls_Record_c_REF_7
* Violates CERT_C-DCL22-a-3: Value of '' is never used
* Justification: The variables '' are intentionally initialized to a defined 
* safe value (TLS_ZERO) before any functional usage
* The TASKING compiler issues a W507 ("possibly uninitialized") diagnostic due 
* to conservative data-flow analysis. Manual code review confirms that the 
* variable is deterministically initialized and cannot remain uninitialized at runtime.
*/


/* parasoft-begin-suppress MISRAC2012-RULE_5_4-c-2 
"Reason: Tls_Record_c_REF_1" */
/* parasoft-begin-suppress MISRAC2012-RULE_5_4-d-2 
"Reason: Tls_Record_c_REF_2" */
/* parasoft-begin-suppress MISRAC2012-RULE_20_1-a-4 
"Reason: Tls_Record_c_REF_3" */
/* parasoft-begin-suppress CERT_C-DCL19-a-3 
"Reason: Tls_Record_c_REF_4" */
/* parasoft-begin-suppress CERT_C-API00-a-3 
"Reason: Tls_Record_c_REF_5" */


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
**                      Global Data                                           **
*******************************************************************************/


/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_EncryptRecord(const uint16 LusActiveConnIdx,
                                const uint8* const LpPlainTxtPtr, 
                                const uint16 LusPlainTxtLen,
                                const uint8* const LpAssociatedDataPtr,
                                uint8* LpEncryptData,
                                uint16* const LpEncryptDataLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_HandlePlainHS(
                                const uint16 LusActiveConnIdx,
                                const uint8* const LpDataPtr,
								const uint16  LusTotalDataLength);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_HandleEncryptedData(
                                const uint16 LusActiveConnIdx,
                                const uint8* const LpDataPtr,
								const uint16  LusTotalDataLength,
								boolean* const LpQueFlag);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"




/*******************************************************************************
** Function Name        : Tls_TransmitRecord                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function handles the Record Protocol.          **
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
**  Design ID           : TLS_SDD_0231                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_TransmitRecord(
                            const uint16 LusActiveConnIdx,
                            const Tls_ContentType LddContentType,
                            const uint8* const LpPlainTxtPtr,
                            const uint16 LusPlainTxtLen)
{ 
	
    Std_ReturnType LddReturnValue;
    TcpIp_TlsConnectionIdType LddConnectionId;
    Tls_FrameType*  LpTlsFrame;
    uint8* LpEncrPtr;
    uint16 LusVersion;
    uint16 LusNegotitdCiprSutId;
    uint16 LusTemp;
    uint16 LusRecordLen;
    uint16 LusEncryptDataLen;
    uint8  LaaAssociatedData[TLS_FIVE];
    uint8 LaaFinalMsg[TLS_MAX_BUFFER_SIZE];
    uint8 LucChoosenTagLen;
    boolean LblHSKeysDerived;
    boolean LblAppKeysDerived;
    
	LusRecordLen = (uint16)TLS_ZERO;

    LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];

    LblHSKeysDerived = LpTlsFrame->blIsHSKeysDerived;
    LblAppKeysDerived = LpTlsFrame->blIsAppKeysDerived;
    LddConnectionId = 
                Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
                
    /* in what buffer , is the encrypted buffer stored, since 
    Tls_GaaAppBuffer is strictly for App data*/
    if (LpTlsFrame->blIsTxInProgress == TLS_TRUE)
    {
        LpEncrPtr = &Tls_GaaAppBuffer[LusActiveConnIdx][TLS_ZERO];
    }
    else
    {
        LpEncrPtr = &LaaFinalMsg[TLS_ZERO];
    }
    
    if (LblHSKeysDerived || LblAppKeysDerived)
    {
        /* get the negotaited cipher suit from server side */
        LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
        Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
        ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId;    

        LucChoosenTagLen = 
                (LusNegotitdCiprSutId == (uint16)TLS_AES_128_CCM_8_SHA256) ? 
                        (uint8)TLS_EIGHT : (uint8)TLS_SIXTEEN;         
                        
        /* since keys are available , so basically alert has to be ecrypted 
        either by HS keys or App keys */
        /* prepare the AassociatedData */
        LaaAssociatedData[TLS_ZERO] = (uint8)TLS_APPLICATION_DATA;
        LaaAssociatedData[TLS_ONE] = (uint8)TLS_THREE;
        LaaAssociatedData[TLS_TWO] = (uint8)TLS_THREE;
        //LaaAssociatedData[TLS_THREE] = (uint8)TLS_ZERO;
        /* depending on choosen tag len 
        Alert = 16 bytes tag(max) + 2 bytes Alert + 1 bytes Content type
        HS msg = 16 bytes tag(max) + CF or SF msg + 1 bytes Content type */
        LusTemp = (uint16)LucChoosenTagLen + LusPlainTxtLen;

        LaaAssociatedData[TLS_THREE] = 
            (uint8)((uint16)(LusTemp >> (uint16)TLS_EIGHT) & 
                                                    (uint16)TLS_U16_LSB_MASK);

        LaaAssociatedData[TLS_FOUR] = 
                                (uint8)(LusTemp & (uint16)TLS_U16_LSB_MASK);
        

        
        LddReturnValue = Tls_EncryptRecord(LusActiveConnIdx,
                                            &LpPlainTxtPtr[TLS_ZERO], 
                                            LusPlainTxtLen,
                                            &LaaAssociatedData[TLS_ZERO],
                                            &LpEncrPtr[TLS_FIVE],
                                            &LusEncryptDataLen);
                                            
        if (E_OK == LddReturnValue)
        {
            if (LusTemp == LusEncryptDataLen)
            {  
                /* add +5 bytes of record header */       
                LusRecordLen = LusEncryptDataLen + (uint16)TLS_FIVE;        
                LusTemp = Tls_Htons(LusEncryptDataLen);  

                LpTlsFrame->ulWriteSeqNum +=  (uint64)TLS_ONE;                  
            }
            else
            {
                LddReturnValue = E_NOT_OK;
            }    
        }

        /* if anything fails in Encr record */
        if (E_NOT_OK == LddReturnValue)
        {
            Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
        }
    }
    else
    {    
        LusTemp = Tls_Htons(LusPlainTxtLen);

        /* copy whole HandShake messge into local array  */
        Tls_MemCpy(&LpEncrPtr[TLS_FIVE], &LpPlainTxtPtr[TLS_ZERO], 
                                                                LusPlainTxtLen);  
 
        /* add +5 bytes of record header */       
        LusRecordLen = LusPlainTxtLen + (uint16)TLS_FIVE; 

        LddReturnValue = E_OK;        
    }

    if (E_OK == LddReturnValue)
    {
        if (TLS_APPLICATION_DATA == LddContentType)
        {
            LusVersion = Tls_Htons((uint16)TLS_VERSION_V12);
        }
        
        if ((TLS_HANDSHAKE == LddContentType) || (TLS_ALERT == LddContentType))
        {
            LusVersion = Tls_Htons((uint16)TLS_VERSION_V12);
        }
        
        /* Record Content Type */
        LpEncrPtr[TLS_ZERO] = (uint8)LddContentType;
            
        /* Record Legacy Version */
        Tls_MemCpy(&LpEncrPtr[TLS_ONE], &LusVersion, (uint16)TLS_TWO); 
        
        /* Record Length  */
        Tls_MemCpy(&LpEncrPtr[TLS_THREE], &LusTemp, (uint16)TLS_TWO);  

        LddReturnValue = Tls_TcpTransmit(
                                    LddConnectionId,
                                    /* Buffer with the whole Message prepared */
                                    &LpEncrPtr[TLS_ZERO],
                                    /* Message Length */
                                    LusRecordLen);                          
    }

    return LddReturnValue;
}/* end of Tls_TransmitRecord */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_HandlePlainHS                                   **
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
**  Design ID           : TLS_SDD_0290                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_HandlePlainHS(
                                const uint16 LusActiveConnIdx,
                                const uint8* const LpDataPtr,
								const uint16  LusTotalDataLength)
{
    Tls_ConnectionType  LddTlsConnectionType;
	TcpIp_TlsConnectionIdType LddConnectionId;
    Std_ReturnType  LddReturnValue;
    uint16 LusRecordSize;


    LddReturnValue = E_OK;
	LddConnectionId = 
                Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;				
    LddTlsConnectionType = Tls_GaaConnection[LddConnectionId].ddTlsConnectionType;	
	
	
    if (TLS_CONNECTION_IN_PROGRESS == Tls_GddConnHSStatus[LddConnectionId])
    {
        /* check the remaining bytes of the record header here only
        0x03,0x03 or 0x03,0x01 */                
        if (LpDataPtr[TLS_ONE] == (uint8)TLS_THREE)                
        {   
            /* if its a client hello , it can be either 0x03,0x03 or 0x03,0x01 */
            if (LpDataPtr[TLS_FIVE] == (uint8)TLS_ONE) 
            {
                if ((LpDataPtr[TLS_TWO] == (uint8)TLS_ONE) || 
                                    (LpDataPtr[TLS_TWO] == (uint8)TLS_THREE))
                {
                    LddReturnValue = E_OK;
                }
                else
                {
                    LddReturnValue = E_NOT_OK;
                }
            }
            /* for all other msgs, it MUST  be 0x03,0x03 , as per rfc 8446 */
            else
            {
                if (LpDataPtr[TLS_TWO] == (uint8)TLS_THREE)
                {
                    LddReturnValue = E_OK;
                }
                else
                {
                    LddReturnValue = E_NOT_OK;
                }
            }
            
            if (E_OK == LddReturnValue)
            {
                LusRecordSize = 
                ((uint16)((uint16)LpDataPtr[TLS_THREE] << (uint16)TLS_EIGHT) | 
                                                (uint16)LpDataPtr[TLS_FOUR]); 
                  
                if ((LusRecordSize > (uint16)TLS_FIVE) && 
                                (LusRecordSize <= (uint16)TLS_MAX_RECORD_LEN))
                {
                    LddReturnValue = Tls_ProcessHandshakeRecord (
                                        LusActiveConnIdx,
                                        LddTlsConnectionType,
                                        &LpDataPtr[TLS_FIVE],
                                        LusTotalDataLength - (uint16)TLS_FIVE);                
                }
                else
                {
                    /*  length:  The length (in bytes) of the following
                    TLSPlaintext.fragment.  The length MUST NOT exceed 2^14 bytes.  An
                    endpoint that receives a record that exceeds this length MUST
                    terminate the connection with a "record_overflow" alert */
                    
                    Tls_GaaAlert[LusActiveConnIdx] = TLS_RECORD_OVERFLOW;
                    LddReturnValue = E_NOT_OK; 
                }               
            }
            else
            {
                /* Implementations MUST NOT send a ClientHello.legacy_version or
                ServerHello.legacy_version set to 0x0300 or less.  Any endpoint
                receiving a Hello message with ClientHello.legacy_version or
                ServerHello.legacy_version set to 0x0300 MUST abort the handshake
                with a "protocol_version" alert. */

                Tls_GaaAlert[LusActiveConnIdx] = TLS_PROTOCOL_VERSION;            
            }
        }
        else
        {
            /* COAS */
            Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
            LddReturnValue = E_NOT_OK;
        }        
    }
    
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_HandleEncryptedData                                   **
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
**  Design ID           : TLS_SDD_0290                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_HandleEncryptedData(
                                const uint16 LusActiveConnIdx,
                                const uint8* const LpDataPtr,
								const uint16  LusTotalDataLength,
								boolean* const LpQueFlag)
{
    Std_ReturnType  LddReturnValue;
    uint16 LusRecordSize;
    uint16 LusNegotitdCiprSutId;
	uint8 LucChoosenTagLen;
	
	/* TBD : when waiting for SH , if EE or SF comes */
	if (TLS_CLIENT_WAIT_FOR_SH != Tls_GaaClientState[LusActiveConnIdx])
	{
		/* check the remaining bytes of the record header here only */       
		if ((LpDataPtr[TLS_ONE] == (uint8)TLS_THREE) && 
			(LpDataPtr[TLS_TWO] == (uint8)TLS_THREE))
		{
			LusRecordSize = 
				((uint16)((uint16)LpDataPtr[TLS_THREE] << (uint16)TLS_EIGHT) | 
												(uint16)LpDataPtr[TLS_FOUR]); 
												
			/* get the choosen tag len 
			since at this  point in time , already for sure the cipher suite
			will be selected*/
			/* get the negotaited cipher suit from server side */
			LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
			Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
			ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId;    
	
			LucChoosenTagLen = 
				(LusNegotitdCiprSutId == (uint16)TLS_AES_128_CCM_8_SHA256) ? 
						(uint8)TLS_EIGHT : (uint8)TLS_SIXTEEN;  
			
			/* why + 2 bytes : assuming atleast 1 byte of data will come 
			along with one byte of Content type , which was encrypted */
			/* Above assumption is wrong : 
			Application Data records may contain a zero-length
			TLSInnerPlaintext.content if the sender desires.  This permits
			generation of plausibly sized cover traffic in contexts where the
			presence or absence of activity may be sensitive.  Implementations
			MUST NOT send Handshake and Alert records that have a zero-length
			TLSInnerPlaintext.content; if such a message is received, the
			receiving implementation MUST terminate the connection with an
			"unexpected_message" alert.
			*/	
			if (LusRecordSize <= (uint16)TLS_MAX_CIPHER_RECORD_LEN)
			{
				if (LusRecordSize >= 
						((uint16)((uint16)LucChoosenTagLen + (uint16)TLS_ONE)))
				{
					/* here we cannot say wether its alert type or Application type data
					or Handshake data
					which we will get to know , only after its decrypted */
					
					*LpQueFlag = TLS_TRUE;
					
					/* Cirecualr desigh From Here */
					LddReturnValue =  Tls_EnqueueRecord(LusActiveConnIdx,
													LpDataPtr,
													LusTotalDataLength); 
				}
				else
				{
					Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
					LddReturnValue = E_NOT_OK;   
				}
			}
			else
			{
				/* The length MUST NOT exceed 2^14 + 256 bytes.  An endpoint that 
				receives a record that exceeds this length MUST terminate the
				connection with "record_overflow" alert. */
	
				Tls_GaaAlert[LusActiveConnIdx] = TLS_RECORD_OVERFLOW;
				LddReturnValue = E_NOT_OK;  
			}
		}
		else
		{
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
** Function Name        : Tls_HandleRecordReception                                   **
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
**  Design ID           : TLS_SDD_0290                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_HandleRecordReception (
                                const TcpIp_TlsConnectionIdType LddConnectionId,
                                const uint8* const LucDataPtr,
                                const uint16  LusTotalDataLength)
{
    Std_ReturnType  LddReturnValue;
    uint16 LusActiveConnIdx;
    boolean LblQueFlag;
	
	LddReturnValue = E_OK;
    LblQueFlag = TLS_FALSE;
	
    LusActiveConnIdx = Tls_GetActIdx(LddConnectionId);


	/* reduce complexity */
	switch (LucDataPtr[TLS_ZERO])
	{
		case (uint8)TLS_HANDSHAKE:
			LddReturnValue = Tls_HandlePlainHS(LusActiveConnIdx,
												LucDataPtr,
												LusTotalDataLength);
			break;

		case (uint8)TLS_APPLICATION_DATA:
			LddReturnValue = Tls_HandleEncryptedData(LusActiveConnIdx,
												LucDataPtr,
												LusTotalDataLength,
												&LblQueFlag);
			break;	
			
		case (uint8)TLS_ALERT:
			/* Handle Alert reception */
			LddReturnValue = Tls_ProcessAlertRecord(LddConnectionId,
                                &LucDataPtr[TLS_FIVE],
                                LusTotalDataLength - (uint16)TLS_FIVE);
			break;		
		
		case (uint8)TLS_CHANGE_CIPHER_SPEC:
			/* Ignore the Change Cipher Reception */
			/* An implementation which receives any other change_cipher_spec value or
			which receives a protected change_cipher_spec record MUST abort the
			handshake with an "unexpected_message" alert. */
			
			if (LucDataPtr[TLS_FIVE] != (uint8)TLS_ONE)
			{
				Tls_GaaAlert[LusActiveConnIdx] = TLS_UNEXPECTED_MESSAGE;
				LddReturnValue = E_NOT_OK;
			}
			break;

		default:
			/* If a TLS implementation receives an unexpected record type, it MUST terminate
			the connection with an "unexpected_message" alert. */
			Tls_GaaAlert[LusActiveConnIdx] = TLS_UNEXPECTED_MESSAGE;
			LddReturnValue = E_NOT_OK;
			break;		
	}
    
    
    if ((E_OK != LddReturnValue) && 
            (TLS_CONNECTION_CLOSED != Tls_GddConnHSStatus[LddConnectionId]))
    {
        if (TLS_FALSE == LblQueFlag)
        {
            if (TLS_CONNECTION_IN_PROGRESS == Tls_GddConnHSStatus[LddConnectionId])
            {
                /* our design */
                Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_FAILED;
                Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_FAILED);                    
            }
            else
            {
                Tls_GddConnHSStatus[LddConnectionId] = TLS_CONNECTION_ABORTED;
                Tls_ConnStatsNotification(LddConnectionId, TLS_CONNECTION_ABORTED);                     
            }            
        }
    }
    
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_ParseRecord                                     **
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
**  Design ID           : TLS_SDD_0291                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ParseRecord (
					const TcpIp_TlsConnectionIdType  LddConnectionId)
{
	TLS_UNUSED(LddConnectionId);
    return E_OK;	
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_EncryptRecord                                   **
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
**  Design ID           : TLS_SDD_0292                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_EncryptRecord(const uint16 LusActiveConnIdx,
                                const uint8* const LpPlainTxtPtr, 
                                const uint16 LusPlainTxtLen,
                                const uint8* const LpAssociatedDataPtr,
                                uint8* LpEncryptData,
                                uint16* const LpEncryptDataLen)
{
    uint64 LullWriteSeqNum;
    Std_ReturnType LddReturnValue;
    TcpIp_TlsConnectionIdType LddConnectionId;
    uint32 LulWriteIvKeyId;
    uint32 LulWriteKey;
    uint32 LulAEADEncryptJobId;
    uint32 LulAEADEncryptKeyId;
    uint16 LusNegotitdCiprSutId;
    uint8  LaaNounce[TLS_TWELVE];
    boolean LblAppKeysDerived;

    
    LblAppKeysDerived = Tls_HandshakeFrame[LusActiveConnIdx].blIsAppKeysDerived;
	LddConnectionId = 
                Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
    LullWriteSeqNum = Tls_HandshakeFrame[LusActiveConnIdx].ulWriteSeqNum;
                
    /* if it has come to this point meaaning , either HandShake keys or App 
    keys will be surely generated */
    if (LblAppKeysDerived)
    {
        /* encrypt it with Application keys */
        if (Tls_GaaConnection[LddConnectionId].ddTlsConnectionType == TLS_CLIENT)
        {
            LulWriteIvKeyId = 
            Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
            LusActiveConnIdx].usNegotiatedCiprSutIdx].
            ulTlsCsmClientApplWriteIVRefIdx; 
    
            LulWriteKey = 
            Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
            LusActiveConnIdx].usNegotiatedCiprSutIdx].
            ulTlsCsmClientApplWriteKeyRefIdx;                
        }
        else
        {
            LulWriteIvKeyId = 
            Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
            LusActiveConnIdx].usNegotiatedCiprSutIdx].
            ulTlsCsmServerApplWriteIVRefIdx;  
    
            LulWriteKey = 
            Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
            LusActiveConnIdx].usNegotiatedCiprSutIdx].
            ulTlsCsmServerApplWriteKeyRefIdx;              
        }
    }
    else
    {
        if (Tls_GaaConnection[LddConnectionId].ddTlsConnectionType == 
                                                                    TLS_CLIENT)
        {
            LulWriteIvKeyId = 
            Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
            LusActiveConnIdx].usNegotiatedCiprSutIdx].
            usTlsConnHandshakeRefBaseIndx].
            ulTlsCsmClientHandshakeWriteIVKeyRefIdx;  
        
            LulWriteKey = Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[
            Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
            usTlsConnHandshakeRefBaseIndx].
            ulTlsCsmClientHandshakeWritekeyRefIdx;              
        }
        else
        {
            LulWriteIvKeyId = 
            Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
            LusActiveConnIdx].usNegotiatedCiprSutIdx].
            usTlsConnHandshakeRefBaseIndx].
            ulTlsCsmServerHandshakeWriteIVKeyRefIdx;  
        
            LulWriteKey = Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[
            Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
            usTlsConnHandshakeRefBaseIndx].
            ulTlsCsmServerHandshakeWritekeyRefIdx;            
        }        
    }

    /* get the nounce prepared */
    LddReturnValue = Tls_ConstructNonce(LullWriteSeqNum,
                                        LulWriteIvKeyId,            
                                        &LaaNounce[TLS_ZERO]);

    if (E_OK == LddReturnValue)
    {
                                                        
        LulAEADEncryptJobId = 
        Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[LusActiveConnIdx].
                usNegotiatedCiprSutIdx].ulTlsCipherCsmEncryptJobRefIdx;

        LulAEADEncryptKeyId = 
        Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[LusActiveConnIdx].
                usNegotiatedCiprSutIdx].ulTlsCipherCsmEncryptKeyRefIdx; 
                
        /* get the negotaited cipher suit from server side */
        LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
        Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
        ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId;
    
        LddReturnValue = Tls_PerformAEADEncrypt(
                            LulAEADEncryptJobId,
                            LulAEADEncryptKeyId,
                            LulWriteKey,
                            LusNegotitdCiprSutId,
                            &LpPlainTxtPtr[TLS_ZERO],
                            (uint32)LusPlainTxtLen,
                            &LpAssociatedDataPtr[TLS_ZERO],
                            (uint32)TLS_FIVE,
                            &LaaNounce[TLS_ZERO],
                            (uint32)TLS_TWELVE,
                            &LpEncryptData[TLS_ZERO],
                            LpEncryptDataLen);
    }
    
    return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_DecryptRecord                                   **
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
**  Design ID           : TLS_SDD_0293                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType Tls_DecryptRecord(const uint16 LusActiveConnIdx,
                                const uint8* const LpEncryptPtr, 
                                uint8* LpDecryptData,
                                uint16* const LpDecryptDataLen)
{
    uint64 LullReadSeqNum;
    TcpIp_TlsConnectionIdType LddConnectionId;
    Std_ReturnType  LddReturnValue;
    Tls_FrameType*  LpTlsFrame;
    uint32 LulWriteIvKeyId;
    uint32 LulAEADDDecryptJobId;
    uint32 LulAEADDecryptKeyId;
    uint32 LulWriteKey;
    uint16 LusEncryptLen;
    uint16 LusNegotitdCiprSutId;
    boolean LblHSKeysDerived;
    uint8 LaaNounce[TLS_TWELVE];
    LddConnectionId = Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
    LblHSKeysDerived = Tls_HandshakeFrame[LusActiveConnIdx].blIsHSKeysDerived;

    /* Tasking waring */
	LulWriteKey = (uint32)TLS_ZERO; // parasoft-suppress MISRAC2012-RULE_2_2-b-2 CERT_C-DCL22-a-3 "Reason: Tls_Record_c_REF_6 Tls_Record_c_REF_7.
	LulWriteIvKeyId = (uint32)TLS_ZERO; // parasoft-suppress MISRAC2012-RULE_2_2-b-2 CERT_C-DCL22-a-3 "Reason: Tls_Record_c_REF_6 Tls_Record_c_REF_7.
    
    LullReadSeqNum = Tls_HandshakeFrame[LusActiveConnIdx].ulReadSeqNum;

    LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];

    if ((TLS_CONNECTION_IN_PROGRESS == Tls_GddConnHSStatus[LddConnectionId]) && 
                                                            (LblHSKeysDerived))
    {               
        /* if something was sent ecrypted by server , its nounce would be generated
        by server write iV, so here we have to select the same write IV */
        if (Tls_GaaConnection[LddConnectionId].ddTlsConnectionType == TLS_CLIENT)
        {
            LulWriteIvKeyId = 
            Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
            LusActiveConnIdx].usNegotiatedCiprSutIdx].
            usTlsConnHandshakeRefBaseIndx].
            ulTlsCsmServerHandshakeWriteIVKeyRefIdx;  

            LulWriteKey = Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[
            Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
            usTlsConnHandshakeRefBaseIndx].
            ulTlsCsmServerHandshakeWritekeyRefIdx;      
        }
        else
        {
            LulWriteIvKeyId = 
            Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
            LusActiveConnIdx].usNegotiatedCiprSutIdx].
            usTlsConnHandshakeRefBaseIndx].
            ulTlsCsmClientHandshakeWriteIVKeyRefIdx;   

            LulWriteKey = Tls_GaaHandshake[Tls_GaaCiphrSuiteWrkr[
            Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
            usTlsConnHandshakeRefBaseIndx].
            ulTlsCsmClientHandshakeWritekeyRefIdx;             
        }
		LddReturnValue = E_OK;
    }
    else if (TLS_CONNECTION_READY == Tls_GddConnHSStatus[LddConnectionId])
    {
        /* if something was sent ecrypted by server , its nounce would be generated
        by server write iV, so here we have to select the same write IV */
        if (Tls_GaaConnection[LddConnectionId].ddTlsConnectionType == TLS_CLIENT)
        {
            LulWriteIvKeyId = 
            Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
            LusActiveConnIdx].usNegotiatedCiprSutIdx].
            ulTlsCsmServerApplWriteIVRefIdx; 
    
            LulWriteKey = 
            Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
            LusActiveConnIdx].usNegotiatedCiprSutIdx].
            ulTlsCsmServerApplWriteKeyRefIdx;                
        }
        else
        {
            LulWriteIvKeyId = 
            Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
            LusActiveConnIdx].usNegotiatedCiprSutIdx].
            ulTlsCsmClientApplWriteIVRefIdx;  
    
            LulWriteKey = 
            Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[
            LusActiveConnIdx].usNegotiatedCiprSutIdx].
            ulTlsCsmClientApplWriteKeyRefIdx;              
        }
		LddReturnValue = E_OK;
    }
    else
    {
        /* Do nothing 
        verify if it will ever come here */
		LddReturnValue = E_NOT_OK;
    }  
    
    
    if (E_OK == LddReturnValue)
    {
        /* get the nounce */
        LddReturnValue = Tls_ConstructNonce(LullReadSeqNum,
                                            LulWriteIvKeyId,        
                                            &LaaNounce[TLS_ZERO]); 
                                            
        if (E_OK == LddReturnValue)
        {
            LulAEADDDecryptJobId = 
                Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[LusActiveConnIdx].
                        usNegotiatedCiprSutIdx].ulTlsCipherCsmDecryptJobRefIdx;
                        
            LulAEADDecryptKeyId = 
                Tls_GaaCiphrSuiteWrkr[Tls_HandshakeFrame[LusActiveConnIdx].
                        usNegotiatedCiprSutIdx].ulTlsCipherCsmDecryptKeyRefIdx;

            LusEncryptLen = 
            ((uint16)((uint16)LpEncryptPtr[TLS_THREE] << (uint16)TLS_EIGHT) | 
                            (uint16)LpEncryptPtr[TLS_THREE + (uint16)TLS_ONE]); 
                            
            LusNegotitdCiprSutId = Tls_GaaCiphersuiteDef[Tls_GaaCiphrSuiteWrkr[
            Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedCiprSutIdx].
            ulTlsCiphrSutDefRefIdx].usTlsCiphersuiteId;
    
            LddReturnValue = Tls_PerformAEADDecrypt(
                                        LulAEADDDecryptJobId,
                                        LulAEADDecryptKeyId,
                                        LulWriteKey,
                                        LusNegotitdCiprSutId,
                                        /* cipher text */
                                        &LpEncryptPtr[TLS_FIVE],
                                        (uint32)LusEncryptLen,
                                        /* associated data */
                                        &LpEncryptPtr[TLS_ZERO],
                                        (uint32)TLS_FIVE,
                                        &LaaNounce[TLS_ZERO],
                                        (uint32)TLS_TWELVE,
                                        &LpDecryptData[TLS_ZERO],
                                        LpDecryptDataLen);
                                           
            if (E_OK == LddReturnValue)
            {
                /* checking for the Last Bytes "record Type */
                /* if HandSHake is in progress, that means it should be an 
                handshake msg , so record type should be 0x16 */
                if (TLS_CONNECTION_IN_PROGRESS == 
                                        Tls_GddConnHSStatus[LddConnectionId])
                {
                    /* 0x08, 0x00, 0x00, 0x01, 0x00, 0x16 */
                    if (!(((uint8)TLS_HANDSHAKE == LpDecryptData[
                                        *LpDecryptDataLen - (uint16)TLS_ONE])
					||
                    ((uint8)TLS_ALERT == LpDecryptData[*LpDecryptDataLen - 
                                                            (uint16)TLS_ONE])))
                    {
                        LddReturnValue = E_NOT_OK;
                    }
                }
                /* if HandSHake is not in progress, that means it should be an 
                application data , so record type should be 0x17 */
                else if (TLS_CONNECTION_READY == 
                                        Tls_GddConnHSStatus[LddConnectionId])
                {
                    /* 0x08, 0x00, 0x00, 0x01, 0x00, 0x17 */
                    if (!(((uint8)TLS_APPLICATION_DATA == LpDecryptData[
                                        *LpDecryptDataLen - (uint16)TLS_ONE])
					||
                    ((uint8)TLS_ALERT == LpDecryptData[*LpDecryptDataLen - 
                                                            (uint16)TLS_ONE])))
                    {
                        LddReturnValue = E_NOT_OK;
                    }
                }
                else
                {
                    LddReturnValue = E_NOT_OK; 
                }
                
                /* if everything ok , then copy pre the output data correctly*/
                if (E_OK == LddReturnValue)
                {
                    /* Why this check here : 
                    Application Data records may contain a zero-length
                    TLSInnerPlaintext.content if the sender desires.  This permits
                    generation of plausibly sized cover traffic in contexts where the
                    presence or absence of activity may be sensitive.  Implementations
                    MUST NOT send Handshake and Alert records that have a zero-length
                    TLSInnerPlaintext.content; if such a message is received, the
                    receiving implementation MUST terminate the connection with an
                    "unexpected_message" alert. */

                    if ((uint8)TLS_APPLICATION_DATA != LpDecryptData[
                                        *LpDecryptDataLen - (uint16)TLS_ONE])
                    {
                        if (*LpDecryptDataLen < (uint16)TLS_TWO)
                        {
                            Tls_GaaAlert[LusActiveConnIdx] = 
                                                        TLS_UNEXPECTED_MESSAGE;                           
                            LddReturnValue = E_NOT_OK;
                        }
                    }
                    /* return the length with removing that Record type at the 
                    end eg : 0x08, 0x00, 0x00, 0x01, 0x00, 0x17 */                                        
                    *LpDecryptDataLen -= (uint16)TLS_ONE;
                    
                    LpTlsFrame->ulReadSeqNum += (uint64)TLS_ONE;                    
                }
                else
                {
                    Tls_GaaAlert[LusActiveConnIdx] = TLS_HANDSHAKE_FAILURE;
                }
            }
            else
            {
                /* If the decryption fails, the receiver MUST terminate the connection
                with a "bad_record_mac" alert. */

                Tls_GaaAlert[LusActiveConnIdx] = TLS_BAD_RECORD_MAC;
                LddReturnValue = E_NOT_OK; 
            }
        }
        else
        {
            /* generating nounce failed */
            Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
            LddReturnValue = E_NOT_OK;
        }

        if (E_OK != LddReturnValue)
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
        }
        
    }                                              
    return LddReturnValue;	
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_FragmentRecord                                  **
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
**  Design ID           : TLS_SDD_0294                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_FragmentRecord (
					const TcpIp_TlsConnectionIdType  LddConnectionId)
{
	TLS_UNUSED(LddConnectionId);
    return E_OK;	
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/* parasoft-end-suppress CERT_C-API00-a-3 */
/* parasoft-end-suppress CERT_C-DCL19-a-3 */
/* parasoft-end-suppress MISRAC2012-RULE_20_1-a-4 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-d-2 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 */