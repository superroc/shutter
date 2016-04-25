#ifndef _CO_SKBUFF_H
#define _CO_SKBUFF_H

#include "stm32f10x_type.h"

struct sk_buff {
    struct sk_buff *next;
    struct sk_buff *prev;

    uint32 len;
    uint8 *head;
    uint8 *data;
    uint8 *tail;
    uint8 *end;
};


struct sk_buff_head {
    /* These two members must be first. */
    struct sk_buff	*next;
    struct sk_buff	*prev;
    uint32	    qlen;
};

struct sk_buff *skb_fill (uint8 *data, uint32 datalen);
struct sk_buff *skb_alloc(uint32 size);
void skb_free( struct sk_buff *skb);
uint8 *skb_put(struct sk_buff *skb, uint32 len);
void skb_putdata(struct sk_buff *skb, uint8 *data, uint32 len);
uint8 *skb_push(struct sk_buff *skb, uint32 len);
uint8 *skb_pull(struct sk_buff *skb, uint32 len);
void skb_reserve(struct sk_buff *skb, int len);
void skb_trim(struct sk_buff *skb, unsigned int len);

#endif

