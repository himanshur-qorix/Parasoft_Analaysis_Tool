/*******************************************************************************
**                        KPIT Technologies Limited                           **
**                                                                            **
** KPIT Technologies Limited owns all the rights to this work. This work      **
** shall not be copied, reproduced, used, modified or its information         **
** disclosed without the prior written authorization of KPIT Technologies     **
** Limited.                                                                   **
**                                                                            **
** SRC-MODULE: EthIf_Version.h                                                **
**                                                                            **
** TARGET    : All                                                            **
**                                                                            **
** PRODUCT   : AUTOSAR EthIf                                                  **
**                                                                            **
** PURPOSE   : This file provides internal data structures of EthIf module.   **
**                                                                            **
** PLATFORM DEPENDANT [yes/no]: no                                            **
**                                                                            **
** TO BE CHANGED BY USER [yes/no]: no                                         **
**                                                                            **
** Prev. Safety Complaint AUTOSAR Version : R23-11                            **
**                                                                            **
** Prev. Safety Complaint Software Version : 1.0.0                            **
**                                                                            **
** Design ID : ETHIF_SDD_0016                                                 **
*******************************************************************************/

/*******************************************************************************
**                      Revision History                                      **
********************************************************************************
** Date          Changed By      Description                                  **
********************************************************************************
** 27-Feb-2026   Atreya          As per feature #CP-21187 and #CP-1450,       **
**               Chiplunkar,     updated Software version                     **
**               Vini Kadam                                                   **
** 03-Feb-2026   Atreya          As per #CP-24814, updated Software version   **
**               Chiplunkar                                                   **
** 20-Dec-2025   Atreya          As per feature #CP-10982, PDU based Reception**
**               Chiplunkar      support(LsduR), updated Software version     **
** 11-Nov-2025   A Naresh        As per feature #CP-12795, Transmission and   **
**                               Reception for MKPDU updated Software version **
** 07-Nov-2025   Devika Ramesh   As per #CP-10982, updated Software version   **
** 14-Oct-2025   A Naresh        As per  #CP-14301, updated Software version  **
** 14-Oct-2025   A Naresh        As per  #CP-12821, updated Software version  **
** 10-Oct-2025   A Naresh        As per US #CP-9168, updated Software version **
** 17-Sep-2025   A Naresh        As per US #CP-1895, updated Software version **
**                               for Hardware TimeStamp and SecuirtyEvents    **
** 04-Aug-2025   A Naresh        As per Bug #CP3894, updated                  **
**                               software patch version                       **
** 30-Jul-2025   A Naresh        As per US #CP-1018, Updated SW MINOR Version **
**                               as per Compiler Abstraction                  **
** 11-Jul-2025   A Naresh        As per US #198921, Initial version work-     **
**                               -product is carry over from R23-11 V1.0.0    **
*******************************************************************************/

/*******************************************************************************
**                      MISRA-C:2012 violations Section                       **
*******************************************************************************/

/*******************************************************************************
**                      CERT-C Violations Section                             **
*******************************************************************************/
#ifndef ETHIF_VERSION_H
#define ETHIF_VERSION_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
/* SWS_EthIf_00011 */
/* Design ID  : ETHIF_SDD_0016 */
/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/
/* AUTOSAR Specification Version Information */
/* Design ID  : ETHIF_SDD_0564, ETHIF_SDD_0565, ETHIF_SDD_0566 */

#define ETHIF_VERSION_AR_RELEASE_MAJOR_VERSION       4
#define ETHIF_VERSION_AR_RELEASE_MINOR_VERSION       10
#define ETHIF_VERSION_AR_RELEASE_REVISION_VERSION    0

/* Design ID  : ETHIF_SDD_0576, ETHIF_SDD_0577, ETHIF_SDD_0578 */
/* Software Version Information */
#define ETHIF_VERSION_SW_MAJOR_VERSION               0
#define ETHIF_VERSION_SW_MINOR_VERSION               7
#define ETHIF_VERSION_SW_PATCH_VERSION               0

/* Other (Interface) module version information */
#define ETHIF_ETHSM_AR_RELEASE_MAJOR_VERSION         4
#define ETHIF_ETHSM_AR_RELEASE_MINOR_VERSION         10
#define ETHIF_ETHSM_AR_RELEASE_REVISION_VERSION      0

#if (ETHIF_DEV_ERROR_DETECT == STD_ON)
#define ETHIF_DET_AR_RELEASE_MAJOR_VERSION           4
#define ETHIF_DET_AR_RELEASE_MINOR_VERSION           10
#define ETHIF_DET_AR_RELEASE_REVISION_VERSION        0
#endif

#define ETHIF_TCPIP_AR_RELEASE_MAJOR_VERSION         4
#define ETHIF_TCPIP_AR_RELEASE_MINOR_VERSION         10
#define ETHIF_TCPIP_AR_RELEASE_REVISION_VERSION      0

/*******************************************************************************
**                               Macro Definitions                            **
*******************************************************************************/

/*******************************************************************************
**                         Global Data                                        **
*******************************************************************************/

/*******************************************************************************
**                              In-line Functions                             **
*******************************************************************************/

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

#endif /* ETHIF_VERSION_H */
/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
