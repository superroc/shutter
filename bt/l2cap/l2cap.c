/*
 ****************************************************************************************
 * Copyright (C) unisim Semiconductors Corp. 2006
 *
 * Project: Bluetooth Software
 *
 * Description:
 *
 *
 * Revision History:
 * 
 *
 *****************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "l2cap.h"
#include "btm_security.h"
#include "co_timer.h"

#define PSM_SDP		0x0001
#define PSM_HIDC	0x0011
#define PSM_HIDI	0x0013

#define  __set_mask(var, mask)   {var |= mask;}
#define  __test_mask(var, mask)   (var & mask)

#define  __test_and_clear_mask(var, mask)  ((var & mask)?(var &= ~mask,1):0)

struct l2cap_registered_psm_item_t hid_control_registered_psm;
struct l2cap_registered_psm_item_t hid_interrupt_registered_psm;
struct l2cap_registered_psm_item_t sdp_registered_psm;

struct l2cap_channel hid_control_l2cap_channel;
struct l2cap_channel hid_interrupt_l2cap_channel;
struct l2cap_channel sdp_l2cap_channel;

struct l2cap_conn l2cap_connection;

int8 l2cap_init_flag = 0;
int8 ssp_done = 0;

struct l2cap_channel *l2cap_channel_search_l2caphandle(uint32 l2cap_handle);
struct l2cap_channel *l2cap_channel_search_scid(struct l2cap_conn *conn,uint16 scid);
int8 l2cap_channel_close (struct l2cap_channel *channel, uint8 reason);
int8 l2cap_send_disconnect_req(struct l2cap_channel *channel);

struct l2cap_registered_psm_item_t *l2cap_registered_psm_search (uint16 psm)
{
    if(psm == PSM_SDP)
		return &sdp_registered_psm;
	else if(psm == PSM_HIDC)
		return &hid_control_registered_psm;
	else if(psm == PSM_HIDI)
		return &hid_interrupt_registered_psm;
	else
	    return NULL;
}

uint32 l2cap_overrun_newhandle(void)
{
    uint32 id;
    for (id=1; id<0xffffffff; id++) {
        if (l2cap_channel_search_l2caphandle(id) == NULL) {
            return id;
        }
    }
    return 0;
}

/*0 error*/
uint32 l2cap_newhandle (void)
{
    static uint32 handle_id = 1;
    if(handle_id < 0xffffffff) {
        return handle_id++;
    }

    return l2cap_overrun_newhandle();
}

uint16 l2cap_overrun_new_scid (struct l2cap_conn *conn)
{
    uint32 id;
    for (id=0x40; id<0xffff; id++) {
        if (l2cap_channel_search_scid(conn, id) == NULL) {
            return id;
        }
    }
    return 0;
}

uint16 l2cap_new_scid(struct l2cap_conn *conn)
{
    static uint32 scid = 0x40;
    if(scid < 0xffff) {
        return scid++;
    }

    return l2cap_overrun_new_scid(conn);
}



uint8 l2cap_new_sigid(void)
{
    static uint8 l2cap_sigid = 1;
    
    if (l2cap_sigid == 0xff) {
        l2cap_sigid = 1;
    }
    return l2cap_sigid++;
}



struct l2cap_conn *l2cap_conn_search (struct bdaddr_t *bdaddr)
{
    struct l2cap_conn *conn = &l2cap_connection;

	if (bdaddr_equal(bdaddr, &conn->remote)) {
		return conn;
	}
	
    return NULL;
}


struct l2cap_conn *l2cap_conn_add_new (struct bdaddr_t *bdaddr)
{
    struct l2cap_conn *conn = &l2cap_connection;

    conn->conn_handle = 0xffff;            /*invalid conn handle*/
    conn->remote = *bdaddr;
    
    return conn;
}


struct l2cap_conn *l2cap_conn_search_conn_handle(uint16 conn_handle)
{
    struct l2cap_conn *conn = &l2cap_connection;

	if(conn->conn_handle == conn_handle)
		return conn;
	else
		return NULL;
}


void l2cap_conn_close (struct l2cap_conn *conn, uint8 reason)
{
    struct l2cap_channel *channel, *temp;

	if(hid_control_l2cap_channel.state != L2CAP_CLOSE) {
		hid_control_l2cap_channel.state = L2CAP_CLOSE;
		l2cap_channel_close(&hid_control_l2cap_channel, reason);
	}

	if(hid_interrupt_l2cap_channel.state != L2CAP_CLOSE) {
		hid_interrupt_l2cap_channel.state = L2CAP_CLOSE;
		l2cap_channel_close(&hid_interrupt_l2cap_channel, reason);
	}

	if(sdp_l2cap_channel.state != L2CAP_CLOSE) {
		sdp_l2cap_channel.state = L2CAP_CLOSE;
		l2cap_channel_close(&sdp_l2cap_channel, reason);
	}

    l2cap_connection.conn_handle = 0xFFFF;

    btm_conn_acl_close(&conn->remote);
}


struct l2cap_channel *l2cap_channel_search_sigid(struct l2cap_conn *conn, uint8 sigid)
{
    if((hid_control_l2cap_channel.state != L2CAP_CLOSE) && (hid_control_l2cap_channel.sigid_last_send == sigid))
		return &hid_control_l2cap_channel;
	if((hid_interrupt_l2cap_channel.state != L2CAP_CLOSE) && (hid_interrupt_l2cap_channel.sigid_last_send == sigid))
		return &hid_interrupt_l2cap_channel;
	if((sdp_l2cap_channel.state != L2CAP_CLOSE) && (sdp_l2cap_channel.sigid_last_send == sigid))
		return &sdp_l2cap_channel;
    return NULL;
}

struct l2cap_channel *l2cap_channel_search_l2caphandle(uint32 l2cap_handle)
{
    if((hid_control_l2cap_channel.state != L2CAP_CLOSE) && (hid_control_l2cap_channel.l2cap_handle== l2cap_handle))
		return &hid_control_l2cap_channel;
	if((hid_interrupt_l2cap_channel.state != L2CAP_CLOSE) && (hid_interrupt_l2cap_channel.l2cap_handle == l2cap_handle))
		return &hid_interrupt_l2cap_channel;
	if((sdp_l2cap_channel.state != L2CAP_CLOSE) && (sdp_l2cap_channel.l2cap_handle == l2cap_handle))
		return &sdp_l2cap_channel;
    return NULL;
}

struct l2cap_channel *l2cap_channel_search_scid(struct l2cap_conn *conn,uint16 scid)
{
	if((hid_control_l2cap_channel.state != L2CAP_CLOSE) && (hid_control_l2cap_channel.scid == scid))
		return &hid_control_l2cap_channel;
	if((hid_interrupt_l2cap_channel.state != L2CAP_CLOSE) && (hid_interrupt_l2cap_channel.scid == scid))
		return &hid_interrupt_l2cap_channel;
	if((sdp_l2cap_channel.state != L2CAP_CLOSE) && (sdp_l2cap_channel.scid == scid))
		return &sdp_l2cap_channel;
    return NULL;
}

struct l2cap_channel *l2cap_channel_search_dcid(struct l2cap_conn *conn,uint16 dcid)
{
	if((hid_control_l2cap_channel.state != L2CAP_CLOSE) && (hid_control_l2cap_channel.dcid== dcid))
		return &hid_control_l2cap_channel;
	if((hid_interrupt_l2cap_channel.state != L2CAP_CLOSE) && (hid_interrupt_l2cap_channel.dcid == dcid))
		return &hid_interrupt_l2cap_channel;
	if((sdp_l2cap_channel.state != L2CAP_CLOSE) && (sdp_l2cap_channel.dcid == dcid))
		return &sdp_l2cap_channel;
	return NULL;
}

struct l2cap_channel *l2cap_channel_add_new ( struct l2cap_conn *conn, uint16 psm)
{
    struct l2cap_channel *channel;

	if(psm == PSM_SDP)
		channel = &sdp_l2cap_channel;
	else if(psm == PSM_HIDC)
		channel = &hid_control_l2cap_channel;
	else if(psm == PSM_HIDI)
		channel = &hid_interrupt_l2cap_channel;
	else
		return NULL;

    channel->conn = conn;
    channel->l2cap_handle = 0;  /*invalid l2cap handle, need to set value later.*/
    channel->scid = l2cap_new_scid(conn);
    channel->dcid = 0;
    channel->psm_remote = 0;

    channel->sigid_last_recv = 0;
    channel->sigid_last_send = 0;
    

    channel->cfgout.cfgout_flag = 0;
    channel->cfgout.mtu_remote.mtu = L2CAP_CFG_MTU_INCOMING;
    channel->cfgout.flushto_local.flushto = L2CAP_CFG_FLUSH_TO;
    channel->cfgout.qos_local.service_type = L2CAP_CFG_QOS_SERVICE_TYPE;
    channel->cfgout.rfc_remote.mode = L2CAP_CFG_RFC_MODE;

    channel->cfgin.cfgin_flag = 0;
    channel->cfgin.mtu_local.mtu = L2CAP_CFG_MTU;
    channel->cfgin.flushto_remote.flushto = L2CAP_CFG_FLUSH_TO;
    channel->cfgin.qos_remote.service_type = L2CAP_CFG_QOS_SERVICE_TYPE;
    channel->cfgin.rfc_local.mode = L2CAP_CFG_RFC_MODE;
	channel->disconnect_req_timeout_timer = 0;
	channel->close_delay_timer = 0;
	channel->wait_config_req_timer = 0;
	channel->wait_conn_req_timer = 0;
	channel->disconnect_req_reason = 0;
	
    
    channel->state = L2CAP_CLOSE;
    channel->l2cap_notify_callback= NULL;
    channel->l2cap_datarecv_callback = NULL;

    return channel;
}

void l2cap_disconn_req_timeout(void *arg)
{
    struct l2cap_channel *channel;

    channel = (struct l2cap_channel *)arg;

    channel->state = L2CAP_CLOSE;

    l2cap_channel_close(channel, channel->disconnect_req_reason);
}

extern int8 l2cap_send_cfg_req(struct l2cap_channel *channel);
void l2cap_wait_cfg_req_timeout(void *arg)
{
    struct l2cap_channel *channel;

    channel = (struct l2cap_channel *)arg;


    channel->state = L2CAP_WAIT_CONFIG_REQ_RSP;

    l2cap_send_cfg_req(channel);
    //co_printf("\r\ntimer in timeout\r\n");
    if (channel->wait_config_req_timer!=0){
    	co_timer_del(channel->wait_config_req_timer);
    	channel->wait_config_req_timer = 0;
    }
}

void l2cap_wait_conn_req_timeout(void *arg)
{
	struct l2cap_channel *channel;

	channel = (struct l2cap_channel *)arg;
	if(channel->state!=L2CAP_WAIT_CONNECTION_RSP){
	    if (channel->wait_conn_req_timer!=0){
	    	co_timer_del(channel->wait_conn_req_timer);
	    	channel->wait_conn_req_timer = 0;
	    }
		return;
	}

	btm_security_askfor_authority(channel->conn->conn_handle,channel->psm_remote);
	
    if (channel->wait_conn_req_timer!=0){
    	co_timer_del(channel->wait_conn_req_timer);
    	channel->wait_conn_req_timer = 0;
    }

}

int8 l2cap_channel_close ( struct l2cap_channel *channel, uint8 reason )
{

    if (channel->state >= L2CAP_WAIT_CONFIG){
        channel->state = L2CAP_WAIT_DISCONNECT;
        channel->disconnect_req_timeout_timer=co_timer_new(1000,l2cap_disconn_req_timeout,channel,1);
        if (channel->disconnect_req_timeout_timer==0) {
        	return FAILURE;
        }
        channel->disconnect_req_reason = reason;
        l2cap_send_disconnect_req(channel);
	    return SUCCESS;
        
    }
    if (channel->state == L2CAP_WAIT_DISCONNECT){
    	return SUCCESS;
    }
    
    if (channel->disconnect_req_timeout_timer!=0){
    	co_timer_del(channel->disconnect_req_timeout_timer);
    	channel->disconnect_req_timeout_timer = 0;
    }
    if (channel->wait_config_req_timer!=0){
    	co_timer_del(channel->wait_config_req_timer);
    	channel->wait_config_req_timer = 0;
    }
    if (channel->wait_conn_req_timer!=0){
    	co_timer_del(channel->wait_conn_req_timer);
    	channel->wait_conn_req_timer = 0;
    }
    if (channel->close_delay_timer!=0){
    	co_timer_del(channel->close_delay_timer);
    	channel->close_delay_timer = 0;
    }

    if (channel->l2cap_handle != 0) {
        channel->l2cap_notify_callback(L2CAP_CHANNEL_CLOSED, channel->l2cap_handle, NULL, reason);
    }

    return SUCCESS;
}



uint8 *l2cap_make_sig_req(struct l2cap_channel *channel,
                                                                uint8 sig_code,
                                                                uint16 sig_datalen,
                                                                struct sk_buff *skb)
{
    struct l2cap_hdr *hdr;
    struct l2cap_sig_hdr *sighdr;
    
    hdr = (void *)skb_put(skb, sizeof(*hdr)); 
    hdr->len = sizeof(*sighdr)+sig_datalen;
    hdr->cid = L2CAP_SIG_CID;

    sighdr = (void *)skb_put(skb, sizeof(*sighdr));
    sighdr->code = sig_code;
    sighdr->id     = l2cap_new_sigid();
    sighdr->len   = sig_datalen;

    channel->sigid_last_send = sighdr->id;
    
    return (uint8 *)skb_put(skb, sig_datalen);

}

__inline uint8 *l2cap_make_sig_rsp(  uint8 sig_code,
                                                                uint8 sigid,
                                                                uint16 siglen,
                                                                struct sk_buff *skb)
{
    struct l2cap_hdr *hdr;
    struct l2cap_sig_hdr *sighdr;
    
    hdr = (void *)skb_put(skb, sizeof(*hdr)); 
    hdr->len = sizeof(*sighdr)+siglen;
    hdr->cid = L2CAP_SIG_CID;

    sighdr = (void *)skb_put(skb, sizeof(*sighdr));
    sighdr->code = sig_code;
    sighdr->id     = sigid;
    sighdr->len   = siglen;
   
    return (uint8 *)skb_put(skb, siglen);

}


int8 l2cap_send_frame(uint16 conn_handle, struct sk_buff *skb)
{
    if (conn_handle==0xffff) {
        return FAILURE;
    }
    return btm_conn_acl_send_skb(conn_handle , skb);
}
int8 __l2cap_send_data_skb(struct l2cap_channel *channel, struct sk_buff *skb)
{
    struct l2cap_hdr *hdr;

    hdr = (void *)skb_push(skb, sizeof(*hdr));
    hdr->cid = channel->dcid;
    hdr->len = skb->len - (sizeof(*hdr));
    return l2cap_send_frame(channel->conn->conn_handle , skb);
}

int8 l2cap_send_command_rej (uint16 conn_handle, uint8 sigid, uint16 reason, uint16 scid_or_mtu, uint16 dcid)
{

    
    struct sk_buff *skb;
    struct l2cap_sig_rej *sig;
    uint8 siglen;

    switch (reason) {
        case L2CAP_SIG_REASON_NOT_UNDERSTOOD:
            siglen = 2;
        break;
        case L2CAP_SIG_REASON_MTU_EXCEED:
            siglen = 4;
        break;
        case L2CAP_SIG_REASON_INVALID_CID:
            siglen = 6;
        break;
        default:
            return FAILURE;
    }
    
    skb = skb_alloc(sizeof(struct l2cap_hdr)+sizeof(struct l2cap_sig_hdr)+sizeof(*sig));

    if (skb == NULL) return FAILURE;
    sig = (void *)l2cap_make_sig_rsp(L2CAP_SIG_REJ, sigid, siglen, skb);

    sig->reason = reason;
    sig->scid = scid_or_mtu;
    sig->dcid = dcid;
    
    return l2cap_send_frame(conn_handle, skb);
    
}


int8 l2cap_send_connect_req( struct l2cap_channel *channel)
{

    struct sk_buff *skb;
    struct l2cap_sig_conn_req *sig;

    skb = skb_alloc(sizeof(struct l2cap_hdr)+sizeof(struct l2cap_sig_hdr)+sizeof(*sig));

    if (skb == NULL) return FAILURE;

    sig = (void *)l2cap_make_sig_req(channel, L2CAP_SIG_CONN_REQ, sizeof(*sig), skb);

    sig->psm = channel->psm_remote;
    sig->scid = channel->scid;

    return l2cap_send_frame(channel->conn->conn_handle , skb);
    
}

int8 l2cap_send_connect_rsp( struct l2cap_channel *channel, uint16 result, uint16 status)
{

    struct l2cap_sig_conn_rsp *sig;
    
    struct sk_buff *skb;

    skb = skb_alloc(sizeof(struct l2cap_hdr)+sizeof(struct l2cap_sig_hdr)+sizeof(*sig));

    if (skb == NULL) return FAILURE;

    sig = (void *)l2cap_make_sig_rsp(L2CAP_SIG_CONN_RSP, channel->sigid_last_recv, sizeof(*sig), skb);
    sig->dcid = channel->scid;
    sig->scid = channel->dcid;
    sig->result = result;
    sig->status = status;

    return l2cap_send_frame(channel->conn->conn_handle, skb);
    
}

int8 l2cap_send_connect_rsp_raw( uint16 conn_handle, uint8 sigid, uint16 dcid, uint16 scid,uint16 result, uint16 status)
{

    struct l2cap_sig_conn_rsp *sig;
    
    struct sk_buff *skb;

    skb = skb_alloc(sizeof(struct l2cap_hdr)+sizeof(struct l2cap_sig_hdr)+sizeof(*sig));

    if (skb == NULL) return FAILURE;

    sig = (void *)l2cap_make_sig_rsp(L2CAP_SIG_CONN_RSP, sigid, sizeof(*sig), skb);

    sig->dcid = dcid;
    sig->scid = scid;
    sig->result = result;
    sig->status = status;

    return l2cap_send_frame(conn_handle, skb);
}



int8 l2cap_send_cfg_req(struct l2cap_channel *channel)
{

    struct sk_buff *skb;

    struct l2cap_hdr *hdr;
    struct l2cap_sig_hdr *sighdr;

    struct l2cap_sig_cfg_req *sig;
    struct l2cap_sig_cfg_opt_hdr *cfghdr;
    
    uint8 *ptr;

    uint8 cfg_len = 0;

    if (channel->cfgout.mtu_remote.mtu != L2CAP_DEFAULT_MTU) {
        cfg_len += sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_mtu);
        __set_mask(channel->cfgout.cfgout_flag,L2CAP_SIG_CFG_MTU_MASK);
    }
    if (channel->cfgout.flushto_local.flushto != L2CAP_DEFAULT_FLUSH_TO) {
        cfg_len += sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_flushto);
        __set_mask(channel->cfgout.cfgout_flag,L2CAP_SIG_CFG_FLUSHTO_MASK);
    }
    if (channel->cfgout.qos_local.service_type != L2CAP_DEFAULT_QOS_SERVICE_TYPE) {
        cfg_len += sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_qos);
        __set_mask(channel->cfgout.cfgout_flag,L2CAP_SIG_CFG_QOS_MASK);
    }
    if (channel->cfgout.rfc_remote.mode != L2CAP_DEFAULT_RFC_MODE) {
        cfg_len += sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_rfc);
        __set_mask(channel->cfgout.cfgout_flag,L2CAP_SIG_CFG_RFC_MASK);
    }

    
    skb = skb_alloc(sizeof(struct l2cap_hdr)+sizeof(struct l2cap_sig_hdr)+sizeof(*sig)+cfg_len);

    if (skb == NULL) return FAILURE;

   
    hdr = (void *)skb_put(skb, sizeof(*hdr)); 
    hdr->len = sizeof(*sighdr)+sizeof(*sig)+cfg_len;
    hdr->cid = L2CAP_SIG_CID;

    sighdr = (void *)skb_put(skb, sizeof(*sighdr));
    sighdr->code = L2CAP_SIG_CFG_REQ;
    sighdr->id     = l2cap_new_sigid();
    sighdr->len   = sizeof(*sig)+cfg_len;

    sig = (void *)skb_put(skb, sizeof(*sig));
    sig->dcid = channel->dcid;
    sig->flags = 0x0;

    if (cfg_len == 0) goto send;

    if (__test_mask(channel->cfgout.cfgout_flag,L2CAP_SIG_CFG_MTU_MASK)) {
        cfghdr = (void *)skb_put(skb, sizeof(*cfghdr));
        cfghdr->type = L2CAP_CFG_TYPE_MTU;
        cfghdr->len  = sizeof(struct l2cap_sig_cfg_opt_mtu);

        ptr = skb_put(skb,sizeof(struct l2cap_sig_cfg_opt_mtu));
        *(struct l2cap_sig_cfg_opt_mtu *)ptr = channel->cfgout.mtu_remote;
        
    }


    if (__test_mask(channel->cfgout.cfgout_flag,L2CAP_SIG_CFG_FLUSHTO_MASK)) {
        cfghdr = (void *)skb_put(skb, sizeof(*cfghdr));
        cfghdr->type = L2CAP_CFG_TYPEF_FLUSH_TO;
        cfghdr->len  = sizeof(struct l2cap_sig_cfg_opt_flushto);

        ptr = skb_put(skb,sizeof(struct l2cap_sig_cfg_opt_flushto));
        *(struct l2cap_sig_cfg_opt_flushto *)ptr = channel->cfgout.flushto_local;
        
    }


    if (__test_mask(channel->cfgout.cfgout_flag,L2CAP_SIG_CFG_QOS_MASK)) {
        cfghdr = (void *)skb_put(skb, sizeof(*cfghdr));
        cfghdr->type = L2CAP_CFG_TYPE_QOS;
        cfghdr->len  = sizeof(struct l2cap_sig_cfg_opt_qos);

        ptr = skb_put(skb,sizeof(struct l2cap_sig_cfg_opt_qos));
        *(struct l2cap_sig_cfg_opt_qos *)ptr = channel->cfgout.qos_local;
        
    }


    if (__test_mask(channel->cfgout.cfgout_flag,L2CAP_SIG_CFG_RFC_MASK)) {
        cfghdr = (void *)skb_put(skb, sizeof(*cfghdr));
        cfghdr->type = L2CAP_CFG_TYPE_RFC;
        cfghdr->len  = sizeof(struct l2cap_sig_cfg_opt_rfc);

        ptr = skb_put(skb,sizeof(struct l2cap_sig_cfg_opt_rfc));
        *(struct l2cap_sig_cfg_opt_rfc *)ptr = channel->cfgout.rfc_remote;
        
    }


send:

    channel->sigid_last_send = sighdr->id;
    return l2cap_send_frame(channel->conn->conn_handle , skb);
    
}


/*-------------------------------------------------------------------------
    Function    :  l2cap_send_cfg_rsp             xxx
    Parameters:
        N/A
    Return:
        SUCCESS:means the before recved cfg req has been all accepted
        FAILURE  : means the cfg req not be accepted or other error happens.
    Description:
        N/A
-------------------------------------------------------------------------*/
int8 l2cap_send_cfg_rsp( struct l2cap_channel *channel)
{

    struct l2cap_hdr *hdr;
    struct l2cap_sig_hdr *sighdr;
    struct l2cap_sig_cfg_rsp *sig;
    struct l2cap_sig_cfg_opt_hdr *cfghdr;

    uint8 cfg_len;
    uint8 *ptr;

    int8 err;
    
    struct sk_buff *skb;

    skb = skb_alloc(sizeof(*hdr)+sizeof(*sighdr)+sizeof(*sig)+
                              sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_mtu) +
                              sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_flushto) +
                              sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_qos) +
                              sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_rfc));
    
    if (skb == NULL) return FAILURE;
    
    cfg_len = 0;
    hdr = (void *)skb_put(skb, sizeof(*hdr)); 
    sighdr = (void *)skb_put(skb, sizeof(*sighdr));
    sig = (void *)skb_put(skb, sizeof(*sig));
    sig->result = L2CAP_CFGRSP_SUCCESS;


    /* recv mtu cfg req */
    if (__test_and_clear_mask(channel->cfgin.cfgin_flag,L2CAP_SIG_CFG_MTU_MASK)) {

        if (channel->cfgin.mtu_local.mtu<L2CAP_MIN_MTU) {
            /*reject*/
            sig->result = L2CAP_CFGRSP_REJ;
            cfg_len = 0;
            goto tx;
        }
        else
        if (channel->cfgin.mtu_local.mtu>L2CAP_CFG_MTU) {

            
            /* trim the before putin data*/
            /*no data*/
            cfghdr = (void *)skb_put(skb, sizeof(*cfghdr));
            cfghdr->type = L2CAP_CFG_TYPE_MTU;
            cfghdr->len  = sizeof(struct l2cap_sig_cfg_opt_mtu);
            ptr = skb_put(skb,sizeof(struct l2cap_sig_cfg_opt_mtu));

            cfg_len += sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_mtu);
            *(struct l2cap_sig_cfg_opt_mtu *)ptr = channel->cfgin.mtu_local;


            /* we still accept this, but we send out little mtu in real */            
            
            channel->cfgin.mtu_local.mtu = L2CAP_CFG_MTU;
        }
        else {
            /* success, just copy */
            cfghdr = (void *)skb_put(skb, sizeof(*cfghdr));
            cfghdr->type = L2CAP_CFG_TYPE_MTU;
            cfghdr->len  = sizeof(struct l2cap_sig_cfg_opt_mtu);
            ptr = skb_put(skb,sizeof(struct l2cap_sig_cfg_opt_mtu));
            
            cfg_len += sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_mtu);
            *(struct l2cap_sig_cfg_opt_mtu *)ptr = channel->cfgin.mtu_local;
        }
        
    }

    /* recv flushto cfg req */
    if (__test_and_clear_mask(channel->cfgin.cfgin_flag,L2CAP_SIG_CFG_FLUSHTO_MASK)) {
        
        if (0)  //(channel->cfgin.flushto_remote.flushto!=L2CAP_DEFAULT_FLUSH_TO) 
        {
            /* adjust the value */

            /* first trim the before putin data */
            skb_trim(skb, skb->len - cfg_len);

            cfghdr = (void *)skb_put(skb, sizeof(*cfghdr));
            cfghdr->type = L2CAP_CFG_TYPEF_FLUSH_TO;
            cfghdr->len  = sizeof(struct l2cap_sig_cfg_opt_flushto);
            ptr = skb_put(skb,sizeof(struct l2cap_sig_cfg_opt_flushto));

            ((struct l2cap_sig_cfg_opt_flushto *)ptr)->flushto  = L2CAP_DEFAULT_FLUSH_TO;
            sig->result = L2CAP_CFGRSP_UNACCEPT_PARAMS;
            
            cfg_len = sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_flushto);
            goto tx;
        }
        else {
            /* success, just copy */
            cfghdr = (void *)skb_put(skb, sizeof(*cfghdr));
            cfghdr->type = L2CAP_CFG_TYPEF_FLUSH_TO;
            cfghdr->len  = sizeof(struct l2cap_sig_cfg_opt_flushto);
            ptr = skb_put(skb,sizeof(struct l2cap_sig_cfg_opt_flushto));
            
            cfg_len += sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_flushto);
            *(struct l2cap_sig_cfg_opt_flushto *)ptr = channel->cfgin.flushto_remote;
        }
        
    }

    /* recv qos cfg req */
    if (__test_and_clear_mask(channel->cfgin.cfgin_flag,L2CAP_SIG_CFG_QOS_MASK)) {


        if (channel->cfgin.qos_remote.service_type==L2CAP_QOS_GUARANTEED) {

            /* adjust the value */
            /* first trim the before putin data */
            skb_trim(skb, skb->len - cfg_len);

            cfghdr = (void *)skb_put(skb, sizeof(*cfghdr));
            cfghdr->type = L2CAP_CFG_TYPE_QOS;
            cfghdr->len  = sizeof(struct l2cap_sig_cfg_opt_qos);
            
            ptr = skb_put(skb,sizeof(struct l2cap_sig_cfg_opt_qos));

            ((struct l2cap_sig_cfg_opt_qos *)ptr)->service_type  = L2CAP_DEFAULT_QOS_SERVICE_TYPE;
            /*asdf:maybe add more parameter default value?*/
            
            sig->result = L2CAP_CFGRSP_UNACCEPT_PARAMS;
            
            cfg_len = sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_qos);
            goto tx;
        }
        else {
            /* success, just copy */
            cfghdr = (void *)skb_put(skb, sizeof(*cfghdr));
            cfghdr->type = L2CAP_CFG_TYPE_QOS;
            cfghdr->len  = sizeof(struct l2cap_sig_cfg_opt_qos);
            
            ptr = skb_put(skb,sizeof(struct l2cap_sig_cfg_opt_qos));
            
            cfg_len += sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_qos);
            *(struct l2cap_sig_cfg_opt_qos *)ptr = channel->cfgin.qos_remote;
        }

        
    }

    /* recv rfc cfg req */
    if (__test_and_clear_mask(channel->cfgin.cfgin_flag,L2CAP_SIG_CFG_RFC_MASK)) {

        
        if (channel->cfgin.rfc_local.mode != L2CAP_DEFAULT_RFC_MODE) {
            
            /* adjust the value */
            /* first trim the before putin data */
            skb_trim(skb, skb->len - cfg_len);

            cfghdr = (void *)skb_put(skb, sizeof(*cfghdr));
            cfghdr->type = L2CAP_CFG_TYPE_RFC;
            cfghdr->len  = sizeof(struct l2cap_sig_cfg_opt_rfc);
            
            ptr = skb_put(skb,sizeof(struct l2cap_sig_cfg_opt_rfc));

            ((struct l2cap_sig_cfg_opt_rfc *)ptr)->mode  = L2CAP_DEFAULT_RFC_MODE;
            /*asdf:maybe add more parameter default value?*/
            
            sig->result = L2CAP_CFGRSP_UNACCEPT_PARAMS;
            
            cfg_len = sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_rfc);
            goto tx;
        }
        else {
            /* success, just copy */
            cfghdr = (void *)skb_put(skb, sizeof(*cfghdr));
            cfghdr->type = L2CAP_CFG_TYPE_RFC;
            cfghdr->len  = sizeof(struct l2cap_sig_cfg_opt_rfc);
            
            ptr = skb_put(skb,sizeof(struct l2cap_sig_cfg_opt_rfc));
            
            cfg_len += sizeof(*cfghdr) + sizeof(struct l2cap_sig_cfg_opt_rfc);
            *(struct l2cap_sig_cfg_opt_rfc *)ptr = channel->cfgin.rfc_local;
        }
        
    }
    
tx:
    hdr->cid = L2CAP_SIG_CID;
    hdr->len = sizeof(*sighdr)+sizeof(*sig)+cfg_len;


    sighdr->code = L2CAP_SIG_CFG_RSP;
    sighdr->id     = channel->sigid_last_recv;
    sighdr->len = sizeof(*sig)+cfg_len;

    sig->scid = channel->dcid;
    sig->flags = 0;      /*no continue cfg rsp*/

    err = SUCCESS;

    if (sig->result != L2CAP_CFGRSP_SUCCESS) {
        err = FAILURE;
    }

    if (l2cap_send_frame(channel->conn->conn_handle , skb) == FAILURE) {
        err = FAILURE;
    }
    
    return err;
}


int8 l2cap_send_cfg_rsp_reject( uint16 conn_handle, uint8 sigid, uint16 scid)
{

    struct l2cap_sig_cfg_rsp *sig;
    
    struct sk_buff *skb;
    
    skb = skb_alloc(sizeof(struct l2cap_hdr)+sizeof(struct l2cap_sig_hdr)+sizeof(*sig));

    if (skb == NULL) return FAILURE;

    sig = (void *)l2cap_make_sig_rsp(L2CAP_SIG_CFG_RSP, sigid, sizeof(*sig), skb);

    sig->scid = scid;
    sig->flags = 0x0;
    sig->result = L2CAP_CFGRSP_REJ;

    return l2cap_send_frame(conn_handle, skb);
}

int8 l2cap_send_cfg_rsp_unkown( uint16 conn_handle, uint8 sigid, uint16 scid, uint8 *cfg_p, uint8 cfg_len)
{

    struct l2cap_sig_cfg_rsp *sig;
    uint8 *ptr;
    
    struct sk_buff *skb;

    
    skb = skb_alloc(sizeof(struct l2cap_hdr)+sizeof(struct l2cap_sig_hdr)+sizeof(*sig)+cfg_len);

    if (skb == NULL) return FAILURE;

    sig = (void *)l2cap_make_sig_rsp(L2CAP_SIG_CFG_RSP, sigid, sizeof(*sig)+cfg_len, skb);

    sig->scid = scid;
    sig->flags = 0x0;
    sig->result = L2CAP_CFGRSP_UNKNOWN;

    ptr = (uint8 *)sig + sizeof(*sig) ;
    memcpy(ptr, cfg_p,cfg_len);

    return l2cap_send_frame(conn_handle, skb);
}


int8 l2cap_send_disconnect_req(struct l2cap_channel *channel)
{

    struct l2cap_sig_disconn_req *sig;
    
    struct sk_buff *skb;
    
    skb = skb_alloc(sizeof(struct l2cap_hdr)+sizeof(struct l2cap_sig_hdr)+sizeof(*sig));

    if (skb == NULL) return FAILURE;

    sig = (void *)l2cap_make_sig_req(channel, L2CAP_SIG_DISCONN_REQ, sizeof(*sig), skb);
    
    sig->dcid = channel->dcid;
    sig->scid = channel->scid;

    return l2cap_send_frame(channel->conn->conn_handle , skb);
    
}



int8 l2cap_send_disconnect_rsp(struct l2cap_channel *channel)
{

    struct l2cap_sig_disconn_rsp *sig;
    
    struct sk_buff *skb;

    skb = skb_alloc(sizeof(struct l2cap_hdr)+sizeof(struct l2cap_sig_hdr)+sizeof(*sig));

    if (skb == NULL) return FAILURE;

    sig = (void *)l2cap_make_sig_rsp(L2CAP_SIG_DISCONN_RSP, channel->sigid_last_recv, sizeof(*sig), skb);

    sig->dcid = channel->scid;
    sig->scid = channel->dcid;

    return l2cap_send_frame(channel->conn->conn_handle , skb);
    
}

int8 l2cap_send_disconnect_rsp_raw(uint16 conn_handle,uint8 sigid_ack,uint16 dcid,uint16 scid)
{

    struct l2cap_sig_disconn_rsp *sig;
    
    struct sk_buff *skb;

    skb = skb_alloc(sizeof(struct l2cap_hdr)+sizeof(struct l2cap_sig_hdr)+sizeof(*sig));

    if (skb == NULL) return FAILURE;

    sig = (void *)l2cap_make_sig_rsp(L2CAP_SIG_DISCONN_RSP, sigid_ack, sizeof(*sig), skb);

    sig->dcid = dcid;
    sig->scid = scid;

    return l2cap_send_frame(conn_handle , skb);
    
}

int8 l2cap_send_echo_rsp(uint16 conn_handle, uint8 sigid)
{

    struct l2cap_sig_disconn_rsp *sig;
    
    struct sk_buff *skb;

    
    skb = skb_alloc(sizeof(struct l2cap_hdr)+sizeof(struct l2cap_sig_hdr)+sizeof(*sig));

    if (skb == NULL) return FAILURE;

    sig = (void *)l2cap_make_sig_rsp(L2CAP_SIG_ECHO_RSP, sigid, 0, skb);


    return l2cap_send_frame(conn_handle, skb);
    
}

int8 l2cap_send_echo_req(uint16 conn_handle)
{
    struct l2cap_hdr *hdr;
    struct l2cap_sig_hdr *sighdr;
    
    struct sk_buff *skb;

    
    skb = skb_alloc(sizeof(struct l2cap_hdr)+sizeof(struct l2cap_sig_hdr));

    if (skb == NULL) return FAILURE;
    
    hdr = (void *)skb_put(skb, sizeof(*hdr)); 
    hdr->len = sizeof(*sighdr);
    hdr->cid = L2CAP_SIG_CID;

    sighdr = (void *)skb_put(skb, sizeof(*sighdr));
    sighdr->code = L2CAP_SIG_ECHO_REQ;
    sighdr->id     = 1; /*any thing*/
    sighdr->len   = 0;

    return l2cap_send_frame(conn_handle, skb);

}

int8 l2cap_send_info_rsp(uint16 conn_handle, uint8 sigid, uint16 infotype)
{

    struct l2cap_sig_info_rsp *sig;
    
    struct sk_buff *skb;

    
    skb = skb_alloc(sizeof(struct l2cap_hdr)+sizeof(struct l2cap_sig_hdr)+sizeof(*sig));

    if (skb == NULL) return FAILURE;

    else
    if (infotype == L2CAP_INFOTYPE_EXTENED_FEATURE) {
        sig = (void *)l2cap_make_sig_rsp(L2CAP_SIG_INFO_RSP, sigid, sizeof(*sig), skb);

        sig->infotype = infotype;
        sig->result = L2CAP_INFOTYPE_SUCCESS;
        sig->mask = 0;
    }
    else {  /*L2CAP_INFOTYPE_CONNLESS_MTU or others*/
        sig = (void *)l2cap_make_sig_rsp(L2CAP_SIG_INFO_RSP, sigid, 4, skb);
        sig->infotype = infotype;
        sig->result = L2CAP_INFOTYPE_NOT_SUPPORT;
    }

    return l2cap_send_frame(conn_handle, skb);
    
}


int8 l2cap_save_cfgin(struct l2cap_channel *channel, uint16 cfg_len, struct sk_buff *skb)
{

    struct l2cap_sig_cfg_opt_hdr *cfghdr;
    uint8 *ptr;

    while(cfg_len !=0) {
        cfghdr = (void *)skb->data;
        ptr = skb_pull(skb,sizeof(*cfghdr));
        skb_pull(skb,cfghdr->len);
        cfg_len -= sizeof(*cfghdr) + cfghdr->len;
        switch(cfghdr->type&0x7F) {
            case L2CAP_CFG_TYPE_MTU:
                channel->cfgin.mtu_local = *(struct l2cap_sig_cfg_opt_mtu *)ptr;
                __set_mask(channel->cfgin.cfgin_flag,L2CAP_SIG_CFG_MTU_MASK);
                
            break;
            case L2CAP_CFG_TYPEF_FLUSH_TO:
                channel->cfgin.flushto_remote = *(struct l2cap_sig_cfg_opt_flushto *)ptr;
                __set_mask(channel->cfgin.cfgin_flag,L2CAP_SIG_CFG_FLUSHTO_MASK);
            break;
            case L2CAP_CFG_TYPE_QOS:
                channel->cfgin.qos_remote = *(struct l2cap_sig_cfg_opt_qos *)ptr;
                __set_mask(channel->cfgin.cfgin_flag,L2CAP_SIG_CFG_QOS_MASK);
            break;
            case L2CAP_CFG_TYPE_RFC:
                channel->cfgin.rfc_local = *(struct l2cap_sig_cfg_opt_rfc *)ptr;
                __set_mask(channel->cfgin.cfgin_flag,L2CAP_SIG_CFG_RFC_MASK);
            break;
            default:
                if (cfghdr->type &0x80) {/*hint*/
                }
                else {
                    skb_pull(skb,cfg_len);
                    l2cap_send_cfg_rsp_unkown(channel->conn->conn_handle, channel->sigid_last_recv, channel->dcid, (void *)cfghdr, sizeof(*cfghdr) + cfghdr->len);
                    return FAILURE;
                }
            break;
        }
    }
    return SUCCESS;

   
}

void l2cap_get_sig_rej(struct l2cap_conn *conn, uint8 sigid, uint16 siglen, struct sk_buff *skb)
{

    struct l2cap_sig_rej *sig;
    struct l2cap_channel *channel;

    channel = l2cap_channel_search_sigid(conn, sigid);
    if (channel == NULL) {
        /* discard it */
        goto exit;
    }
    
    sig = (void *)skb->data;
	
    channel->state = L2CAP_CLOSE;
    l2cap_channel_close(channel, sig->reason);
    
exit:
    skb_pull(skb, siglen);

}



void l2cap_get_conn_req(struct l2cap_conn *conn, uint8 sigid, struct sk_buff *skb)
{

    struct l2cap_channel *channel;
    struct l2cap_registered_psm_item_t *item;

    struct l2cap_sig_conn_req *sig;

    sig = (void *)skb->data;

    item = l2cap_registered_psm_search(sig->psm);
    if (item == NULL || item->conn_count == 0) {
        l2cap_send_connect_rsp_raw(conn->conn_handle,sigid, 0, sig->scid, 
                                  L2CAP_SIG_RESULT_REFUSE_PSM , 0);
        goto exit;
    }

    if (item->conn_count == 0){
        l2cap_send_connect_rsp_raw(conn->conn_handle, sigid, 0, sig->scid,
                                   L2CAP_SIG_RESULT_REFUSE_RESOURCE , 0);
        
        goto exit;

    }

    channel = l2cap_channel_search_dcid(conn, sig->scid);

    if (channel != NULL) {
        l2cap_send_connect_rsp_raw(conn->conn_handle, sigid, 0, sig->scid,
                                    L2CAP_SIG_RESULT_REFUSE_RESOURCE , 0);
        
        goto exit;

    }

    channel = l2cap_channel_add_new(conn, sig->psm);
    
    if (channel == NULL) {
        l2cap_send_connect_rsp_raw(conn->conn_handle, sigid, 0, sig->scid, 
                                   L2CAP_SIG_RESULT_REFUSE_RESOURCE , 0);
        
        goto exit;

    }
    
    channel->sigid_last_recv = sigid;

    channel->dcid = sig->scid;
    channel->l2cap_notify_callback = item->l2cap_notify_callback;
    channel->l2cap_datarecv_callback = item->l2cap_datarecv_callback;

    channel->state = L2CAP_AUTH_PENDING;

    if (FAILURE == l2cap_send_connect_rsp  (channel, 
                                            L2CAP_SIG_RESULT_PENDING, 
                                            L2CAP_SIG_RESULT_PENDING_AUTHOR)) {

        l2cap_channel_close(channel, 0);
        goto exit;
    }
    
    btm_security_askfor_authority(conn->conn_handle, sig->psm);


exit:
    skb_pull(skb, sizeof(*sig));
    
}


uint8 isSecurityBlocked = 0;
void l2cap_get_conn_rsp(struct l2cap_conn *conn, uint8 sigid, struct sk_buff *skb)
{
    struct l2cap_channel *channel;

    struct l2cap_sig_conn_rsp *sig;

    sig = (void *)skb->data;

    channel = l2cap_channel_search_scid(conn, sig->scid);

    if (channel == NULL) {
        goto exit;
    }
    
    if (channel->state != L2CAP_WAIT_CONNECTION_RSP 
        || channel->sigid_last_send != sigid) {
        l2cap_channel_close(channel, 0);
        goto exit;
    }

	
	if (channel->wait_conn_req_timer!=0){
		co_timer_del(channel->wait_conn_req_timer);
		channel->wait_conn_req_timer = 0;
	}

    if (sig->result == L2CAP_SIG_RESULT_PENDING) {
        goto exit;
    }


    if (sig->result != L2CAP_SIG_RESULT_SUCCESS) {
    	if (sig->result & L2CAP_SIG_RESULT_REFUSE_SECURITY)	/* L2CAP_SIG_RESULT_REFUSE_SECURITY */
			isSecurityBlocked = 1;
        l2cap_channel_close(channel, 0);
        goto exit;
    }

    /*success*/
    channel->dcid = sig->dcid;
    channel->state = L2CAP_WAIT_CONFIG_REQ_RSP;

    l2cap_send_cfg_req(channel);
    
exit:
    skb_pull(skb, sizeof(*sig));

}



void l2cap_get_cfg_req(struct l2cap_conn *conn, uint8 sigid, uint16 siglen, struct sk_buff *skb)
{

    struct l2cap_channel *channel;

    struct l2cap_sig_cfg_req *sig;

    sig = (void *)skb->data;

    channel = l2cap_channel_search_scid(conn, sig->dcid);

    if (channel == NULL) {
        l2cap_send_command_rej(conn->conn_handle, sigid, L2CAP_SIG_REASON_INVALID_CID, 0x0000, sig->dcid);
        skb_pull(skb,siglen);
        return;
    }
    
    /* when we need send command reject sig */
    if (channel->state < L2CAP_WAIT_CONFIG
        || channel->state > L2CAP_OPEN) {
        l2cap_send_cfg_rsp_reject(channel->conn->conn_handle, sigid, channel->dcid);
        skb_pull(skb,siglen);
        return;
    }

    channel->sigid_last_recv = sigid;

    skb_pull(skb, sizeof(*sig));

    if (FAILURE == l2cap_save_cfgin(channel,siglen - sizeof(*sig), skb)) { /*it will pull all the data out*/
        /*get unknown option*/
        return;
    }
    
    if (sig->flags & 0x01) {
        /*continue flag, we do nothing, just wait for next config req*/
        return;
    }

    /*send response*/
    if (l2cap_send_cfg_rsp(channel) == FAILURE) {
        return;
    }

    /* if accept the request */

    if ( channel->state == L2CAP_WAIT_CONFIG || channel->state == L2CAP_OPEN) {
		if (channel->wait_config_req_timer!=0){
			co_timer_del(channel->wait_config_req_timer);
			channel->wait_config_req_timer = 0;
		}
        channel->state = L2CAP_WAIT_CONFIG_RSP;
        /* send config request */
        l2cap_send_cfg_req(channel);
        return;
    }
    else
    if ( channel->state == L2CAP_WAIT_CONFIG_REQ_RSP) {
        channel->state = L2CAP_WAIT_CONFIG_RSP;
    }
    else
    if ( channel->state == L2CAP_WAIT_CONFIG_REQ ) {
        channel->state = L2CAP_OPEN;
        /* indicate upper layer */
        if (channel->l2cap_handle == 0) {
            channel->l2cap_handle = l2cap_newhandle();
            channel->l2cap_notify_callback(L2CAP_CHANNEL_NEW_OPENED,channel->l2cap_handle,(void *)&channel->conn->remote, 0);
        }
        else {
            channel->l2cap_notify_callback(L2CAP_CHANNEL_OPENED,channel->l2cap_handle,(void *)&channel->conn->remote, 0);
        }
    }
}

void l2cap_get_cfg_rsp(struct l2cap_conn *conn, uint8 sigid, uint16 siglen, struct sk_buff *skb)
{
    struct l2cap_channel *channel;
    struct l2cap_sig_cfg_rsp *sig;
    struct l2cap_sig_cfg_opt_hdr *cfghdr;
    uint16 cfg_len;
    uint8 *ptr;
    int8 err = 0;

    sig = (void *)skb->data;
    
    channel = l2cap_channel_search_scid(conn, sig->scid);

    if (channel == NULL) {
        goto exit;
    }

    if ((channel->state != L2CAP_WAIT_CONFIG_REQ_RSP
        &&channel->state != L2CAP_WAIT_CONFIG_RSP)
        || channel->sigid_last_send != sigid) {
        goto exit;
    }

    if (sig->result == L2CAP_CFGRSP_REJ
      ||sig->result == L2CAP_CFGRSP_UNKNOWN) {
        l2cap_channel_close(channel,0);
        goto exit;
    }

    /*  parse the unaccept params */
    if (sig->result == L2CAP_CFGRSP_UNACCEPT_PARAMS) {
        cfg_len = siglen - sizeof(*sig);
	skb_pull(skb,sizeof(*sig));
        while(cfg_len !=0) {
            cfghdr = (void *)skb->data;
            ptr = skb_pull(skb,sizeof(*cfghdr));
            skb_pull(skb,cfghdr->len);
            cfg_len -= sizeof(*cfghdr) + cfghdr->len;
            
            switch(cfghdr->type&0x7F) {
                case L2CAP_CFG_TYPE_MTU:
                    if (((struct l2cap_sig_cfg_opt_mtu *)ptr)->mtu<L2CAP_MIN_MTU) {
                        err = 1;
                    }
                    else {
                        channel->cfgout.mtu_remote = *(struct l2cap_sig_cfg_opt_mtu *)ptr;
                    }
                break;
                case L2CAP_CFG_TYPEF_FLUSH_TO:
                    if (((struct l2cap_sig_cfg_opt_flushto *)ptr)->flushto != L2CAP_DEFAULT_FLUSH_TO) {
                        err = 1;
                    }
                    else {
                        channel->cfgout.flushto_local.flushto = L2CAP_DEFAULT_FLUSH_TO;
                    }
                break;
                case L2CAP_CFG_TYPE_QOS:
                    if (((struct l2cap_sig_cfg_opt_qos *)ptr)->service_type == L2CAP_QOS_GUARANTEED) {
                        err = 1;
                    }
                    else {
                        channel->cfgout.qos_local =  *(struct l2cap_sig_cfg_opt_qos *)ptr;
                    }
                break;
                case L2CAP_CFG_TYPE_RFC:
                    if (((struct l2cap_sig_cfg_opt_rfc *)ptr)->mode != L2CAP_MODE_BASE) {
                        err = 1;
                    }
                    else {
                        channel->cfgout.rfc_remote =  *(struct l2cap_sig_cfg_opt_rfc *)ptr;
                    }
                break;
                default:
                break;
            }
            
            if (err == 1) {
                skb_pull(skb,cfg_len);
                l2cap_channel_close(channel, 0);
                return;
            }
        }

        if (sig->flags & 0x01) {
            /*continue flag, we do nothing, just wait for next config req*/
            return;
        }

        l2cap_send_cfg_req(channel);
        return;
    }



    /* success */    
    if (sig->flags & 0x01) {
        /*continue flag, we do nothing, just wait for next config req*/
        skb_pull(skb,siglen);
        return;
    }


    /* if success in cfg response */
    if ( channel->state == L2CAP_WAIT_CONFIG_REQ_RSP ) {
       channel->state = L2CAP_WAIT_CONFIG_REQ;
    } 
    else
    if ( channel->state == L2CAP_WAIT_CONFIG_RSP ) {
        channel->state = L2CAP_OPEN;
        
        /* indicate upper layer */
        if (channel->l2cap_handle == 0) {
            channel->l2cap_handle = l2cap_newhandle();
            channel->l2cap_notify_callback(L2CAP_CHANNEL_NEW_OPENED,channel->l2cap_handle,(void *)&channel->conn->remote, 0);
        }
        else {
            channel->l2cap_notify_callback(L2CAP_CHANNEL_OPENED,channel->l2cap_handle,(void *)&channel->conn->remote, 0);
        }
    } 

exit:
    skb_pull(skb,siglen);

}



void l2cap_get_disconn_req(struct l2cap_conn *conn, uint8 sigid, struct sk_buff *skb)
{
    struct l2cap_channel *channel;
    struct l2cap_sig_disconn_req *sig;

    sig = (void *)skb->data;

    channel = l2cap_channel_search_scid(conn, sig->dcid);

    if ((channel == NULL)||(channel->state==L2CAP_WAIT_DISCONNECT)) {
        l2cap_send_disconnect_rsp_raw(conn->conn_handle,sigid,sig->dcid,sig->scid);
        goto exit;
    }
    

    channel->sigid_last_recv = sigid;
    channel->state = L2CAP_CLOSE;
    l2cap_send_disconnect_rsp(channel);

    l2cap_channel_close(channel, 0);

exit:
    skb_pull(skb, sizeof(*sig));
  
}


void l2cap_get_disconn_rsp(struct l2cap_conn *conn, uint8 sigid, struct sk_buff *skb)
{
    struct l2cap_channel *channel;
    struct l2cap_sig_disconn_rsp *sig;

    sig = (void *)skb->data;

    channel = l2cap_channel_search_scid(conn, sig->scid);

    if (channel == NULL) {
        goto exit;
    }
    
    if (channel->state != L2CAP_WAIT_DISCONNECT
        || channel->sigid_last_send != sigid) {
        goto exit;
    }

    channel->state = L2CAP_CLOSE;

    l2cap_channel_close(channel, channel->disconnect_req_reason);

exit:
    skb_pull(skb, sizeof(*sig));

}

void l2cap_get_info_rsp(struct l2cap_conn *conn, uint16 siglen, struct sk_buff *skb)
{
    struct l2cap_sig_info_rsp *sig;
    sig = (void *)skb->data;

    skb_pull(skb, siglen);
}


void l2cap_handle_signal ( struct l2cap_conn *conn, struct sk_buff *skb)
{

    struct l2cap_sig_hdr *sighdr;

    
    while(skb->data != skb->tail) {
    
        sighdr = (void *)skb->data;
        skb_pull(skb, sizeof(*sighdr));

        switch (sighdr->code) {
            case  L2CAP_SIG_REJ :
                l2cap_get_sig_rej(conn, sighdr->id, sighdr->len, skb);
            break;

            case  L2CAP_SIG_CONN_REQ :
                l2cap_get_conn_req(conn,sighdr->id,skb);
            break;

            case  L2CAP_SIG_CONN_RSP :
                l2cap_get_conn_rsp(conn,sighdr->id,skb);
            break;

            case  L2CAP_SIG_CFG_REQ :
                l2cap_get_cfg_req(conn, sighdr->id, sighdr->len, skb);
            break;

            case  L2CAP_SIG_CFG_RSP :
                l2cap_get_cfg_rsp(conn, sighdr->id, sighdr->len, skb);
            break;

            case  L2CAP_SIG_DISCONN_REQ :
                l2cap_get_disconn_req(conn, sighdr->id, skb);
            break;

            case  L2CAP_SIG_DISCONN_RSP :
                l2cap_get_disconn_rsp(conn, sighdr->id, skb);
            break;

            case  L2CAP_SIG_ECHO_REQ :
                l2cap_send_echo_rsp(conn->conn_handle, sighdr->id);
            break;

            case  L2CAP_SIG_ECHO_RSP :
                /*nothing*/
            break;

            case  L2CAP_SIG_INFO_REQ :
                l2cap_send_info_rsp(conn->conn_handle, sighdr->id, 
                                                    ((struct l2cap_sig_info_req *)skb->data)->infotype);

                skb_pull(skb, sighdr->len);
            break;

            case  L2CAP_SIG_INFO_RSP :
                l2cap_get_info_rsp(conn, sighdr->len, skb);
            break;

            default :
                l2cap_send_command_rej(conn->conn_handle, sighdr->id, L2CAP_SIG_REASON_NOT_UNDERSTOOD, 0, 0);
                skb_free(skb);
                return;
            break;

        }
        

    }

    skb_free(skb);
    
}

void l2cap_handle_data (struct l2cap_channel *channel, struct sk_buff *skb)
{


    if ( channel->state != L2CAP_OPEN ) {
        skb_free(skb);
        return;
    }
    if(channel->close_delay_timer!=0){
	    co_timer_del(channel->close_delay_timer);
	    channel->close_delay_timer=0;
    }
    /*base mode*/
    channel->l2cap_datarecv_callback(channel->l2cap_handle, skb);
    
}

void l2cap_receive_auth_success(struct l2cap_channel *channel)
{
	if (channel->state == L2CAP_AUTH_PENDING) {
		if (l2cap_send_connect_rsp(channel, L2CAP_SIG_RESULT_SUCCESS, 0) == FAILURE) {
			l2cap_channel_close(channel, 0x01);
			return;
		}
		channel->state = L2CAP_WAIT_CONFIG;
		channel->wait_config_req_timer=co_timer_new(500,l2cap_wait_cfg_req_timeout,channel,1);
		co_timer_start(channel->wait_config_req_timer);
	} else if(channel->state == L2CAP_WAIT_CONNECTION_RSP){
		if (channel->wait_conn_req_timer!=0){
			co_timer_del(channel->wait_conn_req_timer);
			channel->wait_conn_req_timer = 0;
		}
	}
}

void l2cap_btm_notify_callback(enum btm_l2cap_event_enum event, uint16 conn_handle, void *pdata, uint8 reason)
{
    struct l2cap_channel *channel;
    struct l2cap_channel *temp;
    struct l2cap_conn *conn;


    conn = l2cap_conn_search_conn_handle(conn_handle);


    switch (event) {
        case BTM_EV_CONN_ACL_OPENED:
           
            if (conn == NULL) {
                conn = l2cap_conn_search((struct bdaddr_t *)pdata);
                if (conn == NULL) {
                    /*create new l2cap conn, wait for more data*/
                    conn = l2cap_conn_add_new((struct bdaddr_t *)pdata);
                    if (conn == NULL) {
                        btm_conn_acl_close((struct bdaddr_t *)pdata);
                        return;
                    }
                    conn->conn_handle = conn_handle;
                    return;
                }
            }
            conn->conn_handle = conn_handle;

			channel = &hid_control_l2cap_channel;
            if (channel->state == L2CAP_WAITING) {
                channel->state = L2CAP_WAIT_CONNECTION_RSP;
                /*  send connection request signal */    
                if (l2cap_send_connect_req(channel) == FAILURE) {
                    l2cap_channel_close(channel, reason);
		   			return;
                }
				channel->wait_conn_req_timer =co_timer_new(300,l2cap_wait_conn_req_timeout,channel,1);
				co_timer_start(channel->wait_conn_req_timer);
            }
            
        break;
        
        case BTM_EV_CONN_ACL_CLOSED:
            if (conn == NULL) {
                conn = l2cap_conn_search((struct bdaddr_t *)pdata); /*in case this is a connecting timeout event*/
                if (conn == NULL) {
                    return;
                }
            }
            l2cap_conn_close(conn, reason);

        break;
        
        case BTM_EV_SECURITY_AUTORITY_SUCCESS:

            if (conn == NULL) {
                return;
            }

			l2cap_receive_auth_success(&sdp_l2cap_channel);
			l2cap_receive_auth_success(&hid_control_l2cap_channel);
			l2cap_receive_auth_success(&hid_interrupt_l2cap_channel);
            
        break;
        
        case BTM_EV_SECURITY_AUTORITY_FAILURE:
            
            if (channel == NULL) {
                return;
            }

			channel = &hid_control_l2cap_channel;
			
			if (channel->state == L2CAP_AUTH_PENDING) {
                l2cap_send_connect_rsp(channel, L2CAP_SIG_RESULT_REFUSE_SECURITY, 0);
                l2cap_channel_close(channel, reason);
            }
            break;            
    }

}

void l2cap_btm_datarecv_callback (uint16 conn_handle, struct sk_buff *skb)
{
    struct l2cap_conn *conn;
    struct l2cap_channel *channel;
    struct l2cap_hdr *hdr;

    conn = l2cap_conn_search_conn_handle(conn_handle);

    if (conn == NULL) {
        return;
    }

    hdr = (void *)skb->data;
    skb_pull(skb,sizeof(*hdr));

    switch (hdr->cid) {
    
        case  L2CAP_SIG_CID:
            l2cap_handle_signal (conn, skb);

        break;

        case  L2CAP_CONNECTIONLESS_CID:
            skb_free(skb);
        break;

        default:
            channel = l2cap_channel_search_scid(conn,hdr->cid);
            if (channel == NULL) {
                skb_free(skb);
                return;
            }
 
            l2cap_handle_data (channel, skb);
            
        break;

    }
}


/*api*/


/*-------------------------------------------------------------------------
    Function    :  l2cap_init             xxx
    Parameters:
        N/A
    Return:
        N/A
    Description:
        init l2cap layer, before invoke any other l2cap api
-------------------------------------------------------------------------*/
int8 l2cap_init(void)
{
	if (l2cap_init_flag == 1) return SUCCESS;
		l2cap_init_flag = 1;

    l2cap_connection.conn_handle = 0xFFFF;

	hid_control_l2cap_channel.state = L2CAP_CLOSE;
	hid_interrupt_l2cap_channel.state = L2CAP_CLOSE;
	sdp_l2cap_channel.state = L2CAP_CLOSE;

	return SUCCESS;
}


/*-------------------------------------------------------------------------
    Function    :  l2cap_register             xxx
    Parameters:
        psm:    Protocol/Service Multiplexor need to registered, like PSM_RFCOMM PSM_SDP
        l2cap_conn_count_max:   the max acceptable connection of this psm
        
        l2cap_notify_callback:  register the callback function to be called when the l2cap layer has
                                          information to notify upper layer

                                          callback parameter:
                                          event: event type
                                          l2cap_handle: the l2cap handle which recv the notification
                                          pdata: for event L2CAP_CHANNEL_OPENED and L2CAP_CHANNEL_NEW_OPENED,
                                                    pdata is the pointer to the remote bdaddr. (void *)&bdaddr


        l2cap_datarecv_callback:  the callback function to be called when the l2cap layer has data
                                              recved

                                              callback parameter:
                                              l2cap_handle: the l2cap handle which recv the data
                                              skb:  a sk_buff structure pointer, which contain the recved data.
                                                      data address is pointered in skb->data
                                                      data length is in skb->len

                                              the skb must be free by skb_free(skb)
                                              
        
    Return:
        SUCCESS or FAILURE
    Description:
        register the psm, when a incoming l2cap connection is created, the upper layer will
        be called by l2cap_notify_callback, and the event is L2CAP_CHANNEL_NEW_OPENED
-------------------------------------------------------------------------*/
int8 l2cap_register ( uint16 psm, 
                      int8 l2cap_conn_count_max, 
                      void (*l2cap_notify_callback)(enum l2cap_event_enum event, uint32 l2cap_handle, void *pdata, uint8 reason),
                      void (*l2cap_datarecv_callback)(uint32 l2cap_handle, struct sk_buff *skb)
                      )
{

    struct l2cap_registered_psm_item_t *item;

	item = l2cap_registered_psm_search ( psm );
	if(item == NULL) {
		return FAILURE;
	}

    item->psm = psm;
    item->conn_count = l2cap_conn_count_max;
    item->l2cap_notify_callback = l2cap_notify_callback;
    item->l2cap_datarecv_callback = l2cap_datarecv_callback;
	
    return SUCCESS;
}

/*-------------------------------------------------------------------------
    Function    :  l2cap_unregister             xxx
    Parameters:
        psm:    Protocol/Service Multiplexor need to unregistered, like PSM_RFCOMM PSM_SDP
    Return:
        N/A
    Description:
        unregister the psm
-------------------------------------------------------------------------*/
int8 l2cap_unregister(uint16 psm)
{
    return SUCCESS;
}

/*-------------------------------------------------------------------------
    Function    :  l2cap_open             xxx
    Parameters:

        remote: remote device's bdaddress need to connect to
        
        psm:    Protocol/Service Multiplexor need to connect to, like PSM_RFCOMM PSM_SDP
        
        l2cap_notify_callback:  register the callback function to be called when the l2cap layer has
                                          information to notify upper layer

                                          callback parameter:
                                          event: event type
                                          l2cap_handle: the l2cap handle which recv the notification
                                          pdata: for event L2CAP_CHANNEL_OPENED and L2CAP_CHANNEL_NEW_OPENED,
                                                    pdata is the pointer to the remote bdaddr. (void *)&bdaddr


        l2cap_datarecv_callback:  the callback function to be called when the l2cap layer has data
                                              recved

                                              callback parameter:
                                              l2cap_handle: the l2cap handle which recv the data
                                              skb:  a sk_buff structure pointer, which contain the recved data.
                                                      data address is pointered in skb->data
                                                      data length is in skb->len

                                              the skb must be free by skb_free(skb)
                                              
        
    Return:
        >=1 l2cap handle id 
        0: error
    Description:
        connect to remote, when the l2cap connection is created, the upper layer will
        be called by l2cap_notify_callback, and the event is L2CAP_CHANNEL_OPENED,
        if can't connected, the event is L2CAP_CHANNEL_CLOSED
-------------------------------------------------------------------------*/
uint32 l2cap_open (struct bdaddr_t *remote, 
                    uint16 psm, 
                    void (*l2cap_notify_callback)(enum l2cap_event_enum event, uint32 l2cap_handle, void *pdata, uint8 reason),
                    void (*l2cap_datarecv_callback)(uint32 l2cap_handle, struct sk_buff *skb)
                    )
{

    struct l2cap_channel *channel;
    struct l2cap_conn  *conn;

    conn = l2cap_conn_search(remote);
    if (conn == NULL) {
        conn = l2cap_conn_add_new(remote);
        if (conn == NULL) {
            return 0;
        }
    }
    
    channel = l2cap_channel_add_new (conn, psm);
    if (channel == NULL) {
        return 0;
    }
    channel->psm_remote = psm;
    channel->l2cap_datarecv_callback = l2cap_datarecv_callback;
    channel->l2cap_notify_callback = l2cap_notify_callback;

    if (conn->conn_handle == 0xffff) {
        /*acl connection not exist*/        
        if (btm_conn_acl_req( remote) == FAILURE) {
            l2cap_channel_close(channel, 0);
            return 0;
        }
        channel->state = L2CAP_WAITING;
    }
    else 
    {
        if (l2cap_send_connect_req(channel) == FAILURE) {
            l2cap_channel_close(channel,0);
            return 0;
        }

		channel->wait_conn_req_timer =co_timer_new(300,l2cap_wait_conn_req_timeout,channel,1);
		co_timer_start(channel->wait_conn_req_timer);
		
	    channel->state = L2CAP_WAIT_CONNECTION_RSP;
    }

    channel->l2cap_handle = l2cap_newhandle();

    return channel->l2cap_handle;
}



/*-------------------------------------------------------------------------
    Function    :  l2cap_close             xxx
    Parameters:
        N/A
    Return:
        SUCCESS or FAILURE
    Description:
        close the handle
-------------------------------------------------------------------------*/
int8 l2cap_close (uint32 l2cap_handle)
{
    struct l2cap_channel *channel;

    if (l2cap_handle == 0) {
        return FAILURE;
    }
    channel = l2cap_channel_search_l2caphandle(l2cap_handle);
    if (channel == NULL) {
        return FAILURE;
    }

    return l2cap_channel_close(channel, 0);
}

void l2cap_channel_close_timer(void *arg)
{
    l2cap_channel_close((struct l2cap_channel *)arg, 0);
    
}

/*-------------------------------------------------------------------------
    Function    :  l2cap_data_skb_alloc             xxx
    Parameters:
        datalen: the upper layer data's len wanted to allocate
    Return:
        pointer to the allocated sk_buffer structure
        NULL: if error
    Description:
        if upper layer want to reduce the copying process and improve performance, it can 
        use l2cap_send_data_skb, and allocate the sk_buffer structure using 
        l2cap_data_skb_alloc(datalen)
-------------------------------------------------------------------------*/
struct sk_buff *l2cap_data_skb_alloc(uint32 datalen)
{
    struct sk_buff *skb;

    skb = skb_alloc(L2CAP_SKB_RESERVE+datalen);

    if (skb == NULL) {
        return NULL;
    }

    skb_reserve(skb, L2CAP_SKB_HEAD_RESERVE);
    return skb;
}


/*-------------------------------------------------------------------------
    Function    :  l2cap_send_data_skb             xxx
    Parameters:
        l2cap_handle: handle
        skb: contain the data to send.
    Return:
        SUCCESS or FAILURE
    Description:
        send the data, using sk_buff structure. 
        compared with l2cap_send_data(), this function can reduce the copying process 
        and improve performance.
        the skb must has space  for add l2cap header or tailer, so the upper layer can 
        allocate skb using l2cap_data_skb_alloc(upper_layer_data_len);
        the skb will be free by low layer.
-------------------------------------------------------------------------*/
int8 l2cap_send_data_skb( uint32 l2cap_handle, struct sk_buff *skb)
{
    struct l2cap_channel *channel;

    if (l2cap_handle == 0) {
        return FAILURE;
    }

    if (L2CAP_SKB_HEAD_RESERVE > (skb->data-skb->head)
        ||L2CAP_SKB_TAIL_RESERVE > (skb->end - skb->tail)) {
        return FAILURE;
    }

    
    channel = l2cap_channel_search_l2caphandle(l2cap_handle);
    if (channel == NULL) {
        return FAILURE;
    }
    
    if (channel->state != L2CAP_OPEN) {
        return FAILURE;
    }

    return __l2cap_send_data_skb(channel, skb);
}

/*-------------------------------------------------------------------------
    Function    :  l2cap_send_data             xxx
    Parameters:
        l2cap_handle: handle
        data: pointer to the sended data buffer
        datalen: data's length
    Return:
        SUCCESS or FAILURE
    Description:
        if data pointed to the memory created by malloc(), the data buffer will not be free by the low layer, 
        it must be free by the upper layer using free()
-------------------------------------------------------------------------*/
int8 l2cap_send_data( uint32 l2cap_handle, uint8 *data, uint32 datalen)
{
    struct l2cap_channel *channel;
    struct sk_buff *skb;
    uint8 *ptr;

    if (l2cap_handle == 0) {
        return FAILURE;
    }
    
    channel = l2cap_channel_search_l2caphandle(l2cap_handle);
    if (channel == NULL) {
        return FAILURE;
    }
    
    if (channel->state != L2CAP_OPEN) {
        return FAILURE;
    }


    skb = skb_alloc(L2CAP_SKB_RESERVE+datalen);

    if (skb == NULL) {
        return FAILURE;
    }

    skb_reserve(skb, L2CAP_SKB_HEAD_RESERVE);
    ptr = skb_put(skb, datalen);
    memcpy(ptr,data,datalen);

    return __l2cap_send_data_skb(channel, skb);
}

