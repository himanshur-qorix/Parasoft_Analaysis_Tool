/*******************************************************************************
**                               QORIX GmbH                                   **
**                                                                            **
** QORIX GmbH owns all the rights to this work. This work shall not be copied,**
** reproduced, used, modified or its information disclosed without the prior  **
** written authorization of QORIX GmbH.                                       **
**                                                                            **
** SRC-MODULE: Tls_Certificate.c                                              **
**                                                                            **
** TARGET    : All                                                            **
**                                                                            **
** PRODUCT   : AUTOSAR Transport Layer Security                               **
**                                                                            **
** PURPOSE   : This file provides definitions of following Tls APIs:          **
**                                                                            **
**  PLATFORM DEPENDANT [yes/no]: no                                           **
**                                                                            **
**  TO BE CHANGED BY USER [yes/no]: no                                        **
**                                                                            **
**  Design ID  : TLS_SDD_0747                                                 **
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
/*******************************************************************************
**                      Parasoft violations Section                           **
*******************************************************************************/
/** 
*
* #section Tls_Certificate_c_REF_1
* Violates MISRAC2012-RULE_5_4-c-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Certificate_c_REF_2
* Violates MISRAC2012-RULE_5_4-d-2: The '' macro has the same name (or the first
* 31 characters of the name) as the '' macro previously defined in the current
* file
* Justification: As macros are defined by AUTOSAR standards and internal
* specifications the length of name shall not be restricted
*
* #section Tls_Certificate_c_REF_3
* Violates MISRAC2012-RULE_20_1-a-4: Do not precede the #include directive with
* executable code
* Justification: Macros are defined by AUTOSAR standards and internal
* specifications.
*
* #section Tls_Certificate_c_REF_4
* Violates CERT_C-DCL19-a-3: Variable "" should be declared as locally as
* possible
* Justification: Coding guidelines suggest the variables are declared on the
* start of the function.
*
* #section Tls_Certificate_c_REF_5
* Violates CERT_C-API00-a-3: Parameter '' is not validated before use.
* Justification: This parameter either originates from the configuration or is
* validated in the main API before being passed as an argument to the local API.
* Hence, additional validation within the local API is not required and has been
* manually verified.
*
* #section Tls_Certificate_c_REF_6
* Violates MISRAC2012-RULE_2_2-b-2: Value of '' is never used
* Justification: The variables '' are intentionally initialized to a defined 
* safe value (TLS_ZERO) before any functional usage
* The TASKING compiler issues a W507 ("possibly uninitialized") diagnostic due 
* to conservative data-flow analysis. Manual code review confirms that the 
* variable is deterministically initialized and cannot remain uninitialized at runtime.
*
* #section Tls_Certificate_c_REF_7
* Violates CERT_C-DCL22-a-3: Value of '' is never used
* Justification: The variables '' are intentionally initialized to a defined 
* safe value (TLS_ZERO) before any functional usage
* The TASKING compiler issues a W507 ("possibly uninitialized") diagnostic due 
* to conservative data-flow analysis. Manual code review confirms that the 
* variable is deterministically initialized and cannot remain uninitialized at runtime.
*/

/* parasoft-begin-suppress MISRAC2012-RULE_5_4-c-2 
"Reason: Tls_Certificate_c_REF_1" */
/* parasoft-begin-suppress MISRAC2012-RULE_5_4-d-2 
"Reason: Tls_Certificate_c_REF_2" */
/* parasoft-begin-suppress MISRAC2012-RULE_20_1-a-4 
"Reason: Tls_Certificate_c_REF_3" */
/* parasoft-begin-suppress CERT_C-DCL19-a-3 
"Reason: Tls_Certificate_c_REF_4" */
/* parasoft-begin-suppress CERT_C-API00-a-3 
"Reason: Tls_Certificate_c_REF_5" */

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

#include "Tls_Internal.h"
#include "Tls_External.h"
#include "Tls_Inline.h"
#include "Tls_Certificate.h"
#if (STD_ON == TLS_CERTIFICATE_SUPPORT_ENABLED)
#include "KeyM.h"

/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/

/*******************************************************************************
**                      Version Check                                         **
*******************************************************************************/

    /* Padding bytes for CertificateVerify message (RFC 8446 4.4.3) */
    static const uint8 LaaSignPadding[TLS_HEX_40] = 
	{
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO,
		TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO, TLS_THIRTYTWO
    };
	  
static const uint8 TLS13_CLIENT_CERT_VERIFY[TLS_MAX_CONTEXTSTRING_SIZE] = 
{
	0x54U, 0x4CU, 0x53U, 0x20U, 0x31U, 0x2EU, 0x33U, 0x2CU, 0x20U, 0x63U,
	0x6CU, 0x69U, 0x65U, 0x6EU, 0x74U, 0x20U, 0x43U, 0x65U, 0x72U, 0x74U,
	0x69U, 0x66U, 0x69U, 0x63U, 0x61U, 0x74U, 0x65U, 0x56U, 0x65U, 0x72U,
	0x69U, 0x66U, 0x79U
};

static const uint8 TLS13_SERVER_CERT_VERIFY[TLS_MAX_CONTEXTSTRING_SIZE] = 
{
    0x54U, 0x4CU, 0x53U, 0x20U, 0x31U, 0x2EU, 0x33U, 0x2CU, 0x20U,
    0x73U, 0x65U, 0x72U, 0x76U, 0x65U, 0x72U, 0x20U,
    0x43U, 0x65U, 0x72U, 0x74U, 0x69U, 0x66U, 0x69U, 0x63U, 0x61U,
    0x74U, 0x65U, 0x56U, 0x65U, 0x72U, 0x69U, 0x66U, 0x79U
};
	  
/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_GetMappedCertSigAlgo(const uint8* const LpCertElementData,
                                      const uint32* const LpCertElementDataLength,
									  uint16* const LpMappedCertSigAlgo);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType Tls_GetOID(const uint8* const LpCertElementData,
                                      const uint32* const LpCertElementDataLength,
									  uint8* const LpOID,
									  uint16* const LpOIDLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static boolean Tls_ChkOID(const uint8* const LpCertElementData,
                                      const uint16* const LpCertElementDataLength,
									  const uint8* const LpOID,
									  const uint16 LucOIDLen);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"	

#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateCRSign(const uint16 LusActiveConnIdx, 
												const uint8* const LpHShakeBuf);
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"
								  
/*******************************************************************************
**                      Global Data                                           **
*******************************************************************************/

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

/*******************************************************************************
** Function Name        : Tls_ProcessCertMessage                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function retrives the certificate from keym and 
                            places it in the handshake buffer. **   
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0364                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ProcessCertMessage(const uint16 LusActiveConnIdx)
{
    Std_ReturnType LddReturnValue;
	Std_ReturnType LddProcessFullCert;
    TcpIp_TlsConnectionIdType LddConnectionId;
    KeyM_CertificateIdType LddLocalCertId;
    KeyM_CertDataType LddCertificateData;
	Tls_ConnectionType LddConType;
	uint8* LpHShakeBuf;
    uint32 LulCertDataLength;
    uint16 LusCerttIdx;
	uint16 LusTemp;
	uint8  LaaInnerPlainText[TLS_NINE]; 
	boolean LblClientAuthReq;
	
	/* Tasking waring */
	LddReturnValue = E_NOT_OK; // parasoft-suppress MISRAC2012-RULE_2_2-b-2 CERT_C-DCL22-a-3 "Reason: Tls_Certificate_c_REF_6 Tls_Certificate_c_REF_7.
    LddConnectionId = 
    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;

    /* from where to store the CERT message */
    LusCerttIdx = Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition; 
	

	LddConType = Tls_GaaConnection[LddConnectionId].ddTlsConnectionType;
	
	if (TLS_CLIENT == LddConType)
	{
		/* if recived certificate Req from server was correct , only then send full certificate */
		/* check if Client Auth is enabled for Client */
		LblClientAuthReq = Tls_GaaConnection[LddConnectionId].blTlsUseClintAuthReq;
				
		if (LblClientAuthReq)
		{
			/* send Full Certificate */
			LddProcessFullCert = E_OK;
		}
		else
		{
			LddProcessFullCert = E_NOT_OK;
			/* send Empty Certificate */
			LpHShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusCerttIdx];
			
			LpHShakeBuf[TLS_ZERO] = (uint8)TLS_CERTIFICATE; 
			LpHShakeBuf[TLS_ONE] = (uint8)TLS_ZERO;
			LpHShakeBuf[TLS_TWO] = (uint8)TLS_ZERO;
			LpHShakeBuf[TLS_THREE] = (uint8)TLS_FOUR;
			LpHShakeBuf[TLS_FOUR] = (uint8)TLS_ZERO;
			LpHShakeBuf[TLS_FIVE] = (uint8)TLS_ZERO;
			LpHShakeBuf[TLS_SIX] = (uint8)TLS_ZERO;
			LpHShakeBuf[TLS_SEVEN] = (uint8)TLS_ZERO;
			
			Tls_MemCpy(&LaaInnerPlainText[TLS_ZERO], &LpHShakeBuf[TLS_ZERO],
															(uint16)TLS_EIGHT);
															
			LaaInnerPlainText[TLS_EIGHT] = (uint8)TLS_HANDSHAKE;  
			
			LddReturnValue = Tls_TransmitRecord(
												LusActiveConnIdx,
												TLS_APPLICATION_DATA,
												&LaaInnerPlainText[TLS_ZERO],
												(uint16)TLS_NINE); 
						
			if (E_OK == LddReturnValue)
			{			
				Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition += 
				(uint16)TLS_EIGHT;
				Tls_HandshakeFrame[LusActiveConnIdx].usProcessedHSBytes += 
				(uint16)TLS_EIGHT;
			}
			else
			{
				Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
				LddReturnValue = E_NOT_OK;
			}			
		}
	}
	else
	{
		/* for server , just send the certificate HS */
		LddProcessFullCert = E_OK;
	}	

	if (E_OK == LddProcessFullCert)
	{
		/* since the max buffer size is always generated based on the highest len certificate + 1024 extra bytes 
		so what if the the higest cert bytes was itsef given by Keym, to make it accomodate */
		
		//LusTemp = (uint16)(TLS_MAX_BUFFER_SIZE - TLS_CERT_OFFSET_SIZE);
		
		if (((uint32)LusCerttIdx + (uint32)TLS_MAX_CERTIFICATE_SIZE) < (uint32)TLS_MAX_BUFFER_SIZE)
		{	
		
			LpHShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusCerttIdx];
	
			
			LddLocalCertId = Tls_GaaCertificateIdentity[Tls_GaaConnection[
				LddConnectionId].ulTlsCertIdnRefIdx].ddTlsCipherKeyMLocalCertificateIdx;
			
			
			/* why 11 : since from 11 byte the actuall certificate data will be give to us 
			from keym */
			LddCertificateData.certData = &LpHShakeBuf[TLS_ELEVEN];
			
			/* we have to tell in advance to keym , how much space is there in our buffer */
			LulCertDataLength = 
							(uint32)((uint32)TLS_MAX_BUFFER_SIZE - (uint32)LusCerttIdx);
							
			/* since 11 bytes are reserved for handshake header and certificate request context */
			LulCertDataLength -= (uint32)TLS_FOURTEEN;
			
			LddCertificateData.certDataLength =  LulCertDataLength;
			
			
			
			/*  actual certificate data start froms */
			LddReturnValue =  KeyM_GetCertificate (LddLocalCertId,
																&LddCertificateData); 
			
			LulCertDataLength = LddCertificateData.certDataLength;
			
			LusTemp = (uint16)((uint32)LulCertDataLength + (uint32)TLS_FOURTEEN);
			LusTemp += LusCerttIdx;
			
			
			if (LusTemp < TLS_MAX_BUFFER_SIZE)
			{
				if (E_OK == LddReturnValue)
				{
					LpHShakeBuf[TLS_ZERO] = (uint8)TLS_CERTIFICATE; 
					/* update for handshake header
					0x0B, --> TAG
					0x00, 0x00, 0x02, --> Length
					CERT data  --. VALUE  */ 
						
				
					/* Certificate Request context */
					LpHShakeBuf[TLS_FOUR] = (uint8)TLS_ZERO;   
				
					/* update the one and only certificate which was retrieved from Keym
					into the certificate 3 bytes field (each certificate 3 byte field) */
				
					if (LulCertDataLength != (uint32)TLS_ZERO)
					{
						LpHShakeBuf[TLS_EIGHT] = 
									(uint8)((uint32)(LulCertDataLength >> (uint32)TLS_SIXTEEN) & 
																		(uint32)TLS_LSB_MASK);
						
						LpHShakeBuf[TLS_NINE] = 
									(uint8)((uint32)(LulCertDataLength >> (uint32)TLS_EIGHT) & 
																		(uint32)TLS_LSB_MASK);
						
						LpHShakeBuf[TLS_TEN] = 
									(uint8)((uint32)(LulCertDataLength) & (uint32)TLS_LSB_MASK); 
				
						
				
						/* now update the Main Certificate Length field ( which is sum of 
						all certificates field ) 
						Why 5+ : 3 bytes each certificate len field + 2 bytes extension field */
						LulCertDataLength += (uint32)TLS_FIVE;
				
						LpHShakeBuf[TLS_FIVE] = 
						(uint8)((uint32)(LulCertDataLength >> (uint32)TLS_SIXTEEN) & 
																		(uint32)TLS_LSB_MASK);
						
						LpHShakeBuf[TLS_SIX] = 
						(uint8)((uint32)(LulCertDataLength >> (uint32)TLS_EIGHT) & 
																		(uint32)TLS_LSB_MASK);
						
						LpHShakeBuf[TLS_SEVEN] = 
									(uint8)((uint32)(LulCertDataLength) & (uint32)TLS_LSB_MASK); 
				
				
						/* now update len in Handshake header
						Why 4+ : 3 bytes each main certificate len field + 1 
																Certificate request Context*/
						LulCertDataLength += (uint32)TLS_FOUR;
				
						LpHShakeBuf[TLS_ONE] = 
						(uint8)((uint32)(LulCertDataLength >> (uint32)TLS_SIXTEEN) & 
																		(uint32)TLS_LSB_MASK);
						
						LpHShakeBuf[TLS_TWO] = 
						(uint8)((uint32)(LulCertDataLength >> (uint32)TLS_EIGHT) & 
																		(uint32)TLS_LSB_MASK);
						
						LpHShakeBuf[TLS_THREE] = 
									(uint8)((uint32)(LulCertDataLength) & (uint32)TLS_LSB_MASK);
				
				
						/* after each certificate there is two byte extension field , 
						update that two zero, currently */
						LpHShakeBuf[TLS_ELEVEN + LddCertificateData.certDataLength] = 
																				(uint8)TLS_ZERO;
						LpHShakeBuf[TLS_TWELVE + LddCertificateData.certDataLength] = 
																				(uint8)TLS_ZERO;
						/* Last byte shd be content as per rfc */
						LpHShakeBuf[TLS_THIRTEEN + LddCertificateData.certDataLength] = 
																		(uint8)TLS_HANDSHAKE;
				
						/* why 5+ : 3 bytes HS header Len + 1 Tag + 1 Last Byte Content field */
						LulCertDataLength += (uint32)TLS_FIVE;
				
						LddReturnValue = Tls_TransmitRecord(
														LusActiveConnIdx,
														TLS_APPLICATION_DATA,
														&LpHShakeBuf[TLS_ZERO],
														(uint16)LulCertDataLength); 
														
						if (E_OK == LddReturnValue)
						{
							/* shd leave the last content byte */
							Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition += 
							(uint16)(LulCertDataLength - (uint32)TLS_ONE);
							
							Tls_HandshakeFrame[LusActiveConnIdx].usProcessedHSBytes += 
							(uint16)(LulCertDataLength - (uint32)TLS_ONE);
						}
						else
						{
							Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
							LddReturnValue = E_NOT_OK;
						}	
					}
					else
					{
						Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
						LddReturnValue = E_NOT_OK;
					}
				}
				else
				{
					Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
					LddReturnValue = E_NOT_OK;		
				}			
			}
			else
			{
				Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
				LddReturnValue = E_NOT_OK;					
			}
		}
		else
		{
			Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
			LddReturnValue = E_NOT_OK;			
		}		
	}

    return LddReturnValue;
}/* end of Tls_ProcessCertMessage */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_ValidateCertMsg                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function retrives the certificate from keym and 
                            places it in the handshake buffer. **   
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0365                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ValidateCertMsg(const uint16 LusActiveConnIdx,
												const uint8* const LpHShakeBuf)
{
	Std_ReturnType LddReturnValue;
	KeyM_CertDataType LddCertificateData;
	KeyM_CertificateIdType LddRemoteCertId;
	uint32 LulHSHdrLen;
	uint32 LulMainCertLen;
	uint32 LulSingleCertLen;
	void* LpHShakeBufTemp;
	
	LpHShakeBufTemp = 
		&Tls_GaaHShakeBuffer[LusActiveConnIdx][(Tls_HandshakeFrame[LusActiveConnIdx].usProcessedHSBytes) + TLS_ELEVEN];
	
	if ((uint8)TLS_CERTIFICATE == (*LpHShakeBuf))
	{
		LulHSHdrLen =
		((uint32)TLS_ZERO  << (uint8)TLS_TWENTYFOUR) |
		((uint32)LpHShakeBuf[TLS_ONE]  << (uint8)TLS_SIXTEEN) |
		((uint32)LpHShakeBuf[TLS_TWO]  << (uint8)TLS_EIGHT)  |
		((uint32)LpHShakeBuf[TLS_THREE]); 
		
		/* Certificate Request Context Length shd be 0*/
		if ((uint8)TLS_ZERO == LpHShakeBuf[TLS_FOUR])
		{
			LulMainCertLen =
			((uint32)TLS_ZERO  << (uint8)TLS_TWENTYFOUR) |
			((uint32)LpHShakeBuf[TLS_FIVE]  << (uint8)TLS_SIXTEEN) |
			((uint32)LpHShakeBuf[TLS_SIX]  << (uint8)TLS_EIGHT)  |
			((uint32)LpHShakeBuf[TLS_SEVEN]); 
			
			/* Main Certificate Length shd be 4 bytes extra then HS hedr */
			if (((uint32)(LulMainCertLen + (uint32)TLS_FOUR)) == LulHSHdrLen)
			{
				LulSingleCertLen =
				((uint32)TLS_ZERO  << (uint8)TLS_TWENTYFOUR) |
				((uint32)LpHShakeBuf[TLS_EIGHT]  << (uint8)TLS_SIXTEEN) |
				((uint32)LpHShakeBuf[TLS_NINE]  << (uint8)TLS_EIGHT)  |
				((uint32)LpHShakeBuf[TLS_TEN]); 
					
				if (((uint32)(LulSingleCertLen + (uint32)TLS_FIVE)) == 
																LulMainCertLen)
				{	
					if ((LpHShakeBuf[TLS_ELEVEN + LulSingleCertLen] == 
                                                            (uint8)TLS_ZERO) &&
						(LpHShakeBuf[TLS_ELEVEN + LulSingleCertLen + TLS_ONE] 
                                                            == (uint8)TLS_ZERO))
					{
						/* Curently the extension Field , i am not checking here 
						assuming it will be taken care in future*/
						LddRemoteCertId = Tls_GaaCertificateIdentity[
						Tls_GaaConnection[Tls_GddActiveConnManager[
						LusActiveConnIdx].ddTlsConnectionId].ulTlsCertIdnRefIdx].
						ddTlsCipherKeyMRemoteCertificateIdx;
						
						LddCertificateData.certData = LpHShakeBufTemp;
						LddCertificateData.certDataLength = LulSingleCertLen;
						
						LddReturnValue = KeyM_SetCertificate(LddRemoteCertId,
														&LddCertificateData);	

						if (E_OK != LddReturnValue)
						{
							/* Not sure */
							Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
							LddReturnValue = E_NOT_OK;	
						}
					}
					else
					{
						/* Not sure */
						Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
						LddReturnValue = E_NOT_OK;							
					}
				}
				else
				{
					Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
					LddReturnValue = E_NOT_OK;						
				}
			}
			else
			{
				/* chances are there it can be an empty certificate 
				QORIX Design Decision: The Server shall reject the handshake with a "certificate_required" alert. */
				if ((uint32)TLS_ZERO == LulMainCertLen)
				{
					Tls_GaaAlert[LusActiveConnIdx] = TLS_CERTIFICATE_REQUIRED;
				}
				else
				{
					Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
				}			
				LddReturnValue = E_NOT_OK;						
			}
		}
		else
		{
			Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
			LddReturnValue = E_NOT_OK;				
		}
	}
	else
	{
        Tls_GaaAlert[LusActiveConnIdx] = TLS_UNEXPECTED_MESSAGE;
        LddReturnValue = E_NOT_OK;		
	}
    return LddReturnValue;
}/* end of Tls_ValidateCertMsg */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_VerifyCertificate                         **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function retrives the certificate from keym and 
                            places it in the handshake buffer. **   
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0366                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Tls_AsyncEventType  Tls_VerifyCertificate(const uint16 LusActiveConnIdx)
{
    Std_ReturnType LddReturnValue;
	Std_ReturnType LddKeyMRetrnVal;
	Tls_AsyncEventType LddReturnAsyncEvent;
    TcpIp_TlsConnectionIdType LddConnectionId;
    KeyM_CertificateIdType LddRemoteCertId;
	KeyM_CertElementIdType LddLocalCertElementId;
    uint8 LaaCertElementData[TLS_MAX_CERTIFICATE_ELEMENT_SIZE];
	uint32 LusCertElementDataLength;
	KeyM_CertificateStatusType LddStatus;
	boolean Lblstatus;
	uint16 LusMappedSignature;
	Tls_FrameType*      LpTlsFrame;
	
	LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];
	
		/* check for Asyncrnolsy was it called , once KeyM_VerifyCertificate is called
	no matter how many times, in next main function only the KeyM_CertGetStatus
	has to be called */

	LddConnectionId = 
				Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
		
	/*  Get the configured certificate ID */
	LddRemoteCertId = Tls_GaaCertificateIdentity[Tls_GaaConnection[
	LddConnectionId].ulTlsCertIdnRefIdx].ddTlsCipherKeyMRemoteCertificateIdx;
			
	if (LpTlsFrame->ddVerifyCertState != TLS_INVOKED)
	{
		
		LddKeyMRetrnVal = KeyM_VerifyCertificate(LddRemoteCertId);
		
		if ((E_OK != LddKeyMRetrnVal) && (KEYM_E_BUSY != LddKeyMRetrnVal))
		{
			Tls_GaaAlert[LusActiveConnIdx] = 
										TLS_INTERNAL_ERROR;
			LddReturnAsyncEvent = TLS_E_NOT_OK;
		}
		else
		{
			/* just update busy for both , since this i will handling in the 
			handshake.c */
			LddReturnAsyncEvent = TLS_E_BUSY;

			if (KEYM_E_BUSY == LddKeyMRetrnVal)
			{
				/* in nxt main function from function Tls_VerifyCertificate
				call KeyM_VerifyCertificate again */
				LpTlsFrame->ddVerifyCertState = TLS_BUSY;
			}
			else
			{
				/* if E_OK then keep calling keym get status from Tls_VerifyCertificate */
				LpTlsFrame->ddVerifyCertState = TLS_INVOKED;
			}
		}		
	}
	else
	{				
		/*  Get the configured Element ID for the certificate */	
		LddLocalCertElementId = Tls_GaaCertificateIdentity[Tls_GaaConnection[
			LddConnectionId].ulTlsCertIdnRefIdx].ddTlsCipherKeyMRemoteCertElementIdx;
		
		/*  Get the status of the certificate */
		LddReturnValue = KeyM_CertGetStatus(LddRemoteCertId, &LddStatus); 
		
		if (E_OK == LddReturnValue)
		{
			/*  check for the status of the certificate */
			if (LddStatus == KEYM_CERTIFICATE_VALID)
			{				
				/*  Get the certificate element data and Certificate data length */
				LddReturnValue = KeyM_CertElementGet(
									LddRemoteCertId,
									LddLocalCertElementId,
									&LaaCertElementData[TLS_ZERO],
									&LusCertElementDataLength
									);
									
				if (E_OK == LddReturnValue)
				{
				
					/*  Check if it is present in signature schemes */
					LddReturnValue = Tls_GetMappedCertSigAlgo(&LaaCertElementData[TLS_ZERO],
					&LusCertElementDataLength, &LusMappedSignature);
					
					if (E_OK == LddReturnValue)
					{
						Lblstatus = Tls_CheckSupportedSigAlgo(LusMappedSignature,
															LusActiveConnIdx);  
						if (Lblstatus)
						{
							LddReturnAsyncEvent = TLS_E_OK;
						}
						else
						{
							Tls_GaaAlert[LusActiveConnIdx] = 
												TLS_UNSUPPORTED_CERTIFICATE;
							LddReturnAsyncEvent = TLS_E_NOT_OK;
						}
					}
					else
					{
						Tls_GaaAlert[LusActiveConnIdx] = TLS_UNSUPPORTED_CERTIFICATE;
						LddReturnAsyncEvent = TLS_E_NOT_OK;
					}
				}
				else
				{
					Tls_GaaAlert[LusActiveConnIdx] = TLS_UNSUPPORTED_CERTIFICATE;
					LddReturnAsyncEvent = TLS_E_NOT_OK;
				}			
			}
			else
			{
				if (LddStatus == KEYM_E_CERTIFICATE_REVOKED)
				{
					Tls_GaaAlert[LusActiveConnIdx] = TLS_CERTIFICATE_REVOKED;
					LddReturnAsyncEvent = TLS_E_NOT_OK;
				}
				else if (LddStatus == KEYM_E_CERTIFICATE_VALIDITY_PERIOD_FAIL)
				{
					Tls_GaaAlert[LusActiveConnIdx] = TLS_CERTIFICATE_EXPIRED;
					LddReturnAsyncEvent = TLS_E_NOT_OK;
				}
				else if (LddStatus == KEYM_CERTIFICATE_NOT_AVAILABLE)
				{
					Tls_GaaAlert[LusActiveConnIdx] = TLS_CERTIFICATE_UNKNOWN;
					LddReturnAsyncEvent = TLS_E_NOT_OK;
				}
				else if ((LddStatus == KEYM_CERTIFICATE_INVALID) || (LddStatus == KEYM_E_CERTIFICATE_SIGNATURE_FAIL))
				{
					Tls_GaaAlert[LusActiveConnIdx] = TLS_BAD_CERTIFICATE;
					LddReturnAsyncEvent = TLS_E_NOT_OK;
				}
				
				else if ((LddStatus == KEYM_E_CERTIFICATE_INVALID_CHAIN_OF_TRUST) 
						|| (LddStatus == KEYM_E_CERTIFICATE_INVALID_TYPE)
						|| (LddStatus == KEYM_E_CERTIFICATE_INVALID_FORMAT)
						|| (LddStatus == KEYM_E_CERTIFICATE_INVALID_CONTENT))
				{
					Tls_GaaAlert[LusActiveConnIdx] = TLS_UNSUPPORTED_CERTIFICATE;
					LddReturnAsyncEvent = TLS_E_NOT_OK;
				}
				else
				{
					/* may be this for case where this time is needed */
					LddReturnAsyncEvent = TLS_E_BUSY;
				}
			}			
		}
		else
		{
			Tls_GaaAlert[LusActiveConnIdx] = TLS_UNSUPPORTED_CERTIFICATE;
			LddReturnAsyncEvent = TLS_E_NOT_OK;
		}		
	}

    return LddReturnAsyncEvent;
}/* end of Tls_ProcessHShakeProtocol */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_GetMappedCertSigAlgo                            **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Prepares the referred PSKs to connection in the buffer **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0363                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetMappedCertSigAlgo(const uint8* const LpCertElementData,
                                      const uint32* const LpCertElementDataLength,
									  uint16* const LpMappedCertSigAlgo)
{
	Std_ReturnType LddReturnValue;
    uint8 LaaOID[TLS_TEN];
	uint8*  LpOID;
    uint16  LucOIDLen;
	
	static const uint8 TLS_SIG_RSA_PKCS1_SHA256[TLS_NINE] =
	{
		TLS_HEX_2A, TLS_HEX_86, TLS_HEX_48, TLS_HEX_86,
		TLS_HEX_F7, TLS_HEX_0D, TLS_HEX_01, TLS_HEX_01, TLS_HEX_0B
	};
	
	static const uint8 TLS_SIG_RSA_PKCS1_SHA384[TLS_NINE] =
	{
		TLS_HEX_2A, TLS_HEX_86, TLS_HEX_48, TLS_HEX_86,
		TLS_HEX_F7, TLS_HEX_0D, TLS_HEX_01, TLS_HEX_01, TLS_HEX_0C
	};
	
	static const uint8 TLS_SIG_RSA_PKCS1_SHA512[TLS_NINE] =
	{
		TLS_HEX_2A, TLS_HEX_86, TLS_HEX_48, TLS_HEX_86,
		TLS_HEX_F7, TLS_HEX_0D, TLS_HEX_01, TLS_HEX_01, TLS_HEX_0D
	};
	
	static const uint8 TLS_SIG_RSA_PSS_RSAE_SHA256[TLS_NINE] =
	{
		TLS_HEX_60, TLS_HEX_86, TLS_HEX_48, TLS_HEX_01,
		TLS_HEX_65, TLS_HEX_03, TLS_HEX_04, TLS_HEX_02, TLS_HEX_01
	};
	
	static const uint8 TLS_SIG_RSA_PSS_RSAE_SHA384[TLS_NINE] =
	{
		TLS_HEX_60, TLS_HEX_86, TLS_HEX_48, TLS_HEX_01,
		TLS_HEX_65, TLS_HEX_03, TLS_HEX_04, TLS_HEX_02, TLS_HEX_02
	};
	
	static const uint8 TLS_SIG_RSA_PSS_RSAE_SHA512[TLS_NINE] =
	{
		TLS_HEX_60, TLS_HEX_86, TLS_HEX_48, TLS_HEX_01,
		TLS_HEX_65, TLS_HEX_03, TLS_HEX_04, TLS_HEX_02, TLS_HEX_03
	};



	LpOID = &LaaOID[TLS_ZERO];
    LddReturnValue = Tls_GetOID(LpCertElementData, LpCertElementDataLength, 
	                            &LpOID[TLS_ZERO], &LucOIDLen);
	if (LddReturnValue == E_OK)	
    {
		if (Tls_ChkOID(LpOID, &LucOIDLen, 
		TLS_SIG_RSA_PKCS1_SHA256, (uint16)sizeof(TLS_SIG_RSA_PKCS1_SHA256)))
		{
			*LpMappedCertSigAlgo = (uint16)TLS_RSA_PKCS1_SHA256;
		}
		
		else if (Tls_ChkOID(LpOID, &LucOIDLen, 
		TLS_SIG_RSA_PKCS1_SHA384, (uint16)sizeof(TLS_SIG_RSA_PKCS1_SHA384)))
		{
			*LpMappedCertSigAlgo = (uint16)TLS_RSA_PKCS1_SHA384;
		}
		
		else if (Tls_ChkOID(LpOID, &LucOIDLen, 
		TLS_SIG_RSA_PKCS1_SHA512, (uint16)sizeof(TLS_SIG_RSA_PKCS1_SHA512)))
		{
			*LpMappedCertSigAlgo = (uint16)TLS_RSA_PKCS1_SHA512;
		}
		
		else if (Tls_ChkOID(LpOID, &LucOIDLen, 
		TLS_SIG_RSA_PSS_RSAE_SHA256, (uint16)sizeof(TLS_SIG_RSA_PSS_RSAE_SHA256)))
		{
			*LpMappedCertSigAlgo = (uint16)TLS_RSA_PSS_RSAE_SHA256;
		}
		
		else if (Tls_ChkOID(LpOID, &LucOIDLen, 
		TLS_SIG_RSA_PSS_RSAE_SHA384, (uint16)sizeof(TLS_SIG_RSA_PSS_RSAE_SHA384)))
		{
			*LpMappedCertSigAlgo = (uint16)TLS_RSA_PSS_RSAE_SHA384;
		}
		
		else if (Tls_ChkOID(LpOID, &LucOIDLen, 
		TLS_SIG_RSA_PSS_RSAE_SHA512, (uint16)sizeof(TLS_SIG_RSA_PSS_RSAE_SHA512)))
		{
			*LpMappedCertSigAlgo = (uint16)TLS_RSA_PSS_RSAE_SHA512;
		}
		
		else 
		{
			LddReturnValue = E_NOT_OK;
		}
	}
	else
	{
		LddReturnValue = E_NOT_OK;
	}
	
    return LddReturnValue;	
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_GetOID                                         **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Prepares the referred PSK,s to connection in the buffer **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0362                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_GetOID(const uint8* const LpCertElementData,
                                      const uint32* const LpCertElementDataLength,
									  uint8* const LpOID,
									  uint16* const LpOIDLen)
{
    uint16  LusCertDataLen;
    uint16  LusIndex; 
    Std_ReturnType  LddReturnValue;
	uint16  LusOIDLen;
    LusIndex = (uint16)TLS_ZERO;
    LddReturnValue = E_NOT_OK;
	
	if ((NULL_PTR != LpCertElementData) && 
	   ((uint32)TLS_ZERO < *LpCertElementDataLength))
	{
		if (LpCertElementData[LusIndex] == TLS_FOURTYEIGHT)
	    {
	    	LusIndex += (uint16)TLS_ONE;
	    	LusCertDataLen = LpCertElementData[LusIndex];
	    	
			LusIndex += (uint16)TLS_ONE;
	    	if ((LpCertElementData[LusIndex] == TLS_SIX) 
				&& (LusCertDataLen != TLS_ZERO))
	    	{
	    		LusIndex += (uint16)TLS_ONE;
				
				LusOIDLen = (uint16)LpCertElementData[LusIndex];
	    		
	    		*LpOIDLen = LusOIDLen;
	    		
				LusIndex += (uint16)TLS_ONE;
	    		
                Tls_MemCpy(&LpOID[TLS_ZERO], &LpCertElementData[LusIndex],
                                                                LusOIDLen); 
	    		LddReturnValue = E_OK;
	    	}
	    }
	}
	
	return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"

/*******************************************************************************
** Function Name        : Tls_ChkOID                                         **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : Prepares the referred PSK,s to connection in the buffer **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Non Reentrant                                       **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0361                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static boolean  Tls_ChkOID(const uint8* const LpCertElementData,
                                      const uint16* const LpCertElementDataLength,
									  const uint8* const LpOID,
									  const uint16 LucOIDLen)
{
    boolean  LblOIDFound;
	
	LblOIDFound = TLS_FALSE;
	
	if ((*LpCertElementDataLength == LucOIDLen) && 
	(memcmp(LpCertElementData, LpOID, LucOIDLen) == 0))
	{
		LblOIDFound = TLS_TRUE;
	}
	
	return LblOIDFound;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_ValidateCVMSg                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function retrives the certificate from keym and 
                            places it in the handshake buffer. **   
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0368                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ValidateCVMSg(const uint16 LusActiveConnIdx, uint16* const LpCVHSLen)
{
	Std_ReturnType LddReturnValue;
	Tls_FrameType* LpTlsFrame;
    TcpIp_TlsConnectionIdType LddConnectionId;
    Tls_ConnectionType LddTlsConnectionType;
	KeyM_CertificateIdType LddRemoteCertId;
	KeyM_CertElementIdType LddRemoteCertElemtId;
	uint32 LulJobId;
	uint32 LulPubKeyId;
	uint32 LulHSHdrLen;
	uint32 LulPublicKeyLen;
	const uint8* LpSignToVerify;
	const uint8* LpContextString;
	const uint8* LpHShakeBuf;
	uint8 LaaTranscriptHash[TLS_FOURTYEIGHT];
	uint8 LaaPublicKey[TLS_MAX_CERTIFICATE_ELEMENT_SIZE];
    uint16 LusTranscriptHashLen;
	uint8 LaaCertVerifyMsg[TLS_MAX_HASH_CERTIFICATE_VERIFY];
	uint16 LusCertVerifyMsgLen;
	uint16 LusHSEndPos;
	uint16 LusRecivedSign;
	uint16 LusRecivedSignLen;
	uint16 LusNegotiatedSigAlgoIdx;
	boolean Lblstatus;
	
	LusCertVerifyMsgLen = (uint16)TLS_ZERO;
    LddConnectionId = Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
	LddTlsConnectionType = Tls_GaaConnection[LddConnectionId].ddTlsConnectionType;

	LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];
	/* this will tell us where the Certificate Verify HS starts */
	LusHSEndPos = LpTlsFrame->usProcessedHSBytes;
	LpHShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusHSEndPos];

	if ((uint8)TLS_CERTIFICATE_VERIFY == (*LpHShakeBuf))
	{
		/* get the total length of the handshake message */
		LulHSHdrLen =
		((uint32)TLS_ZERO  << (uint8)TLS_TWENTYFOUR) |
		((uint32)LpHShakeBuf[TLS_ONE]  << (uint8)TLS_SIXTEEN) |
		((uint32)LpHShakeBuf[TLS_TWO]  << (uint8)TLS_EIGHT)  |
		((uint32)LpHShakeBuf[TLS_THREE]); 
		
		*LpCVHSLen = (uint16)((uint16)LulHSHdrLen + (uint16)TLS_FOUR);

		if (
		(LulHSHdrLen != (uint32)TLS_ZERO) && 
		(LulHSHdrLen <= (uint32)((uint32)TLS_MAX_CERTVERIFY_LEN + (uint32)TLS_FOUR)))
		{
			/* now check if the recived Signature algorithm is the one sent 
			in Client Hello */
			LusRecivedSign = 
                ((uint16)((uint16)LpHShakeBuf[TLS_FOUR] << (uint16)TLS_EIGHT) | 
                            (uint16)LpHShakeBuf[TLS_FIVE]);	

			Lblstatus = Tls_CheckSupportedSigAlgo(LusRecivedSign, LusActiveConnIdx);

			if (Lblstatus)
			{
				LusRecivedSignLen = 
                ((uint16)((uint16)LpHShakeBuf[TLS_SIX] << (uint16)TLS_EIGHT) | 
                            (uint16)LpHShakeBuf[TLS_SEVEN]);	

				if ((LusRecivedSignLen != (uint32)TLS_ZERO) && 
						(LusRecivedSignLen <= (uint32)TLS_MAX_CERTVERIFY_LEN))
				{
					/* Step 1: Prepare transcript hash calculation */
    				LddReturnValue = 
					Tls_ComputeTranscriptHash(LusActiveConnIdx, 
									LaaTranscriptHash, &LusTranscriptHashLen);
					
					if (E_OK == LddReturnValue)
					{
						/*  A string that consists of octet 32 (0x20) repeated 64 times */
						Tls_MemCpy(&LaaCertVerifyMsg[LusCertVerifyMsgLen], 
											LaaSignPadding, (uint16)TLS_HEX_40);

						LusCertVerifyMsgLen += (uint16)TLS_HEX_40;



						/* Context: 32 bytes, always zero-padded */
						if (LddTlsConnectionType == TLS_SERVER)
						{
							LpContextString = TLS13_CLIENT_CERT_VERIFY;
						}
						else
						{
							LpContextString = TLS13_SERVER_CERT_VERIFY;
						}

						Tls_MemCpy(&LaaCertVerifyMsg[LusCertVerifyMsgLen],
									LpContextString, TLS_MAX_CONTEXTSTRING_SIZE);

						LusCertVerifyMsgLen += (uint16)TLS_MAX_CONTEXTSTRING_SIZE;



						/* A single 0 byte which serves as the separator */
						LaaCertVerifyMsg[LusCertVerifyMsgLen] = 
																(uint8)TLS_ZERO;
															
						LusCertVerifyMsgLen++;

																
						/* The content to be signed */										
						Tls_MemCpy(&LaaCertVerifyMsg[LusCertVerifyMsgLen],
										LaaTranscriptHash, LusTranscriptHashLen);

						LusCertVerifyMsgLen += LusTranscriptHashLen;


						/* Signature start at this position */				
						LpSignToVerify = &LpHShakeBuf[TLS_EIGHT];

						LusNegotiatedSigAlgoIdx = 
						Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedSigAlgoIdx;
						
						LulJobId = 
						Tls_GaaHandshake[LusNegotiatedSigAlgoIdx].
									ulTlsCsmKeyExchangeSignatureVerifyJobRefIdx;

						LddRemoteCertId = 
						Tls_GaaCertificateIdentity[Tls_GaaConnection[
						LddConnectionId].ulTlsCertIdnRefIdx].
						ddTlsCipherKeyMRemoteCertificateIdx;

						/*  Get the configured Element ID for the certificate
						which is configured for the public key */	
						LddRemoteCertElemtId = 
						Tls_GaaCertificateIdentity[Tls_GaaConnection[
						LddConnectionId].ulTlsCertIdnRefIdx].
						ddTlsCipherKeyMRemoteCertElementPublicKeyIdx;

						/* get the public key from Cert */
						/*  Get the certificate element data and Certificate data length */
						LddReturnValue = KeyM_CertElementGet(
									LddRemoteCertId,
									LddRemoteCertElemtId,
									&LaaPublicKey[TLS_ZERO],
									&LulPublicKeyLen);

						if ((E_OK == LddReturnValue) && 
										(LulPublicKeyLen != (uint32)TLS_ZERO))
						{
							LulPubKeyId = Tls_GaaHandshake[LusNegotiatedSigAlgoIdx].
									ulTlsCsmKeyExchangeSignatureVerifyKeyRefIdx;

							LddReturnValue = Tls_PerformSignatureVerify(
													LulJobId,
													LulPubKeyId,
													&LaaPublicKey[TLS_ZERO],
													(uint16)LulPublicKeyLen,
													LaaCertVerifyMsg,
													(uint32)LusCertVerifyMsgLen,
													LpSignToVerify,
													(uint32)LusRecivedSignLen);

							if (E_OK != LddReturnValue)
							{
								/* decrypt_error:  A handshake (not record layer) cryptographic
								operation failed, including being unable to correctly verify a
								signature or validate a Finished message or a PSK binder. */
								
								Tls_GaaAlert[LusActiveConnIdx] = 
															TLS_DECRYPT_ERROR;
							}
						}
						else
						{
							Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
							LddReturnValue = E_NOT_OK;	
						}	
					}
					else
					{
						Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
						LddReturnValue = E_NOT_OK;						
					}
				}
				else
				{
					Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
					LddReturnValue = E_NOT_OK;						
				}
			}
			else
			{
				Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
				LddReturnValue = E_NOT_OK;				
			}
		}
		else
		{
        	Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
        	LddReturnValue = E_NOT_OK;		
		}
	}
	else
	{
        Tls_GaaAlert[LusActiveConnIdx] = TLS_UNEXPECTED_MESSAGE;
        LddReturnValue = E_NOT_OK;		
	}
	return LddReturnValue;
}/* end of Tls_ValidateCVMSg */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/* Assisted by GitHub Copilot */
/*******************************************************************************
** Function Name        : Tls_PrepCVHSMessage                        **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function prepares certificate verify message.  **
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0367                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType Tls_PrepCVHSMessage(const uint16 LusActiveConnIdx)
{
    Std_ReturnType LddReturnValue;
    TcpIp_TlsConnectionIdType LddConnectionId;
	Tls_ConnectionType LddTlsConnectionType;
    uint32 LulJobId;
    uint32 LulKeyId;
    uint32 LulDataLength;
    uint32 LulSignatureLength;
    uint32 LulMsgLen;
    const uint8* LpContextString;
    uint8* LpHShakeBuf;
    uint16 LusTranscriptHashLen;
    uint16 LusNegotiatedSigAlgoIdx;
    uint16 LusCertVerifyMsgLen;
    uint16 LusSignatureLen;
    uint16 LusHandshakeBufIdx;
    uint16 LusHandshakeMsgLen;
    uint16 LusSignatureScheme;
    uint8 LaaTranscriptHash[TLS_FOURTYEIGHT];
	/* why 146 : 64 bytes padding + 33 (client\server string context) + 1 (byte seprator) + 48 (cipher)   */
    uint8 LaaCertVerifyMssg[TLS_MAX_HASH_CERTIFICATE_VERIFY];
    uint8 LaaSignature[TLS_MAX_SIZE_512];


    /* Initialize required variables to default values */
    LusCertVerifyMsgLen = (uint16)TLS_ZERO;
    LusHandshakeMsgLen = (uint16)TLS_ZERO;
    LddConnectionId = Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
	LddTlsConnectionType = Tls_GaaConnection[LddConnectionId].ddTlsConnectionType;



    /* Step 1: Prepare transcript hash calculation */
    LddReturnValue = Tls_ComputeTranscriptHash(LusActiveConnIdx, 
									LaaTranscriptHash, &LusTranscriptHashLen);

    /* Step 2: Choose the context based on role */
    if (E_OK == LddReturnValue)
    {
        /* Padding: 64 bytes */
        Tls_MemCpy(&LaaCertVerifyMssg[LusCertVerifyMsgLen], LaaSignPadding, (uint16)TLS_HEX_40);
        LusCertVerifyMsgLen += (uint16)TLS_HEX_40;


        /* Context: 32 bytes, always zero-padded */
        if (LddTlsConnectionType == TLS_SERVER)
        {
            LpContextString = TLS13_SERVER_CERT_VERIFY;
        }
        else
        {
            LpContextString = TLS13_CLIENT_CERT_VERIFY;
        }

        Tls_MemCpy(&LaaCertVerifyMssg[LusCertVerifyMsgLen], LpContextString,
													TLS_MAX_CONTEXTSTRING_SIZE);

        LusCertVerifyMsgLen += (uint16)TLS_MAX_CONTEXTSTRING_SIZE;


        LaaCertVerifyMssg[LusCertVerifyMsgLen] = (uint8)TLS_ZERO;
		LusCertVerifyMsgLen++;


        Tls_MemCpy(&LaaCertVerifyMssg[LusCertVerifyMsgLen], LaaTranscriptHash,
														LusTranscriptHashLen);

        LusCertVerifyMsgLen += LusTranscriptHashLen;


        /* Step 3: Perform signature generation for CertificateVerify (common for client/server) */
        LusNegotiatedSigAlgoIdx = 
				Tls_HandshakeFrame[LusActiveConnIdx].usNegotiatedSigAlgoIdx;
				
        LulJobId = Tls_GaaHandshake[LusNegotiatedSigAlgoIdx].
								ulTlsCsmKeyExchangeSignatureGenerateJobRefIdx;

        LulKeyId = Tls_GaaHandshake[LusNegotiatedSigAlgoIdx].
								ulTlsCsmKeyExchangeSignatureGenerateKeyRefIdx;
        
        LulDataLength = (uint32)LusCertVerifyMsgLen;
        
        LddReturnValue = Tls_PerformSignatureGeneration(
														LulJobId,
														LulKeyId,
														LaaCertVerifyMssg,
														LulDataLength,
														LaaSignature,
														&LulSignatureLength);

        /* Step 4: Prepare handshake buffer and transmit (server only) */
        if (E_OK == LddReturnValue)
        {
			LusSignatureLen = (uint16)LulSignatureLength;

            /* Use handshake buffer position for assignment */
            LusHandshakeBufIdx = Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition;
            LpHShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusHandshakeBufIdx];
            LusSignatureScheme = Tls_GaaHandshake[LusNegotiatedSigAlgoIdx].usTlsSignatureId;
            
            /* Add handshake header: HandshakeType, Length, etc. */
            LpHShakeBuf[LusHandshakeMsgLen] = (uint8)TLS_CERTIFICATE_VERIFY;
			LusHandshakeMsgLen++;
            
            /* Length: 2 (scheme) + 2 (sig len) + signature */
            LulMsgLen = (uint32)TLS_TWO + (uint32)TLS_TWO + LusSignatureLen;
            LpHShakeBuf[LusHandshakeMsgLen] = (uint8)((LulMsgLen >> (uint32)TLS_SIXTEEN) & TLS_LSB_MASK);
			LusHandshakeMsgLen++;
            LpHShakeBuf[LusHandshakeMsgLen] = (uint8)((LulMsgLen >> (uint32)TLS_EIGHT) & TLS_LSB_MASK);
			LusHandshakeMsgLen++;
            LpHShakeBuf[LusHandshakeMsgLen] = (uint8)(LulMsgLen & TLS_LSB_MASK);
			LusHandshakeMsgLen++;
            
            /* SignatureScheme (2 bytes, network order) */		
			LpHShakeBuf[LusHandshakeMsgLen] = 
			(uint8)((uint16)(LusSignatureScheme >> (uint16)TLS_EIGHT) & 
															(uint16)TLS_LSB_MASK);
															
			LusHandshakeMsgLen++;
            LpHShakeBuf[LusHandshakeMsgLen] = (uint8)(LusSignatureScheme & TLS_LSB_MASK);
			LusHandshakeMsgLen++;
            
            /* Signature length (2 bytes, network order) */
			LpHShakeBuf[LusHandshakeMsgLen] = 
			(uint8)((uint16)(LusSignatureLen >> (uint16)TLS_EIGHT) & 
															(uint16)TLS_LSB_MASK);
															
			LusHandshakeMsgLen++;
            LpHShakeBuf[LusHandshakeMsgLen] = (uint8)(LusSignatureLen & TLS_LSB_MASK);
			LusHandshakeMsgLen++;
            
            /* Signature */
            Tls_MemCpy(&LpHShakeBuf[LusHandshakeMsgLen], LaaSignature, LusSignatureLen);
            LusHandshakeMsgLen += LusSignatureLen;
				
			/*  1 Last Byte Content field */
			LpHShakeBuf[LusHandshakeMsgLen] = (uint8)TLS_HANDSHAKE;
			LusHandshakeMsgLen++;
            
            LddReturnValue = Tls_TransmitRecord(
												LusActiveConnIdx,
												TLS_APPLICATION_DATA,
												&LpHShakeBuf[TLS_ZERO],
												LusHandshakeMsgLen);
											
			if (E_OK == LddReturnValue)
			{
				/* shd leave the last content byte */
				Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition += 
				(uint16)(LusHandshakeMsgLen - (uint32)TLS_ONE);
				
				Tls_HandshakeFrame[LusActiveConnIdx].usProcessedHSBytes += 
				(uint16)(LusHandshakeMsgLen - (uint32)TLS_ONE);
			}
			else
			{
				Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
				LddReturnValue = E_NOT_OK;
			}	
        }
		else
		{
			Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
			LddReturnValue = E_NOT_OK;
		}
    }
	else
	{
		Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
		LddReturnValue = E_NOT_OK;
	}

	return LddReturnValue;
}
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_ProcessCertRequestMsg                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function retrives the certificate from keym and 
                            places it in the handshake buffer. **   
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0364                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ProcessCertRequestMsg(const uint16 LusActiveConnIdx)
{

	/* for now in the Extensions
	only Signature Algorithms extension we are adding */
    Std_ReturnType LddReturnValue;
    TcpIp_TlsConnectionIdType LddConnectionId;
	uint8* LpHShakeBuf;
	uint16 LusCertReqIdx;
	uint16 LusNumOfHSWrkrs;
	uint16 LusTemp;
	uint16 LusHSWrkrCtr;
    uint16 LusHSWrkrStartIdx;
    uint16 LusHSWrkrLookUpIdx;
	uint16  LusIndex;
	

    LddConnectionId = 
    Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;

	LusNumOfHSWrkrs = Tls_GaaConnection[LddConnectionId].usNumOfHSWorkers;
	LusHSWrkrStartIdx = Tls_GaaConnection[LddConnectionId].usHSWrkrIdx;

	/* from where to store the CERT Request message */
    LusCertReqIdx = Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition; 
	LpHShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusCertReqIdx];

	/* Tag */
	LusIndex = (uint16)TLS_ZERO;
	LpHShakeBuf[LusIndex] = (uint8)TLS_CERTIFICATE_REQUEST; 

    /* HandShake Header Field */
    LusIndex += (uint16)TLS_ONE;  
	LpHShakeBuf[LusIndex] = (uint8)TLS_ZERO;   

    /* HandShake Header Field second and third byte*/
    LusIndex += (uint16)TLS_ONE;  
    LusTemp = LusNumOfHSWrkrs * (uint16)TLS_TWO;
    
    LusTemp = Tls_Htons(LusTemp + (uint16)TLS_NINE);
    Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO);  

	/* certificate_request_context */
    LusIndex += (uint16)TLS_TWO;  
	LpHShakeBuf[LusIndex] = (uint8)TLS_ZERO; 

	/* Extensions Length */
    LusIndex += (uint16)TLS_ONE;
	LusTemp = LusNumOfHSWrkrs * (uint16)TLS_TWO;
	LusTemp = Tls_Htons(LusTemp + (uint16)TLS_SIX);
	Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO); 



    /* Signature Algorithm extension : Tag */
    LusIndex += (uint16)TLS_TWO;
    LusTemp = Tls_Htons((uint16)TLS_CH_SIGNATURE_ALGORITHMS);
    Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO);
        

    /* Signature Algorithm extension : Main Length */
    LusIndex += (uint16)TLS_TWO;    
    LusTemp = LusNumOfHSWrkrs * (uint16)TLS_TWO;
    /* why +2 : its the len inside the value field */
    LusTemp = Tls_Htons(LusTemp + (uint16)TLS_TWO);
    Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO);   
 

    /* Signature Algorithm extension : VALUE : LEN */
    LusIndex += (uint16)TLS_TWO; 
    LusTemp = Tls_Htons(LusNumOfHSWrkrs * (uint16)TLS_TWO);
    Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO); 


    /* Signature Algorithm extension : VALUE : Actual */
    LusIndex += (uint16)TLS_TWO; 
    for (LusHSWrkrCtr = (uint16)TLS_ZERO; LusHSWrkrCtr < LusNumOfHSWrkrs;
                                                            LusHSWrkrCtr++)
    { 
        LusHSWrkrLookUpIdx = Tls_GaaHSLookUp[LusHSWrkrStartIdx + LusHSWrkrCtr];

		/* convert and copy */
        LusTemp = 
        Tls_Htons(Tls_GaaHandshake[LusHSWrkrLookUpIdx].usTlsSignatureId);
		
		Tls_MemCpy(&LpHShakeBuf[LusIndex], &LusTemp, (uint16)TLS_TWO);
        
        LusIndex += (uint16)TLS_TWO;
    }
                          
	/* Last byte shd be content as per rfc */
	LpHShakeBuf[LusIndex] = (uint8)TLS_HANDSHAKE;
			
	LusTemp = LusNumOfHSWrkrs * (uint16)TLS_TWO;
    LusTemp = LusTemp + (uint16)TLS_FOURTEEN;

	LddReturnValue = Tls_TransmitRecord(
										LusActiveConnIdx,
										TLS_APPLICATION_DATA,
										&LpHShakeBuf[TLS_ZERO],
										LusTemp); 
													
	if (E_OK == LddReturnValue)
	{
		/* shd leave the last content byte */
		Tls_HandshakeFrame[LusActiveConnIdx].usHShakeBufferPosition += 
		(uint16)(LusTemp - (uint16)TLS_ONE);
						
		Tls_HandshakeFrame[LusActiveConnIdx].usProcessedHSBytes += 
		(uint16)(LusTemp - (uint16)TLS_ONE);
	}
	else
	{
		Tls_GaaAlert[LusActiveConnIdx] = TLS_INTERNAL_ERROR;
		LddReturnValue = E_NOT_OK;
	}
	
    return LddReturnValue;
}/* end of Tls_ProcessCertRequestMsg */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"



/*******************************************************************************
** Function Name        : Tls_ValidateCertReqMSg                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function retrives the certificate from keym and 
                            places it in the handshake buffer. **   
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0368                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
Std_ReturnType  Tls_ValidateCertReqMSg(const uint16 LusActiveConnIdx, uint16* const LpCertReqHSLen)
{
	Std_ReturnType LddReturnValue;
	Tls_FrameType*  LpTlsFrame;
	uint32 LulHSHdrLen;
	uint8* LpHShakeBuf;
	uint16 LusExtLen;
	uint16 LusHSEndPos;
	uint16 LusIndex;
	uint16 LusSummedExtLen;
	uint16 LusTotalExtLen;
	uint16 LusTag;

	LusSummedExtLen = (uint16)TLS_ZERO;
	LpTlsFrame = &Tls_HandshakeFrame[LusActiveConnIdx];
	/* this will tell us where the Certificate Request HS starts */
	LusHSEndPos = LpTlsFrame->usProcessedHSBytes;
	LpHShakeBuf = &Tls_GaaHShakeBuffer[LusActiveConnIdx][LusHSEndPos];

	if ((uint8)TLS_CERTIFICATE_REQUEST == (*LpHShakeBuf))
	{
		/* get the total length of the handshake message */
		LulHSHdrLen =
		((uint32)TLS_ZERO  << (uint8)TLS_TWENTYFOUR) |
		((uint32)LpHShakeBuf[TLS_ONE]  << (uint8)TLS_SIXTEEN) |
		((uint32)LpHShakeBuf[TLS_TWO]  << (uint8)TLS_EIGHT)  |
		((uint32)LpHShakeBuf[TLS_THREE]); 
		
		*LpCertReqHSLen = (uint16)((uint16)LulHSHdrLen + (uint16)TLS_FOUR);

		if (LulHSHdrLen != (uint32)TLS_ZERO)
		{
			/* Move to Certificate Request Contxte length */
			LusIndex = (uint16)TLS_FOUR;
			/* Certificate Request Contxte length shd be zero */
			if ((uint8)TLS_ZERO == LpHShakeBuf[LusIndex])
			{
				/* Move to Total Extension Length */
				LusIndex += (uint16)TLS_ONE;
				/* Get the Total Extension Length */
				LusTotalExtLen = 
                ((uint16)((uint16)LpHShakeBuf[LusIndex] << (uint16)TLS_EIGHT) | 
                            (uint16)LpHShakeBuf[LusIndex + (uint8)TLS_ONE]);	

				if ((LusTotalExtLen != (uint16)TLS_ZERO) && 
				((uint32)(LusTotalExtLen + (uint32)TLS_THREE) == LulHSHdrLen))
				{
					/* Move to first extension */
					LusIndex += (uint16)TLS_TWO;
					
				    do
					{
						LddReturnValue = 
						Tls_CheckSupportedTag(TLS_CERTIFICATE_REQUEST, 
														&LpHShakeBuf[LusIndex]);
						
						if (E_OK == LddReturnValue)
						{
							LusTag = 
							((uint16)((uint16)LpHShakeBuf[LusIndex] << 
															(uint16)TLS_EIGHT) | 
                            (uint16)LpHShakeBuf[LusIndex + (uint16)TLS_ONE]); 

							/* why this here : get the len of the extension, even if its 
							suported or not by us */
							LusExtLen = ((uint16)((uint16)LpHShakeBuf[LusIndex 
									+ (uint16)TLS_TWO] << (uint16)TLS_EIGHT) | 
							(uint16)LpHShakeBuf[LusIndex + (uint16)TLS_THREE]);	
							
							
							if ((uint16)TLS_CH_SIGNATURE_ALGORITHMS == LusTag)
							{
								/* LpHShakeBuf starts at start of the tag */
								LddReturnValue = Tls_ValidateCRSign(LusActiveConnIdx, &LpHShakeBuf[LusIndex]);
							}
						
							/* for now if its other the signature algo , then Skip */
							/* even if it is supported or not supported , we have move to 
							next Tag */          
							LusIndex += (uint16)(TLS_FOUR + LusExtLen);	
							
							/* Keep Tracking it, so that All extension summed up should be 
							equal to Total extension size */
							LusSummedExtLen += (uint16)(TLS_FOUR + LusExtLen);
						}
						else
						{
							Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
							LddReturnValue = E_NOT_OK;  							
						}
						
					}while ((LusSummedExtLen < LusTotalExtLen) && (E_OK == LddReturnValue));
				}
				else
				{
					Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
					LddReturnValue = E_NOT_OK;						
				}
			}
			else
			{
				Tls_GaaAlert[LusActiveConnIdx] = TLS_ILLEGAL_PARAMETER;
				LddReturnValue = E_NOT_OK;				
			}
		}
		else
		{
        	Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
        	LddReturnValue = E_NOT_OK;		
		}
	}
	else
	{
        Tls_GaaAlert[LusActiveConnIdx] = TLS_UNEXPECTED_MESSAGE;
        LddReturnValue = E_NOT_OK;		
	}
	
	return LddReturnValue;
}/* end of Tls_ValidateCertReqMSg */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


/*******************************************************************************
** Function Name        : Tls_ValidateCRSign                           **
**                                                                            **
** Service ID           : --                                                  **
**                                                                            **
** Description          : This function retrives the certificate from keym and 
                            places it in the handshake buffer. **   
**                                                                            **
** Sync/Async           : Synchronous                                         **
**                                                                            **
** Reentrancy           : Reentrant                                           **
**                                                                            **
** Input Parameters     : None                                                **
**                                                                            **
** InOut Parameters     : None                                                **
**                                                                            **
** Output Parameters    : None                                                **
**                                                                            **
** Return parameter     : None                                                **
**                                                                            **
** Preconditions        : None                                                **
**                                                                            **
** Remarks              : Global Variable(s)     :                            **
**                                                                            **
**                        Function(s) invoked    :                            **
**                                                                            **
**  Design ID           : TLS_SDD_0368                                        **
*******************************************************************************/
#define TLS_START_SEC_CODE
#include "Tls_MemMap.h"
static Std_ReturnType  Tls_ValidateCRSign(const uint16 LusActiveConnIdx, 
												const uint8* const LpHShakeBuf)
{
	Std_ReturnType LddReturnValue;
	TcpIp_TlsConnectionIdType  LddConnectionId;
	uint16 LusExtLen;
	uint16 LusIndex;
	uint16 LusSecondryLen;
	uint16 LusHSWrkrStartIdx;
	uint16 LusNumOfHSWrkrs;
	uint16 LusHSWrkrCtr;
	uint16 LusCRSign;
	uint16 LusHSWrkrLookUpIdx;
	uint16 LusClientSign;
	boolean Lblstatus;
	boolean LblSignFound;

	
	/* Validate Signature extension */
	LblSignFound = TLS_FALSE;
	LusIndex = (uint16)TLS_TWO;
	/* becuz of tasking warning */
	LddReturnValue = E_NOT_OK;
	
	/* get the extension length */
	LusExtLen = ((uint16)((uint16)LpHShakeBuf[LusIndex] << (uint16)TLS_EIGHT) | 
                            (uint16)LpHShakeBuf[LusIndex + (uint16)TLS_ONE]); 
							
	if ((LusExtLen >= (uint16)TLS_FOUR) && (LusExtLen <= (uint16)TLS_MAX_SG_SH_KS_LEN)
		&& ((LusExtLen & ((uint16)TLS_ONE)) == ((uint16)TLS_ZERO)))
	{
		/* Move to Seconday Len field */
		LusIndex += (uint16)TLS_TWO;
		LusSecondryLen = Tls_GetExtensionLength(&LpHShakeBuf[LusIndex]); 
		
		if (
			((LusSecondryLen >= (uint16)TLS_TWO) 
			&& 
			(LusSecondryLen <= ((uint16)TLS_MAX_SG_SH_KS_LEN - (uint16)TLS_TWO)))
			&&
			((LusSecondryLen & (uint16)TLS_ONE) == (uint16)TLS_ZERO)
			&&
			(LusExtLen == (uint16)(LusSecondryLen + (uint16)TLS_TWO))
			)
		{
			LddConnectionId = 
				Tls_GddActiveConnManager[LusActiveConnIdx].ddTlsConnectionId;
	
			LusHSWrkrStartIdx = Tls_GaaConnection[LddConnectionId].usHSWrkrIdx;
	
			LusNumOfHSWrkrs = Tls_GaaConnection[LddConnectionId].usNumOfHSWorkers;   
			
			do
			{
				/* move to each signture */
				LusIndex += (uint16)TLS_TWO;
				
				LusCRSign = 
				((uint16)((uint16)LpHShakeBuf[LusIndex] << (uint16)TLS_EIGHT) | 
							(uint16)LpHShakeBuf[LusIndex + (uint16)TLS_ONE]); 
														
				Lblstatus = Tls_CheckSignatureAlgoValue(LusCRSign);  
				
				if (Lblstatus)
				{
					for (LusHSWrkrCtr = (uint16)TLS_ZERO; LusHSWrkrCtr < 
												LusNumOfHSWrkrs; LusHSWrkrCtr++)
					{ 
						LusHSWrkrLookUpIdx = 
							Tls_GaaHSLookUp[LusHSWrkrStartIdx + LusHSWrkrCtr];
						
						LusClientSign = 
						Tls_GaaHandshake[LusHSWrkrLookUpIdx].usTlsSignatureId;
						
						if (LusClientSign == LusCRSign)
						{                           
							LblSignFound = TLS_TRUE;
							LddReturnValue = E_OK;
							break;						
						}
					}                    
				}
				LusSecondryLen -= (uint16)TLS_TWO;
			} while ((LusSecondryLen > (uint16)TLS_ZERO) && (!LblSignFound));
	
			if (!LblSignFound)
			{
				/* If the server is unable to negotiate a supported set of parameters
				(i.e., there is no overlap between the client and server parameters),
				it MUST abort the handshake with either a "handshake_failure" or
				"insufficient_security" fatal alert */      
				
				Tls_GaaAlert[LusActiveConnIdx] = TLS_HANDSHAKE_FAILURE;
				LddReturnValue = E_NOT_OK;                 
			}
		}
		else
		{
			Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
			LddReturnValue = E_NOT_OK;               
		}
	}
	else
	{
		Tls_GaaAlert[LusActiveConnIdx] = TLS_DECODE_ERROR;
		LddReturnValue = E_NOT_OK;         
	}
						
	
	return LddReturnValue;
}/* end of Tls_ValidateCRSign */
#define TLS_STOP_SEC_CODE
#include "Tls_MemMap.h"


#endif

/* parasoft-end-suppress CERT_C-API00-a-3 */
/* parasoft-end-suppress CERT_C-DCL19-a-3 */
/* parasoft-end-suppress MISRAC2012-RULE_20_1-a-4 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-d-2 */
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 */
