/*
 ****************************************************************************************
 * Copyright (C) Accel Semiconductors Corp. 2006
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
#ifndef _L2CAP_H
#define _L2CAP_H
#include "stm32f10x_type.h"
#include "co_skbuff.h"
#include "l2cap_api.h"
#include "btm_hci.h"


/* base frame */
#define L2CAP_SIG_CID 0x0001
#define L2CAP_CONNECTIONLESS_CID 0x0002

__packed struct l2cap_hdr {
    uint16 len;
    uint16 cid;
};


/* command code */
#define L2CAP_SIG_REJ 0x01
#define L2CAP_SIG_CONN_REQ 0x02
#define L2CAP_SIG_CONN_RSP 0x03
#define L2CAP_SIG_CFG_REQ 0x04
#define L2CAP_SIG_CFG_RSP 0x05
#define L2CAP_SIG_DISCONN_REQ 0x06
#define L2CAP_SIG_DISCONN_RSP 0x07
#define L2CAP_SIG_ECHO_REQ 0x08
#define L2CAP_SIG_ECHO_RSP 0x09
#define L2CAP_SIG_INFO_REQ 0x0A
#define L2CAP_SIG_INFO_RSP 0x0B
__packed struct l2cap_sig_hdr {
    uint8 code;
    uint8 id;
    uint16 len;
};


#define L2CAP_SIG_REASON_NOT_UNDERSTOOD 0x0
#define L2CAP_SIG_REASON_MTU_EXCEED 0x1
#define L2CAP_SIG_REASON_INVALID_CID 0x2
__packed struct l2cap_sig_rej {

    uint16 reason;
/*data*/
    uint16 scid;            /*the data len is 0 - 4*/
    uint16 dcid;
};



__packed struct l2cap_sig_conn_req {
    uint16 psm;
    uint16 scid;
};

#define L2CAP_SIG_RESULT_SUCCESS 0x0
#define L2CAP_SIG_RESULT_PENDING 0x1
#define L2CAP_SIG_RESULT_REFUSE_PSM 0x2
#define L2CAP_SIG_RESULT_REFUSE_SECURITY 0x3
#define L2CAP_SIG_RESULT_REFUSE_RESOURCE 0x4    

#define L2CAP_SIG_RESULT_PENDING_NOINFO 0x00    
#define L2CAP_SIG_RESULT_PENDING_AUTHEN 0x01
#define L2CAP_SIG_RESULT_PENDING_AUTHOR 0x02
__packed struct l2cap_sig_conn_rsp {
    uint16 dcid;
    uint16 scid;

    uint16 result;
    uint16 status;   /*only defined when result = pending */
};




__packed struct l2cap_sig_cfg_req {
    uint16 dcid;
    uint16 flags;           /* bit0=1:continue  bit0=0:complete  */
};

#define L2CAP_CFGRSP_SUCCESS                        0x0000
#define L2CAP_CFGRSP_UNACCEPT_PARAMS       0x0001
#define L2CAP_CFGRSP_REJ                                 0x0002
#define L2CAP_CFGRSP_UNKNOWN                       0x0003
__packed struct l2cap_sig_cfg_rsp {
    uint16 scid;
    uint16 flags;
    uint16 result;
};
    
#define L2CAP_CFG_TYPE_MTU		0x01
#define L2CAP_CFG_TYPEF_FLUSH_TO	0x02
#define L2CAP_CFG_TYPE_QOS		0x03
#define L2CAP_CFG_TYPE_RFC		0x04
__packed struct l2cap_sig_cfg_opt_hdr{
    uint8 type;
    uint8 len;
};

__packed struct l2cap_sig_cfg_opt_mtu {

    uint16 mtu;
};
__packed struct l2cap_sig_cfg_opt_flushto {
    
    uint16 flushto;
};

#define L2CAP_QOS_NO_TRAFFIC		0x00
#define L2CAP_QOS_BEST_EFFORT		0x01
#define L2CAP_QOS_GUARANTEED		0x02    
__packed struct l2cap_sig_cfg_opt_qos {
   
    uint8	 flags;
    uint8     service_type;
    uint32  token_rate;
    uint32  token_size;
    uint32  bandwidth;
    uint32  latency;
    uint32  delay_variation;    
};

#define L2CAP_MODE_BASE 0
#define L2CAP_MODE_RETRANSMISSION 1
#define L2CAP_MODE_FLOWCONTROL  2   
__packed struct l2cap_sig_cfg_opt_rfc {

    uint8     mode;
    uint8      txwindow;
    uint8      maxtransmit;
    uint16   retransmission_timeout;
    uint16   monitor_timeout;
    uint16   mps;
};
__packed struct l2cap_sig_disconn_req {

    uint16 dcid;
    uint16 scid;
};

__packed struct l2cap_sig_disconn_rsp {
    uint16 dcid;
    uint16 scid;
};



#define L2CAP_INFOTYPE_CONNLESS_MTU         0x01
#define L2CAP_INFOTYPE_EXTENED_FEATURE   0x02
__packed struct l2cap_sig_info_req {
    uint16 infotype;
};

#define L2CAP_INFOTYPE_SUCCESS              0x00
#define L2CAP_INFOTYPE_NOT_SUPPORT      0x01

#define L2CAP_INFOTYPE_SUPPORT_FLOWCONTROL_MASK         0x01
#define L2CAP_INFOTYPE_SUPPORT_RETRANSMISSION_MASK     0x02
#define L2CAP_INFOTYPE_SUPPORT_BIQOS_MASK                       0x04
__packed struct l2cap_sig_info_rsp {
    uint16 infotype;
    uint16 result;
    /*if result == success, data: mtu(2 bytes), feature mask(4 bytes) */
    uint32 mask;
};



#define L2CAP_SIG_CFG_MTU_MASK                  (1<<0)
#define L2CAP_SIG_CFG_FLUSHTO_MASK          (1<<1)
#define L2CAP_SIG_CFG_QOS_MASK                  (1<<2)
#define L2CAP_SIG_CFG_RFC_MASK                   (1<<3)

struct config_in_t {
    uint8  cfgin_flag;
    struct l2cap_sig_cfg_opt_mtu mtu_local;
    struct l2cap_sig_cfg_opt_flushto flushto_remote;
    struct l2cap_sig_cfg_opt_qos qos_remote;
    struct l2cap_sig_cfg_opt_rfc rfc_local;
};

struct config_out_t {
    uint8 cfgout_flag;
    struct l2cap_sig_cfg_opt_mtu mtu_remote;
    struct l2cap_sig_cfg_opt_flushto flushto_local;
    struct l2cap_sig_cfg_opt_qos qos_local;
    struct l2cap_sig_cfg_opt_rfc rfc_remote;
};


struct l2cap_registered_psm_item_t {
    uint16 psm;
    int8 conn_count;    /*how many conn can be created*/
    void (*l2cap_notify_callback)(enum l2cap_event_enum event, uint32 l2cap_handle, void *pdata, uint8 reason);
    void (*l2cap_datarecv_callback)(uint32 l2cap_handle, struct sk_buff *skb);
};


enum l2cap_channel_state_enum {
    L2CAP_CLOSE,                /*baseband connection closed, wait for hci conn openning, and then can send out conn request signal*/
    L2CAP_WAIT_DISCONNECT,
    L2CAP_WAITING,              /* waitf for the baseband connection to send out conn req signal */
    L2CAP_AUTH_PENDING, /* waiting for baseband authentication or encryption */
    L2CAP_WAIT_CONNECTION_RSP,
    L2CAP_WAIT_CONFIG,
    L2CAP_WAIT_CONFIG_REQ_RSP,
    L2CAP_WAIT_CONFIG_RSP, 
    L2CAP_WAIT_CONFIG_REQ,
    L2CAP_OPEN
};


struct l2cap_channel{

    struct l2cap_conn *conn;


    uint32          l2cap_handle;


    uint16 scid;
    uint16 dcid;
    uint16 psm_remote;

    uint8 sigid_last_send;   /*to save our last request signal id*/
    uint8 sigid_last_recv;  /*to save the last remote's request signal id*/

	int disconnect_req_timeout_timer;/*to avoid disconnect req not response,so we need to give a timeout flag*/
	int close_delay_timer;
	int wait_conn_req_timer;
    int wait_config_req_timer;
    uint8 disconnect_req_reason;
    /* for config req and resp */
    struct config_in_t cfgin;
    struct config_out_t cfgout;


    enum l2cap_channel_state_enum state;
    
    void (*l2cap_notify_callback)(enum l2cap_event_enum event, uint32 l2cap_handle, void *pdata, uint8 reason);
    void (*l2cap_datarecv_callback)(uint32 l2cap_handle, struct sk_buff *skb);
    
};

struct l2cap_conn {
    
    struct bdaddr_t remote;
    uint16 conn_handle;
};



/* default value in spec */
#define L2CAP_DEFAULT_MTU 	                          672
#define L2CAP_MIN_MTU		                           48

#define L2CAP_DEFAULT_FLUSH_TO	                   0xFFFF

#define L2CAP_DEFAULT_QOS_SERVICE_TYPE                      L2CAP_QOS_BEST_EFFORT 
#define L2CAP_DEFAULT_QOS_TOKEN_RATE                         0x00000000
#define L2CAP_DEFAULT_QOS_TOKEN_BUCKET_SIZE            0x00000000
#define L2CAP_DEFAULT_QOS_PEEK_BANDWIDTH                0x00000000
#define L2CAP_DEFAULT_QOS_LATENCY                          0xffffffff
#define L2CAP_DEFAULT_QOS_DELAY_VARIATION                        0xffffffff


#define L2CAP_DEFAULT_RFC_MODE                                            L2CAP_MODE_BASE                  
#define L2CAP_DEFAULT_RFC_TXWINDOW                                        32      /*1 to 32*/
#define L2CAP_DEFAULT_RFC_MAXTRANSMIT                                      32
#define L2CAP_DEFAULT_RFC_RETRANSMISSION_TIMEOUT         1000
#define L2CAP_DEFAULT_RFC_MONITOR_TIMEOUT                 1000
#define L2CAP_DEFAULT_RFC_MPS                                          0xFFFF



/* used for sk_buff */
#define L2CAP_SKB_HEAD_RESERVE      4       /*len+cid+control+sdulen  2+2+2+2*/
#define L2CAP_SKB_TAIL_RESERVE	       0        /* fcs 2 */
#define L2CAP_SKB_RESERVE  (L2CAP_SKB_HEAD_RESERVE + L2CAP_SKB_TAIL_RESERVE)


/* config value */

//#define L2CAP_CFG_MTU 	                                                     (339-L2CAP_SKB_RESERVE)   /* BUF_MAX - L2CAP_SKB_RESERVE */
#define L2CAP_CFG_MTU 	                                             335  //enlarge from 335
#define L2CAP_CFG_MTU_INCOMING 	                                     335//1012

#define L2CAP_CFG_FLUSH_TO	                                               L2CAP_DEFAULT_FLUSH_TO

#define L2CAP_CFG_QOS_SERVICE_TYPE                                  L2CAP_DEFAULT_QOS_SERVICE_TYPE 
#define L2CAP_CFG_QOS_TOKEN_RATE                                       0x00000000
#define L2CAP_CFG_QOS_TOKEN_BUCKET_SIZE                        0x00000000
#define L2CAP_CFG_QOS_PEEK_BANDWIDTH                            0x00000000
#define L2CAP_CFG_QOS_LATENCY                                             0xffffffff
#define L2CAP_CFG_QOS_DELAY_VARIATION                             0xffffffff


#define L2CAP_CFG_RFC_MODE                                                     L2CAP_DEFAULT_RFC_MODE                   /*L2CAP_MODE_BASE*/
/* below is only available when rfc mode is not base mode*/
#define L2CAP_CFG_RFC_TXWINDOW                                            32      /*1 to 32*/
#define L2CAP_CFG_RFC_MAXTRANSMIT                                        32
#define L2CAP_CFG_RFC_RETRANSMISSION_TIMEOUT                 1000
#define L2CAP_CFG_RFC_MONITOR_TIMEOUT                               1000
#define L2CAP_CFG_RFC_MPS                                                       0xFFFF


/* L2CAP_RTX: The Responsive Timeout eXpired timer is used to terminate
   the channel when the remote endpoint is unresponsive to signalling
   requests (min 1s, max 60s) */
#define L2CAP_CFG_RTX 60
/* L2CAP_RTX_MAXN: Maximum number of Request retransmissions before
   terminating the channel identified by the request. The decision
   should be based on the flush timeout of the signalling link. If the
   flush timeout is infinite, no retransmissions should be performed */
#define L2CAP_CFG_RTX_MAXN 0
/* L2CAP_ERTX: The Extended Response Timeout eXpired timer is used in
   place of the RTC timer when a L2CAP_ConnectRspPnd event is received
   (min 60s, max 300s) */
#define L2CAP_CFG_ERTX 300



#endif
