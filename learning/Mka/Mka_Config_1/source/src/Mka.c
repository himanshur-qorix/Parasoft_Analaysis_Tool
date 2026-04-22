/*******************************************************************************
**                       Qorix Technologies Limited                           **
**                                                                            **
** Qorix Technologies Limited owns all the rights to this work. This          **
** work shall not be copied,  reproduced,  used,  modified or its information **
** disclosed without the prior written authorization of Qorix Technologies    **
** Limited.                                                                   **
**                                                                            **
**  SRC-MODULE:  Mka.c                                                        **
**                                                                            **
**  TARGET    :  All                                                          **
**                                                                            **
**  PRODUCT   :  AUTOSAR Crc Library Module                                   **
**  PRODUCT   :  AUTOSAR Crc Library Module                                   **
**                                                                            **
**  PURPOSE   :  Implementation of Version Information for CRC Library Module **
**                                                                            **
**  PLATFORM DEPENDANT [yes/no]: no                                           **
**                                                                            **
**  TO BE CHANGED BY USER [yes/no]: no                                        **
**
**  Design ID :   CRC_SDD_0127                                                **
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
**  12-Sep-25     Samarth G           Initial Version R24-11 **

*******************************************************************************/

/*******************************************************************************
**                      MISRA-C:2012 Violations Section                       **
*******************************************************************************/
/**
*
* #page misra_violations MISRA-QAC violations
*
* #section Crc_C_REF_1
* MISRA Rule: Violates MISRA QAC message 2211,  Current identifier is not
aligned with the previously declared identifier.
* Declaration of current identifier is not aligned at the same column
* as the previous declaration as Standard autosar variable declarations may
* cause the identifiers to have datatypes of different lengths.
*
**/

/**
 *
 * #page misra_violations MISRA-C:2012 violations
 *
 * #section Crc_c_REF_2
 * MISRA Rule: Violates MISRA 2012 Advisory Rule 20.1, message 5087 #include
 *directives should only be preceded by preprocessor directives or comments.
 * Justification: An This violation is not fixed since the inclusion of MemMap.h
 *is as per AUTOSAR requirement SWS_MemMap_00003
 **/

/**
 *
 * #page misra_violations MISRA-QAC violations
 *
 * #section Crc_c_REF_3
 * #violates #ref Crc_c_REF_3 Violates MISRA 2012  Required
 * MISRA Rule: Msg(qac-10.1.0-2109) Integral promotion _Bool promoted to signed
 * int. Justification: An operand of Boolean type is being converted to type
 * signed int under the rules of integral promotion.
 */

/**
 * #section Crc_c_REF_4
 * MISRA Rule: Violates MISRA 2012 Required DIR 1.1, Message Number 0380.
 * The included and defined macros values exceeds 4095 count.
 * This macors are part of AUTOSAR Specific defintions for inclusion of
 * memory map files for Code and Global data secions and configuration Specific
 * macros present in Cfg.h files
 */

/*******************************************************************************
**                      CERT-C Violations Section                             **
*******************************************************************************/

/**
 *
 *   Justification: QAC warning message 0857 and 0380 is for exceeding macro
 *   limits of 1024 and 4096 as per C:90 and C:99 ANSI C standards. Though,  the
 *   actual number of macros used in the module does not exceed 1024 or 4096,
 *   the module file may get this warning  in integration environment due to
 *   macros used in complete stack. So,  this suppression is added to prevent
 *   the same
 *
 *
 *   #section Crc_c_Cert_REF_2
 *   Cert Rule:  01_PRE:PRE08 Msg(certccm-2.1.0-5002)
 *   Violation: The header '%s' file name shadows a previously included header.
 *   Justification: This violation is not fixed since the inclusion of MemMap.h
 *   is as per AUTOSAR requirement SWS_MemMap_00003
 *
 *
 *   #page Helix QAC violations
 *   #section Crc_c_Cert_REF_3
 *   Cert Rule: M3CM-2,48_MSC:Rule-2.1,MSC07 Msg(rcma-3.1.0-1503)
 *   Violation:  The function 'function name' is defined but is not used within
 *this project Justification:These module APIs are standard AUTOSAR services
 *which will be called by application or other dependent BSW modules. Hence
 *these warning is suppressed as it does not impact the module funtionality.
 *
 *   #section Crc_c_Cert_REF_4
 *   Cert Rule: 02_DCL:DCL00          message 3227
 *   Violation:  The parameter 'parameter name' is never modified and so it
 *   could be declared with the 'const' qualifier.
 *   Justification:Since the signature of each standard API is provided by
 *   AUTOSAR specifications,  this violation is suppressed as it does not
 *   impact the module funtionality.
 *
 *   #section Crc_c_Cert_REF_5
 *   Cert Rule: 04_INT:INT02 Msg(qac-10.1.0-1251)
 *   Violation:  Suffixed integer constant causes implicit conversion of other
 *operand Justification: 'uint8',uint16 and 'uint32' are standard AUTOSAR types
 *   (Annexure A. 2 of of AUTOSAR_SWS_PlatformTypes.pdf) mapped to
 *   'unsigned char' and 'unsigned short' respectively.The operations perfomed
 *   on this types are to fulfill the module functionality and hence the
 *   integral promotion of these types is unavoidable. Hence these warning is
 *   suppressed as it does not impact the module funtionality.
 *
 *   #section Crc_c_Cert_REF_6
 *   Cert Rule: 48_MSC:MSC01    message 2000
 *   Violation:  No 'else' clause exists for this 'if' statement
 *   Justification: In the case of a simple if statement then the else
 *   statement need not be included since it shall increase the code complexity
 *   of function. Hence this warning is suppressed as it does not impact the
 *   module functionality
 *
 *   #section Crc_c_Cert_REF_7
 *   Cert Rule: 04_INT:INT02 Msg(qac-10.1.0-2100)
 *   Violation:  Integral promotion : unsigned char promoted to signed int.
 *   Justification: 'uint8' and 'uin16' are standard AUTOSAR types
 *   (Annexure A. 2 of of AUTOSAR_SWS_PlatformTypes.pdf) mapped to 'unsigned
 *char' and 'unsigned short' respectively.The operations perfomed on this types
 *are to fulfill the module functionality and hence the integral promotion of
 *these types is unavoidable. Hence these warning is suppressed as it does not
 *impact the module funtionality.
 *
 *
 **/

/*******************************************************************************
**                          PolySpace violations Section                      **
*******************************************************************************/
/**
**
** #page PolySpace violations
**
** #section Crc_c_Poly_REF_1 : Unreachable Code
** Polyspace_Start : Crc_c_Poly_REF_1
** The Polyspace tool doesnt pass the function arguments to cover the given
** part of code and hence it is unreachable. However the same part
** of code is covered and tested using functional testing and unit
** testing and hence doesnt have impact on functionality.
** Polyspace_End : Crc_c_Poly_REF_1
**/

/*******************************************************************************
**           Parasoft MISRA and CERT violations                               **
*******************************************************************************/
/* parasoft-begin-suppress MISRAC2012-RULE_20_1-a-4 "Reason: IEEE1722Tp_Cbk_c_Parasoft_REF_6" */
/* parasoft-begin-suppress MISRAC2012-RULE_1_1-c-2 "Reason: IEEE1722Tp_Cbk_c_Parasoft_REF_5" */
/* parasoft-begin-suppress CERT_C-DCL19-a-3 "Reason: IEEE1722Tp_Cbk_c_Parasoft_REF_4" */
/* parasoft-begin-suppress MISRAC2012-RULE_5_4-c-2 MISRAC2012-RULE_5_4-d-2 "Reason: IEEE1722Tp_Cbk_c_Parasoft_REF_3" */
/* parasoft-begin-suppress CERT_C-DCL00-b-3 "Reason: IEEE1722Tp_Cbk_c_Parasoft_REF_2" */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
/*DET_EEROR_DETECT MACRO has to be included*/
#include "Mka.h"
#include "Det.h"
#include "Mka_External_Cfg.h"
#include "Platform_Types.h"


/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
Mka_PaeInstaceStatusType Mka_GaaPaeServerStatus[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
Mka_AnouncementSet_Type
/* parasoft-begin-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Mka_Parasoft_REF_209" */
    Mka_GaaAnouncementParameterSet[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
/* parasoft-end-suppress MISRAC2012-RULE_2_8-a-4 */
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

/* parasoft-begin-suppress MISRAC2012-RULE_2_8-a-4 "Reason: Mka_Parasoft_REF_215" */
#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
/* parasoft-end-suppress MISRAC2012-RULE_2_8-a-4 */
#include "Mka_MemMap.h"
uint8 Mka_GaaICV[MKA_TOTAL_NUM_MKA_PAE_INSTANCE][MKA_ICV_LENGTH];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
Mka_PeerList_type Mka_GaaPeerPoteList[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
Mka_BasicParameterSet_Type
    Mka_GaaBasicparameterset[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
  Mka_Selected_KayParticipantType 
                               Mka_Selectedparticipant;
 #define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
**                      Function Definitions                                  **
*******************************************************************************/
/* polyspace-begin RTE:UNR [Justified:Low] "Refer Crc_c_Poly_REF_1" */

/*******************************************************************************
** Function Name        : Mka_Init                                  **
** Service ID           : 0x01                                                **
** Description          : Initializes the MKA module. **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Non Reentrant **
** Input Parameters     : None                                                **
** InOut parameter      : None                                                **
** Output Parameters    : None                                         **
** Return parameter     : None                                                **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
void Mka_Init() {

  uint8 Luscount;
   const Mka_PaeInstanceType * Lp_PaeInstanceStatic;
   const Mka_KayParticipants * Lp_KayParticipants;
 Mka_Selectedparticipant.blkaypartselect = MKA_FALSE;
 Mka_Selectedparticipant.index = MKA_ZERO;



  for (Luscount = 0; Luscount < MKA_TOTAL_NUM_MKA_PAE_INSTANCE; Luscount++) {


    
    Mka_GaaPaeServerStatus[Luscount].ddpaetatus = MKA_STATUS_WAITING_PEER_LINK;
    Mka_GaaPaeServerStatus[Luscount].ddServerInternalState = MKA_INTR_STATE_MKA_INITIALIZED;
    Mka_JobRegistyCounter[Luscount] = MKA_ZERO;
    Mka_BascsicParameterMemberNumber[Luscount] = MKA_ZERO;
    Lp_PaeInstanceStatic = &Mka_GaaPaeInstances[Luscount];

    for (Luscount = Lp_PaeInstanceStatic->ucindexofKayParticipant;
         Luscount < Lp_PaeInstanceStatic->ucnoofKayParticipant; Luscount++) {
          Lp_KayParticipants = &Mka_KayParticipant[Luscount];

      if (Lp_KayParticipants->blMkaParticipantActivate == STD_ON) {

        MKa_GaaKayStatus[Luscount] = MKA_INTR_INITIAL;

      } else {
        MKa_GaaKayStatus[Luscount] = MKA_INTR_DISCARD;
      }
    }
  /* for (Luscount = 0; Luscount < MKA_TOTAL_NUM_KAY_PARTICIPANT; Luscount++) {
   }*/
  /* Mka_GaaCknSts = NvM_ReadBlock(Mka_GaaCknSts) */
  /* Mka_GaaCknSts = NvM_ReadBlock(Enable) */
}
}

/*******************************************************************************
** Function Name        : Mka_GetVersionInfo                                  **
** Service ID           : 0x02                                               **
** Description          : Returns the version information of this module. **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant                                           **
** Input Parameters     : None                                                **
** InOut parameter      : None                                                **
** Output Parameters    : VersionInfoPtr: Version information of this module **
** Return parameter     : None                                                **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
void Mka_GetVersionInfo(Std_VersionInfoType *VersionInfoPtr) {
  if (VersionInfoPtr) {
  }
  /*Do nothing*/
}

/*******************************************************************************
** Function Name        : Mka_SetCknStatus                                  **
** Service ID           : 0x03                                               **
** Description          : Set status of a CKN from a PAE. **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module

                        : Enable: Boolean to control the Mka Participant
Activate status. True -> The MKA Participant exchanges MKPDUs. False -> The MKA
Participant does not exchange MKPDUs

                          Ckn: Connectivity Association Key Name to identify the
KaY participant

                          CknLength: Length of the CKN parameter provided **
** InOut parameter      : None                                                **
** Output Parameters    : None                                         **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and the Status of
the CKN from a PAE has been set. E_NOT_OK: The status could not be set because
call to lower layer failed.                                                 **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
Std_ReturnType Mka_SetCknStatus(uint8 MkaPaeIdx, boolean Enable,
                                const uint8 *Ckn, uint8 CknLength) {
  if (Ckn && Enable && CknLength && MkaPaeIdx) {

    /*Nothing*/
  }
  /*

  Search for Ckn in Mka_GaaCknList and provide CknIndex using internal function

  Mka_GaaMkaPae[MkaPaeIdx].Mka_GaaSelectedCkn = &Mka_GaaCknList[CknIndex];


  Mka_GaaMkaPae[MkaPaeIdx].Mka_GaaSelectedCkn->Mka_GaaCknList.Status = Enable;
  Mka_GaaMkaPae[MkaPaeIdx].Mka_GaaSelectedCkn->Mka_GaaCknList->Ckn = Ckn;
  Mka_GaaMkaPae[MkaPaeIdx].Mka_GaaSelectedCkn->Mka_GaaCknList.CknLength =
  CknLength;


  NvM_WriteBlock(Mka_GaaCknSts)
  */

  return E_OK;
}

/*******************************************************************************
** Function Name        : Mka_GetCknStatus                                  **
** Service ID           : 0x04                                               **
** Description          : Get status of a CKN from a PAE. **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Ckn: Connectivity Association Key Name to identify the KaY
participant CknLength: Length of the CKN parameter provided
                        :



                                                    **
** InOut parameter      : None                                                **
** Output Parameters    : EnablePtr: Pointer to the Mka Participant activation
status. True -> The MKA Participant exchanges MKPDUs. False -> The MKA
Participant does not exchange MKPDUs                                          **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and EnablePtr is
returned (see Parameters (out)) E_NOT_OK: The status could not be retrieved
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
Std_ReturnType Mka_GetCknStatus(uint8 MkaPaeIdx, const uint8 *Ckn,
                                uint8 CknLength, boolean *EnablePtr) {
  if (MkaPaeIdx && Ckn && EnablePtr && CknLength) {
  }
  return 0;
}

/*******************************************************************************
** Function Name        : Mka_SetEnable                                  **
** Service ID           : 0x08                                               **
** Description          : Set the MKA activation status of a PAE. **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module

                        : Enable: Boolean to control the Mka Participant
Activate status. True -> False ->

** InOut parameter      : None                                                **
** Output Parameters    : None                                         **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and MKA activation
status has been set E_NOT_OK: MKA activation status could not be set because
call to lower layer failed                                                 **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/

Std_ReturnType Mka_SetEnable(uint8 MkaPaeIdx, boolean Enable) {
  if (MkaPaeIdx && Enable) {
  }
  /* Mka_GaaMkaPae[MkaPaeIdx].MkaKay = Enable;
    NvM_WriteBlock(Enable)
  */

  return 0;
}

/*******************************************************************************
** Function Name        : Mka_GetEnable                                  **
** Service ID           : 0x05                                               **
** Description          : Get the MKA activation status of a PAE. **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module

                        :

** InOut parameter      : None                                                **
** Output Parameters    : EnablePtr: Pointer to the Mka Participant activation
status of a PAE. True -> False ->                                         **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and EnablePtr is
returned (see Parameters (out)) E_NOT_OK: MKA activation status could not be
retrieved because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
Std_ReturnType Mka_GetEnable(uint8 MkaPaeIdx, boolean *EnablePtr) {
  if (MkaPaeIdx && EnablePtr) {
    /*Nothing*/
  }
  return 0;
}

/*******************************************************************************
** Function Name        : Mka_GetPaeStatus                                  **
** Service ID           : 0x06                                               **
** Description          : Get detailed information of a PAE. **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module

                        :

** InOut parameter      : None                                                **
** Output Parameters    : PaeStatusPtr: Pointer to the status structure, which
includes detailed information of a PAE                                        **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PaeStatusPtr
is returned (see Parameters (out)) E_NOT_OK: The status could not be retrieved
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
Std_ReturnType Mka_GetPaeStatus(uint8 MkaPaeIdx,
                                Mka_PaeStatusType *PaeStatusPtr) {
  if (PaeStatusPtr && MkaPaeIdx) {
    // "Do Nonting"
  }
  return 0;
}

/*******************************************************************************
** Function Name        : Mka_SetPaePermissiveMode **
** Service ID           : 0x09                                               **
** Description          : Set Permissive Mode of a KaY. **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module

                        : PermissiveMode: Permissive mode to set in the PAE

** InOut parameter      : None                                                **
** Output Parameters    : None                               **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and Permissive
mode of KaY has been set E_NOT_OK: The Permissive mode of KaY could not be set
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
Std_ReturnType Mka_SetPaePermissiveMode(uint8 MkaPaeIdx,
                                        Mka_PermissiveModeType PermissiveMode) {
  if (MkaPaeIdx && PermissiveMode) {
    /*Do nothing*/
  }

  return 0;
}

/*******************************************************************************
** Function Name        : Mka_StartPae                                  **
** Service ID           : 0x10                                               **
** Description          : Set Permissive Mode of a KaY. **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
Std_ReturnType Mka_StartPae(uint8 MkaPaeIdx) {
  if (MkaPaeIdx) {
    // Do Nothing
  }

  return 0;
}

/*******************************************************************************
** Function Name        : Mka_GetMacSecStatistics **
** Service ID           : 0x7                                               **
** Description          : Get Statistics of a PAE. **
** Sync/Async           : Asynchronous **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : MacSecStatsPtr: Pointer to a structure including the
MACsec statistics of an MKA participant                                      **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and MacSec
Statistics has been returned. E_NOT_OK: MacSec Statistics could not be retrieved
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
Std_ReturnType Mka_GetMacSecStatistics(uint8 MkaPaeIdx,
                                       Mka_Stats_SecYType *MacSecStatsPtr) {

  if (MkaPaeIdx && MacSecStatsPtr) {
    /*Do nothing*/
  }

  return 0;
}

/*******************************************************************************
** Function Name        : Mka_LinkStateChange                                 **
** Service ID           : 0x1d **
** Description          : To inform MKA that a dedicated Trcv/Switch/PAC port **
                          link state has changed                              **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx,                  **
**                        Non reentrant for the same MkaPaeIdx                **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of   **
**                        the MKA module                                      **
**                        TransceiverLinkState:                               **
**						  The Ethernet link state of a
physical Ethernet      **
**						  connection. **
** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType                                      **
**                        E_OK: The request has been accepted and MKA module has
**                        been informed that a dedicated Trcv/Switch proteced
port
**                        link state has changed and can start with the
**						  MKA sequence.
**                        E_NOT_OK: MKA didn’t start the MKA sequence. **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
Std_ReturnType Mka_LinkStateChange(uint8 MkaPaeIdx,
                                   EthTrcv_LinkStateType TransceiverLinkState) {
  Std_ReturnType LddReturnValue;
  LddReturnValue = E_OK;
  if (MkaPaeIdx >= MKA_TOTAL_NUM_MKA_PAE_INSTANCE) {
    LddReturnValue = E_NOT_OK;

#if (MKA_DEV_ERROR_DETECT == STD_ON)
    Det_ReportError(MKA_MODULE_ID, MKA_INSTANCE_ID, MKA_LINK_STATE_SET_ID,
                    MKA_E_INVALID_PARAMETER);
#endif

  } else {

    if (TransceiverLinkState == ETHTRCV_LINK_STATE_ACTIVE) {

      LddReturnValue = E_OK;
      /*Start the MKA Sequence*/

      if (Mka_GaaPaeServerStatus[MkaPaeIdx].ddpaetatus ==
          MKA_STATUS_WAITING_PEER_LINK) {

        if (!LddReturnValue) {
          /*linkup for a PAE INSTANCE -> */
          Mka_InternalLinkUpState[MkaPaeIdx] = MKA_TRUE;
          /*Mka_GaaPaeServerStatus[MkaPaeIdx].ddServerInternalState =
              MKA_INTR_STATE_MKA_INITIALIZED;*/
        } else {
          /*Report DET Error*/
        }
      }

    } else if (TransceiverLinkState == ETHTRCV_LINK_STATE_DOWN) {
      /*Donot start the MKA Sequence*/

      Mka_GaaPaeServerStatus[MkaPaeIdx].ddpaetatus =
          MKA_STATUS_WAITING_PEER_LINK;
      LddReturnValue = E_OK;

    } else if ((TransceiverLinkState != ETHTRCV_LINK_STATE_DOWN) &
               (TransceiverLinkState != ETHTRCV_LINK_STATE_ACTIVE)) {
      /*For Third Scenario*/
      LddReturnValue = E_NOT_OK;
#if (MKA_DEV_ERROR_DETECT == STD_ON)
      Det_ReportError(MKA_MODULE_ID, MKA_INSTANCE_ID, MKA_LINK_STATE_SET_ID,
                      MKA_E_INVALID_PARAMETER);
#endif
    } else {
      /*Do nothing*/
    }
  }
  return LddReturnValue;
}
/*******************************************************************************
** Function Name        : Mka_MainFunction                                    **
** Service ID           : 0x1d                                                **
** Description          : To inform MKA that a dedicated Trcv/Switch/PAC port **
                          link state has changed                              **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx,                  **
**                        Non reentrant for the same MkaPaeIdx                **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of   **
**                        the MKA module                                      **
**                        TransceiverLinkState:                               **
**						  The Ethernet link state of a
physical Ethernet      **
**						  connection. **
** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType                                      **
**                        E_OK: The request has been accepted and MKA module has
**                        been informed that a dedicated Trcv/Switch proteced
port
**                        link state has changed and can start with the
**						  MKA sequence.
**                        E_NOT_OK: MKA didn’t start the MKA sequence.        **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : MKA_SDD_XXXX                                        **
*******************************************************************************/
void Mka_MainFunction(void) {


  uint8 Luscount;
  

  for (Luscount = MKA_ZERO; Luscount < MKA_TOTAL_NUM_MKA_PAE_INSTANCE; Luscount++) {

    if(Mka_InternalLinkUpState[Luscount] == MKA_TRUE )
    {
       Mka_MainHandler(Luscount);
    }
     


    /*TBD for Key Generation */
    /*#if(MKA_SERVER_ROLE)*/


    /*#endif*/
    /*#if(MKA_SERVER_ROLE)*/

 
    /*MKPDU Preparation Using negotiation*/
 
    

 
    /*
      TBD
      1. MKA Sequence start after Mka_LinkStateChange is invoked
      2. ..
      3. MKPDU prepare
      4. ..
      5. Invoke EthIf_MacSecOperational


    */

    /* Requests the Ethernet Switch to update the SecY/PAC of the the provided
     * port with the provided parameters */
    /* A Transmission Secure Channel with the provided SCI shall be configured
     * during the first call. */
    // EthIf_MacSecUpdateSecY();
  }
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/* parasoft-end-suppress MISRAC2012-RULE_20_1-a-4 */
/* parasoft-end-suppress MISRAC2012-RULE_1_1-c-2 */
/* parasoft-end-suppress CERT_C-DCL19-a-3 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 MISRAC2012-RULE_5_4-d-2 */
/* parasoft-end-suppress CERT_C-DCL00-b-3 */
/*******************************************************************************
**                          End of File                                       **
*******************************************************************************/
