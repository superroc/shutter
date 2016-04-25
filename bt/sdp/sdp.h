/***************************************************************
*      Accel¡¡Semiconductor Corp.                          
*      (c) Copyright 2004 - 2006, All Rights Reserved      
*                                                      
*  File Name           : sdp.h                       
*  Object              :                                    
*  Description:         :Service discovery protocol
*  
*  7/11/06   		: Creation                            
*----------------------------------------------------------- */


#ifndef	SDP_H
#define	SDP_H

#include "l2cap_api.h"
#include "stm32f10x_type.h"

/*
  **************************************************************
  *	Constant and Macro defination
  **************************************************************
  */
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 
#endif

#define MAX_SDP_SOCK_NUMBER 7
#define MAX_SDP_QUE_SIZE    4
#define SDP_CONNECT_TIMEOUT 5000// to be fixed.

#define SDP_DATA_MSG	1
#define SDP_LINK_MSG	0

#define SDP_NO_BROWSE   //if no browse function added sdp serveice search attr buffrer can be reduced a lot.

#define SDP_UUID_SEQ_SIZE 256
#define USHRT_MAX 0xffff

#define SDP_REQ_BUFFER_SIZE	100
#define HANDLE_RSP_BUF_SIZE 100
#define ATTR_RSP_BUF_SIZE 200
#define SDP_ATTR_SEQ_SIZE   256
#ifdef SDP_NO_BROWSE
#define SVC_SEARCH_ATTR_RSP_BUF_SIZE 200
#define SERVICE_SEARCH_ATTR_RSP_BUF_SIZE 200
#else
#define SVC_SEARCH_ATTR_RSP_BUF_SIZE 300
#define SERVICE_SEARCH_ATTR_RSP_BUF_SIZE 300
#endif

#define SDP_MAX_RSP_BUF_SIZE 512
 
#define SDP_RESPONSE_TIMEOUT	20000

//#define SDP_ERROR_RSP             1
#define SDP_SERVICESEARCH_REQ     2
#define SDP_SERVICESEARCH_RSP     3
#define SDP_SERVICEATTR_REQ       4
#define SDP_SERVICEATTR_RSP       5
#define SDP_SERVICESEARCHATTR_REQ 6
#define SDP_SERVICESEARCHATTR_RSP 7

#define SDP_INVALID_SDP_VERSION           0x0001
#define SDP_INVALID_SERVICE_RECORD_HANDLE 0x0002
#define SDP_INVALID_REQUEST_SYNTAX        0x0003
#define SDP_INVALID_PDU_SIZE              0x0004
#define SDP_INVALID_CONTINUATION_STATE    0x0005
#define SDP_INSUFFICIENT_RESOURCES        0x0006

#define SERVICE_SEARCH_RSP_BUF_SIZE 100
#define SERVICE_ATTR_RSP_BUF_SIZE 128
/*
  * macro used in sdp database
  */
#define MAX_ATTR_ID_NUMBER 20  //  max attr id in a record.
#define ATTR_ID_MIN SDP_ATTR_RECORD_HANDLE
#define ATTR_ID_MAX SDP_ATTR_HID_BOOT_DEVICE//note:it may be changed!
#define HEADSET_VERSION 0x0100
#define HANDSFREE_VERSION 0x0105
#define OPP_VERSION       0x0100
#define FTP_VERSION       0x0100
#define DUN_VERSION       0x0100
#define AUDIO_SINK_VERSION  0x0102
#define AUDIO_SOURCE_VERSION 0x0102
#define RC_CONTROLLER_VERSION 0x0103
#define ADVANCED_AUDIO_VERSION 0x0100
#define ADVANCED_REMOTECONTROL_VERSION 0x0104
#define PNP_VERSION				0x0100

//#define HANDSFREE_SUPPORTED_FEATURE  0x1E
#define HFAG_SUPPORTED_FEATURE      0x09

/*
  * record handles
  */
#define SERVER_SERVICE_RECORD_HANDLE                0x00000000
#define BROWSE_GROUP_SERVICE_RECORD_HANDLE  0x00100000
#define HEADSET_RECORD_HANDLE                           0x00100001
#define HANDSFREE_RECORD_HANDLE                         0x00100002
#define HFAG_RECORD_HANDLE                           0x00100009
#define HSAG_RECORD_HANDLE                           0x00100008
#define SERPORT_RECORD_HANDLE                             0x00100003
#define OPP_RECORD_HANDLE                               0x00100004
#define FTP_RECORD_HANDLE                               0x00100005
#define AUDIOSINK_RECORD_HANDLE                             0x00100006
#define AUDIOSOURCE_RECORD_HANDLE                           0x00100007
#define RCCONTROLLER_RECORD_HANDLE                           0x0010000C
#define DUN_RECORD_HANDLE                           0x0010000A

#define HID_RECORD_HANDLE                     0x0010000B
#define UPNP_INFOR_RECORD_HANDLE                     0x0010000D
/*
 * Protocol UUIDs
 */
#define SDP_UUID	0x0001
#define UDP_UUID	0x0002
#define RFCOMM_UUID	0x0003
#define TCP_UUID	0x0004
#define TCS_BIN_UUID	0x0005
#define TCS_AT_UUID	0x0006
#define OBEX_UUID	0x0008
#define IP_UUID		0x0009
#define FTP_UUID	0x000a
#define HTTP_UUID	0x000c
#define WSP_UUID	0x000e
#define BNEP_UUID	0x000f
#define UPNP_UUID	0x0010
#define HIDP_UUID	0x0011
#define HCRP_CTRL_UUID	0x0012
#define HCRP_DATA_UUID	0x0014
#define HCRP_NOTE_UUID	0x0016
#define AVCTP_UUID	0x0017
#define AVDTP_UUID	0x0019
#define CMTP_UUID	0x001b
#define UDI_UUID	0x001d
#define L2CAP_UUID	0x0100

/*
 * The PDU identifiers of SDP packets between client and server
 */
#define SDP_ERROR_RSP		0x01
#define SDP_SVC_SEARCH_REQ	0x02
#define SDP_SVC_SEARCH_RSP	0x03
#define SDP_SVC_ATTR_REQ	0x04
#define SDP_SVC_ATTR_RSP	0x05
#define SDP_SVC_SEARCH_ATTR_REQ	0x06
#define SDP_SVC_SEARCH_ATTR_RSP	0x07



/*
 * Standard profile descriptor identifiers; note these
 * may be identical to some of the service classes defined above
 */
#define SDP_SERVER_PROFILE_ID		SDP_SERVER_SVCLASS_ID
#define BROWSE_GRP_DESC_PROFILE_ID	BROWSE_GRP_DESC_SVCLASS_ID
#define SERIAL_PORT_PROFILE_ID		SERIAL_PORT_SVCLASS_ID
#define LAN_ACCESS_PROFILE_ID		LAN_ACCESS_SVCLASS_ID
#define DIALUP_NET_PROFILE_ID		DIALUP_NET_SVCLASS_ID
#define IRMC_SYNC_PROFILE_ID		IRMC_SYNC_SVCLASS_ID
#define OBEX_OBJPUSH_PROFILE_ID		OBEX_OBJPUSH_SVCLASS_ID
#define OBEX_FILETRANS_PROFILE_ID	OBEX_FILETRANS_SVCLASS_ID
#define IRMC_SYNC_CMD_PROFILE_ID	IRMC_SYNC_CMD_SVCLASS_ID
#define HEADSET_PROFILE_ID		HEADSET_SVCLASS_ID
#define CORDLESS_TELEPHONY_PROFILE_ID	CORDLESS_TELEPHONY_SVCLASS_ID
#define AUDIO_SOURCE_PROFILE_ID		AUDIO_SOURCE_SVCLASS_ID
#define AUDIO_SINK_PROFILE_ID		AUDIO_SINK_SVCLASS_ID
#define AV_REMOTE_TARGET_PROFILE_ID	AV_REMOTE_TARGET_SVCLASS_ID
#define ADVANCED_AUDIO_PROFILE_ID	ADVANCED_AUDIO_SVCLASS_ID
#define AV_REMOTE_PROFILE_ID		AV_REMOTE_SVCLASS_ID
#define VIDEO_CONF_PROFILE_ID		VIDEO_CONF_SVCLASS_ID
#define INTERCOM_PROFILE_ID		INTERCOM_SVCLASS_ID
#define FAX_PROFILE_ID			FAX_SVCLASS_ID
#define HEADSET_AGW_PROFILE_ID		HEADSET_AGW_SVCLASS_ID
#define WAP_PROFILE_ID			WAP_SVCLASS_ID
#define WAP_CLIENT_PROFILE_ID		WAP_CLIENT_SVCLASS_ID
#define PANU_PROFILE_ID			PANU_SVCLASS_ID
#define NAP_PROFILE_ID			NAP_SVCLASS_ID
#define GN_PROFILE_ID			GN_SVCLASS_ID
#define DIRECT_PRINTING_PROFILE_ID	DIRECT_PRINTING_SVCLASS_ID
#define REFERENCE_PRINTING_PROFILE_ID	REFERENCE_PRINTING_SVCLASS_ID
#define IMAGING_PROFILE_ID		IMAGING_SVCLASS_ID
#define IMAGING_RESPONDER_PROFILE_ID	IMAGING_RESPONDER_SVCLASS_ID
#define IMAGING_ARCHIVE_PROFILE_ID	IMAGING_ARCHIVE_SVCLASS_ID
#define IMAGING_REFOBJS_PROFILE_ID	IMAGING_REFOBJS_SVCLASS_ID
#define HANDSFREE_PROFILE_ID		HANDSFREE_SVCLASS_ID
#define HANDSFREE_AGW_PROFILE_ID	HANDSFREE_AGW_SVCLASS_ID
#define DIRECT_PRT_REFOBJS_PROFILE_ID	DIRECT_PRT_REFOBJS_SVCLASS_ID
#define REFLECTED_UI_PROFILE_ID		REFLECTED_UI_SVCLASS_ID
#define BASIC_PRINTING_PROFILE_ID	BASIC_PRINTING_SVCLASS_ID
#define PRINTING_STATUS_PROFILE_ID	PRINTING_STATUS_SVCLASS_ID
#define HID_PROFILE_ID			HID_SVCLASS_ID
#define HCR_PROFILE_ID			HCR_SCAN_SVCLASS_ID
#define HCR_PRINT_PROFILE_ID		HCR_PRINT_SVCLASS_ID
#define HCR_SCAN_PROFILE_ID		HCR_SCAN_SVCLASS_ID
#define CIP_PROFILE_ID			CIP_SVCLASS_ID
#define VIDEO_CONF_GW_PROFILE_ID	VIDEO_CONF_GW_SVCLASS_ID
#define UDI_MT_PROFILE_ID		UDI_MT_SVCLASS_ID
#define UDI_TA_PROFILE_ID		UDI_TA_SVCLASS_ID
#define AV_PROFILE_ID			AV_SVCLASS_ID
#define SAP_PROFILE_ID			SAP_SVCLASS_ID
#define PBAP_PCE_PROFILE_ID		PBAP_PCE_PROFILE_ID
#define PBAP_PSE_PROFILE_ID		PBAP_PSE_PROFILE_ID
#define PNP_INFO_PROFILE_ID		PNP_INFO_SVCLASS_ID
#define GENERIC_NETWORKING_PROFILE_ID	GENERIC_NETWORKING_SVCLASS_ID
#define GENERIC_FILETRANS_PROFILE_ID	GENERIC_FILETRANS_SVCLASS_ID
#define GENERIC_AUDIO_PROFILE_ID	GENERIC_AUDIO_SVCLASS_ID
#define GENERIC_TELEPHONY_PROFILE_ID	GENERIC_TELEPHONY_SVCLASS_ID
#define UPNP_PROFILE_ID			UPNP_SVCLASS_ID
#define UPNP_IP_PROFILE_ID		UPNP_IP_SVCLASS_ID
#define UPNP_PAN_PROFILE_ID		UPNP_PAN_SVCLASS_ID
#define UPNP_LAP_PROFILE_ID		UPNP_LAP_SVCLASS_ID
#define UPNP_L2CAP_PROFILE_ID		UPNP_L2CAP_SVCLASS_ID
#define VIDEO_SOURCE_PROFILE_ID		VIDEO_SOURCE_SVCLASS_ID
#define VIDEO_SINK_PROFILE_ID		VIDEO_SINK_SVCLASS_ID
#define VIDEO_DISTRIBUTION_PROFILE_ID	VIDEO_DISTRIBUTION_SVCLASS_ID


/*
 * The Data representation in SDP PDUs (pps 339, 340 of BT SDP Spec)
 * These are the exact data type+size descriptor values
 * that go into the PDU buffer.
 *
 * The datatype (leading 5bits) + size descriptor (last 3 bits)
 * is 8 bits. The size descriptor is critical to extract the
 * right number of bytes for the data value from the PDU.
 *
 * For most basic types, the datatype+size descriptor is
 * straightforward. However for constructed types and strings,
 * the size of the data is in the next "n" bytes following the
 * 8 bits (datatype+size) descriptor. Exactly what the "n" is
 * specified in the 3 bits of the data size descriptor.
 *
 * TextString and URLString can be of size 2^{8, 16, 32} bytes
 * DataSequence and DataSequenceAlternates can be of size 2^{8, 16, 32}
 * The size are computed post-facto in the API and are not known apriori
 */
#define SDP_DATA_NIL 	0x00
#define SDP_UINT8  		0x08
#define SDP_UINT16		0x09
#define SDP_UINT32		0x0A
#define SDP_UINT64		0x0B
#define SDP_UINT128		0x0C
#define SDP_INT8		0x10
#define SDP_INT16		0x11
#define SDP_INT32		0x12
#define SDP_INT64		0x13
#define SDP_INT128		0x14
#define SDP_UUID_UNSPEC		0x18
#define SDP_UUID16		0x19
#define SDP_UUID32		0x1A
#define SDP_UUID128		0x1C
#define SDP_TEXT_STR_UNSPEC	0x20
#define SDP_TEXT_STR8		0x25
#define SDP_TEXT_STR16		0x26
#define SDP_TEXT_STR32		0x27
#define SDP_BOOL		0x28
#define SDP_SEQ_UNSPEC		0x30
#define SDP_SEQ8		0x35
#define SDP_SEQ16		0x36
#define SDP_SEQ32		0x37
#define SDP_ALT_UNSPEC		0x38
#define SDP_ALT8		0x3D
#define SDP_ALT16		0x3E
#define SDP_ALT32		0x3F
#define SDP_URL_STR_UNSPEC	0x40
#define SDP_URL_STR8		0x45
#define SDP_URL_STR16		0x46
#define SDP_URL_STR32		0x47

#define SDP_HDR_TYPE        0
#define SDP_HDR_TRANS_ID_MS 1
#define SDP_HDR_TRANS_ID_LS 2
#define SDP_HDR_LENGTH_MS   3
#define SDP_HDR_LENGTH_LS   4
#define SDP_HDR_SIZE        5

/*data element */
#define DES_TYPE 0x6
#define DES_HDR_LEN 2//des type: 1byte + len: 1byte
#define DES_HDR_2B_LEN 3//des type: 1byte + len: 2bytes
#define UUID16_HDR 0x19
#define UUID32_HDR 0x1a
#define UUID128_HDR 0x1c


/*
 * Attribute identifier codes
 */
#define SDP_SERVER_RECORD_HANDLE		0x0000

/*
 * Possible values for attribute-id are listed below.
 * See SDP Spec, section "Service Attribute Definitions" for more details.
 */
#define SDP_ATTR_RECORD_HANDLE		0x0000
#define SDP_ATTR_SVCLASS_ID_LIST		0x0001
#define SDP_ATTR_RECORD_STATE			0x0002
#define SDP_ATTR_SERVICE_ID			0x0003
#define SDP_ATTR_PROTO_DESC_LIST		0x0004
#define SDP_ATTR_BROWSE_GRP_LIST		0x0005
#define SDP_ATTR_LANG_BASE_ATTR_ID_LIST		0x0006
#define SDP_ATTR_SVCINFO_TTL			0x0007
#define SDP_ATTR_SERVICE_AVAILABILITY		0x0008
#define SDP_ATTR_PFILE_DESC_LIST		0x0009
#define SDP_ATTR_DOC_URL			0x000a
#define SDP_ATTR_CLNT_EXEC_URL			0x000b
#define SDP_ATTR_ICON_URL			0x000c
#define SDP_ATTR_ADD_PROTO_DESC_LIST		0x000d

#define SDP_ATTR_GROUP_ID			0x0200
#define SDP_ATTR_IP_SUBNET			0x0200
#define SDP_ATTR_VERSION_NUM_LIST		0x0200
#define SDP_ATTR_SVCDB_STATE			0x0201

#define SDP_ATTR_SERVICE_VERSION		0x0300
#define SDP_ATTR_EXTERNAL_NETWORK		0x0301
#define SDP_ATTR_SUPPORTED_DATA_STORES_LIST	0x0301
#define SDP_ATTR_FAX_CLASS1_SUPPORT		0x0302
#define SDP_ATTR_REMOTE_AUDIO_VOLUME_CONTROL	0x0302
#define SDP_ATTR_FAX_CLASS20_SUPPORT		0x0303
#define SDP_ATTR_SUPPORTED_FORMATS_LIST		0x0303
#define SDP_ATTR_FAX_CLASS2_SUPPORT		0x0304
#define SDP_ATTR_AUDIO_FEEDBACK_SUPPORT		0x0305
#define SDP_ATTR_NETWORK_ADDRESS		0x0306
#define SDP_ATTR_WAP_GATEWAY			0x0307
#define SDP_ATTR_HOMEPAGE_URL			0x0308
#define SDP_ATTR_WAP_STACK_TYPE			0x0309
#define SDP_ATTR_SECURITY_DESC			0x030a
#define SDP_ATTR_NET_ACCESS_TYPE		0x030b
#define SDP_ATTR_MAX_NET_ACCESSRATE		0x030c
#define SDP_ATTR_IP4_SUBNET			0x030d
#define SDP_ATTR_IP6_SUBNET			0x030e
#define SDP_ATTR_SUPPORTED_CAPABILITIES		0x0310
#define SDP_ATTR_SUPPORTED_FEATURES		0x0311
#define SDP_ATTR_SUPPORTED_FUNCTIONS		0x0312
#define SDP_ATTR_TOTAL_IMAGING_DATA_CAPACITY	0x0313
#define SDP_ATTR_SUPPORTED_REPOSITORIES		0x0314

#define SDP_ATTR_SPECIFICATION_ID		0x0200
#define SDP_ATTR_VENDOR_ID			0x0201
#define SDP_ATTR_PRODUCT_ID			0x0202
#define SDP_ATTR_VERSION			0x0203
#define SDP_ATTR_PRIMARY_RECORD			0x0204
#define SDP_ATTR_VENDOR_ID_SOURCE		0x0205

#define SDP_ATTR_HID_DEVICE_RELEASE_NUMBER	0x0200
#define SDP_ATTR_HID_PARSER_VERSION		0x0201
#define SDP_ATTR_HID_DEVICE_SUBCLASS		0x0202
#define SDP_ATTR_HID_COUNTRY_CODE		0x0203
#define SDP_ATTR_HID_VIRTUAL_CABLE		0x0204
#define SDP_ATTR_HID_RECONNECT_INITIATE		0x0205
#define SDP_ATTR_HID_DESCRIPTOR_LIST		0x0206
#define SDP_ATTR_HID_LANG_ID_BASE_LIST		0x0207
#define SDP_ATTR_HID_SDP_DISABLE		0x0208
#define SDP_ATTR_HID_BATTERY_POWER		0x0209
#define SDP_ATTR_HID_REMOTE_WAKEUP		0x020a
#define SDP_ATTR_HID_PROFILE_VERSION		0x020b
#define SDP_ATTR_HID_SUPERVISION_TIMEOUT	0x020c
#define SDP_ATTR_HID_NORMALLY_CONNECTABLE	0x020d
#define SDP_ATTR_HID_BOOT_DEVICE		0x020e


#define SDP_PRIMARY_LANG_BASE 		0x0100

#define SDP_ATTR_SVCNAME_PRIMARY	(0x0000 + SDP_PRIMARY_LANG_BASE)
#define SDP_ATTR_SVCDESC_PRIMARY	(0x0001 + SDP_PRIMARY_LANG_BASE)
#define SDP_ATTR_PROVNAME_PRIMARY	(0x0002 + SDP_PRIMARY_LANG_BASE)

/*
  *  PSM define. perhaps we should get it in "l2cap.h"
  */
#define PSM_SDP            0x0001
#define PSM_RFCOMM     0x0003
#define PSM_AVDTP      0x0019
#define PSM_AVCTP      0x0017
#define PSM_HIDC        0x0011
#define PSM_HIDI        0x0013
#define PSM_UPNP        0x0015

#define SDP_PROTOCOL SDP_UUID	

#define UCHAR_MAX 65535

/*
  **************************************************************
  *		Define data structure
  **************************************************************
  */



typedef uint32 uuid32_t;
typedef uint128_t uuid128_t;
//typedef unsigned char byte;


typedef enum{
    SDP_LINK_INITIAL = 0,    //the init state;
    SDP_LINK_CONN, 		//connect the remote server successfully
    SDP_LINK_DISCONN,	//disconnect
    SDP_LINK_CONN_REJ, 	//connect request rejected by the remote server.
    SDP_LINK_TIMEOUT,	//connection timeout
    SDP_LINK_CONN_REQ, //received a remote connect request
    SDP_LINK_ACCEPT		//accept a remote connection
}sdp_link_status_t;



typedef struct{
    uint8 idle;// should be initialized to 1
//    OS_EVENT   *sem;
//    OS_EVENT   *dataQueue;
//    void * sdp_data_que_buf[MAX_SDP_QUE_SIZE];
}sdp_control_block_t;


/*
  *Description:
  *The socket use to connect with remote.
  */
typedef struct {
    int sock;
    int server;// 1: the sock is used by sdp server to handle request; 0 means the sock is used by client to send request
    sdp_link_status_t status;
    uint16 tid;
}sdp_socket_t;


/*
 * Common definitions for attributes in the SDP.
 * Should the type of any of these change, you need only make a change here.
 */
#ifndef OS_LINUX
typedef struct{
	uint8  data[8];
}int64_t;

typedef struct{
	uint8 data[8];
}uint64_t;
#endif



typedef struct uuid16_list{
    uint8 num;//uuid number
    uuid16_t uuids[12];//warning: if the array len changed , record database should be modified also.
}uuid16_list_t;




typedef struct{
    int type;
    sdp_socket_t *sdp_sock;
    struct sk_buff *buf;
}sdp_msg_t;


typedef struct service_search_request_struct
{
  unsigned int max_rec_cnt;
  unsigned int service_class_cnt;
  unsigned int service_class_list[12];
}service_search_request_t;


typedef struct sdp_search_handler
{
  char *search_name;
  char *search_attr;
  char *search_val;
} sdp_search_handler;


typedef struct rec_hdl_search_struct
{
  char *uuid;
  unsigned int tmp_hdl;
  unsigned int hdl_list_len;
  unsigned int hdl_list_max;
  unsigned int *hdl_list;
} rec_hdl_search_struct;


typedef struct sdp_data_struct sdp_data_t;
struct sdp_data_struct {
    uint8 dtd;
    uint16 attrId;
    union {
    	int8    int8;
    	int16   int16;
    	int32   int32;
    	int64_t   int64;
    	uint128_t int128;
    	uint8   uint8;
    	uint16  uint16;
    	uint32  uint32;
    	uint64_t  uint64;
    	uint128_t uint128;
    	uuid_t    uuid;
    	char     *str;
    	sdp_data_t *dataseq;
    } val;
    sdp_data_t *next;
    int unitSize;
};

typedef struct {
    uint8 *data;
    uint32 data_size;
    uint32 buf_size;
} sdp_buf_t;


#ifdef OS_LINUX 
typedef struct {
	uint8  pdu_id;
	uint16 tid;
	uint16 plen;
}__attribute__ ((packed)) sdp_pdu_hdr_t;
#else
typedef __packed struct {
	uint8  pdu_id;
	uint16 tid;
	uint16 plen;
} sdp_pdu_hdr_t;
#endif

#ifdef OS_LINUX
typedef struct {
	uint8 length;
	unsigned char data[16];
} __attribute__ ((packed)) sdp_cstate_t;//compile acording the accord byte space
#else
typedef __packed struct {
	uint8 length;
	unsigned char data[16];
}sdp_cstate_t;
#endif

typedef struct database_query_struct{
  sdp_socket_t *sdp_sock;
  unsigned short pkt_type;
} database_query_struct_t;

typedef struct cont_state_struct{
  uint8 len;
  uint8 *data;
} cont_state_struct_t;

typedef struct service_search_struct{
  sdp_socket_t *sdp_sock;
  cont_state_struct_t con_state;
  uint32 max_rec_cnt;
  uint32 service_class_cnt;
//  uuid16_t service_class_list[12];
  uuid16_t *service_class_list;  
}service_search_struct_t;

typedef struct service_attr_struct{
  sdp_socket_t *sdp_sock;
  cont_state_struct_t con_state;  
  uint32 max_attr_byte_cnt;
  uint32 max_buffer_size;//maximum rsp buffer size 
  uint32 rec_hdl;
  uint32 attr_cnt;
  uint32 *attr_list;
} service_attr_struct_t;


typedef struct service_search_attr_struct{
  service_attr_struct_t service_attr;
  uint32 service_class_cnt;
  uuid16_t  *service_class_list;
} service_search_attr_struct_t;

typedef struct{
    uint32 handle;
    uint8   num;
    uuid16_t uuids[20];
}sdp_uuids_in_record_t;

/*
  * sdp database structure
  */
typedef struct{
    uint16 attrId;
    uint32 record_handle;
}attr_serviceRecordHandle_t;

typedef struct{
    uint16 attrId;
    uuid16_list_t uuid_list;
}attr_serviceClassIdList_t;

typedef struct{
    uuid16_t uuid;
    uint16 para;
}protocolValue_t;

typedef struct{
    uint16 attrId;
    uint8 protocol_num;
    protocolValue_t protocols[5];
}attr_protocolDescriptorList_t;

typedef struct{
    uint16 attrId;
    uuid16_t uuid;
}attr_browseGroupList_t;

typedef struct{
    uint16 attrId;
    uint8 value;
}attr_serviceAvailability_t;

typedef struct{
    uuid16_t profileId;
    uint16 version;
}profile_t;

typedef struct{
    uint16 attrId;
    profile_t profile;
}attr_bluetoothProfileDescriptorList_t;

typedef struct{
    uint16 attrId;
    uint16 langId;
    uint16 encoding;
    uint16 langBaseId;
} attr_langAttrList_t;

typedef struct{
    uint16 attrId;
    uint16 langId;
    uint16 langBaseId;
} attr_langBaseList_t;

typedef struct{
    uint16 attrId;
    uint8 value;
} attr_network_t;

typedef struct{
    uint16 attrId;
    uint16 feature;
}attr_supportedFeature_t;

typedef struct{
    uint16 attrId;
    uint8 count;
    uint8 format[7];
}attr_supportedFormat_t;

#define DATA_LEN 90
typedef struct{
    uint8 classType;
    uint8 classDataLen;
    uint8 classData[DATA_LEN];
}hid_class_descriptor_t;
typedef struct{
    uint16 attrId;
    hid_class_descriptor_t descriptor;
}attr_hidDescriptorList_t;

typedef struct{
    uint16 attrId;
    uint8  len;
    uint8  str[9];
} attr_serviceName_t;
typedef struct{
    uint16 attrId;
    uint8  len;
    uint8  str[16];
} attr_serviceDesc_t;
typedef struct{
    uint16 attrId;
    uint8  len;
    uint8  str[9];
} attr_providerName_t;

typedef struct{
    uint16 attrId;
    uint16 value;
}attr_versionNumList_t;

typedef struct{
    uint16 attrId;
    uint16 value;
}attr_parseVersion_t;
typedef struct{
    uint16 attrId;
    uint8 value;
}attr_deviceSubclass_t;
typedef struct{
    uint16 attrId;
    uint8 value;
}attr_countryCode_t;
typedef struct{
    uint16 attrId;
    uint8 value;
}attr_virtualCable_t;
typedef struct{
    uint16 attrId;
    uint8 value;
}attr_reconnInitiate_t;
typedef struct{
    uint16 attrId;
    uint8 value;
}attr_normConnectable_t;
typedef struct{
    uint16 attrId;
    uint8 value;
}attr_bootDevice_t;

typedef struct{
    uint16 attrId;
    uint16 value;
}attr_profileVersion_t;

typedef struct{
    uint32 handle;
    uuid16_list_t uuid_list;
    uint8 attr_id_num;
    uint16 attr_id_list[18];
    attr_serviceRecordHandle_t          attr_recordHandle;//attrid: 00000
    attr_serviceClassIdList_t              attr_serviceClassIdList;//attrid:0001
    attr_protocolDescriptorList_t         attr_protocolDesciptorList;//attrid:0004	
//    attr_browseGroupList_t                attr_browseGroupList;	//attrid:0005
    attr_serviceAvailability_t              attr_serviceAvailability;//attrid:0008
    attr_bluetoothProfileDescriptorList_t attr_bluetoothProfile;//0x14
    attr_langAttrList_t                  attr_lang;
    attr_langBaseList_t                  attr_langBase;
    attr_network_t                       attr_network;
    attr_supportedFeature_t                attr_supportedFeature; 
    attr_supportedFormat_t                attr_supportedFormat;
    attr_serviceName_t                    attr_svcName;
    attr_providerName_t                   attr_provName;
    attr_protocolDescriptorList_t    attr_protocolDesciptorList2;
    attr_parseVersion_t              attr_parseVersion;
    attr_profileVersion_t            attr_profileVersion;
    attr_countryCode_t               attr_countryCode;
    attr_deviceSubclass_t            attr_deviceSubclass;
    attr_reconnInitiate_t            attr_reconnInitiate;
    attr_normConnectable_t           attr_normConnectable;
    attr_virtualCable_t              attr_virtualCable;
    attr_bootDevice_t                attr_bootDevice;
}sdp_record_t;



#define SDP_FREE(ptr)  free_buffer((uint8*)(ptr))


#define SDPDBG DEBUG_INFO

#define SDPERR DEBUG_ERROR

#define GET_TYPE(ch) (((ch) >> 3) & 0x1f)
#define GET_SIZE(ch) ((ch) & 0x7)
#define CHAR2INT10(c1,c0) (((uint32)((c1) & 0x03) << 8) + \
			   (uint32)((c0) & 0xff))

#define CHAR2INT12(c1,c0) (((uint32)((c1) & 0x0f) << 8) + \
			   (uint32)((c0) & 0xff))

#define CHAR2INT16(c1,c0) (((uint32)((c1) & 0xff) << 8) + \
			   (uint32)((c0) & 0xff))

#define CHAR2INT32(c3,c2,c1,c0) (((uint32)((c3) & 0xff) << 24) + \
				 ((uint32)((c2) & 0xff) << 16) + \
				 ((uint32)((c1) & 0xff) << 8) + \
				 (uint32)((c0) &0xff))


#define SHORT2CHAR_MS(x) (((x) >> 8) & 0xff)
#define SHORT2CHAR_LS(x) ((x) & 0xff)

#define FIRST_VALID_CHAR(x) (((x)>0x20) && ((x)<0x7f))

#define NWRITE_INT8(offset, p, data)                        \
  ( *(INT8U *)(((INT8U *)p) + offset) = (INT8U)data )

#define NWRITE_INT16(data, p )                       \
 NWRITE_INT8(0, p, ((((INT16U)data) >> 8) & 0xFF));             \
 NWRITE_INT8(1, p, (((INT16U)data) & 0xFF))

   
#define NWRITE_INT32(data, p)						\
	NWRITE_INT8( 3, p,  ( data & 0x000000FF) );  \
	NWRITE_INT8( 2, p,  ( (data>>8) & 0x000000FF) );  \
	NWRITE_INT8( 1, p,  ( (data>>16) & 0x000000FF) );  \
	NWRITE_INT8( 0, p,  ( (data>>24) & 0x000000FF) ) 
	

/* convert parameters into single data*/

#define NREAD_INT8(offset, p)                              \
  ( *(INT8U *)(((INT8U *)p) + offset) )

#define NREAD_INT16(p)                             \
  (INT16U )(NREAD_INT8(1, p) +                        \
         (NREAD_INT8(0, p) << 8))
         
         
  		   
#define NREAD_INT32(p)							\
	(INT32U) ((NREAD_INT8(3,p) +               \
	         (NREAD_INT8(2,p)<<8) +               \
		  (NREAD_INT8(1,p) <<16) +               \
	         (NREAD_INT8(0,p) <<24)) )

#define HWRITE_INT8(offset, p, data)                        \
  ( *(INT8U *)(((INT8U *)p) + offset) = (INT8U)data )

#define HWRITE_INT16( data, p)                       \
  HWRITE_INT8(0,  p, (((uint16)data) & 0xFF));              \
  HWRITE_INT8(1, p, ((((uint16)data) >> 8) & 0xFF))

   
#define HWRITE_INT32( data, p)						\
	HWRITE_INT8( 0,   p,  ( data & 0x000000FF) );  \
	HWRITE_INT8( 1, p,  ( (data>>8) & 0x000000FF) );  \
	HWRITE_INT8( 2, p,  ( (data>>16) & 0x000000FF) );  \
	HWRITE_INT8( 3, p,  ( (data>>24) & 0x000000FF) ) 
	


/* convert parameters into single data*/

#define HREAD_INT8(offset, p)                              \
  ( *(INT8U *)(((INT8U*)p) + offset) )

#define HREAD_INT16(p)                             \
  (INT16U )(HREAD_INT8(0, p) +                        \
         (HREAD_INT8(1, p) << 8))
         
         
  		   
#define HREAD_INT32(p)							\
	(INT32U) ((HREAD_INT8(0, p) +               \
	         (HREAD_INT8(1,p)<<8) +               \
			 (HREAD_INT8(2,p) <<16) +               \
	         (HREAD_INT8(3,p) <<24)) )
	         

/* Byte order conversions */

extern  void ntoh64(uint64_t *src, uint64_t *dst);
#define ntohs(A)  ((((uint16)(A) & 0xff00) >> 8) | \
                   (((uint16)(A) & 0x00ff) << 8))
#define ntohl(A)  ((((uint32)(A) & 0xff000000) >> 24) | \
                   (((uint32)(A) & 0x00ff0000) >> 8)  | \
                   (((uint32)(A) & 0x0000ff00) << 8)  | \
                   (((uint32)(A) & 0x000000ff) << 24))

extern  void ntoh128(uint128_t *src, uint128_t *dst);


#define  htons(x)              ntohs(x)
#define  htonl(x)              ntohl(x)
#define  hton64         ntoh64
#define  hton128(x,y)    ntoh128(x,y)

#define SDP_IS_UUID(x) ((x) == SDP_UUID16 || (x) == SDP_UUID32 || (x) ==SDP_UUID128)


/*
  *********************************************************************************************************
  *	Function prototype
  *********************************************************************************************************
  */

sdp_socket_t *sdp_socket(int server);

sdp_socket_t *sdp_findSockInList(int sock);

sdp_status_t sdp_close(sdp_socket_t *sdp_sock);

sdp_status_t sdp_send(sdp_socket_t *sdp_sock, void *buf, int bufLen);

sdp_status_t sdp_connect(sdp_socket_t *sdp_sock,  const struct bdaddr_t *dst);

int    sdp_recv(sdp_socket_t* sdp_sock, char *buf, int bufLen);

void sdp_parseData(sdp_msg_t *sdp_msg);

int sdp_getSize(uint8 *data, int *new_pos);

#endif
