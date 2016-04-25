#ifndef _SDP_API_H
#define _SDP_API_H

#include "stm32f10x_type.h"

#define HID_SVCLASS_ID			0x1124

typedef struct {
	uint8 data[16];
} uint128_t;

typedef uint16 uuid16_t;

typedef struct {
	uint8 type;
	union {
		uint16  uuid16;
		uint32  uuid32;
		uint128_t uuid128;
	} value;
} uuid_t;

typedef enum {
    SDP_OK=0,
    SDP_MORE_THAN_MAX_BYTE,
    SDP_ERROR = -1    
}sdp_status_t;

typedef struct uuid_list{
    uint8 num;//uuid number
    uuid_t uuids[12];//warning: if the array len changed , record database should be modified also.
}uuid_list_t;

typedef enum {
    SDP_ATTR_REQ_SINGLE,
    SDP_ATTR_REQ_RANGE
}sdp_attr_req_type_t;

typedef struct sdp_attr_req_struct sdp_attr_req_t;

struct sdp_attr_req_struct{
    sdp_attr_req_type_t type;
    
    uint8 num;                       
    union{
	    uint16 *attr_id;
	    uint32 *attr_range;    
    }attr;
};

#endif	//_SDP_API_H

