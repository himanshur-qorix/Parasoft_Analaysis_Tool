/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_Inline.h                                                   **
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
** Design ID  : TLS_SDD_0300                                                  **
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
#ifndef TLS_INLINE_H
#define TLS_INLINE_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Tls_InternalTypes.h"
#include "ComStack_Types.h"
/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/
/*******************************************************************************
**                      Macros                                                **
*******************************************************************************/


/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static inline void Tls_MemCpy(
							void* LpDestPtr,
							const void* LpSrcPtr,
							uint16 LusNumBytesToCopy);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static void Tls_MemSet(
						void * const LpDestPtr,
						const uint8 LucValue, 
						const uint16 LusNumBytesToCopy);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static inline uint16 Tls_Htons(uint16 LusInput);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static inline uint16 Tls_Ntohs(uint16 LusInput);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_Htons                                         **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : Convert uint16 var from host to network byte order  **
**                                                                            **
** Sync/Async           : NA                                                  **
**                                                                            **
** Re-entrancy          : NA                                                  **
**                                                                            **
** Input Parameters     : LusInput                                            **
**                                                                            **
** InOut parameter      : uint16 n : variable to be converted                 **
**                                                                            **
** Output Parameters    : LusRet                                              **
**                                                                            **
** Return parameter     : LusRet                                              **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)  : None                          **
**                        Function(s) invoked : None                          **
**  Design ID           : TLS_SDD_0752                                      **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static inline uint16 Tls_Htons(uint16 LusInput)
{
	#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
	uint8* LpSrcPtr;
	uint8* LpDestPtr;
	#endif
	uint16 LusRet;

	#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
	LpSrcPtr = (uint8*)(&LusInput);
	LpDestPtr = (uint8*)(&LusRet);
	LpDestPtr[TLS_ZERO] = LpSrcPtr[TLS_ONE];
	LpDestPtr[TLS_ONE] = LpSrcPtr[TLS_ZERO];
	#else
	LusRet = LusInput;
	#endif
	return(LusRet);
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_Ntohs                                         **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : Convert uint16 var from network byte to host order  **
**                                                                            **
** Sync/Async           : NA                                                  **
**                                                                            **
** Re-entrancy          : NA                                                  **
**                                                                            **
** Input Parameters     : LusInput                                            **
**                                                                            **
** InOut parameter      : uint16 x : variable to be converted                 **
**                                                                            **
** Output Parameters    : LusRet                                              **
**                                                                            **
** Return parameter     : LusRet                                              **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)  : None                          **
**                        Function(s) invoked : Tls_Htons                   **
**  Design ID           : TLS_SDD_0753                                      **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static inline uint16 Tls_Ntohs(uint16 LusInput)
{
	uint16 LusRet;
	#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
	LusRet = Tls_Htons(LusInput);
	#else
	LusRet = LusInput;
	#endif
	return(LusRet);
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_MemCpy                                        **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : Copy data from source to destination                **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant                                           **
**                                                                            **
** Input Parameters     : LpDestPtr, LpSrcPtr, LusNumBytesToCopy              **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : Tls module must be initialized                    **
**                                                                            **
** Remarks              : Global Variable(s)  : None                          **
**                        Function(s) invoked : None                          **
**  Design ID           : TLS_SDD_0754                                       **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static inline void Tls_MemCpy(
							void* LpDestPtr,
							const void* LpSrcPtr,
							uint16 LusNumBytesToCopy)
{
	while (LusNumBytesToCopy != (uint16)TLS_ZERO)
	{
		LusNumBytesToCopy--;
		((uint8*)LpDestPtr)[LusNumBytesToCopy] =
									((const uint8*)LpSrcPtr)[LusNumBytesToCopy];
	}
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_MemSet                                        **
**                                                                            **
** Service ID           : NA                                                  **
**                                                                            **
** Description          : Set the value to the memory                         **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Re-entrancy          : Reentrant                                           **
**                                                                            **
** Input Parameters     : LpDestPtr, LucValue, LusNumBytesToCopy              **
**                                                                            **
** InOut parameter      : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : TLS module must be initialized                    **
**                                                                            **
** Remarks              : Global Variable(s)  : None                          **
**                        Function(s) invoked : None                          **
**  Design ID           : TLS_SDD_0247                                     **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static inline void Tls_MemSet(void * const LpDestPtr, const uint8 LucValue,
                                                const uint16 LusNumBytesToCopy)
{
    uint16  LusDataIdx;
    
    LusDataIdx = LusNumBytesToCopy;
    while (LusDataIdx != (uint16)TLS_ZERO)
    {
        LusDataIdx--;
        ((uint8*)LpDestPtr)[LusDataIdx] = LucValue;
    }
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#endif
/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/


