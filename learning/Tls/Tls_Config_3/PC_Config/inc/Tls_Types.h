/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_Types.h                                                    **
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
** Design ID  : TLS_SDD_0298                                                  **
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

#ifndef TLS_TYPES_H
#define TLS_TYPES_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

/*************************Design ID: TLS_SDD_0402****************************/
/* Used by TCPIP module , during Tls_openConnection */
typedef enum ETag_Tls_ConnectionType
{
  TLS_CLIENT = 0x00,
  
  TLS_SERVER
  
} Tls_ConnectionType;

/*************************Design ID: TLS_SDD_0401****************************/
typedef enum ETag_Tls_ConnectionStatusType
{
    /* Indicates no TLS connection attempt has started */
    TLS_CONNECTION_NOT_INITIATED = 0x00,
    
    /* Represents Handshake is in progress  */
    TLS_CONNECTION_IN_PROGRESS,
    
    /* Confirms the TLS connection is established and secure communication
       can proceed*/
    TLS_CONNECTION_READY,
    
    /* Indicates the TLS handshake failed */
    TLS_CONNECTION_FAILED,
    
    /* Indicates the TLS Conection is closed */
    TLS_CONNECTION_CLOSED,
    
    /* Indicates the TLS connection terminated */
    TLS_CONNECTION_ABORTED
    
} Tls_ConnectionStatusType;

#endif