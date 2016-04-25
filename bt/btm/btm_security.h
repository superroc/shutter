#ifndef _BTM_SECURITY_H
#define _BTM_SECURITY_H

#include "stm32f10x_type.h"

int8* btlib_read_stored_linkkey(struct bdaddr_t *bdaddr);
int8 btlib_hcicmd_write_stored_linkkey(struct bdaddr_t *bdaddr, uint8 *linkkey);
void btm_security_link_key_req(struct bdaddr_t *remote);
void btm_security_authen_complete(uint8 status, uint16 conn_handle);
void btm_security_encryption_change(uint8 status, uint16 conn_handle, uint8 encryption_enable);

#endif	//_BTM_SECURITY_H

