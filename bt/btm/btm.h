#ifndef _BTM_H
#define _BTM_H

enum conn_acl_stat_enum{
	BTM_CONN_ACL_OPENED=1,
	BTM_CONN_ACL_CLOSED
};

enum acl_pkt_boundary_enum{
	ACL_START = 0x02,
	ACL_CONTINUE = 0x01
};

enum device_mode_dis_enum{
    DEVICE_MODE_DISCOVERABLE = 0x01,
    DEVICE_MODE_NON_DISCOVERABLE = 0x0,
};
enum device_mode_conn_enum{
    DEVICE_MODE_CONNECTABLE = 0x01,
    DEVICE_MODE_NON_CONNECTABLE = 0x0
};

enum device_mode_enum {
    DEVICE_MODE_NO_CHANGE = 0x04
};

struct btm_device_mode_t {
    enum device_mode_dis_enum discoverable;
    enum device_mode_conn_enum connectable;
};

struct btm_conn_item_t {
    struct sk_buff * skb_recv;
    
    struct bdaddr_t remote;
    uint16 conn_handle;

    /*1: positive connet to the remote or 0: negtive be connected*/
    uint8 positive;
    
    /* received in inquiry result */
    uint8 page_scan_repetition_mode;
    uint8 page_scan_period_mode;
    uint8 class_dev[3];
    uint16 clk_off;
    
    uint8 disconn_flag;  /*if this is need to disconn*/
    uint8 disconn_count; /*to undercount to disconn*/

    uint8 lowpower_flag;  /*if is lowpower. decided to buffer tx data*/
    uint8 lowpower_count; /*to count when to enter lowpower*/
    uint8 sniff_count;  /*to count how many sniff req sent*/
    uint8 authen_enable_flag;
    uint8 encry_enable_flag;/*tell if the entryption is enabled in this acl conn*/
    uint8 encry_need_flag;/*tell if the entryption is need*/
	uint8 authen_finish_flag; //xujg add
    enum conn_acl_stat_enum status;  /*BTM_CONN_ACL_OPENED or BTM_CONN_ACL_CLOSED*/
};

enum btm_pairing_event {
	PAIRING_OK, 
	PAIRING_TIMEOUT,
	PAIRING_FAILED,
	UNPAIR_OK
};

struct btm_ctrl_t {
    uint8 pairing_flag;  /*tell whether the device in pairing state, 1:yes, 0:no*/
    uint32 pairing_timeout;
    void (*btm_pairing_notify_callback)(enum btm_pairing_event event,void *pdata);
	void (*passkey_cb)( void *pData );

    uint8 security_waitfor_linkkey_reply;
    struct bdaddr_t security_waitfor_linkkey_reply_bdaddr;    
};

int8 btm_device_mode_set(uint8 discoverable, uint8 connectable);
void btm_init(void);

#endif	//_BTM_H

