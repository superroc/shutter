#include "l2cap_api.h"
#include "hid.h"

struct hid_ctl_t hid_ctl;

void hid_interrupt_l2cap_notify_callback(enum l2cap_event_enum event, uint32 l2cap_handle, void *pdata, uint8 reason);

//len should not bu longer than 8
uint8 hid_send_interrupt_data(uint8 *data, uint8 len)
{
	struct sk_buff *skb;
	struct hidrep_keyb_t *rep;
	
	if(hid_ctl.state != HID_CONNECTED) {
		return FAILURE;
	}

	//skb = skb_alloc(sizeof(struct hidrep_keyb_t));
	skb = l2cap_data_skb_alloc(sizeof(struct hidrep_keyb_t));
	if(skb == NULL) {
		return FAILURE;
	}

	rep = (struct hidrep_keyb_t *)skb_put(skb, sizeof(struct hidrep_keyb_t));

	rep->btcode = 0xA1;
	rep->rep_id = REPORTID_KEYBD;
	rep->modify = 0;
	rep->reserved = 0;
	memset(rep->key, 0, 6);
	rep->key[0] = data[0];
	/*if(len > 8) {
		len = 8;
	}
	memcpy(rep->key, data, len);*/

	return l2cap_send_data_skb(hid_ctl.interrupt_channel, skb);
}

uint8 hid_send_interrupt_data2(uint8 *data, uint8 len)
{
	struct sk_buff *skb;
	struct hidrep_keyb_t *rep;
	
	if(hid_ctl.state != HID_CONNECTED) {
		return FAILURE;
	}

	//skb = skb_alloc(sizeof(struct hidrep_keyb_t));
	skb = l2cap_data_skb_alloc(sizeof(struct hidrep_keyb_t));
	if(skb == NULL) {
		return FAILURE;
	}

	rep = (struct hidrep_keyb_t *)skb_put(skb, sizeof(struct hidrep_keyb_t)-4);

	rep->btcode = 0xA1;
	rep->rep_id = 3;
	rep->modify = 0;
	rep->reserved = data[0];
	memset(rep->key, 0, 6);
	//rep->key[0] = data[0];
	/*if(len > 8) {
		len = 8;
	}
	memcpy(rep->key, data, len);*/

	return l2cap_send_data_skb(hid_ctl.interrupt_channel, skb);
}

void hid_l2cap_data_callback(uint32 l2cap_handle, struct sk_buff *skb)
{
	
}

void hid_control_l2cap_notify_callback(enum l2cap_event_enum event, uint32 l2cap_handle, void *pdata, uint8 reason)
{
	switch(event) {
		case L2CAP_CHANNEL_OPENED:
		case L2CAP_CHANNEL_NEW_OPENED:
			hid_ctl.control_channel = l2cap_handle;
			if(hid_ctl.state == HID_CONNECTING) {
				hid_ctl.interrupt_channel = l2cap_open(pdata, HID_INTERRUPT_PSM, hid_interrupt_l2cap_notify_callback, hid_l2cap_data_callback);
				hid_ctl.state = HID_CONTROL_CONNECTED;
			} else if(hid_ctl.interrupt_channel != 0) {
				hid_ctl.state = HID_CONNECTED;
				//notice upper layer connection is established.
			} else {
				hid_ctl.state = HID_CONTROL_CONNECTED;
			}
			break;
		case L2CAP_CHANNEL_CLOSED:
			hid_ctl.control_channel = 0;
			hid_ctl.state = HID_DISCONNECTING;
			if(hid_ctl.interrupt_channel == 0) {
				hid_ctl.state = HID_STANDBY;
				//notice upper layer connection is closed.
			}
			break;
		default:
			break;
	}
}

void hid_interrupt_l2cap_notify_callback(enum l2cap_event_enum event, uint32 l2cap_handle, void *pdata, uint8 reason)
{
	switch(event) {
		case L2CAP_CHANNEL_OPENED:
		case L2CAP_CHANNEL_NEW_OPENED:
			hid_ctl.interrupt_channel = l2cap_handle;
			hid_ctl.state = HID_INTERRUPT_CONNECTED;
			if(hid_ctl.control_channel != 0) {
				hid_ctl.state = HID_CONNECTED;
				//notice upper layer connection is established.
			}
			break;
		case L2CAP_CHANNEL_CLOSED:
			hid_ctl.interrupt_channel = 0;
			hid_ctl.state = HID_DISCONNECTING;
			if(hid_ctl.control_channel == 0) {
				hid_ctl.state = HID_STANDBY;
				//notice upper layer connection is closed.
			}
			break;
		default:
			break;
	}
}

uint8 hid_open(struct bdaddr_t *remote)
{
	if(hid_ctl.state == HID_CONNECTED)
		return SUCCESS;
	
	if(hid_ctl.state == HID_STANDBY) {
		hid_ctl.control_channel = l2cap_open(remote, HID_CONTROL_PSM, hid_control_l2cap_notify_callback, hid_l2cap_data_callback);
		if(hid_ctl.control_channel != 0) {
			hid_ctl.state = HID_CONNECTING;
			return SUCCESS;
		}
	} else if(hid_ctl.state == HID_CONTROL_CONNECTED) {
		hid_ctl.interrupt_channel = l2cap_open(remote, HID_INTERRUPT_PSM, hid_interrupt_l2cap_notify_callback, hid_l2cap_data_callback);
		if(hid_ctl.interrupt_channel != 0)
			return SUCCESS;
	} else if(hid_ctl.state == HID_CONNECTING) {
		return SUCCESS;
	}

	return FAILURE;
}

void hid_init(void)
{
	memset((void *)&hid_ctl, 0, sizeof(struct hid_ctl_t));

	l2cap_register(HID_CONTROL_PSM, 1,
			hid_control_l2cap_notify_callback, hid_l2cap_data_callback);
	l2cap_register(HID_INTERRUPT_PSM, 1,
			hid_interrupt_l2cap_notify_callback, hid_l2cap_data_callback);

	hid_ctl.state = HID_STANDBY;
}

