/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_Ram.h                                                      **
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
** Design ID  : TLS_SDD_0303                                                  **
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
#ifndef TLS_RAM_H
#define TLS_RAM_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Tls_InternalTypes.h"
/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* This Buffer Contains frames created for client hello for each Client Connections*/
extern Tls_FrameType Tls_HandshakeFrame[TLS_MAX_ACTIVE_CONNECTIONS];
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"

/* Holds the states of TLS connection while preapring CH message */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
extern Tls_CHStateType Tls_GddCHStates[TLS_TOTAL_CONNECTIONS];
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"

/* Holds the states Handshake of All TLS Connection */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
extern Tls_ConnectionStatusType Tls_GddConnHSStatus[TLS_TOTAL_CONNECTIONS];
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"

/* Holds the information of a connection , wether it is ACTIVE or not */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
extern Tls_ActiveConnType Tls_GddActiveConnManager[TLS_MAX_ACTIVE_CONNECTIONS];
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"


#if (STD_ON == TLS_GROUP_CONFIGURED)
/* array to hold the if a connection a active in a group */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
extern Tls_GrpConnStatusType Tls_GddGroups[TLS_TOTAL_GROUPS];
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"

#endif
/* Design ID : TLS_SDD_0134 */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Variable to store Element of Structure to be prepared during client hello extension*/
extern Tls_ElementsOfClientHelloExtensions 
Tls_GaaFetchClientHelloExtensions[TLS_TOTAL_CONNECTIONS];
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Design ID : TLS_SDD_0135 */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Variable to store Element of Structure to be prepared during client hello extension*/
extern Tls_ElmntsOfHShakeProto Tls_GaaFetchHShakeElmnts[TLS_MAX_ACTIVE_CONNECTIONS];
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Design ID : TLS_SDD_0136 */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Variable to store Index for Element of Structure to be used in Handshake*/
extern uint8 Tls_GaaHandshakeElementIndex[TLS_MAX_ACTIVE_CONNECTIONS];
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Design ID : TLS_SDD_0132 */
#define TLS_START_SEC_VAR_CLEARED_BOOLEAN
#include "Tls_MemMap.h"
/* Variable to hold Tls Initialization Status */
extern boolean Tls_GblInitStatus;
#define TLS_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Tls_MemMap.h"
/* Design ID : TLS_SDD_0137 */
#define TLS_START_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"
/* This Buffer is used to store all the HandShake Messgs 
and once connected the same is used for Tx indication  */
extern uint8 Tls_GaaHShakeBuffer[TLS_MAX_ACTIVE_CONNECTIONS][TLS_MAX_BUFFER_SIZE];
#define TLS_STOP_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"

#define TLS_START_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"
/* This Buffer is used to store the final CH or Handshake messge */
extern uint8 Tls_GaaBuffer[TLS_MAX_ACTIVE_CONNECTIONS][TLS_MAX_BUFFER_SIZE];
#define TLS_STOP_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"

#define TLS_START_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"
/* This Buffer is used to store the application encrypted buffer */
extern uint8 Tls_GaaAppBuffer[TLS_MAX_ACTIVE_CONNECTIONS][TLS_MAX_FRAGMENT_SIZE];
#define TLS_STOP_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"

#define TLS_START_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"
/* This Buffer is used to store the application encrypted buffer */
extern uint8 Tls_GaaRecAppBuffer[TLS_MAX_ACTIVE_CONNECTIONS][TLS_MAX_FRAGMENT_SIZE];
#define TLS_STOP_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"
/* Design ID : TLS_SDD_0133 */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Variable to store the state of Clients */
extern Tls_ClientStateType Tls_GaaClientState[TLS_MAX_ACTIVE_CONNECTIONS];
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Design ID : TLS_SDD_0138 */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Variable to store the state of Clients */
extern Tls_ServerStateType Tls_GaaServerState[TLS_MAX_ACTIVE_CONNECTIONS];
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"

/* Design ID : TLS_SDD_0XXX */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Variable to Log Alert */
extern Tls_AlertDescription Tls_GaaAlert[TLS_MAX_ACTIVE_CONNECTIONS];
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"

#endif /* End of ifndef TLS_RAM_H */
/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
