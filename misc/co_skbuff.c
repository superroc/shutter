#include "co_skbuff.h"
#include "buffer.h"

struct sk_buff *skb_fill (uint8 *data, uint32 datalen)
{
    struct sk_buff *skb;
    
    skb = (void *)malloc_buffer(sizeof(struct sk_buff));
    if (skb == NULL) {
        return NULL;
    }
    
    skb->len = datalen;
    skb->head = data;
    skb->data = data;
    skb->tail = data + datalen;
    skb->end = data + datalen;
    
    return skb;
    
}
struct sk_buff *skb_alloc(uint32 size)
{
    struct sk_buff *skb;
    uint8 *data;
    
    skb = (void *)malloc_buffer(sizeof(struct sk_buff));
    if (skb == NULL) {
        return NULL;
    }
    
    data = malloc_buffer(size);
    if (data == NULL) {
        free_buffer((void *)skb);
        return NULL;
    }
    skb->len = 0;
    skb->head = data;
    skb->data = data;
    skb->tail = data;
    skb->end = data + size;
    
    return skb;
    
}

void skb_free( struct sk_buff *skb)
{
	if (skb==NULL)return;
    free_buffer((void *)skb->head);
    free_buffer((void *)skb);
}

/**
 *	skb_put - add data to a buffer
 *	@skb: buffer to use
 *	@len: amount of data to add
 */

uint8 *skb_put(struct sk_buff *skb, uint32 len)
{
	uint8 *tmp = skb->tail;
	skb->tail += len;
	skb->len  += len;
	return tmp;
}

/**
 *	skb_putdata - add data to a buffer
 *	@skb: buffer to use
 *    @data: the "in" data pointer
 *	@len: amount of data to add
 */

void skb_putdata(struct sk_buff *skb, uint8 *data, uint32 len)
{
	memcpy(skb->tail, data, len);
	skb->tail += len;
	skb->len  += len;
}

/**
 *	skb_push - add data to the start of a buffer
 *	@skb: buffer to use
 *	@len: amount of data to add
 */
uint8 *skb_push(struct sk_buff *skb, uint32 len)
{
	skb->data -= len;
	skb->len  += len;
	return skb->data;
}


/**
 *	skb_pull - remove data from the start of a buffer
 *	@skb: buffer to use
 *	@len: amount of data to remove
 */
uint8 *skb_pull(struct sk_buff *skb, uint32 len)
{
    skb->len -= len;
    return skb->data += len;
}

/**
 *	skb_reserve - adjust headroom
 *	@skb: buffer to alter
 *	@len: bytes to move
 *
 *	Increase the headroom of an empty &sk_buff by reducing the tail
 *	room. This is only allowed for an empty buffer.
 */
void skb_reserve(struct sk_buff *skb, int len)
{
	skb->data += len;
	skb->tail += len;
}

/**
 *	skb_trim - remove end from a buffer
 *	@skb: buffer to alter
 *	@len: new length
 */
void skb_trim(struct sk_buff *skb, unsigned int len)
{
	skb->len  = len;
	skb->tail = skb->data + len;
}

