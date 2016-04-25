#include "uart.h"
#include "hci.h"
#include "buffer.h"
#include "stm32f10x_type.h"

#define READ_INT8(offset, p)                              \
  ( *(uint8 *)(((uint8 *)p) + offset) )

#define READ_INT16(offset, p)                             \
  (uint16 )(READ_INT8(offset, p) +                        \
         (READ_INT8(offset+1, p) << 8))

int8 hcicmd_msg_send(struct hci_cmd_packet *param)
{
	uint8 type;
	uint16 len;
	uint8 *data_p;

	type = READ_INT8(0, param);

	if(type == HCI_ACL_PACKET){
		uart_put_data_noint( (uint8 *)param, 5); /* head */
        printf("\r\nTX: ");
        print_data((void*)param, 5);
		len = READ_INT16(3, param);
		data_p =  (uint8 *)READ_INT32(5,param);           
        print_data(data_p, len);
		uart_put_data_noint( data_p, len); /* data */
		free_buffer(data_p);
	} else if (type == HCI_CMD_PACKET){
		len = 4 + READ_INT8(3, param);
		uart_put_data_noint( (uint8 *)param, len); /* head */
	}

	free_buffer((uint8 *)param);
	
	return SUCCESS;
}

struct hci_cmd_packet *hcicmd_packet_create (uint16 opcode, uint8 param_len)
{
	struct hci_cmd_packet *head;
    
	head = (struct hci_cmd_packet *)malloc_buffer(sizeof(struct hci_cmd_packet)-1+param_len);
	if (head == NULL) {
		return NULL;
	}

	head->cmd_flag = HCI_CMD_PACKET;
	head->opcode = opcode;
	head->length= param_len;

	return head;
}

int8 btlib_hcicmd_accep_conn_req(struct bdaddr_t *bdaddr, uint8 role)
{
    struct hci_cmd_packet *head;
    struct hci_cp_accept_conn_req *param;

    head = hcicmd_packet_create (HCI_ACCEPT_CONNECTION_REQ,sizeof(struct hci_cp_accept_conn_req));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_accept_conn_req *)head->data;

    param->bdaddr = *bdaddr;
    param->role = role;

    return hcicmd_msg_send(head);
}

int8 btlib_hcicmd_pincode_reply(struct bdaddr_t *bdaddr, uint8 *pin, int8 pinlen)
{
    struct hci_cmd_packet *head;
    struct hci_cp_pin_code_reply *param;

    head = hcicmd_packet_create (HCI_PIN_CODE_REQ_REPLY,sizeof(struct hci_cp_pin_code_reply));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_pin_code_reply *)head->data;

    param->bdaddr = *bdaddr;
    param->pin_len = pinlen;
    memcpy((void *)param->pin_code, pin, 16);

    return hcicmd_msg_send(head);
}

int8 btlib_hcicmd_linkkey_neg_reply(struct bdaddr_t *bdaddr)
{
	struct hci_cmd_packet *head;
	struct hci_cp_link_key_neg_reply *param;

	head = hcicmd_packet_create (HCI_LINK_KEY_REQ_NEG_REPLY,sizeof(struct hci_cp_link_key_neg_reply));
	if (head == NULL) {
		return FAILURE;
	}
	param = (struct hci_cp_link_key_neg_reply *)head->data;

	param->bdaddr = *bdaddr;

	return hcicmd_msg_send(head);
}

int8 btlib_hcicmd_linkkey_reply(struct bdaddr_t *bdaddr, uint8 *linkkey)
{
	struct hci_cmd_packet *head;
	struct hci_cp_link_key_reply *param;

	head = hcicmd_packet_create (HCI_LINK_KEY_REQ_REPLY,sizeof(struct hci_cp_link_key_reply));
	if (head == NULL) {
		return FAILURE;
	}
	param = (struct hci_cp_link_key_reply *)head->data;

	param->bdaddr = *bdaddr;
	memcpy((void *)param->link_key, linkkey, 16);

	return hcicmd_msg_send(head);
}

int8 btlib_hcicmd_disconnect (uint16 conn_handle, uint8 reason)
{
    struct hci_cmd_packet *head;
    struct hci_cp_disconnect *param;

    head = hcicmd_packet_create (HCI_DISCONNECT,sizeof(struct hci_cp_disconnect));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_disconnect *)head->data;

    param->handle = conn_handle;
    param->reason = reason;

    return hcicmd_msg_send(head);
}

int8 btlib_hcicmd_set_conn_encryption (uint16 conn_handle, uint8 encry_enable)
{
    struct hci_cmd_packet *head;
    struct hci_cp_set_conn_encryption *param;

    head = hcicmd_packet_create (HCI_SET_CONN_ENCRYPTION,sizeof(struct hci_cp_set_conn_encryption));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_set_conn_encryption *)head->data;

    param->conn_handle = conn_handle;
    param->encryption_enable = encry_enable;

    return hcicmd_msg_send(head);
}

int8 btlib_hcicmd_user_confirm_req_reply(struct bdaddr_t *bdaddr)
{
    struct hci_cmd_packet *head;
    struct hci_cp_user_confirm_req_reply *param;

    head = hcicmd_packet_create (HCI_USER_CONFIRM_REQ_REPLY,sizeof(struct hci_cp_user_confirm_req_reply));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_user_confirm_req_reply *)head->data;

    param->bdaddr = *bdaddr;
    return hcicmd_msg_send(head);
}

int8 btlib_hcicmd_set_localbdaddr(uint8 *bdaddr)
{
	struct hci_cmd_packet *head;
    struct hci_cp_set_bdaddr *param;

    head = hcicmd_packet_create (HCI_SET_BD_ADDR,sizeof(struct hci_cp_set_bdaddr));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_set_bdaddr *)head->data;

    param->bdaddr.addr[0] = bdaddr[0];
    param->bdaddr.addr[1] = bdaddr[1];
    param->bdaddr.addr[2] = bdaddr[2];
	param->bdaddr.addr[3] = bdaddr[3];
    param->bdaddr.addr[4] = bdaddr[4];
    param->bdaddr.addr[5] = bdaddr[5];

    return hcicmd_msg_send(head);
}

int8 btlib_hcicmd_write_classofdevice (uint8 *class_dev)
{
    struct hci_cmd_packet *head;
    struct hci_cp_write_class_of_device *param;

    head = hcicmd_packet_create (HCI_WRITE_CLASS_OF_DEVICE,sizeof(struct hci_cp_write_class_of_device));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_write_class_of_device *)head->data;

    param->class_dev[0] = class_dev[0];
    param->class_dev[1] = class_dev[1];
    param->class_dev[2] = class_dev[2];

    return hcicmd_msg_send(head);
}

int8 btlib_hcicmd_write_localname (uint8 *local_name)
{
    struct hci_cmd_packet *head;
    struct hci_cp_write_local_name *param;

    head = hcicmd_packet_create (HCI_WRITE_LOCAL_NAME,sizeof(struct hci_cp_write_local_name));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_write_local_name *)head->data;

    strcpy((void *)param->local_name, (void *)local_name);
    
    return hcicmd_msg_send(head);
}

int8 btlib_hcicmd_write_page_timeout(uint16 page_timeout)
{
	struct hci_cmd_packet *head;
	struct hci_cp_write_page_timeout *param;

	head = hcicmd_packet_create (HCI_WRITE_PAGE_TIMEOUT, sizeof(struct hci_cp_write_page_timeout));
	if (head == NULL) {
		return FAILURE;
	}

	param = (struct hci_cp_write_page_timeout *)head->data;
	param->page_timeout = page_timeout;

	return hcicmd_msg_send(head);
}

int8 btlib_hcicmd_acl_connect(struct bdaddr_t *bdaddr, uint16 pkt_type, uint8 page_scan_repetition_mode, uint16 clk_off, uint8 allow_role_switch)
{

    struct hci_cmd_packet *head;
    struct hci_cp_create_conn *param;

    head = hcicmd_packet_create (HCI_CREATE_CONNECTION,sizeof(struct hci_cp_create_conn));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_create_conn *)head->data;

    param->bdaddr = *bdaddr;
    param->pkt_type = pkt_type;
    param->page_scan_repetition_mode = page_scan_repetition_mode; 
    param->reserved = 0x00;
    param->clk_off = clk_off;
    param->allow_role_switch = allow_role_switch;

    return hcicmd_msg_send(head);

}

int8 btlib_hcicmd_write_scan_enable(int8 scan_enable)
{
    struct hci_cmd_packet *head;
    struct hci_cp_write_scan_enable *param;

    head = hcicmd_packet_create (HCI_WRITE_SCAN_ENABLE,sizeof(struct hci_cp_write_scan_enable));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_write_scan_enable *)head->data;

    param->scan_enable = 0x03 & scan_enable;

    return hcicmd_msg_send(head);
    
}

int8 btlib_hcicmd_write_memory(uint32 addr, uint32 value, uint8 bytelen)
{
    struct hci_cmd_packet *head;
    struct hci_cp_write_memory *param;

    head = hcicmd_packet_create (HCI_WRITE_MEMORY,sizeof(struct hci_cp_write_memory));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_write_memory *)head->data;
    param->address = addr;
    param->value = value;
    param->bytelen = bytelen;

    return hcicmd_msg_send(head);
}

int8 btlib_hcicmd_authentication_req (uint16 conn_handle)
{
    struct hci_cmd_packet *head;
    struct hci_cp_auth_req *param;

    head = hcicmd_packet_create (HCI_AUTH_REQ,sizeof(struct hci_cp_auth_req));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_auth_req *)head->data;

    param->conn_handle = conn_handle;

    return hcicmd_msg_send(head);

}

int8 btlib_hcicmd_write_supervisiontimeout(uint16 conn_handle, uint16 timeout)
{
    struct hci_cmd_packet *head;
    struct hci_cp_write_super_timeout *param;

    head = hcicmd_packet_create (HCI_WRITE_LINK_SUPERV_TIMEOUT, sizeof(struct hci_cp_write_super_timeout));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_write_super_timeout *)head->data;

    param->conn_handle = conn_handle;
    param->timeout = timeout;

    return hcicmd_msg_send(head);
}

int8 btlib_hcicmd_write_page_scan_activity(uint16 interval, uint16 window)
{
    struct hci_cmd_packet *head;
    struct hci_cp_write_pagescan_activity *param;

    head = hcicmd_packet_create (HCI_WRITE_PAGESCAN_ACTIVITY, sizeof(struct hci_cp_write_pagescan_activity));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_write_pagescan_activity *)head->data;

    param->pagescan_interval = interval;
    param->pagescan_window = window;

    return hcicmd_msg_send(head);
}

int8 btlib_hcicmd_write_inquiry_scan_activity(uint16 interval, uint16 window)
{
    struct hci_cmd_packet *head;
    struct hci_cp_write_inquiryscan_activity *param;

    head = hcicmd_packet_create (HCI_WRITE_INQUIRYSCAN_ACTIVITY, sizeof(struct hci_cp_write_inquiryscan_activity));
    if (head == NULL) {
        return FAILURE;
    }
    param = (struct hci_cp_write_inquiryscan_activity *)head->data;

    param->inquiryscan_interval = interval;
    param->inquiryscan_window = window;

    return hcicmd_msg_send(head);
}

