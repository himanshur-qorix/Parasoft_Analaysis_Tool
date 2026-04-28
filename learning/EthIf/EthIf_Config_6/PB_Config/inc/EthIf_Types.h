/*******************************************************************************
**                        KPIT Technologies Limited                           **
**                                                                            **
** KPIT Technologies Limited owns all the rights to this work. This work      **
** shall not be copied, reproduced, used, modified or its information         **
** disclosed without the prior written authorization of KPIT Technologies     **
** Limited.                                                                   **
**                                                                            **
** SRC-MODULE: EthIf_Types.h                                                  **
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
** Design ID : ETHIF_SDD_0030                                                 **
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
**                      MISRA-C:2012 violations Section                       **
*******************************************************************************/

/*******************************************************************************
**                      CERT-C Violations Section                             **
*******************************************************************************/
#ifndef ETHIF_TYPES_H
#define ETHIF_TYPES_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Eth_GeneralTypes.h"
/*******************************************************************************
**                            Version Information                             **
*******************************************************************************/

/*******************************************************************************
**                             Macro Definitions                              **
*******************************************************************************/
/* ETHIF_ZERO                 */
/* Design ID: ETHIF_SDD_0375 */
#define ETHIF_ZERO                     (0x00u)
/* Design ID: ETHIF_SDD_0046 */
#define ETHIF_MODULE_INSTANCE_ID       (0x01u)
/*-------------------------------Error ID's ----------------------------------*/
/* Design ID: ETHIF_SDD_0380,ETHIF_SDD_0557 */
/* ETHIF_E_INV_CTRL_IDX       */
#define ETHIF_E_INV_CTRL_IDX           (0x01u)
/* Design ID: ETHIF_SDD_0382,ETHIF_SDD_0559 */
/* ETHIF_E_INV_TRCV_IDX       */
#define ETHIF_E_INV_TRCV_IDX           (0x02u)

#define ETHIF_E_INV_CLKUNIT_IDX        (0x03u)


/* Design ID: ETHIF_SDD_0383,ETHIF_SDD_0561 */
/* ETHIF_E_UNINIT    */
#define ETHIF_E_UNINIT        (0x03u)
/* Design ID: ETHIF_SDD_0384,ETHIF_SDD_0560 */
/* ETHIF_E_PARAM_POINTER      */
#define ETHIF_E_PARAM_POINTER          (0x04u)
/* Design ID: ETHIF_SDD_0381,ETHIF_SDD_0558 */
/* ETHIF_E_INV_PARAM          */
#define ETHIF_E_INV_PARAM              (0x05u)
/* Design ID: ETHIF_SDD_0379,ETHIF_SDD_0556 */
/* ETHIF_E_INIT_FAILED        */
#define ETHIF_E_INIT_FAILED            (0x06u)


/* ETHIF_E_PDU_STATE_TRANSITION_FAILED */
#define ETHIF_E_PDU_STATE_TRANSITION_FAILED    (0x01u)

/*******************************************************************************
*                               Type Definitions                               *
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                              In-line Functions                             **
*******************************************************************************/

/*******************************************************************************
**                              Function Prototypes                           **
*******************************************************************************/

#endif /* ETHIF_TYPES_H */

/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
