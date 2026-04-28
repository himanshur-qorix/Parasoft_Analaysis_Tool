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
**  PLATFORM DEPENDANT [yes/no]: no                                           **
**                                                                            **
**  TO BE CHANGED BY USER [yes/no]: no                                        **
**                                                                            **
**  Design ID  : TLS_SDD_0314                                                 **
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
**                      Include Section                                       **
*******************************************************************************/
#include "Tls_Version.h"              /* Inclusion of Version header file */
#include "Tls_Cfg.h"
#include "ComStack_Types.h"
#if (STD_ON == TLS_DEV_ERROR_DETECT)
#include "Det.h"                       /* Inclusion of Det header file */
#endif
/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/

/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/
#if (TLS_AR_RELEASE_MAJOR_VERSION != TLS_VERSION_AR_RELEASE_MAJOR_VERSION)
#error "Tls_Version.c : Mismatch in Specification Major Version"
#endif

#if (TLS_AR_RELEASE_MINOR_VERSION != TLS_VERSION_AR_RELEASE_MINOR_VERSION)
#error "Tls_Version.c : Mismatch in Specification Minor Version"
#endif

#if (TLS_AR_RELEASE_REVISION_VERSION != \
TLS_VERSION_AR_RELEASE_REVISION_VERSION)
#error "Tls_Version.c : Mismatch in Specification Patch Version"
#endif

#if (TLS_SW_MAJOR_VERSION != TLS_VERSION_SW_MAJOR_VERSION)
#error "Tls_Version.c : Mismatch in Major Version"
#endif

#if (TLS_SW_MINOR_VERSION != TLS_VERSION_SW_MINOR_VERSION)
#error "Tls_Version.c : Mismatch in Minor Version"
#endif


/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Function Definitions                                  **
*******************************************************************************/

/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/

