#ifndef PROTOCOL_CANOPEN_CANOPEN_CO_INIT_H_
#define PROTOCOL_CANOPEN_CANOPEN_CO_INIT_H_

#include <stdlib.h>
#include <stdio.h>
#include "gen_define.h"
#include "co_canopen.h"
#include "co_direct_read.h"

#ifndef CO_CONST_STORAGE_CLASS
#define CO_CONST_STORAGE_CLASS
#endif
/* number of objects */
#define CO_OD_ASSIGN_CNT 118u
#define CO_OBJ_DESC_CNT 558u

#if defined(DEBUG)
	RET_T coCanOpenStackInit(CO_EVENT_STORE_T pLoadFunction);
#else
	void coSimpleInit(void);
#endif


#endif /* PROTOCOL_CANOPEN_CANOPEN_CO_INIT_H_ */
