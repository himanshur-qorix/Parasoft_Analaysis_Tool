/*******************************************************************************
**                       KPIT Technologies Limited                            **
**                                                                            **
** KPIT Technologies Limited owns all the rights to this work. This           **
** work shall not be copied,  reproduced,  used,  modified or its information **
** disclosed without the prior written authorization of KPIT Technologies     **
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
**  Design ID :   Mka_SDD_0127                                                **
*******************************************************************************/

/*******************************************************************************
**                      Revision History                                      **
********************************************************************************
**  Date            By                  Description                           **
********************************************************************************\
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

#include "Det.h"
#include "MKa.h"
#include "Mka.h"
#include "Mka_External_Cfg.h"
#include "Mka_Internal.h"
#include "Platform_Types.h"


/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Function Definitions                                  **
*******************************************************************************/
/* polyspace-begin RTE:UNR [Justified:Low] "Refer Crc_c_Poly_REF_1" */


/*******************************************************************************
** Function Name        : Mka_GetMacSecStatisticsNotification **
** Service ID           : 0x1E                                               **
** Description          : Callback to notify that EthIf_GetMacSecStatistics or
EthIf_SwitchGetMacSecStatistics has finished and provide the requested
statistics.                                     **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Result: E_OK:MacSecStatistics have been received
                          E_NOT_OK:MacSecStatistics have not been received
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
extern void Mka_GetMacSecStatisticsNotification(uint8 MkaPaeIdx,
                                                Std_ReturnType Result) {

  if (MkaPaeIdx && Result) {
    /*Do nothing*/
  }
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_RxIndication                                  **
** Service ID           : 0x42                                               **
** Description          : Indication of a received PDU from a lower layer
communication interface module.                                     **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different PduIds. Non reentrant for the
same PduId                                           **
** Input Parameters     : RxPduId: ID of the received PDU.
                          PduInfoPtr:
                          Contains the length (SduLength) of the received PDU, a
pointer to a buffer (SduDataPtr) containing the PDU, and the MetaData related to
this PDU.
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_RxIndication(
    uint8 CtrlIdx, Eth_FrameType FrameType, boolean IsBroadcast, /**/
    const uint8 *PhysAddrPtr, /*Mac Address of the ECU from the where the Frame
                                 is comming is the Understanding*/
    const uint8 *DataPtr, uint16 LenByte) {

  Mka_PaeInstanceType const *LpMkaPaeInstance;


  uint8 const *LpSduDataPtr;
  uint8 *Offset_Ptr;

  
  uint8 offset;
  uint8 PaeIdx;
  uint8 Luscount;
  uint8 ParameterType;
  uint16 ParamterLen;
  uint8 Mka_EPOL_Type;
  Std_ReturnType LddReturnValue;
  boolean Mka_Select_flag;



  Mka_Select_flag = MKA_FALSE;
  LpSduDataPtr = DataPtr;
  PaeIdx = MKA_ZERO;
  offset = MKA_FOUR;
  Offset_Ptr = &offset;
  LpMkaPaeInstance = NULL_PTR;
  LddReturnValue = E_OK;
  Frame_list_type[CtrlIdx] = MKA_BASIC_FRAME_TYPE;
 

  /*LpMkaPaeInstance = &Mka_GaaPaeInstances[0];*/
  for (Luscount = MKA_ZERO; (Luscount < MKA_TOTAL_NUM_MKA_PAE_INSTANCE) &&
                            (Mka_Select_flag != MKA_TRUE);
       Luscount++) {
    LpMkaPaeInstance = &Mka_GaaPaeInstances[Luscount];

    if (LpMkaPaeInstance->ucMkaEthIfControllerIndex == CtrlIdx) {
      PaeIdx = Luscount;
      Mka_Select_flag = MKA_TRUE;
    }
  }
  if (Mka_Select_flag != MKA_TRUE) {
    /*DET_REMOTE Error*/

  } else {

    Mka_EPOL_Type = LpSduDataPtr[MKA_ONE];
   memcpy(&MKA_PortPhysicalAddress[PaeIdx],PhysAddrPtr,MKA_SIX);
    /*Len byte have to be included in the  frame int the signature
    before it data and length have to matched for DET check*/

    /*{
      Validation of MKPdu Before ICV Validation:

       A received EAPOL PDU shall be processed as specified by Table 11-3 if and
only if a)The destination MAC address of the MAC service indication is either
the group address recognized by the receiving MSAP for the application scenario
(see 11.1.1) or the individual MAC address of the MSAP used as the source
address of transmitted MPDUs; and

 b) The received MPDU contains the PAE Ethernet Type encoded as specified
in 11.1.4;and

 c) The received EAPOL PDU contains at least two octets, i.e., at least a Packet
Type field;and

 d)  The Packet Type is one of the values specified in Table 11-3, and the
receiving EAPOL client(s) include a protocol entity of the appropriate type as
specified in Table 11-3.

 e)Table 11-4 specifies that the receiving EAPOL client receives individual
destination addressed frames (if the destination address of the MPDU is an
individual address) or group addressed frames (if the destination address is a
group address).

 f)The Packet Body Length denotes a Packet Body that is contained within the
octets of the received EAPOL MPDU.

  Otherwise the received EAPOL PDU shall be discarded.
 Any octets following the Packet Body field in the frame conveying the EAPOL PDU
shall be ignored.

  11.5 EAPOL protocol version handling

   To ensure that backward compatibility is maintained between versions of this
protocol, a version A protocol implementation shall interpret a received EAPOL
PDU with protocol version number B as follows:

  a) Where B is greater than or equal to A, the EAPOL PDU shall be interpreted
as if it carried the supported version number, A, as follows: 1)All parameters
that are defined in version A shall be interpreted in the manner specified for
       version A of the protocol.

  b)   All parameters not defined in version A for the given EAPOL Packet Type
shall be ignored.

  c)    All octets that appear in the EAPOL PDU beyond the largest numbered
octet defined for version A for the received EAPOL Packet Type shall be ignored.

   NOTE 1—As a consequence of these rules, a version 1 implementation ignores
the version number. The rules allow future specification of protocol extensions,
identified as new versions. Subsequent versions can be required to check the
 version number in order to correctly interpret the received PDU.


 b) Where B is less than A, the EAPOL PDU shall be interpreted as specified for
the version number, B, as follows:

 1) All parameters shall be interpreted in the manner specified for version B of
the protocol. 2) All parameters not defined in version B for the given EAPOL
Packet Type shall be ignored. 3) All octets that appear in the EAPOL PDU beyond
the largest numbered octet defined for version B for the received EAPOL Packet
Type shall be ignored.

 NOTE 2—This edition of this standard provides all the information necessary to
comply with the provisions of this subclause (11.5), without the need to consult
prior editions for information on prior protocol versions. NOTE 3—IEEE Std
802.1Xbx-2014 added support for in-service upgrades including suspension of MKA
operation and recovery of the most significant bits of the PN for MACsec Cipher
Suites that use Extended Packet Numbering. The EAPOL version number was
unaffected by this amendment. Each MKPDU (an EAPOL PDU with a Packet Type of
 EAPOL-MKA) carries its own MKA Version Identifier in the Basic Parameter Set
(11.11, Table 11-6, and Figure 11-8).

 IEEE 802.1X Standard.


} */

    Mka_ProcessMkPdu(PaeIdx, LpSduDataPtr, LenByte, Offset_Ptr);
    Frame_list_type[PaeIdx] = MKA_BASIC_FRAME_TYPE;

    while (offset + MKA_FIVE <= LenByte) {

      ParameterType = LpSduDataPtr[offset];

      ParamterLen = ((LpSduDataPtr[offset + MKA_TWO] & MKA_FIFTEEN) |
                     LpSduDataPtr[offset + MKA_THREE]);

      if (ParamterLen + offset + MKA_THREE > LenByte) {
        LddReturnValue = E_NOT_OK;
        break;
      }
      if (!LddReturnValue) {
        switch (ParameterType) {
        case MKA_SIX: // Basic Paramter Set Type

          /* code */
          /*  Mka_ProcessMkPdu(PaeIdx, LpSduDataPtr, LenByte, Offset_Ptr);
            Frame_list_type = MKA_BASIC_FRAME_TYPE;*/
          break;

        case MKA_POTENTIAL_TYPE: {
          Mka_ProcessPotentialPeerList(PaeIdx, LpSduDataPtr, LenByte,
                                       Offset_Ptr);
          Frame_list_type[PaeIdx] = MKA_POTENTIAL_TYPE;
          break;
        }
        case MKA_LIVE_TYPE: {
          Mka_ProcessLivePeerList(PaeIdx, LpSduDataPtr, LenByte, Offset_Ptr);
          if (Frame_list_type[PaeIdx] != MKA_USE_LIVE_TYPE) {
            Frame_list_type[PaeIdx] = MKA_LIVE_TYPE;
          } else {
            /* Do Nothing*/
          }
          break;
        }
        case MKA_USE_LIVE_TYPE: {
          Mka_ProcessUseParamterSet(PaeIdx, LpSduDataPtr, LenByte, Offset_Ptr);
          Frame_list_type[PaeIdx] = MKA_USE_LIVE_TYPE;
          break;
        }
        case MKA_DISTRIBUTED_TYPE: {
          Mka_ProcessDistributedParamter(PaeIdx, LpSduDataPtr, LenByte,
                                         Offset_Ptr);
          Frame_list_type[PaeIdx] = MKA_USE_LIVE_TYPE;
          break;
        }
        case MKA_ICV_INDICATOR: {
          Mka_ProcessICVIndicatior(PaeIdx, LpSduDataPtr, LenByte, Offset_Ptr);
          break;
        }
          /*This is only handled for the peer */
        case MKA_MACSEC_TLV_TYPE: {
          Mka_ProcessMacSecTLV(PaeIdx, LpSduDataPtr, LenByte, Offset_Ptr);
          break;
        }
        default:
          break;
        }
      }
    }
  }
LpMkaPaeInstance    = &Mka_GaaPaeInstances[PaeIdx];
  for(Luscount = LpMkaPaeInstance->ucindexofKayParticipant;Luscount < LpMkaPaeInstance->ucnoofKayParticipant;Luscount++ )
  {
    MKa_GaaKayStatus[Luscount] = MKA_INTR_CKN_CHECK;
  }
   
    if (CtrlIdx && FrameType && IsBroadcast && LenByte && PaeIdx &&
        Mka_EPOL_Type) {
    } else {
    }

}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/* TBD where to find reference of PduID */

/*******************************************************************************
** Function Name        : Mka_TxConfirmation                                  **
** Service ID           : 0x40                                               **
** Description          : The lower layer communication interface module
confirms the transmission of a PDU, or the failure to transmit a PDU. **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different PduIds. Non reentrant for the
same PduId                                          **
** Input Parameters     : TxPduId: ID of the PDU that has been transmitted.
                          Result:
                          E_OK: The PDU was transmitted.
                          E_NOT_OK: Transmission of
                          the PDU failed.
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
extern void Mka_TxConfirmation(PduIdType TxPduId, Std_ReturnType result) {

  if (TxPduId && result) {
  }
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_MacSecUpdateSecYNotification **
** Service ID           : 0x21                                               **
** Description          : Callback to notify that EthIf_MacSecUpdateSecY or
EthIf_SwitchMacSecUpdateSecY has finished.. **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Result: E_OK: EthIf_MacSecUpdateSecY or EthIf_SwitchMacSec UpdateSecY
has finished and SecY is updated with the provided parameters of
EthIf_MacSecUpdateSecY or EthIf_SwitchMacSec UpdateSecY E_NOT_OK:SecY has not
been updated with the provided parameters of EthIf_MacSecUpdateSecY or
EthIf_SwitchMacSec UpdateSecY
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
extern void Mka_MacSecUpdateSecYNotification(uint8 MkaPaeIdx,
                                             Std_ReturnType Result) {
  if (MkaPaeIdx && Result) {
    /*Do nothing*/
  }
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_MacSecAddTxSaNotification **
** Service ID           : 0x22                                               **
** Description          : Callback to notify that EthIf_MacSecAddTxSa or
EthIf_SwitchMacSecAddTxSa has finished                                     **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Result: E_OK: E_OK: EthIf_MacSecAddTxSa or EthIf_SwitchMacSecAdd TxSa
has finished and Transmission Secure Association is created E_NOT_OK:The
Transmission Secure Association is not created through EthIf_MacSecAddTxSa or
EthIf_SwitchMacSecAddTxSa
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
extern void Mka_MacSecAddTxSaNotification(uint8 MkaPaeIdx,
                                          Std_ReturnType Result) {
  if (!Result) {

    if (Mka_GaaPaeServerStatus[MkaPaeIdx].ddpaetatus ==
        MKA_STATUS_IN_PROGRESS) {
      Mka_GaaPaeServerStatus[MkaPaeIdx].ddpaetatus =
        MKA_STATUS_MACSEC_RUNNING;
        MKa_GaaKayStatus[ Mka_Selectedparticipant.index] = MKA_INTR_KAY_RUNNIG;


    }
  }
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_MacSecAddRxSaNotification **
** Service ID           : 0x23                                               **
** Description          : Callback to notify that EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa has finished.                                     **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Result: E_OK: EthIf_MacSecAddRxSa or EthIf_SwitchMacSecAddRxSa has
finished and Reception Secure Association is created E_NOT_OK:The Reception
Secure Association is not created through EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
extern void Mka_MacSecAddRxSaNotification(uint8 MkaPaeIdx,
                                          Std_ReturnType Result) {

  if (!Result) {

    if (Mka_GaaPaeServerStatus[MkaPaeIdx].ddpaetatus ==
        MKA_STATUS_IN_PROGRESS) {
     Mka_GaaPaeServerStatus[MkaPaeIdx].ddpaetatus =
        MKA_STATUS_MACSEC_RUNNING;
      MKa_GaaKayStatus[ Mka_Selectedparticipant.index] = MKA_INTR_KAY_RUNNIG;
    }
  }
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_Csm_CbkNotification_MkaCryptoIckDeriveJob **
** Service ID           : 0x23                                               **
** Description          : Callback to notify that EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa has finished.                                     **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Result: E_OK: EthIf_MacSecAddRxSa or EthIf_SwitchMacSecAddRxSa has
finished and Reception Secure Association is created E_NOT_OK:The Reception
Secure Association is not created through EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

void Mka_Csm_CbkNotification_MkaCryptoIckDeriveJob(const uint32 jobId,
                                                   Std_ReturnType Result)
{

  uint8 Luscount;
  if (Result == E_OK) {
/*Rationale for using loop for updation of state during and notification invocation by CSM because CP_SWS_Mka_00006*/
 for(Luscount = MKA_ZERO; Luscount < MKA_SEVEN; Luscount++ )
 {
    if ((Mka_JobRegistry[Luscount].JobId == jobId) && (Mka_JobRegistry[Luscount].status == MKA_TRUE))
     {
        if( Mka_JobRegistyCounter[Mka_JobRegistry[Luscount].PaeId] !=MKA_ZERO)
        {
          Mka_JobRegistyCounter[Mka_JobRegistry[Luscount].PaeId]--;
          Mka_JobRegistry[Luscount].status = MKA_FALSE;

          if( Mka_JobRegistyCounter[Mka_JobRegistry[Luscount].PaeId] == MKA_ZERO)
          {
        	  MKa_GaaKayStatus[Luscount] = MKA_INTR_ICK_COMPLETE;
          }
        }
      }
        
    
    } 
  }
    else {
      /*
        For the Retry Mechanism 
        Mka_JobRegistry[Luscount].status  == MKA_FALSE;
      */
    }
  


}


#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Csm_CbkNotification_MkaCryptoKekDeriveJob **
** Service ID           : 0x23                                               **
** Description          : Callback to notify that EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa has finished.                                     **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Result: E_OK: EthIf_MacSecAddRxSa or EthIf_SwitchMacSecAddRxSa has
finished and Reception Secure Association is created E_NOT_OK:The Reception
Secure Association is not created through EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"

void Mka_Csm_CbkNotification_MkaCryptoKekDeriveJob(const uint32 jobId,
                                                   Std_ReturnType Result)

{

  uint8 Luscount;
  if (Result == E_OK) {
/*Rationale for using loop for updation of state during and notification invocation by CSM because CP_SWS_Mka_00006*/
 for(Luscount = MKA_ZERO; Luscount < MKA_SEVEN; Luscount++ )
 {
    if ((Mka_JobRegistry[Luscount].JobId == jobId) && (Mka_JobRegistry[Luscount].status == MKA_TRUE))
     {
        if( Mka_JobRegistyCounter[Mka_JobRegistry[Luscount].PaeId] !=MKA_ZERO)
        {
          Mka_JobRegistyCounter[Mka_JobRegistry[Luscount].PaeId]--;
          Mka_JobRegistry[Luscount].status = MKA_FALSE;

          if( Mka_JobRegistyCounter[Mka_JobRegistry[Luscount].PaeId] == MKA_ZERO)
          {
        	    MKa_GaaKayStatus[Luscount] = MKA_INTR_KEK_COMPLETE;
          }
        }
      }
        
    
    } 
  }
    else {
      /*
        For the Retry Mechanism 
        Mka_JobRegistry[Luscount].status  == MKA_FALSE;
      */
    }
  


}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_Csm_CbkNotification_MkaCryptoIcvGenerateJob **
** Service ID           : 0x23                                                **
** Description          : Callback to notify that EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa has finished.                                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Result: E_OK: EthIf_MacSecAddRxSa or EthIf_SwitchMacSecAddRxSa has
finished and Reception Secure Association is created E_NOT_OK:The Reception
Secure Association is not created through EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_Csm_CbkNotification_MkaCryptoIcvGenerateJob(const uint32 jobId,
                                                     Std_ReturnType Result)

{

  uint8 Luscount;
  if (Result == E_OK) {
/*Rationale for using loop for updation of state during and notification invocation by CSM because CP_SWS_Mka_00006*/
 for(Luscount = MKA_ZERO; Luscount < MKA_SEVEN; Luscount++ )
 {
    if ((Mka_JobRegistry[Luscount].JobId == jobId) && (Mka_JobRegistry[Luscount].status == MKA_TRUE))
     {
        if( Mka_JobRegistyCounter[Mka_JobRegistry[Luscount].PaeId] !=MKA_ZERO)
        {
          Mka_JobRegistyCounter[Mka_JobRegistry[Luscount].PaeId]--;
          Mka_JobRegistry[Luscount].status = MKA_FALSE;

          if( Mka_JobRegistyCounter[Mka_JobRegistry[Luscount].PaeId] == MKA_ZERO)
          {
            
        	    MKa_GaaKayStatus[Luscount] = MKA_INTR_KEK_COMPLETE;

           
         
          }
        }
      }
        
    
    } 
  }
    else {
      /*
        For the Retry Mechanism 
        Mka_JobRegistry[Luscount].status  == MKA_FALSE;
      */
    }
  


}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"
/*******************************************************************************
** Function Name        : Mka_Csm_CbkNotification_MkaCryptoIcvGenerateJob **
** Service ID           : 0x23                                                **
** Description          : Callback to notify that EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa has finished.                                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Result: E_OK: EthIf_MacSecAddRxSa or EthIf_SwitchMacSecAddRxSa has
finished and Reception Secure Association is created E_NOT_OK:The Reception
Secure Association is not created through EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_Csm_CbkNotification_MkaCryptoIcvVerifyJob(const uint32 jobId,
                                                   Std_ReturnType Result)

{
  if (Result == E_OK) {
    if (Mka_JobRegistry[jobId].JobId == jobId) {
      Mka_GaaPaeServerStatus[Mka_JobRegistry[jobId].PaeId]
          .ddServerInternalState = MKA_INTR_STATE_ICK_COMPLETED;
    } else {
      /*Do nothing*/
    }
  }
}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Csm_CbkNotification_MkaCryptoSakDeriveJob      **
** Service ID           : 0x23                                                **
** Description          : Callback to notify that EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa has finished.                                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Result: E_OK: EthIf_MacSecAddRxSa or EthIf_SwitchMacSecAddRxSa has
finished and Reception Secure Association is created E_NOT_OK:The Reception
Secure Association is not created through EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_Csm_CbkNotification_MkaCryptoSakDeriveJob(const uint32 jobId,
                                                   Std_ReturnType Result)

{
  if (Result == E_OK) {
    if (Mka_JobRegistry[jobId].JobId == jobId) {
      Mka_GaaPaeServerStatus[Mka_JobRegistry[jobId].PaeId]
          .ddServerInternalState = MKA_INTR_STATE_ICK_COMPLETED;
    } else {
      /*Do nothing*/
    }
  }
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Csm_CbkNotification_MkaCryptoSakDeriveJob      **
** Service ID           : 0x23                                                **
** Description          : Callback to notify that EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa has finished.                                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Result: E_OK: EthIf_MacSecAddRxSa or EthIf_SwitchMacSecAddRxSa has
finished and Reception Secure Association is created E_NOT_OK:The Reception
Secure Association is not created through EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_Csm_CbkNotification_MkaCryptoHashKey128DerivationJob(
    const uint32 jobId, Std_ReturnType Result)

{
  if (Result == E_OK) {
    if (Mka_JobRegistry[jobId].JobId == jobId) {
      Mka_GaaPaeServerStatus[Mka_JobRegistry[jobId].PaeId]
          .ddServerInternalState = MKA_INTR_STATE_ICK_COMPLETED;
    } else {
      /*Do nothing*/
    }
  }
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Csm_CbkNotification_MkaCryptoSakDeriveJob      **
** Service ID           : 0x23                                                **
** Description          : Callback to notify that EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa has finished.                                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Result: E_OK: EthIf_MacSecAddRxSa or EthIf_SwitchMacSecAddRxSa has
finished and Reception Secure Association is created E_NOT_OK:The Reception
Secure Association is not created through EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_Csm_CbkNotification_MkaCryptoHashKey256DerivationJob(
    const uint32 jobId, Std_ReturnType Result)

{
  if (Result == E_OK) {
    if (Mka_JobRegistry[jobId].JobId == jobId) {
      Mka_GaaPaeServerStatus[Mka_JobRegistry[jobId].PaeId]
          .ddServerInternalState = MKA_INTR_STATE_ICK_COMPLETED;
    } else {
      /*Do nothing*/
    }
  }
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Csm_CbkNotification_MkaCryptoSakDeriveJob      **
** Service ID           : 0x23                                                **
** Description          : Callback to notify that EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa has finished.                                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Result: E_OK: EthIf_MacSecAddRxSa or EthIf_SwitchMacSecAddRxSa has
finished and Reception Secure Association is created E_NOT_OK:The Reception
Secure Association is not created through EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_Csm_CbkNotification_MkaCryptoRandomJob(const uint32 jobId,
                                                Std_ReturnType Result)

{
  uint8 Luscount;
  if (Result == E_OK) {
/*Rationale for using loop for updation of state during and notification invocation by CSM because CP_SWS_Mka_00006*/
 for(Luscount = MKA_ZERO; Luscount < MKA_SEVEN; Luscount++ )
 {
    if ((Mka_JobRegistry[Luscount].JobId == jobId) && (Mka_JobRegistry[Luscount].status == MKA_TRUE))
     {
        if( Mka_JobRegistyCounter[Mka_JobRegistry[Luscount].PaeId] !=MKA_ZERO)
        {
          Mka_JobRegistyCounter[Mka_JobRegistry[Luscount].PaeId]--;
          Mka_JobRegistry[Luscount].status = MKA_FALSE;

          if( Mka_JobRegistyCounter[Mka_JobRegistry[Luscount].PaeId] == MKA_ZERO)
          {
        	      MKa_GaaKayStatus[Luscount]  = MKA_INTR_RANDOM_COMPLETE;;
          }
        }
      }
        
    
    } 
  }
    else {
      /*
        For the Retry Mechanism 
        Mka_JobRegistry[Luscount].status  == MKA_FALSE;
      */
    }
  

}
#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Csm_CbkNotification_MkaCryptoKeyWrapJob      **
** Service ID           : 0x23                                                **
** Description          : Callback to notify that EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa has finished.                                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Result: E_OK: EthIf_MacSecAddRxSa or EthIf_SwitchMacSecAddRxSa has
finished and Reception Secure Association is created E_NOT_OK:The Reception
Secure Association is not created through EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_Csm_CbkNotification_MkaCryptoKeyWrapJob(const uint32 jobId,
                                                 Std_ReturnType Result)

{
  if (Result == E_OK) {
    if (Mka_JobRegistry[jobId].JobId == jobId) {
      Mka_GaaPaeServerStatus[Mka_JobRegistry[jobId].PaeId]
          .ddServerInternalState = MKA_INTR_STATE_ICK_COMPLETED;
    } else {
      /*Do nothing*/
    }
  }
}

#define MKA_STOP_SEC_CODE
#include "Mka_MemMap.h"

/*******************************************************************************
** Function Name        : Mka_Csm_CbkNotification_MkaCryptoKeyUnwrapJob      **
** Service ID           : 0x23                                                **
** Description          : Callback to notify that EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa has finished.                                       **
** Sync/Async           : Synchronous                                         **
** Reentrancy           : Reentrant for different MkaPaeIdx, Non reentrant for
the same MkaPaeIdx                                           **
** Input Parameters     : MkaPaeIdx: Index of the PAE within the context of the
MKA module Result: E_OK: EthIf_MacSecAddRxSa or EthIf_SwitchMacSecAddRxSa has
finished and Reception Secure Association is created E_NOT_OK:The Reception
Secure Association is not created through EthIf_MacSecAddRxSa or
EthIf_SwitchMacSecAddRxSa
** InOut parameter      : None                                                **
** Output Parameters    : None                                      **
** Return parameter     : None                                     **
** Preconditions        : None                                                **
** Remarks              : Global Variable(s)     : None                       **
**                        Function(s) invoked    : None                       **
** Design_ID            : ,                           **
*******************************************************************************/
#define MKA_START_SEC_CODE
#include "Mka_MemMap.h"
void Mka_Csm_CbkNotification_MkaCryptoKeyUnwrapJob(const uint32 jobId,
                                                   Std_ReturnType Result)

{
  if (Result == E_OK) {
    if (Mka_JobRegistry[jobId].JobId == jobId) {
      Mka_GaaPaeServerStatus[Mka_JobRegistry[jobId].PaeId]
          .ddServerInternalState = MKA_INTR_STATE_ICK_COMPLETED;
    } else {
      /*Do nothing*/
    }
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
