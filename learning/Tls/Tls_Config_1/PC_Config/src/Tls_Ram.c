/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_Ram.c                                                      **
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
**  Design ID  : TLS_SDD_0311                                                 **
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
* #section Tls_Ram_c_REF_1
* Violates MISRAC2012-RULE_2_8-a-4: The '' variable is not used in the testing scope.
* Justification: This warning is reported for globalvariables defined in cfg files and used
* in static files within same project scope. The tool reported wrongly
* report this as unused, likely due to cross file usage detection during 
* testing scope analysis.

* #section Tls_Ram_c_REF_2
* Violates MISRAC2012-RULE_5_4-c-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Ram_c_REF_3
* Violates MISRAC2012-RULE_5_4-d-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Ram_c_REF_4
* Violates MISRAC2012-RULE_20_1-a-4: Do not precede the #include directive with
* executable code
* Justification: Macros are defined by AUTOSAR standards and internal
* specifications.
*
* #section Tls_Ram_c_REF_5
* Violates MISRAC2012-RULE_1_1-c-2: More than 31 initial characters in a macro
* name: 41
* Justification: As macros are defined by AUTOSAR standards and internal
* specification the length of name shall not be restricted.
*
* #section Tls_Ram_c_REF_6
* Violates MISRAC2012-RULE_1_1-a-2: More than 31 initial characters in an
* internal identifier: 34
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted.
*
* #section Tls_Ram_c_REF_7
* Violates MISRAC2012-RULE_1_1-b-2: More than 31 initial characters in an
* external identifier: 35
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted.
**/


/* parasoft-begin-suppress MISRAC2012-RULE_5_4-c-2 
"Reason: Tls_Ram_c_REF_2" */
/* parasoft-begin-suppress MISRAC2012-RULE_5_4-d-2 
"Reason: Tls_Ram_c_REF_3" */
/* parasoft-begin-suppress MISRAC2012-RULE_20_1-a-4 
"Reason: Tls_Ram_c_REF_4" */
/* parasoft-begin-suppress MISRAC2012-RULE_1_1-c-2 
"Reason: Tls_Ram_c_REF_5" */
/* parasoft-begin-suppress MISRAC2012-RULE_1_1-a-2 
"Reason: Tls_Ram_c_REF_6" */
/* parasoft-begin-suppress MISRAC2012-RULE_1_1-b-2 
"Reason: Tls_Ram_c_REF_7" */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Tls_Ram.h"
/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/
/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* This Buffer Contains frames created for client hello for each Client 
Connections*/
Tls_FrameType Tls_HandshakeFrame[TLS_MAX_ACTIVE_CONNECTIONS]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"

/* Holds the states of TLS connection while preapring CH message */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
Tls_CHStateType Tls_GddCHStates[TLS_TOTAL_CONNECTIONS]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"


/* Holds the states of TLS Connection */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
Tls_ConnectionStatusType Tls_GddConnHSStatus[TLS_TOTAL_CONNECTIONS]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"

/* Holds the information of a connection , wether it is ACTIVE or not */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
Tls_ActiveConnType Tls_GddActiveConnManager[TLS_MAX_ACTIVE_CONNECTIONS]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"

#if (STD_ON == TLS_GROUP_CONFIGURED)
/* array to hold the if a connection a active in a group */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
Tls_GrpConnStatusType Tls_GddGroups[TLS_TOTAL_GROUPS]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
#endif
/* Design ID : TLS_SDD_0134 */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Variable to store Element of Structure to be prepared during client hello extension*/
Tls_ElementsOfClientHelloExtensions 
Tls_GaaFetchClientHelloExtensions[TLS_TOTAL_CONNECTIONS]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Design ID : TLS_SDD_0135 */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Variable to store Element of Structure to be prepared during client hello extension*/
Tls_ElmntsOfHShakeProto Tls_GaaFetchHShakeElmnts[TLS_MAX_ACTIVE_CONNECTIONS]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Design ID : TLS_SDD_0136 */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Variable to store Index for Element of Structure to be used in Handshake*/
uint8 Tls_GaaHandshakeElementIndex[TLS_MAX_ACTIVE_CONNECTIONS]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Design ID : TLS_SDD_0132 */
#define TLS_START_SEC_VAR_CLEARED_BOOLEAN
#include "Tls_MemMap.h"
/* Variable to hold Tls Initialization Status */
boolean Tls_GblInitStatus; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Tls_MemMap.h"
/* Design ID : TLS_SDD_0137 */
#define TLS_START_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"
/* This Buffer is used to store the final CH or Handshake messge */
uint8 Tls_GaaHShakeBuffer[TLS_MAX_ACTIVE_CONNECTIONS][TLS_MAX_BUFFER_SIZE]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"

#define TLS_START_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"
/* This Buffer is used to store the application encrypted buffer */
uint8 Tls_GaaAppBuffer[TLS_MAX_ACTIVE_CONNECTIONS][TLS_MAX_FRAGMENT_SIZE]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"

#define TLS_START_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"
/* This Buffer is used to store the final CH or Handshake messge */
uint8 Tls_GaaBuffer[TLS_MAX_ACTIVE_CONNECTIONS][TLS_MAX_BUFFER_SIZE]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"

#define TLS_START_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"
/* This Buffer is used to store the application encrypted buffer */
uint8 Tls_GaaRecAppBuffer[TLS_MAX_ACTIVE_CONNECTIONS][TLS_MAX_FRAGMENT_SIZE]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_NO_INIT_8
#include "Tls_MemMap.h"

/* Design ID : TLS_SDD_0133 */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Variable to store the state of Clients */
Tls_ClientStateType Tls_GaaClientState[TLS_MAX_ACTIVE_CONNECTIONS]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Design ID : TLS_SDD_0138 */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Variable to store the state of Clients */
Tls_ServerStateType Tls_GaaServerState[TLS_MAX_ACTIVE_CONNECTIONS]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"

/* Design ID : TLS_SDD_0XXX */
#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"
/* Variable to Log Alert */
Tls_AlertDescription Tls_GaaAlert[TLS_MAX_ACTIVE_CONNECTIONS]; // parasoft-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Tls_Ram_c_REF_1"
#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Tls_MemMap.h"

/* parasoft-end-suppress MISRAC2012-RULE_1_1-b-2 */
/* parasoft-end-suppress MISRAC2012-RULE_1_1-a-2 */
/* parasoft-end-suppress MISRAC2012-RULE_1_1-c-2 */
/* parasoft-end-suppress MISRAC2012-RULE_20_1-a-4 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-d-2 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 */

/*******************************************************************************
**                      Function Definitions                                  **
*******************************************************************************/

/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
