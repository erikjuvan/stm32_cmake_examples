////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
* @file     func_table.h
* @brief    User and Safe function pointer table.
* @author   Dusan Slavinec
* @date     16.08.2021
* @version  V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
* @addtogroup FUNC_TABLE
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __FUNC_TABLE_H
#define __FUNC_TABLE_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdbool.h>

#include "test_switch_common.h"
#include "safe_version_common.h"
#include "black_channel_common.h"
#include "otdet_common.h"
#include "power_mode_common.h"
#include "mu_safe_common.h"
#include "curr_meas_common.h"
#include "drumspin_common.h"
#include "otdet_common.h"
#include "curr_meas_common.h"
#include "reset_status_log_common.h"

#ifdef UNIT_TESTING
#include "project_config.h"
#endif

////////////////////////////////////////////////////////////////////////////////
// Common definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * 32-bit floating point type.
 */
typedef float float32_t;

////////////////////////////////////////////////////////////////////////////////
// USER function table
////////////////////////////////////////////////////////////////////////////////

//@{
/**
 * USER function defines.
 */
typedef void        ( *pUser_Context_Init_t           )( void );
typedef uint32_t    ( *pUser_App_Init_t               )( void );
typedef void        ( *pUser_App_Loop_t               )( void );

typedef void        ( *pUser_BlackCh_Send_Frame       )( const blackch_frame_t tx_frame );

typedef int         ( *pUser_trace_print              )(int, const char *fmt, ...);
//@}


/**
 * USER function table definition.
 */
typedef struct
{
    pUser_Context_Init_t                fp_User_Context_Init             ;
    pUser_App_Init_t                    fp_User_App_Init                 ;
    pUser_App_Loop_t                    fp_User_App_Loop                 ;

    pUser_BlackCh_Send_Frame            fp_User_BlackCh_Send_Frame       ;

    pUser_trace_print                   fp_User_trace_print;
} user_func_table_t;


////////////////////////////////////////////////////////////////////////////////
// SAFE function table
////////////////////////////////////////////////////////////////////////////////

//@{
/**
 * SAFE function defines.
 */
typedef const char* const               ( *pSafe_Get_Build_Info_t           )( void );
typedef const safe_version_t* const     ( *pSafe_Get_Version_Info_t         )( void );
typedef uint32_t                        ( *pSafe_Get_Crc_t                  )( void );
typedef test_switch_status_t            ( *pSafe_Set_Test_Switches_t        )( const test_switch_t test_switch );

#ifdef BLACKCH_DEVICE_SLAVE
typedef bool                            ( *pSafe_BlackCh_Rcv_n_Resp_Frame_t )( blackch_frame_t  const rx_frame
                                                                              ,blackch_frame_t* const p_tx_frame
                                                                             );
#endif
#ifdef BLACKCH_DEVICE_MASTER
typedef void                            ( *pSafe_BlackCh_Receive_Frame_t    )( blackch_frame_t const rx_frame );
#endif
typedef blackch_status_t                ( *pSafe_Blckch_Get_Status_t        )( void );

typedef bool                            ( *pSafe_Can_Drive_Motor_t          )(void);

typedef uint32_t                        ( *pSafe_Get_ADC1_DMA1Ch1_CMAR_t    )( void );

typedef void                            ( *pSafe_Set_Power_Mode_t           )( const power_mode_t mode );
typedef power_mode_t                    ( *pSafe_Get_Power_Mode_t           )( void );


typedef mu_safe_fsm_t 					( *pSafe_Get_State_t				)( void );
typedef blackch_pyld_pu_t 				( *pSafe_Get_Blckch_Pyld_t			)( void );
typedef curr_meas_status_t 				( *pSafe_Get_Currmeas_Status_t		)( void );
typedef otdet_status_t 					( *pSafe_Get_Otdet_Status_t			)( void );
typedef drumspin_state_t 				( *pSafe_Get_DrumSpin_State_t		)( void );
typedef bool 							( *pSafe_Get_Blckch_Timeout_t		)( void );
typedef bool 							( *pSafe_Get_Otdet_Overtemp_t		)( void );


typedef curr_meas_phases_t 				( *pSafe_Get_Curr_Meas_t			)( void );
typedef otdet_temps_t					( *pSafe_Get_Otdet_t 				)( void );
typedef float32_t   					( *pSafe_Get_VDC_t					)( void );

typedef void							( *pSafe_Enter_Test_Mode_t			)( void );

typedef void                            ( *pSafe_Reset_Status_Log_Chkpt_t   )(const reset_status_log_chkpt_t chkpt);
typedef reset_status_log_entry_t        ( *pSafe_Reset_Status_Log_Get_Entry_t )(uint8_t idx);
typedef uint8_t                         ( *pSafe_Reset_Status_Log_Get_Size_t )(void);

typedef bool                            ( *pSafe_Can_Clear_Break_t          )(void);
typedef bool                            ( *pSafe_Can_Clear_Break2_t         )(void);

typedef void (*pSafe_Enter_Powerfail_t)(void);
typedef void (*pSafe_Recover_Powerfail_t)(void);
//@}


/**
 * SAFE function table definition.
 */
typedef struct
{
    pSafe_Get_Build_Info_t              fp_Get_Build_Info              	;
    pSafe_Get_Version_Info_t            fp_Get_Version_Info            	;
    pSafe_Get_Crc_t                     fp_Get_Crc                     	;
    pSafe_Set_Test_Switches_t           fp_Set_Test_Switches           	;

#ifdef BLACKCH_DEVICE_SLAVE
    pSafe_BlackCh_Rcv_n_Resp_Frame_t    fp_BlackCh_Rcv_n_Resp_Frame    	;
#endif
#ifdef BLACKCH_DEVICE_MASTER
    pSafe_BlackCh_Receive_Frame_t       fp_BlackCh_Receive_Frame       	;
#endif
    pSafe_Blckch_Get_Status_t           fp_Blckch_Get_Status            ;
    pSafe_Can_Drive_Motor_t             fp_Can_Drive_Motor             	;
    pSafe_Get_ADC1_DMA1Ch1_CMAR_t       fp_Get_ADC1_DMA1Ch1_Buffer_Addr	;
    pSafe_Set_Power_Mode_t              fp_Set_Power_Mode              	;
    pSafe_Get_Power_Mode_t              fp_Get_Power_Mode              	;
    pSafe_Get_State_t					fp_Get_State					;
    pSafe_Get_Blckch_Pyld_t				fp_Get_Blckch_Pyld				;
    pSafe_Get_Currmeas_Status_t			fp_Get_Currmeas_Status 			;
    pSafe_Get_Otdet_Status_t			fp_Get_Otdet_Status				;
    pSafe_Get_DrumSpin_State_t			fp_Get_DrumSpin_State   		;
    pSafe_Get_Blckch_Timeout_t			fp_Get_Blckch_Timeout			;
    pSafe_Get_Otdet_Overtemp_t			fp_Get_Otdet_Overtemp			;
    pSafe_Get_Curr_Meas_t				fp_Get_Curr_Meas				;
    pSafe_Get_Otdet_t					fp_Get_Otdet					;
    pSafe_Get_VDC_t						fp_Get_VDC						;
    pSafe_Enter_Test_Mode_t				fp_Enter_Test_Mode				;
    pSafe_Reset_Status_Log_Chkpt_t      fp_Set_Reset_Status_Log_Chkpt   ;
    pSafe_Reset_Status_Log_Get_Entry_t  fp_Get_Reset_Status_Log_Entry   ;
    pSafe_Reset_Status_Log_Get_Size_t   fp_Get_Reset_Status_Log_Size    ;
    pSafe_Can_Clear_Break_t             fp_Can_Clear_Break              ;
    pSafe_Can_Clear_Break2_t            fp_Can_Clear_Break2             ;
    pSafe_Enter_Powerfail_t fp_Enter_Powerfail;
    pSafe_Recover_Powerfail_t fp_Recover_Powerfail;
} safe_func_table_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/**
 * SAFE function table start address in Flash.
 */
extern const uint32_t _MEMPART_FLASH_SAFE_FNC_TABLE_ADDR; /* parasoft-suppress NAMING-06 EMSISO-GLOBAL "Allow here, used in memory definitions." */

/**
 * USER function table start address in Flash.
 */
extern const uint32_t _MEMPART_FLASH_USER_FNC_TABLE_ADDR; /* parasoft-suppress NAMING-06 EMSISO-GLOBAL "Allow here, used in memory definitions." */

/**
 * Pointer to SAFE function table.
 * Used as global variable across USER project.
 */
extern volatile const safe_func_table_t* const pg_safe_func_table;

/**
 * Pointer to USER function table.
 * Used as global variable across SAFE project.
 */
extern volatile const user_func_table_t* const pg_user_func_table;

#endif // __FUNC_TABLE_H
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
