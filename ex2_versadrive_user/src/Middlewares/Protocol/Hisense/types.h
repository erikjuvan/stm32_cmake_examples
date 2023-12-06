//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        types.h
// CREATION DATE:      2019/09/25
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2021/05/19
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Types common variables types
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2021 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#ifndef TYPES_HAL_H
#define TYPES_HAL_H

#include "cfg/platform_cfg.h"

#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
        #include "../STM8_STM8S207/STM8S/stm8s.h"
#else
        #include <stdint.h>
#endif

#ifndef NULL
#define NULL 0
#endif

#define min(a,b)                (a)<(b)?(a):(b)

#define max(a,b)                (a)>(b)?(a):(b)
#define max3(a,b,c)             (max(a, max(b,c)))
#define max4(a,b,c,d)           (max(a, max3(b,c,d)))
#define max5(a,b,c,d,e)         (max(a, max4(b,c,d,e)))
#define max6(a,b,c,d,e,f)       (max(a, max5(b,c,d,e,f)))
#define max7(a,b,c,d,e,f,g)     (max(a, max6(b,c,d,e,f,g)))
#define max8(a,b,c,d,e,f,g,h)   (max(a, max7(b,c,d,e,f,g,h)))


/* Swap the byte ordering of 32 bit value */
#define SWAP_ENDIAN32(x)  \
        ( (uint32_t)( (((x) & 0xFF000000u) >> 24) |           \
                      (((x) & 0x00FF0000u) >> 8)  |           \
                      (((x) & 0x0000FF00u) << 8)  |           \
                      (((x) & 0x000000FFu) << 24) ) )

/* Swap the byte ordering of 16 bit value */
#define SWAP_ENDIAN16(x)  \
        ( (uint16_t) ( (((x) & 0x00FFu) << 8) |               \
                       (((x) & 0xFF00u) >> 8)  ) )

typedef void (* ISRaddress)(void);
typedef void (* callbackFunc)(void*);
typedef void (* callbackFunc_2args)(void*, void*);
typedef void (* callbackFunc_3args)(void*, void*, void*);
typedef void (* callbackFunc_4args)(void*, void*, void*, void*);
typedef void (* callbackFunc_5args)(void*, void*, void*, void*, void*);

/* Signed or unsigned depending on compiler selection */
typedef char char8;


/* *********** types ******************************************************** */
typedef char                CHAR;
typedef int                 INT;

typedef unsigned char       BYTE;
typedef signed short        SHORT;
typedef unsigned short      WORD;
typedef signed long         LONG;
typedef unsigned long       ULONG;
typedef unsigned long       DWORD;
typedef unsigned long long  DULONG;
typedef double              DOUBLE;
typedef SHORT               BOOL;

typedef signed char         INT8;
typedef unsigned char       UINT8;
typedef signed short        INT16;
typedef unsigned short      UINT16;
typedef signed long         INT32;
typedef unsigned long       UINT32;
typedef signed long long    INT64;
typedef unsigned long long  UINT64;

typedef UINT32         SIZE_T;

#ifndef VOID
#define VOID           void
#endif

#ifndef NULL
#define NULL           ((void *) 0)
#endif

/* *********** Defines ****************************************************** */
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif



#endif /* TYPES_HAL_H */

/* [] END OF FILE */
