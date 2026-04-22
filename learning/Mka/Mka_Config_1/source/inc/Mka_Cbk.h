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
#ifndef MKA_CBK_H
#define MKA_CBK_H
/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

#include "Eth_GeneralTypes.h"
#include "Platform_Types.h"
#include "Mka.h"
#include "Mka_Internal.h"





/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
// typedef uint16 PduIdType;
// typedef uint16 PduInfoType;
// typedef uint16 EtherType;

/*******************************************************************************
**                       Function Prototypes                                  **
*******************************************************************************/

#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
extern void Mka_GetMacSecStatisticsNotification(uint8 MkaPaeIdx,
                                                Std_ReturnType Result);

extern void Mka_RxIndication(uint8 CtrlIdx, Eth_FrameType FrameType,
                             boolean IsBroadcast, const uint8 *PhysAddrPtr,
                             const uint8 *DataPtr, uint16 LenByte);

extern void Mka_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);

extern void Mka_MacSecUpdateSecYNotification(uint8 MkaPaeIdx,
                                             Std_ReturnType Result);

//extern void Mka_MacSecAddTxSaNotification(uint8 MkaPaeIdx,
                                     //     Std_ReturnType Result);

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

#endif /* MKA_H */

/*******************************************************************************
**                          End of File                                       **
*******************************************************************************/
