/*******************************************************************************
**                       Qorix Technologies Limited **
**                                                                            **
** Qorix Technologies Limited owns all the rights to this work. This **
** work shall not be copied,  reproduced,  used,  modified or its information **
** disclosed without the prior written authorization of Qorix Technologies **
** Limited.                                                                   **
**                                                                            **
**  SRC-MODULE:  Mka.h                                                        **
**                                                                            **
**  TARGET    :  All                                                          **
**                                                                            **
**  PRODUCT   :  AUTOSAR Mka Library Module                                   **
**                                                                            **
**  PURPOSE   :  Provision of API Prototype Declarations                      **
**                                                                            **
**  PLATFORM DEPENDANT [yes/no]: no                                           **
**                                                                            **
**  TO BE CHANGED BY USER [yes/no]: no                                        **
**                                                                            **
**  Design ID :   MKA_SDD_0062                                                **
*******************************************************************************/

/*******************************************************************************
**                      Revision History                                      **
********************************************************************************
**  Date            By                  Description                           **
********************************************************************************
** 2-Jun-2025    Samarth G         Update for Feature Release                 **
** 24-Dec-2025   Samarth G         Update for Feature Release                 **
** 12-Dec-2025   Samarth G         Update for Feature Release                 **
** 10-Dec-2025   Samarth G         Updated Version for bugfix release         **
** 25-Nov-2025   Samarth G         Updated Version for bugfix release         **
** 21-Nov-2025   Samarth G         Update for Feature Release                 **
** 19-11-2025    Samarth G         Updated Version for bugfix release         **
**                                                                            **
**                                                                            **
**  15-Nov-25     Samarth G         1. Limitation section has been updated    **
**                                                                            **
**                                                                            **
**  12-Sep-25     Samarth G           Initial Version R24-11	                **

*******************************************************************************/
#ifndef MKA_H
#define MKA_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Eth_GeneralTypes.h"
#include "Mka_Cfg.h"
#include "Mka_Types.h"
#include "EthIf.h"




/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

typedef enum ETag_Mka_PermissiveModeType {
  /* MKA_PERMISSIVE_MODE_NEVER : The controlled port will never be set to
 enabled if the participants cannot establish
 and successfully use a MACsec Secure
 Channel*/
  MKA_PERMISSIVE_MODE_NEVER = 0,
  /*The controlled port will be set to enabled and
  MACsec will not be used in the referred port if
  the timeout value (MkaOnFailPermissive
  Mode Timeout) is reached and none MKA
  instance under the PAE instance could
  success the following conditions: - A
  participant belonging to the same CA was
  recognized and authenticated. - A secure
  channel could be established. - Both
  participants can transmit and receive
  MACsec protected traffic through the SC.*/
  MKA_PERMISSIVE_MODE_TIMEOUT
} Mka_PermissiveModeType;
/*******************************************************************************
**                       Function Prototypes                                  **
*******************************************************************************/

#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

void Mka_Init(void);

void Mka_GetVersionInfo(Std_VersionInfoType *VersionInfoPtr);

Std_ReturnType Mka_SetCknStatus(uint8 MkaPaeIdx, boolean Enable,
                                const uint8 *Ckn, uint8 CknLength);

Std_ReturnType Mka_GetCknStatus(uint8 MkaPaeIdx, const uint8 *Ckn,
                                uint8 CknLength, boolean *EnablePtr);

Std_ReturnType Mka_SetEnable(uint8 MkaPaeIdx, boolean Enable);

Std_ReturnType Mka_GetEnable(uint8 MkaPaeIdx, boolean *EnablePtr);

Std_ReturnType Mka_GetPaeStatus(uint8 MkaPaeIdx,
                                Mka_PaeStatusType *PaeStatusPtr);

Std_ReturnType Mka_SetPaePermissiveMode(uint8 MkaPaeIdx,
                                        Mka_PermissiveModeType PermissiveMode);

Std_ReturnType Mka_StartPae(uint8 MkaPaeIdx);

Std_ReturnType Mka_GetMacSecStatistics(uint8 MkaPaeIdx,
                                       Mka_Stats_SecYType *MacSecStatsPtr);

Std_ReturnType Mka_LinkStateChange(uint8 MkaPaeIdx,
                                   EthTrcv_LinkStateType TransceiverLinkState);

void Mka_MainFunction(void);
void Mka_Csm_CbkNotification_MkaCryptoIckDeriveJob ( const uint32 jobId, Std_ReturnType Result) ;
extern void Mka_MacSecAddRxSaNotification(uint8 MkaPaeIdx,
                                          Std_ReturnType Result);
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"



#endif /* MKA_H */

/*******************************************************************************
**                          End of File                                       **
*******************************************************************************/
