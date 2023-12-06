////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   ico_lss.h
*@brief  contains internal defines for LSS
*@author Zdenko Mezgec
*@author EMTAS
*@date   03.12.2020
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup CANOPEN
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef ICO_LSS_H
#define ICO_LSS_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/* LSS command specifier */
#define LSS_CS_SWITCH_GLOBAL				0x04u
#define LSS_CS_NODE_ID						0x11u
#define LSS_CS_BITRATE                      0x13u
#define LSS_CS_ACTIVATE_BITRATE             0x15u
#define LSS_CS_STORE						0x17u
#define LSS_CS_SWITCH_SEL_VENDOR			0x40u
#define LSS_CS_SWITCH_SEL_PRODUCT			0x41u
#define LSS_CS_SWITCH_SEL_REVISION			0x42u
#define LSS_CS_SWITCH_SEL_SERIAL			0x43u
#define LSS_CS_SWITCH_SEL_ANSWER			0x44u
#define LSS_CS_IDENTITY_SLAVE				0x4fu
#define LSS_CS_NON_CONFIG_REMOTE_SLAVE		0x4cu
#define LSS_CS_NON_CONFIG_SLAVE				0x50u
#define LSS_CS_FAST_SCAN					0x51u
#define LSS_CS_INQUIRE_NODEID				0x5eu

#define LSS_MODE_WAITING			0u
#define LSS_MODE_CONFIG				1u

/* LSS can ids */
#define LSS_CAN_ID_SLAVE			0x7e4u
#define LSS_CAN_ID_MASTER			0x7e5u


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void	icoLssResetAppl(void);
void	icoLssReset(UNSIGNED8 *pNodeId);
void	icoLssHandler(const UNSIGNED8 pData[]);
void	icoLssMasterHandler(const UNSIGNED8	pData[]);
void	icoLssResetNodeId(void);
void	icoLssMasterReset(void);


#endif /* ICO_LSS_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
