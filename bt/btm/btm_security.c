#include "stm32f10x_type.h"
#include "btm_hci.h"
#include "hci.h"
#include "l2cap_api.h"

extern struct btm_ctrl_t btm_ctrl;
extern struct btm_conn_item_t btm_conn;

int8* btlib_read_stored_linkkey(struct bdaddr_t *bdaddr)
{
	return NULL;
}

int8 btlib_hcicmd_write_stored_linkkey(struct bdaddr_t *bdaddr, uint8 *linkkey)
{
	return SUCCESS;
}

void btm_security_link_key_req(struct bdaddr_t *remote)
{
	uint8 valid_key_flag;	
	uint8* linkkey;

	linkkey = btlib_read_stored_linkkey(remote);
 	if(linkkey == NULL) {
		btlib_hcicmd_linkkey_neg_reply(remote);
	} else {
		btlib_hcicmd_linkkey_reply(remote,linkkey);
	}
}

void btm_security_authen_complete(uint8 status, uint16 conn_handle)
{
	struct btm_conn_item_t *conn = &btm_conn;
	enum btm_l2cap_event_enum event;

	if (conn->conn_handle == conn_handle) {
		return;
	}

	if (status != HCI_EVENT_STATUS_OK) {
		event = BTM_EV_SECURITY_AUTORITY_FAILURE;
		if((btm_ctrl.pairing_flag == 1) && (btm_ctrl.btm_pairing_notify_callback))
			btm_ctrl.btm_pairing_notify_callback(PAIRING_FAILED,(void*) &conn->remote);

		btlib_hcicmd_disconnect(conn->conn_handle, ERR_CODE_REMOTE_USER_TERMINATE);
	}
	else {
		conn->authen_enable_flag = 1;
		event = BTM_EV_SECURITY_AUTORITY_SUCCESS;
		if((btm_ctrl.pairing_flag == 1) && (btm_ctrl.btm_pairing_notify_callback))
			btm_ctrl.btm_pairing_notify_callback(PAIRING_OK,(void*) &conn->remote);
	}

	if (conn->encry_need_flag == 1&&status == HCI_EVENT_STATUS_OK) {
		btlib_hcicmd_set_conn_encryption(conn->conn_handle, 1);
	} else {
		l2cap_btm_notify_callback(event, conn->conn_handle, NULL, 0);
	}
}

void btm_security_encryption_change(uint8 status, uint16 conn_handle, uint8 encryption_enable)
{
	struct btm_conn_item_t *conn = &btm_conn;
	enum btm_l2cap_event_enum event;

	if (conn->conn_handle != conn_handle) {
		return;
	}

	if (conn->encry_need_flag==0) {
		return;
	}

	if (status != HCI_EVENT_STATUS_OK) {
		event = BTM_EV_SECURITY_AUTORITY_FAILURE;
		btlib_hcicmd_disconnect(conn->conn_handle, ERR_CODE_REMOTE_USER_TERMINATE);
	} else {
		conn->encry_enable_flag = 1;
		event = BTM_EV_SECURITY_AUTORITY_SUCCESS;
	}

	l2cap_btm_notify_callback(event, conn->conn_handle, NULL, 0);
}


#define PSM_SDP		0x0001
#define PSM_HIDC	0x0011
#define PSM_HIDI	0x0013

int8 btm_security_askfor_authority(uint16 conn_handle, uint16 psm)
{
	struct btm_conn_item_t *conn = &btm_conn;

	if ((psm == PSM_SDP)||(psm == PSM_HIDC)||(psm == PSM_HIDI)) {
		l2cap_btm_notify_callback(BTM_EV_SECURITY_AUTORITY_SUCCESS, conn_handle, NULL, 0);
		return SUCCESS;
	} /*else if((psm == PSM_HIDC)||(psm == PSM_HIDI)) {
		if(conn->conn_handle != conn_handle) {
			return FAILURE;
		}

		conn->encry_need_flag = 0x01;
		return btlib_hcicmd_authentication_req(conn->conn_handle);
	}*/

	return FAILURE;
}

