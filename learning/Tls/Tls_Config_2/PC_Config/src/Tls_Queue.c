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
**  Design ID  : TLS_SDD_0760                                                 **
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
* #section Tls_Queue_c_REF_1
* Violates MISRAC2012-RULE_5_4-c-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Queue_c_REF_2
* Violates MISRAC2012-RULE_5_4-d-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Queue_c_REF_3
* Violates MISRAC2012-RULE_20_1-a-4: Do not precede the #include directive with
* executable code
* Justification: Macros are defined by AUTOSAR standards and internal
* specifications.
*
* #section Tls_Queue_c_REF_4
* Violates CERT_C-DCL19-a-3: Variable "" should be declared as locally as
* possible
* Justification: Coding guidelines suggest the variables are declared on the
* start of the function.
*
* #section Tls_Queue_c_REF_5
* Violates CERT_C-API00-a-3: Parameter '' is not validated before use.
* Justification: This parameter either originates from the configuration or is
* validated in the main API before being passed as an argument to the local API.
* Hence, additional validation within the local API is not required and has been
* manually verified.

*/

/* parasoft-begin-suppress MISRAC2012-RULE_5_4-c-2 
"Reason: Tls_Queue_c_REF_1" */
/* parasoft-begin-suppress MISRAC2012-RULE_5_4-d-2 
"Reason: Tls_Queue_c_REF_2" */
/* parasoft-begin-suppress MISRAC2012-RULE_20_1-a-4 
"Reason: Tls_Queue_c_REF_3" */
/* parasoft-begin-suppress CERT_C-DCL19-a-3 
"Reason: Tls_Queue_c_REF_4" */
/* parasoft-begin-suppress CERT_C-API00-a-3 
"Reason: Tls_Queue_c_REF_5" */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Tls_Internal.h"
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
** Function Name        : Tls_EnqueueRecord                           **
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
**  Design ID           : TLS_SDD_0757                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_EnqueueRecord(const uint16 LusActiveConnIdx,
                                const uint8* const LpDataPtr,
                                const uint16 LusDataLength)
{
    Std_ReturnType LddReturnValue;
    Tls_QueueType* LddQue;
    uint16 LusStartWrapIdx;

    LddReturnValue = E_OK;
    LddQue = &Tls_HandshakeFrame[LusActiveConnIdx].ddQueue;

    /* If no enough space in our buffer , then reject
    this is checked by totalData member , not using tail or head  */
    if ((uint16)((uint16)TLS_MAX_BUFFER_SIZE - LddQue->usTotlQData) > 
                                                                LusDataLength)
    {
        if ((LddQue->usQHead + LusDataLength) <= (uint16)TLS_MAX_BUFFER_SIZE)
        {
            /* No Wrap case */
            Tls_MemCpy(&Tls_GaaBuffer[LusActiveConnIdx][LddQue->usQHead],
                                                    LpDataPtr, LusDataLength);

            LddQue->usQHead = (uint16)((uint16)(LddQue->usQHead + 
                                LusDataLength) % (uint16)TLS_MAX_BUFFER_SIZE);
           
        }
        else
        {
            /* Wrap acros boundries , it will come here */
            LusStartWrapIdx = (uint16)TLS_MAX_BUFFER_SIZE - LddQue->usQHead;
        
            Tls_MemCpy(&Tls_GaaBuffer[LusActiveConnIdx][LddQue->usQHead],
                                                    LpDataPtr, LusStartWrapIdx);

            /* Copy remoaning from start , assuming tail is updated*/                   
            Tls_MemCpy(&Tls_GaaBuffer[LusActiveConnIdx][TLS_ZERO],
                                    &LpDataPtr[LusStartWrapIdx], 
                                    LusDataLength - LusStartWrapIdx);
        
            LddQue->usQHead = (uint16)((uint16)(LusDataLength - LusStartWrapIdx)
                                                % (uint16)TLS_MAX_BUFFER_SIZE);
        }

        LddQue->usTotlQData += LusDataLength;
    }
    else
    {
        /* No Enough Space */
        LddReturnValue = E_NOT_OK;
    }

    return LddReturnValue;
}/* end of Tls_TransmitRecord */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_DeQueueRecord                           **
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
**  Design ID           : TLS_SDD_0755                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
void Tls_DeQueueRecord(const uint16 LusActiveConnIdx, const uint16 LusDataLength)
{
    Tls_QueueType* LddQue;

    LddQue = &Tls_HandshakeFrame[LusActiveConnIdx].ddQueue;

    LddQue->usQTail = (uint16)((uint16)(LddQue->usQTail + LusDataLength) % 
                                                (uint16)TLS_MAX_BUFFER_SIZE);
                                                
    LddQue->usTotlQData -= LusDataLength;
}/* end of Tls_DeQueueRecord */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_GetQRecordHdr                           **
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
**  Design ID           : TLS_SDD_0756                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
void Tls_GetQRecordHdr(const uint16 LusActiveConnIdx,  uint8* const LpDstPtr)
{
    Tls_QueueType* LddQue;
    uint16 LusStartWrapIdx;

    LddQue = &Tls_HandshakeFrame[LusActiveConnIdx].ddQueue;

    if ((LddQue->usQTail + (uint16)TLS_FIVE) <= (uint16)TLS_MAX_BUFFER_SIZE)
    {
        /* No Wrap */
        Tls_MemCpy(LpDstPtr, &Tls_GaaBuffer[LusActiveConnIdx][LddQue->usQTail],
                                                            (uint16)TLS_FIVE);
    }
    else
    {
        /* Wrap */
        LusStartWrapIdx = TLS_MAX_BUFFER_SIZE - LddQue->usQTail;
        Tls_MemCpy(LpDstPtr, &Tls_GaaBuffer[LusActiveConnIdx][LddQue->usQTail],
                                                            LusStartWrapIdx);
                                                            
        Tls_MemCpy(&LpDstPtr[LusStartWrapIdx],           
                    &Tls_GaaBuffer[LusActiveConnIdx][TLS_ZERO],
                        (uint16)TLS_FIVE - LusStartWrapIdx);
    }

}/* end of Tls_GetQRecordSize */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/* parasoft-end-suppress CERT_C-API00-a-3 */
/* parasoft-end-suppress CERT_C-DCL19-a-3 */
/* parasoft-end-suppress MISRAC2012-RULE_20_1-a-4 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-d-2 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 */