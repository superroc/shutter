#ifndef _HID_H
#define _HID_H

#include "stm32f10x_type.h"

#define HID_CONTROL_PSM		0x0011
#define HID_INTERRUPT_PSM	0x0013

#define	REPORTID_KEYBD	1

__packed struct hidrep_keyb_t
{
	uint8 btcode; // Fixed value for "Data Frame": 0xA1
	uint8 rep_id; // Will be set to REPORTID_KEYBD for "keyboard"
	uint8 modify; // Modifier keys (shift, alt, the like)
	uint8 reserved;
	uint8 key[6]; // Currently pressed keys, max 8 at once
} __attribute((packed));

enum hid_state {
	HID_STANDBY = 0,
	HID_CONNECTING,
	HID_DISCONNECTING,
	HID_CONTROL_CONNECTED,
	HID_INTERRUPT_CONNECTED,
	HID_CONNECTED
};

struct hid_ctl_t {
	enum hid_state state;

	uint32 control_channel;
	uint32 interrupt_channel;
};

#endif	//_HID_H

