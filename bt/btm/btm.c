#include "stm32f10x_type.h"
#include "btm.h"
#include "hci.h"
#include "buffer.h"
#include "co_skbuff.h"

struct btm_conn_item_t btm_conn;
struct btm_ctrl_t btm_ctrl;
struct btm_device_mode_t btm_device_mode;


struct hci_acl_packet *hciacl_packet_create (uint16 conhdl, uint16 param_len)
{
	struct hci_acl_packet *head;

	head = (struct hci_acl_packet *)malloc_buffer(sizeof(struct hci_acl_packet));
	if (head == NULL) {
		return NULL;
	}

	head->acl_flag = HCI_ACL_PACKET;
	head->conn_handle = conhdl;
	head->length= param_len;

	return head;
}

int8 btlib_send_acl_data( uint16 conn_handle, uint8 *data_ptr, uint16 data_len )
{
	struct hci_acl_packet *head;

	head = hciacl_packet_create (conn_handle,data_len);
	if (head == NULL) {
		return FAILURE;
	}

	WRITE_INT32(5,  head,  (int)data_ptr);

	hcicmd_msg_send((struct hci_cmd_packet *)head);

	return SUCCESS;
}


int8 __btm_conn_acl_send_skb (struct btm_conn_item_t *conn, struct sk_buff *skb)
{
    uint8 *ptr = NULL;
    int8 err;
    
    ptr = malloc_buffer(skb->len);
    if (ptr == NULL) {
		return FAILURE;
    }
    
    memcpy(ptr,skb->data,skb->len);
    conn->disconn_flag = 0;

    err = btlib_send_acl_data(conn->conn_handle|(ACL_START<<12), ptr, (uint16)skb->len);
	
    skb_free(skb);
    return err;
}

int8 btm_conn_acl_senddata (struct bdaddr_t *remote_bdaddr, uint8 *data_buf_p, uint16 data_buf_len)
{
    struct btm_conn_item_t *conn = &btm_conn;
    struct sk_buff *skb = NULL;
     
    if (bdaddr_equal(remote_bdaddr, &conn->remote) == FALSE) {
        return FAILURE;
    }
	
    if (conn->status == BTM_CONN_ACL_CLOSED) {
        return FAILURE;
    }

    skb = skb_fill(data_buf_p, data_buf_len);
    if (skb == NULL) {
        return FAILURE;
    }
    
    return __btm_conn_acl_send_skb(conn, skb);    

}

int8 btm_conn_acl_send_skb (uint16 conn_handle, struct sk_buff *skb)
{
    struct btm_conn_item_t *conn = &btm_conn;

    if (conn->conn_handle != conn_handle) {
        return FAILURE;
    }
    if (conn->status == BTM_CONN_ACL_CLOSED) {
        return FAILURE;
    }

    return __btm_conn_acl_send_skb(conn, skb);
}

int8 btm_conn_acl_req ( struct bdaddr_t *remote_bdaddr)
{
    struct btm_conn_item_t *conn = &btm_conn;

    if (conn->status == BTM_CONN_ACL_OPENED) {
        return SUCCESS;
    }
    
    conn->remote = *remote_bdaddr;
    conn->positive = 1;

    return btlib_hcicmd_acl_connect(remote_bdaddr, 
                                    0xcc18, 
                                    conn->page_scan_repetition_mode, 
                                    conn->clk_off, 
                                    0x01);
}

int8 btm_device_mode_set(uint8 discoverable, uint8 connectable)
{
    int8 change_flag = 0;
    if( (discoverable != btm_device_mode.discoverable)
        && (discoverable != DEVICE_MODE_NO_CHANGE)) {
        btm_device_mode.discoverable = discoverable;
        change_flag = 1;
    }
    if ((connectable != btm_device_mode.connectable)
        && (connectable != DEVICE_MODE_NO_CHANGE) ){
        btm_device_mode.connectable = connectable;
        change_flag = 1;
    }

    if (change_flag == 1) {
        return btlib_hcicmd_write_scan_enable(btm_device_mode.discoverable + (btm_device_mode.connectable<<1));
    }
    
    return SUCCESS;
}

void btm_init(void)
{
	btm_ctrl.passkey_cb = NULL;
    btm_ctrl.btm_pairing_notify_callback = NULL;
}

