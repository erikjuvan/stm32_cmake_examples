#include "co_init.h"

extern CO_CONST CO_OD_ASSIGN_T od_assign[] CO_CONST_STORAGE_CLASS;
extern CO_CONST CO_OBJECT_DESC_T od_description[] CO_CONST_STORAGE_CLASS;
extern CO_CONST CO_OD_DATA_VARIABLES_T od_data_variables;

#if defined(DEBUG)
	extern CO_CONST UNSIGNED8 CO_CONST_STORAGE_CLASS od_const_u8[];
	extern CO_CONST UNSIGNED16 CO_CONST_STORAGE_CLASS od_const_u16[];
	extern CO_CONST UNSIGNED32 CO_CONST_STORAGE_CLASS od_const_u32[];

RET_T coCanOpenStackInit(
         CO_EVENT_STORE_T pLoadFunction
    )
{
    RET_T retVal;

    /* unused argument */
    (void)pLoadFunction;

    /* initialize object dictionary variables and pointers */
    coOdInitOdPtr(&od_assign[0], CO_OD_ASSIGN_CNT, &od_description[0], CO_OBJ_DESC_CNT, NULL, &od_data_variables);

    /* library timer initialization */
    coTimerInit(CO_TIMER_INTERVAL);

#if CAN_PROTOCOL_TYPE == 0	
    /* initialization of LED module */
    coLedInit();
#endif	

    /* initialize sdo server(s) */
    retVal = coSdoServerInit(1u);
    if (retVal != RET_OK) {
        return(retVal);
    }

#if CAN_PROTOCOL_TYPE == 0	
    /* initialize sdo clients(s) */
    retVal = coSdoClientInit(1u);
    if (retVal != RET_OK) {
        return(retVal);
    }
    /* initialize error control */
    retVal = coErrorCtrlInit(od_const_u16[0], 1u);
    if (retVal != RET_OK) {
        return(retVal);
    }
    /* initialize emergency */
    retVal = coEmcyProducerInit();
    if (retVal != RET_OK) {
        return(retVal);
    }
                


	//* initialize sync *//
	retVal = coSyncInit(od_const_u32[3]);
	if (retVal != RET_OK) {
	return(retVal);
	}
	//* initialize TPDO 1 *//
	retVal = coPdoTransmitInit(1u, od_const_u8[3], od_const_u16[0], od_const_u16[0], od_const_u8[0], NULL);
	if (retVal != RET_OK) {
	return(retVal);
	}
	//* initialize TPDO 2 *//
	retVal = coPdoTransmitInit(2u, od_const_u8[3], od_const_u16[0], od_const_u16[0], od_const_u8[0], NULL);
	if (retVal != RET_OK) {
	return(retVal);
	}
	//* initialize TPDO 3 *//
	retVal = coPdoTransmitInit(3u, od_const_u8[3], od_const_u16[0], od_const_u16[0], od_const_u8[0], NULL);
	if (retVal != RET_OK) {
	return(retVal);
	}
	//* initialize TPDO 4 *//
	retVal = coPdoTransmitInit(4u, od_const_u8[3], od_const_u16[0], od_const_u16[0], od_const_u8[0], NULL);
	if (retVal != RET_OK) {
	return(retVal);
	}
	//* initialize RPDO 1 *//
	retVal = coPdoReceiveInit(1u, od_const_u8[8], od_const_u16[0], od_const_u16[0], NULL);
	if (retVal != RET_OK) {
	return(retVal);
	}
	//* initialize RPDO 2 *//
	retVal = coPdoReceiveInit(2u, od_const_u8[8], od_const_u16[0], od_const_u16[0], NULL);
	if (retVal != RET_OK) {
	return(retVal);
	}
	//* initialize RPDO 3 *//
	retVal = coPdoReceiveInit(3u, od_const_u8[8], od_const_u16[0], od_const_u16[0], NULL);
	if (retVal != RET_OK) {
	return(retVal);
	}
	//* initialize RPDO 4 *//
	retVal = coPdoReceiveInit(4u, od_const_u8[8], od_const_u16[0], od_const_u16[0], NULL);
	if (retVal != RET_OK) {
	return(retVal);
	}
#endif
    /* initialize load parameter function */
#ifdef CO_EVENT_STORE
    coEventRegister_LOAD_PARA(pLoadFunction);
#endif /* CO_EVENT_STORE */

#if CAN_PROTOCOL_TYPE == 0	
        /* initialize LSS */
        retVal = coLssInit();
        if (retVal != RET_OK) {
            return(retVal);
        }
        /* initialize NMT master */ 
        retVal = coNmtInit(1u);
        if (retVal != RET_OK) {
            return(retVal);
        }
#else
		/* start canopen additional setup */
    retVal = coAdditionalSetup();
    if (retVal != RET_OK) {
        return(retVal);
    }
#endif

    return(RET_OK);
}
#else
	void coSimpleInit(void)
	{
		co_direct_coOdInitOdPtr(&od_assign[0], CO_OD_ASSIGN_CNT, &od_description[0], CO_OBJ_DESC_CNT, NULL, &od_data_variables);
	}
#endif

