#ifndef _BTM_HANDLE_HCIEVENT_H
#define _BTM_HANDLE_HCIEVENT_H

void btm_event_handle(unsigned char *param);
void btm_acl_handle(unsigned short conn_handle, unsigned short data_len, unsigned char *data_p);
void btm_sco_handle(unsigned short conn_handle, unsigned char data_len, unsigned char *data_p);
void btm_acl_handle_nocopy(unsigned short conn_handle, unsigned short data_len, unsigned char *data_p);

#endif
