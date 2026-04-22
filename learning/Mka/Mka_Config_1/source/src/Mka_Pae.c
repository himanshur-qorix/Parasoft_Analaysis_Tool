/*******************************************************************************
**                       Qorix Technologies Limited **
**                                                                            **
** Qorix Technologies Limited owns all the rights to this work. This **
** work shall not be copied,  reproduced,  used,  modified or its information **
** disclosed without the prior written authorization of Qorix Technologies **
** Limited.                                                                   **
**                                                                            **
**  SRC-MODULE:  Mka_pae.c **
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
**  12-Sep-25     Samarth G           Initial Version R24-11	                **

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

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/
/* #ref Crc_c_REF_3 */
/* PRQA S 2109 EOF*/
/*******************************************************************************
**                      Function Definitions                                  **
*******************************************************************************/

/* parasoft-end-suppress MISRAC2012-RULE_20_1-a-4 */
/* parasoft-end-suppress MISRAC2012-RULE_1_1-c-2 */
/* parasoft-end-suppress CERT_C-DCL19-a-3 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 MISRAC2012-RULE_5_4-d-2 */
/* parasoft-end-suppress CERT_C-DCL00-b-3 */
/*******************************************************************************
**                          End of File                                       **
*******************************************************************************/
