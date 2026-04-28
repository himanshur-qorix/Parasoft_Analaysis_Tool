/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_Certificate.h                                                 **
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
** Design ID  : TLS_SDD_0748, TLS_SDD_0765, TLS_SDD_0766                      **
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
#ifndef TLS_CERTIFICATE_H
#define TLS_CERTIFICATE_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType Tls_ProcessCertMessage(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_ValidateCertMsg(const uint16 LusActiveConnIdx,
												const uint8* const LpHShakeBuf);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Tls_AsyncEventType Tls_VerifyCertificate(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_ValidateCVMSg(const uint16 LusActiveConnIdx, uint16* const LpCVHSLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_PrepCVHSMessage(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_ProcessCertRequestMsg(const uint16 LusActiveConnIdx);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_ValidateCertReqMSg(const uint16 LusActiveConnIdx,
												uint16* const LpCertReqHSLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#endif                              
#endif