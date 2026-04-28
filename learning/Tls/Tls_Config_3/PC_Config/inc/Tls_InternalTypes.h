/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_InternalTypes.h                                            **
**                                                                            **
** TARGET    : All                                                            **
**                                                                            **
** PRODUCT   : AUTOSAR Transport Layer Security                               **
**                                                                            **
** PURPOSE   : Provision of module header                                     **
**                                                                            **
** PLATFORM DEPENDANT [yes/no]: no                                            **
**                                                                            **
** TO BE CHANGED BY USER [yes/no]: no                                         **
**                                                                            **
** Design ID  : TLS_SDD_0302                                                  **
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

#ifndef TLS_INTERNALTYPES_H
#define TLS_INTERNALTYPES_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Tls.h"
#include "TcpIp.h"
#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
#include "Rte_KeyM_Type.h"
#endif

/*************************Design ID: TLS_SDD_0454****************************/
#define TLS_TRUE (boolean)0x01U
/*************************Design ID: TLS_SDD_0455****************************/
#define TLS_FALSE (boolean)0x00U

/*************************Design ID: TLS_SDD_0456****************************/
/* Macro for invalid string length */
#define TLS_INVALID_STRING_LEN                     0x1U
/* Design ID : TLS_SDD_0458 */
#define TLS_INVALID_WORD                               0xFFFFFFFFU
/* Design ID : TLS_SDD_0459 */
#define TLS_INVALID_SHORT                              0xFFFFU
/* Design ID : TLS_SDD_0460 */
#define TLS_DEFAULT_VALUE                              0xFFFFU

/* Common macro values */
/* Design ID : TLS_SDD_0462 */
#define TLS_ZERO         0x00u
/* Design ID : TLS_SDD_0463 */
#define TLS_ONE                                     0x01u
/* Design ID : TLS_SDD_0464 */
#define TLS_TWO                                     0x02u
/* Design ID : TLS_SDD_0465 */
#define TLS_THREE                                   0x03u
/* Design ID : TLS_SDD_0466 */
#define TLS_FOUR                                    0x04u
/* Design ID : TLS_SDD_0467 */
#define TLS_FIVE                                    0x05u
/* Design ID : TLS_SDD_0468 */
#define TLS_SIX                                 0x06u
/* Design ID : TLS_SDD_0469 */
#define TLS_SEVEN                             0x07u
/* Design ID : TLS_SDD_0470 */
#define TLS_EIGHT                             0x08u
/* Design ID : TLS_SDD_0471 */
#define TLS_NINE                             0x09u
/* Design ID : TLS_SDD_0472 */
#define TLS_TEN                             0x0Au
/* Design ID : TLS_SDD_0473 */
#define TLS_ELEVEN                           0x0Bu
/* Design ID : TLS_SDD_0474 */
#define TLS_TWELVE                             0x0Cu
/* Design ID : TLS_SDD_0475 */
#define TLS_THIRTEEN                             0x0Du
/* Design ID : TLS_SDD_0527 */
#define TLS_FOURTEEN                             0x0Eu

/* Design ID : TLS_SDD_0476 */
#define TLS_SIXTEEN                             0x10u
/* Design ID : TLS_SDD_0749 */
#define TLS_EIGHTEEN                            0x12u
/* Design ID : TLS_SDD_0478 */
#define TLS_TWENTYFOUR                               0x18u // 24
/* Design ID : TLS_SDD_XXXX */
#define TLS_TWENTYSIX                               0x1Au // 26
/* Design ID : TLS_SDD_0479 */
#define TLS_USED_KEYS                               0x1Cu
/* Design ID : TLS_SDD_0480 */
#define TLS_THIRTYTWO                               0x20u
/* Design ID : TLS_SDD_0481 */
#define TLS_THIRTYTHREE                              0x21u
/* Design ID : TLS_SDD_0482 */
#define TLS_THIRTYFOUR                              0x22u
/* Design ID : TLS_SDD_0483 */
#define TLS_THIRTYNINE                              0x27u // 39
/* Design ID : TLS_SDD_0484 */
#define TLS_FOURTY                                  0x28u
/* Design ID : TLS_SDD_0486 */
#define TLS_FOURTYONE                               0x29u // 41
/* Design ID : TLS_SDD_0485 */
#define TLS_FOURTYTHREE                             0x2Bu // 43
/* Design ID : TLS_SDD_0487 */
#define TLS_FOURTYFOUR                              0x2Cu // 44
/* Design ID : TLS_SDD_0488 */
#define TLS_FOURTYFIVE                              0x2Du // 45
/* Design ID : TLS_SDD_0489 */
#define TLS_FOURTYSIX                               0x2Eu // 46
/* Design ID : TLS_SDD_0490 */
#define TLS_FOURTYSEVEN                             0x2Fu // 47
/* Design ID : TLS_SDD_0491 */
#define TLS_FOURTYEIGHT                             0x30u // 48
/* Design ID : TLS_SDD_0492 */
#define TLS_FOURTYNINE                              0x31u // 49
/* Design ID : TLS_SDD_0493 */
#define TLS_FIFTY                              0x32u // 50
/* Design ID : TLS_SDD_0494 */
#define TLS_MIN_PSK_EXT_LEN                              0x33u // 51
/* Design ID : TLS_SDD_0495 */
#define TLS_FIFTYTHREE                                0x35u // 53
/* Design ID : TLS_SDD_0496 */
#define TLS_FIFTYFOUR                                0x36u // 54
/* Design ID : TLS_SDD_0497 */
#define TLS_FIFTYSIX                                0x38u // 56
/* Design ID : TLS_SDD_0498 */
#define TLS_FIFTYNINE                                0x3Bu // 59
/* Design ID : TLS_SDD_0499 */
#define TLS_SIXTYEIGHT                                0x44u // 68
/* Design ID : TLS_SDD_0500 */
#define TLS_MAX_LABEL_SIZE                            0x46u // 70
/* Design ID : TLS_SDD_0501 */
#define TLS_MAX_CONTEXTSTRING_SIZE                    0x21 // 33
/* Design ID : TLS_SDD_0502 */
#define TLS_MINMUM_SIZE_OF_CH                    0x69u // 105
/* Design ID : TLS_SDD_0503 */
#define TLS_MINIMUM_SIZE_OF_SH                    0x34u // 52
/* Design ID : TLS_SDD_0504 */
#define TLS_SIZEOF_TEXT_TO_HEX_ARR                0x80u // 128
/* Design ID : TLS_SDD_0505 */
#define TLS_SIZEOF_HEX_TO_TEXT_ARR     0x0100u // 256
/* Design ID : TLS_SDD_0506 */
#define TLS_SIZEOF_BINDER_ARR                    0xFFu // 255
/* Design ID : TLS_SDD_0507 */
#define TLS_MAX_SIZE_255                            0xFFu // 255
/* Design ID : TLS_SDD_0508 */
#define TLS_MAX_SIZE_512                            0x0200u // 512
/* Design ID : TLS_SDD_0509 */
#define TLS_INVALID_CONNECTION                       0xFFu // 255
/* Design ID : TLS_SDD_0510 */
#define TLS_MAX_LEN_OF_PSK_IDEN                     0x20u // 32 size
/* Design ID : TLS_SDD_0512 */
#define TLS_MAX_RECORD_LEN                           0x4000U
/* Design ID : TLS_SDD_0513 */
#define TLS_MAX_CIPHER_RECORD_LEN                    0x4100U
/* Design ID : TLS_SDD_0514 */
/* assumeing Max RSA key size will be (4096 bit)*/
#define TLS_MAX_CERTVERIFY_LEN                    0x0200U
/* Design ID : TLS_SDD_0515 */
#define TLS_MAX_HASH_CERTIFICATE_VERIFY     0x92u // 146
/* Design ID : TLS_SDD_0517 */
/* max boundry condition of a PSK extension len */
#define TLS_MAX_LEN_OF_PSK_EXT                        0x02E8U // 744
/* Design ID : TLS_SDD_0516 */
#define TLS_MIN_LEN_OF_PSK_EXT                        0x33U // 51 aasuming min charcter in a PSK id is 8
/* Mask */
/* Design ID : TLS_SDD_0519 */
#define TLS_LSB_MASK                                0x000000FFU
/* Design ID : TLS_SDD_0520 */
#define TLS_LSB_MASK_U16                            0x0000FFFFU
/* Design ID : TLS_SDD_0521 */
#define TLS_U16_LSB_MASK                            0x00FFU
/* Design ID : TLS_SDD_0522 */
#define TLS_RECORD_SEQ_NUM_MAX                            0xFFFFFFFFFFFFFFFFU
/* Design ID : TLS_SDD_0524 */
/* Key Share Macro */
#define TLS_MIN_SH_KEYSHARE_LEN                             0x24u // 36
/* Design ID : TLS_SDD_0523 */
#define TLS_MAX_SH_KEYSHARE_LEN                             0x89u // 137
/* Design ID : TLS_SDD_0529 */
/* Supported Grps Length */
#define TLS_SECP256R1_SIZE                          0x41u // 65
/* Design ID : TLS_SDD_0530 */
#define TLS_SECP384R1_SIZE                          0x61u // 97
/* Design ID : TLS_SDD_0531 */
#define TLS_SECP521R1_SIZE                          0x85u // 133
/* Design ID : TLS_SDD_0532 */
#define TLS_X25519_SIZE                          0x20u // 32
/* Design ID : TLS_SDD_0750 */
#define TLS_X448_SIZE                          0x38u // 56

/* Design ID : TLS_SDD_0550 */
/* since the larget cipher size is TLS_AES_256_GCM_SHA384, 48 bytes */
#define TLS_MAX_HASH_SIZE                           0x30U
/* Design ID : TLS_SDD_0545 */
#ifndef TLS_UNUSED       /* Check for unused parameter warnings*/
#define TLS_UNUSED(variable) (void) (variable)
#endif
/* Design ID : TLS_SDD_0546 */
/* Maximum Key Share length */
#define TLS_MAX_KEY_SHARE_SIZE                           0x85U
/* Design ID : TLS_SDD_0547 */
#define TLS_MAX_SG_SH_KS_LEN                           0xFFFEu
/* Design ID : TLS_SDD_0649 */
#define TLS_TOTAL_SG_RFC                           0x010Fu // total 271 grps in rfc ,542
/* Design ID : TLS_SDD_0548 */
#define TLS_CERT_OFFSET_SIZE                           0x0400u //1024
/* Design ID : TLS_SDD_0690, TLS_SDD_0691, TLS_SDD_0692, TLS_SDD_0693, TLS_SDD_0694, TLS_SDD_0695, TLS_SDD_0696, TLS_SDD_0697, TLS_SDD_0698, TLS_SDD_0699, TLS_SDD_0700, TLS_SDD_0701, TLS_SDD_0702, TLS_SDD_0703, TLS_SDD_0704, TLS_SDD_0705, TLS_SDD_0706 */


/* Look up table for TEXT to HEX  */
#define TLS_HEX_00  (uint8)0x00u
#define TLS_HEX_01  (uint8)0x01u
#define TLS_HEX_02  (uint8)0x02u
#define TLS_HEX_03  (uint8)0x03u
#define TLS_HEX_04  (uint8)0x04u
#define TLS_HEX_05  (uint8)0x05u
#define TLS_HEX_06  (uint8)0x06u
#define TLS_HEX_07  (uint8)0x07u
#define TLS_HEX_08  (uint8)0x08u
#define TLS_HEX_09  (uint8)0x09u
#define TLS_HEX_0A  (uint8)0x0Au
#define TLS_HEX_0B  (uint8)0x0Bu
#define TLS_HEX_0C  (uint8)0x0Cu
#define TLS_HEX_0D  (uint8)0x0Du
#define TLS_HEX_0E  (uint8)0x0Eu
#define TLS_HEX_0F  (uint8)0x0Fu
/* Design ID : TLS_SDD_0671, TLS_SDD_0672, TLS_SDD_0673, TLS_SDD_0674, TLS_SDD_0675, TLS_SDD_0676, TLS_SDD_0677, TLS_SDD_0680, TLS_SDD_0681, TLS_SDD_0682, TLS_SDD_0683, TLS_SDD_0684, TLS_SDD_0685, TLS_SDD_0686, TLS_SDD_0687, TLS_SDD_0688, TLS_SDD_0689 */
#define TLS_HEX_10  (uint8)0x10u
#define TLS_HEX_11  (uint8)0x11u
#define TLS_HEX_12  (uint8)0x12u
#define TLS_HEX_13  (uint8)0x13u
#define TLS_HEX_14  (uint8)0x14u
#define TLS_HEX_15  (uint8)0x15u
#define TLS_HEX_16  (uint8)0x16u
#define TLS_HEX_17  (uint8)0x17u
#define TLS_HEX_18  (uint8)0x18u
#define TLS_HEX_19  (uint8)0x19u
#define TLS_HEX_1A  (uint8)0x1Au
#define TLS_HEX_1B  (uint8)0x1Bu
#define TLS_HEX_1C  (uint8)0x1Cu
#define TLS_HEX_1D  (uint8)0x1Du
#define TLS_HEX_1E  (uint8)0x1Eu
#define TLS_HEX_1F  (uint8)0x1Fu
/* Design ID : TLS_SDD_0655, TLS_SDD_0656, TLS_SDD_0657, TLS_SDD_0658, TLS_SDD_0659, TLS_SDD_0660, TLS_SDD_0661, TLS_SDD_0662, TLS_SDD_0663, TLS_SDD_0664, TLS_SDD_0665, TLS_SDD_0666, TLS_SDD_0667, TLS_SDD_0668, TLS_SDD_0669, TLS_SDD_0670 */
#define TLS_HEX_20  (uint8)0x20u
#define TLS_HEX_21  (uint8)0x21u
#define TLS_HEX_22  (uint8)0x22u
#define TLS_HEX_23  (uint8)0x23u
#define TLS_HEX_24  (uint8)0x24u
#define TLS_HEX_25  (uint8)0x25u
#define TLS_HEX_26  (uint8)0x26u
#define TLS_HEX_27  (uint8)0x27u
#define TLS_HEX_28  (uint8)0x28u
#define TLS_HEX_29  (uint8)0x29u
#define TLS_HEX_2A  (uint8)0x2Au
#define TLS_HEX_2B  (uint8)0x2Bu
#define TLS_HEX_2C  (uint8)0x2Cu
#define TLS_HEX_2D  (uint8)0x2Du
#define TLS_HEX_2E  (uint8)0x2Eu
#define TLS_HEX_2F  (uint8)0x2Fu
/* Design ID : TLS_SDD_0639, TLS_SDD_0640, TLS_SDD_0641, TLS_SDD_0642, TLS_SDD_0643, TLS_SDD_0644, TLS_SDD_0645, TLS_SDD_0646, TLS_SDD_0647, TLS_SDD_0648, TLS_SDD_0649, TLS_SDD_0650, TLS_SDD_0651, TLS_SDD_0652, TLS_SDD_0653, TLS_SDD_0654 */
#define TLS_HEX_30  (uint8)0x30u
#define TLS_HEX_31  (uint8)0x31u
#define TLS_HEX_32  (uint8)0x32u
#define TLS_HEX_33  (uint8)0x33u
#define TLS_HEX_34  (uint8)0x34u
#define TLS_HEX_35  (uint8)0x35u
#define TLS_HEX_36  (uint8)0x36u
#define TLS_HEX_37  (uint8)0x37u
#define TLS_HEX_38  (uint8)0x38u
#define TLS_HEX_39  (uint8)0x39u
#define TLS_HEX_3A  (uint8)0x3Au
#define TLS_HEX_3B  (uint8)0x3Bu
#define TLS_HEX_3C  (uint8)0x3Cu
#define TLS_HEX_3D  (uint8)0x3Du
#define TLS_HEX_3E  (uint8)0x3Eu
#define TLS_HEX_3F  (uint8)0x3Fu
/* Design ID : TLS_SDD_0638, TLS_SDD_0637, TLS_SDD_0636, TLS_SDD_0635, TLS_SDD_0612, TLS_SDD_0582, TLS_SDD_0551, TLS_SDD_0552, TLS_SDD_0553, TLS_SDD_0554, TLS_SDD_0555, TLS_SDD_0556, TLS_SDD_0557, TLS_SDD_0558, TLS_SDD_0559, TLS_SDD_0560 */
#define TLS_HEX_40  (uint8)0x40u
#define TLS_HEX_41  (uint8)0x41u
#define TLS_HEX_42  (uint8)0x42u
#define TLS_HEX_43  (uint8)0x43u
#define TLS_HEX_44  (uint8)0x44u
#define TLS_HEX_45  (uint8)0x45u
#define TLS_HEX_46  (uint8)0x46u
#define TLS_HEX_47  (uint8)0x47u
#define TLS_HEX_48  (uint8)0x48u
#define TLS_HEX_49  (uint8)0x49u
#define TLS_HEX_4A  (uint8)0x4Au
#define TLS_HEX_4B  (uint8)0x4Bu
#define TLS_HEX_4C  (uint8)0x4Cu
#define TLS_HEX_4D  (uint8)0x4Du
#define TLS_HEX_4E  (uint8)0x4Eu
#define TLS_HEX_4F  (uint8)0x4Fu
/* Design ID : TLS_SDD_0561, TLS_SDD_0562, TLS_SDD_0563, TLS_SDD_0564, TLS_SDD_0565, TLS_SDD_0566, TLS_SDD_0567, TLS_SDD_0568, TLS_SDD_0569, TLS_SDD_0570, TLS_SDD_0571, TLS_SDD_0572, TLS_SDD_0573, TLS_SDD_0574, TLS_SDD_0575, TLS_SDD_0576 */
#define TLS_HEX_50  (uint8)0x50u
#define TLS_HEX_51  (uint8)0x51u
#define TLS_HEX_52  (uint8)0x52u
#define TLS_HEX_53  (uint8)0x53u
#define TLS_HEX_54  (uint8)0x54u
#define TLS_HEX_55  (uint8)0x55u
#define TLS_HEX_56  (uint8)0x56u
#define TLS_HEX_57  (uint8)0x57u
#define TLS_HEX_58  (uint8)0x58u
#define TLS_HEX_59  (uint8)0x59u
#define TLS_HEX_5A  (uint8)0x5Au
#define TLS_HEX_5B  (uint8)0x5Bu
#define TLS_HEX_5C  (uint8)0x5Cu
#define TLS_HEX_5D  (uint8)0x5Du
#define TLS_HEX_5E  (uint8)0x5Eu
#define TLS_HEX_5F  (uint8)0x5Fu
/* Design ID : TLS_SDD_0577, TLS_SDD_0578, TLS_SDD_0579, TLS_SDD_0580, TLS_SDD_0581, TLS_SDD_0582, TLS_SDD_0583, TLS_SDD_0584, TLS_SDD_0585, TLS_SDD_0586, TLS_SDD_0587, TLS_SDD_0588, TLS_SDD_0589, TLS_SDD_0590, TLS_SDD_0591, TLS_SDD_0592 */
#define TLS_HEX_60  (uint8)0x60u
#define TLS_HEX_61  (uint8)0x61u
#define TLS_HEX_62  (uint8)0x62u
#define TLS_HEX_63  (uint8)0x63u
#define TLS_HEX_64  (uint8)0x64u
#define TLS_HEX_65  (uint8)0x65u
#define TLS_HEX_66  (uint8)0x66u
#define TLS_HEX_67  (uint8)0x67u
#define TLS_HEX_68  (uint8)0x68u
#define TLS_HEX_69  (uint8)0x69u
#define TLS_HEX_6A  (uint8)0x6Au
#define TLS_HEX_6B  (uint8)0x6Bu
#define TLS_HEX_6C  (uint8)0x6Cu
#define TLS_HEX_6D  (uint8)0x6Du
#define TLS_HEX_6E  (uint8)0x6Eu
#define TLS_HEX_6F  (uint8)0x6Fu
/* Design ID : TLS_SDD_0593, TLS_SDD_0594, TLS_SDD_0595, TLS_SDD_0596, TLS_SDD_0597, TLS_SDD_0598, TLS_SDD_0599, TLS_SDD_0600, TLS_SDD_0601, TLS_SDD_0602, TLS_SDD_0603, TLS_SDD_0604, TLS_SDD_0605, TLS_SDD_0606, TLS_SDD_0607, TLS_SDD_0608 */
#define TLS_HEX_70  (uint8)0x70u
#define TLS_HEX_71  (uint8)0x71u
#define TLS_HEX_72  (uint8)0x72u
#define TLS_HEX_73  (uint8)0x73u
#define TLS_HEX_74  (uint8)0x74u
#define TLS_HEX_75  (uint8)0x75u
#define TLS_HEX_76  (uint8)0x76u
#define TLS_HEX_77  (uint8)0x77u
#define TLS_HEX_78  (uint8)0x78u
#define TLS_HEX_79  (uint8)0x79u
#define TLS_HEX_7A  (uint8)0x7Au
#define TLS_HEX_7B  (uint8)0x7Bu
#define TLS_HEX_7C  (uint8)0x7Cu
#define TLS_HEX_7D  (uint8)0x7Du
#define TLS_HEX_7E  (uint8)0x7Eu
#define TLS_HEX_7F  (uint8)0x7Fu
/* Design ID : TLS_SDD_0609, TLS_SDD_0610, TLS_SDD_0611, TLS_SDD_0612, TLS_SDD_0613, TLS_SDD_0614, TLS_SDD_0615, TLS_SDD_0616, TLS_SDD_0617, TLS_SDD_0618, TLS_SDD_0619, TLS_SDD_0620, TLS_SDD_0621, TLS_SDD_0622, TLS_SDD_0623, TLS_SDD_0624, TLS_SDD_0625, TLS_SDD_0626, TLS_SDD_0627, TLS_SDD_0628, TLS_SDD_0629, TLS_SDD_0630, TLS_SDD_0631, TLS_SDD_0632, TLS_SDD_0633, TLS_SDD_0634, TLS_SDD_0457, TLS_SDD_0461, TLS_SDD_0528, TLS_SDD_0526, TLS_SDD_0533, TLS_SDD_0525  */


/* emptuy hash macros */
#define TLS_HEX_80  (uint8)0x80u
#define TLS_HEX_81  (uint8)0x81u
#define TLS_HEX_83  (uint8)0x83u
#define TLS_HEX_87  (uint8)0x87u
#define TLS_HEX_93  (uint8)0x93u
#define TLS_HEX_95  (uint8)0x95u
#define TLS_HEX_96  (uint8)0x96u
#define TLS_HEX_98  (uint8)0x98u
#define TLS_HEX_99  (uint8)0x99u
#define TLS_HEX_9A  (uint8)0x9Au
#define TLS_HEX_9B  (uint8)0x9Bu
#define TLS_HEX_9C  (uint8)0x9Cu
#define TLS_HEX_A4  (uint8)0xA4u
#define TLS_HEX_AE  (uint8)0xAEu
#define TLS_HEX_B0  (uint8)0xB0u
#define TLS_HEX_B8  (uint8)0xB8u
#define TLS_HEX_B9  (uint8)0xB9u
#define TLS_HEX_C4  (uint8)0xC4u
#define TLS_HEX_C6  (uint8)0xC6u
#define TLS_HEX_C8  (uint8)0xC8u
#define TLS_HEX_D4  (uint8)0xD4u
#define TLS_HEX_D8  (uint8)0xD8u
#define TLS_HEX_DA  (uint8)0xDAu
#define TLS_HEX_E1  (uint8)0xE1u
#define TLS_HEX_E3  (uint8)0xE3u
#define TLS_HEX_E4  (uint8)0xE4u
#define TLS_HEX_F4  (uint8)0xF4u
#define TLS_HEX_FC  (uint8)0xFCu
#define TLS_HEX_FB  (uint8)0xFBu



#define TLS_HEX_86        ((uint8)0x86u)
#define TLS_HEX_F7        ((uint8)0xF7u)

/* TLS PSK Exchange mode supported */
/*************************Design ID: TLS_SDD_0403****************************/
typedef enum ETag_Tls_PSKExModeType
{
 TLS_PSK_KE = 0x00,
 TLS_PSK_DHE_KE = 0x01
}Tls_PSKExModeType;

/* TLS versions supported */
/*************************Design ID: TLS_SDD_0404****************************/
typedef enum ETag_Tls_VersionType
{
 TLS_SSL_VERSION_V30 = 0x0300,
 TLS_VERSION_V10 = 0x0301,
 TLS_VERSION_V12 = 0x0303,
 TLS_VERSION_V13 = 0x0304
}Tls_VersionType;
/* Design ID : TLS_SDD_0414 */
/* List of cyphersuites supported as per TLS 1.3 */
typedef enum ETag_Tls_CipherSuiteType
{
 TLS_AES_128_GCM_SHA256              = 0x1301,
 TLS_AES_256_GCM_SHA384              = 0x1302,
 TLS_CHACHA20_POLY1305_SHA256        = 0x1303,
 TLS_AES_128_CCM_SHA256              = 0x1304,
 TLS_AES_128_CCM_8_SHA256            = 0x1305
}Tls_CipherSuiteType;

/* Design ID : TLS_SDD_0407 */
typedef enum ETag_Tls_HandShakeProtocolType
{
 TLS_CLIENT_HELLO                   = 0x01, 
 TLS_SERVER_HELLO                   = 0x02, 
 TLS_NEW_SESSION_TICKET             = 0x04, 
 TLS_END_OF_EARLY_DATA              = 0x05, 
 TLS_ENCRYPTED_EXTENSIONS           = 0x08, 
 TLS_CERTIFICATE                    = 0x0B,  
 TLS_CERTIFICATE_REQUEST            = 0x0D, 
 TLS_CERTIFICATE_VERIFY             = 0x0F, 
 TLS_FINISHED                       = 0x14, 
 TLS_UNKNOWN_HANDSHAKE              = 0xFF
}Tls_HandShakeProtocolType;
/* Design ID : TLS_SDD_0409 */
typedef struct STag_Tls_HandShakeMessage
{
 /* Handshake message type */
 Tls_HandShakeProtocolType ddHandShakeType;
 
 /* Remaining bytes of data present in handshake message */
 uint32 ulLengthOfHandShakeMsg;
 
 /* Data Frame of Handshake Message */
 uint32*  pHandShakeMessageData;
}Tls_HandShakeMessage;

/* Design ID : TLS_SDD_0410 */
/* Enum types of severity level of the alert message */
typedef enum ETag_Tls_AlertLevel
{
 TLS_WARNING         = 0x01, 
 TLS_FATAL           = 0x02, 
 TLS_UNKNOWN_ALERT   = 0xFF
}Tls_AlertLevel;
/* Design ID : TLS_SDD_0411 */
/* Enum to hold types of descriptions of the alert message */
typedef enum ETag_Tls_AlertDescription
{
 TLS_CLOSE_NOTIFY                     = 0x00,
 TLS_UNEXPECTED_MESSAGE               = 0x0A,
 TLS_BAD_RECORD_MAC                   = 0x14,
 TLS_RECORD_OVERFLOW                  = 0x16,
 TLS_HANDSHAKE_FAILURE                = 0x28,
 TLS_BAD_CERTIFICATE                  = 0x2A,
 TLS_UNSUPPORTED_CERTIFICATE          = 0x2B,
 TLS_CERTIFICATE_REVOKED              = 0x2C,
 TLS_CERTIFICATE_EXPIRED              = 0x2D,
 TLS_CERTIFICATE_UNKNOWN              = 0x2E,
 TLS_ILLEGAL_PARAMETER                = 0x2F,
 TLS_UNKNOWN_CA                       = 0x30,
 TLS_ACCESS_DENIED                    = 0x31,
 TLS_DECODE_ERROR                     = 0x32,
 TLS_DECRYPT_ERROR                    = 0x33,
 TLS_PROTOCOL_VERSION                 = 0x46,
 TLS_INSUFFICIENT_SECURITY            = 0x37,
 TLS_INTERNAL_ERROR                   = 0x50,
 TLS_INAPPROPRIATE_FALLBACK           = 0x56,
 TLS_USER_CANCELED                    = 0x5A,
 TLS_MISSING_EXTENSION                = 0x6D,
 TLS_UNSUPPORTED_EXTENSION            = 0x6E,
 TLS_UNRECOGNIZED_NAME                = 0x70,
 TLS_BAD_CERTIFICATE_STATUS_RESPONSE  = 0x71,
 TLS_UNKNOWN_PSK_IDENTITY             = 0x73,
 TLS_CERTIFICATE_REQUIRED             = 0x74,
 TLS_NO_APPLICATION_PROTOCOL          = 0x78,
 TLS_UNKNOWN_ALERT_DESCRIPTION        = 0xFF
}Tls_AlertDescription;
/* Design ID : TLS_SDD_0412 */
/* Alert Protocol message format */
typedef struct STag_Tls_Alert
{
 /* Conveys the severity level of the alert message  */
 Tls_AlertLevel ddLevel;
 /* Conveys the description of the alert message*/
 Tls_AlertDescription ddDescription;
}Tls_Alert;

/* Design ID : TLS_SDD_0413 */
/* Extensions supported by the Client Hello Handshake and SERVER hello handshake 
Protocol */
typedef enum ETag_Tls_ExtType
{
 /* Mandatory Extensions */
 TLS_SUPPORTED_VERSIONS          = 0x002B,
 
 /* Mandatory Extensions while Considering Certificate */ 
 TLS_CH_SUPPORTED_GROUPS                                    = 0x000A,
 TLS_CH_SIGNATURE_ALGORITHMS                                = 0x000D,
 TLS_CH_KEY_SHARE                                           = 0x0033,  
 
 /* Mandatory Extensions while Considering PSK  */
 TLS_CH_PSK_KEY_EXCHANGE_MODES                              = 0x002D,
 TLS_PRE_SHARED_KEY                                       = 0x0029,
  
 TLS_CH_SERVER_NAME                                         = 0x0000,
 
 TLS_STATUS_REQUEST                                          = 0x0005,
 TLS_SIGNED_CERT_TIMESTAMP                                   = 0x0012,
 TLS_CERTIFICATE_AUTHORITIES                                 = 0x002F,
 TLS_OID_FILTERS                                             = 0x0030, 
 TLS_SIGNATURE_ALGORITHMS_CERT                               = 0x0032,
 /* Invalid Extension */
 TLS_CH_UNKNOWN_EXTENSION                                   = 0xFFFF  
}Tls_ExtType;

/* Design ID : TLS_SDD_0415 */
/* Extensions supported by the Encrypted Extension Handshake Protocol */
typedef enum ETag_Tls_EncryptedExtensionType
{
 TLS_EE_SUPPORTED_GROUPS                          = 0x000A,
 TLS_EE_SIGNATURE_ALGORITHMS                      = 0x000D,
 
 /* Invalid Extension */
 TLS_EE_UNKNOWN_EXTENSION                         = 0xFFFF  
}Tls_EncryptedExtensionType;

/* Design ID : TLS_SDD_0417 */
/* For Future use when supported */
typedef struct STag_Tls_CHSessionIdType
{
 /* Length of SessionId Data. */
 uint8*  pLengthofSessionId;
 
 /* Contains data of SessionId. */
 uint8*  pSessionIdData;
}Tls_CHSessionIdType;


/* Design ID : TLS_SDD_0420 */
typedef struct STag_Tls_QueueType
{ 
 /* */
 uint16  usQHead; 
 
 /* */
 uint16  usQTail;

 uint16  usTotlQData;
}Tls_QueueType;
/* Design ID : TLS_SDD_0422 */
#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
typedef enum ETag_Tls_VerifyStatusType
{
 TLS_IDLE          = 0x00,
 TLS_INVOKED,
 TLS_BUSY
}Tls_VerifyStatusType;
#endif


/* Design ID : TLS_SDD_0423 */
typedef struct STag_Tls_FrameType
{ 

    uint64 ulWriteSeqNum;
    uint64 ulReadSeqNum;    
    
    Tls_QueueType ddQueue;

#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
    /* Keym Verify Certificate Type*/
    Tls_VerifyStatusType ddVerifyCertState;
#endif
    /* Negotiated PSK csm Key Id  */
    uint32 ulNegotiatedPskCsmKeyId;
    
    /* Contains data of selected cipherSuite from server. */
    uint16*  pServerCipherSuite;
    
    /* The client shall provide 32 bytes of random data*/
    uint8*  pClientRandomNumber; 
    
    /* The session id is not supported however kept as zero-length vector,
        shall not support session resumption*/
    uint8*  pClientSessionId;
    
    /* The Server shall provide 32 bytes of random data*/
    uint8*  pServerRandomNumber; 

    /* The session id is not supported however kept as zero-length vector,
        shall not support session resumption*/
    uint8*  pServerSessionId;
    
    /* A single byte which MUST have the value 0.*/
    uint8* pServerCompressionMethod; 

    /* pinter to store the addr, which points to Tls_GaaHShakeBuffer */
    const uint8* pPatnerKeyShare;

    uint16 usProcessedHSBytes;
    
    /* Negotiated CipherSuite worker index  */
    uint16 usNegotiatedCiprSutIdx; 
    
    /* The last position in the buffer*/
    uint16 usHShakeBufferPosition;
    
    /* this is used to store the total Plain application data recived via Tx indication */
    uint16 usTotlAppData;
    
    uint16 usProcessedData;
    
    uint16 usUnProcessedData;

    uint16 usNegotiatedSupGrpIdx;

    uint16 usNegotiatedSigAlgoIdx;
    
    boolean blIsHSKeysDerived;
    
    boolean blIsAppKeysDerived;
    
    boolean blIsTxInProgress; 
    /* this flag indicates if close connection was called during HS COMPLETED STATE */
    boolean blIsCloseConnAfterHS;

	#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
		/* this is used , when a certificate Req HS msg is recived from server, and
		Client Needs to decide wether to Cert HS and Certificate Verify or not */
		boolean blIsCertReqReceived;
	#endif
}Tls_FrameType;



/* typdef for function pointer for callback of type
 <Up_TlsClientGetPskIdentity> */
typedef Std_ReturnType (*TlsPskGetClientKeyIdentityFuncpointerType) 
(
 TcpIp_TlsConnectionIdType TlsConnectionId,
 uint16 PskIdentityHintLength, // no use for current TLS
 const uint8* PskIdentityHintPtr, // no use for current TLS
 uint16* PskKeyIdentityLengthPtr,
 uint8* PskKeyIdentityPtr,
 uint32* CsmKeyId);
 
/* typdef for function pointer for callback of type
 <Up_TlsServerGetPskIdentity> */
typedef Std_ReturnType (*TlsPskGetServerKeyIdentityFuncpointerType) 
(
 TcpIp_TlsConnectionIdType TlsConnectionId,
 uint16 PskKeyIdentityLength,
 const uint8* PskKeyIdentityPtr,
 uint32* CsmKeyId);
 
/* typdef for function pointer for callback of type 
<Up_TlsServerGetPskIdentityHint> TlsPskGetKeyIdentityHintFunc  */
typedef Std_ReturnType (*TlsPskGetKeyIdentityHintFuncpointerType) 
(
 TcpIp_TlsConnectionIdType TlsConnectionId,
 uint16* IdentityHintLengthPtr,
 uint8* IdentityHintPtr);


/*************************Design ID: TLS_SDD_0426****************************/
typedef enum ETag_Tls_CsmPRFSupportType
{
 TLS_PRF_CSM_FULL_SUPPORT = 0U,
 TLS_PRF_CSM_INOUT_REDIRECT_SUPPORT,
 TLS_PRF_CSM_NO_SUPPORT,
 TLS_PRF_NO_SUPPORT,
 TLS_PRF_INVALID
}TlsCsmPRFSupportType;

/* Design ID: TLS_SDD_0153 */
typedef struct STag_Tls_Config
{
 /* Max. number of TLS connections simultaneously */
 uint16 usTlsMaxConnections;
}Tls_ConfigType;


/*************************Design ID: TLS_SDD_0429****************************/
typedef enum ETag_Tls_AuthType
{
 TLS_AUTH_PSK = 0U,
 TLS_AUTH_CERT = 2U
}Tls_AuthType;

/*************************Design ID: TLS_SDD_0402************************/
typedef struct STag_Tls_Connection
{
    /* Specifies if the TLS connection authenticates via Cert or PSK */
    Tls_AuthType ddTlsAuthType;
    
    /* Specifies if the TLS connection is a server or a client. */
    Tls_ConnectionType ddTlsConnectionType;
    
    /* Random generate Job Id */
    uint32 ulTlsCsmRndmGenJobRefId;
    
    #if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED) 
    /* points to certificate identity conatiner which is linked to this connection*/
        uint32 ulTlsCertIdnRefIdx;
    #endif

    uint16 usNumOfCiphrWrkrs;
    
    uint16 usCiphrWrkrConnIdx;
    
    /* Max length in bytes of a TLS fragment that is sent as a block. */
    uint16 usTlsMaxFragmentLen;

    uint16 usHSWrkrIdx;

    uint16 usNumOfHSWorkers;

    #if (STD_ON == TLS_TIMER_SUPPORT)  
        /* Waiting Time before Tls closes the Connection */ 
        uint16 usTlsWaitingTimeout;
    #endif 
    
    /* Identifier of the connection. */
    TcpIp_TlsConnectionIdType ddTlsConnectionId;
    
    #if (STD_ON == TLS_PSK_IDENTITY_SUPPORT_ENABLED)  
        /* this prameter exactly indexes the generated look up table 
        (Tls_GaaPSKLookUp)which says what all PSK conatiners are referred*/
        uint8 ucPSKConnLookUpIdx;
        
        /* numberof PSK referred(so that it can be used in Looping in static code)*/
        uint8 ucNumOfPreSharedKeys;
    #endif
    
    /* Defines if client authentication shall be applied for 
    this TLS connection. */
    boolean blTlsUseClintAuthReq;
 
    /* Defines if the security extension for max_fragment_length 
    shall be supported */
    boolean blTlsUseSecurityExtensionRecordSizeLimit;
    
    boolean blTlsUseExtensionCertificateStatusRequest;
    
    boolean usTlsUseExtensionMaxFragmentLength;
    
    boolean blTlsUseExtensionTrustedCAKeys;
    
    #if (STD_ON == TLS_GROUP_CONFIGURED)
        boolean blTlsIsGrpEnbld;
    #endif
  
}TlsConnectionStrType;

/*************************Design ID: TLS_SDD_0424************************/
#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
typedef struct STag_Tls_CertificateIdentity
{
	/* KeyM Local certificate container Index */
	KeyM_CertificateIdType ddTlsCipherKeyMLocalCertificateIdx;
	
	/* KeyM remote certificate container Index */
	KeyM_CertificateIdType ddTlsCipherKeyMRemoteCertificateIdx;
	
	/* KeyM certificate element container Index for Signature */
	KeyM_CertElementIdType ddTlsCipherKeyMRemoteCertElementIdx;

    /* KeyM certificate element container Index */
	KeyM_CertElementIdType ddTlsCipherKeyMRemoteCertElementPublicKeyIdx;

}Tls_CertificateIdentityType;
#endif

/*************************Design ID: TLS_SDD_0428****************************/
typedef enum ETag_Tls_PSKHashType
{
 TLS_SHA_256 = 0x20,  //32 bytes
 TLS_SHA_384 = 0x30   //48 bytes
}Tls_PSKHashType;
 
#if (STD_ON == TLS_PSK_IDENTITY_SUPPORT_ENABLED)
/*************************Design ID: TLS_SDD_0430************************/
typedef struct STag_Tls_PskIdentity
{
    /* specifies what Hash does the PSK support */
    Tls_PSKHashType ddTlsPSKHashType;
    
    /* Reference to a CSM key associated to the CSM job that performs the 
    PRF hash operation */
    uint32 ulTlsPSKCsmKeyRefId;
    
    /* Reference to a CSM key associated to the CSM job that performs the 
    PRF hash operation */
    uint32 ulTlsCsmBinderKeyRefId;
    
    /* func ptr for PskGetClientKeyIdentity */
    TlsPskGetClientKeyIdentityFuncpointerType TlsPskGetClientKeyIdentityFunc;
    
    /* func ptr for PskGetServerKeyIdentity */
    TlsPskGetServerKeyIdentityFuncpointerType TlsPskGetServerKeyIdentityFunc;
    
    /* func ptr for PskGetKeyIdentityHint TlsPskGetKeyIdentityHintFunc  */
    TlsPskGetKeyIdentityHintFuncpointerType TlsPskGetKeyIdentityHintFunc;  
 
    /* Length of aaPskIdentity Array */
    uint16 usTlsPskIdentityLen;
    
    /* Length of aaPskIdentityHint Array */
    uint16 usTlsPskIdentityHintLen;
    
    /* This item provides the key identification. The TLS client selects the 
    preshared key based on the identification hint provided by the server and
    returns the key identification name back to the server */
    const uint8 aaPskIdentity[TLS_PSK_IDENTITY_MAX_LEN];
    
    /*  Provides the identity hint for a pre-shared key. 
    This information is transmitted by the TLS Server to provide its 
    identification to the TLS client. The TLS client uses the same 
    information to select the pre-shared key. */
    /* Currently not used in Implementation , this will not come in TLS1.3*/
    const uint8 aaPresharedKeyIdentityHint[TLS_PSK_IDENTITY_HINT_MAX_LEN];
    
    /* defines if a psk has dynamic psk configured */
    boolean blIsDynPskAvail;
}Tls_PskIdentityType;
#endif

/*************************Design ID: TLS_SDD_0442************************/
typedef struct STag_Tls_CiphersuiteDefinition
{
   /* Declares the TLS version that this ciphersuite shall be used for */
   Tls_VersionType ddTlsVersion;
   
    /* ID that represents the ciphersuite according to IETF, e.g. RFC4492,Sect.6
   RFC8446, Appendix B.4 or RFC5246, Appendix A.5. */
   uint16 usTlsCiphersuiteId;
   
   /* Defines the priority of the cipher. The higher the number the 
   lower the priority. */
   uint16 usTlsCiphersuitePriority;
   
   /* Specifies the Length of TlsCiphersuiteName */
   uint8 ucTlsCiphersuiteNameLen;
   
   /* Specifies if the ciphersuite supports AEAD for data en-/decryption. */
   boolean blTlsUseAEADCipher;
   
   /* Defines if this ciphersuite uses pre-shared keys. If so, additional 
    configuration or callbacks will be used for pre-shared key negotiation.*/
   boolean blTlsUsePresharedKeys;
   
   /* Defines if the security extension according to IETF RFC 7366 shall be 
   supported. This is useful for ciphersuites using CBC mode */
   boolean blTlsUseSecurityExtensionForceEncryptThenMac;
   
}Tls_CiphersuiteDefinition;


/*************************Design ID: TLS_SDD_0443************************/
typedef struct STag_Tls_CiphersuiteWorker
{   

    /* Reference to a CSM job to perform the data decryption operation */
    uint32 ulTlsCipherCsmDecryptJobRefIdx;
    
    /* Reference to a CSM key associated to the CSM job that performs the 
    data decryption operation */
    uint32 ulTlsCipherCsmDecryptKeyRefIdx;
    
    /* Reference to a CSM job to perform the data decryption operation */
    uint32 ulTlsCsmClientApplTrafficSecretKeyRefIdx;
    
    /* Reference to a CSM key associated to the CSM job that performs the 
    data decryption operation */
    uint32 ulTlsCsmServerApplTrafficSecretKeyRefIdx;
    
    /* Reference to a CSM job to perform the data decryption operation */
    uint32 ulTlsCsmClientApplWriteKeyRefIdx;
    
    /* Reference to a CSM key associated to the CSM job that performs the 
    data decryption operation */
    uint32 ulTlsCsmServerApplWriteKeyRefIdx;

    /* Reference to a CSM job to perform the data decryption operation */
    uint32 ulTlsCsmClientApplWriteIVRefIdx;
    
    /* Reference to a CSM key associated to the CSM job that performs the 
    data decryption operation */
    uint32 ulTlsCsmServerApplWriteIVRefIdx;
    
    /* Reference to a CSM job to perform the data encryption operation */
    uint32 ulTlsCipherCsmEncryptJobRefIdx;
    
    /* Reference to a CSM key associated to the CSM job that performs the 
    data encryption operation */
    uint32 ulTlsCipherCsmEncryptKeyRefIdx;
    
    /* Reference to a CSM job to perform the MAC generate operation */
    uint32 ulTlsCipherCsmMacGenerateJobRefIdx;
    
    /* Reference to a CSM key associated to the CSM job that performs 
    the MAC generate operation */
    uint32 ulTlsCipherCsmMacGenerateKeyRefIdx;
 
    /* Reference to a CSM job to perform the MAC verify operation */
    uint32 ulTlsCipherCsmMacVerifyJobRefIdx;
    
    /* Reference to a CSM key associated to the CSM job that performs 
    the MAC verify operation */
    uint32 ulTlsCipherCsmMacVerifyKeyRefIdx;
    
    /* Reference to a a ciphersuite definition container */
    uint32 ulTlsCiphrSutDefRefIdx;
    
    /* Defines the key length for en- / decryption with authentication data 
    (AEAD). */
    uint16 usTlsCipherAEADCipherKeyLength;
    
    /* Defines the key length used for en- or decryption. 
    The key length is valid for (symmetric) encryption and decryption. */
    uint16 usTlsCipherEncryptKeyLength;
    
    /* Specifies the length of the MAC key */
    uint16 usTlsCipherMacKeyLength;
 
    /* BaseIndex of the corresponding Tls_ConnectionHandshakeRef configured 
    parameter in Tls_GaaConnectionHandshakeRefIndx array */
    uint16 usTlsConnHandshakeRefBaseIndx;
  
}Tls_CiphersuiteWorkerType;


/*************************Design ID: TLS_SDD_0431************************/
typedef struct STag_Tls_Handshake
{
    /* Reference to a CSM job to perform the hash operation for the whole 
    handshake. */
    uint32 ulTlsCsmHashVerifyJobRefIdx;
    
    /* Reference to CSM job to perform the DH Key Exchange algorithm operation*/
    uint32 ulTlsCsmKeyExchangeCalcPubValJobRefIdx;
    
    /* Reference to a CSM job to perform the Key Exchange algorithm operation */
    uint32 ulTlsCsmKeyExchangeCalcSecretJobRefIdx;
    
    /* Reference to a CSM job to perform data decryption, 
    e.g. with RSA key exchange operation. */
    uint32 ulTlsCsmKeyExchangeDecryptJobRefIdx;
    
    /* Reference to a CSM key to perform data decryption, 
    e.g. with RSA, used for exchange operation */
    uint32 ulTlsCsmKeyExchangeDecryptKeyRefIdx;

    /* Reference to a CSM key to perform data decryption, 
    e.g. with RSA, used for exchange operation */
    uint32 ulTlsCsmClientHSTrafficSecretKeyRefIdx;
    
    /* Reference to a CSM key to perform data decryption, 
    e.g. with RSA, used for exchange operation */
    uint32 ulTlsCsmClientHandshakeWriteIVKeyRefIdx;
    
    /* Reference to a CSM key to perform data decryption, 
    e.g. with RSA, used for exchange operation */
    uint32 ulTlsCsmClientHandshakeWritekeyRefIdx;
    
    /* Reference to a CSM key to perform data decryption, 
    e.g. with RSA, used for exchange operation */
    uint32 ulTlsCsmServerHSTrafficSecretKeyRefIdx;
    
    /* Reference to a CSM key to perform data decryption, 
    e.g. with RSA, used for exchange operation */
    uint32 ulTlsCsmServerHandshakeWriteIVKeyRefIdx;

    /* Reference to a CSM key to perform data decryption, 
    e.g. with RSA, used for exchange operation */
    uint32 ulTlsCsmServerHandshakeWritekeyRefIdx;
    
    /* Reference to a CSM key to perform data decryption, 
    e.g. with RSA, used for exchange operation */
    uint32 ulTlsCsmDerivedKeyRefIdx;
    
    /* Reference to a CSM key to perform data decryption, 
    e.g. with RSA, used for exchange operation */
    uint32 ulTlsCsmEarlySecretKeyRefIdx;
    
    /* Reference to a CSM job to perform the 
    HKDF Expand operation */
    uint32 ulTlsCsmHKDFExpandJobRefIdx;

    /* Reference to a CSM key associated to the
    CSM job that performs the HKDF Expand operation */
    uint32 ulTlsCsmHKDFExpandKeyRefIdx;
    
    /* Reference to a CSM Job to perform HKDF 
    Extract operation */
    uint32 ulTlsCsmHKDFExtractJobRefIdx;
    
    /* Reference to a CSM key associated to 
    the CSM job that performs the HKDF Extract operation */
    uint32 ulTlsCsmHKDFExtractKeyRefIdx;
    
    /* Reference to a CSM key to perform data decryption, 
    e.g. with RSA, used for exchange operation */
    uint32 ulTlsCsmHandshakeSecretKeyRefIdx;
    
    /* Reference to a CSM job to perform data encryption, 
    e.g. with RSA key exchange operation. */
    uint32 ulTlsCsmKeyExchangeEncryptJobRefIdx;
 
    /* Reference to a CSM key to perform data encryption, 
    e.g. with RSA, used for exchange operation. */
    uint32 ulTlsCsmKeyExchangeEncryptKeyRefIdx;
    
    /*Reference to CSM key used for Diffie Hellman(DH) key exchange operation */
    uint32 ulTlsCsmKeyExchangeKeyRefIdx;
    
    /*Reference to a CSM job to perform signature generation for DH operation */
    uint32 ulTlsCsmKeyExchangeSignatureGenerateJobRefIdx;
    
    /*Reference to a CSM key to perform signature generation for DH operation */
    uint32 ulTlsCsmKeyExchangeSignatureGenerateKeyRefIdx;
    
    /*Reference to CSM job to perform signature verification for DH operation */
    uint32 ulTlsCsmKeyExchangeSignatureVerifyJobRefIdx;
    
    /*Reference to CSM key to perform signature verification for DH operation */
    uint32 ulTlsCsmKeyExchangeSignatureVerifyKeyRefIdx;
 
    /* Reference to the master key that is calculated during the session. */
    uint32 ulTlsCsmMasterSecretKeyRefIdx;
    
    /* Reference to a CSM job to perform the PRF hash operation */
    uint32 ulTlsCsmPrfMacJobRefIdx;
    
    /* Reference to a CSM key associated to the CSM job that performs the PRF
    hash operation */
    uint32 ulTlsCsmPrfMacKeyRefIdx;
    
    /* Specifies how the CSM job supports the PRF operation. */
    TlsCsmPRFSupportType ddTlsCsmPRFSupportType;
 
    uint16 usTlsSupportedGrpId;

    uint16 usTlsSignatureId;

    uint16 usTlsGroupPriority;

}Tls_HandshakeType;


/*************************Design ID: TLS_SDD_0433****************************/
typedef enum ETag_Tls_CHState
{
    /* Indicates IDLE state */
    TLS_CH_PREPARE_SYNC_PARTS = 0x00,
    
    /* Indicates Random number Generation Stage  */
    TLS_CH_RANDOM,
    
    /* Indicates Csm Key Derive generation for binder  */
    TLS_CH_KEY_DERIVE_BINDER,
    
    /* Indicates HMAC generation for binder  */
    TLS_CH_HMAC_BINDER

} Tls_CHStateType;


/*************************Design ID: TLS_SDD_0434****************************/
#if (STD_ON == TLS_GROUP_CONFIGURED)
typedef enum ETag_Tls_GrpConnStatus
{
    /* indicates no connections a group are active */
    TLS_GROUP_CONNECTION_INACTIVE = 0x00,
    
    /* indicates a connection in a group is active */
    TLS_GROUP_CONNECTION_ACTIVE

} Tls_GrpConnStatusType;
#endif

/* Design ID : TLS_SDD_0432 */
typedef enum ETag_Tls_ElmntsOfHShakeProto
{
    TLS_PERFORM_CLIENT_HELLO_HANDSHAKE = 0x00u,
    TLS_HANDLE_CLIENT_HELLO_RECEPTION, //2
    TLS_PERFORM_SERVER_HELLO_HANDSHAKE,  //3      
    TLS_HANDLE_SERVER_HELLO_RECEPTION,   //4     
    TLS_PERFORM_CLIENT_HANDSHAKE_KEYS_CALCULATION,   //5      
    TLS_PERFORM_NEW_SESSION_TICKET_HANDSHAKE,  //6
    TLS_PERFORM_END_OF_EARLY_DATA_HANDSHAKE, //7
    TLS_PERFORM_SERVER_HANDSHAKE_KEYS_CALCULATION,  //8 
    TLS_PERFORM_ENCRYPTED_EXTENSIONS_HANDSHAKE, //9
    TLS_HANDLE_ENCRYPTED_EXTENSIONS_RECEPTION, //10
    TLS_PERFORM_CERTIFICATE_REQUEST_HANDSHAKE, //13
    TLS_HANDLE_CERTIFICATE_REQUEST_HANDSHAKE, //13  
    TLS_PERFORM_CERTIFICATE_HANDSHAKE, //11
    TLS_HANDLE_CERTIFICATE_HANDSHAKE, //12
	TLS_PERFORM_CERTIFICATE_VERIFICATION,
    TLS_PERFORM_CERTIFICATE_VERIFY, //14
    TLS_HANDLE_CERTIFICATE_VERIFY, //15
    TLS_PERFORM_SERVER_FINISHED_HANDSHAKE, //16
    TLS_HANDLE_SERVER_FINISHED_HANDSHAKE, //17
    TLS_PERFORM_CLIENT_FINISHED_HANDSHAKE, //18
    TLS_HANDLE_CLIENT_FINISHED_HANDSHAKE, //19
    TLS_PERFORM_CLIENT_APPLICATION_KEYS_CALCULATION, //20
    TLS_PERFORM_SERVER_APPLICATION_KEYS_CALCULATION,  //21
    TLS_DUMMY_HANDSHAKE  //22
}Tls_ElmntsOfHShakeProto;

/* Design ID : TLS_SDD_0435 */
typedef enum ETag_Tls_ElementsOfClientHelloExtensions
{
    TLS_PREPARE_CH_LEGACYVERSION = 0x00u, 
    TLS_PREPARE_CH_RANDOMNUMBER,        
    TLS_PREPARE_CH_SESSIONID,  
    TLS_PREPARE_CH_CIPHERSUITES,   
    TLS_PREPARE_CH_COMPRESSIONMETHOD,
    TLS_PREPARE_CH_EXTENSIONS          
}Tls_ElementsOfClientHelloExtensions;

/* Internal Pointer to Functions */
typedef void (*Tls_HShakeProtoFuncPtrType)(const uint16 LusActiveConnIdx);
               
typedef Std_ReturnType (*Tls_CHProtoFuncPtrType)
                            (const TcpIp_TlsConnectionIdType LddConnectionId,
                            uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed); 
        
typedef Std_ReturnType (*Tls_CHValidationFuncPtrType)
                            (const TcpIp_TlsConnectionIdType LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            uint16* const LpLengthProcessed);

typedef Std_ReturnType (*Tls_CHExtFuncPtrType)
                            (const TcpIp_TlsConnectionIdType LddConnectionId,
                            uint8* LpHShakeBuf,
                            uint16* const LpExtLen); 

typedef Std_ReturnType (*Tls_SHProtocolFuncPtrType)
                            (const TcpIp_TlsConnectionIdType LddConnectionId,
                            uint8* const LpHShakeBuf,
                            uint16* const LpLengthProcessed);
                                    
typedef Std_ReturnType (*Tls_SHValidationFuncPtrType)
                        (const TcpIp_TlsConnectionIdType LddConnectionId,
                        const uint8* const LpSHShakeBuf,
                        uint16* const LpLengthProcessed);



/*********STATE MACHINES FOR C and S *****************/
/* Design ID : TLS_SDD_0437 */
/* state machine for client */
typedef enum ETag_Tls_ClientStateType
{
	TLS_CLIENT_START               = 0x00,
	TLS_CLIENT_WAIT_FOR_SH,
	TLS_CLIENT_RECVD_SH,
	TLS_CLIENT_WAIT_FOR_EE,
	TLS_CLIENT_RECVD_EE,
	TLS_CLIENT_WAIT_FOR_CR,
	TLS_CLIENT_RECVD_CR,
	TLS_CLIENT_WAIT_FOR_CERT,
	TLS_CLIENT_RECVD_CERT,
	TLS_CLIENT_WAIT_FOR_CV,
	TLS_CLIENT_RECVD_CV,
	TLS_CLIENT_WAIT_FOR_FINSHED,
	TLS_CLIENT_RECVD_FINSHED,
	TLS_CLIENT_CONNECTED
}Tls_ClientStateType;
/* Design ID : TLS_SDD_0436 */
/* state machine for server */
typedef enum ETag_Tls_ServerStateType
{
 TLS_SERVER_WAIT_FOR_CH               = 0x00,
 TLS_SERVER_RECVD_CH,
 TLS_SERVER_NEGOTIATED,
 TLS_SERVER_WAIT_FOR_CERT,
 TLS_SERVER_RECVD_CERT,
 TLS_SERVER_WAIT_FOR_CV,
 TLS_SERVER_RECVD_CV,
 TLS_SERVER_WAIT_FOR_FINISHED,
 TLS_SERVER_RECVD_FINISHED,
 TLS_SERVER_CONNECTED
}Tls_ServerStateType;

/*********STATE MACHINES FOR C and S *****************/



/*********typdef for function pointer for tls tx indicatiion*****************/


/*********typdef for ACTIVE CONNECTION MANAGER *****************/
/*************************Design ID: TLS_SDD_0751************************/
typedef enum ETag_Tls_ActiveConnStatusType
{
  TLS_CONNECTION_ACTIVE = 0x00,
  
  TLS_CONNECTION_INACTIVE
  
}Tls_ActiveConnStatusType;

 
/*************************Design ID: TLS_SDD_0438************************/
typedef struct STag_Tls_ActiveConnType
{
    /* func ptr for TlsTxIndication, once u are implementing , update these things  */
    Tls_ActiveConnStatusType ddConnectionState;
 
    #if (STD_ON == TLS_TIMER_SUPPORT)
        uint16 usWaitingTimeout;
    #endif   
	
    TcpIp_TlsConnectionIdType ddTlsConnectionId;
}Tls_ActiveConnType;
/*********typdef for ACTIVE CONNECTION MANAGER *****************/


/*********typdef for RECORD PROTOCOL *****************/
/*************************Design ID: TLS_SDD_0439************************/
typedef enum ETag_Tls_ContentType
{
 TLS_INVALID = 0x00,
 TLS_CHANGE_CIPHER_SPEC = 0x14,
 TLS_ALERT = 0x15,
 TLS_HANDSHAKE = 0x16,
 TLS_APPLICATION_DATA = 0x17
}Tls_ContentType;
/*********typdef for RECORD PROTOCOL *****************/

/* Design ID : TLS_SDD_0440 */
/* List of Supported Groups as per TLS 1.3 */
typedef enum ETag_Tls_SupportedGrpType
{
 TLS_SECP256R1              = 0x0017,
 TLS_SECP384R1              = 0x0018,
 TLS_SECP521R1              = 0x0019,
 TLS_X25519                 = 0x001D,
 TLS_X448                   = 0x001E
}Tls_SupportedGrpType;

/* Design ID : TLS_SDD_0441 */
/* List of supported Qorix Signature as per TLS 1.3 */
typedef enum ETag_Tls_SignatureType
{
	TLS_RSA_PKCS1_SHA256           = 0x0401,
	TLS_RSA_PKCS1_SHA384              = 0x0501,
	TLS_RSA_PKCS1_SHA512              = 0x0601,
	TLS_RSA_PSS_RSAE_SHA256                 = 0x0804,
	TLS_RSA_PSS_RSAE_SHA384                   = 0x0805,
	TLS_RSA_PSS_RSAE_SHA512                 = 0x0806,
	TLS_RSA_PSS_PSS_SHA256                   = 0x0809,
	TLS_RSA_PSS_PSS_SHA384                   = 0x080A,
	TLS_RSA_PSS_PSS_SHA512                   = 0x080B
}Tls_SignatureType;



/* Validation of CH required types */
/* Design ID : TLS_SDD_0692 */
#define TLS_SUPPORTED_VERSIONS_BIT                          (uint8)(1u << 0u)
/* Design ID : TLS_SDD_0679 */
#define TLS_PSK_KEY_EXCHANGE_MODES_BIT                          (uint8)(1u << 1u)
/* Design ID : TLS_SDD_0678 */
#define TLS_PRE_SHARED_KEY_BIT                          (uint8)(1u << 2u)
/* Design ID : TLS_SDD_0707 */
#define TLS_CH_SUPPORTED_GROUPS_BIT                          (uint8)(1u << 3u)
/* Design ID : TLS_SDD_0708*/
#define TLS_CH_KEY_SHARE_BIT                          (uint8)(1u << 4u)
/* Design ID : TLS_SDD_0685 */
#define TLS_SIGNATURE_ALGORITHMS_BIT                          (uint8)(1u << 5u)

/* Design ID : TLS_SDD_0709 */
#define TLS_AUTH_PSK_MASK                            0x07u
/* Design ID : TLS_SDD_0711 */
#define TLS_AUTH_CERT_MASK                            0x39u
              
typedef Std_ReturnType (*Tls_CHExtFn)                        
                        (const uint16 LusActiveConnIdx,
                        const uint8* const LpCHExtBuff);
 /* Design ID : TLS_SDD_0408 */                                 
typedef struct STag_Tls_ExtHandler
{
    Tls_ExtType ddExtType;
    Tls_CHExtFn aaValidateFn;
    uint8 ucRequiresMask;
    uint8 ucExtbit;
}Tls_ExtHandler;
/* Design ID : TLS_SDD_0406 */
/* this is like what extension was parsed */
typedef struct STag_Tls_ParsedExt
{
    const Tls_ExtHandler* pExtHandler;
    const uint8* pExtData;
}Tls_ParsedExt;


typedef Std_ReturnType (*Tls_CHExtHandlrFnPtrType)
                            (const TcpIp_TlsConnectionIdType LddConnectionId,
                            const uint8* const LpCHShakeBuf,
                            Tls_ParsedExt* const LpParsedExtns,
                            uint8* const LpParsedCount,
                            uint8* const LpSeenMask);
/* Design ID : TLS_SDD_0444 */
#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)                           
typedef enum ETag_Tls_AsyncEventType
{
 TLS_E_OK = 0x00,
 TLS_E_NOT_OK = 0x01,
 TLS_E_BUSY = 0x02
}Tls_AsyncEventType;
#endif

/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/* Design ID: TLS_SDD_0296 */
#define TLS_START_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"
extern const Tls_ConfigType Tls_GaaConfig[TLS_ONE];
#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"

/* Design ID: TLS_SDD_0148 */
#define TLS_START_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"
extern const TlsConnectionStrType Tls_GaaConnection[TLS_TOTAL_CONNECTIONS];
#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"

/*************************Design ID: TLS_SDD_0144****************************/
#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
#define TLS_START_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"
extern const Tls_CertificateIdentityType 
Tls_GaaCertificateIdentity[TLS_NUM_OF_CERTIFICATE_IDENTITYS];
#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"
#endif

/* Design ID: TLS_SDD_0151 */
#if (STD_ON == TLS_PSK_IDENTITY_SUPPORT_ENABLED)
#define TLS_START_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"
extern const Tls_PskIdentityType  Tls_GaaPskIdentity[TLS_NUM_OF_PSK_CONFIGURED];
#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"
#endif

/* Design ID: TLS_SDD_0145 */
#define TLS_START_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"
extern const Tls_CiphersuiteDefinition 
Tls_GaaCiphersuiteDef[TLS_NUM_OF_CIPHERSUITE_DEFINITION];
#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"

/* Design ID: TLS_SDD_0146 */
#define TLS_START_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h" 
extern const Tls_CiphersuiteWorkerType 
Tls_GaaCiphrSuiteWrkr[TLS_NUM_OF_CIPHERSUITE_WORKER];
#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"

/* Design ID: TLS_SDD_0150 */
#define TLS_START_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"
extern const Tls_HandshakeType 
Tls_GaaHandshake[TLS_NUM_OF_HANDSHAKE];
#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include "Tls_MemMap.h"

/* Design ID: TLS_SDD_0152 */
#if (STD_ON == TLS_PSK_IDENTITY_SUPPORT_ENABLED)  
#define TLS_START_SEC_CONST_8
#include "Tls_MemMap.h"
extern const uint8  Tls_GaaPSKLookUp[];
#define TLS_STOP_SEC_CONST_8
#include "Tls_MemMap.h"
#endif
/* Design ID: TLS_SDD_0147 */
#define TLS_START_SEC_CONST_16
#include "Tls_MemMap.h"
extern const uint16  Tls_GaaCiphrWrkrLookUp[];
#define TLS_STOP_SEC_CONST_16
#include "Tls_MemMap.h"


/* Design ID: TLS_SDD_0147 */
#define TLS_START_SEC_CONST_16
#include "Tls_MemMap.h"
extern const uint16  Tls_GaaHSLookUp[];
#define TLS_STOP_SEC_CONST_16
#include "Tls_MemMap.h"

/* Design ID: TLS_SDD_0149 */
#if (STD_ON == TLS_GROUP_CONFIGURED)
 
#define TLS_START_SEC_CONST_16
#include "Tls_MemMap.h"
extern const uint16  Tls_GaaGroupArray[TLS_TOTAL_CONNECTIONS];
#define TLS_STOP_SEC_CONST_16
#include "Tls_MemMap.h"

#endif

#endif