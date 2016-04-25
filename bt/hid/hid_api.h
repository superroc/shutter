#ifndef _HID_API_H
#define _HID_API_H

#include "btlib_type.h"

uint8 hid_open(struct bdaddr_t *remote);
void hid_init(void);
uint8 hid_send_interrupt_data(uint8 *data, uint8 len);
uint8 hid_send_interrupt_data2(uint8 *data, uint8 len);

#endif	//_HID_API_H
