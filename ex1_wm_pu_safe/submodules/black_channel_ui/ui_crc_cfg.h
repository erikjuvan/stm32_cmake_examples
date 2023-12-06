////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
* @file     ui_crc_cfg.h
* @brief    CRC configuration file.
* @author   Matej Otic
* @date     03.06.2021
* @version  V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
* @addtogroup CRC_CONFIGURATION
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __UI_CRC_CFG_H
#define __UI_CRC_CFG_H

////////////////////////////////////////////////////////////////////////////////
// CRC settings
////////////////////////////////////////////////////////////////////////////////

/**
 * Enable/disable use of CRC-8/ITU.
 * - Polynomial: 0x07
 * - Reflected: No
 * - Initial: 0x00
 * - XOR: 0x55
 */
#define CRC_USE_CRC_8_ITU                   ( 1 )

/**
 * Enable/disable use of CRC-16/ARC.
 * - Polynomial: 0x8005
 * - Reflected: Yes
 * - Initial: 0x0000
 * - XOR: 0x0000
 */
#define CRC_USE_CRC_16_ARC                  ( 1 )

/**
 * Enable/disable use of CRC-16/KERMIT.
 * - Polynomial: 0x1021
 * - Reflected: Yes
 * - Initial: 0x0000
 * - XOR: 0x0000
 */
#define CRC_USE_CRC_16_KERMIT               ( 1 )

/**
 * Enable/disable use of CRC-16/MODBUS.
 * - Polynomial: 0x8005
 * - Reflected: Yes
 * - Initial: 0xFFFF
 * - XOR: 0x0000
 */
#define CRC_USE_CRC_16_MODBUS               ( 1 )

/**
 * Enable/disable use of CRC-32/ISO-HDLC.
 * - Polynomial: 0x04C11DB7
 * - Reflected: Yes
 * - Initial: 0xFFFFFFFF
 * - XOR: 0xFFFFFFFF
 */
#define CRC_USE_CRC_32_ISO_HDLC             ( 1 )

////////////////////////////////////////////////////////////////////////////////
// Debug settings
////////////////////////////////////////////////////////////////////////////////

/**
 * Define if ASSERTs are enabled or not.
 */
#define CRC_ASSERT_ENABLE                   ( 1 )


// parasoft-begin-suppress FORMAT-02 FORMAT-03 FORMAT-34  "Used only for debugging"
#if (CRC_ASSERT_ENABLE)
    #define CRC_ASSERT(x)                   if (!(x))   { while(1){} }
#else
    #define CRC_ASSERT(x)                   {;}
#endif
// parasoft-end-suppress FORMAT-02 FORMAT-03 FORMAT-34

#endif // __UI_CRC_CFG_H
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
