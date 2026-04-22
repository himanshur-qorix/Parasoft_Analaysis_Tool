/*******************************************************************************
**                       Qorix Technologies Limited **
**                                                                            **
** Qorix Technologies Limited owns all the rights to this work. This **
** work shall not be copied,  reproduced,  used,  modified or its information **
** disclosed without the prior written authorization of Qorix Technologies **
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
#include "Mka.h"
#include "Csm.h"
#include "EthIf.h"
#include "Eth_GeneralTypes.h"
#include "Mka_Cfg.h"
#include "Mka_External_Cfg.h"
#include "Platform_Types.h"




/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/* Design_ID          : MKA_SDD_XXXX  */

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
uint32 Mka_BascsicParameterMemberNumber[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
Mka_Eapol_Type Mka_Eapol_parameterSet[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
Mka_PeerList_type Mka_PotentialPeerList[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
Mka_TLV_Type Mka_MacsecTLV[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
Mka_PeerList_type Mka_LivePeerList[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
Mka_MemberIdentifierStorageType Mka_StorageMI[MKA_TOTAL_NUM_MKA_PAE_INSTANCE]
                                             [MKA_ONE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
Mka_Distributed_SAK_type Mka_DistributedSak[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
Mka_Use_ParamterSet_Type Mka_UseParameterSetSak[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
uint8 MKa_CipherText[MKA_TWENTY_FOUR];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
 boolean Mka_InternalLinkUpState[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
Mka_MacSecConfigType Mka_GaaMacSecConfig[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

/*For Asynchronous CSM  JOBS */
#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
 Mka_JobRegistryType Mka_JobRegistry[MKA_TOTAL_NUM_KAY_PARTICIPANT];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
 uint8 Mka_JobRegistyCounter[MKA_TOTAL_NUM_MKA_PAE_INSTANCE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
uint8 Mka_MiValue[MKA_TOTAL_NUM_KAY_PARTICIPANT][MKA_TWELEVE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
Mka_Kay_StatusType MKa_GaaKayStatus[MKA_TOTAL_NUM_KAY_PARTICIPANT];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
  uint8 Mka_MI_Value[MKA_TOTAL_NUM_KAY_PARTICIPANT][MKA_TWELEVE];
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"


#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
 Mka_MkPduType Mka_Pdu[MKA_TOTAL_NUM_KAY_PARTICIPANT] ;
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"
uint8 Mka_IcvGen[MKA_TOTAL_NUM_KAY_PARTICIPANT][MKA_ICV_LENGTH] ;
#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"


#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

uint8 MKA_PortPhysicalAddress[MKA_TOTAL_NUM_KAY_PARTICIPANT];

#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

#define MKA_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

 Mka_FrameType Frame_list_type[MKA_TOTAL_NUM_KAY_PARTICIPANT];

#define MKA_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Mka_MemMap.h"

boolean Mka_CAK_AVAILABLE  = MKA_FALSE;


/*******************************************************************************
**                      Function Definitions                                  **
*******************************************************************************/
/*******************************************************************************
** Function Name        : Mka_StartSeverElection **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
Std_ReturnType Mka_StartSeverElection(void) {
  Std_ReturnType LocalRet;
  /*
   * Callout shall be filled by the system designer
   */
  LocalRet = E_OK;

  Mka_CipherSuiteSelection();

  return LocalRet;
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_StartSeverElection **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
Std_ReturnType Mka_StartNegotiation(
    uint8 PaeIndx /*Pae index for Index for Port access entity*/) {
  Mka_PaeInstanceType const *Lp_PaeInstaceStatic;
  Mka_KayParticipants const *LpMkaKayParticipants;

  BufReq_ReturnType LddBuffRetStatus;
  Std_ReturnType LddReturnValue;
  uint8 *LpBuffptr;
  uint8 LusCount;
  uint8 Luscount2;
  uint8 offset;
  uint32 Mka_ICVLength;
  const uint8 *LpPhyAddressptr;
  uint16 *LusTotalLen;
  uint16 *MkaBasic_Length;
  uint32 Mka_MI_length;
  uint8 b0;
  uint8 b1;
  uint16 buffer_len;

  Eth_BufIdxType Mka_BuffrIdx_Id;
  Lp_PaeInstaceStatic = &Mka_GaaPaeInstances[PaeIndx];
  Mka_MI_length = MKA_MEMBER_IDENTIFIER_LENGTH;
  LddReturnValue = E_NOT_OK;
  offset = MKA_ZERO;
  Mka_ICVLength = MKA_ICV_LENGTH;
  b0 = MKA_ZERO;
  if (Lp_PaeInstaceStatic->ddMkaRole == MKA_PEER) {
    buffer_len = 68U;
  } else {
    buffer_len = 68U;
  }

  if ((Lp_PaeInstaceStatic->pMkaKayParticipants != NULL_PTR) &&
      (Lp_PaeInstaceStatic->pMkaKayParticipants)->blMkaParticipantActivate ==
          STD_ON) {
    for (Luscount2 = Lp_PaeInstaceStatic->ucindexofKayParticipant;
         Luscount2 < Lp_PaeInstaceStatic->ucnoofKayParticipant; Luscount2++) {
      LpMkaKayParticipants = &Mka_KayParticipant[Luscount2];
      LpBuffptr = (uint8 *)NULL_PTR;
      LpPhyAddressptr = (uint8 *)&(Lp_PaeInstaceStatic->usMkaDstMacAddress);
     
      /*Storing the MI for a particular KAY*/
      memcpy(&Mka_StorageMI[PaeIndx][MKA_ZERO].MI,&Mka_IcvGen[Luscount2][PaeIndx], Mka_MI_length);
      /*Mka Pdu Creation Creation*/
      LddBuffRetStatus = EthIf_ProvideTxBuffer(
          Lp_PaeInstaceStatic->ucMkaEthIfControllerIndex, MKA_MACSEC_FRAME_TYPE,
          /*priority value fetch */ MKA_ZERO, &Mka_BuffrIdx_Id, &LpBuffptr,
          &buffer_len);
      if (!LddBuffRetStatus) {
        LpBuffptr[offset++] = EAPOL_VERSION_3;
        LpBuffptr[offset++] = EAPOL_TYPE_MKA;
        LusTotalLen = (uint16 *)&LpBuffptr[offset];
        LpBuffptr[offset++] = MKA_ZERO;
        LpBuffptr[offset++] = MKA_ZERO;
        LpBuffptr[offset++] = MKA_VERSION_BASCIC_PARAMETER;
        LpBuffptr[offset++] =
            (uint8)Lp_PaeInstaceStatic->ucMkaKeyServerPriority;
        MkaBasic_Length = (uint16 *)&LpBuffptr[offset];
        LpBuffptr[offset++] = MKA_ZERO;
        LpBuffptr[offset++] = MKA_ZERO;
        /*SCI*/
        for (LusCount = MKA_ZERO; LusCount < MKA_SIX; LusCount++) {
          LpBuffptr[offset++] =
              *((Lp_PaeInstaceStatic->usMkaSrcMacAddress) + LusCount);
        }
        LpBuffptr[offset++] = Lp_PaeInstaceStatic->ucMkaEthIfControllerIndex;
        LpBuffptr[offset++] = MKA_ZERO;
        memcpy(&LpBuffptr[offset], &Mka_IcvGen[Luscount2][PaeIndx], MKA_MEMBER_IDENTIFIER_LENGTH);
        offset += MKA_MEMBER_IDENTIFIER_LENGTH;
        if ((Mka_BascsicParameterMemberNumber[PaeIndx] == MKA_ZERO) ||
            Mka_BascsicParameterMemberNumber[PaeIndx] >
                MKA_MAXIMUM_LENGTH_OF_AMN) {
          LpBuffptr[offset++] = MKA_ZERO;
          LpBuffptr[offset++] = MKA_ZERO;
          LpBuffptr[offset++] = MKA_ZERO;
          LpBuffptr[offset++] = MKA_ONE;
          Mka_BascsicParameterMemberNumber[PaeIndx]++;
        } else {
          Mka_BascsicParameterMemberNumber[PaeIndx]++;

          LpBuffptr[offset++] = ((Mka_BascsicParameterMemberNumber[PaeIndx]) &
                                 MKA_TWO_FIFTY_FIVE);
          LpBuffptr[offset++] =
              ((Mka_BascsicParameterMemberNumber[PaeIndx] >> MKA_EIGHT) &
               MKA_TWO_FIFTY_FIVE);
          LpBuffptr[offset++] =
              ((Mka_BascsicParameterMemberNumber[PaeIndx] >> MKA_SIXTEEN) &
               MKA_TWO_FIFTY_FIVE);
          LpBuffptr[offset++] =
              ((Mka_BascsicParameterMemberNumber[PaeIndx] >> MKA_TWENTY_FOUR) &
               MKA_TWO_FIFTY_FIVE);
        }

        LpBuffptr[offset++] =
            (MKA_ALGO_AGILITY_STANDARD >> MKA_TWENTY_FOUR) & MKA_TWO_FIFTY_FIVE;
        LpBuffptr[offset++] =
            (MKA_ALGO_AGILITY_STANDARD >> MKA_SIXTEEN) & MKA_TWO_FIFTY_FIVE;
        LpBuffptr[offset++] =
            (MKA_ALGO_AGILITY_STANDARD >> MKA_EIGHT) & MKA_TWO_FIFTY_FIVE;

        LpBuffptr[offset++] = (uint8)(MKA_ALGO_AGILITY_STANDARD);

        for (LusCount = MKA_ZERO; LusCount < (MKA_CKN_LENGTH); LusCount++) {
          LpBuffptr[offset++] = (LpMkaKayParticipants)->Mka_Ckn[LusCount];
        }

        b0 |= (Lp_PaeInstaceStatic->ddMkaRole << MKA_SEVEN);
        b0 |= (MKA_TRUE << MKA_SIX);
        b0 |= ((Mka_GaaCryptoAlgoConfig[(LpMkaKayParticipants)
                                            ->ucMkaCryptoAlgoRefConfigIdx]
                    .ddMkaMacSecCapability &
                MKA_THREE)
               << MKA_FOUR);
        b0 |= (uint8)((((offset - MKA_EIGHT) & (MKA_MASK)) >> MKA_EIGHT) &
                      MKA_FIFTEEN);
        b1 = (uint8)(((offset - MKA_EIGHT) & (MKA_MASK)) & MKA_TWO_FIFTY_FIVE);

        ((uint8 *)MkaBasic_Length)[MKA_ZERO] = b0;
        ((uint8 *)MkaBasic_Length)[MKA_ONE] = b1;

  
        /*TLV Frame for MacSec Cipher Suites*/
       
        LpBuffptr[offset++] = MKA_TWO_FIFTY_FIVE;
        LpBuffptr[offset++] = MKA_ZERO;
        LpBuffptr[offset++] = MKA_ZERO;
        LpBuffptr[offset++] = MKA_SIXTEEN;

        memcpy(&LpBuffptr[offset],&Mka_IcvGen[Luscount2][MKA_ZERO], MKA_ICV_LENGTH);
        offset += Mka_ICVLength;

        ((uint8 *)(LusTotalLen))[MKA_ZERO] = (offset - MKA_FOUR);
        ((uint8 *)(LusTotalLen))[MKA_ONE] = MKA_ZERO;

        LddReturnValue =
            EthIf_Transmit(Lp_PaeInstaceStatic->ucMkaEthIfControllerIndex,
                           Mka_BuffrIdx_Id, MKA_MACSEC_FRAME_TYPE,
                           /*Where will this value come from*/ (boolean)1,
                           (uint16)offset, LpPhyAddressptr);

        Mka_GaaPaeServerStatus[PaeIndx].ddServerInternalState = MKA_INTRA_STATE_TX_ADV_FRAME_SENT;
        Mka_GaaPaeServerStatus[PaeIndx].ddpaetatus  = MKA_STATUS_IN_PROGRESS;
        MKa_GaaKayStatus[Luscount2] = MKA_INTR_WAIT_FOR_BP_SET;
        
      } else {
        LddReturnValue = E_NOT_OK;
      }
    }
  }

  return LddReturnValue;
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_ProcessKeyServerAnouncementRecd **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_ProcessMkPdu(uint8 PaeIdx, const uint8 *LpSduDataPtr, uint16 LenBytes,
                      uint8 *offset_main) {

  uint8 *offset_;
  uint8 off;
  uint8 Luscount;
  uint16 MKA_FLAG_and_length;
  uint16 length;
  off = MKA_ZERO;
  MKA_FLAG_and_length = MKA_ZERO;
  offset_ = &off;

  Mka_Intr_be7bit(LpSduDataPtr, &Mka_Eapol_parameterSet[PaeIdx].usEapol_version,
                  &LenBytes, offset_);
  Mka_Intr_be7bit(LpSduDataPtr,
                  &Mka_Eapol_parameterSet[PaeIdx].usEapol_PSetType, &LenBytes,
                  offset_);
  Mka_Intr_be16(LpSduDataPtr, &Mka_Eapol_parameterSet[PaeIdx].usEapo_pdu_Length,
                &LenBytes, offset_);
  Mka_Intr_be7bit(LpSduDataPtr,
                  &Mka_GaaBasicparameterset[PaeIdx].Mka_VersionIdentifier,
                  &LenBytes, offset_);
  Mka_Intr_be7bit(LpSduDataPtr,
                  &Mka_GaaBasicparameterset[PaeIdx].Mka_KeyServerPriority,
                  &LenBytes, offset_);
  Mka_Intr_be16(LpSduDataPtr, &MKA_FLAG_and_length, &LenBytes, offset_);
  Mka_Intr_be64(LpSduDataPtr, &Mka_GaaBasicparameterset[PaeIdx].Mka_Sci,
                &LenBytes, offset_);

  for (Luscount = MKA_ZERO; Luscount < 12; Luscount++) {
    Mka_Intr_be7bit(LpSduDataPtr,
                    &Mka_GaaBasicparameterset[PaeIdx].Mka_AcMi[Luscount],
                    &LenBytes, offset_);
  }

  Mka_Intr_be32(LpSduDataPtr, &Mka_GaaBasicparameterset[PaeIdx].Mka_AMn,
                &LenBytes, offset_);

  Mka_Intr_be32(LpSduDataPtr, &Mka_GaaBasicparameterset[PaeIdx].Mka_AlgoAgility,
                &LenBytes, offset_);

  for (Luscount = MKA_ZERO; Luscount < MKA_CKN_LENGTH; Luscount++) {
    Mka_Intr_be7bit(LpSduDataPtr,
                    &Mka_GaaBasicparameterset[PaeIdx].Mka_Ckn[Luscount],
                    &LenBytes, offset_);
  }

  /* for (Luscount = MKA_ZERO; Luscount < MKA_ICV_LENGTH; Luscount++) {
     Mka_Intr_be7bit(LpSduDataPtr, &Mka_GaaICV[PaeIdx][Luscount], &LenBytes,
   offset_);
   }*/

  length =
      (((MKA_FLAG_and_length >> 8) & 0x0F) | (MKA_FLAG_and_length & 0x0FFFF));
  Mka_GaaBasicparameterset[PaeIdx].Mka_flag.ddkeyserver =
      ((MKA_FLAG_and_length >> 14) & 0x01);
  Mka_GaaBasicparameterset[PaeIdx].Mka_flag.ddmacdesired =
      ((MKA_FLAG_and_length >> 13) & 0x01);
  Mka_GaaBasicparameterset[PaeIdx].Mka_flag.ddmacsecCapability =
      ((MKA_FLAG_and_length >> 12) & 0x03);

  /*
  typedef struct STag_Mka_StatusFlag_Type {
    uint8 ddkeyserver : 1;
    uint8 ddmacdesired : 1;
    uint8 ddmacsecCapability : 2;
    uint16 ddmacparameterser : 12;

  } Mka_StatusFlag_Type;

  typedef struct STag_Mka_BasicParameterSet_Type {
    uint8 Mka_VersionIdentifier;
    uint8 Mka_KeyServerPriority;
    Mka_StatusFlag_Type Mka_flag;
    uint64 Mka_Sci;
    uint32 Mka_AcMi[3];
    uint32 Mka_AMn;
    uint32 Mka_AlgoAgility;
    uint16 Mka_Ckn[16];
  } Mka_BasicParameterSet_Type;
  */

  if (PaeIdx && LpSduDataPtr && LenBytes) {
  } else {
  }
  *offset_main += (MKA_FOUR + length);

  if (offset_main) {
  }
  /*ICV Generation */

  /*ICV Generation for each KAY Participant and Verification on Peers end*/
  /*
     Create Potential peer list and live peer list from place holder
  */
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_CknMatching                                  **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
Std_ReturnType Mka_CknMatching(Mka_BasicParameterSet_Type *Basic_pramtersetType,
                               const uint8 *CKN) {

  uint8 Luscount;
  uint16 Mka_leangth;
  Std_ReturnType LddReturnValue;
  boolean LusMatch = MKA_TRUE;
  Mka_leangth = MKA_CKN_LENGTH;

  for (Luscount = 0; (Luscount < Mka_leangth) && (LusMatch != MKA_FALSE);
       Luscount++) {
    if (Basic_pramtersetType->Mka_Ckn[Luscount] != CKN[Luscount]) {
      LusMatch = MKA_FALSE;
    }
  }

  if (LusMatch) {
    LddReturnValue = E_OK;
  } else {
    LddReturnValue = E_NOT_OK;
  }

  return LddReturnValue;
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_IcvMatching                                  **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

Std_ReturnType Mka_IcvMatching(Mka_BasicParameterSet_Type *Basic_pramtersetType,
                               const uint8 *CKN, uint8 PaeIdx,
                               Mka_PaeInstanceType const *LpMkaPaeInstance,
                               uint8 *Mka_ICV, const uint8 *PhysAddrPtr)

{

  uint8 Mka_MPDUPtr[84];
  uint8 Luscount;
  uint8 Mka_Dst[5];
  uint8 Mka_ICV_Arr[16];
  uint8 *Mka_ICV_ptr;
  uint8 *verifyPtr;
  uint32 Mka_MSPduLength;
  uint32 Mka_ICVLength;
  Std_ReturnType LddReturnValue;

  Mka_ICV_ptr = Mka_ICV_Arr;
  verifyPtr = NULL_PTR;
  /*Mistake This needs to be corrected */

  if (Basic_pramtersetType[PaeIdx].Mka_AlgoAgility != MKA_ALGO_AGILITY_STANDARD)

  {
    LddReturnValue = E_NOT_OK;
  } else {
    /*Prepare MSPDU*/

    memcpy(Mka_MPDUPtr, PhysAddrPtr, MKA_SIX * sizeof(uint8));
    for (Luscount = MKA_ZERO; Luscount <= MKA_FIVE; Luscount++) {

      Mka_Dst[Luscount] = *((LpMkaPaeInstance->usMkaDstMacAddress) + Luscount);
    }
    /*For Conversion to hexa decimnal we will use a Macro*/
    memcpy(Mka_MPDUPtr + MKA_SIX * sizeof(uint8), Mka_Dst, sizeof(Mka_Dst));
    memcpy(Mka_MPDUPtr + MKA_TWELEVE * sizeof(uint8), Basic_pramtersetType,
           sizeof(Mka_BasicParameterSet_Type));
    memcpy(Mka_MPDUPtr + MKA_TWELEVE * sizeof(uint8) +
               sizeof(Mka_BasicParameterSet_Type),
           &Mka_GaaAnouncementParameterSet[PaeIdx],
           sizeof(Mka_AnouncementSet_Type));

    Mka_MSPduLength = MKA_TWELEVE * sizeof(uint8) + sizeof(Mka_BasicParameterSet_Type) +
                      sizeof(Mka_AnouncementSet_Type);
    /*Macro Values to ve used*/
    Mka_ICVLength = MKA_SIXTEEN;
    LddReturnValue = Mka_ICV_Generation_Callout(
        PaeIdx,
        LpMkaPaeInstance->pMkaKayParticipants
            ->usMkaCryptoIcvGenerateJobRefCsmJobId,
        LpMkaPaeInstance->pMkaKayParticipants->usMkaCryptoICKKeyRefCsmKeyId,
        &Mka_MPDUPtr[MKA_ZERO], Mka_MSPduLength, Mka_ICV_ptr, &Mka_ICVLength);
    if (!LddReturnValue) {

    /*
     Mka_CALLOUT_MAC_VERIFY(
    uint8 Jobid,
    Crypto_OperationModeType operationmode,
    const uint8 * dataptr,
    uint32 Length,
    const  uint8 *Macptr,
    uint32 Length,
    Crypto_VerifyResultType *verifyPtr);
    */
   
      LddReturnValue = Mka_Callout_Mac_Verify(
          LpMkaPaeInstance->pMkaKayParticipants
              ->usMkaCryptoIcvVerifyJobRefCsmJobId,
          CRYPTO_OPERATIONMODE_SINGLECALL, &Mka_MPDUPtr[MKA_ZERO], Mka_MSPduLength,
          Mka_ICV_ptr, Mka_ICVLength, (Crypto_VerifyResultType *)verifyPtr);
    }
    /*  for (Luscount = 0; (Luscount < 16) && (LblMatch != MKA_FALSE);
      Luscount++) { if (Mka_GaaICV[PaeIdx][Luscount] == Mka_ICV_Arr[Luscount]) {

          LblMatch = MKA_TRUE;
          LddReturnValue = E_OK;
        } else {
          LblMatch = MKA_FALSE;
          LddReturnValue = E_NOT_OK;
        }
      }
  */
    if (LddReturnValue == E_NOT_OK) {
      /*Nothing*/
    } else {
      /*Nothing*/
    }

    if (Mka_MSPduLength && Mka_ICV && CKN && Mka_ICVLength) {
    } else {
      /*Do nothing*/
    }
  }

  return LddReturnValue;
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_PreparePotentialList **
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

#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_ServerStateMachine(uint8 Paeidx) {

  Std_ReturnType LddReturnValue;
  boolean Mka_status;
  uint8 Luscount;
  const Mka_PaeInstanceType * Lp_PaeInstanceStatic;
  Lp_PaeInstanceStatic = &Mka_GaaPaeInstances[Paeidx];
  LddReturnValue = E_NOT_OK;
  for(Luscount = Lp_PaeInstanceStatic->ucindexofKayParticipant; Luscount < Lp_PaeInstanceStatic->ucnoofKayParticipant; Luscount++ )
  {
  switch (MKa_GaaKayStatus[Luscount])
  {
    
    case MKA_INTR_INITIAL:
    {
       LddReturnValue = Mka_ICKCallhandler(Luscount,Paeidx);
       if(E_OK == LddReturnValue)
       {

       }
      break;
    }
    case MKA_INTR_ICK_COMPLETE:
    {
      LddReturnValue = Mka_KEKCallhandler(Luscount,Paeidx);
       if(E_OK == LddReturnValue)
       {

       }
       break;
    }

    case MKA_INTR_KEK_COMPLETE:
    {
             LddReturnValue = Mka_GetLinkUpsate(Paeidx,&Mka_status);
       if(E_OK == LddReturnValue)
    {
        if(Mka_status == MKA_TRUE)
        {
          MKa_GaaKayStatus[Luscount] = MKA_INTR_IN_PROGRESS;
        
         Mka_GaaPaeServerStatus[Paeidx].ddpaetatus = MKA_STATUS_WAITING_PEER;
       
        }

    }
      break;

    }
    case MKA_INTR_WAIT_FOR_ICK:
    {
      
      break;

    }

    case MKA_INTR_WAIT_FOR_KEK:
    {

      break;
    }
default:
{
  break;
}


 
   }

  }


         if ((Mka_GaaPaeServerStatus[Paeidx].ddServerInternalState ==
          MKA_STATUS_WAITING_PEER_SAK_DERIVATION_INPROGRESS) && Mka_Selectedparticipant.blkaypartselect == MKA_TRUE) {
        
              
           Mka_GaaPaeServerStatus[Paeidx].ddServerInternalState = MKA_STATUS_WAITING_PEER_ICV_DERIVATION_INPROGRESS;

      }
  }

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_PreparePotentialList **
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

#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_ClientStateMachine(
                            uint8 Paeidx) {
  Std_ReturnType LddReturnValue;
  boolean Mka_status;
  uint8  Luscount;
  const Mka_PaeInstanceType * Lp_PaeInstanceStatic;
  Lp_PaeInstanceStatic = &Mka_GaaPaeInstances[Paeidx];
  /*Mka_KayParticipants const *LpMkaKayParticipants;*/
  LddReturnValue = E_NOT_OK;
   for(Luscount = Lp_PaeInstanceStatic->ucindexofKayParticipant; Luscount < Lp_PaeInstanceStatic->ucnoofKayParticipant; Luscount++ )
  {
  switch (MKa_GaaKayStatus[Luscount])
  {
    
    case MKA_INTR_INITIAL:
    {
       LddReturnValue = Mka_ICKCallhandler(Luscount,Paeidx);
       if(E_OK == LddReturnValue)
       {

       }
      break;
    }
    case MKA_INTR_ICK_COMPLETE:
    {
      LddReturnValue = Mka_KEKCallhandler(Luscount,Paeidx);
       if(E_OK == LddReturnValue)
       {

       }
       break;
    }

    case MKA_INTR_KEK_COMPLETE:
    {
       LddReturnValue = Mka_GetLinkUpsate(Paeidx,&Mka_status);
       if(E_OK == LddReturnValue)
    {
        if(Mka_status == MKA_TRUE)
        {
          MKa_GaaKayStatus[Luscount] = MKA_INTR_IN_PROGRESS;
        
         Mka_GaaPaeServerStatus[Paeidx].ddpaetatus = MKA_STATUS_WAITING_PEER;
       
        }

    }
      break;

    }
    case MKA_INTR_WAIT_FOR_ICK:
    {
      /*Do nothing*/
      break;

    }

    case MKA_INTR_WAIT_FOR_KEK:
    {
      /*Do nothing*/
      break;
    }
default:
{
  break;
}

   }

  }
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_PreparePotentialList **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_PreparePotentialPeerList(Mka_PaeInstanceType const *LpMkaPaeInstance,
                                  uint8 Paeidx, uint8 *LpBuffPtr,
                                  uint8 *offset_ptr) {

  uint8 LusCount;
  uint8 local_Offset;

  local_Offset = *offset_ptr;

  /*   Parameter Set Type*/
  if (LpMkaPaeInstance != NULL_PTR) {
    LpBuffPtr[local_Offset++] = 0x02;
    /*0 for Potentila Peer as stated in Figure 11-7 IEEE802.1.X*/
    LpBuffPtr[local_Offset++] = 0x00;
    LpBuffPtr[local_Offset++] = 0x00;
    LpBuffPtr[local_Offset++] = 0x10;

    for (LusCount = 0; LusCount < 12; LusCount++) {
      LpBuffPtr[local_Offset++] =
          Mka_GaaBasicparameterset[Paeidx].Mka_AcMi[LusCount];
    }
    LpBuffPtr[local_Offset++] = Mka_GaaBasicparameterset[Paeidx].Mka_AMn;
    LpBuffPtr[local_Offset++] =
        Mka_GaaBasicparameterset[Paeidx].Mka_AMn >> 8 & 0xFF;
    LpBuffPtr[local_Offset++] =
        Mka_GaaBasicparameterset[Paeidx].Mka_AMn >> 16 & 0xFF;
    LpBuffPtr[local_Offset++] =
        Mka_GaaBasicparameterset[Paeidx].Mka_AMn >> 24 & 0xFF;
  }

  *offset_ptr = local_Offset;

  if (Paeidx) {
    /* Do nothing */
  }
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_PreparePotentialList **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_PrepareLivePeerList(Mka_PaeInstanceType const *LpMkaPaeInstance,
                             uint8 Paeidx, uint8 *LpBuffPtr,
                             uint8 *offsetmain) {
  Std_ReturnType LddReturnValue;
  uint8 local_offset;

  uint8 LusCount;

  local_offset = *offsetmain;

  LddReturnValue = E_OK;
  /*   Parameter Set Type*/
  if (LpMkaPaeInstance != NULL_PTR) {
    LpBuffPtr[local_offset++] = 0x01;
    /*The least significant octet of the Key Server’s transmit SSCI is encoded
    in MKPDUs containing a Distributed SAK parameter set for use with an XPN
    Cipher Suite; otherwise, 0 is encoded.The Key Server’s SSCI is distributed
    only in Live Peer Lists and is transmitted as zero and ignored on receipt in
    Potential Peer Lists.*/

    LpBuffPtr[local_offset++] = 0x01;
    LpBuffPtr[local_offset++] = 0x00;
    LpBuffPtr[local_offset++] = 0x10;
  }
  for (LusCount = 0; (LusCount < 12) && (!LddReturnValue); LusCount++) {
    if (Mka_PotentialPeerList[Paeidx].Mka_LivePotentialMId[LusCount] ==
        Mka_StorageMI[Paeidx][MKA_ZERO].MI[LusCount]) {
      LddReturnValue = E_OK;
    } else {
      LddReturnValue = E_NOT_OK;
    }
  }
  if (!LddReturnValue) {
    for (LusCount = 0; LusCount < 12; LusCount++) {
      LpBuffPtr[local_offset++] =
          Mka_GaaBasicparameterset[Paeidx].Mka_AcMi[LusCount];
    }

    if (Mka_GaaBasicparameterset[Paeidx].Mka_AMn !=
        Mka_BascsicParameterMemberNumber[Paeidx]) {
      LpBuffPtr[local_offset++] = 0x00;
      LpBuffPtr[local_offset++] = 0x00;
      LpBuffPtr[local_offset++] = 0x00;
      LpBuffPtr[local_offset++] = Mka_BascsicParameterMemberNumber[Paeidx];
    }
    *offsetmain = local_offset;
  } else {
    /*Nothing*/
  }

  if (Paeidx) {
    /*Do nothing*/
  } else {
    /* Do nothing*/
  }
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Intr_be32 **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_Intr_be32(const uint8 *data, uint32 *out, uint16 *len, uint8 *offset) {
  uint8 pos = *offset;

  *out = ((uint32)data[pos] << 24);
  *out |= ((uint32)data[pos + 1] << 16);
  *out |= ((uint32)data[pos + 2] << 8);
  *out |= (uint32)data[pos + 3];

  pos += 4; // advance offset
  *offset = pos;

  *len -= 4; // reduce length
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Intr_be64 **
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

#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

void Mka_Intr_be64(const uint8 *data, uint64 *out, uint16 *len, uint8 *offset) {
  uint8 pos = *offset;

  *out = ((uint64)data[pos] << 56);
  *out |= ((uint64)data[pos + 1] << 48);
  *out |= ((uint64)data[pos + 2] << 40);
  *out |= ((uint64)data[pos + 3] << 32);
  *out |= ((uint64)data[pos + 4] << 24);
  *out |= ((uint64)data[pos + 5] << 16);
  *out |= ((uint64)data[pos + 6] << 8);
  *out |= (uint64)data[pos + 7];

  pos += 8; // advance the logical offset
  *offset = pos;

  *len -= 8; // reduce remaining length
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Intr_be16 **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

void Mka_Intr_be16(const uint8 *data, uint16 *out, uint16 *len, uint8 *offset) {
  uint8 pos = *offset; // starting index

  *out = (uint16)data[pos] << 8;
  *out |= (uint16)data[pos + 1];

  pos += 2;      // advance logical offset
  *offset = pos; // write back

  *len -= 2; // reduce remaining length
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Intr_be7bit                                     **
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

#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

void Mka_Intr_be7bit(const uint8 *data, uint8 *out, uint16 *len,
                     uint8 *offset) {
  uint8 data_num = *offset;
  *out = data[data_num++];
  data += 1;

  *len -= 1;

  *offset = data_num;
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_PreparePotentialList **
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

#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

void Mka_ProcessPotentialPeerList(uint8 PaeIdx, const uint8 *LpSduDataPtr,
                                  uint16 LenBytes, uint8 *offset_main) {

  uint8 *offset_;
  uint8 off;
  uint8 Luscount;
  uint16 MKA_FLAG_and_length;
  uint16 length;
  off = MKA_ZERO;
  MKA_FLAG_and_length = MKA_ZERO;
  offset_ = &off;

  Mka_Intr_be7bit(LpSduDataPtr,
                  &Mka_PotentialPeerList[PaeIdx].mka_parameterSetType,
                  &LenBytes, offset_main);
  Mka_Intr_be7bit(LpSduDataPtr, &Mka_PotentialPeerList[PaeIdx].Mka_keySSCI,
                  &LenBytes, offset_main);
  Mka_Intr_be16(LpSduDataPtr,
                &Mka_PotentialPeerList[PaeIdx].Mka_parameterBodySetlength,
                &LenBytes, offset_main);
  for (Luscount = MKA_ZERO; Luscount < MKA_TWELEVE; Luscount++) {
    Mka_Intr_be7bit(
        LpSduDataPtr,
        &Mka_PotentialPeerList[PaeIdx].Mka_LivePotentialMId[Luscount],
        &LenBytes, offset_main);
  }

  for (Luscount = MKA_ZERO; Luscount < MKA_FOUR; Luscount++) {
    Mka_Intr_be7bit(
        LpSduDataPtr,
        &Mka_PotentialPeerList[PaeIdx].Mka_LivePotentialMNum[Luscount],
        &LenBytes, offset_main);
  }

  if (offset_ && Luscount) {
  }

  /*
  typedef struct STag_Mka_StatusFlag_Type {
    uint8 ddkeyserver : 1;
    uint8 ddmacdesired : 1;
    uint8 ddmacsecCapability : 2;
    uint16 ddmacparameterser : 12;

  } Mka_StatusFlag_Type;

  typedef struct STag_Mka_BasicParameterSet_Type {
    uint8 Mka_VersionIdentifier;
    uint8 Mka_KeyServerPriority;
    Mka_StatusFlag_Type Mka_flag;
    uint64 Mka_Sci;
    uint32 Mka_AcMi[3];
    uint32 Mka_AMn;
    uint32 Mka_AlgoAgility;
    uint16 Mka_Ckn[16];
  } Mka_BasicParameterSet_Type;
  */

  if (PaeIdx && LpSduDataPtr && LenBytes && MKA_FLAG_and_length) {
  } else {
  }
  // *offset_main +=
  // (MKA_FOUR+Mka_PotentialPeerList[PaeIdx].Mka_parameterBodySetlength);

  if (offset_main && length) {
  }
  /*ICV Generation */

  /*ICV Generation for each KAY Participant and Verification on Peers end*/
  /*
     Create Potential peer list and live peer list from place holder
  */
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Transmission **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_Create_TxPdu(uint8 paeindix,uint8 kay, uint8 mkaframe,
                      const uint8 *PhysAddrPtr) {

  uint8 *LpBuffptr;
  uint8 offset;
  uint8 *offset_ptr;
  const uint8 *Lp_AES_KEY;
  const uint8 *Lp_authenticator_ptr;
  uint16 buffersize;
  uint16 total_len;
  uint16 *total_len_ptr;
  Mka_PaeInstanceType const *Lp_PaeInstaceStatic;
  Mka_KayParticipants const *LpMkaKayParticipants;
  BufReq_ReturnType LddBuffRetStatus;
  Eth_BufIdxType Mka_BuffrIdx_Id;
  Std_ReturnType LddReturnValue;
  Crypto_VerifyResultType *Verify_ptr;
  Mka_SakKeyPtrType *LP_Sak_key_Ptr;
  Crypto_VerifyResultType Verify;
  offset = MKA_ZERO;
  total_len = MKA_ZERO;
  offset_ptr = &offset;
  total_len_ptr = &total_len;
  LP_Sak_key_Ptr = NULL_PTR;
  Verify = CRYPTO_E_VER_OK;

  Verify_ptr = &Verify;

  Lp_PaeInstaceStatic = &Mka_GaaPaeInstances[paeindix];
  LpBuffptr = (uint8 *)NULL_PTR;
  buffersize = (uint16)MKA_ZERO;

  if (paeindix) {
  } else {
  }

  switch (mkaframe) {
  case MKA_BASIC_FRAME_TYPE:
{
    buffersize = (uint16)MKA_EAPOL_BASIC_PDU_LENGTH;
     Mka_Selectedparticipant.blkaypartselect = MKA_TRUE;
    Mka_Selectedparticipant.index = kay;

    LddBuffRetStatus = EthIf_ProvideTxBuffer(
        Lp_PaeInstaceStatic->ucMkaEthIfControllerIndex, MKA_MACSEC_FRAME_TYPE,
        /*priority value fetch */ MKA_ZERO, &Mka_BuffrIdx_Id, &LpBuffptr, &buffersize);

    Mka_CreateBasic_parameter(paeindix, LpBuffptr, offset_ptr, total_len_ptr);
    Mka_PreparePotentialPeerList(Lp_PaeInstaceStatic, paeindix, LpBuffptr,
                                 offset_ptr);
    if (Mka_GaaPaeInstances[paeindix].ddMkaRole == MKA_KEY_SERVER) {
      LddReturnValue = Mka_StartSeverElection();
    }

   /*The TLV macsec is prepared on the peer because the roles are very clearly  Mentione by the autosar*/
   if(Lp_PaeInstaceStatic->ddMkaRole == MKA_PEER)
   {
    LpMkaKayParticipants = &Mka_KayParticipant[kay];
     Mka_PrepareMacsecTLVPeerList(Lp_PaeInstaceStatic,LpMkaKayParticipants,
                                  paeindix, LpBuffptr,
                                   offset_ptr);
   }
    Mka_CreateICV(paeindix,kay, total_len_ptr, LpBuffptr, offset_ptr);
   

    if (!LddBuffRetStatus) {

      LddReturnValue = Mka_Transmission(paeindix, LpBuffptr, buffersize,
                                        Mka_BuffrIdx_Id, PhysAddrPtr);

      if (LddReturnValue) {
        /*E_NOT_OK*/
      } else {

        /*Do nothing */
      }
    } else {
      /* Buffer Fails */
    }

    /*Create Basic Basic Paramter Set PDU*/
    break;
  }

  case MKA_POTENTIAL_TYPE:
  {
    if (Lp_PaeInstaceStatic->ddMkaRole == MKA_KEY_SERVER) {
      buffersize = (uint16)MKA_EAPOL_DISTRI_USE_LIVE_PDU_LENGTH;
      LddBuffRetStatus = EthIf_ProvideTxBuffer(
          Lp_PaeInstaceStatic->ucMkaEthIfControllerIndex, MKA_MACSEC_FRAME_TYPE,
          /*priority value fetch */ MKA_ZERO, &Mka_BuffrIdx_Id, &LpBuffptr,
          &buffersize);
      
 

      Mka_CreateBasic_parameter(paeindix, LpBuffptr, offset_ptr, total_len_ptr);

      Mka_PrepareUseParaList(Lp_PaeInstaceStatic, paeindix, LpBuffptr,
                             offset_ptr);
      Mka_PrepareDistributedSAKList(Lp_PaeInstaceStatic, kay,paeindix, LpBuffptr,
                                    offset_ptr);

      Mka_PrepareLivePeerList(Lp_PaeInstaceStatic, paeindix, LpBuffptr,
                              offset_ptr);
      Mka_CreateICV(paeindix,kay, total_len_ptr, LpBuffptr, offset_ptr);
    } else {
      buffersize = (uint16)MKA_EAPOL_LIVE_Potential_PDU_LENGTH;
      LddBuffRetStatus = EthIf_ProvideTxBuffer(
          Lp_PaeInstaceStatic->ucMkaEthIfControllerIndex, MKA_MACSEC_FRAME_TYPE,
          /*priority value fetch */ MKA_ZERO, &Mka_BuffrIdx_Id, &LpBuffptr,
          &buffersize);

      Mka_CreateBasic_parameter(paeindix, LpBuffptr, offset_ptr, total_len_ptr);

      Mka_PrepareLivePeerList(Lp_PaeInstaceStatic, paeindix, LpBuffptr,
                              offset_ptr);
      Mka_CreateICV(paeindix,kay, total_len_ptr, LpBuffptr, offset_ptr);
    }

    if (!LddBuffRetStatus) {

      LddReturnValue = Mka_Transmission(paeindix, LpBuffptr, buffersize,
                                        Mka_BuffrIdx_Id, PhysAddrPtr);

      /*LP_Sak_key_Ptr->SakKeyPtr = MKa_CipherText[MKA_ZERO]; */
      LddReturnValue = EthIf_MacSecAddTxSa(
          Lp_PaeInstaceStatic->ucMkaEthIfControllerIndex, MKA_ZERO, MKA_ZERO,
          Mka_LivePeerList[paeindix].Mka_keySSCI, LP_Sak_key_Ptr, true);

      if (LddReturnValue) {
        /*E_NOT_OK*/
      } else {

        /*Do nothing */
      }
    } else {
      /* Buffer Fails */
    }

    /*Create Basic Basic Paramter Set PDU*/
    break;
  }

  case MKA_USE_LIVE_TYPE:
  {

    /*Create Basic Basic Paramter Set PDU*/
    Lp_AES_KEY = &Mka_DistributedSak[paeindix].Mka_AES_KEY[MKA_ZERO];
    Lp_authenticator_ptr = &Mka_GaaICV[paeindix][MKA_ZERO];



 
    if(MKa_GaaKayStatus[kay] !=  MKA_INTR_WAIT_UNWRAP)
    {
      LpMkaKayParticipants = &Mka_KayParticipant[kay];
    if(LpMkaKayParticipants->usMkaCryptoUnwrapDeriveJobRefCsmProcessingType != MKA_CRYPTO_PROCESSING_SYNC)
    {
   LddReturnValue = Mka_Callout_Unwrap((Lp_PaeInstaceStatic->pMkaKayParticipants)
                                          ->usMkaCryptoKeyUnwrapJobRefCsmJobId,
                                      (Lp_PaeInstaceStatic->pMkaKayParticipants)
                                          ->usMkaCryptoSakKeyRefCsmKeyId,
                                      Lp_AES_KEY, MKA_TWENTY_FOUR, Lp_authenticator_ptr,
                                      MKA_SIXTEEN, Verify_ptr);
      MKa_GaaKayStatus[kay] = MKA_INTR_UNWRAP_COMPLETE;
    }

    }
    else
    {
      
    }
    if (!LddReturnValue & !(*Verify_ptr)) {
      /*  LP_Sak_key_Ptr->SakKeyPtr = (uint8 *)
       * &((Lp_PaeInstaceStatic->pMkaKayParticipants)->usMkaCryptoSakKeyRefCsmKeyId);*/

      LddReturnValue = EthIf_MacSecAddRxSa(
          Lp_PaeInstaceStatic->ucMkaEthIfControllerIndex,
          Mka_UseParameterSetSak[paeindix].Mka_Latest_Key_AN,
          Mka_UseParameterSetSak[paeindix].Mka_Latest_Key_Lowest_Acceptable_PN,
          Mka_LivePeerList[paeindix].Mka_keySSCI, LP_Sak_key_Ptr, true);
    }

    break;
  }
  /*Create Basic Basic Paramter Set PDU*/
  case MKA_LIVE_TYPE:
  {
    /*Create Basic Basic Paramter Set PDU*/
    break;

  default:
    /*Create Basic Basic Paramter Set PDU*/
    break;
  }
}

  if (PhysAddrPtr) {
  }
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Transmission **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_CreateBasic_parameter(uint8 PaeIndx, uint8 *LpBuffptr,
                               uint8 *offset_ptr, uint16 *LusTotalLen) {
  Mka_PaeInstanceType const *Lp_PaeInstaceStatic;
  Std_ReturnType LddReturnValue;
  uint8 *Mka_MI_ptr;
  uint8 Mka_MI_Value[12];
  uint8 LusCount;
  uint8 local_offset;
  uint16 *MkaBasic_Length;
  uint32 Mka_MI_length;
  uint8 b0;
  uint8 b1;
  //  Mka_Pdu_Type Mka_pdu;
  local_offset = *offset_ptr;
  Lp_PaeInstaceStatic = &Mka_GaaPaeInstances[PaeIndx];
  Mka_MI_ptr = Mka_MI_Value;
  Mka_MI_length = MKA_MEMBER_IDENTIFIER_LENGTH;
  LddReturnValue = E_NOT_OK;
  b0 = MKA_ZERO;
  if ((Lp_PaeInstaceStatic->pMkaKayParticipants != NULL_PTR) &&
      (Lp_PaeInstaceStatic->pMkaKayParticipants)->blMkaParticipantActivate ==
          STD_ON) {

    LddReturnValue =
        Mka_Callout_Random_Generate((Lp_PaeInstaceStatic->pMkaKayParticipants)
                               ->usMkaCryptoRandomJobRefCsmJobId,
                           Mka_MI_ptr, &Mka_MI_length);
    /*Mka Pdu Creation Creation*/

    LpBuffptr[local_offset++] = EAPOL_VERSION_3;
    LpBuffptr[local_offset++] = EAPOL_TYPE_MKA;
    if (LusTotalLen != NULL_PTR) {
      LusTotalLen = (uint16 *)&LpBuffptr[local_offset];
      LpBuffptr[local_offset++] = MKA_ZERO;
      LpBuffptr[local_offset++] = MKA_ZERO;
    }
    LpBuffptr[local_offset++] = MKA_VERSION_BASCIC_PARAMETER;
    LpBuffptr[local_offset++] =
        (uint8)Lp_PaeInstaceStatic->ucMkaKeyServerPriority;
    MkaBasic_Length = (uint16 *)&LpBuffptr[local_offset];
    LpBuffptr[local_offset++] = MKA_ZERO;
    LpBuffptr[local_offset++] = MKA_ZERO;
    /*SCI*/
    for (LusCount = MKA_ZERO; LusCount < MKA_SIX; LusCount++) {
      LpBuffptr[local_offset++] =
          *((Lp_PaeInstaceStatic->usMkaSrcMacAddress) + LusCount);
    }
    LpBuffptr[local_offset++] = Lp_PaeInstaceStatic->ucMkaEthIfControllerIndex;
    LpBuffptr[local_offset++] = MKA_ZERO;
    memcpy(&LpBuffptr[local_offset], Mka_MI_ptr, MKA_MEMBER_IDENTIFIER_LENGTH);
    local_offset += MKA_MEMBER_IDENTIFIER_LENGTH;
    if ((Mka_BascsicParameterMemberNumber[PaeIndx] == MKA_ZERO) ||
        Mka_BascsicParameterMemberNumber[PaeIndx] > MKA_MAXIMUM_LENGTH_OF_AMN) {
      LpBuffptr[local_offset++] = MKA_ZERO;
      LpBuffptr[local_offset++] = MKA_ZERO;
      LpBuffptr[local_offset++] = MKA_ZERO;
      LpBuffptr[local_offset++] = MKA_ONE;
      Mka_BascsicParameterMemberNumber[PaeIndx]++;
    } else {
      Mka_BascsicParameterMemberNumber[PaeIndx]++;

      LpBuffptr[local_offset++] =
          ((Mka_BascsicParameterMemberNumber[PaeIndx] >> 24) & 0xFF);
      LpBuffptr[local_offset++] =
          ((Mka_BascsicParameterMemberNumber[PaeIndx] >> 16) & 0xFF);
      LpBuffptr[local_offset++] =
          ((Mka_BascsicParameterMemberNumber[PaeIndx] >> 8) & 0xFF);
      LpBuffptr[local_offset++] =
          (uint8)((Mka_BascsicParameterMemberNumber[PaeIndx]));
    }

    LpBuffptr[local_offset++] = (MKA_ALGO_AGILITY_STANDARD >> 24) & 0xFF;
    LpBuffptr[local_offset++] = (MKA_ALGO_AGILITY_STANDARD >> 16) & 0xFF;
    LpBuffptr[local_offset++] = (MKA_ALGO_AGILITY_STANDARD >> 8) & 0xFF;

    LpBuffptr[local_offset++] = (uint8)(MKA_ALGO_AGILITY_STANDARD);

    for (LusCount = 0; LusCount < (MKA_CKN_LENGTH); LusCount++) {
      LpBuffptr[local_offset++] =
          (Lp_PaeInstaceStatic->pMkaKayParticipants)->Mka_Ckn[LusCount];
    }

    b0 |= (Lp_PaeInstaceStatic->ddMkaRole << 7);
    b0 |= (MKA_TRUE << 6);
    b0 |= ((Mka_GaaCryptoAlgoConfig[(Lp_PaeInstaceStatic->pMkaKayParticipants)
                                        ->ucMkaCryptoAlgoRefConfigIdx]
                .ddMkaMacSecCapability &
            0x03)
           << 4);
    b0 |= (uint8)((((local_offset - MKA_EIGHT) & (0x0FFFU)) >> 8) & 0x0F);
    b1 = (uint8)(((local_offset - MKA_EIGHT) & (0x0FFFU)) & 0xFF);

    ((uint8 *)MkaBasic_Length)[MKA_ZERO] = b0;
    ((uint8 *)MkaBasic_Length)[MKA_ONE] = b1;

    *offset_ptr = local_offset;
  }

  if (LddReturnValue) {
    /*DET Raised*/
  }
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Transmission **
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

#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

Std_ReturnType Mka_Transmission(uint8 PaeIdx, const uint8 *LpSduDataPtr,
                                uint16 LenBytes, uint16 bufferId,
                                const uint8 *addr)

{
  Std_ReturnType LddReturnValue;
  const uint8 *LpPhyAddressptr;
  Mka_PaeInstanceType const *Lp_PaeInstaceStatic;
  Lp_PaeInstaceStatic = &Mka_GaaPaeInstances[PaeIdx];
  LpPhyAddressptr = (uint8 *)&(Lp_PaeInstaceStatic->usMkaDstMacAddress);

  LddReturnValue = EthIf_Transmit(
      Lp_PaeInstaceStatic->ucMkaEthIfControllerIndex, bufferId,
      MKA_MACSEC_FRAME_TYPE,
      /*Where will this value come from*/ (boolean)1, LenBytes, addr);
  if (PaeIdx && LpSduDataPtr && LenBytes && LpPhyAddressptr) {
  }

  return LddReturnValue;
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_PrepareDistributedSAKList **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

void Mka_PrepareDistributedSAKList(Mka_PaeInstanceType const *LpMkaPaeInstance,
                                   uint8 Paeidx,uint8 kay ,uint8 *LpBuffPtr,
                                   uint8 *offset_ptr) {

  uint8 local_Offset;
  uint8 Luscount;

  Std_ReturnType LddReturnValue;
  uint8 *Lp_CipherTextPtr;

  uint8 *Lp_authenticator_ptr;
  uint32  Lciphertextlength;
  uint32 *Lp_CipherTextLengthPtr;
  uint32 *Lp_authenticatorLengthPtr;
  Mka_KayParticipants const *LpMkaKayParticipants;

  Mka_MacSecConfigType * Lp_MacsecConfigptr;
  LpMkaKayParticipants = &Mka_KayParticipant[kay];
  Lp_MacsecConfigptr = &Mka_GaaMacSecConfig[Paeidx];


  Lp_CipherTextPtr = &MKa_CipherText[0];
  Lp_authenticator_ptr = NULL_PTR;
  Lp_authenticatorLengthPtr = NULL_PTR;
/*Shall we check integrity and confidentiality here ? */
  if ((Lp_MacsecConfigptr->CurrentCipherSuite == GCM_AES_128) ||
      (Lp_MacsecConfigptr->CurrentCipherSuite == GCM_AES_XPN_128)) {
    Lciphertextlength = MKA_SIXTEEN;
  } else {
    Lciphertextlength = MKA_THIRTY_TWO;
  }
  Lp_CipherTextLengthPtr = &Lciphertextlength;

  local_Offset = *offset_ptr;

  LpBuffPtr[local_Offset++] = MKA_FOUR;
  LpBuffPtr[local_Offset++] = MKA_ZERO;
  LpBuffPtr[local_Offset++] = MKA_ZERO;
  if(Lp_MacsecConfigptr->CurrentCipherSuite != GCM_AES_128)
  {
      LpBuffPtr[local_Offset++] = (MKA_TWENTY_FOUR + MKA_FOUR + MKA_EIGHT);
  }
  else
  {
      LpBuffPtr[local_Offset++] = (MKA_TWENTY_FOUR + MKA_FOUR);
  }

  LpBuffPtr[local_Offset++] = MKA_ZERO;
  LpBuffPtr[local_Offset++] = MKA_ZERO;
  LpBuffPtr[local_Offset++] = MKA_ZERO;
  /*Rekeying is not supported */
  LpBuffPtr[local_Offset++] = MKA_ONE;

  if(Lp_MacsecConfigptr->CurrentCipherSuite != GCM_AES_128)
  {
      if(Lp_MacsecConfigptr->CurrentCipherSuite == GCM_AES_256 )
      {
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = 0x80U;
              LpBuffPtr[local_Offset++] = 0xC2;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ONE;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_TWO;
      }
      if(Lp_MacsecConfigptr->CurrentCipherSuite == GCM_AES_XPN_128 )
      {
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = 0x80U;
              LpBuffPtr[local_Offset++] = 0xC2;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ONE;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_THREE;
      }
      if(Lp_MacsecConfigptr->CurrentCipherSuite == GCM_AES_XPN_256 )
      {
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = 0x80U;
              LpBuffPtr[local_Offset++] = 0xC2;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ONE;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_FOUR;
      }
 }  

/*If GCM_AES_128 and XPN_256*/

if(LpMkaKayParticipants->usMkaCryptoWrapDeriveJobRefCsmProcessingType != MKA_CRYPTO_PROCESSING_SYNC)
{
  if(MKa_GaaKayStatus[kay] !=  MAK_INTR_WRAP_COMPLETE)
  {
  LddReturnValue = Mka_Callout_Wrap(
     LpMkaKayParticipants->usMkaCryptoKeyWrapJobRefCsmJobId,
     LpMkaKayParticipants
          ->usMkaCryptoSakDeriveJobRefCsmJobId,
      Lp_CipherTextPtr, Lp_CipherTextLengthPtr, Lp_authenticator_ptr,
      Lp_authenticatorLengthPtr);
      MKa_GaaKayStatus[kay]  = MKA_INTR_WAIT_WRAP;

  }
}
else
{

    LddReturnValue = Mka_Callout_Wrap(
     LpMkaKayParticipants->usMkaCryptoKeyWrapJobRefCsmJobId,
     LpMkaKayParticipants
          ->usMkaCryptoSakDeriveJobRefCsmJobId,
      Lp_CipherTextPtr, Lp_CipherTextLengthPtr, Lp_authenticator_ptr,
      Lp_authenticatorLengthPtr);
      MKa_GaaKayStatus[kay] =  MAK_INTR_WRAP_COMPLETE;
}

  if (!LddReturnValue) {
    for (Luscount = MKA_ZERO; Luscount < MKA_TWENTY_FOUR; Luscount++) {
      LpBuffPtr[local_Offset++] = Lp_CipherTextPtr[Luscount];
    }
  }

  if (LpMkaPaeInstance && Paeidx && LpBuffPtr && offset_ptr) {
  }
  *offset_ptr = local_Offset;
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_PrepareDistributedSAKList **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_PrepareUseParaList(Mka_PaeInstanceType const *LpMkaPaeInstance,
                            uint8 Paeidx, uint8 *LpBuffPtr, uint8 *offset_ptr) {
  uint8 local_Offset;
  uint8 Luscount;
  uint16 *Mka_thirdbyte;
  Mka_MacSecConfigType *Lp_MacsecConfigptr;
  local_Offset = *offset_ptr;
  Lp_MacsecConfigptr = &Mka_GaaMacSecConfig[Paeidx];

  if (LpMkaPaeInstance != NULL_PTR) {
    LpBuffPtr[local_Offset++] = MKA_THREE;

    LpBuffPtr[local_Offset++] = MKA_ZERO;
    Mka_thirdbyte = (uint16 *)&LpBuffPtr[local_Offset];
    LpBuffPtr[local_Offset++] = MKA_ZERO;
    LpBuffPtr[local_Offset++] = MKA_ZERO;

    for (Luscount = MKA_ZERO; Luscount < MKA_TWELEVE; Luscount++) {
      LpBuffPtr[local_Offset++] = Mka_StorageMI[Paeidx][MKA_ONE].MI[Luscount];
    }

    LpBuffPtr[local_Offset++] = MKA_ZERO;
    LpBuffPtr[local_Offset++] = MKA_ZERO;
    LpBuffPtr[local_Offset++] = MKA_ZERO;
    LpBuffPtr[local_Offset++] = MKA_ONE;

    /*Latest Lowest Acceptable PN */
    LpBuffPtr[local_Offset++] = MKA_ZERO;
    LpBuffPtr[local_Offset++] = MKA_ZERO;
    LpBuffPtr[local_Offset++] = MKA_ZERO;
    LpBuffPtr[local_Offset++] = MKA_ZERO;
    if ((Lp_MacsecConfigptr->CurrentCipherSuite == GCM_AES_XPN_128) ||
        (Lp_MacsecConfigptr->CurrentCipherSuite == GCM_AES_XPN_256)) {
      LpBuffPtr[local_Offset++] = MKA_ZERO;
      LpBuffPtr[local_Offset++] = MKA_ZERO;
      LpBuffPtr[local_Offset++] = MKA_ZERO;
      LpBuffPtr[local_Offset++] = MKA_ZERO;
    }
    /*If XPN Add 32 more*/

    for (Luscount = MKA_ZERO; Luscount < MKA_TWELEVE; Luscount++) {
      LpBuffPtr[local_Offset++] = Mka_StorageMI[Paeidx][MKA_ONE].MI[Luscount];
    }

    LpBuffPtr[local_Offset++] = MKA_ZERO;
    LpBuffPtr[local_Offset++] = MKA_ZERO;
    LpBuffPtr[local_Offset++] = MKA_ZERO;
    LpBuffPtr[local_Offset++] = MKA_ZERO;

    /*Latest Lowest Acceptable PN */
    LpBuffPtr[local_Offset++] = MKA_ZERO;
    LpBuffPtr[local_Offset++] = MKA_ZERO;
    LpBuffPtr[local_Offset++] = MKA_ZERO;
    LpBuffPtr[local_Offset++] = MKA_ZERO;
    /*If XPN Add 32 more*/
    if ((Lp_MacsecConfigptr->CurrentCipherSuite == GCM_AES_XPN_128) ||
        (Lp_MacsecConfigptr->CurrentCipherSuite == GCM_AES_XPN_256)) {
      LpBuffPtr[local_Offset++] = MKA_ZERO;
      LpBuffPtr[local_Offset++] = MKA_ZERO;
      LpBuffPtr[local_Offset++] = MKA_ZERO;
      LpBuffPtr[local_Offset++] = MKA_ZERO;
    }

    ((uint8 *)(Mka_thirdbyte))[MKA_ZERO] = MKA_ZERO;
    if ((Lp_MacsecConfigptr->CurrentCipherSuite == GCM_AES_XPN_128) ||
        (Lp_MacsecConfigptr->CurrentCipherSuite == GCM_AES_XPN_256)) {
      ((uint8 *)(Mka_thirdbyte))[MKA_ONE] =
          (MKA_TWENTY_FOUR + MKA_FOUR + MKA_EIGHT);
    } else {
      ((uint8 *)(Mka_thirdbyte))[MKA_ONE] = (MKA_TWENTY_FOUR + MKA_FOUR);
    }
  }

  *offset_ptr = local_Offset;
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_CreateICV                                     **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_CreateICV(uint8 paeindx, uint8 Kay,uint16 *LusTotalLen, uint8 *LpBuffptr,
               uint8 *offset_ptr) {

  uint8 *Mka_Icv_ptr;
  uint8 Mka_ICV_Value[16];
  uint8 Local_offset;
  uint32 Mka_ICVLength;
  Mka_PaeInstanceType const *Lp_PaeInstaceStatic;
  Mka_KayParticipants const *LpMkaKayParticipants;
  Std_ReturnType LddReturnValue;
  Mka_Icv_ptr = Mka_ICV_Value;
  Mka_ICVLength = MKA_ICV_LENGTH;
  Lp_PaeInstaceStatic = &Mka_GaaPaeInstances[paeindx];
  LpMkaKayParticipants = &Mka_KayParticipant[Kay];

  Local_offset = *offset_ptr;
  LpBuffptr[Local_offset++] = MKA_TWO_FIFTY_FIVE;
  LpBuffptr[Local_offset++] = MKA_ZERO;
  LpBuffptr[Local_offset++] = MKA_ZERO;
  LpBuffptr[Local_offset++] = MKA_SIXTEEN;


 { 

  if(LpMkaKayParticipants->usMkaCryptoIcvDeriveJobRefCsmProcessingType != MKA_CRYPTO_PROCESSING_SYNC )

  {
  if(  MKa_GaaKayStatus[Kay] != MKA_INTR_ICV_GEN_COMPLETE )
  {
  LddReturnValue = Mka_ICV_Generation_Callout(
      paeindx,
     LpMkaKayParticipants
          ->usMkaCryptoIcvGenerateJobRefCsmJobId,
      Lp_PaeInstaceStatic->pMkaKayParticipants->usMkaCryptoICKKeyRefCsmKeyId,
      LpBuffptr, Local_offset, Mka_Icv_ptr, &Mka_ICVLength);

      MKa_GaaKayStatus[Kay]  = MKA_INTR_WAIT_FOR_ICV;
  }
  else
  {
    /*Do nothing*/
  }
  }
  else
  {
      LddReturnValue = Mka_ICV_Generation_Callout(
      paeindx,
     LpMkaKayParticipants
          ->usMkaCryptoIcvGenerateJobRefCsmJobId,
      Lp_PaeInstaceStatic->pMkaKayParticipants->usMkaCryptoICKKeyRefCsmKeyId,
      LpBuffptr, Local_offset, Mka_Icv_ptr, &Mka_ICVLength);
  }
  

    
 }

  if (!LddReturnValue) {
    memcpy(&LpBuffptr[Local_offset], Mka_Icv_ptr, Mka_ICVLength);
    Local_offset += Mka_ICVLength;
  } else {

    /*No Update */
  }

  ((uint8 *)(LusTotalLen))[MKA_ZERO] = (Local_offset - MKA_FOUR);
  ((uint8 *)(LusTotalLen))[MKA_ONE] = MKA_ZERO;

  *offset_ptr = Local_offset;
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_CreateICV                                     **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

void Mka_ProcessLivePeerList(uint8 PaeIdx, const uint8 *LpSduDataPtr,
                             uint16 LenBytes, uint8 *offset_main)

{
  uint8 *offset_;
  uint8 off;
  uint8 Luscount;
  uint16 MKA_FLAG_and_length;
  uint16 length;
  off = MKA_ZERO;
  MKA_FLAG_and_length = MKA_ZERO;
  offset_ = &off;

  Mka_Intr_be7bit(LpSduDataPtr, &Mka_LivePeerList[PaeIdx].mka_parameterSetType,
                  &LenBytes, offset_main);
  Mka_Intr_be7bit(LpSduDataPtr, &Mka_LivePeerList[PaeIdx].Mka_keySSCI,
                  &LenBytes, offset_main);
  Mka_Intr_be16(LpSduDataPtr,
                &Mka_LivePeerList[PaeIdx].Mka_parameterBodySetlength, &LenBytes,
                offset_main);
  for (Luscount = MKA_ZERO; Luscount < MKA_TWELEVE; Luscount++) {
    Mka_Intr_be7bit(LpSduDataPtr,
                    &Mka_LivePeerList[PaeIdx].Mka_LivePotentialMId[Luscount],
                    &LenBytes, offset_main);
  }

  for (Luscount = MKA_ZERO; Luscount < MKA_FOUR; Luscount++) {
    Mka_Intr_be7bit(LpSduDataPtr,
                    &Mka_LivePeerList[PaeIdx].Mka_LivePotentialMNum[Luscount],
                    &LenBytes, offset_main);
  }

  if (offset_ && Luscount) {
  }

  /*
  typedef struct STag_Mka_StatusFlag_Type {
    uint8 ddkeyserver : 1;
    uint8 ddmacdesired : 1;
    uint8 ddmacsecCapability : 2;
    uint16 ddmacparameterser : 12;

  } Mka_StatusFlag_Type;

  typedef struct STag_Mka_BasicParameterSet_Type {
    uint8 Mka_VersionIdentifier;
    uint8 Mka_KeyServerPriority;
    Mka_StatusFlag_Type Mka_flag;
    uint64 Mka_Sci;
    uint32 Mka_AcMi[3];
    uint32 Mka_AMn;
    uint32 Mka_AlgoAgility;
    uint16 Mka_Ckn[16];
  } Mka_BasicParameterSet_Type;
  */

  if (PaeIdx && LpSduDataPtr && LenBytes && MKA_FLAG_and_length) {
  } else {
  }
  // *offset_main +=
  // (MKA_FOUR+Mka_PotentialPeerList[PaeIdx].Mka_parameterBodySetlength);

  if (offset_main && length) {
  }
  /*ICV Generation */

  /*ICV Generation for each KAY Participant and Verification on Peers end*/
  /*
     Create Potential peer list and live peer list from place holder
  */
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_CreateICV                                     **
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

#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

boolean Mka_CheckforConnectivityAssociation(uint8 paeindi,uint8 *Mka_Capability) {
  boolean Mka_Flag;
  uint8 MkaCapabilityVal;
  Mka_PaeInstanceType const *Lp_PaeInstaceStatic;
  Lp_PaeInstaceStatic = &Mka_GaaPaeInstances[paeindi];

  Mka_Flag = MKA_FALSE;
  if ((Mka_GaaCryptoAlgoConfig[Lp_PaeInstaceStatic->pMkaKayParticipants
                                   ->ucMkaCryptoAlgoRefConfigIdx]
           .ddMkaMacSecCapability) == INTEGRITY_WITHOUT_CONFIDENTIALITY) {
    MkaCapabilityVal = MKA_ONE;
  }

  else if (((Mka_GaaCryptoAlgoConfig[Lp_PaeInstaceStatic->pMkaKayParticipants
                                         ->ucMkaCryptoAlgoRefConfigIdx]
                 .ddMkaMacSecCapability) == INTEGRITY_AND_CONFIDENTIALITY) &&
           ((Mka_GaaCryptoAlgoConfig[Lp_PaeInstaceStatic->pMkaKayParticipants
                                         ->ucMkaCryptoAlgoRefConfigIdx]
                 .ddMkaMacSecConfidentialityOffset) ==
            CONFIDENTIALITY_OFFSET_0)) {
    MkaCapabilityVal = MKA_TWO;
  } else if (((Mka_GaaCryptoAlgoConfig[Lp_PaeInstaceStatic->pMkaKayParticipants
                                           ->ucMkaCryptoAlgoRefConfigIdx]
                   .ddMkaMacSecCapability) == INTEGRITY_AND_CONFIDENTIALITY) &&
             (((Mka_GaaCryptoAlgoConfig[Lp_PaeInstaceStatic->pMkaKayParticipants
                                            ->ucMkaCryptoAlgoRefConfigIdx]
                    .ddMkaMacSecConfidentialityOffset) ==
               CONFIDENTIALITY_OFFSET_30) ||
              ((Mka_GaaCryptoAlgoConfig[Lp_PaeInstaceStatic->pMkaKayParticipants
                                            ->ucMkaCryptoAlgoRefConfigIdx]
                    .ddMkaMacSecConfidentialityOffset) ==
               CONFIDENTIALITY_OFFSET_50))) {
    MkaCapabilityVal = MKA_THREE;
  } else {
    MkaCapabilityVal = MKA_ZERO;
  }

  if (MkaCapabilityVal ==
      Mka_GaaBasicparameterset[paeindi].Mka_flag.ddmacsecCapability)

  {
    Mka_Flag = MKA_TRUE;
  }

  else {

    {
      Mka_Flag = MKA_FALSE;
    }
  }
*Mka_Capability =  MkaCapabilityVal;
  return Mka_Flag;
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_ProcessICVIndicatior                           **
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

#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_ProcessICVIndicatior(uint8 PaeIdx, const uint8 *LpSduDataPtr,
                              uint16 LenByte, uint8 *Offset_Ptr) {
  uint8 Luscount = MKA_ZERO;
  uint8 Type;
  uint8 null;
  uint16 length;

  Mka_Intr_be7bit(LpSduDataPtr, &Type, &LenByte, Offset_Ptr);
  Mka_Intr_be7bit(LpSduDataPtr, &null, &LenByte, Offset_Ptr);
  Mka_Intr_be16(LpSduDataPtr, &length, &LenByte, Offset_Ptr);

  for (Luscount = MKA_ZERO; Luscount < MKA_ICV_LENGTH; Luscount++) {
    Mka_Intr_be7bit(LpSduDataPtr, &Mka_GaaICV[PaeIdx][Luscount], &LenByte,
                    Offset_Ptr);
  }
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_CipherSuiteSelection                           **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_CipherSuiteSelection() {

  /*Currently  Default Ciphersuite  AES_GCM_128 is supported as MKA capability
   * is set to Integrity without Confidentialty.*/
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_ProcessDistributedParamter **
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

#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_ProcessDistributedParamter(uint8 PaeIdx, const uint8 *LpSduDataPtr,
                                    uint16 LenBytes, uint8 *offset_main) {

  uint8 Luscount;
  uint8 MKA_FLAG_and_length;
  MKA_FLAG_and_length = MKA_ZERO;

  Mka_Intr_be7bit(LpSduDataPtr,
                  &Mka_DistributedSak[PaeIdx].Mka_ParameterSetType, &LenBytes,
                  offset_main);
  Mka_Intr_be7bit(LpSduDataPtr, &MKA_FLAG_and_length, &LenBytes, offset_main);
  Mka_Intr_be16(LpSduDataPtr,
                &Mka_DistributedSak[PaeIdx].parametersetBodylength, &LenBytes,
                offset_main);
  Mka_Intr_be32(LpSduDataPtr, &Mka_DistributedSak[PaeIdx].Mka_Keynumber,
                &LenBytes, offset_main);
  for (Luscount = MKA_ZERO; Luscount < MKA_TWENTY_FOUR; Luscount++) {
    Mka_Intr_be7bit(LpSduDataPtr,
                    &Mka_DistributedSak[PaeIdx].Mka_AES_KEY[Luscount],
                    &LenBytes, offset_main);
  }
  Mka_DistributedSak[PaeIdx].Mka_Distributed_An =
      (MKA_FLAG_and_length > MKA_SIX) & MKA_FIFTEEN;
  Mka_DistributedSak[PaeIdx].Mka_ConfidentialityOffset =
      (MKA_FLAG_and_length > MKA_FOUR) & MKA_THREE;

  /*Distributed SAK */
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_ProcessUseParamterSet                           **
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

#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_ProcessUseParamterSet(uint8 PaeIdx, const uint8 *LpSduDataPtr,
                               uint16 LenBytes, uint8 *offset_main) {

  uint8 Luscount;
  uint8 Mka_2nd_Byte;
  uint8 Mka_3rd_Byte;
  uint8 Mka_Length_2nd_half;

  Mka_Intr_be7bit(LpSduDataPtr,
                  &Mka_UseParameterSetSak[PaeIdx].Mka_ParamterSetType,
                  &LenBytes, offset_main);
  Mka_Intr_be7bit(LpSduDataPtr, &Mka_2nd_Byte, &LenBytes, offset_main);
  Mka_Intr_be7bit(LpSduDataPtr, &Mka_3rd_Byte, &LenBytes, offset_main);
  Mka_Intr_be7bit(LpSduDataPtr, &Mka_Length_2nd_half, &LenBytes, offset_main);
  for (Luscount = MKA_ZERO; Luscount < MKA_TWELEVE; Luscount++) {
    Mka_Intr_be7bit(LpSduDataPtr,
                    &Mka_UseParameterSetSak[PaeIdx]
                         .Mka_MI_Latest_key_Server_Member[Luscount],
                    &LenBytes, offset_main);
  }
  Mka_Intr_be32(LpSduDataPtr,
                &Mka_UseParameterSetSak[PaeIdx].Mka_Latest_Key_Number,
                &LenBytes, offset_main);
  Mka_Intr_be32(
      LpSduDataPtr,
      &Mka_UseParameterSetSak[PaeIdx].Mka_Latest_Key_Lowest_Acceptable_PN,
      &LenBytes, offset_main);

  for (Luscount = MKA_ZERO; Luscount < MKA_TWELEVE; Luscount++) {
    Mka_Intr_be7bit(
        LpSduDataPtr,
        &Mka_UseParameterSetSak[PaeIdx].Mka_MI_Old_key_Server_Member[Luscount],
        &LenBytes, offset_main);
  }

  Mka_Intr_be32(LpSduDataPtr,
                &Mka_UseParameterSetSak[PaeIdx].Mka_Old_Key_Key_number,
                &LenBytes, offset_main);
  Mka_Intr_be32(
      LpSduDataPtr,
      &Mka_UseParameterSetSak[PaeIdx].Mka_Old_Key_Lowest_Acceptable_PN,
      &LenBytes, offset_main);

  Mka_UseParameterSetSak[PaeIdx].Mka_Latest_Key_AN = (Mka_2nd_Byte >> MKA_SIX) & MKA_THREE;
  Mka_UseParameterSetSak[PaeIdx].Mka_Latest_Key_Tx = (Mka_2nd_Byte >> MKA_FIVE) & MKA_ONE;
  Mka_UseParameterSetSak[PaeIdx].Mka_Latest_Key_Rx = (Mka_2nd_Byte >> MKA_THREE) & MKA_ONE;
  Mka_UseParameterSetSak[PaeIdx].Mka_OLD_Key_AN = (Mka_2nd_Byte >> MKA_TWO) & MKA_THREE;
  Mka_UseParameterSetSak[PaeIdx].Mka_OLD_Key_Tx = (Mka_2nd_Byte >> MKA_ONE) & MKA_ONE;
  Mka_UseParameterSetSak[PaeIdx].Mka_OLD_Key_Rx = (Mka_2nd_Byte >> MKA_ONE) & MKA_ONE;
  Mka_UseParameterSetSak[PaeIdx].Mka_PlainText = (Mka_3rd_Byte >> MKA_ONE) & MKA_ONE;
  Mka_UseParameterSetSak[PaeIdx].Mka_Plainrx = (Mka_3rd_Byte >> MKA_SIX) & MKA_ONE;
  Mka_UseParameterSetSak[PaeIdx].reserved = (Mka_3rd_Byte >> MKA_FIVE) & MKA_ONE;
  Mka_UseParameterSetSak[PaeIdx].Mka_Delay_Protect = (Mka_3rd_Byte >> MKA_FOUR) & MKA_ONE;
  Mka_UseParameterSetSak[PaeIdx].ParamterSetBodyLength =
      (uint16)(((Mka_3rd_Byte) & MKA_FIFTEEN) | Mka_Length_2nd_half);

  /*Distributed SAK */
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"


/* parasoft-end-suppress MISRAC2012-RULE_20_1-a-4 */
/* parasoft-end-suppress MISRAC2012-RULE_1_1-c-2 */
/* parasoft-end-suppress CERT_C-DCL19-a-3 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 MISRAC2012-RULE_5_4-d-2 */
/* parasoft-end-suppress CERT_C-DCL00-b-3 */
/*******************************************************************************
** Function Name        : Mka_ProcessUseParamterSet                           **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_MainHandler(uint8 PaeInst ){



    switch (Mka_GaaPaeServerStatus[PaeInst].ddpaetatus)
    {
        case MKA_STATUS_WAITING_PEER_LINK:
       {
         Mka_HandleInternalStateWaitingPeerLink(PaeInst);
  
        /* code */
         break;
       }
        case MKA_STATUS_WAITING_PEER:
       {
         Mka_HandleInternalStateWaitingPeer(PaeInst);
        
        /* code */
         break;
       }
       case MKA_STATUS_IN_PROGRESS:
       {
         Mka_HandleInternalStateInProgress(PaeInst);
        
        /* code */
        break;
       }
      
      case MKA_STATUS_AUTH_FAIL_UNKNOWN_PEER:
      {
        Mka_HandleInternalStateAuthFailUnknownPeer(PaeInst);
        
        /* code */
        break;
      }
      
        case MKA_STATUS_MACSEC_RUNNING:
      {
        Mka_HandleInternalStateMacsecRunning(PaeInst);
        
        /* code */
        break;
      }
   default:
      break;
    }






}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_HandleStateWaitingPeerLink                           **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_HandleInternalStateWaitingPeerLink(uint8 PaeInst)
{
  Std_ReturnType LddRetunrValue;
  Mka_PaeInstanceType const *LpMkaPaeInstance;
  LpMkaPaeInstance = &Mka_GaaPaeInstances[PaeInst];
  LddRetunrValue = E_NOT_OK;

if(!Mka_CAK_AVAILABLE)
{
  LddRetunrValue = Mka_CheckCAKAvailability(PaeInst);
}
else
{
	LddRetunrValue = E_OK;

}

  
 if(LddRetunrValue == E_OK)
 {
     Mka_CAK_AVAILABLE = MKA_TRUE;
    if (LpMkaPaeInstance->ddMkaRole == MKA_KEY_SERVER) {
      Mka_ServerStateMachine(PaeInst);
    }

    if (LpMkaPaeInstance->ddMkaRole == MKA_PEER) {
      Mka_ClientStateMachine( PaeInst);
      /*#endif*/
    }
  }
  /*
   if(LinkUp)
   
  
  */
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_HandleStateWaitingPeerLink                           **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_HandleInternalStateWaitingPeer(uint8 PaeInst)
{

  Std_ReturnType LddRetunrValue;
  uint8 Luscount;
  Mka_KayParticipants const *LpMkaKayParticipants;
  const Mka_PaeInstanceType * Lp_PaeInstanceStatic;
  Lp_PaeInstanceStatic = &Mka_GaaPaeInstances[PaeInst];

    for(Luscount = Lp_PaeInstanceStatic->ucindexofKayParticipant; Luscount < Lp_PaeInstanceStatic->ucnoofKayParticipant; Luscount++ )
  {  

  
   if(E_OK == LddRetunrValue)
   {

   }

   switch (MKa_GaaKayStatus[Luscount])
   {

    case MKA_INTR_IN_PROGRESS:
    {
      Mka_RandomNumberGeneration(Luscount,PaeInst);
       break;
    }
    case MKA_INTR_WAIT_FOR_RANDOM:
    {
      /*Do nothing*/
      break;
    }
    case MKA_INTR_RANDOM_COMPLETE:
    {
      Mka_BuildMkPduWithoutICV(Luscount,PaeInst);
      break;
    }
    case MKA_INTR_WAIT_FOR_ICV:
    {
      /*Do nothing*/
      break;
    } 
    case MKA_INTR_ICV_GEN_COMPLETE:
    {
      LpMkaKayParticipants = &Mka_KayParticipant[Luscount];
      if(LpMkaKayParticipants->usMkaCryptoIcvDeriveJobRefCsmProcessingType != MKA_CRYPTO_PROCESSING_SYNC )
    {
      memcpy(&Mka_Pdu[Luscount].MkPdu[MKA_ZERO], &Mka_IcvGen[Luscount][MKA_ZERO], MKA_ICV_LENGTH);
      Mka_Pdu[Luscount].length += MKA_ICV_LENGTH;
      Mka_Pdu[Luscount].MkPdu[MKA_TWO] = (Mka_Pdu[Luscount].length - MKA_FOUR);
      Mka_Pdu[Luscount].MkPdu[MKA_THREE] = MKA_ZERO;
      MKa_GaaKayStatus[Luscount] = MKA_INTR_NEGO_IN_PROGRESS;

    }
    else
    {
      MKa_GaaKayStatus[Luscount] = MKA_INTR_NEGO_IN_PROGRESS;
    }

      break;
    } 

    case MKA_INTR_NEGO_IN_PROGRESS:
    {
       Mka_StartNegotiation( PaeInst);

    }
    
    default:
    {
      break;
    }
   /* case MKA_INTRA_STATE_TX_ADV_FRAME_SENT:
   {
    Mka_InternalAdvframesent( PaeInst);
  
    break;
   }    
    case MKA_INTR_STATE_TX_ADV_FRAME_COMPLETED:
   {
     
    break;
   }
   case MKA_INTR_STATE_RX_ADV_FRAME_RECEIVED:
   {
  
    break;
   }

   case MKA_INTR_STATE_RX_ADV_FRAME_VALIDATED:
   {
  
    break;
   }
   case  MKA_INTR_STATE_KAY_PART_SELECTION_COMPLETED:
   {
    Mka_InternalKayPartSel(PaeInst);
  
    break;
   }
      case  MKA_INTR_STATE_PP_FRAME_SENT:
   {
    Mka_Internalppstatesent(PaeInst);
    break;
   }
    case  MKA_INTR_STATE_LP_FRAME_RECEIVED:
   {
  
    break;
   }
   case  MKA_INTR_STATE_LP_FRAME_VALIDATED:
   {
  
    break;
   }
   case  MKA_INTR_STATE_SAK_WAITING:
   {
    
    break;
   }
   case  MKA_INTR_STATE_SAK_COMPLETED:
   {
    Mka_InternalSakCompleted( PaeInst);
    break;
   }
   case MKA_INTR_STATE_RANDOM_NUMBER_GENERATION_IN_PROGRESS:
   {

      
    break;
   }
    case MKA_INTR_STATE_RANDOM_NUMBER_GENERATION_COMPLETED:
   { 


    break;
   }
   default:
    break;*/
   }
  }
  /*
   if(LinkUp)
  MKA_INTR_STATE_LP_FRAME_VALIDATED,
  MKA_INTR_STATE_SAK_WAITING,
  MKA_INTR_STATE_SAK_COMPLETED
  
  */
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_HandleStateInProgress                           **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_HandleInternalStateInProgress(uint8 PaeInst)
{
  uint8 Luscount;
  Mka_PaeInstanceType const *LpMkaPaeInstance;
  
  LpMkaPaeInstance = &Mka_GaaPaeInstances[PaeInst];
   for(Luscount = LpMkaPaeInstance->ucindexofKayParticipant; Luscount < LpMkaPaeInstance->ucnoofKayParticipant; Luscount++ )
{
  switch (MKa_GaaKayStatus[Luscount])
  {
  case MKA_INTR_WAIT_FOR_BP_SET:
    /* code */
 {
    break;
  }
  case MKA_INTR_WAIT_FOR_BP_RECEIVED:

  {

    Mka_Paramtersethandler(PaeInst,Luscount);
       
    break;
  }
  case MKA_INTR_WAIT_FOR_ICV:
  {
    /*Do nothing*/
    break;
  }
  case MKA_INTR_ICV_GEN_COMPLETE:
  {

        if(Frame_list_type[PaeInst] == MKA_BASIC_FRAME_TYPE )
    {  
         MKa_GaaKayStatus[Luscount] = MKA_INTR_PDU_CREATE;
    }
    else if(Frame_list_type[PaeInst] == MKA_POTENTIAL_TYPE )
    {
        Mka_SakGenerationHandler(PaeInst,Luscount);

    }
   
    
    
    // Mka_Paramtersethandler(PaeInst,Luscount);
    break;
  }

  case MKA_INTR_CKN_CHECK:
 { 
   Mka_CknCheckHandler(PaeInst,Luscount);
   break;
 }
 case MKA_INTR_ICV_VERIFY:
  
  {
    Mka_IcvVerifyHandler(PaeInst,Luscount);
     break;
  }

  case MKA_INTR_FRAME_CHECK:
  {
    Mka_FrameCheck(PaeInst,Luscount);
    break;
  }

  case MKA_INTR_POTENTIAL:
  {

    Mka_Potentialhandler(PaeInst,Luscount);
    break;
  }

  case MKA_INTR_LIVE:
  {

  /*  Mka_LiveHandler(PaeInst,Luscount);*/
    break;

  }

 case MKA_INTR_USE_PARA:
  {

   // Mka_UseHandler(PaeInst,Luscount);
    break;
  }

  case MKA_INTR_DISTRIBUTED:
  {

  //  Mka_DistributedHandler(PaeInst,Luscount);
    break;
  }
  case MKA_INTR_FRAME_BASIC:
  {
    Mka_BasicHandler(PaeInst,Luscount);
    break;
  }
    case MKA_INTR_WAIT_FOR_RANDOM:
  {
    /*Do Nothing*/
    break;
  }
      case MKA_INTR_RANDOM_COMPLETE:
  {


      Mka_Icvhandler(Luscount,PaeInst);
    

    
    break;
  }
  case MKA_INTR_PDU_CREATE:
  {

    Mka_Paramtersethandler(PaeInst,Luscount);
    break;
  }

 default:
  {
    break;
  }
  }


}
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_HandleStateWaitingPeerLink                           **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_HandleInternalStateAuthFailUnknownPeer(uint8 PaeInst)
{

if(PaeInst)
{


}
  /*
   if(LinkUp)
   
  
  */
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_HandleStateWaitingPeerLink                           **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_HandleInternalStateMacsecRunning(uint8 PaeInst)
{


switch (MKa_GaaKayStatus[ Mka_Selectedparticipant.index])
{
case MKA_INTR_KAY_RUNNIG:
{
  if(PaeInst)
  {
    
  }
  break;
}

default:
  break;
}
  /*
   if(LinkUp)
   
  
  */
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_HandleStateWaitingPeerLink                           **
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
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
Std_ReturnType Mka_CheckCAKAvailability(uint8 PaeInst){

uint8 LusCount;
Std_ReturnType LddReturnValue;
Mka_PaeInstanceType const *LpMkaPaeInstance;
Mka_KayParticipants const *LpMkaKayParticipants;
LpMkaPaeInstance = &Mka_GaaPaeInstances[PaeInst];
LpMkaPaeInstance = &Mka_GaaPaeInstances[PaeInst];
LddReturnValue = E_NOT_OK;


LusCount = MKA_ZERO;




     /*If that returns True == E_OK
       Call for these state machines for Generating KEYS
       else
       DET()
        */
      
       if( (Mka_GaaPaeServerStatus[PaeInst].ddServerInternalState == MKA_INTR_STATE_MKA_INITIALIZED ) 
       || (Mka_GaaPaeServerStatus[PaeInst].ddServerInternalState == MKA_INTR_STATE_KEK_COMPLETED ))
      {
        for (LusCount = LpMkaPaeInstance->ucindexofKayParticipant;
             LusCount < LpMkaPaeInstance->ucnoofKayParticipant; LusCount++) 
        {
          LpMkaKayParticipants = &Mka_KayParticipant[LusCount];
          LddReturnValue = Mka_CAK_Available_Callout(
              (LpMkaKayParticipants)->usMkaCryptoCknCakKeyRefCsmKeyId);
        }
      }
      if(E_OK == LddReturnValue)
      {
      /* Raise DET*/    
      }
  

return LddReturnValue;



}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_ICKCallhandler                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
Std_ReturnType Mka_ICKCallhandler(uint8 KayId,uint8 paeid) {
  Std_ReturnType LddReturnValue;
  Mka_KayParticipants const *LpMkaKayParticipants;
  LddReturnValue = E_NOT_OK;

    LpMkaKayParticipants = &Mka_KayParticipant[KayId];

  
      if ((LpMkaKayParticipants)->blMkaParticipantActivate == STD_ON) {
        LddReturnValue = Mka_Callout_ICK_KDF(
            (LpMkaKayParticipants)->usMkaCryptoIckDeriveJobRefCsmJobId,
            (LpMkaKayParticipants)->usMkaCryptoCknCakKeyRefCsmKeyId,
            (LpMkaKayParticipants)->usMkaCryptoICKKeyRefCsmKeyId, MKA_KEK_LABEL,
            (LpMkaKayParticipants)->Mka_Ckn, MKA_CALLOUT_KEY_LENGTH);
        if (LpMkaKayParticipants->usMkaCryptoIckDeriveJobRefCsmProcessingType !=
            MKA_CRYPTO_PROCESSING_SYNC) {
          Mka_JobRegistry[KayId]
              .JobId =
              (LpMkaKayParticipants)->usMkaCryptoIckDeriveJobRefCsmJobId;
          Mka_JobRegistry[KayId]
              .PaeId = paeid;
          Mka_JobRegistry[KayId]
              .KayIndex = KayId;
              Mka_JobRegistry->status = MKA_TRUE;
              Mka_JobRegistry->JobCounter++;
              Mka_JobRegistyCounter[KayId]++;
              MKa_GaaKayStatus[KayId] = MKA_INTR_WAIT_FOR_ICK;
            
        }
        else
        {

           MKa_GaaKayStatus[KayId] = MKA_INTR_ICK_COMPLETE;
        }
      }
    
  

  if(LpMkaKayParticipants->usMkaCryptoIckDeriveJobRefCsmProcessingType  != CRYPTO_PROCESSING_SYNC)
  {
   /* Mka_GaaPaeServerStatus[paeid].ddServerInternalState =
      MKA_INTR_STATE_ICK_COMPLETED;*/
  }
  else
  {
  
   /* Mka_GaaPaeServerStatus[paeid].ddServerInternalState =
      MKA_INTR_STATE_ICK_WAITING;*/
  }

     return LddReturnValue;
  
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_KEKCallhandler                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
Std_ReturnType Mka_KEKCallhandler(uint8 KayIndex,uint8 paeid) {
  Std_ReturnType LddReturnValue;

  Mka_KayParticipants const *LpMkaKayParticipants;
  LddReturnValue = E_NOT_OK;

    LpMkaKayParticipants = &Mka_KayParticipant[KayIndex];

 
      if ((LpMkaKayParticipants)->blMkaParticipantActivate == MKA_TRUE) {
        LddReturnValue = Mka_Callout_KEK_KDF(
            (LpMkaKayParticipants)->usMkaCryptoKekDeriveJobRefCsmJobId,
            (LpMkaKayParticipants)->usMkaCryptoCknCakKeyRefCsmKeyId,
            (LpMkaKayParticipants)->usMkaCryptoKekKeyRefCsmKeyId, MKA_KEK_LABEL,
            (LpMkaKayParticipants)->Mka_Ckn, MKA_CALLOUT_KEY_LENGTH);

       if (LpMkaKayParticipants->usMkaCryptoKekDeriveJobRefCsmProcessingType !=
            MKA_CRYPTO_PROCESSING_SYNC) {
          Mka_JobRegistry[KayIndex]
              .JobId =
              (LpMkaKayParticipants)->usMkaCryptoKekDeriveJobRefCsmJobId;
          Mka_JobRegistry[KayIndex]
              .PaeId = paeid;
          Mka_JobRegistry[KayIndex]
              .KayIndex = KayIndex;
              Mka_JobRegistry->status = MKA_TRUE;
              Mka_JobRegistry->JobCounter++;
              Mka_JobRegistyCounter[paeid]++;
                MKa_GaaKayStatus[KayIndex] = MKA_INTR_WAIT_FOR_KEK;
            
        }
        else
        {
           MKa_GaaKayStatus[KayIndex] = MKA_INTR_KEK_COMPLETE;
        }
      }
    
  

   if(LpMkaKayParticipants->usMkaCryptoKekDeriveJobRefCsmProcessingType  != MKA_CRYPTO_PROCESSING_SYNC)
  {
 // Mka_GaaPaeServerStatus[paeid].ddServerInternalState =
 //     MKA_INTR_STATE_KEK_COMPLETED;
  }
  else
  {
  
  //  Mka_GaaPaeServerStatus[paeid].ddServerInternalState =
  //    MKA_INTR_STATE_KEK_WAITING;
  }


  return LddReturnValue;
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_KEKCallhandler                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
Std_ReturnType Mka_GetLinkUpsate(uint8 paeid,boolean *LinkUpstatus)
{



   *LinkUpstatus = Mka_InternalLinkUpState[paeid];
     
    
  /*No error Identified in this function Paeid is already validated in Mka_LinkstateUp*/
  return E_OK;
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_KEKCallhandler                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_ProcessMacSecTLV(uint8 PaeIdx,const uint8 *LpSduDataPtr,uint16 LenByte, uint8 *Offset_Ptr)
{
  uint8 Luscount;
  
      Mka_Intr_be7bit(LpSduDataPtr,
                  &Mka_MacsecTLV[PaeIdx].Mka_TLV_FType, &LenByte,
                  Offset_Ptr);
      Mka_Intr_be7bit(LpSduDataPtr,
                  &Mka_MacsecTLV[PaeIdx].Mka_Tlv_String_len, &LenByte,
                  Offset_Ptr);
      for(Luscount = MKA_ZERO; Luscount  < Mka_MacsecTLV[PaeIdx].Mka_Tlv_String_len;Luscount++ )
      {
        Mka_Intr_be7bit(LpSduDataPtr,
                  &Mka_MacsecTLV[PaeIdx].Mka_Infor_String[Luscount], &LenByte,
                  Offset_Ptr);
      }
       

}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_KEKCallhandler                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
boolean Mka_SelectCiphersuites(uint8 paeid,uint8 * Selected_Cipher, Mka_KayParticipants const *LpMkaKayParticipants)
{

  uint8 luscount;
  uint8 luscount2;
  uint8 luscount3;
  uint8 Select_Bit;
  boolean LbCipherfourd;
  uint8 selection;
  boolean blCipherSelec;
  boolean blprio;
  Mka_MacSecConfigType *LpMacsecConfig;
  blCipherSelec = MKA_FALSE;
  blprio = MKA_FALSE;
  LpMacsecConfig = &Mka_GaaMacSecConfig[paeid];
  Select_Bit = MKA_ZERO;
  LbCipherfourd = MKA_FALSE;

  if (paeid) {
  }
  for (luscount = MKA_ONE; (luscount <= MKA_FOUR) && (!blCipherSelec);
       luscount++) {
    for (luscount2 = MKA_ZERO;
         (luscount2 < Mka_GaaCryptoAlgoConfig[LpMkaKayParticipants
                                                  ->ucMkaCryptoAlgoRefConfigIdx]
                          .ucnoofciphersuites) &
         !blprio;
         luscount2++) {
      if (Mka_GaaCryptoAlgoConfig[LpMkaKayParticipants
                                      ->ucMkaCryptoAlgoRefConfigIdx]
              .pMkaCipherSuites[luscount2]
              .ucMkaMacSecCipherSuitePrio == luscount)

      {
        blprio = MKA_TRUE;
        selection = Mka_GaaCryptoAlgoConfig[LpMkaKayParticipants
                                                ->ucMkaCryptoAlgoRefConfigIdx]
                        .pMkaCipherSuites[luscount2]
                        .ddMkaMacSecCipherSuite;
        for (luscount3 = MKA_ZERO;
             (luscount3 < (Mka_MacsecTLV[paeid].Mka_Tlv_String_len) / MKA_TEN);
             luscount3++) {

          if (Mka_MacsecTLV[paeid].Mka_Infor_String[MKA_NINE + Select_Bit] ==
              selection) {
            blCipherSelec = MKA_TRUE;
            LpMacsecConfig->CurrentCipherSuite = selection;
            *Selected_Cipher = selection;
            LbCipherfourd = MKA_TRUE;
          }
          Select_Bit = Select_Bit + MKA_TEN;
        }
      }
    }
  }
  return LbCipherfourd;
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_KEKCallhandler                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_PrepareMacsecTLVPeerList(Mka_PaeInstanceType const *LpMkaPaeInstance,Mka_KayParticipants const *LpMkaKayParticipants,
                                  uint8 Paeidx, uint8 *LpBuffPtr,
                                  uint8 *offset_ptr)
{

  uint8 LusCount;
  uint8 local_Offset;  
  uint8 Mka_cofidentialty_check;
       uint16 *Mka_TLV_Bytes;
  local_Offset = *offset_ptr;

  if(Paeidx)
  {

  }
  if(LpMkaPaeInstance != NULL_PTR)
  {
  
 if (LpMkaPaeInstance->ddMkaRole == MKA_PEER) {

          Mka_TLV_Bytes = (uint16 *)&LpBuffPtr[local_Offset];
          LpBuffPtr[local_Offset++] = MKA_ZERO;
          LpBuffPtr[local_Offset++] = MKA_ZERO;

          if ((Mka_GaaCryptoAlgoConfig[LpMkaKayParticipants
                                           ->ucMkaCryptoAlgoRefConfigIdx]
                   .ddMkaMacSecCapability) ==
              INTEGRITY_WITHOUT_CONFIDENTIALITY) {
            Mka_cofidentialty_check = MKA_ONE;
          }

          else if (((Mka_GaaCryptoAlgoConfig[LpMkaKayParticipants
                                                 ->ucMkaCryptoAlgoRefConfigIdx]
                         .ddMkaMacSecCapability) ==
                    INTEGRITY_AND_CONFIDENTIALITY) &&
                   ((Mka_GaaCryptoAlgoConfig[LpMkaKayParticipants
                                                 ->ucMkaCryptoAlgoRefConfigIdx]
                         .ddMkaMacSecConfidentialityOffset) ==
                    CONFIDENTIALITY_OFFSET_0)) {
            Mka_cofidentialty_check = MKA_TWO;
          } else if (((Mka_GaaCryptoAlgoConfig
                           [LpMkaKayParticipants->ucMkaCryptoAlgoRefConfigIdx]
                               .ddMkaMacSecCapability) ==
                      INTEGRITY_AND_CONFIDENTIALITY) &&
                     (((Mka_GaaCryptoAlgoConfig
                            [LpMkaKayParticipants->ucMkaCryptoAlgoRefConfigIdx]
                                .ddMkaMacSecConfidentialityOffset) ==
                       CONFIDENTIALITY_OFFSET_30) ||
                      ((Mka_GaaCryptoAlgoConfig
                            [LpMkaKayParticipants->ucMkaCryptoAlgoRefConfigIdx]
                                .ddMkaMacSecConfidentialityOffset) ==
                       CONFIDENTIALITY_OFFSET_50))) {
            Mka_cofidentialty_check = MKA_THREE;
          } else {
            Mka_cofidentialty_check = MKA_ZERO;
          }

          for (LusCount = MKA_ZERO;
               LusCount <
               Mka_GaaCryptoAlgoConfig[LpMkaKayParticipants
                                           ->ucMkaCryptoAlgoRefConfigIdx]
                   .ucnoofciphersuites;
               LusCount++) {
            if (Mka_GaaCryptoAlgoConfig[LpMkaKayParticipants
                                            ->ucMkaCryptoAlgoRefConfigIdx]
                    .pMkaCipherSuites[LusCount]
                    .ddMkaMacSecCipherSuite == GCM_AES_128) {
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = Mka_cofidentialty_check;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = 0x80U;
              LpBuffPtr[local_Offset++] = 0xC2;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ONE;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ONE;
            }
            if (Mka_GaaCryptoAlgoConfig[LpMkaKayParticipants
                                            ->ucMkaCryptoAlgoRefConfigIdx]
                    .pMkaCipherSuites[LusCount]
                    .ddMkaMacSecCipherSuite == GCM_AES_256) {
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = Mka_cofidentialty_check;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = 0x80U;
              LpBuffPtr[local_Offset++] = 0xC2;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ONE;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_TWO;
            }
            if (Mka_GaaCryptoAlgoConfig[LpMkaKayParticipants
                                            ->ucMkaCryptoAlgoRefConfigIdx]
                    .pMkaCipherSuites[LusCount]
                    .ddMkaMacSecCipherSuite == GCM_AES_XPN_128) {
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = Mka_cofidentialty_check;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = 0x80U;
              LpBuffPtr[local_Offset++] = 0xC2;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ONE;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_THREE;
            }
            if (Mka_GaaCryptoAlgoConfig[LpMkaKayParticipants
                                            ->ucMkaCryptoAlgoRefConfigIdx]
                    .pMkaCipherSuites[LusCount]
                    .ddMkaMacSecCipherSuite == GCM_AES_XPN_256) {
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = Mka_cofidentialty_check;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = 0x80U;
              LpBuffPtr[local_Offset++] = 0xC2;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ONE;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_ZERO;
              LpBuffPtr[local_Offset++] = MKA_FOUR;
            }
          }
          ((uint8 *)(Mka_TLV_Bytes))[MKA_ZERO] = EAPOL_TYPE_MACSEC_ANNOUNCEMENT;
          ((uint8 *)(Mka_TLV_Bytes))[MKA_ONE] =
              MKA_TEN *
              Mka_GaaCryptoAlgoConfig[LpMkaKayParticipants
                                          ->ucMkaCryptoAlgoRefConfigIdx]
                  .ucnoofciphersuites;
        }

  }

}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_InternalAdvframesent                            **
** Service ID           :                                                     **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void   Mka_InternalAdvframesent(uint8 PaeInst)
{

  Mka_GaaPaeServerStatus[PaeInst].ddServerInternalState = MKA_INTRA_STATE_TX_ADV_FRAME_SENT;
  Mka_GaaPaeServerStatus[PaeInst].ddpaetatus =  MKA_STATUS_IN_PROGRESS;

       
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_KEKCallhandler                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void   Mka_InternalKayPartSel(uint8 PaeInst)
{

  Mka_GaaPaeServerStatus[PaeInst].ddServerInternalState = MKA_INTR_STATE_KAY_PART_SELECTION_COMPLETED;

       
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Internalppstatesent                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void   Mka_Internalppstatesent(uint8 PaeInst)
{

  Mka_GaaPaeServerStatus[PaeInst].ddServerInternalState = MKA_INTR_STATE_PP_FRAME_SENT;
  
       
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_InternallpstateRec                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void   Mka_InternallpstateRec(uint8 PaeInst)
{

  Mka_GaaPaeServerStatus[PaeInst].ddServerInternalState = MKA_INTR_STATE_PP_FRAME_SENT;
  
       
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_InternalSakwaiting                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void   Mka_InternalSakwaiting(uint8 PaeInst)
{

  Mka_GaaPaeServerStatus[PaeInst].ddServerInternalState = MKA_INTR_STATE_SAK_WAITING;
  
       
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_InternalSakwaiting                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void   Mka_InternalSakCompleted(uint8 PaeInst)
{

  Mka_GaaPaeServerStatus[PaeInst].ddServerInternalState = MKA_INTR_STATE_SAK_COMPLETED;
  
       
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_InternalSakwaiting                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_RandomNumberGeneration(uint8 KayId,uint8 paeId)
{
  Std_ReturnType LddReturnValue;
  uint8 *Mka_MI_ptr;
  uint32 Mka_MI_length;
  Mka_KayParticipants const *LpMkaKayParticipants;
  Mka_PaeInstanceType const *Lp_PaeInstaceStatic;
  Lp_PaeInstaceStatic = &Mka_GaaPaeInstances[paeId];
  LpMkaKayParticipants = &Mka_KayParticipant[KayId];
   Mka_MI_ptr = &Mka_MI_Value[KayId][MKA_ZERO];
   Mka_MI_length = MKA_MEMBER_IDENTIFIER_LENGTH;
//Mka_MiValue[]
 if((LpMkaKayParticipants)->usMkaCryptoRandomDeriveJobRefCsmProcessingType != MKA_CRYPTO_PROCESSING_SYNC)
  {
  LddReturnValue  =   Mka_Callout_Random_Generate(LpMkaKayParticipants->usMkaCryptoRandomJobRefCsmJobId, 
                                 Mka_MI_ptr,
                                &Mka_MI_length);
   Mka_JobRegistry[KayId].JobId =  (LpMkaKayParticipants)->usMkaCryptoRandomJobRefCsmJobId;
           Mka_JobRegistry[KayId].JobCounter++;
            Mka_JobRegistry[KayId].KayIndex = KayId;
            Mka_JobRegistry[KayId].PaeId = Lp_PaeInstaceStatic->ucMkaPaeIdx;
            Mka_JobRegistry[KayId].status = MKA_TRUE;
            Mka_JobRegistyCounter[Lp_PaeInstaceStatic->ucMkaPaeIdx]++;

            MKa_GaaKayStatus[KayId] = MKA_INTR_WAIT_FOR_RANDOM;


  if(LddReturnValue)
  {

  }
  }
  else
  {
LddReturnValue  =   Mka_Callout_Random_Generate(LpMkaKayParticipants->usMkaCryptoRandomJobRefCsmJobId, 
                                 Mka_MI_ptr,
                                &Mka_MI_length);
     MKa_GaaKayStatus[KayId] = MKA_INTR_RANDOM_COMPLETE;

       if(LddReturnValue)
  {
    
  }

  }

}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"


/*******************************************************************************
** Function Name        : Mka_BuildMkPduWithoutICV                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_BuildMkPduWithoutICV(uint8 KayId,uint8 PaeInst)
{
  uint8 Length;
  uint8 Mkabyte0;
  uint8 Mkabyte1;
  uint8 *Lp_MkaPdu;
  uint8 LusCount;
 
  uint16 *MkaBasicLength;
   uint16 *LusTotalLen;
   uint32 MkaIcvLength;
  Std_ReturnType LddReturnValue;
  Mka_KayParticipants const *LpMkaKayParticipants;
  Mka_PaeInstanceType const *Lp_PaeInstaceStatic;
  Lp_MkaPdu = &Mka_Pdu[KayId].MkPdu[MKA_ZERO];
  Length = Mka_Pdu[KayId].length;
  MkaIcvLength = MKA_ICV_LENGTH;
  Mkabyte0 = MKA_ZERO;
  Mkabyte1 = MKA_ZERO;
LpMkaKayParticipants = &Mka_KayParticipant[KayId];
Lp_PaeInstaceStatic =  &Mka_GaaPaeInstances[KayId];
   Lp_MkaPdu[Length++] = EAPOL_VERSION_3;
        Lp_MkaPdu[Length++] = EAPOL_TYPE_MKA;
        LusTotalLen = (uint16 *)&Lp_MkaPdu[Length];
        Lp_MkaPdu[Length++] = MKA_ZERO;
        Lp_MkaPdu[Length++] = MKA_ZERO;
        Lp_MkaPdu[Length++] = MKA_VERSION_BASCIC_PARAMETER;
        Lp_MkaPdu[Length++] =
            (uint8)Lp_PaeInstaceStatic->ucMkaKeyServerPriority;
        MkaBasicLength = (uint16 *)&Lp_MkaPdu[Length];
        Lp_MkaPdu[Length++] = MKA_ZERO;
        Lp_MkaPdu[Length++] = MKA_ZERO;
        /*SCI*/
        for (LusCount = MKA_ZERO; LusCount < MKA_SIX; LusCount++) {
          Lp_MkaPdu[Length++] =
              *((Lp_PaeInstaceStatic->usMkaSrcMacAddress) + LusCount);
        }
        Lp_MkaPdu[Length++] = Lp_PaeInstaceStatic->ucMkaEthIfControllerIndex;
        Lp_MkaPdu[Length++] = MKA_ZERO;
        memcpy(&Lp_MkaPdu[Length], &Mka_MI_Value[KayId][MKA_ZERO], MKA_MEMBER_IDENTIFIER_LENGTH);
        Length += MKA_MEMBER_IDENTIFIER_LENGTH;
        if ((Mka_BascsicParameterMemberNumber[PaeInst] == MKA_ZERO) ||
            Mka_BascsicParameterMemberNumber[PaeInst] >
                MKA_MAXIMUM_LENGTH_OF_AMN) {
          Lp_MkaPdu[Length++] = MKA_ZERO;
          Lp_MkaPdu[Length++] = MKA_ZERO;
          Lp_MkaPdu[Length++] = MKA_ZERO;
          Lp_MkaPdu[Length++] = MKA_ONE;
          Mka_BascsicParameterMemberNumber[PaeInst]++;
        } else {
          Mka_BascsicParameterMemberNumber[PaeInst]++;

          Lp_MkaPdu[Length++] = ((Mka_BascsicParameterMemberNumber[PaeInst]) &
                                 MKA_TWO_FIFTY_FIVE);
          Lp_MkaPdu[Length++] =
              ((Mka_BascsicParameterMemberNumber[PaeInst] >> MKA_EIGHT) &
               MKA_TWO_FIFTY_FIVE);
          Lp_MkaPdu[Length++] =
              ((Mka_BascsicParameterMemberNumber[PaeInst] >> MKA_SIXTEEN) &
               MKA_TWO_FIFTY_FIVE);
          Lp_MkaPdu[Length++] =
              ((Mka_BascsicParameterMemberNumber[PaeInst] >> MKA_TWENTY_FOUR) &
               MKA_TWO_FIFTY_FIVE);
        }

        Lp_MkaPdu[Length++] =
            (MKA_ALGO_AGILITY_STANDARD >> MKA_TWENTY_FOUR) & MKA_TWO_FIFTY_FIVE;
        Lp_MkaPdu[Length++] =
            (MKA_ALGO_AGILITY_STANDARD >> MKA_SIXTEEN) & MKA_TWO_FIFTY_FIVE;
        Lp_MkaPdu[Length++] =
            (MKA_ALGO_AGILITY_STANDARD >> MKA_EIGHT) & MKA_TWO_FIFTY_FIVE;

        Lp_MkaPdu[Length++] = (uint8)(MKA_ALGO_AGILITY_STANDARD);

        for (LusCount = MKA_ZERO; LusCount < (MKA_CKN_LENGTH); LusCount++) {
          Lp_MkaPdu[Length++] = (LpMkaKayParticipants)->Mka_Ckn[LusCount];
        }

        Mkabyte0 |= (Lp_PaeInstaceStatic->ddMkaRole << MKA_SEVEN);
        Mkabyte0 |= (MKA_TRUE << MKA_SIX);
        Mkabyte0 |= ((Mka_GaaCryptoAlgoConfig[(LpMkaKayParticipants)
                                            ->ucMkaCryptoAlgoRefConfigIdx]
                    .ddMkaMacSecCapability &
                MKA_THREE)
               << MKA_FOUR);
        Mkabyte0 |= (uint8)((((Length - MKA_EIGHT) & (MKA_MASK)) >> MKA_EIGHT) &
                      MKA_FIFTEEN);
        Mkabyte1 = (uint8)(((Length - MKA_EIGHT) & (MKA_MASK)) & MKA_TWO_FIFTY_FIVE);

        ((uint8 *)MkaBasicLength)[MKA_ZERO] = Mkabyte0;
        ((uint8 *)MkaBasicLength)[MKA_ONE] = Mkabyte1;

  
        /*TLV Frame for MacSec Cipher Suites*/
       
        Lp_MkaPdu[Length++] = MKA_TWO_FIFTY_FIVE;
        Lp_MkaPdu[Length++] = MKA_ZERO;
        Lp_MkaPdu[Length++] = MKA_ZERO;


        if(LpMkaKayParticipants->usMkaCryptoIcvDeriveJobRefCsmProcessingType != MKA_CRYPTO_PROCESSING_SYNC )
        {
           LddReturnValue = Mka_ICV_Generation_Callout(
            PaeInst, LpMkaKayParticipants->usMkaCryptoIcvGenerateJobRefCsmJobId,
            LpMkaKayParticipants->usMkaCryptoICKKeyRefCsmKeyId, Lp_MkaPdu,
            Length,&Mka_IcvGen[KayId][PaeInst] , &MkaIcvLength);

             Mka_JobRegistry[KayId].JobId =  (LpMkaKayParticipants)->usMkaCryptoRandomJobRefCsmJobId;
             Mka_JobRegistry[KayId].JobCounter++;
             Mka_JobRegistry[KayId].KayIndex = KayId;
             Mka_JobRegistry[KayId].PaeId = Lp_PaeInstaceStatic->ucMkaPaeIdx;
             Mka_JobRegistry[KayId].status = MKA_TRUE;
             Mka_JobRegistyCounter[Lp_PaeInstaceStatic->ucMkaPaeIdx]++;
             MKa_GaaKayStatus[KayId] = MKA_INTR_WAIT_FOR_ICV;

        }
        else
        
      {
        
            LddReturnValue = Mka_ICV_Generation_Callout(
            PaeInst, LpMkaKayParticipants->usMkaCryptoIcvGenerateJobRefCsmJobId,
            LpMkaKayParticipants->usMkaCryptoICKKeyRefCsmKeyId, Lp_MkaPdu,
            Length,&Mka_IcvGen[KayId][PaeInst] , &MkaIcvLength);
            memcpy(&Lp_MkaPdu[Length], &Mka_IcvGen, MkaIcvLength);

              Length += MKA_ICV_LENGTH;
                ((uint8 *)(LusTotalLen))[MKA_ZERO] = (Length - MKA_FOUR);
               ((uint8 *)(LusTotalLen))[MKA_ONE] = MKA_ZERO;
             MKa_GaaKayStatus[KayId] = MKA_INTR_ICV_GEN_COMPLETE;

      }

          if(LddReturnValue)
          {

          }
          


          
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_BuildMkPduWithoutICV                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_Paramtersethandler(uint8 PaeInst,uint8 KayId)
{

  
  boolean blcipherselection;
 
  uint8 Select_part;
  uint8 Select_Cipher;
  boolean blcaprecrd;

 
  Mka_KayParticipants const *LpMkaKayParticipants;
  Mka_PaeInstanceType const *LpMkaPaeInstance;
  blcaprecrd = MKA_FALSE;
  blcipherselection = MKA_FALSE;
   
  
  LpMkaPaeInstance = &Mka_GaaPaeInstances[PaeInst];
  LpMkaKayParticipants = &Mka_KayParticipant[KayId];

   blcaprecrd = Mka_CheckforConnectivityAssociation(PaeInst, &Select_part);


     if (  blcaprecrd) {
  
    if ((Mka_GaaBasicparameterset[PaeInst].Mka_flag.ddkeyserver == MKA_ONE) ||
        (Mka_GaaBasicparameterset[PaeInst].Mka_flag.ddkeyserver == MKA_ZERO)) {

     
           if (Mka_Selectedparticipant.blkaypartselect != MKA_TRUE)  {
        LpMkaKayParticipants = &Mka_KayParticipant[KayId];
        blcipherselection = Mka_SelectCiphersuites(PaeInst, &Select_Cipher,
                                                   LpMkaKayParticipants);
        if (!blcipherselection) {
          MKA_E_ALGO_MISMATCH_INSTANCE(LpMkaPaeInstance);
        }
      }
      if (Mka_Selectedparticipant.blkaypartselect != MKA_FALSE) {
        LpMkaKayParticipants =
            &Mka_KayParticipant[Mka_Selectedparticipant.index];
      }



        /* Creaeting of Potential peer List*/
        if (!Mka_Selectedparticipant.blkaypartselect) {

          Mka_Create_TxPdu(PaeInst, KayId, (uint8)Frame_list_type[PaeInst],
                           &MKA_PortPhysicalAddress[PaeInst]);
        } else {
          if (Mka_Selectedparticipant.index == KayId) {
            Mka_Create_TxPdu(PaeInst, Mka_Selectedparticipant.index,
                             (uint8)Frame_list_type[PaeInst], &MKA_PortPhysicalAddress[PaeInst]);
          } else {
            /*Discard*/
          }
        }

     
    }

    /*Dummy values use case not i*/


  } else {
    MKA_E_KEY_MISMATCH_INSTANCE(LpMkaPaeInstance);
  }

          
}


#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_BuildMkPduWithoutICV                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

void Mka_CknCheckHandler(uint8 paeId, uint8 kayId)
{
  
Std_ReturnType LddReturnValue;
uint8 CipherSuites;
Mka_KayParticipants const *LpMkaKayParticipants;
Mka_PaeInstanceType const *LpMkaPaeInstance;
LddReturnValue = E_NOT_OK;

LpMkaKayParticipants = &Mka_KayParticipant[kayId];
LpMkaPaeInstance    = &Mka_GaaPaeInstances[paeId];
LddReturnValue =   Mka_CknMatching(&Mka_GaaBasicparameterset[paeId],
                                            ((LpMkaKayParticipants)->Mka_Ckn));



  if (E_OK == LddReturnValue)
  {
    LddReturnValue = Mka_SelectCiphersuites(paeId, &CipherSuites,
                                                   LpMkaKayParticipants);

    if(E_OK == LddReturnValue)
    {
          MKa_GaaKayStatus[paeId]  = MKA_INTR_ICV_VERIFY;
    }
    else
    {
        
          MKA_E_ALGO_MISMATCH_INSTANCE(LpMkaPaeInstance);
        
    }
  }
  else
  {
         MKA_E_KEY_MISMATCH_INSTANCE(LpMkaPaeInstance);
  }

}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_BuildMkPduWithoutICV                                  **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_IcvVerifyHandler(uint8 paeId, uint8 kayId)
{

Mka_KayParticipants const *LpMkaKayParticipants;
Mka_PaeInstanceType const *LpMkaPaeInstance;
Std_ReturnType LddReturnValue;


LpMkaKayParticipants = &Mka_KayParticipant[kayId];
LpMkaPaeInstance    = &Mka_GaaPaeInstances[paeId];
 LddReturnValue = Mka_IcvMatching(
        &Mka_GaaBasicparameterset[paeId],
        (LpMkaKayParticipants->Mka_Ckn), paeId,
        LpMkaPaeInstance, &Mka_GaaICV[paeId][MKA_ZERO], &MKA_PortPhysicalAddress[paeId]);

  if(E_OK == LddReturnValue)
 {
   MKa_GaaKayStatus[paeId] = MKA_INTR_FRAME_CHECK;
 }
 else
{
  MKA_E_KEY_MISMATCH_INSTANCE(LpMkaPaeInstance);
}

}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_FrameCheck                                      **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_FrameCheck(uint8 paeId, uint8 kayId)
{

   if(Frame_list_type[paeId] ==  MKA_POTENTIAL_TYPE)
   {
     MKa_GaaKayStatus[kayId] = MKA_INTR_POTENTIAL;
   }
   if(Frame_list_type[paeId] ==  MKA_LIVE_TYPE)
   {
     MKa_GaaKayStatus[kayId] = MKA_INTR_LIVE;
   }
   if(Frame_list_type[paeId] == MKA_USE_LIVE_TYPE)
   {
      MKa_GaaKayStatus[kayId] =  MKA_INTR_LIVE;
   }
   if(Frame_list_type[paeId] == MKA_DISTRIBUTED_TYPE)
   {
      MKa_GaaKayStatus[kayId] =  MKA_INTR_DISTRIBUTED;
   }
   if(Frame_list_type[paeId] == MKA_BASIC_FRAME_TYPE )
   {
      MKa_GaaKayStatus[kayId] =  MKA_INTR_FRAME_BASIC;
   }
  
  
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_FrameCheck                                      **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_Potentialhandler(uint8 PaeInst,uint8 kayId)
{
  
 if(PaeInst && kayId)
 {

 }


}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_FrameCheck                                      **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_BasicHandler(uint8 PaeInst,uint8 kayId){



Mka_RandomNumberGeneration(kayId,PaeInst);



}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_FrameCheck                                      **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

void Mka_Icvhandler (uint8 PaeInst,uint8 kayId)
{
  if(Frame_list_type[PaeInst] == MKA_BASIC_FRAME_TYPE )
  {
      Mka_BuildMkPduWithoutICV(kayId,PaeInst);
  }
 else if(Frame_list_type[PaeInst] == MKA_POTENTIAL_TYPE)
 {
    Mka_BuildMkPduPotentialWithoutICV(kayId,PaeInst);
 }
  else if(Frame_list_type[PaeInst] == MKA_LIVE_TYPE)
 {
  Mka_BuildMkPduLiveWithoutICV(kayId,PaeInst);
 }


}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_FrameCheck                                      **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_BuildMkPduPotentialWithoutICV(uint8 kayId,uint8 PaeInst)
{
uint8 Length;
  uint8 Mkabyte0;
  uint8 Mkabyte1;
  uint8 *Lp_MkaPdu;
  uint8 LusCount;
 
  uint16 *MkaBasicLength;
  uint16 *Mka_thirdbyte;
   uint16 *LusTotalLen;
   uint32 MkaIcvLength;
  Std_ReturnType LddReturnValue;
  Mka_KayParticipants const *LpMkaKayParticipants;
  Mka_PaeInstanceType const *Lp_PaeInstaceStatic;
  Lp_MkaPdu = &Mka_Pdu[kayId].MkPdu[MKA_ZERO];
  Length = Mka_Pdu[kayId].length;
  MkaIcvLength = MKA_ICV_LENGTH;
  Mkabyte0 = MKA_ZERO;
  Mkabyte1 = MKA_ZERO;
LpMkaKayParticipants = &Mka_KayParticipant[kayId];
Lp_PaeInstaceStatic =  &Mka_GaaPaeInstances[kayId];
   Lp_MkaPdu[Length++] = EAPOL_VERSION_3;
        Lp_MkaPdu[Length++] = EAPOL_TYPE_MKA;
        LusTotalLen = (uint16 *)&Lp_MkaPdu[Length];
        Lp_MkaPdu[Length++] = MKA_ZERO;
        Lp_MkaPdu[Length++] = MKA_ZERO;
        Lp_MkaPdu[Length++] = MKA_VERSION_BASCIC_PARAMETER;
        Lp_MkaPdu[Length++] =
            (uint8)Lp_PaeInstaceStatic->ucMkaKeyServerPriority;
        MkaBasicLength = (uint16 *)&Lp_MkaPdu[Length];
        Lp_MkaPdu[Length++] = MKA_ZERO;
        Lp_MkaPdu[Length++] = MKA_ZERO;
        /*SCI*/
        for (LusCount = MKA_ZERO; LusCount < MKA_SIX; LusCount++) {
          Lp_MkaPdu[Length++] =
              *((Lp_PaeInstaceStatic->usMkaSrcMacAddress) + LusCount);
        }
        Lp_MkaPdu[Length++] = Lp_PaeInstaceStatic->ucMkaEthIfControllerIndex;
        Lp_MkaPdu[Length++] = MKA_ZERO;
        memcpy(&Lp_MkaPdu[Length], &Mka_MI_Value[kayId][MKA_ZERO], MKA_MEMBER_IDENTIFIER_LENGTH);
        Length += MKA_MEMBER_IDENTIFIER_LENGTH;
        if ((Mka_BascsicParameterMemberNumber[PaeInst] == MKA_ZERO) ||
            Mka_BascsicParameterMemberNumber[PaeInst] >
                MKA_MAXIMUM_LENGTH_OF_AMN) {
          Lp_MkaPdu[Length++] = MKA_ZERO;
          Lp_MkaPdu[Length++] = MKA_ZERO;
          Lp_MkaPdu[Length++] = MKA_ZERO;
          Lp_MkaPdu[Length++] = MKA_ONE;
          Mka_BascsicParameterMemberNumber[PaeInst]++;
        } else {
          Mka_BascsicParameterMemberNumber[PaeInst]++;

          Lp_MkaPdu[Length++] = ((Mka_BascsicParameterMemberNumber[PaeInst]) &
                                 MKA_TWO_FIFTY_FIVE);
          Lp_MkaPdu[Length++] =
              ((Mka_BascsicParameterMemberNumber[PaeInst] >> MKA_EIGHT) &
               MKA_TWO_FIFTY_FIVE);
          Lp_MkaPdu[Length++] =
              ((Mka_BascsicParameterMemberNumber[PaeInst] >> MKA_SIXTEEN) &
               MKA_TWO_FIFTY_FIVE);
          Lp_MkaPdu[Length++] =
              ((Mka_BascsicParameterMemberNumber[PaeInst] >> MKA_TWENTY_FOUR) &
               MKA_TWO_FIFTY_FIVE);
        }

        Lp_MkaPdu[Length++] =
            (MKA_ALGO_AGILITY_STANDARD >> MKA_TWENTY_FOUR) & MKA_TWO_FIFTY_FIVE;
        Lp_MkaPdu[Length++] =
            (MKA_ALGO_AGILITY_STANDARD >> MKA_SIXTEEN) & MKA_TWO_FIFTY_FIVE;
        Lp_MkaPdu[Length++] =
            (MKA_ALGO_AGILITY_STANDARD >> MKA_EIGHT) & MKA_TWO_FIFTY_FIVE;

        Lp_MkaPdu[Length++] = (uint8)(MKA_ALGO_AGILITY_STANDARD);

        for (LusCount = MKA_ZERO; LusCount < (MKA_CKN_LENGTH); LusCount++) {
          Lp_MkaPdu[Length++] = (LpMkaKayParticipants)->Mka_Ckn[LusCount];
        }
/*Starts*/

    Lp_MkaPdu[Length++] = MKA_THREE;

    Lp_MkaPdu[Length++] = MKA_ZERO;
    Mka_thirdbyte = (uint16 *)&Lp_MkaPdu[Length];
    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_ZERO;

    for (LusCount = MKA_ZERO; LusCount < MKA_TWELEVE; LusCount++) {
      Lp_MkaPdu[Length++] = Mka_StorageMI[PaeInst][MKA_ONE].MI[LusCount];
    }

    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_ONE;

    /*Latest Lowest Acceptable PN */
    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_ZERO;

        for (LusCount = MKA_ZERO; LusCount < MKA_TWELEVE; LusCount++) {
      Lp_MkaPdu[Length++] = Mka_StorageMI[PaeInst][MKA_ONE].MI[LusCount];
    }

    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_ZERO;

    /*Latest Lowest Acceptable PN */
    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_ZERO;
    
     ((uint8 *)(Mka_thirdbyte))[MKA_ZERO] = MKA_ZERO;
      ((uint8 *)(Mka_thirdbyte))[MKA_ONE] = (MKA_TWENTY_FOUR + MKA_FOUR);

  Lp_MkaPdu[Length++] = MKA_FOUR;
  Lp_MkaPdu[Length++] = MKA_ZERO;
  Lp_MkaPdu[Length++] = MKA_ZERO;

  Lp_MkaPdu[Length++] = (MKA_TWENTY_FOUR + MKA_FOUR);

    Lp_MkaPdu[Length++] = MKA_ZERO;
  Lp_MkaPdu[Length++] = MKA_ZERO;
  Lp_MkaPdu[Length++] = MKA_ZERO;
  /*Rekeying is not supported */
  Lp_MkaPdu[Length++] = MKA_ONE;
  

        
/*Ends Here*/



        Mkabyte0 |= (Lp_PaeInstaceStatic->ddMkaRole << MKA_SEVEN);
        Mkabyte0 |= (MKA_TRUE << MKA_SIX);
        Mkabyte0 |= ((Mka_GaaCryptoAlgoConfig[(LpMkaKayParticipants)
                                            ->ucMkaCryptoAlgoRefConfigIdx]
                    .ddMkaMacSecCapability &
                MKA_THREE)
               << MKA_FOUR);
        Mkabyte0 |= (uint8)((((Length - MKA_EIGHT) & (MKA_MASK)) >> MKA_EIGHT) &
                      MKA_FIFTEEN);
        Mkabyte1 = (uint8)(((Length - MKA_EIGHT) & (MKA_MASK)) & MKA_TWO_FIFTY_FIVE);

        ((uint8 *)MkaBasicLength)[MKA_ZERO] = Mkabyte0;
        ((uint8 *)MkaBasicLength)[MKA_ONE] = Mkabyte1;

  
        /*TLV Frame for MacSec Cipher Suites*/
       
        Lp_MkaPdu[Length++] = MKA_TWO_FIFTY_FIVE;
        Lp_MkaPdu[Length++] = MKA_ZERO;
        Lp_MkaPdu[Length++] = MKA_ZERO;


        if(LpMkaKayParticipants->usMkaCryptoIcvDeriveJobRefCsmProcessingType != MKA_CRYPTO_PROCESSING_SYNC )
        {
           LddReturnValue = Mka_ICV_Generation_Callout(
            PaeInst, LpMkaKayParticipants->usMkaCryptoIcvGenerateJobRefCsmJobId,
            LpMkaKayParticipants->usMkaCryptoICKKeyRefCsmKeyId, Lp_MkaPdu,
            Length,&Mka_IcvGen[kayId][PaeInst] , &MkaIcvLength);

             Mka_JobRegistry[kayId].JobId =  (LpMkaKayParticipants)->usMkaCryptoRandomJobRefCsmJobId;
             Mka_JobRegistry[kayId].JobCounter++;
             Mka_JobRegistry[kayId].KayIndex = kayId;
             Mka_JobRegistry[kayId].PaeId = Lp_PaeInstaceStatic->ucMkaPaeIdx;
             Mka_JobRegistry[kayId].status = MKA_TRUE;
             Mka_JobRegistyCounter[Lp_PaeInstaceStatic->ucMkaPaeIdx]++;
             MKa_GaaKayStatus[kayId] = MKA_INTR_WAIT_FOR_ICV;

        }
        else
        
      {
        
            LddReturnValue = Mka_ICV_Generation_Callout(
            PaeInst, LpMkaKayParticipants->usMkaCryptoIcvGenerateJobRefCsmJobId,
            LpMkaKayParticipants->usMkaCryptoICKKeyRefCsmKeyId, Lp_MkaPdu,
            Length,&Mka_IcvGen[kayId][PaeInst] , &MkaIcvLength);
            memcpy(&Lp_MkaPdu[Length], &Mka_IcvGen, MkaIcvLength);

              Length += MKA_ICV_LENGTH;
                ((uint8 *)(LusTotalLen))[MKA_ZERO] = (Length - MKA_FOUR);
               ((uint8 *)(LusTotalLen))[MKA_ONE] = MKA_ZERO;
             MKa_GaaKayStatus[kayId] = MKA_INTR_ICV_GEN_COMPLETE;

      }

          if(LddReturnValue)
          {

          }
          


        }

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_FrameCheck                                      **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_BuildMkPduLiveWithoutICV(uint8 kayId,uint8 PaeInst)
{

  uint8 Length;
  uint8 Mkabyte0;
  uint8 Mkabyte1;
  uint8 *Lp_MkaPdu;
  uint8 LusCount;
 
  uint16 *MkaBasicLength;
   uint16 *LusTotalLen;
   uint32 MkaIcvLength;
  Std_ReturnType LddReturnValue;
  Mka_KayParticipants const *LpMkaKayParticipants;
  Mka_PaeInstanceType const *Lp_PaeInstaceStatic;
  Lp_MkaPdu = &Mka_Pdu[kayId].MkPdu[MKA_ZERO];
  Length = Mka_Pdu[kayId].length;
  MkaIcvLength = MKA_ICV_LENGTH;
  Mkabyte0 = MKA_ZERO;
  Mkabyte1 = MKA_ZERO;
LpMkaKayParticipants = &Mka_KayParticipant[kayId];
Lp_PaeInstaceStatic =  &Mka_GaaPaeInstances[kayId];
   Lp_MkaPdu[Length++] = EAPOL_VERSION_3;
        Lp_MkaPdu[Length++] = EAPOL_TYPE_MKA;
        LusTotalLen = (uint16 *)&Lp_MkaPdu[Length];
        Lp_MkaPdu[Length++] = MKA_ZERO;
        Lp_MkaPdu[Length++] = MKA_ZERO;
        Lp_MkaPdu[Length++] = MKA_VERSION_BASCIC_PARAMETER;
        Lp_MkaPdu[Length++] =
            (uint8)Lp_PaeInstaceStatic->ucMkaKeyServerPriority;
        MkaBasicLength = (uint16 *)&Lp_MkaPdu[Length];
        Lp_MkaPdu[Length++] = MKA_ZERO;
        Lp_MkaPdu[Length++] = MKA_ZERO;
        /*SCI*/
        for (LusCount = MKA_ZERO; LusCount < MKA_SIX; LusCount++) {
          Lp_MkaPdu[Length++] =
              *((Lp_PaeInstaceStatic->usMkaSrcMacAddress) + LusCount);
        }
        Lp_MkaPdu[Length++] = Lp_PaeInstaceStatic->ucMkaEthIfControllerIndex;
        Lp_MkaPdu[Length++] = MKA_ZERO;
        memcpy(&Lp_MkaPdu[Length], &Mka_MI_Value[kayId][MKA_ZERO], MKA_MEMBER_IDENTIFIER_LENGTH);
        Length += MKA_MEMBER_IDENTIFIER_LENGTH;
        if ((Mka_BascsicParameterMemberNumber[PaeInst] == MKA_ZERO) ||
            Mka_BascsicParameterMemberNumber[PaeInst] >
                MKA_MAXIMUM_LENGTH_OF_AMN) {
          Lp_MkaPdu[Length++] = MKA_ZERO;
          Lp_MkaPdu[Length++] = MKA_ZERO;
          Lp_MkaPdu[Length++] = MKA_ZERO;
          Lp_MkaPdu[Length++] = MKA_ONE;
          Mka_BascsicParameterMemberNumber[PaeInst]++;
        } else {
          Mka_BascsicParameterMemberNumber[PaeInst]++;

          Lp_MkaPdu[Length++] = ((Mka_BascsicParameterMemberNumber[PaeInst]) &
                                 MKA_TWO_FIFTY_FIVE);
          Lp_MkaPdu[Length++] =
              ((Mka_BascsicParameterMemberNumber[PaeInst] >> MKA_EIGHT) &
               MKA_TWO_FIFTY_FIVE);
          Lp_MkaPdu[Length++] =
              ((Mka_BascsicParameterMemberNumber[PaeInst] >> MKA_SIXTEEN) &
               MKA_TWO_FIFTY_FIVE);
          Lp_MkaPdu[Length++] =
              ((Mka_BascsicParameterMemberNumber[PaeInst] >> MKA_TWENTY_FOUR) &
               MKA_TWO_FIFTY_FIVE);
        }

        Lp_MkaPdu[Length++] =
            (MKA_ALGO_AGILITY_STANDARD >> MKA_TWENTY_FOUR) & MKA_TWO_FIFTY_FIVE;
        Lp_MkaPdu[Length++] =
            (MKA_ALGO_AGILITY_STANDARD >> MKA_SIXTEEN) & MKA_TWO_FIFTY_FIVE;
        Lp_MkaPdu[Length++] =
            (MKA_ALGO_AGILITY_STANDARD >> MKA_EIGHT) & MKA_TWO_FIFTY_FIVE;

        Lp_MkaPdu[Length++] = (uint8)(MKA_ALGO_AGILITY_STANDARD);

        for (LusCount = MKA_ZERO; LusCount < (MKA_CKN_LENGTH); LusCount++) {
          Lp_MkaPdu[Length++] = (LpMkaKayParticipants)->Mka_Ckn[LusCount];
        }
/*Starts*/

  Lp_MkaPdu[Length++] = MKA_TWO;
    /*0 for Potentila Peer as stated in Figure 11-7 IEEE802.1.X*/
    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_ZERO;
    Lp_MkaPdu[Length++] = MKA_SIXTEEN;

    for (LusCount = MKA_ZERO; LusCount < MKA_TWELEVE; LusCount++) {
      Lp_MkaPdu[Length++] =
          Mka_GaaBasicparameterset[PaeInst].Mka_AcMi[LusCount];
    }
    Lp_MkaPdu[Length++] = Mka_GaaBasicparameterset[PaeInst].Mka_AMn;
    Lp_MkaPdu[Length++] =
        Mka_GaaBasicparameterset[PaeInst].Mka_AMn >> MKA_EIGHT & MKA_TWO_FIFTY_FIVE;
    Lp_MkaPdu[Length++] =
        Mka_GaaBasicparameterset[PaeInst].Mka_AMn >> MKA_SIXTEEN & MKA_TWO_FIFTY_FIVE;
    Lp_MkaPdu[Length++] =
        Mka_GaaBasicparameterset[PaeInst].Mka_AMn >> MKA_TWENTY_FOUR & MKA_TWO_FIFTY_FIVE;
  

        
/*Ends Here*/
        Mkabyte0 |= (Lp_PaeInstaceStatic->ddMkaRole << MKA_SEVEN);
        Mkabyte0 |= (MKA_TRUE << MKA_SIX);
        Mkabyte0 |= ((Mka_GaaCryptoAlgoConfig[(LpMkaKayParticipants)
                                            ->ucMkaCryptoAlgoRefConfigIdx]
                    .ddMkaMacSecCapability &
                MKA_THREE)
               << MKA_FOUR);
        Mkabyte0 |= (uint8)((((Length - MKA_EIGHT) & (MKA_MASK)) >> MKA_EIGHT) &
                      MKA_FIFTEEN);
        Mkabyte1 = (uint8)(((Length - MKA_EIGHT) & (MKA_MASK)) & MKA_TWO_FIFTY_FIVE);

        ((uint8 *)MkaBasicLength)[MKA_ZERO] = Mkabyte0;
        ((uint8 *)MkaBasicLength)[MKA_ONE] = Mkabyte1;

  
        /*TLV Frame for MacSec Cipher Suites*/
       
        Lp_MkaPdu[Length++] = MKA_TWO_FIFTY_FIVE;
        Lp_MkaPdu[Length++] = MKA_ZERO;
        Lp_MkaPdu[Length++] = MKA_ZERO;


        if(LpMkaKayParticipants->usMkaCryptoIcvDeriveJobRefCsmProcessingType != MKA_CRYPTO_PROCESSING_SYNC )
        {
           LddReturnValue = Mka_ICV_Generation_Callout(
            PaeInst, LpMkaKayParticipants->usMkaCryptoIcvGenerateJobRefCsmJobId,
            LpMkaKayParticipants->usMkaCryptoICKKeyRefCsmKeyId, Lp_MkaPdu,
            Length,&Mka_IcvGen[kayId][PaeInst] , &MkaIcvLength);

             Mka_JobRegistry[kayId].JobId =  (LpMkaKayParticipants)->usMkaCryptoRandomJobRefCsmJobId;
             Mka_JobRegistry[kayId].JobCounter++;
             Mka_JobRegistry[kayId].KayIndex = kayId;
             Mka_JobRegistry[kayId].PaeId = Lp_PaeInstaceStatic->ucMkaPaeIdx;
             Mka_JobRegistry[kayId].status = MKA_TRUE;
             Mka_JobRegistyCounter[Lp_PaeInstaceStatic->ucMkaPaeIdx]++;
             MKa_GaaKayStatus[kayId] = MKA_INTR_WAIT_FOR_ICV;

        }
        else
        
      {
        
            LddReturnValue = Mka_ICV_Generation_Callout(
            PaeInst, LpMkaKayParticipants->usMkaCryptoIcvGenerateJobRefCsmJobId,
            LpMkaKayParticipants->usMkaCryptoICKKeyRefCsmKeyId, Lp_MkaPdu,
            Length,&Mka_IcvGen[kayId][PaeInst] , &MkaIcvLength);
            memcpy(&Lp_MkaPdu[Length], &Mka_IcvGen, MkaIcvLength);

              Length += MKA_ICV_LENGTH;
                ((uint8 *)(LusTotalLen))[MKA_ZERO] = (Length - MKA_FOUR);
               ((uint8 *)(LusTotalLen))[MKA_ONE] = MKA_ZERO;
             MKa_GaaKayStatus[kayId] = MKA_INTR_ICV_GEN_COMPLETE;

      }

          if(LddReturnValue)
          {

          }
          



}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_FrameCheck                                      **
** Service ID           : 0x10                                                **
** Description          : Set Permissive Mode of a KaY.                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                                            **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module


** InOut parameter      : None                                                **
** Output Parameters    : None                                                **
** Return parameter     : Std_ReturnType
                          E_OK: The request has been accepted and PAE instance
has been started (In case MkaPaeConfiguration.Autostart = False this method, the
PAE operation has been started) E_NOT_OK: PAE instance could not be started
because call to lower layer failed **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            :  MKA_SDD_XXXX                                       **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

void Mka_SakGenerationHandler(uint8 PaeId,uint8  KayId)
{
  Std_ReturnType LddReturnValue;
  Mka_KayParticipants const *LpMkaKayParticipants;

        LpMkaKayParticipants = &Mka_KayParticipant[ Mka_Selectedparticipant.index];
        LddReturnValue = Mka_Callout_SAK_KDF(
            (LpMkaKayParticipants)->usMkaCryptoSakDeriveJobRefCsmJobId,
            (LpMkaKayParticipants)->usMkaCryptoCknCakKeyRefCsmKeyId,
            (LpMkaKayParticipants)->usMkaCryptoSakKeyRefCsmKeyId, MKA_SAK_LABEL,
            (LpMkaKayParticipants)->Mka_Ckn, 0x80);

            MKa_GaaKayStatus[Mka_Selectedparticipant.index] = MKA_INTR_PDU_CREATE;

            if(LddReturnValue && PaeId && KayId)
            {

            }
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
**                          End of File                                       **
***************_****************************************************************/
