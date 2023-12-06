/* *********************************************************************** */
/*                                                                         */
/* Header      : NETBUF.H                                                  */
/* Date        : 2021/08/09                                                */
/* Changed:    : 2021/12/15                                                */
/* Description : Network Buffers for Communication Protocols               */
/*               Gary R. Wright and W. Richard Stevens. TCP/IP Illustrated,*/
/*               Volume 2: The Implementation.                             */
/*                                                                         */
/* *********************************************************************** */
#ifndef NETBUF_H
#define NETBUF_H

/* *********** includes ************************************************** */
#include "types.h"
#include "cfg/project_cfg.h"
#include "Platform/proc.h"


/* *********************************************************************** */
/* TRACE LEVELS                                                            */
/* 0 = NONE                                                                */
/* 1 = NORMAL                                                              */
/* 2 = DETAILED                                                            */
/* *********************************************************************** */
#ifndef NETBUF_TRACE_LEVEL
#ifdef _DEBUG
#define NETBUF_TRACE_LEVEL   2
#else
#define NETBUF_TRACE_LEVEL   0
#endif
#endif

/* ************************************************************************** */
#ifndef NETBUF_USE_SNAPSHOT
#ifdef _DEBUG
#define NETBUF_USE_SNAPSHOT  0
#else
#define NETBUF_USE_SNAPSHOT  0
#endif
#endif


/* *********** Configuration Defines ************************************* */
#ifndef  NETBUF_SIZE
#define  NETBUF_SIZE            170
#endif

#ifndef  NETBUF_POOL_BUFS
#define  NETBUF_POOL_BUFS         4
#endif

#ifndef  NETBUF_RESERVE_BUFS
#define  NETBUF_RESERVE_BUFS      2
#endif


/* *********** Defines *************************************************** */
#define  NETBUF_DATASIZE       (NETBUF_SIZE - sizeof(struct t_netbuf_hdr))
#define  NETBUF_PHDATASIZE     (NETBUF_DATASIZE - sizeof(struct t_netbuf_pkthdr))
#define  NETBUF_POOLSIZE       (NETBUF_SIZE * NETBUF_POOL_BUFS)

/* *********************************************************************** */
#define  NBF_PKTHDR    0x01       /* start of record                       */
#define  NBF_EOR       0x02       /* end of record                         */
#define  NBF_BCAST     0x10       /* send/received as link-level broadcast */
#define  NBF_MCAST     0x20       /* send/received as link-level multicast */

#define  NB_COPYFLAGS  (NBF_PKTHDR | NBF_EOR | NBF_BCAST | NBF_MCAST)

#define  NBT_FREE      0          /* should be on free list                */
#define  NBT_DATA      1          /* dynamic (data) allocation             */
#define  NBT_HEADER    2          /* Packet Header                         */
#define  N_NBT         3          /* Number of Net Buffer Types            */

#define  NB_MAXPROTOHDR 40        /* Maximal Protocol Header               */


/* *********************************************************************** */
/*struct t_netdev;*/

/* *********************************************************************** */
typedef INT16                    NETBUF_LEN;

typedef struct t_netbuf          NETBUF;
typedef struct t_netbuf_hdr      NETBUF_HDR;
typedef struct t_netbuf_pkthdr   NETBUF_PKTHDR;
typedef struct t_netbuf_stat     NETBUF_STAT;
typedef struct t_netbuf_queue    NETBUF_QUEUE;

/* *********** structs *************************************************** */
struct t_netbuf_hdr
{
    NETBUF*     nh_next;          /* next buffer in chain                  */
    NETBUF*     nh_nextpkt;       /* next chain in queue/record            */
    BYTE*       nh_pdata;         /* data location                         */
    NETBUF_LEN  nh_len;           /* data length in buffer                 */
    BYTE        nh_type;          /* type of buffer                        */
    BYTE        nh_flags;         /* flags , see above                     */
};

struct t_netbuf_pkthdr
{
    struct t_netdev*  ph_rcvnetdev;     /* receive network device          */
    NETBUF_LEN        ph_len;           /* total packet length             */
};

struct t_netbuf
{
    NETBUF_HDR  nb_hdr;
    union u_nb_data
    {
        struct t_nb_ph
        {
            NETBUF_PKTHDR ph_nb_pkthdr;
            BYTE          ph_nb_databuf[NETBUF_PHDATASIZE];
        } ph;
        BYTE   nb_databuf[NETBUF_DATASIZE];
    } nb_data;
};


struct t_netbuf_stat 
{
    WORD   ns_ntypes[N_NBT];       /* Number of Net Buffers Per Type       */
    WORD   ns_drops;               /* Times Failed to Get Frame            */
};

struct t_netbuf_queue
{
    NETBUF*    head;
    NETBUF*    tail;
    NETBUF_LEN len;
    NETBUF_LEN maxlen;
    WORD       drops;
}; 


#define nb_next              nb_hdr.nh_next
#define nb_nextpkt           nb_hdr.nh_nextpkt
#define nb_pdata             nb_hdr.nh_pdata
#define nb_len               nb_hdr.nh_len
#define nb_type              nb_hdr.nh_type
#define nb_flags             nb_hdr.nh_flags
#define nb_pkthdr            nb_data.ph.ph_nb_pkthdr
#define nb_pktlen            nb_data.ph.ph_nb_pkthdr.ph_len
#define nb_pktrcvchannel     nb_data.ph.ph_nb_pkthdr.ph_rcvchannel
#define nb_databuf           nb_data.nb_databuf
#define nb_pktdatabuf        nb_data.ph.ph_nb_databuf


/* *********** Macros **************************************************** */

/* NBTOD(nb,t)  - convert NETBUF pointer to data pointer of correct type */

#define  NBTOD(nb,t)   ((t)((nb)->nb_pdata))

/* Copy Net Buffer pkthdr from from to to
 * From must have NBF_PKTHDR set and to must be empty
 */
#define  NB_COPY_PKTHDR(to, from) \
{  (to)->nb_pkthdr  = (from)->nb_pkthdr; \
   (to)->nb_flags   = (from)->nb_flags & NB_COPYFLAGS; \
   (to)->nb_pdata   = (to)->nb_pktdatabuf; \
}

/* Set the nb_pdata pointer of a newly-allocated net buffer (NETBUF_get) to place 
 * an object of the specified size at the end of net buffer, longword aligned
 */
#define  NB_ALIGN(m, len)\
{ (m)->nb_pdata += (NETBUF_DATASIZE - (len)) &~ (sizeof(LONG) -1); }

/* Same as above, for net buffers allocated with NETBUF_gethdr or
 * initialised by NETBUF_COPY_PKTHDR
 */
#define  NB_PHALIGN(m, len)\
{ (m)->nb_pdata += (NETBUF_PHDATASIZE - (len)) &~ (sizeof(LONG) -1); }

/* Compute the amount of space available before the current start of data
 * in a net buffer
 */
#define  NB_LEADINGSPACE(m)\
( (m)->nb_flags & NBF_PKTHDR ?  (m)->nb_pdata - (m)->nb_pktdatabuf :\
                                (m)->nb_pdata - (m)->nb_databuf )

/* Compute the amount of space available after the end of data in a 
 * net buffer
 */
#define  NB_TRAILINGSPACE(m)\
( (&((m)->nb_databuf[0]) + NETBUF_DATASIZE) - ((m)->nb_pdata + (m)->nb_len) )




/* *********************************************************************** */
#define NQ_QFULL(nq)  \
    ((nq)->len >= (nq)->maxlen)

#define NQ_DROP(nq) \
    ((nq)->drops++)

#define NQ_ENQUEUE(nq, m) \
{ \
    (m)->nb_nextpkt = 0; \
    if ((nq)->tail == 0) \
        (nq)->head = m; \
    else \
        (nq)->tail->nb_nextpkt = m; \
    (nq)->tail = m; \
    (nq)->len++; \
}

#define NQ_PREPEND(nq, m) \
{ \
    (m)->nb_nextpkt = (nq)->head; \
    if ((nq)->tail == 0) \
        (nq)->tail = (m); \
    (nq)->head = (m); \
    (nq)->len++; \
}

#define NQ_DEQUEUE(nq, m) \
{ \
    (m) = (nq)->head; \
    if (m) { \
        if (((nq)->head = (m)->nb_nextpkt) == 0) \
            (nq)->tail = 0; \
        (m)->nb_nextpkt = 0; \
        (nq)->len--; \
    } \
}


/* *********** globals *************************************************** */

/* *********** function prototypes *************************************** */
#if NETBUF_USE_SNAPSHOT
VOID        netbuf_snapshot_make(VOID);
VOID        netbuf_snapshot_diff(VOID);
#endif

                                 BOOL        netbuf_init(VOID);
PROC_RAM_FUNCTION_ALWAYS_INFRONT NETBUF*     netbuf_get(VOID);
PROC_RAM_FUNCTION_ALWAYS_INFRONT NETBUF*     netbuf_gethdr(VOID);
PROC_RAM_FUNCTION_ALWAYS_INFRONT NETBUF*     netbuf_free(NETBUF* pnb);
PROC_RAM_FUNCTION_ALWAYS_INFRONT VOID        netbuf_freem(NETBUF* pnb);
                                 VOID        netbuf_freeq(NETBUF_QUEUE* queue);
                                 NETBUF_LEN  netbuf_copyfrom(NETBUF* pnb, NETBUF_LEN off, NETBUF_LEN len, VOID* data);
                                 NETBUF_LEN  netbuf_copyto(NETBUF* pnb, NETBUF_LEN off, NETBUF_LEN len, VOID* data);
                                 NETBUF*     netbuf_copy_nb(NETBUF* pnb);
PROC_RAM_FUNCTION_ALWAYS_INFRONT NETBUF_LEN  netbuf_append(NETBUF* pnb, NETBUF_LEN len, VOID* data);
                                 NETBUF*     netbuf_prepend(NETBUF* pnb, NETBUF_LEN len); 
                                 NETBUF_LEN  netbuf_remove(NETBUF* pnb, NETBUF_LEN len);
                                 NETBUF*     netbuf_pullup(NETBUF* pnb, NETBUF_LEN len);
                                 BOOL        netbuf_empty(NETBUF* pnb);
                                 BOOL        netbuf_ptrvalid(NETBUF* pnb);
                                 BOOL        netbuf_isvalid(NETBUF* pnb);
                                 uint8_t     netbuf_pool_isvalid(VOID);
PROC_RAM_FUNCTION_ALWAYS_INFRONT BOOL        netbuf_recovery(VOID);
                                 NETBUF*     netbuf_lastpkt(NETBUF* pnb);
                                 WORD        netbuf_getfreecount(VOID);
                                 WORD        netbuf_getdrops(VOID);

#ifdef _DEBUG
VOID        netbuf_trace(NETBUF* pnb);
VOID        netbuf_trace_state(VOID);
#endif

/* *********************************************************************** */

#endif /* NETBUF_H */

/* *********** EOF ******************************************************* */
