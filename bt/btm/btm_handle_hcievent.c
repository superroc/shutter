#include "btm_handle_hcievent.h"
#include "btm_security.h"
#include "btm_hci.h"
#include "stm32f10x_type.h"
#include "hci.h"
#include "l2cap_api.h"
#include "l2cap.h"
#include "buffer.h"
#include "co_skbuff.h"

extern struct btm_conn_item_t btm_conn;
extern struct btm_ctrl_t btm_ctrl;

void btm_process_cmd_complete_evt(struct hci_evt_packet *pkt)
{
	struct hci_ev_cmd_complete *param;
	param = (struct hci_ev_cmd_complete *)pkt->data;
	
	switch(param->cmd_opcode) {
		case HCI_READ_STORED_LINK_KEY:
			//btm_process_cmd_complete_read_stored_linkkey((uint8 *)pkt->data);
			break;
		case HCI_INQUIRY_CANCEL:
			//btm_process_cmd_complete_inquiry_cancel((uint8 *)param->param);
			break;
		case HCI_REMOTE_NAME_CANCEL:
			//btm_process_cmd_complete_remote_name_cancel((uint8 *)param->param);
			break;
		case HCI_READ_BUFFER_SIZE:
			//btm_process_cmd_complete_read_buffer_size((uint8 *)param->param);
			break;
	}
 }

struct btm_conn_item_t *btm_conn_add ( struct bdaddr_t *bdaddr )
{
	struct btm_conn_item_t *conn = NULL;

	conn = &btm_conn;

	conn->positive = 0;
	conn->skb_recv = NULL;

	conn->page_scan_repetition_mode = PAGE_SCAN_REPETITION_MODE_R2;
	conn->page_scan_period_mode = PAGE_SCAN_PERIOD_MODE_P2;
	conn->clk_off = 0;
	conn->authen_enable_flag = 0;
	conn->encry_enable_flag = 0;
	conn->encry_need_flag = 0;
	conn->disconn_flag = 0;

	//conn->lowpower_count = pskeys.app_para.other_para.app_to_sniff;
	conn->lowpower_flag = 0;
	conn->sniff_count = 0;

	conn->status = BTM_CONN_ACL_CLOSED;
	conn->remote = *bdaddr;

	return conn;
}

extern uint8 isSecurityBlocked;
void btm_conn_acl_close(struct bdaddr_t *bdaddr)
{
	struct btm_conn_item_t *conn;

	if (memcmp((void *)bdaddr, &btm_conn.remote, 6) != 0) {
		return;
	}
	conn = &btm_conn;

	if (conn->status == BTM_CONN_ACL_OPENED) {
 		if (isSecurityBlocked == 1) {
			isSecurityBlocked = 0;
			conn->disconn_count = 15;
		}
		else {
			conn->disconn_count = BTM_DISCONN_WAITING_TIME;
		}
		conn->disconn_flag = 1;
	}
}

void btm_process_conn_complete_evt(struct hci_evt_packet *pkt)
{
	struct hci_ev_conn_complete *param;
	struct btm_conn_item_t *conn;

	param = (struct hci_ev_conn_complete *)pkt->data;

	conn = btm_conn_add(&param->bdaddr);

	/*error to inform upper layer*/
	if (param->status != HCI_EVENT_STATUS_OK) {
		if (param->link_type == HCI_LINK_TYPE_ACL) {
			l2cap_btm_notify_callback(BTM_CONN_ACL_CLOSED, 0xffff, (void *)&conn->remote, param->status);
			btm_conn_acl_close(&param->bdaddr);
			//isPageTimeOut = 1;
		}
		return;
	}

	/*ok to inform upper layer */    
	if (param->link_type == HCI_LINK_TYPE_ACL) {
		conn->conn_handle = param->handle;
		conn->status = BTM_CONN_ACL_OPENED;

        btlib_hcicmd_write_supervisiontimeout(conn->conn_handle, 0x0640);   //set to 1s

		l2cap_btm_notify_callback(BTM_CONN_ACL_OPENED, conn->conn_handle, (void *)&conn->remote, 0);
	}
}

void btm_process_conn_req_evt(struct hci_evt_packet *pkt)
{
	struct hci_ev_conn_request *param;
	struct btm_conn_item_t *conn;

	param = (struct hci_ev_conn_request *)pkt->data;

	conn = btm_conn_add(&param->bdaddr);
	if (conn == NULL) {
		return;
	}
	
	conn->class_dev[0] = param->class_dev[0];
	conn->class_dev[1] = param->class_dev[1];
	conn->class_dev[2] = param->class_dev[2];

	if (param->link_type == HCI_LINK_TYPE_ACL) {
		conn->positive = 0;
		#define ROLE_REMAIN_SLAVE 0x01
		btlib_hcicmd_accep_conn_req(&conn->remote, ROLE_REMAIN_SLAVE);
	}
}

void btm_process_disconn_complete_evt(struct hci_evt_packet *pkt)
{
	struct hci_ev_disconn_complete *param;
	struct btm_conn_item_t *conn;

	param = (struct hci_ev_disconn_complete *)pkt->data;
	if (param->status != 0x0) {
		return;
	}

	conn = &btm_conn;
	if (conn->conn_handle == param->handle) {
		conn->status = BTM_CONN_ACL_CLOSED;
		l2cap_btm_notify_callback(BTM_CONN_ACL_CLOSED, conn->conn_handle, (void *)&conn->remote, param->reason);
		btm_conn_acl_close(&conn->remote);
		return;
	}

	return;
}

void btm_process_pin_code_req_evt(struct hci_evt_packet *pkt)
{
	struct hci_ev_pin_code_req *param;

	param = (struct hci_ev_pin_code_req *)pkt->data;
	btlib_hcicmd_pincode_reply(&param->bdaddr, "0000", 4);
}

void btm_process_link_key_req_evt(struct hci_evt_packet *pkt)
{
    struct hci_ev_link_key_req *param;
    param = (struct hci_ev_link_key_req *)pkt->data;
    btm_security_link_key_req(&param->bdaddr);
}

void btm_process_link_key_notify_evt(struct hci_evt_packet *pkt)
{
	struct hci_ev_link_key_notify *param;

	param = (struct hci_ev_link_key_notify *)pkt->data;

	btm_ctrl.pairing_timeout = 0; //stop timer
	//if(timer_handle_pairing)
		//co_timer_stop(timer_handle_pairing);
	if(btm_ctrl.btm_pairing_notify_callback)
		btm_ctrl.btm_pairing_notify_callback(PAIRING_OK, (void *)&param->bdaddr);

	btlib_hcicmd_write_stored_linkkey(&param->bdaddr,(uint8 *)param->link_key);
}

void btm_process_authentication_complete_evt(struct hci_evt_packet *pkt)
{
	struct hci_ev_authentication_complete *param;

	param = (struct hci_ev_authentication_complete *)pkt->data;

	btm_security_authen_complete(param->status,param->handle);
}

void btm_process_encryption_change_evt(struct hci_evt_packet *pkt)
{
	struct hci_ev_encryption_change *param;

	param = (struct hci_ev_encryption_change *)pkt->data;

	btm_security_encryption_change(param->status,param->conn_handle,param->encryption_enable);
}

void btm_process_return_linkkeys_evt (struct hci_evt_packet *pkt)
{
    struct hci_ev_return_linkkeys *param;
    param = (struct hci_ev_return_linkkeys *)pkt->data;
    if (btm_ctrl.security_waitfor_linkkey_reply == 1
        &&param->num_keys==1
        &&bdaddr_equal(&param->bdaddr,&btm_ctrl.security_waitfor_linkkey_reply_bdaddr)) {
        btm_ctrl.security_waitfor_linkkey_reply = 0;
        btlib_hcicmd_linkkey_reply(&param->bdaddr,(uint8 *)param->link_key);
    }
}

void btm_process_user_confirm_req_evt(struct hci_evt_packet *pkt)
{
	struct hci_user_confirm_req *param;
	param = (struct hci_user_confirm_req *) pkt->data;
	//btm_security_user_confirm_req(&param->bdaddr, param->numeric_value);
	btlib_hcicmd_user_confirm_req_reply(&param->bdaddr);
}

void btm_event_handle(unsigned char *param)
{
	struct hci_evt_packet *pkt = (struct hci_evt_packet *)param;

	switch (pkt->evt_code) {
		case HCI_EV_CMD_COMPLETE:
			btm_process_cmd_complete_evt(pkt);
			break;
		case HCI_EV_CONN_COMPLETE:
			btm_process_conn_complete_evt(pkt);
			break;
		case HCI_EV_CONN_REQUEST:
			btm_process_conn_req_evt(pkt);
			break;
		case HCI_EV_DISCONN_COMPLETE:
			btm_process_disconn_complete_evt(pkt);
			break;
		case HCI_EV_PIN_CODE_REQ:
			btm_process_pin_code_req_evt(pkt);
			break;
		case HCI_EV_LINK_KEY_REQ:
			btm_process_link_key_req_evt(pkt);
			break;
		case HCI_EV_LINK_KEY_NOTIFY:
			btm_process_link_key_notify_evt(pkt);
			break;
		case HCI_EV_AUTHENTICATION_COMPLETE:
			btm_process_authentication_complete_evt(pkt);
			break;
		case HCI_EV_ENCRYPTION_CHANGE:
			btm_process_encryption_change_evt(pkt);
			break;
		case HCI_EV_MODE_CHANGE:
			//btm_process_mode_change_evt(pkt);
			break;
		case HCI_EV_RETURN_LINKKEYS:
			btm_process_return_linkkeys_evt(pkt);
			break;
		case HCI_EV_NUM_OF_CMPLT:
			//btm_process_num_of_complete_evt(pkt);
			break;
		case HCI_EV_CMD_STATUS:
			//btm_process_status_evt(pkt);
			break;
		case HCI_EV_IO_CAPABILITY_REQ:
			//btm_process_io_capability_req_evt(pkt);
			break;
		case HCI_EV_IO_CAPABILITY_RESP:
			//btm_process_io_capability_resp_evt(pkt);
			break;
		case HCI_USER_CONFIRM_REQ:
			btm_process_user_confirm_req_evt(pkt);
			break;
		case HCI_SIMPLE_PAIRING_COMP:
			//btm_process_simple_pairing_comp_evt(pkt);
			break;
		default:
			break;
	}
}

void btm_acl_handle_nocopy(unsigned short conn_handle, unsigned short data_len, unsigned char *data_p)
{
	enum acl_pkt_boundary_enum packet_boundary;
	struct btm_conn_item_t *conn;

	uint16 pkt_len;

    printf("\r\nRX: ");
    print_data(data_p, data_len);

	packet_boundary = (conn_handle&HCI_FLAG_PB_MASK)>>12;

	if ((packet_boundary != ACL_START)
		&& (packet_boundary != ACL_CONTINUE)) {
		free_buffer(data_p);
		data_p = NULL;
		return;
	}

	if ((conn_handle&HCI_HANDLE_MASK) != btm_conn.conn_handle) {
		free_buffer(data_p);
		return;
	}

	conn = &btm_conn;

	if (conn->status == BTM_CONN_ACL_CLOSED) {
		free_buffer(data_p);
		return;
	}

    /* start */
	if( packet_boundary == ACL_START) { 
		if(conn->skb_recv != NULL) {
			skb_free(conn->skb_recv);
			conn->skb_recv = NULL;
		}

		pkt_len = ((struct l2cap_hdr *)data_p)->len+4;
		if (pkt_len == data_len) {
			conn->skb_recv = skb_fill(data_p, data_len);

			if(conn->skb_recv == NULL) {
				//meet error
				return;
			}

			l2cap_btm_datarecv_callback(conn->conn_handle, conn->skb_recv);
			conn->skb_recv = NULL;
			return;
		}

		conn->skb_recv = skb_fill(data_p, pkt_len);

		if (conn->skb_recv == NULL){
			//meet error
			return;
		}

		skb_trim(conn->skb_recv, data_len);

		return;
	}
	
	if(conn->skb_recv==NULL) {
		free_buffer(data_p);
		return;
	}

	skb_putdata(conn->skb_recv,data_p,data_len);
	free_buffer(data_p);
    if ((conn->skb_recv->tail)  == conn->skb_recv->end) {
        l2cap_btm_datarecv_callback(conn->conn_handle, conn->skb_recv);
        conn->skb_recv = NULL;
    }
}

