#ifndef _BTM_HCI_H
#define _BTM_HCI_H

#include "stm32f10x_type.h"
#include "co_skbuff.h"
#include "btm.h"

#define HCI_HANDLE_MASK		0x0FFF
#define HCI_FLAG_PB_MASK	0x3000

/* ERROR CODES */
#define HCI_EVENT_STATUS_OK  0x00
#define ERR_CODE_PAGE_TIMEOUT	0x04
#define ERR_CODE_CONN_TIMEOUT 0x08
#define ERR_CODE_CONN_REJECT_LIMIT_RESOURCE  0x0D
#define ERR_CODE_REMOTE_USER_TERMINATE  0x13

#define BTM_DISCONN_WAITING_TIME    5 /*seconds, decide the timeout be disconnect an acl link after it is not used by l2cap*/

enum btm_l2cap_event_enum {
    BTM_EV_CONN_ACL_OPENED=1,
    BTM_EV_CONN_ACL_CLOSED,
    BTM_EV_SECURITY_AUTORITY_SUCCESS,
    BTM_EV_SECURITY_AUTORITY_FAILURE
};

/*********************************HCI EVENT DEFINATION BEGIN******************************/
#define HCI_EV_INQUIRY_COMPLETE	0x01
__packed struct hci_ev_inquiry_complete {
    uint8 status;
};

#define HCI_EV_INQUIRY_RESULT	0x02
#define PAGE_SCAN_REPETITION_MODE_R0    0x00
#define PAGE_SCAN_REPETITION_MODE_R1    0x01
#define PAGE_SCAN_REPETITION_MODE_R2    0x02
#define PAGE_SCAN_PERIOD_MODE_P0    0x00
#define PAGE_SCAN_PERIOD_MODE_P1    0x01
#define PAGE_SCAN_PERIOD_MODE_P2    0x02
__packed struct hci_ev_inquiry_result {
    uint8 num_responses;
    struct bdaddr_t bdaddr;
    uint8 page_scan_repetition_mode;
    uint8 reserved1;
    uint8 reserved2;/*must be 0*/
    uint8 class_dev[3];
    uint16 clk_off;
};

#define HCI_EV_CONN_COMPLETE 	0x03
__packed struct hci_ev_conn_complete {
	uint8     status;
	uint16   handle;
	struct bdaddr_t bdaddr;
	uint8     link_type;
	uint8     encr_mode;
};

#define HCI_EV_CONN_REQUEST	0x04
#define HCI_LINK_TYPE_SCO 0x00
#define HCI_LINK_TYPE_ACL 0x01
#define HCI_LINK_TYPE_ESCO 0x02
__packed struct hci_ev_conn_request {
	struct bdaddr_t bdaddr;
	uint8     class_dev[3];
	uint8     link_type;
};

#define HCI_EV_DISCONN_COMPLETE	0x05
__packed struct hci_ev_disconn_complete {
	uint8     status;
	uint16   handle;
	uint8     reason;
};

#define HCI_EV_AUTHENTICATION_COMPLETE	0x06
__packed struct hci_ev_authentication_complete {
	uint8     status;
	uint16   handle;
};

#define HCI_EV_REMOTENAMEREQ_COMPLETE	0x07
#define HCI_REMOTENAME_MAX    248
__packed struct hci_ev_remote_name_req_complete {
	uint8    status;
	struct   bdaddr_t bdaddr;
	uint8    name[HCI_REMOTENAME_MAX];
};

#define HCI_EV_ENCRYPTION_CHANGE	0x08
__packed struct hci_ev_encryption_change {
	uint8    status;
	uint16   conn_handle;
	uint8    encryption_enable;
};

#define HCI_EV_CMD_COMPLETE	0x0e
__packed struct hci_ev_cmd_complete{
	uint8 num_hci_cmd_packets;
	uint16 cmd_opcode;
	uint8 param[1];
};

#define HCI_EV_CMD_STATUS	0x0f
__packed struct hci_ev_cmd_status{
	uint8 status;
	uint8     num_hci_cmd_packets;
	uint16   cmd_opcode;
};

#define HCI_EV_NUM_OF_CMPLT 0x13
__packed struct hci_ev_num_of_complete{
    uint8  num_handle;
    uint16 handle;
    uint16 num_of_comp;
};

#define HCI_EV_MODE_CHANGE	0x14

#define HCI_EV_RETURN_LINKKEYS  0x15
__packed struct hci_ev_return_linkkeys{
    uint8     num_keys;
    struct bdaddr_t bdaddr;
    uint8   link_key[16];
};

#define HCI_EV_PIN_CODE_REQ	0x16
__packed struct hci_ev_pin_code_req {
	struct bdaddr_t bdaddr;
};

#define HCI_EV_LINK_KEY_REQ	0x17
__packed struct hci_ev_link_key_req {
	struct bdaddr_t bdaddr;
};

#define HCI_EV_LINK_KEY_NOTIFY	0x18
__packed struct hci_ev_link_key_notify {
	struct bdaddr_t bdaddr;
	uint8	 link_key[16];
	uint8	 key_type;
};

#define HCI_EV_SYNC_CONN_COMPLETE	0x2c
__packed struct hci_ev_sync_conn_complete {
	uint8     status;
	uint16   handle;
	struct bdaddr_t bdaddr;
	uint8 link_type;
	uint8 tx_interval;
	uint8 retx_window;
	uint16 rx_pkt_length;
	uint16 tx_pkt_length;
	uint8 air_mode;
};

#define HCI_EV_IO_CAPABILITY_REQ	0x31
__packed struct hci_ev_io_capability_req{
	struct bdaddr_t bdaddr;
};

#define HCI_EV_IO_CAPABILITY_RESP  0x32
__packed struct hci_ev_io_capability_resp{
	struct bdaddr_t bdaddr;
	uint8 io_capability;
	uint8 oob_data_present;
	uint8 auth_requirements;
};

#define HCI_USER_CONFIRM_REQ 0x33
__packed struct hci_user_confirm_req{
	struct bdaddr_t bdaddr;
	uint32 numeric_value;
};

#define HCI_SIMPLE_PAIRING_COMP  0x36
__packed struct hci_simple_pairing_comp{
	uint8 status;
	struct bdaddr_t bdaddr;
};
/*********************************HCI EVENT DEFINATION END******************************/

#endif	//_BTM_HCI_H

