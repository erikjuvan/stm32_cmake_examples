/* ************************************************************************** */
/*                                                                            */
/* Module      : NETBUF.C                                                     */
/* Date        : 2021/09/10                                                   */
/* Changed:    : 2022/03/01                                                   */
/* Description : Network Buffers for Communication Protocols                  */
/*               Gary R. Wright and W. Richard Stevens. TCP/IP Illustrated,   */
/*               Volume 2: The Implementation.                                */
/*                                                                            */
/* ************************************************************************** */

/* *********** includes ***************************************************** */
#include "cfg/project_cfg.h"
#include "Netbuf/netbuf.h"
#include <string.h>

/* ************************************************************************** */
#ifdef _DEBUG
#define NETBUF_STATIC
#else
#define NETBUF_STATIC static
#endif

/* ************************************************************************** */


/* *********** globals ****************************************************** */


/* Network Buffers Pool  */
NETBUF_STATIC NETBUF       netbuf_pool[NETBUF_POOL_BUFS];

/* Free List             */
NETBUF_STATIC NETBUF*      netbuf_freelist;

/* Buffer Statistics     */
NETBUF_STATIC NETBUF_STAT  netbuf_stat;


/* ************************************************************************** */
#if NETBUF_USE_SNAPSHOT

typedef struct t_netbuf_snapshot
{
    NETBUF       netbuf_pool[NETBUF_POOL_BUFS];
    BYTE         netbuf_diff[NETBUF_POOL_BUFS];
    WORD         netbuf_diff_count;
    BOOL         netbuf_snapshot_taken;
} NETBUF_SNAPSHOT;

NETBUF_SNAPSHOT netbuf_snapshot;

#endif

/* *********** macros ******************************************************* */
#define NETBUF_PTRROUNDDOWN(pnb)\
   ( ( ( ((pnb) - netbuf_pool) / NETBUF_SIZE ) * NETBUF_SIZE ) + netbuf_pool)

#define NETBUF_PTRINRANGE(pnb)\
   ( ( (pnb) >= netbuf_pool ) && ( (pnb) <= &netbuf_pool[NETBUF_POOL_BUFS-1]) )

#define NETBUF_PTRVALID(pnb)\
   ( ( (pnb) != NULL ) && NETBUF_PTRINRANGE(pnb) )



/* *********** functions **************************************************** */

/* ************************************************************************** */
#if NETBUF_USE_SNAPSHOT
/* ************************************************************************** */
/* Function name    : netbuf_snapshot_make                                    */
/*                                                                            */
/* Return type      : TRUE if OK else FALSE                                   */
/* Argument         :                                                         */
/*                                                                            */
/* Description      : Snapshot of Net Buffers                                 */
/*                                                                            */
/* ************************************************************************** */
VOID netbuf_snapshot_make(VOID)
{
    memcpy(&netbuf_snapshot.netbuf_pool[0],&netbuf_pool[0],sizeof(netbuf_pool));
    netbuf_snapshot.netbuf_snapshot_taken = TRUE;
}

VOID netbuf_snapshot_diff(VOID)
{
    INT i;
    
    netbuf_snapshot.netbuf_diff_count = 0;

    for (i=0; i<NETBUF_POOL_BUFS; i++)
    {
        if ( memcmp(&netbuf_snapshot.netbuf_pool[i],&netbuf_pool[i],sizeof(NETBUF)) == 0 )
        {
            netbuf_snapshot.netbuf_diff[i] = 0;
        }
        else
        {
            netbuf_snapshot.netbuf_diff[i] = 1;
            netbuf_snapshot.netbuf_diff_count++;
        }
    }
}
#endif


/* ************************************************************************** */
/* Function name    : netbuf_init                                             */
/*                                                                            */
/* Return type      : TRUE if OK else FALSE                                   */
/* Argument         :                                                         */
/*                                                                            */
/* Description      : Init Net Buffers                                        */
/*                                                                            */
/* ************************************************************************** */
BOOL netbuf_init(VOID)
{
    /* Alternative Clear net buffer pool */
    SHORT i;
    for (i=0; i<NETBUF_POOL_BUFS; i++)
    {
        /* All Net Buffers Are In The Free List */
        if (i < (NETBUF_POOL_BUFS-1))
        {
           netbuf_pool[i].nb_next     = &netbuf_pool[i+1];
        }
        else
        {
           netbuf_pool[i].nb_next     = NULL;
        }

        netbuf_pool[i].nb_nextpkt  = NULL;
        netbuf_pool[i].nb_pdata    = &netbuf_pool[i].nb_databuf[0];
        netbuf_pool[i].nb_len      = 0;
        netbuf_pool[i].nb_type     = NBT_FREE;
        netbuf_pool[i].nb_flags    = 0;
    }

    /*netbuf_freelist = &netbuf_pool[0];*/
    netbuf_freelist = netbuf_pool;

    netbuf_stat.ns_ntypes[NBT_FREE] = NETBUF_POOL_BUFS;
    for (i=1; i<N_NBT; i++)
    {
       netbuf_stat.ns_ntypes[i] = 0;
    }
    netbuf_stat.ns_drops = 0;

    #if NETBUF_USE_SNAPSHOT
    #endif

    return TRUE;
}


#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_START
#else
VOID netbuf_memcpy_ram(void* dest, void* src, uint16_t size) PROC_RAM_FUNCTION;
#endif
PROC_RAM_FUNCTION_ALWAYS_INFRONT VOID netbuf_memcpy_ram(void* dest, void* src, uint16_t size)
{
    uint8_t *d;
    uint8_t *s;
    uint16_t i;

    d = (uint8_t*) dest;
    s = (uint8_t*) src;
    for (i = 0; i < size; i++)
    {
        *(d+i) = *(s+i);
    }
}
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_END
#endif


/* ************************************************************************** */
/* Function name    : netbuf_get                                              */
/*                                                                            */
/* Return type      : net buffer pointer (NULL if not available)              */
/* Argument         : fbtype   - net buffer type                              */
/*                                                                            */
/* Description      : Get data net buffer from pool                           */
/*                                                                            */
/* ************************************************************************** */
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_START
#else
NETBUF* netbuf_get(VOID) PROC_RAM_FUNCTION;
#endif
PROC_RAM_FUNCTION_ALWAYS_INFRONT NETBUF* netbuf_get(VOID)
{
    NETBUF* pnb;
    uint8_t enableInterrupts;

    /* Take Net Buffer From Free List */
    enableInterrupts = proc_EnterCriticalSection();
    if ( (pnb = netbuf_freelist) != NULL)
    {
        netbuf_freelist = pnb->nb_next;

        pnb->nb_next    = NULL;
        pnb->nb_nextpkt = NULL;
        pnb->nb_pdata   = &pnb->nb_databuf[0];
        pnb->nb_len     = 0;
        pnb->nb_type    = NBT_DATA;
        pnb->nb_flags   = 0;

        netbuf_stat.ns_ntypes[NBT_FREE]--;
        netbuf_stat.ns_ntypes[NBT_DATA]++;

        proc_ExitCriticalSection(enableInterrupts);

        /* TRACE */
        #if (NETBUF_TRACE_LEVEL > 1)
        TRACE2("netbuf_get [%08X] - Ok Free=%d\n", 
               pnb, netbuf_stat.ns_ntypes[0]);
        #endif

        return pnb;
    }
    /* Free List is Empty Or Corrupted */
    else
    {
        netbuf_stat.ns_drops++;
        proc_ExitCriticalSection(enableInterrupts);

        /* TRACE */
        #if (NETBUF_TRACE_LEVEL > 1)
        TRACE2("netbuf_get [%08X] - Fail Free=%d\n", 
                pnb, netbuf_stat.ns_ntypes[0]);
        IFTRACE(netbuf_trace_state());
        #endif

        return NULL;
    }
}
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_END
#endif


/* ************************************************************************** */
/* Function name    : netbuf_gethdr                                           */
/*                                                                            */
/* Return type      : net buffer pointer (NULL if not available)              */
/* Argument         :                                                         */
/*                                                                            */
/* Description      : Get net buffer and init it to contain a packet header   */
/*                                                                            */
/* ************************************************************************** */
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_START
#else
NETBUF* netbuf_gethdr(VOID) PROC_RAM_FUNCTION;
#endif
PROC_RAM_FUNCTION_ALWAYS_INFRONT NETBUF* netbuf_gethdr(VOID)
{
    NETBUF* pnb;
    uint8_t enableInterrupts;

    enableInterrupts = proc_EnterCriticalSection();
    /* Take Net Buffer From Free List */
    if ( (pnb = netbuf_freelist) != NULL)
    {
        netbuf_freelist = pnb->nb_next;

        pnb->nb_next    = NULL;
        pnb->nb_nextpkt = NULL;
        pnb->nb_pdata   = &pnb->nb_pktdatabuf[0];
        pnb->nb_len     = 0;
        pnb->nb_pktlen  = 0;
        pnb->nb_type    = NBT_HEADER;
        pnb->nb_flags   = NBF_PKTHDR;

        netbuf_stat.ns_ntypes[NBT_FREE]--;
        netbuf_stat.ns_ntypes[NBT_HEADER]++;

        proc_ExitCriticalSection(enableInterrupts);

        /* TRACE */
        #if (NETBUF_TRACE_LEVEL > 1)
        TRACE2("netbuf_gethdr [%08X] - Ok Free=%d\n", 
               pnb, netbuf_stat.ns_ntypes[0]);
        #endif

        return pnb;
    }
    /* Free List is Empty Or Corrupted */
    else
    {
        netbuf_stat.ns_drops++;
        proc_ExitCriticalSection(enableInterrupts);

        #if (NETBUF_TRACE_LEVEL > 1)
        TRACE2("netbuf_gethdr [%08X] - Fail Free=%d\n", 
                pnb, netbuf_stat.ns_ntypes[0]);
        IFTRACE(netbuf_trace_state());
        #endif

        return NULL;
    }
}
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_END
#endif


/* ************************************************************************** */
/* Function name    : netbuf_free                                             */
/*                                                                            */
/* Return type      : net buffer pointer of pnb successor if any              */
/* Argument         : pnb    -  net buffer pointer to free                    */
/*                                                                            */
/* Description      : Free Net Buffer (pnb) and put it on the Free List       */
/*                                                                            */
/* ************************************************************************** */
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_START
#else
NETBUF* netbuf_free(NETBUF* pnb) PROC_RAM_FUNCTION;
#endif
PROC_RAM_FUNCTION_ALWAYS_INFRONT NETBUF* netbuf_free(NETBUF* pnb)
{
    NETBUF* npfb;
    uint8_t enableInterrupts;

    if ( NETBUF_PTRVALID(pnb) )
    {
        enableInterrupts = proc_EnterCriticalSection();
        if (pnb->nb_type == NBT_FREE)
        {
            proc_ExitCriticalSection(enableInterrupts);

            #if (NETBUF_TRACE_LEVEL > 1)
            TRACE2("netbuf_free [%08X] - Fail Already Free, Free=%d\n",
                    pnb, netbuf_stat.ns_ntypes[0]);
            #endif

            return NULL;
        }

        netbuf_stat.ns_ntypes[pnb->nb_type]--;

        npfb             = pnb->nb_next;
        pnb->nb_type     = NBT_FREE;
        netbuf_stat.ns_ntypes[NBT_FREE]++;

        /* Put it to Start of Free List */
        pnb->nb_next     = netbuf_freelist;
        netbuf_freelist  = pnb;

        pnb->nb_nextpkt  = NULL;
        pnb->nb_pdata    = NULL;
        pnb->nb_len      = 0;
        pnb->nb_flags    = 0;

        proc_ExitCriticalSection(enableInterrupts);

        #if (NETBUF_TRACE_LEVEL > 1)
        TRACE2("netbuf_free [%08X] - Ok Free=%d\n", 
               pnb, netbuf_stat.ns_ntypes[0]);
        #endif

        return npfb;
    }
    else
    {
        /* Recovery from corrupted Net Buffers */
        if ( pnb != NULL )
        {
            netbuf_recovery();
        }

        #if (NETBUF_TRACE_LEVEL > 1)
        TRACE2("netbuf_free - Fail PTR INVALID[%0x] Free=%d\n", 
                pnb, netbuf_stat.ns_ntypes[0]);
        #endif

        return NULL;
    }
}
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_END
#endif


/* ************************************************************************** */
/* Function name    : netbuf_freem                                            */
/*                                                                            */
/* Return type      :                                                         */
/* Argument         : pnb    -  net buffer pointer of chain to free           */
/*                                                                            */
/* Description      : Free All Net Buffers in Chain and put them on Free      */
/*                    List                                                    */
/*                                                                            */
/* ************************************************************************** */
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_START
#else
VOID netbuf_freem(NETBUF* pnb) PROC_RAM_FUNCTION;
#endif
PROC_RAM_FUNCTION_ALWAYS_INFRONT VOID netbuf_freem(NETBUF* pnb)
{
    NETBUF* npfb;

    if (pnb == NULL)
    {
        return;
    }

    do 
    {
       npfb = netbuf_free(pnb);
       pnb  = npfb;
    } while (pnb);
}
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_END
#endif


/* ************************************************************************** */
/* Function name    : netbuf_freeq                                            */
/*                                                                            */
/* Return type      :                                                         */
/* Argument         : queue    -  queue pointer of queue to free              */
/*                                                                            */
/* Description      : Free All Net Buffers in Chain in Queue                  */
/*                    and put them on Free List                               */
/*                                                                            */
/* ************************************************************************** */
VOID netbuf_freeq(NETBUF_QUEUE* queue)
{
    NETBUF* pnb;

    if (queue == NULL)
    {
        return;
    }

    while (TRUE)
    {
        NQ_DEQUEUE(queue, pnb);
        if (pnb == NULL)
        {
            break;
        }
        netbuf_freem(pnb);
    }
    queue->len = 0;
    queue->head = (NETBUF*) 0;
    queue->tail = (NETBUF*) 0;
}


/* ************************************************************************** */
/* Function name    : netbuf_copyfrom                                         */
/*                                                                            */
/* Return type      : number of bytes copied                                  */
/* Argument         : pnb    -  net buffer pointer of the net chain           */
/*                    off    -  net chain offset to copy from                 */
/*                    len    -  data length to copy from the net chain        */
/*                    data   -  where to store data from net chain            */
/*                                                                            */
/* Description      : Copy len bytes of data from net buffer chain. The       */
/*                    data is copied from specified byte offset in the        */
/*                    net buffer chain.                                       */
/*                                                                            */
/* ************************************************************************** */
NETBUF_LEN netbuf_copyfrom(NETBUF* pnb, NETBUF_LEN off, NETBUF_LEN len, VOID* data)
{
    NETBUF_LEN  count;
    NETBUF_LEN  cpylen;
    BYTE*       d;
    NETBUF* m;

    if ( !(NETBUF_PTRVALID(pnb)) )
    {
        return 0;
    }

    d      = (BYTE*)data;
    cpylen = 0;
    m      = pnb;

    if (off < 0 || len == 0)
    {
        return 0;
    }

    while (off > 0)
    {
        if (m == NULL)
        {
            return 0;
        }
        if (off < m->nb_len)
        {
            break;
        }
        off -= m->nb_len;
        m    = m->nb_next;
    }

    while (len > 0)
    {
        if (m == NULL)
        {
            break;
        }
        count = min(m->nb_len - off, len);
        memcpy( d, NBTOD(m,BYTE*)+off, count);
        len    -= count;
        d      += count;
        cpylen += count;
        off     = 0;
        m       = m->nb_next;
    }

    if (cpylen == 0)
    {
        return 0;
    }

    return cpylen;
}


/* ************************************************************************** */
/* Function name    : netbuf_copyto                                           */
/*                                                                            */
/* Return type      : number of bytes copied                                  */
/* Argument         : pnb    -  net buffer pointer of the net chain           */
/*                    off    -  net chain offset to copy to                   */
/*                    len    -  data length to copy to the net chain          */
/*                    data   -  data to copy to net chain                     */
/*                                                                            */
/* Description      : Copy len bytes of data to the net buffer chain. The     */
/*                    data is stored at the specified byte offset in the      */
/*                    net buffer chain. The chain is extended if necessary    */
/*                                                                            */
/* ************************************************************************** */
NETBUF_LEN netbuf_copyto(NETBUF* pnb, NETBUF_LEN off, NETBUF_LEN len, VOID* data)
{
    NETBUF_LEN  fblen;
    NETBUF_LEN  totlen;
    NETBUF_LEN  cpylen;
    NETBUF* m;
    NETBUF* n;
    BYTE*       d;
    WORD        space;

    d       = (BYTE*)data;
    m       = pnb;
    totlen  = 0;
    cpylen  = 0;

    /* Net Head must exist */
    if ( !(NETBUF_PTRVALID(pnb)) )
    {
        return 0;
    }

    /* TODO: Remove
    if (pnb == NULL)
    {
        return 0;
    }
    */

    while (off > (fblen = m->nb_len))
    {
        off    -= fblen;
        totlen += fblen;
        if (m->nb_next == NULL)
        {
           space = NB_TRAILINGSPACE(m);
           if ( (fblen = min(space, len)) != 0 )
           {
               memcpy( NBTOD(m,BYTE*)+m->nb_len, d, fblen);
               cpylen    += fblen;
               d         += fblen;
               len       -= fblen;
               totlen    += fblen;
               m->nb_len += fblen;
           }
           if (len == 0)
           {
               break;
           }

           #if (NETBUF_RESERVE_BUFS > 0)
           if (netbuf_stat.ns_ntypes[NBT_FREE] <= NETBUF_RESERVE_BUFS)
           {
               break;
           }
           #endif

           n = netbuf_get();
           if (n == NULL)
           {
               if ( ((m = pnb)->nb_flags & NBF_PKTHDR) && (m->nb_pktlen < totlen) )
               {
                  m->nb_pktlen = totlen;
               }
               return 0;
           }
           n->nb_len  = min(NETBUF_DATASIZE, len + off);
           m->nb_next = n;
        }
        m = m->nb_next;
    }

    while (len > 0)
    {
        if ( (fblen = min(m->nb_len - off, len)) != 0)
        {
            memcpy( NBTOD(m,BYTE*)+off, d, fblen);
            cpylen += fblen;
            d      += fblen;
            len    -= fblen;
        }
        fblen  += off;
        off     = 0;
        totlen += fblen;

        if (len == 0)
        {
            break;
        }
        if (m->nb_next == NULL)
        {
           space = NB_TRAILINGSPACE(m);
           if ( (fblen = min(space, len)) != 0 )
           {
               memcpy( NBTOD(m,BYTE*)+m->nb_len, d, fblen);
               cpylen    += fblen;
               d         += fblen;
               len       -= fblen;
               totlen    += fblen;
               m->nb_len += fblen;
           }
           if (len == 0)
           {
               break;
           }

           #if (NETBUF_RESERVE_BUFS > 0)
           if (netbuf_stat.ns_ntypes[NBT_FREE] <= NETBUF_RESERVE_BUFS)
           {
                break;
           }
           #endif

           n = netbuf_get();
           if (n == NULL)
           {
               break;
           }
           n->nb_len  = min((NETBUF_LEN)NETBUF_DATASIZE, len);
           m->nb_next = n;
        }
        m = m->nb_next;
    }

    if (((m = pnb)->nb_flags & NBF_PKTHDR) && (m->nb_pktlen < totlen))
    {
       m->nb_pktlen = totlen;
    }

    return cpylen;
}


/* ************************************************************************** */
/* Function name    : netbuf_copy_nb                                          */
/*                                                                            */
/* Return type      : copy of "pnb" netbuffer                                 */
/* Argument         : pnb    -  net buffer pointer of the net chain           */
/*                                                                            */
/* Description      : Copy "pnb" netbuffer data.                              */
/*                    New netbuffer is allocated for the copy netbuffer.      */
/*                    Source netbuffer is not released !!!                    */
/*                                                                            */
/* ************************************************************************** */
NETBUF* netbuf_copy_nb(NETBUF* pnb)
{
    NETBUF* src;
    NETBUF* dst;
    NETBUF_LEN nblen;

    /* Net Head must exist */
    if ( !(NETBUF_PTRVALID(pnb)) )
    {
        return NULL;
    }

    dst = netbuf_gethdr();
    if (dst == NULL)
    {
        return NULL;
    }

    src = pnb;
    nblen = src->nb_len;
    while ((nblen > 0) && (src != NULL))
    {
        if (netbuf_append(dst, nblen, NBTOD(src, BYTE*)) != nblen)
        {
            netbuf_freem(dst);
            return NULL;
        }
        src = src->nb_next;
        if (src != NULL)
        {
            nblen = src->nb_len;
        }
    }

    return dst;
}


/* ************************************************************************** */
/* Function name    : netbuf_append                                           */
/*                                                                            */
/* Return type      : number of bytes appended                                */
/* Argument         : pnb    -  net buffer pointer of the net chain           */
/*                    len    -  data length to copy to the net chain          */
/*                    data   -  data to copy to net chain                     */
/*                                                                            */
/* Description      : Copy len bytes of data to the net buffer chain. The     */
/*                    data is stored at the end of the net buffer chain.      */
/*                    The chain is extended if necessary                      */
/*                                                                            */
/* ************************************************************************** */
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_START
#else
NETBUF_LEN netbuf_append(NETBUF* pnb, NETBUF_LEN len, VOID* data) PROC_RAM_FUNCTION;
#endif
PROC_RAM_FUNCTION_ALWAYS_INFRONT NETBUF_LEN netbuf_append(NETBUF* pnb, NETBUF_LEN len, VOID* data)
{
    NETBUF_LEN  llen;
    NETBUF_LEN  fblen;
    NETBUF_LEN  count;
    NETBUF* m;
    NETBUF* n;
    BYTE*   d;

    m = pnb;

    if ( !(NETBUF_PTRVALID(pnb)) )
    {
        return 0;
    }

    while ( m->nb_next )
    {
        m = m->nb_next;
    }

    d     = (BYTE*)data;
    llen  = len;

    while (llen > 0)
    {
        count = NB_TRAILINGSPACE(m);
        fblen = min( count, llen);
        if (fblen)
        {
           /* Instead of:    memcpy( NBTOD(m,BYTE*)+m->nb_len, d, fblen );   */
           netbuf_memcpy_ram( NBTOD(m,BYTE*)+m->nb_len, d, fblen );
           d         += fblen;
           llen      -= fblen;
           m->nb_len += fblen;
        }

        if (llen == 0)
        {
            break;
        }
        
        #if (NETBUF_RESERVE_BUFS > 0)
        if (netbuf_stat.ns_ntypes[NBT_FREE] <= NETBUF_RESERVE_BUFS)
        {
            break;
        }
        #endif

        n = netbuf_get();
        if ( n == NULL)
        {
            break;
        }
        m->nb_next = n;
        m          = m->nb_next;
    }

    count = (len - llen);

    if (((m = pnb)->nb_flags & NBF_PKTHDR))
    {
       m->nb_pktlen += count;
    }

    return count;
}
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_END
#endif


/* ************************************************************************** */
/* Function name    : netbuf_prepend                                          */
/*                                                                            */
/* Return type      : number of bytes appended                                */
/* Argument         : pnb    -  net buffer pointer of the net chain           */
/*                    len    -  data length to copy to the net chain          */
/*                                                                            */
/* Description      : Prepand len bytes to the net buffer chain.              */
/*                                                                            */
/* ************************************************************************** */
NETBUF*  netbuf_prepend(NETBUF* pnb, NETBUF_LEN len)
{
    NETBUF* mn;

    if ( !(NETBUF_PTRVALID(pnb)) )
    {
        return NULL;
    }

    if (NB_LEADINGSPACE(pnb) >= (len))
    {
        pnb->nb_pdata -= len;
        pnb->nb_len   += len;
    } 
    else
    {
        if ( pnb->nb_flags & NBF_PKTHDR)
        {
           if ( len > NETBUF_PHDATASIZE)
           {
               netbuf_freem(pnb);
               return NULL;
           }
           mn = netbuf_gethdr();
        }
        else
        {
           if (len > (NETBUF_LEN)NETBUF_DATASIZE)
           {
               netbuf_freem(pnb);
               return NULL;
           }
           mn = netbuf_get();
        }

        if ( mn == NULL )
        {
            netbuf_freem(pnb);
            return NULL;
        }

        if ( pnb->nb_flags & NBF_PKTHDR)
        {
            NB_COPY_PKTHDR(mn,pnb);
            pnb->nb_flags &= ~NBF_PKTHDR;

            /* *** */
            pnb->nb_type = NBT_DATA;
            netbuf_stat.ns_ntypes[NBT_HEADER]--;
            netbuf_stat.ns_ntypes[NBT_DATA]++;
            /* *** */
        }

        mn->nb_next  = pnb;
        pnb          = mn;
        pnb->nb_len  = len;

    }

    if (pnb && pnb->nb_flags & NBF_PKTHDR)
    {
        pnb->nb_pktlen += len;
    }

    return pnb;
}


/* ************************************************************************** */
/* Function name    : netbuf_remove                                           */
/*                                                                            */
/* Return type      : number of bytes removed                                 */
/* Argument         : pnb    -  net buffer pointer of the buffer chain        */
/*                    len    -  data length to remove from the net buffer     */
/*                                                                            */
/* Description      : Remove len bytes from the net buffer pointed by         */
/*                    pnb. If len is positive that number of bytes is trimmed */
/*                    from the start of the data in the net buffer,           */
/*                    otherwise the absolute value of len bytes is trimmed    */
/*                    from the end of the data in the net buffer              */
/*                                                                            */
/* ************************************************************************** */
NETBUF_LEN netbuf_remove(NETBUF* pnb, NETBUF_LEN len)
{
    NETBUF_LEN l;
    NETBUF* m;
    NETBUF_LEN count;
    NETBUF_LEN remlen;
    
    if ( !(NETBUF_PTRVALID(pnb)) )
    {
        return 0;
    }

    l      = len;
    remlen = 0;

    if ( (m = pnb) == NULL)
    {
        return 0;
    }

    if (l >= 0)
    {
        /* Trim from Head */
        while (m != NULL && l > 0)
        {
            if (m->nb_len <= l)
            {
                l         -= m->nb_len;
                remlen    += m->nb_len;
                m->nb_len  = 0;
                m = m->nb_next;
            }
            else
            {
                m->nb_len   -= l;
                remlen      += l;
                m->nb_pdata += l;
                l            = 0;
            }
        }
        m = pnb;
        if (m->nb_flags & NBF_PKTHDR)
        {
            m->nb_pktlen -= (len - l);
        }
    }
    else
    {
        /* Trim from Tail. Scan the mbuf chain calculating its length and finding
         * the last Net Buffer. If the adjustment affects only this net buffer
         * then just adjust and return. Otherwise rescan rescan and truncate after 
         * the remaining size
         */
        l     = -l;
        count = 0;
        for (;;)
        {
            count += m->nb_len;
            if (m->nb_next == NULL)
            {
                break;
            }
            m = m->nb_next;
        }
        if (m->nb_len >= l)
        {
            m->nb_len  -= l;
            remlen     += l;
            if (pnb->nb_flags & NBF_PKTHDR)
            {
               pnb->nb_pktlen -= l;
            }
            return remlen;
        }
        count    -= l;
        remlen   += l;
        if (count < 0)
        {
            remlen += count;
            count   = 0;
        }
        /* Correct length for chain is "count". Find the net buffer with last data, 
         * adjust its length, and toss data from remainding mbufs on chain
         */
        m = pnb;
        if (m->nb_flags & NBF_PKTHDR)
        {
            m->nb_pktlen = count;
        }
        for (; m; m = m->nb_next)
        {
            if (m->nb_len >= count)
            {
                m->nb_len = count;
                break;
            }
            count -= m->nb_len;
        }
        while (m->nb_next)
        {
            (m = m->nb_next)->nb_len = 0;
        }
    }

    return remlen;
}


/* ************************************************************************** */
/* Function name    : netbuf_pullup                                           */
/*                                                                            */
/* Return type      : Resulting Net Chain on Success, NULL on failure         */
/* Argument         : pnb    -  net buffer pointer of the net chain           */
/*                    len    -  data length to pullup                         */
/*                                                                            */
/* Description      : Rearange a Net Buffer chain so that len bytes are       */
/*                    contigous and in the data are of a net buffer (so       */
/*                    that NBTOD will work for a structure of size len).      */
/*                    Returns the resulting net chain on success, frees it    */
/*                    and returns NULL on failure. If there is room, it will  */
/*                    add up to max_protohdr-len extra bytes to the contigous */
/*                    region in an attempt to avoid being called next time    */
/* ************************************************************************** */
NETBUF* netbuf_pullup(NETBUF* pnb, NETBUF_LEN len)
{
    NETBUF*     m;
    NETBUF_LEN  count;
    NETBUF_LEN  space;

    if ( !(NETBUF_PTRVALID(pnb)) )
    {
        return NULL;
    }

    /* If first net buffer has room for len bytes without shifting current
     * data, pullup into it, otherwise allocate new net buffer to prepand 
     * to the chain
     */
    if ( pnb->nb_pdata + len < (&pnb->nb_databuf[0]+NETBUF_DATASIZE) &&
         pnb->nb_next)
    {
        if (pnb->nb_len >= len)
        {
            return pnb;
        }
        m    = pnb;
        pnb  = pnb->nb_next;
        len -= m->nb_len;
    }
    else
    {
        if (len > (NETBUF_LEN)NETBUF_PHDATASIZE)
        {
            netbuf_freem(pnb);
            return NULL;
        }
        m = (pnb->nb_flags & NBF_PKTHDR) ? netbuf_gethdr() : netbuf_get();
        if ( m == NULL )
        {
            netbuf_freem(pnb);
            return NULL;
        }
        m->nb_len = 0;
        if (pnb->nb_flags & NBF_PKTHDR)
        {
            NB_COPY_PKTHDR(m, pnb);
            pnb->nb_flags &= ~NBF_PKTHDR;

            /* *** */
            pnb->nb_type = NBT_DATA;
            netbuf_stat.ns_ntypes[NBT_HEADER]--;
            netbuf_stat.ns_ntypes[NBT_DATA]++;
            /* *** */
        }
    }

    space = (&m->nb_databuf[0] + NETBUF_DATASIZE) - (m->nb_pdata + m->nb_len);
    do 
    {
        count        = min(min(max(len, NB_MAXPROTOHDR), space), pnb->nb_len);
        memcpy(NBTOD(m, BYTE*) + m->nb_len, NBTOD(pnb, BYTE*), count);
        len         -= count;
        m->nb_len   += count;
        pnb->nb_len -= count;
        space       -= count;
        if (pnb->nb_len)
        {
            pnb->nb_pdata += count;
        }
        else
        {
            pnb = netbuf_free(pnb);
        }
    } while (len > 0 && pnb);

    if (len > 0)
    {
        netbuf_free(m);
        netbuf_freem(pnb);
        return NULL;
    }

    m->nb_next = pnb;

    return (m);

}


/* ************************************************************************** */
/* Function name    : netbuf_empty                                            */
/*                                                                            */
/* Return type      : TRUE = Net Buffer is empty, else error                  */
/* Argument         : pnb  - Net Buffer Ptr                                   */
/*                                                                            */
/* Description      : Remove all data from net chain leaving just one         */
/*                    net buffer                                              */
/*                                                                            */
/* ************************************************************************** */
BOOL netbuf_empty(NETBUF* pnb)
{
    if ( !NETBUF_PTRVALID(pnb) )
    {
        return FALSE;
    }

    /* Free the net chain except first net buffer */
    netbuf_freem(pnb->nb_next);
    pnb->nb_next = NULL;

    pnb->nb_len = 0;

    if (pnb->nb_flags & NBF_PKTHDR)
    {
        pnb->nb_pktlen = 0;
    }
        
    return TRUE;
}


/* ************************************************************************** */
/* Function name    : netbuf_ptrvalid                                         */
/*                                                                            */
/* Return type      : TRUE = valid Net Buffer Ptr, else invalid               */
/* Argument         : pnb  - Net Buffer Ptr                                   */
/*                                                                            */
/* Description      : Test if Net Buffer Ptr is Valid                         */
/*                                                                            */
/* ************************************************************************** */
BOOL netbuf_ptrvalid(NETBUF* pnb)
{
    return NETBUF_PTRVALID(pnb);
}


/* ************************************************************************** */
BOOL netbuf_isvalid(NETBUF*  pnb)
{
    if ( !netbuf_ptrvalid(pnb) )
    {
        return FALSE;
    }

    if (pnb->nb_flags & NBF_PKTHDR)
    {
        if ( pnb->nb_len > NETBUF_PHDATASIZE ||
             pnb->nb_pdata < &pnb->nb_pktdatabuf[0] ||
             pnb->nb_pdata >= &pnb->nb_pktdatabuf[NETBUF_PHDATASIZE] )
        {
            return FALSE;
        }
    }
    else
    {
        if ( pnb->nb_len > NETBUF_DATASIZE ||
             pnb->nb_pdata < &pnb->nb_databuf[0] ||
             pnb->nb_pdata >= &pnb->nb_databuf[NETBUF_DATASIZE] )
        {
            return FALSE;
        }
    }

    return TRUE;
}


/* ************************************************************************** */
/* Function name    : netbuf_pool_isvalid                                     */
/*                                                                            */
/* Return type      : 1              - Netbuffer pool is valid                */
/*                    255,254,253... - Netbuffer is NOT valid                 */
/* Argument         : /                                                       */
/*                                                                            */
/* Description      : Checks if netbuffer pool is valid.                      */
/*                    If returned value is not equal to 1, netbuffer is not   */
/*                    valid. In this case netbuf_init() function has to be    */
/*                    called to fix the netbuffer pool.                       */
/*                    IMPORTANT: Also all modules that use the netbuffer      */
/*                    functionality have to be reinitialized as the netbuffer */
/*                    pointers does not belong to their (previous) owners     */
/*                    anymore !!!                                             */
/*                                                                            */
/* ************************************************************************** */
uint8_t netbuf_pool_isvalid(VOID)
{
    WORD     i;
    WORD     free_count;
    WORD     data_count;
    WORD     header_count;
    NETBUF*  pnb;
    uint8_t  enableInterrupts;

    /* Check Free List */
    free_count = 0;

    enableInterrupts = proc_EnterCriticalSection();
    pnb = netbuf_freelist;
    while (pnb)
    {
        if (netbuf_ptrvalid(pnb))
        {
            if ((pnb->nb_len == 0) &&
                (pnb->nb_type == NBT_FREE))
            {
                free_count++;
            }
            else
            {
                if ((pnb->nb_len != 0) &&
                    (pnb->nb_type == NBT_FREE))
                {
                    proc_ExitCriticalSection(enableInterrupts);
                    return 255;
                }
            }

            /* Exit and reenter Critical section between each netbuffer check
             * to allow other interrupts to handle.
             * This may result the netbuf pool is quasi invalid (return value different to 1)
             * but if the function is called again the result will be valid netbuf pool (return value 1).
             */
            proc_ExitCriticalSection(enableInterrupts);
            /* Allow other interrupts to handle here */
            enableInterrupts = proc_EnterCriticalSection();

            pnb = pnb->nb_next;
        } /* --> netbuf_ptrvalid(pnb) */
        else
        {
            proc_ExitCriticalSection(enableInterrupts);
            return 254;
        }
    } /* --> while (pnb) */

    if (free_count != netbuf_stat.ns_ntypes[NBT_FREE])
    {
        proc_ExitCriticalSection(enableInterrupts);
        return 253;
    }
    proc_ExitCriticalSection(enableInterrupts);

    free_count   = 0;
    data_count   = 0;
    header_count = 0;

    for (i=0; i<NETBUF_POOL_BUFS; i++)
    {
        if (netbuf_pool[i].nb_type == NBT_FREE)
        {
            free_count++;
        }
        else if (netbuf_pool[i].nb_type == NBT_DATA)
        {
            if (!netbuf_isvalid(&netbuf_pool[i]))
            {
                return 252;
            }
            data_count++;
        }
        else if (netbuf_pool[i].nb_type == NBT_HEADER)
        {
            if (!netbuf_isvalid(&netbuf_pool[i]))
            {
                return 251;
            }
            header_count++;
        }
        else
        {
           return 250;
        }
    }

    return 1;
}


/* ************************************************************************** */
/* Function name    : netbuf_recovery                                         */
/*                                                                            */
/* Return type      :                                                         */
/* Argument         :                                                         */
/*                                                                            */
/* Description      :                                                         */
/*                                                                            */
/* ************************************************************************** */
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_START
#else
BOOL netbuf_recovery(VOID) PROC_RAM_FUNCTION;
#endif
PROC_RAM_FUNCTION_ALWAYS_INFRONT BOOL netbuf_recovery(VOID)
{
    /* TODO: Implement Recovery */
    return TRUE;
}
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_END
#endif


/* ************************************************************************** */
/* Function name    : netbuf_lastpkt                                          */
/*                                                                            */
/* Return type      :                                                         */
/* Argument         :                                                         */
/*                                                                            */
/* Description      :                                                         */
/*                                                                            */
/* ************************************************************************** */
NETBUF* netbuf_lastpkt(NETBUF* pnb)
{
    NETBUF* m;

    m = pnb;

    if ( !(NETBUF_PTRVALID(m)) )
    {
        return NULL;
    }

    while ( m->nb_nextpkt )
    {
        m = m->nb_nextpkt;
    }

    return m;
}


/* ************************************************************************** */
/* Function name    : netbuf_getfreecount                                     */
/*                                                                            */
/* Return type      : WORD                                                    */
/* Argument         : VOID                                                    */
/*                                                                            */
/* Description      : Returns current number of free network buffers          */
/*                    in the netbuffer pool.                                  */
/*                                                                            */
/* ************************************************************************** */
WORD netbuf_getfreecount(VOID)
{
    return netbuf_stat.ns_ntypes[NBT_FREE];
}


/* ************************************************************************** */
/* Function name    : netbuf_getdrops                                         */
/*                                                                            */
/* Return type      : WORD                                                    */
/* Argument         : VOID                                                    */
/*                                                                            */
/* Description      :                                                         */
/*                                                                            */
/* ************************************************************************** */
WORD netbuf_getdrops(VOID)
{
    return netbuf_stat.ns_drops;
}


#ifdef _DEBUG
/* *********************************************************************** */
/* Function    :   netbuf_single_trace                                     */
/* Input       :                                                           */
/* Output      :                                                           */
/* Returns     :                                                           */
/* Description :                                                           */
/* *********************************************************************** */
VOID netbuf_single_trace(NETBUF* pnb)
{
    NETBUF_LEN i;
    CHAR       tracestr[80];
    CHAR*      dest;
    NETBUF_LEN count;
    NETBUF_LEN tocopy;
    NETBUF_LEN ncopied;
    NETBUF_LEN off;
    BYTE  buffer[16];

    if (pnb == NULL)
    {
        TRACE0("NETBUF NULL\n");
        return;
    }

    sprintf(tracestr, "FRAME LEN=%d", pnb->nb_len);

    TRACE1("%s\n", tracestr);

    off   = 0;
    count = pnb->nb_len;
    while (count)
    {
       tocopy  = min(count, 16);
       ncopied = netbuf_copyfrom(pnb, off, tocopy, buffer);
       dest    = tracestr;
       dest   += sprintf(dest, " %04X   ", off);

       for (i=0; i< ncopied; i++)
       {
           dest += sprintf(dest, "%02X ", buffer[i]);
       }

       for (i=ncopied; i<16; i++)
       {
           dest += sprintf(dest, "   ");
       }

       dest += sprintf(dest, "   ");

       for (i=0; i< ncopied; i++)
       {
           if (isprint(buffer[i]))
           {
              dest += sprintf(dest, "%c", buffer[i]);
           }
           else
           {
              dest += sprintf(dest, ".");
           }
       }

       TRACE1("%s\n", tracestr);

       if (ncopied)
       {
           count -= ncopied;
           off   += ncopied;
       }
       else
       {
           count = 0;
       }

    }
}


/* *********************************************************************** */
/* Function    :   netbuf_trace                                            */
/* Input       :                                                           */
/* Output      :                                                           */
/* Returns     :                                                           */
/* Description :                                                           */
/* *********************************************************************** */
VOID netbuf_trace(NETBUF* pnb)
{
    NETBUF* m;

    m = pnb;

    if ( m )
    {
         TRACE1("FRAME PKTLEN=%d\n", pnb->nb_pktlen);
    }

    while ( m )
    {
        netbuf_single_trace(m);
        m = m->nb_next;
    }
}


/* *********************************************************************** */
/* Function    :                                                           */
/* Input       :                                                           */
/* Output      :                                                           */
/* Returns     :                                                           */
/* Description :                                                           */
/* *********************************************************************** */
VOID netbuf_trace_state()
{
    int i;

    TRACE0("\n\nNETBUF Pool Trace\n\n");

    for (i=0; i<NETBUF_POOL_BUFS; i++)
    {
        TRACE1("NETBUF[%d]=\n", i);
        netbuf_trace(&netbuf_pool[i]);
    }

}

#endif



/* *********** EOF ********************************************************** */
