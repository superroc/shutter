/*
 ****************************************************************************************
 * Copyright (C) Accel Semiconductors Corp. 2006
 *
 * Project: Bluetooth Software
 *
 * Description: provide l2cap api
 *
 *
 * Revision History:
 * 
 *
 *****************************************************************************************/
#ifndef _L2CAP_API_H
#define _L2CAP_API_H

#include "co_skbuff.h"
#include "stm32f10x_type.h"
#include "btm_hci.h"

enum l2cap_event_enum {
    L2CAP_CHANNEL_OPENED,
    L2CAP_CHANNEL_NEW_OPENED,
    L2CAP_CHANNEL_CLOSED
};


int8 l2cap_init ( void );


int8 l2cap_register  (uint16 psm, 
                      int8 l2cap_conn_count_max, 
                      void (*l2cap_notify_callback)(enum l2cap_event_enum event, uint32 l2cap_handle, void *pdata, uint8 reason),
                      void (*l2cap_datarecv_callback)(uint32 l2cap_handle, struct sk_buff *skb)
                      );


uint32 l2cap_open (struct bdaddr_t *remote, 
                    uint16 psm, 
                    void (*l2cap_notify_callback)(enum l2cap_event_enum event, uint32 l2cap_handle, void *pdata, uint8 reason),
                    void (*l2cap_datarecv_callback)(uint32 l2cap_handle, struct sk_buff *skb)
                    );


int8 l2cap_close (uint32 l2cap_handle);

struct sk_buff *l2cap_data_skb_alloc(uint32 datalen);

int8 l2cap_send_data_skb( uint32 l2cap_handle, struct sk_buff *skb);

int8 l2cap_send_data( uint32 l2cap_handle, uint8 *data, uint32 datalen);

int8 l2cap_unregister(uint16 psm);

int8 l2cap_close_delay (uint32 l2cap_handle, int delay_sec);


/* below is called by lower layer (btm) */
void l2cap_btm_notify_callback(enum btm_l2cap_event_enum event, uint16 conn_handle, void *pdata, uint8 reason);
void l2cap_btm_datarecv_callback (uint16 conn_handle, struct sk_buff *skb);

#endif

