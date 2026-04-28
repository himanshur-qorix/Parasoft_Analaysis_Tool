/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls.h                                                          **
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
** Design ID  : TLS_SDD_0297, TLS_SDD_0336, TLS_SDD_0345                      **
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

#ifndef TLS_H
#define TLS_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "TcpIp.h"
#include "Tls_Cfg.h"
#include "Tls_Types.h"

/*******************************************************************************
**                      Macro Definitions                                     **
*******************************************************************************/
/* API Service Ids */
/* Design ID : TLS_SDD_0446 */
#define TLS_INSTANCE_ID                                     (uint8)0x00

/* Service id for Tls_OpenConnection */
/* Design ID : TLS_SDD_0447 */
#define TLS_OPENCONNECTION_SID        (uint8)0x01

/* Service id for Tls_CloseConnection */
/* Design ID : TLS_SDD_0448 */
#define TLS_CLOSECONNECTION_SID        (uint8)0x02

/* Service id for Tls_RxIndication */
/* Design ID : TLS_SDD_0449 */
#define TLS_RXINDICATION_SID        (uint8)0x03

/* Service id for Tls_MainFunction */
/* Design ID : TLS_SDD_0450 */
#define TLS_MAINFUNCTION_SID        (uint8)0x04

/* Service id for Tls_GetHandshakeStatus */
/* Design ID : TLS_SDD_0451 */
#define TLS_GETCONNECTIONSTATUS_SID       (uint8)0x05

/* Service id for Tls_GetHandshakeStatus */
/* Design ID : TLS_SDD_0452 */
#define TLS_TXINDICATION_SID           (uint8)0x06

/* Service id for Tls_GetVersionInfo */
/* Design ID : TLS_SDD_0453 */
#define TLS_GETVERSIONINFO_SID           (uint8)0x07


/* Design ID : TLS_SDD_0129 */
/* DET Error Codes */
#define TLS_E_UNINIT                                        (uint8) 0x01
/* DET Error Codes */
/* Design ID : TLS_SDD_0130 */
#define TLS_E_INVALID_PARAMETER                             (uint8) 0x02
/* DET Error Codes */
/* Design ID : TLS_SDD_0131 */
#define TLS_E_PARAM_POINTER                                 (uint8) 0x03

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern void  Tls_Init(void);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_OpenConnection(
                                TcpIp_TlsConnectionIdType TlsConnectionId,
                                const Tls_ConnectionType  TlsConnectionType);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_CloseConnection(
                                    TcpIp_TlsConnectionIdType TlsConnectionId,
                                    boolean Abort);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_RxIndication(
                                TcpIp_TlsConnectionIdType TlsConnectionId,
                                const uint8*  DataPtr,
                                const uint16  TotalDataLength);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_GetConnectionStatus(
                            TcpIp_TlsConnectionIdType TlsConnectionId, 
                            Tls_ConnectionStatusType*  ConnectionResultPtr);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern Std_ReturnType  Tls_TxIndication(
                                    TcpIp_TlsConnectionIdType TlsConnectionId,
                                    const uint8*  DataPtr,
                                    const uint16  TotalDataLength);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
extern void Tls_GetVersionInfo(Std_VersionInfoType*  versioninfo);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"
#endif