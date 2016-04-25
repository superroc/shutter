#include "sdp_api.h"
#include "sdp.h"
#include "l2cap_api.h"
#include "stm32f10x_type.h"
#include "buffer.h"

#define MAX_SDP_ATTR_LENGTH		150
#define MIN(A,B)  (((A)>(B))?(B):(A))

const uint8 sdp_hid_attr_list[] = {
	0x36, 0x01, 0x65,
	0x36, 0x01, 0x62, 0x09, 0x00, 0x00, 0x0a, 0x00, 0x10, 0x00, 0x0b, 0x09, 
	0x00, 0x01, 0x35, 0x03, 0x19, 0x11, 0x24, 0x09, 0x00, 0x04, 0x35, 0x0d, 
	0x35, 0x06, 0x19, 0x01, 0x00, 0x09, 0x00, 0x11, 0x35, 0x03, 0x19, 0x00, 
	0x11, 0x09, 0x00, 0x06, 0x35, 0x09, 0x09, 0x65, 0x6e, 0x09, 0x00, 0x6a, 
	0x09, 0x01, 0x00, 0x09, 0x00, 0x0d, 0x35, 0x0f, 0x35, 0x0d, 0x35, 0x06, 
	0x19, 0x01, 0x00, 0x09, 0x00, 0x13, 0x35, 0x03, 0x19, 0x00, 0x11, 0x09, 
	0x00, 0x09, 0x35, 0x08, 0x35, 0x06, 0x19, 0x11, 0x24, 0x09, 0x01, 0x00, 
	0x09, 0x02, 0x01, 0x09, 0x01, 0x00, 0x09, 0x02, 0x02, 0x08, 0x40, 0x09, 
	0x02, 0x03, 0x08, 0x21, 0x09, 0x02, 0x04, 0x28, 0x01, 0x09, 0x02, 0x05, 
	0x28, 0x01, 0x09, 0x02, 0x06, 0x35, 0xcc, 0x35, 0xca, 0x08, 0x22, 0x25, 
	0xc6, 0x05, 0x01, 0x09, 0x06, 0xa1, 0x01, 0x85, 0x01, 0x05, 0x07, 0x19, 
	0xe0, 0x29, 0xe7, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x08, 0x81, 
	0x02, 0x95, 0x01, 0x75, 0x08, 0x81, 0x03, 0x95, 0x05, 0x75, 0x01, 0x05, 
	0x08, 0x19, 0x01, 0x29, 0x05, 0x91, 0x02, 0x95, 0x01, 0x75, 0x03, 0x91, 
	0x03, 0x95, 0x06, 0x75, 0x08, 0x15, 0x00, 0x26, 0xff, 0x00, 0x05, 0x07, 
	0x19, 0x00, 0x29, 0xff, 0x81, 0x00, 0xc0, 0x05, 0x0c, 0x09, 0x01, 0xa1, 
	0x01, 0x85, 0x03, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x1e, 0x0a, 
	0x24, 0x02, 0x0a, 0x25, 0x02, 0x0a, 0x26, 0x02, 0x0a, 0x27, 0x02, 0x0a, 
	0x21, 0x02, 0x0a, 0x2a, 0x02, 0x0a, 0x23, 0x02, 0x0a, 0x8a, 0x01, 0x09, 
	0xe2, 0x09, 0xea, 0x09, 0xe9, 0x09, 0xcd, 0x09, 0xb7, 0x09, 0xb6, 0x09, 
	0xb5, 0x0a, 0x83, 0x01, 0x0a, 0x94, 0x01, 0x0a, 0x92, 0x01, 0x0a, 0x09, 
	0x02, 0x09, 0xb2, 0x09, 0xb3, 0x09, 0xb4, 0x09, 0x8d, 0x09, 0x04, 0x09, 
	0x30, 0x0a, 0x07, 0x03, 0x0a, 0x0a, 0x03, 0x0a, 0x0b, 0x03, 0x0a, 0xb1, 
	0x01, 0x09, 0xb8, 0x81, 0x02, 0x95, 0x01, 0x75, 0x02, 0x81, 0x03, 0xc0, 
	0x05, 0x01, 0x09, 0x80, 0xa1, 0x01, 0x85, 0x04, 0x05, 0x01, 0x19, 0x81, 
	0x29, 0x83, 0x15, 0x00, 0x25, 0x01, 0x95, 0x03, 0x75, 0x01, 0x81, 0x06, 
	0x95, 0x01, 0x75, 0x05, 0x81, 0x01, 0xc0, 0x09, 0x02, 0x07, 0x35, 0x08, 
	0x35, 0x06, 0x09, 0x04, 0x09, 0x09, 0x01, 0x00, 0x09, 0x02, 0x08, 0x28, 
	0x00, 0x09, 0x02, 0x09, 0x28, 0x01, 0x09, 0x02, 0x0a, 0x28, 0x00, 0x09, 
	0x02, 0x0d, 0x28, 0x01, 0x09, 0x02, 0x0e, 0x28, 0x00
};

const uint8 sdp_hid_attr_list_empty[] = {0x36, 0x00, 0x00};


uint16   sdp_rsp_left_len = 0;  
uint16   sdp_total_rsp_len;
uint8    *sdp_rsp_buf;

sdp_record_t hid_sdp_record;
sdp_socket_t sdp_sock_global;

static sdp_status_t sdp_make_service_search_attr_rsp(service_search_attr_struct_t *db_handle);

int parse_err;
int get_err(void)
{
  int tmp = parse_err;

  parse_err = 0;
  return tmp;
}

void set_err(int err)
{
  parse_err = err;
}

static void set_sdp_hdr(uint8 *hdr, uint8 pkt_type,
                 unsigned short trans_id, unsigned short len)
{
	hdr[SDP_HDR_TYPE]        = pkt_type;
	hdr[SDP_HDR_TRANS_ID_MS] = SHORT2CHAR_MS(trans_id);
	hdr[SDP_HDR_TRANS_ID_LS] = SHORT2CHAR_LS(trans_id);
	hdr[SDP_HDR_LENGTH_MS]   = SHORT2CHAR_MS(len);
	hdr[SDP_HDR_LENGTH_LS]   = SHORT2CHAR_LS(len);
}

sdp_status_t sdp_send(sdp_socket_t *sdp_sock, void *buf, int len)
{
	if(l2cap_send_data(sdp_sock->sock, buf, len) != SUCCESS ) {
		return SDP_ERROR;
	}
	
	return SDP_OK;
}

static void sdp_send_curr_rsp(service_attr_struct_t *db_hdl, uint8 rsp_type )
{
	uint8 *curr_rsp_pkt;  
	int  curr_rsp_len;
	uint8 rsp_continue = 0,con_field_len;
	uint8 attr_count_len = 2;//attr count occupies 2 bytes in rsp pdu  ;  
	uint16 len_before_attr = SDP_HDR_SIZE+attr_count_len;
	uint16 rsp_len_sent;

	if(rsp_type == SDP_SERVICEATTR_RSP)
		curr_rsp_len = MIN(SERVICE_ATTR_RSP_BUF_SIZE-len_before_attr-3, db_hdl->max_attr_byte_cnt);  
	else if(rsp_type == SDP_SERVICESEARCHATTR_RSP)
		curr_rsp_len = MIN(SERVICE_SEARCH_ATTR_RSP_BUF_SIZE-len_before_attr-3, db_hdl->max_attr_byte_cnt);  
	else{
		return;
	}    

	rsp_len_sent = sdp_total_rsp_len - sdp_rsp_left_len;	

	if(sdp_rsp_left_len <= curr_rsp_len){		
		curr_rsp_len = sdp_rsp_left_len;
		rsp_continue = 0;
		con_field_len = 1;
	} else {
		rsp_continue = 1;
		con_field_len = 3;
	}

	curr_rsp_pkt =  (uint8*)malloc_buffer(curr_rsp_len+len_before_attr+con_field_len); 
	if(!curr_rsp_pkt){
		return;
	}

	memcpy(curr_rsp_pkt+len_before_attr, sdp_rsp_buf+rsp_len_sent, curr_rsp_len);

	sdp_rsp_left_len -=  curr_rsp_len;

	curr_rsp_len += len_before_attr;
	if(rsp_continue == 0){
		curr_rsp_pkt[curr_rsp_len++] = 0;	
	} else {
		curr_rsp_pkt[curr_rsp_len++] = 2;		
		curr_rsp_pkt[curr_rsp_len++] = sdp_rsp_left_len>>8;
		curr_rsp_pkt[curr_rsp_len++] = sdp_rsp_left_len&0xff;
	}                     

	curr_rsp_pkt[SDP_HDR_SIZE]   = SHORT2CHAR_MS(curr_rsp_len - len_before_attr- con_field_len);
	curr_rsp_pkt[SDP_HDR_SIZE+1] = SHORT2CHAR_LS(curr_rsp_len - len_before_attr- con_field_len);

	set_sdp_hdr(curr_rsp_pkt, rsp_type, db_hdl->sdp_sock->tid,
					curr_rsp_len - SDP_HDR_SIZE);

	sdp_send(db_hdl->sdp_sock, curr_rsp_pkt, curr_rsp_len); 
	SDP_FREE(curr_rsp_pkt); 
}

static void send_error_rsp(sdp_socket_t *sdp_sock, unsigned short err_code)
{
	uint8 sdp_data[7];
	unsigned short pdu_len;

	/* Since we do not send any error information, the pdu length is just the
	size of the error code length, which is two bytes */

	if(!sdp_sock){
		return;
	}
	pdu_len = 2;  
	sdp_data[0] = SDP_ERROR_RSP;
	sdp_data[1] = (sdp_sock->tid >> 8) & 0xff;
	sdp_data[2] = sdp_sock->tid & 0xff;
	sdp_data[3] = (pdu_len >> 8) & 0xff;
	sdp_data[4] = pdu_len & 0xff;

	sdp_data[5] = (err_code >> 8) & 0xff;
	sdp_data[6] = err_code & 0xff;

	sdp_send(sdp_sock, sdp_data, 7);
}

#if 0
static void sdp_hidRecordInit(sdp_record_t* record)
{
	record->handle = HID_RECORD_HANDLE;

	record->attr_id_num = 0;

	record->attr_recordHandle.attrId = SDP_ATTR_RECORD_HANDLE;
	record->attr_recordHandle.record_handle = HID_RECORD_HANDLE;
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_RECORD_HANDLE;

	record->attr_serviceClassIdList.attrId = SDP_ATTR_SVCLASS_ID_LIST;
	record->attr_serviceClassIdList.uuid_list.num = 1;
	record->attr_serviceClassIdList.uuid_list.uuids[0] = HID_SVCLASS_ID;//uuid
	record->uuid_list.uuids[record->uuid_list.num++] = HID_SVCLASS_ID;
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_SVCLASS_ID_LIST;

	record->attr_protocolDesciptorList.attrId = SDP_ATTR_PROTO_DESC_LIST;
	record->attr_protocolDesciptorList.protocol_num = 2;
	record->attr_protocolDesciptorList.protocols[0].uuid = L2CAP_UUID;  //uuid
	record->attr_protocolDesciptorList.protocols[0].para = PSM_HIDC;
	record->attr_protocolDesciptorList.protocols[1].uuid = HIDP_UUID;  //uuid
	record->uuid_list.uuids[record->uuid_list.num++] = L2CAP_UUID;
	record->uuid_list.uuids[record->uuid_list.num++] = HIDP_UUID;   
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_PROTO_DESC_LIST;

	record->attr_lang.attrId = SDP_ATTR_LANG_BASE_ATTR_ID_LIST;
	record->attr_lang.langId = 0x656e;
	record->attr_lang.encoding = 0x006a;
	record->attr_lang.langBaseId = 0x0100;
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_LANG_BASE_ATTR_ID_LIST;

	record->attr_protocolDesciptorList2.attrId= SDP_ATTR_ADD_PROTO_DESC_LIST;
	record->attr_protocolDesciptorList2.protocol_num = 2;
	record->attr_protocolDesciptorList2.protocols[0].uuid = L2CAP_UUID;	//uuid
	record->attr_protocolDesciptorList2.protocols[0].para = PSM_HIDI;
	record->attr_protocolDesciptorList2.protocols[1].uuid = HIDP_UUID;  //uuid
	record->uuid_list.uuids[record->uuid_list.num++] = L2CAP_UUID;
	record->uuid_list.uuids[record->uuid_list.num++] = HIDP_UUID;	
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_ADD_PROTO_DESC_LIST;

	record->attr_bluetoothProfile.attrId= SDP_ATTR_PFILE_DESC_LIST;
	record->attr_bluetoothProfile.profile.profileId = HID_SVCLASS_ID;
	record->attr_bluetoothProfile.profile.version = 0x0100;
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_PFILE_DESC_LIST;

	record->attr_parseVersion.attrId = SDP_ATTR_HID_PARSER_VERSION;
	record->attr_parseVersion.value = 0x0100;
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_HID_PARSER_VERSION;

	record->attr_deviceSubclass.attrId = SDP_ATTR_HID_DEVICE_SUBCLASS;
	record->attr_deviceSubclass.value = 0x05;
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_HID_DEVICE_SUBCLASS;

	record->attr_countryCode.attrId = SDP_ATTR_HID_COUNTRY_CODE;
	record->attr_countryCode.value = 0x21;
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_HID_COUNTRY_CODE;

	record->attr_virtualCable.attrId = SDP_ATTR_HID_VIRTUAL_CABLE;
	record->attr_virtualCable.value = 0x01;
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_HID_VIRTUAL_CABLE;

	record->attr_reconnInitiate.attrId = SDP_ATTR_HID_RECONNECT_INITIATE;
	record->attr_reconnInitiate.value = 0x01;
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_HID_RECONNECT_INITIATE;

	//HID Descriptor List
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_HID_DESCRIPTOR_LIST;

	record->attr_langBase.attrId = SDP_ATTR_HID_LANG_ID_BASE_LIST;
	record->attr_langBase.langId = 0x0409;
	record->attr_langBase.langBaseId = 0x0100;
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_HID_LANG_ID_BASE_LIST;

	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_HID_SDP_DISABLE;
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_HID_BATTERY_POWER;
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_HID_REMOTE_WAKEUP;

	record->attr_normConnectable.attrId = SDP_ATTR_HID_NORMALLY_CONNECTABLE;
	record->attr_normConnectable.value = 0x01;
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_HID_NORMALLY_CONNECTABLE;

	record->attr_bootDevice.attrId = SDP_ATTR_HID_BOOT_DEVICE;
	record->attr_bootDevice.value = 0x00;
	record->attr_id_list[record->attr_id_num++] = SDP_ATTR_HID_BOOT_DEVICE;
}
#endif
int sdp_getSize(uint8 *data, int *new_pos)
{
	int size_index;

	size_index = GET_SIZE(*data);
	switch (size_index) {
		case 0: 
			*new_pos = 1;
			return 1;
		case 1:
			*new_pos = 1;
			return 2;
		case 2:
			*new_pos = 1;
			return 4;
		case 3:
			*new_pos = 1;
			return 8;
		case 4:
			*new_pos = 1;
			return 16;
		case 5:
			*new_pos = 2;
			return data[1];
		case 6:
			*new_pos = 3;
			return CHAR2INT16(data[1], data[2]);
		case 7:
			*new_pos = 5;
			return CHAR2INT32(data[1], data[2], data[3], data[4]);
	}
	return 0;
}

#if 0
static int get_rec_hdl(uuid16_t* uuid_list, uint32 uuid_num, uint32 *rec_hdl_list)
{
	int count = 0;
	uint8 i, j, found, total_num = hid_sdp_record.uuid_list.num;
	uint16 *total_uuids = hid_sdp_record.uuid_list.uuids;

	if(!rec_hdl_list){
		set_err(SDP_INSUFFICIENT_RESOURCES);
		return 0;
	}

	found = 0;
	if(uuid_num <= total_num) {
		for(i=0; i<uuid_num; i++){
			for(j=0; j<total_num;j++){
				if(total_uuids[j] == uuid_list[i]){
					found =1;
					break;
				}
			}
			if(!found){// if one of the uuid in request uuid list not found in the record, return.
				return 0;
			}
			found = 0;
		}
		count = 1;
		rec_hdl_list[0] = HID_RECORD_HANDLE;
	}
	
	return count;
}

#if 0
static void handle_service_search_req(service_search_struct_t* db_hdl)
{
	uint8 *rsp_pkt = NULL;
	int rsp_pkt_len;
	int rec_hdl_cnt = 0;
	uint32 rec_hdl_list[1];
	uint32 curr_rec_hdl_cnt=0;
	uint32 continue_hanlde_index=0;
	uint32 count_left=0;
	uint32 max_rec_cnt;
	uint8 con_flag = 0;
	int i; 

	if(!db_hdl){
		return;
	}

	rsp_pkt =  (uint8*)malloc_buffer(SERVICE_ATTR_RSP_BUF_SIZE); 
	if(!rsp_pkt){    
		goto end;
	}

	rec_hdl_cnt = get_rec_hdl(db_hdl->service_class_list, db_hdl->service_class_cnt, rec_hdl_list);

	rsp_pkt_len = SDP_HDR_SIZE;  

	/* Set the total service record count */
	rsp_pkt[rsp_pkt_len++] = SHORT2CHAR_MS(rec_hdl_cnt);
	rsp_pkt[rsp_pkt_len++] = SHORT2CHAR_LS(rec_hdl_cnt);

	max_rec_cnt = db_hdl->max_rec_cnt;
	if(db_hdl->con_state.len){
		continue_hanlde_index = db_hdl->con_state.data[0];
	}
	count_left = rec_hdl_cnt -continue_hanlde_index;   
	if (count_left > max_rec_cnt) //should set the continue state
	{
		curr_rec_hdl_cnt = max_rec_cnt;
		con_flag = 1;
	} else {
		curr_rec_hdl_cnt = count_left;
	}

	/* Set the current service record count */
	rsp_pkt[rsp_pkt_len++] = SHORT2CHAR_MS(curr_rec_hdl_cnt);
	rsp_pkt[rsp_pkt_len++] = SHORT2CHAR_LS(curr_rec_hdl_cnt);

	for (i = continue_hanlde_index; i < continue_hanlde_index+curr_rec_hdl_cnt; i++)//32bit record handle
	{
		rsp_pkt[rsp_pkt_len++] = (rec_hdl_list[i] >> 24) & 0xff;
		rsp_pkt[rsp_pkt_len++] = (rec_hdl_list[i] >> 16) & 0xff;
		rsp_pkt[rsp_pkt_len++] = (rec_hdl_list[i] >> 8) & 0xff;
		rsp_pkt[rsp_pkt_len++] = rec_hdl_list[i] & 0xff;
	}  

	if(con_flag){
		rsp_pkt[rsp_pkt_len++]=1;// 1 byte continue infor indicate numbers of handles have responsed to client.
		rsp_pkt[rsp_pkt_len++] =  i&0xff;
	} else {
		rsp_pkt[rsp_pkt_len++]=0;// the final length should be rsp_pkt_len++
	}

	set_sdp_hdr(rsp_pkt, SDP_SERVICESEARCH_RSP, db_hdl->sdp_sock->tid, rsp_pkt_len - SDP_HDR_SIZE);

	sdp_send(db_hdl->sdp_sock, rsp_pkt, rsp_pkt_len);  

end:  
	if(rsp_pkt)
		SDP_FREE(rsp_pkt);
}
#endif

static void handle_service_search_attr_req(service_search_attr_struct_t *db_handle)
{
	if(db_handle->service_attr.con_state.len == 0){
		sdp_total_rsp_len = 0;
		sdp_rsp_left_len = 0;

		if(sdp_make_service_search_attr_rsp(db_handle) != SDP_OK){
			return;
		}

		sdp_rsp_left_len = sdp_total_rsp_len;
	}      

	sdp_send_curr_rsp(&db_handle->service_attr, SDP_SERVICESEARCHATTR_RSP);
}

const uint8 hid_descriptor_list[] = 
{
	0x35, 0xcc, 0x35, 0xca, 0x08, 0x22, 0x25, 0xc6, 0x05, 0x01, 0x09, 0x06, 0xa1, 
	0x01, 0x85, 0x01, 0x05, 0x07, 0x19, 0xe0, 0x29, 0xe7, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 
	0x08, 0x81, 0x02, 0x95, 0x01, 0x75, 0x08, 0x81, 0x03, 0x95, 0x05, 0x75, 0x01, 0x05, 0x08, 0x19, 
	0x01, 0x29, 0x05, 0x91, 0x02, 0x95, 0x01, 0x75, 0x03, 0x91, 0x03, 0x95, 0x06, 0x75, 0x08, 0x15, 
	0x00, 0x26, 0xff, 0x00, 0x05, 0x07, 0x19, 0x00, 0x29, 0xff, 0x81, 0x00, 0xc0, 0x05, 0x0c, 0x09, 
	0x01, 0xa1, 0x01, 0x85, 0x03, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x1e, 0x0a, 0x24, 0x02, 
	0x0a, 0x25, 0x02, 0x0a, 0x26, 0x02, 0x0a, 0x27, 0x02, 0x0a, 0x21, 0x02, 0x0a, 0x2a, 0x02, 0x0a, 
	0x23, 0x02, 0x0a, 0x8a, 0x01, 0x09, 0xe2, 0x09, 0xea, 0x09, 0xe9, 0x09, 0xcd, 0x09, 0xb7, 0x09, 
	0xb6, 0x09, 0xb5, 0x0a, 0x83, 0x01, 0x0a, 0x94, 0x01, 0x0a, 0x92, 0x01, 0x0a, 0x09, 0x02, 0x09, 
	0xb2, 0x09, 0xb3, 0x09, 0xb4, 0x09, 0x8d, 0x09, 0x04, 0x09, 0x30, 0x0a, 0x07, 0x03, 0x0a, 0x0a, 
	0x03, 0x0a, 0x0b, 0x03, 0x0a, 0xb1, 0x01, 0x09, 0xb8, 0x81, 0x02, 0x95, 0x01, 0x75, 0x02, 0x81, 
	0x03, 0xc0, 0x05, 0x01, 0x09, 0x80, 0xa1, 0x01, 0x85, 0x04, 0x05, 0x01, 0x19, 0x81, 0x29, 0x83, 
	0x15, 0x00, 0x25, 0x01, 0x95, 0x03, 0x75, 0x01, 0x81, 0x06, 0x95, 0x01, 0x75, 0x05, 0x81, 0x01, 
	0xc0
};

static sdp_status_t sdp_setAttrSeq(uint8* pkt_ptr, int* pkt_len, uint16 attr_id, sdp_record_t *record)
{
	int len=0, i;
	uint8 tmp_des_len_pos=0, tmp_des_len_pos2=0;
	uint8 *buf;  
	uint8 tmp_des_len_pos0 = 0;

	if(!( pkt_ptr&&pkt_ptr&&record)){
		return SDP_ERROR;
	}

	buf = (uint8*)malloc_buffer(SDP_ATTR_SEQ_SIZE);
	if(!buf){
		return SDP_ERROR;
	}     

	/*add record attr id*/
	buf[len++] = SDP_UINT16;
	buf[len++] = (attr_id>>8)&0xff;
	buf[len++] = attr_id&0xff;    

	switch(attr_id){            
		case SDP_ATTR_RECORD_HANDLE:  
			/* add record handle value*/
			buf[len++] = SDP_UINT32;
			buf[len++]= (record->attr_recordHandle.record_handle >>24)&0xff;
			buf[len++]= (record->attr_recordHandle.record_handle >>16)&0xff;
			buf[len++]= (record->attr_recordHandle.record_handle >>8)&0xff;
			buf[len++]= (record->attr_recordHandle.record_handle)&0xff;                
			break;

		case SDP_ATTR_SVCLASS_ID_LIST:
			/*attr value consists of des */
			buf[len++] = SDP_SEQ8;
			tmp_des_len_pos = len++;            
			for(i=0;i<record->attr_serviceClassIdList.uuid_list.num; i++){
				buf[len++] = SDP_UUID16;            
				buf[len++] = (record->attr_serviceClassIdList.uuid_list.uuids[i]>>8)&0xff;
				buf[len++] = (record->attr_serviceClassIdList.uuid_list.uuids[i])&0xff;
			}

			buf[tmp_des_len_pos] = len-tmp_des_len_pos-1;	            
			break;

		case SDP_ATTR_PROTO_DESC_LIST:
			switch(record->handle){
				case SERVER_SERVICE_RECORD_HANDLE:    
					/*add protocol class value*/
					tmp_des_len_pos=0;
					buf[len++] = SDP_SEQ8;
					tmp_des_len_pos = len++;//record the postion to set the des length at last    

					/*
					* every prototocol attr value will be set as des in pdu.
					*/
					/*set l2cap  seq*/
					buf[len++] = SDP_SEQ8;
					tmp_des_len_pos2 = len++;
					buf[len++] = SDP_UUID16;
					buf[len++] = (record->attr_protocolDesciptorList.protocols[0].uuid >> 8)&0xff;
					buf[len++] = (record->attr_protocolDesciptorList.protocols[0].uuid)&0xff; 

					buf[len++] = SDP_UINT16;
					buf[len++] = (record->attr_protocolDesciptorList.protocols[0].para >> 8)&0xff;
					buf[len++] = (record->attr_protocolDesciptorList.protocols[0].para)&0xff;
					buf[tmp_des_len_pos2] = len-tmp_des_len_pos2-1;

					buf[len++] = SDP_SEQ8;
					tmp_des_len_pos2 = len++;
					buf[len++] = SDP_UUID16;
					buf[len++] = (record->attr_protocolDesciptorList.protocols[1].uuid >>8)&0xff;
					buf[len++] = (record->attr_protocolDesciptorList.protocols[1].uuid )&0xff; 
					buf[tmp_des_len_pos2] = len-tmp_des_len_pos2-1;

					buf[tmp_des_len_pos] = len-tmp_des_len_pos-1;
					break;
					
				case BROWSE_GROUP_SERVICE_RECORD_HANDLE:                      
					/*add protocol class value*/    
					buf[len++] = SDP_SEQ8;
					tmp_des_len_pos = len++;//record the postion to set the des length at last    

					/*set l2cap and  sdp protocol seq*/
					for(i=0;i<record->attr_protocolDesciptorList.protocol_num;i++){        	 	
					buf[len++] = SDP_SEQ8;
					tmp_des_len_pos2 = len++; 
					buf[len++] = SDP_UUID16;
					buf[len++] = (record->attr_protocolDesciptorList.protocols[i].uuid >>8)&0xff;
					buf[len++] = (record->attr_protocolDesciptorList.protocols[i].uuid )&0xff; 
					buf[tmp_des_len_pos2] = len-tmp_des_len_pos2-1;	
					}

					buf[tmp_des_len_pos] = len-tmp_des_len_pos-1;                  
					break;
					
				case HID_RECORD_HANDLE:
					/*add protocol class value*/
					tmp_des_len_pos=0;
					buf[len++] = SDP_SEQ8;
					tmp_des_len_pos = len++;//record the postion to set the des length at last    

					/*
					* every prototocol attr value will be set as des in pdu.
					*/
					/*set l2cap  seq*/
					buf[len++] = SDP_SEQ8;
					tmp_des_len_pos2 = len++;
					buf[len++] = SDP_UUID16;
					buf[len++] = (record->attr_protocolDesciptorList.protocols[0].uuid >> 8)&0xff;
					buf[len++] = (record->attr_protocolDesciptorList.protocols[0].uuid)&0xff; 

					buf[len++] = SDP_UINT16;
					buf[len++] = (record->attr_protocolDesciptorList.protocols[0].para >> 8)&0xff;
					buf[len++] = (record->attr_protocolDesciptorList.protocols[0].para)&0xff;
					buf[tmp_des_len_pos2] = len-tmp_des_len_pos2-1;

					buf[len++] = SDP_SEQ8;
					tmp_des_len_pos2 = len++;
					buf[len++] = SDP_UUID16;
					buf[len++] = (record->attr_protocolDesciptorList.protocols[1].uuid >>8)&0xff;
					buf[len++] = (record->attr_protocolDesciptorList.protocols[1].uuid )&0xff; 
					buf[tmp_des_len_pos2] = len-tmp_des_len_pos2-1;

					buf[tmp_des_len_pos] = len-tmp_des_len_pos-1;
					break;
			}
			break;//SDP_ATTR_PROTO_DESC_LIST

		case HID_RECORD_HANDLE:
		case UPNP_INFOR_RECORD_HANDLE:               
			/*add protocol class value*/
			tmp_des_len_pos=0;
			buf[len++] = SDP_SEQ8;
			tmp_des_len_pos = len++;//record the postion to set the des length at last    

			/*
			* every prototocol attr value will be set as des in pdu.
			*/
			/*set l2cap  seq*/
			buf[len++] = SDP_SEQ8;
			tmp_des_len_pos2 = len++;
			buf[len++] = SDP_UUID16;
			buf[len++] = (record->attr_protocolDesciptorList.protocols[0].uuid >> 8)&0xff;
			buf[len++] = (record->attr_protocolDesciptorList.protocols[0].uuid)&0xff; 

			buf[len++] = SDP_UINT16;
			buf[len++] = (record->attr_protocolDesciptorList.protocols[0].para >> 8)&0xff;
			buf[len++] = (record->attr_protocolDesciptorList.protocols[0].para)&0xff;
			buf[tmp_des_len_pos2] = len-tmp_des_len_pos2-1;

			buf[len++] = SDP_SEQ8;
			tmp_des_len_pos2 = len++;
			buf[len++] = SDP_UUID16;
			buf[len++] = (record->attr_protocolDesciptorList.protocols[1].uuid >>8)&0xff;
			buf[len++] = (record->attr_protocolDesciptorList.protocols[1].uuid )&0xff; 
			buf[tmp_des_len_pos2] = len-tmp_des_len_pos2-1;

			buf[tmp_des_len_pos] = len-tmp_des_len_pos-1;
			break;

		case SDP_ATTR_ADD_PROTO_DESC_LIST:  //hid
			/*add protocol class value*/
			tmp_des_len_pos=0;
			buf[len++] = SDP_SEQ8;
			tmp_des_len_pos0 = len++;//record the postion to set the des length at last 

			buf[len++] = SDP_SEQ8;
			tmp_des_len_pos = len++;

			/*
			* every prototocol attr value will be set as des in pdu.
			*/
			/*set l2cap  seq*/
			buf[len++] = SDP_SEQ8;
			tmp_des_len_pos2 = len++;
			buf[len++] = SDP_UUID16;
			buf[len++] = (record->attr_protocolDesciptorList2.protocols[0].uuid >> 8)&0xff;
			buf[len++] = (record->attr_protocolDesciptorList2.protocols[0].uuid)&0xff;

			buf[len++] = SDP_UINT16;
			buf[len++] = (record->attr_protocolDesciptorList2.protocols[0].para >> 8)&0xff;
			buf[len++] = (record->attr_protocolDesciptorList2.protocols[0].para)&0xff;
			buf[tmp_des_len_pos2] = len-tmp_des_len_pos2-1;

			buf[len++] = SDP_SEQ8;
			tmp_des_len_pos2 = len++;
			buf[len++] = SDP_UUID16;
			buf[len++] = (record->attr_protocolDesciptorList2.protocols[1].uuid >>8)&0xff;
			buf[len++] = (record->attr_protocolDesciptorList2.protocols[1].uuid)&0xff;
			buf[tmp_des_len_pos2] = len-tmp_des_len_pos2-1;

			buf[tmp_des_len_pos] = len-tmp_des_len_pos-1;
			buf[tmp_des_len_pos0] = len-tmp_des_len_pos0-1;
			break;

		case SDP_ATTR_LANG_BASE_ATTR_ID_LIST:
			buf[len++] = SDP_SEQ8;
			tmp_des_len_pos = len++;

			buf[len++] = SDP_UINT16;
			buf[len++] = (record->attr_lang.langId >> 8)&0xff;
			buf[len++] = (record->attr_lang.langId)&0xff;
			buf[len++] = SDP_UINT16;
			buf[len++] = (record->attr_lang.encoding >> 8)&0xff;
			buf[len++] = (record->attr_lang.encoding) & 0xff;
			buf[len++] = SDP_UINT16;
			buf[len++] = (record->attr_lang.langBaseId >> 8)&0xff;
			buf[len++] = (record->attr_lang.langBaseId) & 0xff;

			buf[tmp_des_len_pos] = len - tmp_des_len_pos - 1;
			break;
			
		case SDP_ATTR_HID_DESCRIPTOR_LIST://HID
			memcpy(buf+len, hid_descriptor_list, sizeof(hid_descriptor_list));
			len += sizeof(hid_descriptor_list);
			break;

		case SDP_ATTR_SVCNAME_PRIMARY:
			buf[len++] = SDP_TEXT_STR8;
			tmp_des_len_pos = len++;

			for(i=0; i<record->attr_svcName.len; i++)
				buf[len++] = record->attr_svcName.str[i];

			buf[tmp_des_len_pos] = len - tmp_des_len_pos - 1;
			break;
			
		case SDP_ATTR_PROVNAME_PRIMARY:
			buf[len++] = SDP_TEXT_STR8;
			tmp_des_len_pos = len++;

			for(i=0; i<record->attr_provName.len; i++)
				buf[len++] = record->attr_provName.str[i];

			buf[tmp_des_len_pos] = len - tmp_des_len_pos - 1;
			break;

		case SDP_ATTR_SERVICE_AVAILABILITY:
			/*add value*/    
			buf[len++] = SDP_UINT8;
			buf[len++] = record->attr_serviceAvailability.value;            
			break;

		case SDP_ATTR_PFILE_DESC_LIST://HID
			buf[len++] = SDP_SEQ8;
			tmp_des_len_pos = len++;  

			/* every protocol descriptor consists of des*/
			buf[len++] = SDP_SEQ8;
			tmp_des_len_pos2 = len++; 
			buf[len++] = SDP_UUID16;
			buf[len++] = (record->attr_bluetoothProfile.profile.profileId>>8)&0xff;
			buf[len++] = (record->attr_bluetoothProfile.profile.profileId)&0xff;     
			buf[len++] = SDP_UINT16;
			buf[len++] = (record->attr_bluetoothProfile.profile.version>>8)&0xff;
			buf[len++] = (record->attr_bluetoothProfile.profile.version)&0xff;
			buf[tmp_des_len_pos2] = len-tmp_des_len_pos2-1;      

			buf[tmp_des_len_pos] = len-tmp_des_len_pos-1;
			break;

		case SDP_ATTR_SUPPORTED_FORMATS_LIST:
			buf[len++] = SDP_SEQ8;
			tmp_des_len_pos = len++;
			tmp_des_len_pos2 = 0;
			for(i=0; i<record->attr_supportedFormat.count; i++){               
				buf[len++] = SDP_UINT8;
				buf[len++]= (record->attr_supportedFormat.format[i])&0xff;
				tmp_des_len_pos2 += 2;
			}

			buf[tmp_des_len_pos] = tmp_des_len_pos2;
			break;

		case SDP_ATTR_SUPPORTED_FEATURES:    
			/* add record handle value*/
			buf[len++] = SDP_UINT16;
			buf[len++]= (record->attr_supportedFeature.feature >>8)&0xff;
			buf[len++]= (record->attr_supportedFeature.feature)&0xff;
			break;

		case SDP_ATTR_EXTERNAL_NETWORK:
			buf[len++] = SDP_SEQ8;
			buf[len++] = record->attr_network.value;
			break;

		case SDP_ATTR_HID_PARSER_VERSION:
			buf[len++] = SDP_UINT16;
			buf[len++] = (record->attr_parseVersion.value >> 8) & 0xff;
			buf[len++] = record->attr_parseVersion.value & 0xff;
			break;
			
		case SDP_ATTR_HID_PROFILE_VERSION:
			buf[len++] = SDP_UINT16;
			buf[len++] = (record->attr_profileVersion.value >> 8) & 0xff;
			buf[len++] = record->attr_profileVersion.value & 0xff;
			break;

		case SDP_ATTR_HID_DEVICE_SUBCLASS:
			buf[len++] = SDP_UINT8;
			buf[len++] = record->attr_deviceSubclass.value;
			break;
			
		case SDP_ATTR_HID_COUNTRY_CODE:
			buf[len++] = SDP_UINT8;
			buf[len++] = record->attr_countryCode.value;
			break;
			
		case SDP_ATTR_HID_SDP_DISABLE:
		case SDP_ATTR_HID_REMOTE_WAKEUP:
			buf[len++] = SDP_BOOL;
			buf[len++] = 0x00;
			break;
			
		case SDP_ATTR_HID_BATTERY_POWER:
			buf[len++] = SDP_BOOL;
			buf[len++] = 0x01;
			break;

		case SDP_ATTR_HID_RECONNECT_INITIATE:
			buf[len++] = SDP_BOOL;
			buf[len++] = record->attr_reconnInitiate.value;
			break;
			
		case SDP_ATTR_HID_VIRTUAL_CABLE:
			buf[len++] = SDP_BOOL;
			buf[len++] = record->attr_virtualCable.value;
			break;
			
		case SDP_ATTR_HID_LANG_ID_BASE_LIST:
			buf[len++] = SDP_SEQ8;
			tmp_des_len_pos = len++;
			buf[len++] = SDP_SEQ8;
			tmp_des_len_pos2 = len++;
			buf[len++] = SDP_UINT16;
			buf[len++] = (record->attr_langBase.langId>>8)&0xff;
			buf[len++] = record->attr_langBase.langId;
			buf[len++] = SDP_UINT16;
			buf[len++] = (record->attr_langBase.langBaseId>>8)&0xff;
			buf[len++] = record->attr_langBase.langBaseId;
			buf[tmp_des_len_pos2] = len-tmp_des_len_pos2-1;
			buf[tmp_des_len_pos] = len-tmp_des_len_pos-1;
			break;
			
		case SDP_ATTR_HID_BOOT_DEVICE:
			buf[len++] = SDP_BOOL;
			buf[len++] = record->attr_bootDevice.value;
			break;

		case SDP_ATTR_HID_NORMALLY_CONNECTABLE:
			buf[len++] = SDP_BOOL;
			buf[len++] = record->attr_normConnectable.value;
			break;

		/*default will not be exec because attr ids have been filtered before this function is called.*/
		default:
			SDP_FREE(buf);
			return SDP_OK;
	}

	/*Check 1. if attribute data size more than max byte
	2. buffer overflow   
	*/
	if((*pkt_len+len)> SDP_MAX_RSP_BUF_SIZE){
		SDP_FREE(buf);
		return SDP_MORE_THAN_MAX_BYTE;
	} else {
		memcpy(pkt_ptr+*pkt_len, buf, len);
		*pkt_len += len;
		SDP_FREE(buf);
		return SDP_OK;
	}
}

static sdp_status_t handle_attribute_list( uint8 *pkt_ptr, int *pkt_len, service_attr_struct_t *db_hdl)
{
	int i,j;
	uint32 attr;
	sdp_status_t status;
	int found = 0;

	sdp_record_t *sdp_recordPtr = &hid_sdp_record;

	for (i = 0; i < db_hdl->attr_cnt; i++){
		attr = db_hdl->attr_list[i] & 0xffff;

		/* get the match attrid in attribute list of the record*/
		for(j=0,found=0; j<sdp_recordPtr->attr_id_num; j++){
			if(attr==sdp_recordPtr->attr_id_list[j]){
				found = 1;
				break;
			}
		}
		if(!found){//fi can not find the attr id in list then check the follows            
			continue;
		}

		/*make response pdu by attr value*/ 
		status = sdp_setAttrSeq(pkt_ptr, pkt_len, attr, sdp_recordPtr);
		if(SDP_OK == status){
			continue;
		} else {
			return status;
		}
	}
  
	return status = SDP_OK;
}

static sdp_status_t  handle_attribute_range( uint8 *pkt_ptr, int *pkt_len, service_attr_struct_t *db_hdl)
{
	int i;
	uint32 attr_begin,attr_end, tmp=0;
	uint8  id_index;//the index in attr list to make the continue response pdu
	uint16 attr_id;
	sdp_status_t status = SDP_ERROR;

	sdp_record_t *sdp_recordPtr = &hid_sdp_record;      

	/*if attribute id range from 0000 to 0xffff, send all of the attribute value of the record.   */
	if((1==db_hdl->attr_cnt)&&(db_hdl->attr_list[0] == 0x0000ffff)){
		for (i= 0; i<sdp_recordPtr->attr_id_num; i++){
			attr_id = sdp_recordPtr->attr_id_list[i];
			status = sdp_setAttrSeq(pkt_ptr, pkt_len, attr_id, sdp_recordPtr);            
			if(status !=SDP_OK){
				return status;
			}
		}
		return SDP_OK;                   
	}

	/* attribute id range should be in the low to high order*/
	for (i = 0; i < db_hdl->attr_cnt; i++){
		attr_begin =  (db_hdl->attr_list[i]>>16)&0xffff;
		attr_end = db_hdl->attr_list[i] & 0xffff;
		/*Check the attr id order*/
		if((attr_end<attr_begin)||(attr_begin<tmp)){
			set_err(SDP_INVALID_REQUEST_SYNTAX);
			return SDP_ERROR;
		}
		tmp = attr_end; 

		for(id_index=0; id_index<sdp_recordPtr->attr_id_num; id_index++){
			if(sdp_recordPtr->attr_id_list[id_index] < attr_begin){
				continue;
			}
			if(sdp_recordPtr->attr_id_list[id_index] > attr_end){
				break;
			}
			status = sdp_setAttrSeq(pkt_ptr, pkt_len, sdp_recordPtr->attr_id_list[id_index], sdp_recordPtr); 

			if (SDP_OK!=status){
				return status;
			}
		}
	}   

	return SDP_OK;
}

static sdp_status_t sdp_make_service_attr_rsp(service_attr_struct_t *db_hdl)
{ 
	int rsp_pkt_len, des_len_pos;
	sdp_status_t status;

	rsp_pkt_len = 0;
	sdp_rsp_buf[rsp_pkt_len++] = SDP_SEQ16;
	des_len_pos = rsp_pkt_len;
	rsp_pkt_len += 2;//skip the des len field

	if(db_hdl->rec_hdl != HID_RECORD_HANDLE){
		send_error_rsp(db_hdl->sdp_sock, SDP_INVALID_SERVICE_RECORD_HANDLE);
		return SDP_ERROR;
	}

	/*Check the attribute id order */
	if ((db_hdl->attr_list[0] & 0xffff) ^ (db_hdl->attr_list[0] >> 16)){//id range
		/*handle range id */
		status = handle_attribute_range(sdp_rsp_buf, &rsp_pkt_len, db_hdl);
	}
	else{
		/*handle individual id*/
		status = handle_attribute_list(sdp_rsp_buf, &rsp_pkt_len, db_hdl);
	}

	if(SDP_OK != status){
		send_error_rsp(db_hdl->sdp_sock, get_err());	  	 		
		return SDP_ERROR;
	}

	/* The length of the attribute list is the same as the packet length minus
	the packet header length minus the attibute byte count filed minus the
	data element sequence header*/
	sdp_rsp_buf[des_len_pos]  = SHORT2CHAR_MS(rsp_pkt_len  - DES_HDR_2B_LEN);
	sdp_rsp_buf[des_len_pos + 1] = SHORT2CHAR_LS(rsp_pkt_len - DES_HDR_2B_LEN);	  

	sdp_total_rsp_len = rsp_pkt_len;

	return SDP_OK;
}

static sdp_status_t sdp_make_service_search_attr_rsp(service_search_attr_struct_t *db_handle)
{ 
	uint32 rec_hdl_list[1];
	uint32 rec_hdl_cnt;
	int  rsp_pkt_len, des_len_pos, tmp_des_len_pos, i = 0;      
	sdp_status_t status;

	rec_hdl_cnt = get_rec_hdl(db_handle->service_class_list, db_handle->service_class_cnt, rec_hdl_list);

	rsp_pkt_len = 0;     
	sdp_rsp_buf[rsp_pkt_len++] = SDP_SEQ16;
	des_len_pos = rsp_pkt_len;
	rsp_pkt_len += 2;//skip the des len field

	for (i = 0; i < rec_hdl_cnt; i++)
	{
		sdp_rsp_buf[rsp_pkt_len++] = SDP_SEQ16;
		tmp_des_len_pos = rsp_pkt_len;
		rsp_pkt_len += 2;
		db_handle->service_attr.rec_hdl = rec_hdl_list[i];
		/*Check the attribute id order */
		if ((db_handle->service_attr.attr_list[0] & 0xffff) ^ (db_handle->service_attr.attr_list[0] >> 16)){
			/*handle range id */
			status = handle_attribute_range(sdp_rsp_buf, &rsp_pkt_len, &db_handle->service_attr);
		} else {
			/*handle individual id*/ 	
			status = handle_attribute_list(sdp_rsp_buf, &rsp_pkt_len, &db_handle->service_attr);
		}
		if(SDP_OK == status){
			sdp_rsp_buf[tmp_des_len_pos] = SHORT2CHAR_MS(rsp_pkt_len-tmp_des_len_pos-2); //SDP_SEQ16: 2
			sdp_rsp_buf[tmp_des_len_pos+1] = SHORT2CHAR_LS(rsp_pkt_len-tmp_des_len_pos-2); //SDP_SEQ16: 2
			continue;
		} else {
			send_error_rsp(db_handle->service_attr.sdp_sock, get_err());	 
			return SDP_ERROR;
		}     
	}

	/* The length of the attribute list is the same as the packet length minus
	the packet header length minus the attibute byte count filed minus the
	data element sequence header*/
	sdp_rsp_buf[des_len_pos]  = SHORT2CHAR_MS(rsp_pkt_len  - DES_HDR_2B_LEN);
	sdp_rsp_buf[des_len_pos + 1] = SHORT2CHAR_LS(rsp_pkt_len - DES_HDR_2B_LEN);	  

	sdp_total_rsp_len = rsp_pkt_len;

	SDP_FREE(rec_hdl_list);

	return SDP_OK;
}


static void handle_service_attr_req(service_attr_struct_t *db_hdl)
{
	if(db_hdl->con_state.len == 0){
		sdp_total_rsp_len = 0;
		sdp_rsp_buf = (uint8 *)sdp_hid_attr_list = 3;
		sdp_rsp_left_len = sizeof(sdp_hid_attr_list)/sizeof(sdp_hid_attr_list[0]) - 3;
	}

	sdp_send_curr_rsp(db_hdl, SDP_SERVICEATTR_RSP);
}
#endif
uint8 sdp_search_response[] = {0x03, 0x00, 0x01, 0x00, 0x09, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x00, 0x0b, 0x00};
uint8 sdp_search_response_empty[] = {0x03, 0x00, 0x01, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00};
static void process_service_search_req(sdp_socket_t *sdp_sock, uint8 *data, uint16 len)
{
	uuid16_t service_search_uuid[12];
	int service_search_uuid_cnt;
	uint8 des_len;
	int tmp_pos, i, cur_pos = 0;
	int max_rec_cnt;
	int new_pos[1];
	int cont_state_len;
	service_search_struct_t db_hdl;

	if (GET_TYPE(data[cur_pos]) != DES_TYPE){
		send_error_rsp(sdp_sock, SDP_INVALID_REQUEST_SYNTAX);
		return;
	}

	des_len = sdp_getSize(data + cur_pos, new_pos);
	cur_pos += *new_pos;

	if (len-cur_pos-des_len-2 <= 0) {
		send_error_rsp(sdp_sock, SDP_INVALID_REQUEST_SYNTAX);
		return;
	}

	tmp_pos = 0;
	i = 0;
	while (tmp_pos < des_len)
	{
		if (i >= 12) {
			send_error_rsp(sdp_sock, SDP_INVALID_REQUEST_SYNTAX);
			break;
		}

		if (data[cur_pos] == UUID16_HDR) {
			service_search_uuid[i] = CHAR2INT16(data[cur_pos + 1],
			      data[cur_pos + 2]);
			i++;
			tmp_pos += 3;
			cur_pos += 3;
		} else if (data[cur_pos] == UUID32_HDR) {
			cur_pos += 2;
			service_search_uuid[i] = CHAR2INT16(data[cur_pos + 1],
			      data[cur_pos + 2]);
			i++;
			tmp_pos += 5;
			cur_pos += 3;
		} else if (data[cur_pos] == UUID128_HDR) {
			cur_pos += 2;
			service_search_uuid[i] = CHAR2INT16(data[cur_pos + 1],
			      data[cur_pos + 2]);
			i++;
			tmp_pos += 17;
			cur_pos += 15;
		} else {
			send_error_rsp(sdp_sock, SDP_INVALID_REQUEST_SYNTAX);
			return;
		}
	}
	service_search_uuid_cnt = i;

	max_rec_cnt = CHAR2INT16(data[cur_pos], data[cur_pos + 1]);

	cur_pos += 2;
	cont_state_len = data[cur_pos++];

	if ((cont_state_len)||(service_search_uuid_cnt != 1)){
		send_error_rsp(sdp_sock, SDP_INSUFFICIENT_RESOURCES);
		return;
	}

	if (len != cur_pos) {
		send_error_rsp(sdp_sock, SDP_INVALID_PDU_SIZE);
		return;
	}

	if((service_search_uuid[0] != L2CAP_UUID)&&(service_search_uuid[0] != HID_SVCLASS_ID)) {
		sdp_search_response_empty[1] = (sdp_sock->tid>>8);
		sdp_search_response_empty[2] = sdp_sock->tid;
		l2cap_send_data(sdp_sock->sock, sdp_search_response_empty, 10);
	} else {
		sdp_search_response[1] = (sdp_sock->tid>>8);
		sdp_search_response[2] = sdp_sock->tid;
		l2cap_send_data(sdp_sock->sock, sdp_search_response, 14);
	}
}

static void process_service_attr_req(sdp_socket_t* sdp_sock, uint8 *data, uint8 len)
{
	service_attr_struct_t *db_hdl = NULL;
	int tmp_len, cur_pos = 0;
	uint8 des_len;

	uint32 rec_hdl;
	uint32 max_attr_cnt;
	int attr_list_pos = 0;

	int cont_state_len;

	int new_pos;

	db_hdl = (service_attr_struct_t*)malloc_buffer(sizeof(service_attr_struct_t));	
	if(db_hdl == NULL)
		goto end;

	rec_hdl = CHAR2INT32(data[cur_pos], data[cur_pos + 1],
					data[cur_pos + 2], data[cur_pos + 3]);

	if(rec_hdl != HID_RECORD_HANDLE) {
		send_error_rsp(sdp_sock, SDP_INVALID_SERVICE_RECORD_HANDLE);
		goto end;
	}

	cur_pos += 4;

	// max amount of attribute data to return
	max_attr_cnt = CHAR2INT16(data[cur_pos], data[cur_pos + 1]);
	cur_pos += 2;

	if (GET_TYPE(data[cur_pos]) != DES_TYPE) {
		send_error_rsp(sdp_sock, SDP_INVALID_REQUEST_SYNTAX);
		goto end;
	}

	des_len = sdp_getSize(data + cur_pos, &new_pos);
	cur_pos += new_pos;

	if (len <= 4+2+new_pos+des_len) {
		send_error_rsp(sdp_sock, SDP_INVALID_REQUEST_SYNTAX);
		goto end;
	}

	while (des_len > 0){
		tmp_len = sdp_getSize(data + cur_pos, &new_pos);

		cur_pos += new_pos;
		if (tmp_len == 4) {
			cur_pos += 4;
			attr_list_pos++;
		} else {
			cur_pos += 2;
			attr_list_pos++;
		}
		des_len -= (new_pos + tmp_len);
	}

	cont_state_len = data[cur_pos++];

	if (cont_state_len) {
		if(cont_state_len != 2){// 2 bytes continue infor to store the last index in attribute array.
			send_error_rsp(sdp_sock, SDP_INVALID_CONTINUATION_STATE);

			goto end;
		}
	}

	cur_pos += cont_state_len;

	if (len != cur_pos) {
		send_error_rsp(sdp_sock, SDP_INVALID_PDU_SIZE);
		goto end;
	}

	if(max_attr_cnt> MAX_SDP_ATTR_LENGTH)
		max_attr_cnt = MAX_SDP_ATTR_LENGTH;
	db_hdl->max_attr_byte_cnt = max_attr_cnt;
	db_hdl->sdp_sock = sdp_sock;

	if(cont_state_len == 0){
		sdp_rsp_buf = (uint8 *)sdp_hid_attr_list + 3;
		sdp_rsp_left_len = sizeof(sdp_hid_attr_list)/sizeof(sdp_hid_attr_list[0]) - 3;
		sdp_total_rsp_len = sdp_rsp_left_len;
	}

	sdp_send_curr_rsp(db_hdl, SDP_SERVICEATTR_RSP);

end:
	if(db_hdl)
		SDP_FREE(db_hdl);     
    
}

static void process_service_search_attr_req(sdp_socket_t *sdp_sock, uint8 *data, unsigned short len)
{
	service_search_attr_struct_t *db_hdl = NULL;
	uuid16_t *service_search_uuid  = NULL;
	int service_search_uuid_cnt;
	uint8 des_len;
	int tmp_pos, tmp_len, i, cur_pos = 0;
	int max_attr_cnt;
	int new_pos;
	int attr_list_pos = 0;
	uint8 cont_state_len, found;

	db_hdl = (service_search_attr_struct_t*)malloc_buffer(sizeof(service_search_attr_struct_t));	
	if(db_hdl == NULL)
		goto end;	

	service_search_uuid = (uuid16_t*)malloc_buffer(12*sizeof(uuid16_t));
	if(service_search_uuid == NULL)
		goto end;	  

	if (GET_TYPE(data[cur_pos]) != DES_TYPE) {
		send_error_rsp(sdp_sock, SDP_INVALID_REQUEST_SYNTAX);
		goto end;
	}

	des_len = sdp_getSize(data + cur_pos, &new_pos);
	cur_pos += new_pos;

	if (des_len > (len - DES_HDR_LEN)) {
		send_error_rsp(sdp_sock, SDP_INVALID_REQUEST_SYNTAX);
		goto end;
	}

	tmp_pos = 0;
	i = 0;
	while (tmp_pos < des_len) {
		if (i >= 12) {
			break;
		}

		if (data[cur_pos] == UUID16_HDR) {
			service_search_uuid[i] = CHAR2INT16(data[cur_pos + 1],
			data[cur_pos + 2]);
			i++;
			tmp_pos += 3;
			cur_pos += 3;
		} else if (data[cur_pos] == UUID32_HDR) { 
			cur_pos += 2;
			service_search_uuid[i] = CHAR2INT16(data[cur_pos + 1],
			data[cur_pos + 2]);
			i++;
			tmp_pos += 5;
			cur_pos += 3;
		} else if (data[cur_pos] == UUID128_HDR) {
			cur_pos += 2;
			service_search_uuid[i] = CHAR2INT16(data[cur_pos + 1],
			data[cur_pos + 2]);
			i++;
			tmp_pos += 17;
			cur_pos += 15;
		} else {
			send_error_rsp(sdp_sock, SDP_INVALID_REQUEST_SYNTAX);
			goto end;
		}
	}
	service_search_uuid_cnt = i;

	max_attr_cnt = CHAR2INT16(data[cur_pos], data[cur_pos + 1]);
	cur_pos += 2;

	if (GET_TYPE(data[cur_pos]) != DES_TYPE) {
		send_error_rsp(sdp_sock, SDP_INVALID_REQUEST_SYNTAX);
		goto end;
	}

	des_len = sdp_getSize(data + cur_pos, &new_pos);
	cur_pos += new_pos;

	if (len <= cur_pos+ des_len) {
		send_error_rsp(sdp_sock, SDP_INVALID_REQUEST_SYNTAX);
		goto end;
	}

	while (des_len > 0) {
		/* Get the length of the data element */
		tmp_len = sdp_getSize(data + cur_pos, &new_pos);
		/* Move the data pointer past the length field to the start of
		the data element */
		cur_pos += new_pos;
		if (tmp_len == 4) {
			cur_pos += 4;
			attr_list_pos++;
		} else {
			cur_pos += 2;
			attr_list_pos++;
		}
		des_len -= (new_pos + tmp_len);
	}

	cont_state_len = data[cur_pos++];
	if (cont_state_len) {
		if(cont_state_len != 2){// 2 bytes continue infor indicates the left length of rsp buffer 
			send_error_rsp(sdp_sock, SDP_INVALID_CONTINUATION_STATE);
			goto end;
		}
	}

	cur_pos += cont_state_len;

	if (len != cur_pos) {
		send_error_rsp(sdp_sock, SDP_INVALID_PDU_SIZE);
		goto end;
	}

	if(max_attr_cnt> MAX_SDP_ATTR_LENGTH)
		max_attr_cnt = MAX_SDP_ATTR_LENGTH;
	db_hdl->service_attr.max_attr_byte_cnt = max_attr_cnt;
	db_hdl->service_attr.sdp_sock = sdp_sock;

	if(cont_state_len == 0) {
		found = 0;
		for(i=0; i<service_search_uuid_cnt; i++) {
			if((service_search_uuid[i] == L2CAP_UUID) || (service_search_uuid[i] == HID_SVCLASS_ID)) {
				found = 1;
				break;
			}
		}
		if(found == 1) {
			sdp_rsp_buf = (uint8 *)sdp_hid_attr_list;
			sdp_rsp_left_len = sizeof(sdp_hid_attr_list)/sizeof(sdp_hid_attr_list[0]);
			sdp_total_rsp_len = sdp_rsp_left_len;
		} else {
			sdp_rsp_buf = (uint8 *)sdp_hid_attr_list_empty;
			sdp_rsp_left_len = sizeof(sdp_hid_attr_list_empty)/sizeof(sdp_hid_attr_list_empty[0]);
			sdp_total_rsp_len = sdp_rsp_left_len;
		}
	}

	sdp_send_curr_rsp(&db_hdl->service_attr, SDP_SERVICESEARCHATTR_RSP);

end:    
	if(db_hdl)
		SDP_FREE(db_hdl);    
	if(service_search_uuid)
		SDP_FREE(service_search_uuid);
}

void sdp_parseData(sdp_msg_t *sdp_msg)
{
	uint8 id;
	unsigned short par_len;
	sdp_socket_t *sdp_sock;
	uint8 *data = (uint8*)sdp_msg->buf->data;   
	int len = sdp_msg->buf->len;

	sdp_sock= sdp_msg->sdp_sock;

	id = data[SDP_HDR_TYPE];
	sdp_sock->tid = CHAR2INT16(data[SDP_HDR_TRANS_ID_MS], data[SDP_HDR_TRANS_ID_LS]);
	par_len = CHAR2INT16(data[SDP_HDR_LENGTH_MS], data[SDP_HDR_LENGTH_LS]);
	data += SDP_HDR_SIZE;

	if (par_len > (len - SDP_HDR_SIZE)){
		return;
	}

	switch (id)
	{
		case SDP_ERROR_RSP:
			break;
		case SDP_SERVICESEARCH_REQ:
			process_service_search_req(sdp_sock, data, par_len);
			break;

		case SDP_SERVICEATTR_REQ:
			process_service_attr_req(sdp_sock, data, par_len);
			break;

		case SDP_SERVICESEARCHATTR_REQ:
			process_service_search_attr_req(sdp_sock, data, par_len);
			break;

		default:
			send_error_rsp(sdp_sock, SDP_INVALID_REQUEST_SYNTAX);
			break;
	}	
}

void sdp_l2cap_notify_callback(enum l2cap_event_enum event, uint32 l2cap_handle, void *pdata, uint8 reason)
{
	switch(event) {
		case L2CAP_CHANNEL_NEW_OPENED:
			sdp_sock_global.sock = l2cap_handle;
			sdp_sock_global.server = 1;
			break;
		case L2CAP_CHANNEL_CLOSED:
			sdp_sock_global.sock = 0xFFFFFFFF;
			break;
		default:
			break;
	}
}
void sdp_l2cap_datarecv_callback(uint32 l2cap_handle, struct sk_buff *skb)
{
	sdp_msg_t  *sdp_msg;
	sdp_socket_t *sdp_sock = &sdp_sock_global;
	
	if(sdp_sock->sock != l2cap_handle){
		return;
	}    

	if(!skb||!skb->data){
		return ;
	}
	sdp_msg = (sdp_msg_t*)malloc_buffer(sizeof(sdp_msg_t));
	if(!sdp_msg) {
		skb_free(skb);
		return;
	}

	sdp_msg->type = SDP_DATA_MSG;
	sdp_msg->sdp_sock = sdp_sock;
	sdp_msg->buf = skb; 
	if(sdp_sock->server){
		sdp_parseData(sdp_msg);
		skb_free(skb);
		SDP_FREE(sdp_msg);
	}
	return;
}

void sdp_init(void)
{
	sdp_sock_global.sock = 0xFFFFFFFF;
	sdp_sock_global.server = 1;
	//sdp_hidRecordInit(&hid_sdp_record);
	l2cap_register(PSM_SDP, 1, sdp_l2cap_notify_callback, sdp_l2cap_datarecv_callback);
}
