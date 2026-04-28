/*******************************************************************************
**                        KPIT Technologies Limited                           **
**                                                                            **
** KPIT Technologies Limited owns all the rights to this work. This work      **
** shall not be copied, reproduced, used, modified or its information         **
** disclosed without the prior written authorization of KPIT Technologies     **
** Limited.                                                                   **
**                                                                            **
** SRC-MODULE: EthIf_Version.c                                                **
**                                                                            **
** TARGET    : All                                                            **
**                                                                            **
** PRODUCT   : AUTOSAR EthIf                                                  **
**                                                                            **
** PURPOSE   : AUTOSAR EthIf API's                                            **
**                                                                            **
** PLATFORM DEPENDANT [yes/no]: no                                            **
**                                                                            **
** TO BE CHANGED BY USER [yes/no]: no                                         **
**                                                                            **
** Design ID : ETHIF_SDD_0017                                                 **
*******************************************************************************/

/*******************************************************************************
**                      Revision History                                      **
********************************************************************************
** Date          Changed By      Description                                  **
********************************************************************************
** 11-Jul-2025   A Naresh        As per US #198921, Initial version work-     **
**                               -product is carry over from R23-11 V1.0.0    **
*******************************************************************************/

/*******************************************************************************
**                   MISRA-C:2012 violations Section                          **
*******************************************************************************/
/* PRQA S 0380 EOF */
/* violates Number of macro definitions exceeds 4095 - program does not */
/* conform strictly to ISO:C99 *//*invalid QAC as total number of macros in */
/* code are less than 4095*/

/*******************************************************************************
**                      CERT-C Violations Section                             **
*******************************************************************************/
/*******************************************************************************
**                      CAT Violations Section                                **
********************************************************************************/
/*******************************************************************************
**                      ECG_Checker Violations Section                        **
********************************************************************************/
/**
*ECG_Checker_Justification_EthIf_Version_c_Start : WRN01
*According to the common QAC Justification, the warnings are suppressed at the
*end of the file (EOF). After reviewing all instances, no potential issues were
*identified.
*ECG_Checker_Justification_EthIf_Version_c_Stop : WRN01
**/
/*******************************************************************************
**                       Polyspace Violations Section                         **
*******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include"EthIf.h"                    /* EthIf Types header file   */
#include"EthIf_Version.h"            /* EthIf Version Header File */
#include"Eth.h"
#include"EthSM.h"
#include"TcpIp.h"
#include"Det.h"

/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/
/* Design ID  : ETHIF_SDD_0017 */
/* EthIf Version Check */
/* SWS_EthIf_00007 */
#if (ETHIF_AR_RELEASE_MAJOR_VERSION != ETHIF_VERSION_AR_RELEASE_MAJOR_VERSION)
#error "EthIf_Version.h : Mismatch in Specification Major Version"
#endif
#if (ETHIF_AR_RELEASE_MINOR_VERSION != ETHIF_VERSION_AR_RELEASE_MINOR_VERSION)
#error "EthIf_Version.h : Mismatch in Specification Minor Version"
#endif
#if (ETHIF_AR_RELEASE_REVISION_VERSION != \
ETHIF_VERSION_AR_RELEASE_REVISION_VERSION)
#error "EthIf_Version.h : Mismatch in Specification Revision Version"
#endif

#if (ETHIF_SW_MAJOR_VERSION != ETHIF_VERSION_SW_MAJOR_VERSION)
#error "EthIf_Version.h : Mismatch in Software Major Version"
#endif
#if (ETHIF_SW_MINOR_VERSION != ETHIF_VERSION_SW_MINOR_VERSION)
#error "EthIf_Version.h : Mismatch in Software Minor Version"
#endif
#if (ETHIF_SW_PATCH_VERSION != ETHIF_VERSION_SW_PATCH_VERSION)
#error "EthIf_Version.h : Mismatch in Software Patch Version"
#endif

#if (ETHIF_ETHSM_AR_RELEASE_MAJOR_VERSION != ETHSM_AR_RELEASE_MAJOR_VERSION)
#error "EthSM.h : Mismatch in Specification Major Version"
#endif
#if (ETHIF_ETHSM_AR_RELEASE_MINOR_VERSION != ETHSM_AR_RELEASE_MINOR_VERSION)
#error "EthSM.h : Mismatch in Specification Minor Version"
#endif
#if (ETHIF_ETHSM_AR_RELEASE_REVISION_VERSION != \
ETHSM_AR_RELEASE_REVISION_VERSION)
#error "EthSM.h : Mismatch in Specification Revision Version"
#endif


#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
#if (ETHIF_DET_AR_RELEASE_MAJOR_VERSION != DET_AR_RELEASE_MAJOR_VERSION)
#error "Det.h : Mismatch in Specification Major Version"
#endif
#if (ETHIF_DET_AR_RELEASE_MINOR_VERSION != DET_AR_RELEASE_MINOR_VERSION)
#error "Det.h : Mismatch in Specification Minor Version"
#endif
#if (ETHIF_DET_AR_RELEASE_REVISION_VERSION != \
DET_AR_RELEASE_REVISION_VERSION)
#error "Det.h : Mismatch in Specification Revision Version"
#endif
#endif

#if (ETHIF_TCPIP_AR_RELEASE_MAJOR_VERSION != TCPIP_AR_RELEASE_MAJOR_VERSION)
#error "TcpIp.h : Mismatch in Specification Major Version"
#endif
#if (ETHIF_TCPIP_AR_RELEASE_MINOR_VERSION != TCPIP_AR_RELEASE_MINOR_VERSION)
#error "TcpIp.h : Mismatch in Specification Minor Version"
#endif
#if (ETHIF_TCPIP_AR_RELEASE_REVISION_VERSION != \
TCPIP_AR_RELEASE_REVISION_VERSION)
#error "TcpIp.h : Mismatch in Specification Revision Version"
#endif


/*******************************************************************************
**                          Global Data                                       **
*******************************************************************************/

/*******************************************************************************
**                              In-line Functions                             **
*******************************************************************************/

/*******************************************************************************
**                      Function Definitions                                  **
*******************************************************************************/

/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/




