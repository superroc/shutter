/*
 ****************************************************************************************
 * $Workfile: hci.h $
 *
 * Copyright (C) unisim Semiconductors Corp. 2005
 *
 * Project: Bluetooth Software
 *
 * Description:
 *  define the HCI layer
 *
 * Revision History:
 * 
 * 1    05/27/05 initial file
 * 
 *
 ****************************************************************************************
 */
#ifndef HCI_H
#define HCI_H

#include "stm32f10x_type.h"

//#define HCI_AFH  //added by cliff 02/27/2006

#define HCI_MAX_PARAM_SIZE      255
#define HCI_COMMAND_FIFO_SIZE   258                     /* size of the transmit fifo    */ 
/* Packet types :                                                                       */ 
#define HCI_CMD_PACKET          0x01
#define HCI_ACL_PACKET          0x02
#define HCI_SCO_PACKET          0x03
#define HCI_EVT_PACKET          0x04
#define HCI_ACL_PACKET_TX       0x12                    /* internal: used for trace only */ 

/* HCI_COMMAND_STATUS_EVT_FIXED_SIZE = evtcode (1) + len (1) + status (1) + num packet (1) + opcode (2) */ 
#define HCI_COMMAND_STATUS_EVT_FIXED_SIZE   6

/* HCI_COMMAND_COMP_EVT_FIXED_SIZE = evtcode (1) + len (1) + num packet (1) + opcode (2) + ... */ 
#define HCI_COMMAND_COMP_EVT_FIXED_SIZE   5

/* HCI_COMMAND_EVT_FIXED_SIZE = evtcode (1) + len (1) + ... */ 
#define HCI_COMMAND_EVT_FIXED_SIZE   2

/* HCI_COMMAND_EVT_FIXED_SIZE = evtcode (1) + len (1) + msgclass(1) + msgtag(1) + ... */ 
#define HCI_DEBUG_EVT_FIXED_SIZE     4

#define HCI_DEBUG_GROUP_CODE         0x3F
#define HCI_DEBUG_PRINT_EVENT_CLASS  0x00

/* this variable should be use to perform a control flow for HCI commands               */ 
#define HCI_NUM_COMMAND_PACKETS      1






/******************************************************************************************
   OGF opcodes group define 
   hence the values 
******************************************************************************************/
#define HCI_OGF_BIT_OFFSET                ((INT8U) 10) /* Number of bit shifts */


#define HCI_OPCODE_MASK                       0x03FF


/******************************************************************************************
   OCF opcode defines 
******************************************************************************************/

/******************************************************************************************
   OCF opcode defines - Link Control Commands  (OGF: 0x01) 
******************************************************************************************/
//#define HCI_INQUIRY                     	  0x0401 
#define HCI_INQUIRY_CANCEL              	  0x0402
#define HCI_PERIODIC_INQUIRY_MODE      		  0x0403
#define HCI_EXIT_PERIODIC_INQUIRY_MODE 		  0x0404
//#define HCI_CREATE_CONNECTION          		  0x0405
//#define HCI_DISCONNECT              		  0x0406
#define HCI_CREATE_CONNECTION_CANCEL          0x0408
//#define HCI_ACCEPT_CONNECTION_REQ   		  0x0409
//#define HCI_REJECT_CONNECTION_REQ 		      0x040A
//#define HCI_LINK_KEY_REQ_REPLY    		      0x040B
//#define HCI_LINK_KEY_REQ_NEG_REPLY  		  0x040C
//#define HCI_PIN_CODE_REQ_REPLY        		  0x040D
//#define HCI_PIN_CODE_REQ_NEG_REPLY  		  0x040E
//#define HCI_AUTH_REQ        			      0x0411
//#define HCI_SET_CONN_ENCRYPTION			      0x0413
#define HCI_CHANGE_CONN_LINK_KEY 		      0x0415
#define HCI_MASTER_LINK_KEY 			      0x0417
//#define HCI_REMOTE_NAME_REQ  			      0x0419
//#define HCI_REMOTE_NAME_CANCEL  			  0x041A
#define HCI_READ_REMOTE_SUP_FEATURES		  0x041B
#define HCI_READ_REMOTE_VER_INFO 		      0x041D
#define HCI_READ_CLOCK_OFFSET       		  0x041F
#define HCI_READREMOTE_EXT_FEATURES           0x041C
#define HCI_READ_LMP_HANDLE                   0x0420
//#define HCI_SETUP_SYNC_CONN		              0x0428
//#define HCI_ACCEPT_SYNC_CONN			      0x0429
//#define HCI_REJECT_SYNC_CONN			      0x042A

/******************************************************************************************
   OCF opcode defines - Link Policy Commands  (OGF 0x02) 
 ******************************************************************************************/
#define HCI_HOLD_MODE                             0x0801
#define HCI_PARK_MODE                             0x0805
#define HCI_EXIT_PARK_MODE                        0x0806
#define HCI_QOS_SETUP                             0x0807
#define HCI_ROLE_DISCOVERY                        0x0809
#define HCI_SWITCH_ROLE                           0x080B
#define HCI_READ_LP_SETTINGS                      0x080C           //LP:LINK POLICY
#define HCI_READ_DEFAULT_LP_SETTINGS              0x080E
#define HCI_WRITE_DEFAULT_LP_SETTINGS             0x080F
#define HCI_FLOW_SPECIFICATION                    0x0810

/******************************************************************************************
 OCF opcode defines -Host Controller and Baseband Commands (OGF 0x03)
******************************************************************************************/ 
//#define HCI_SET_EVENT_MASK            		  0x0C01
#define HCI_RESET                    		    0x0C03
#define HCI_SET_EVENT_FILTER          		  0x0C05
#define HCI_FLUSH                     		  0x0C08
#define HCI_READ_PIN_TYPE                 	  0x0C09
#define HCI_WRITE_PIN_TYPE             		  0x0C0A
#define HCI_CREATE_NEW_UNIT_KEY        		  0x0C0B
//#define HCI_DELETE_STORED_LINK_KEY   		  0x0C12
//#define HCI_WRITE_LOCAL_NAME                  0x0C13
#define HCI_READ_LOCAL_NAME            		  0x0C14
#define HCI_READ_CONN_ACCEPT_TIMEOUT   		  0x0C15
#define HCI_WRITE_CONN_ACCEPT_TIMEOUT    	  0x0C16
#define HCI_READ_PAGE_TIMEOUT          		  0x0C17
#define HCI_WRITE_PAGE_TIMEOUT         		  0x0C18
#define HCI_READ_SCAN_ENABLE            	  0x0C19
//#define HCI_WRITE_SCAN_ENABLE          	 	  0x0C1A
#define HCI_READ_PAGESCAN_ACTIVITY       	  0x0C1B
//#define HCI_WRITE_PAGESCAN_ACTIVITY       	  0x0C1C
#define HCI_READ_INQUIRYSCAN_ACTIVITY   	  0x0C1D
//#define HCI_WRITE_INQUIRYSCAN_ACTIVITY   	  0x0C1E
#define HCI_READ_AUTH_ENABLE 	           	  0x0C1F
//#define HCI_WRITE_AUTH_ENABLE         		  0x0C20
#define HCI_READ_ENCRY_MODE           		  0x0C21
#define HCI_WRITE_ENCRY_MODE     	       	  0x0C22
#define HCI_READ_CLASS_OF_DEVICE      	  	  0x0C23
//#define HCI_WRITE_CLASS_OF_DEVICE     	 	  0x0C24
#define HCI_READ_VOICE_SETTING        		  0x0C25
#define HCI_WRITE_VOICE_SETTING        		  0x0C26
#define HCI_READ_AUTO_FLUSH_TIMEOUT    		  0x0C27
#define HCI_WRITE_AUTO_FLUSH_TIMEOUT   		  0x0C28
#define HCI_READ_NUM_BCAST_RETRANSMIT    	  0x0C29
#define HCI_WRITE_NUM_BCAST_RETRANSMIT    	  0x0C2A
#define HCI_READ_HOLD_MODE_ACTIVITY   		  0x0C2B
#define HCI_WRITE_HOLD_MODE_ACTIVITY    	  0x0C2C
#define HCI_READ_TX_POWER_LEVEL       		  0x0C2D
#define HCI_READ_SYNC_FLOW_CON_ENABLE   	  0x0C2E
#define HCI_WRITE_SYNC_FLOW_CON_ENABLE 		  0x0C2F
#define HCI_SET_HCTOHOST_FLOW_CONTROL   	  0x0C31
#define HCI_HOST_BUFFER_SIZE          		  0x0C33
#define HCI_HOST_NUM_COMPLETED_PACKETS		  0x0C35
#define HCI_READ_LINK_SUPERV_TIMEOUT  		  0x0C36
//#define HCI_WRITE_LINK_SUPERV_TIMEOUT 		  0x0C37
#define HCI_READ_NUM_SUPPORTED_IAC    		  0x0C38
#define HCI_READ_CURRENT_IAC_LAP      		  0x0C39
#define HCI_WRITE_CURRENT_IAC_LAP     		  0x0C3A
#define HCI_READ_PAGESCAN_PERIOD_MODE 		  0x0C3B
#define HCI_WRITE_PAGESCAN_PERIOD_MODE 		  0x0C3C

#define HCI_READ_PAGESCAN_MODE 		          0x0C3D
#define HCI_WRITE_PAGESCAN_MODE 		      0x0C3E

#define SET_AFH_HOST_CHL_CLASSFICAT    		  0x0C3F
#define HCI_READ_INQUIRYSCAN_TYPE        	  0x0C42
#define HCI_WRITE_INQUIRYSCAN_TYPE       	  0x0C43
#define HCI_READ_INQUIRY_MODE      		      0x0C44
#define HCI_WRITE_INQUIRY_MODE			      0x0C45
#define HCI_READ_PAGESCAN_TYPE 			      0x0C46
#define HCI_WRITE_PAGESCAN_TYPE 		 	  0x0C47
#define HCI_READ_AFH_CHL_ASSESS_MODE 		  0x0C48
#define HCI_WRITE_AFH_CHL_ASSESS_MODE 		  0x0C49

/******************************************************************************************
 OCF opcode defines -Information Parameters (OGF  0x04)
******************************************************************************************/ 
#define HCI_READ_LOCAL_VER_INFO        		  0x1001
#define HCI_READ_LOCAL_SUP_COMMANDS   		  0x1002
#define HCI_READ_LOCAL_SUP_FEATURES    		  0x1003
#define HCI_READ_LOCAL_EXT_FEATURES    		  0x1004
#define HCI_READ_BUFFER_SIZE           		  0x1005
#define HCI_READ_BD_ADDR                	  0x1009

/******************************************************************************************
 OCF opcode defines -Status Parameters (0GF 0X05)
******************************************************************************************/ 
#define HCI_READ_FAILED_CONTACT_COUNT              0x1401
#define HCI_RESET_FAILED_CONTACT_COUNT             0x1402
#define HCI_READ_LINK_QUALITY                      0x1403
#define HCI_READ_RSSI                              0x1405
#define HCI_READ_AFH_CHANNEL_MAP                   0x1406
#define HCI_READ_CLOCK                             0x1407

/******************************************************************************************
 *OCF opcode defines -Testing Commands (OGF 0X06)
******************************************************************************************/ 
#define HCI_READ_LOOPBACK_MODE           	  0x1801
#define HCI_WRITE_LOOPBACK_MODE         	  0x1802
#define HCI_ENABLE_DUT_MODE  		          0x1803

/******************************************************************************************
 *OCF opcode defines -Vendor Commands (OGF 0xff)
******************************************************************************************/ 

/*
#define HCI_READ_LMP_PARAM				 0x2001 
#define HCI_SET_AFH                      0x2002
#define HCI_SET_BD_ADDR					 0x2004
#define HCI_PRJ_VERSION					 0x2005	
#define HCI_GET_PKT_STATICS				 0x2006
#define HCI_READ_MEMORY                  0x2007
#define HCI_WRITE_MEMORY                 0x2008
#define HCI_READ_HW_REGISTER             0x2009 
#define HCI_WRITE_HW_REGISTER            0x200A
#define HCI_TEST_CONTROL				 0x200B
#define HCI_SEND_PDU 				 	 0x2010
#define HCI_SET_SCO_CHANNEL				 0x2011
#define HCI_SET_ESCO_CHANNEL             0x2012
#define HCI_DBG_OPCODE					 0x203f
#define HCI_SET_UART_BAUD_RATE           0x2013
#define HCI_SET_UART_PORT                0x2014
#define HCI_SET_CLOCK                    0x2015
#define HCI_GET_PKTS_ERR                 0x2016
#define HCI_DEEP_SLEEP			 		 0x2019
#define HCI_SET_SCOOVER_TYPE             0x201A
#define HCI_GET_SCOOVER_TYPE             0x201B
*/

#define HCI_READ_LMP_PARAM				 0xFC01 
#define HCI_SET_AFH                      0xFC02
//#define HCI_SET_BD_ADDR					 0xFC04
#define HCI_PRJ_VERSION					 0xFC05	
#define HCI_GET_PKT_STATICS				 0xFC06
#define HCI_READ_MEMORY                  0xFC07
//#define HCI_WRITE_MEMORY                 0xFC08
#define HCI_READ_HW_REGISTER             0xFC09 
#define HCI_WRITE_HW_REGISTER            0xFC0A
#define HCI_TEST_CONTROL				 0xFC0B
#define HCI_SEND_PDU 				 	 0xFC10
#define HCI_SET_SCO_CHANNEL				 0xFC11
#define HCI_SET_ESCO_CHANNEL             0xFC12
#define HCI_DBG_OPCODE					 0xFC3f
#define HCI_SET_UART_BAUD_RATE           0xFC77
#define HCI_SET_UART_PORT                0xFC14
#define HCI_SET_CLOCK                    0xFC15
#define HCI_GET_PKTS_ERR                 0xFC16
#define HCI_DEEP_SLEEP			 		 0xFC19
//#define HCI_SET_SCOOVER_TYPE             0xFC1A
#define HCI_SET_SCOOVER_TYPE             0xFC04
//#define HCI_GET_SCOOVER_TYPE             0xFC1B
#define HCI_GET_SCOOVER_TYPE             0xFC03

#define HCI_CONFIG_WRITE             0xFC1C
#define HCI_CONFIG_READ             0xFC1D
#define HCI_CONFIG_FIXED_FREQ             0xFC1E
#define HCI_CONFIG_HOP_FREQ               0xFC1F
#define HCI_GET_IVT_SECCODE             0xfc20
#define HCI_SET_IVT_SECCODE             0xfc21
#define HCI_SET_CLK_DBGMODE				0xfc22
#define HCI_SET_SLAVE_TEST_MODE         0xfc23



#define HCI_OGF_VENDOR  0x3f
#define HCI_OCF_CONFIG_WRITE     0x1C
#define HCI_OCF_CONFIG_READ      0x1D



#define HCI_EVTCODE_VENDOR  0xff


#define HCI_GET_OGF(val)  ((unsigned int)val>>10)
#define HCI_GET_OCF(val)  ((unsigned int)val & 0x3ff)
#define HCI_GET_OPCODE(ogf,ocf)  ((ogf<<10) + ocf)

#ifdef HCI_AFH
#define HCI_AFHEvt_Code     0xFD
#define HCI_ChannelEvt_Code  0xFE
#endif





/******************************************************************************************
 *OCF opcode defines -Debug Commands (OGF )
******************************************************************************************/ 



/* LM/HCI Errors                                                                        */


/****************************************************************************

   HCI_COMMAND, Define Parammeter Length  
 *****************************************************************************/
#define HCI_INQUIRY_PARAM_LEN                    ((INT8U) 5)
#define HCI_INQUIRY_CANCEL_PARAM_LEN             ((INT8U) 0)
#define HCI_PERIODIC_INQUIRY_MODE_PARAM_LEN      ((INT8U) 9)
#define HCI_EXIT_PERIODIC_INQUIRY_MODE_PARAM_LEN ((INT8U) 0)
#define HCI_CREATE_CONNECTION_PARAM_LEN          ((INT8U) 13)
#define HCI_DISCONNECT_PARAM_LEN                 ((INT8U) 3)
#define HCI_ADD_SCO_CONNECTION_PARAM_LEN         ((INT8U) 4)
#define HCI_ACCEPT_CONNECTION_REQ_PARAM_LEN      ((INT8U) 7)
#define HCI_REJECT_CONNECTION_REQ_PARAM_LEN      ((INT8U) 7)
#define HCI_LINK_KEY_REQ_REPLY_PARAM_LEN         ((INT8U) 22)
#define HCI_LINK_KEY_REQ_NEG_REPLY_PARAM_LEN     ((INT8U) 6)
#define HCI_PIN_CODE_REQ_REPLY_PARAM_LEN         ((INT8U) 23)
#define HCI_PIN_CODE_REQ_NEG_REPLY_PARAM_LEN     ((INT8U) 6)
#define HCI_CHANGE_CONN_PKT_TYPE_PARAM_LEN       ((INT8U) 4)
#define HCI_AUTH_REQ_PARAM_LEN                   ((INT8U) 2)
#define HCI_SET_CONN_ENCRYPTION_PARAM_LEN        ((INT8U) 3)
#define HCI_CHANGE_CONN_LINK_KEY_PARAM_LEN       ((INT8U) 2)
#define HCI_MASTER_LINK_KEY_PARAM_LEN            ((INT8U) 1)
#define HCI_REMOTE_NAME_REQ_PARAM_LEN            ((INT8U) 10)
#define HCI_REMOTE_NAME_REQ_CANCEL_LEN			 ((INT8U) 6)
#define HCI_READ_REMOTE_SUPP_FEATURES_PARAM_LEN  ((INT8U) 2)
#define HCI_READ_REMOTE_SUPP_FEATURES_EXT_LEN    ((INT8U) 3)
#define HCI_READ_REMOTE_VER_INFO_PARAM_LEN       ((INT8U) 2)
#define HCI_READ_CLOCK_OFFSET_PARAM_LEN          ((INT8U) 2)
#define HCI_READ_LMP_HANDLE_PARAM_LEN          	 ((INT8U) 2)
#define HCI_SETUP_SYNC_CONNECTION_PARAM_LEN      ((INT8U) 17)
#define HCI_ACCEPT_SYNC_CONNECTION_PARAM_LEN     ((INT8U) 21)
#define HCI_REJECT_SYNC_CONNECTION_PARAM_LEN	 ((INT8U) 7)

#define HCI_HOLD_MODE_PARAM_LEN                  ((INT8U) 6)
#define HCI_SNIFF_MODE_PARAM_LEN                 ((INT8U) 10)
#define HCI_EXIT_SNIFF_MODE_PARAM_LEN            ((INT8U) 2)
#define HCI_PARK_MODE_PARAM_LEN                  ((INT8U) 6)
#define HCI_EXIT_PARK_MODE_PARAM_LEN             ((INT8U) 2)
#define HCI_QOS_SETUP_PARAM_LEN                  ((INT8U) 20)
#define HCI_ROLE_DISCOVERY_PARAM_LEN             ((INT8U) 2)
#define HCI_SWITCH_ROLE_PARAM_LEN                ((INT8U) 7)
#define HCI_READ_LINK_POLICY_SETTINGS_PARAM_LEN  ((INT8U) 2)
#define HCI_WRITE_LINK_POLICY_SETTINGS_PARAM_LEN ((INT8U) 4)
#define HCI_READ_DEF_LP_SETTINGS_PARAM_LEN  	 ((INT8U) 0)
#define HCI_WRITE_DEF_LP_SETTINGS_PARAM_LEN  	 ((INT8U) 2)

#define HCI_SET_EVENT_MASK_PARAM_LEN             ((INT8U) 8)
#define HCI_RESET_PARAM_LEN                      ((INT8U) 0)
#define HCI_SET_EVENT_FILTER_PARAM_LEN           ((INT8U) 8) /* Variable */
#define HCI_FLUSH_PARAM_LEN                      ((INT8U) 2)
#define HCI_READ_PIN_TYPE_PARAM_LEN              ((INT8U) 0)
#define HCI_WRITE_PIN_TYPE_PARAM_LEN             ((INT8U) 1)
#define HCI_CREATE_NEW_UNIT_KEY_PARAM_LEN        ((INT8U) 0)
#define HCI_READ_STORED_LINK_KEY_PARAM_LEN       ((INT8U) 7)
#define HCI_WRITE_STORED_LINK_KEY_PARAM_LEN      ((INT8U) 23) /* Variable */
#define HCI_DELETE_STORED_LINK_KEY_PARAM_LEN     ((INT8U) 7)
#define HCI_CHANGE_LOCAL_NAME_PARAM_LEN          ((INT8U) 248)
#define HCI_READ_LOCAL_NAME_PARAM_LEN            ((INT8U) 0)
#define HCI_READ_CONN_ACCEPT_TIMEOUT_PARAM_LEN   ((INT8U) 0)
#define HCI_WRITE_CONN_ACCEPT_TIMEOUT_PARAM_LEN  ((INT8U) 2)
#define HCI_READ_PAGE_TIMEOUT_PARAM_LEN          ((INT8U) 0)
#define HCI_WRITE_PAGE_TIMEOUT_PARAM_LEN         ((INT8U) 2)
#define HCI_READ_SCAN_ENABLE_PARAM_LEN           ((INT8U) 0)
#define HCI_WRITE_SCAN_ENABLE_PARAM_LEN          ((INT8U) 1)
#define HCI_READ_PAGESCAN_ACTIVITY_PARAM_LEN     ((INT8U) 0)
#define HCI_WRITE_PAGESCAN_ACTIVITY_PARAM_LEN    ((INT8U) 4)
#define HCI_READ_INQUIRYSCAN_ACTIVITY_PARAM_LEN  ((INT8U) 0)
#define HCI_WRITE_INQUIRYSCAN_ACTIVITY_PARAM_LEN ((INT8U) 4)
#define HCI_READ_AUTH_ENABLE_PARAM_LEN           ((INT8U) 0)
#define HCI_WRITE_AUTH_ENABLE_PARAM_LEN          ((INT8U) 1)
#define HCI_READ_ENC_MODE_PARAM_LEN              ((INT8U) 0)
#define HCI_WRITE_ENC_MODE_PARAM_LEN             ((INT8U) 1)
#define HCI_READ_CLASS_OF_DEVICE_PARAM_LEN       ((INT8U) 0)
#define HCI_WRITE_CLASS_OF_DEVICE_PARAM_LEN      ((INT8U) 3)
#define HCI_READ_VOICE_SETTING_PARAM_LEN         ((INT8U) 0)
#define HCI_WRITE_VOICE_SETTING_PARAM_LEN        ((INT8U) 2)
#define HCI_READ_AUTO_FLUSH_TIMEOUT_PARAM_LEN    ((INT8U) 2)
#define HCI_WRITE_AUTO_FLUSH_TIMEOUT_PARAM_LEN   ((INT8U) 4)
#define HCI_READ_NUM_BCAST_RETXS_PARAM_LEN       ((INT8U) 0)
#define HCI_WRITE_NUM_BCAST_RETXS_PARAM_LEN      ((INT8U) 1)
#define HCI_READ_HOLD_MODE_ACTIVITY_PARAM_LEN    ((INT8U) 0)
#define HCI_WRITE_HOLD_MODE_ACTIVITY_PARAM_LEN   ((INT8U) 1)
#define HCI_READ_TX_POWER_LEVEL_PARAM_LEN        ((INT8U) 3)
#define HCI_READ_SYNC_FLOW_CON_ENABLE_PARAM_LEN   ((INT8U) 0)
#define HCI_WRITE_SYNC_FLOW_CON_ENABLE_PARAM_LEN  ((INT8U) 1)
#define HCI_SET_HCTOHOST_FLOW_CONTROL_PARAM_LEN  ((INT8U) 1)
#define HCI_HOST_BUFFER_SIZE_PARAM_LEN           ((INT8U) 7)
#define HCI_HOST_NUM_COMPLETED_PACKETS_PARAM_LEN ((INT8U) 5) /* Variable */
#define HCI_READ_LINK_SUPERV_TIMEOUT_PARAM_LEN   ((INT8U) 2)
#define HCI_WRITE_LINK_SUPERV_TIMEOUT_PARAM_LEN  ((INT8U) 4)
#define HCI_READ_NUM_SUPPORTED_IAC_PARAM_LEN     ((INT8U) 0)
#define HCI_READ_CURRENT_IAC_LAP_PARAM_LEN       ((INT8U) 0)
#define HCI_WRITE_CURRENT_IAC_LAP_PARAM_LEN      ((INT8U) 4) /* Variable */
#define HCI_READ_PAGESCAN_PERIOD_MODE_PARAM_LEN  ((INT8U) 0)
#define HCI_WRITE_PAGESCAN_PERIOD_MODE_PARAM_LEN ((INT8U) 1)
#define HCI_READ_PAGESCAN_MODE_PARAM_LEN         ((INT8U) 0)
#define HCI_WRITE_PAGESCAN_MODE_PARAM_LEN        ((INT8U) 1)

#define HCI_READ_INQUIRYSCAN_TYPE_PARAM_LEN         ((INT8U) 0)
#define HCI_WRITE_INQUIRYSCAN_TYPE_PARAM_LEN        ((INT8U) 1)
#define HCI_READ_INQUIRY_MODE_PARAM_LEN				((INT8U) 0)
#define HCI_WRITE_INQUIRY_MODE_PARAM_LEN			((INT8U) 1)

#define HCI_READ_AFH_CHANNEL_ASSESSMENT_MODE_LEN	((INT8U) 0)
#define HCI_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_LEN	((INT8U) 1)


#define HCI_READ_LOCAL_VER_INFO_PARAM_LEN        ((INT8U) 0)
#define HCI_READ_LOCAL_SUPP_CMD_PARAM_LEN        ((INT8U) 0)
#define HCI_READ_LOCAL_SUPP_FEATURES_PARAM_LEN   ((INT8U) 0)
#define HCI_READ_LOCAL_FEATURES_EXT_PARAM_LEN    ((INT8U) 1)
#define HCI_READ_BUFFER_SIZE_PARAM_LEN           ((INT8U) 0)
#define HCI_READ_COUNTRY_CODE_PARAM_LEN          ((INT8U) 0)
#define HCI_READ_BD_ADDR_PARAM_LEN               ((INT8U) 0)

#define HCI_READ_FAILED_CONTACT_COUNT_PARAM_LEN  ((INT8U) 2)
#define HCI_RESET_FAILED_CONTACT_COUNT_PARAM_LEN ((INT8U) 2)
#define HCI_GET_LINK_QUALITY_PARAM_LEN           ((INT8U) 2)
#define HCI_READ_RSSI_PARAM_LEN                  ((INT8U) 2)
#define HCI_READ_AFH_CHANNEL_MAP_PARAM_LEN       ((INT8U) 2)
#define HCI_READ_CLOCK_PARAM_LEN                 ((INT8U) 3)

#define HCI_READ_LOOPBACK_MODE_PARAM_LEN         ((INT8U) 0)
#define HCI_WRITE_LOOPBACK_MODE_PARAM_LEN        ((INT8U) 1)
#define HCI_ENABLE_DUT_MODE_PARAM_LEN            ((INT8U) 0)

#define HCI_DEBUG_REQUEST_PARAM_LEN              ((INT8U) 2)


/******************************************************************************
        Event Parameter Lengths
 *****************************************************************************/
#define HCI_EV_INQUIRY_COMPLETE_PARAM_LEN                 ((INT8U) 1)
#define HCI_EV_INQUIRY_RESULT_PARAM_LEN                   ((INT8U) 15) /* Variable */
#define HCI_EV_CONN_COMPLETE_PARAM_LEN                    ((INT8U) 11)
#define HCI_EV_CONN_REQUEST_PARAM_LEN                     ((INT8U) 10)
#define HCI_EV_DISCONNECT_COMPLETE_PARAM_LEN              ((INT8U) 4)
#define HCI_EV_AUTH_COMPLETE_PARAM_LEN                    ((INT8U) 3)
#define HCI_EV_REMOTE_NAME_REQ_COMPLETE_MAX_LEN           ((INT8U) 255)
#define HCI_EV_REMOTE_NAME_REQ_COMPLETE_BASIC_LEN         ((INT8U) 7)
#define HCI_EV_ENCRYPTION_CHANGE_PARAM_LEN                ((INT8U) 4)
#define HCI_EV_CHANGE_CONN_LINK_KEY_COMPLETE_PARAM_LEN    ((INT8U) 3)
#define HCI_EV_MASTER_LINK_KEY_COMPLETE_PARAM_LEN         ((INT8U) 4)
#define HCI_EV_READ_REM_SUPP_FEATURES_COMPLETE_PARAM_LEN  ((INT8U) 11)
#define HCI_EV_READ_REMOTE_VER_INFO_COMPLETE_PARAM_LEN    ((INT8U) 8)
#define HCI_EV_QOS_SETUP_COMPLETE_PARAM_LEN               ((INT8U) 21)
#define HCI_EV_COMMAND_COMPLETE_PARAM_LEN                 ((INT8U) 3) /* Variable see below */
#define HCI_EV_COMMAND_STATUS_PARAM_LEN                   ((INT8U) 4)
#define HCI_EV_HARDWARE_ERROR_PARAM_LEN                   ((INT8U) 1)
#define HCI_EV_FLUSH_OCCURRED_PARAM_LEN                   ((INT8U) 2)
#define HCI_EV_ROLE_CHANGE_PARAM_LEN                      ((INT8U) 8)
#define HCI_EV_NUMBER_COMPLETED_PKTS_PARAM_LEN            ((INT8U) 5) /* Variable */
#define HCI_EV_MODE_CHANGE_PARAM_LEN                      ((INT8U) 6)
#define HCI_EV_RETURN_LINK_KEYS_PARAM_LEN                 ((INT8U) 23) /* Variable */
#define HCI_EV_PIN_CODE_REQ_PARAM_LEN                     ((INT8U) 6)
#define HCI_EV_LINK_KEY_REQ_PARAM_LEN                     ((INT8U) 6)
#define HCI_EV_LINK_KEY_NOTIFICATION_PARAM_LEN            ((INT8U) 23)
#define HCI_EV_LOOPBACK_COMMAND_PARAM_LEN                 ((INT8U) 0) /* Variable */
#define HCI_EV_DATA_BUFFER_OVERFLOW_PARAM_LEN             ((INT8U) 1)
#define HCI_EV_MAX_SLOTS_CHANGE_PARAM_LEN                 ((INT8U) 3)
#define HCI_EV_READ_CLOCK_OFFSET_COMPLETE_PARAM_LEN       ((INT8U) 5)
#define HCI_EV_CONN_PACKET_TYPE_CHANGED_PARAM_LEN         ((INT8U) 5)
#define HCI_EV_QOS_VIOLATION_PARAM_LEN                    ((INT8U) 2)
#define HCI_EV_PAGE_SCAN_MODE_CHANGE_PARAM_LEN            ((INT8U) 7)
#define HCI_EV_PAGE_SCAN_REP_MODE_CHANGE_PARAM_LEN        ((INT8U) 7)
#define HCI_EV_DEBUG_PARAM_LEN                            ((INT8U) 20 )


/******************************************************************************
   HCI_COMMAND_COMPLETE, Argument Length Definitions (not full length)
   3 From Command Complete + Return Parameters.
   When an argument length is dependant on the number of elements in the array
   the defined length contains the constant parameter lengths only. The full 
   array length must be calculated.
 *****************************************************************************/
#define HCI_INQUIRY_CANCEL_ARG_LEN             ((INT8U) 1) 
#define HCI_PERIODIC_INQ_MODE_ARG_LEN          ((INT8U) 1)
#define HCI_EXIT_PERIODIC_INQ_MODE_ARG_LEN     ((INT8U) 1)
#define HCI_CREATE_CONN_CANCEL_ARG_LEN          ((INT8U) 6)
#define HCI_LINK_KEY_REQ_REPLY_ARG_LEN         ((INT8U) 7)
#define HCI_LINK_KEY_REQ_NEG_REPLY_ARG_LEN     ((INT8U) 7)
#define HCI_PIN_CODE_REQ_REPLY_ARG_LEN         ((INT8U) 7)
#define HCI_PIN_CODE_REQ_NEG_REPLY_ARG_LEN     ((INT8U) 7)
#define HCI_ROLE_DISCOVERY_ARG_LEN             ((INT8U) 4)
#define HCI_READ_LINK_POLICY_SETTINGS_ARG_LEN  ((INT8U) 5)
#define HCI_WRITE_LINK_POLICY_SETTINGS_ARG_LEN ((INT8U) 3)
#define HCI_SET_EVENT_MASK_ARG_LEN             ((INT8U) 1)
#define HCI_RESET_ARG_LEN                      ((INT8U) 1)
#define HCI_SET_EVENT_FILTER_ARG_LEN           ((INT8U) 1)
#define HCI_FLUSH_ARG_LEN                      ((INT8U) 4)
#define HCI_READ_PIN_TYPE_ARG_LEN              ((INT8U) 2)
#define HCI_WRITE_PIN_TYPE_ARG_LEN             ((INT8U) 1)
#define HCI_CREATE_NEW_UNIT_KEY_ARG_LEN        ((INT8U) 1)
#define HCI_READ_STORED_LINK_KEY_ARG_LEN       ((INT8U) 5)
#define HCI_WRITE_STORED_LINK_KEY_ARG_LEN      ((INT8U) 2)
#define HCI_DELETE_STORED_LINK_KEY_ARG_LEN     ((INT8U) 3)
#define HCI_CHANGE_LOCAL_NAME_ARG_LEN          ((INT8U) 1)
#define HCI_READ_LOCAL_NAME_ARG_LEN            ((INT8U) 249)
#define HCI_READ_CONN_ACCEPT_TIMEOUT_ARG_LEN   ((INT8U) 3)
#define HCI_WRITE_CONN_ACCEPT_TIMEOUT_ARG_LEN  ((INT8U) 1)
#define HCI_READ_PAGE_TIMEOUT_ARG_LEN          ((INT8U) 3)
#define HCI_WRITE_PAGE_TIMEOUT_ARG_LEN         ((INT8U) 1)
#define HCI_READ_SCAN_ENABLE_ARG_LEN           ((INT8U) 2)
#define HCI_WRITE_SCAN_ENABLE_ARG_LEN          ((INT8U) 1)
#define HCI_READ_PAGESCAN_ACTIVITY_ARG_LEN     ((INT8U) 5)
#define HCI_WRITE_PAGESCAN_ACTIVITY_ARG_LEN    ((INT8U) 1)
#define HCI_READ_INQUIRYSCAN_ACTIVITY_ARG_LEN  ((INT8U) 5)
#define HCI_WRITE_INQUIRYSCAN_ACTIVITY_ARG_LEN ((INT8U) 1)
#define HCI_READ_AUTH_ENABLE_ARG_LEN           ((INT8U) 2)
#define HCI_WRITE_AUTH_ENABLE_ARG_LEN          ((INT8U) 1)
#define HCI_READ_ENC_MODE_ARG_LEN              ((INT8U) 2)
#define HCI_WRITE_ENC_MODE_ARG_LEN             ((INT8U) 1)
#define HCI_READ_CLASS_OF_DEVICE_ARG_LEN       ((INT8U) 4)
#define HCI_WRITE_CLASS_OF_DEVICE_ARG_LEN      ((INT8U) 1)
#define HCI_READ_VOICE_SETTING_ARG_LEN         ((INT8U) 3)
#define HCI_WRITE_VOICE_SETTING_ARG_LEN        ((INT8U) 1)
#define HCI_READ_AUTO_FLUSH_TIMEOUT_ARG_LEN    ((INT8U) 5)
#define HCI_WRITE_AUTO_FLUSH_TIMEOUT_ARG_LEN   ((INT8U) 3)
#define HCI_READ_NUM_BCASTXS_ARG_LEN           ((INT8U) 2)
#define HCI_WRITE_NUM_BCASTXS_ARG_LEN          ((INT8U) 1)
#define HCI_READ_HOLD_MODE_ACTIVITY_ARG_LEN    ((INT8U) 2)
#define HCI_WRITE_HOLD_MODE_ACTIVITY_ARG_LEN   ((INT8U) 1)
#define HCI_READ_TX_POWER_LEVEL_ARG_LEN        ((INT8U) 4)
#define HCI_READ_SCO_FLOW_CON_ENABLE_ARG_LEN   ((INT8U) 2)
#define HCI_WRITE_SCO_FLOW_CON_ENABLE_ARG_LEN  ((INT8U) 1)
#define HCI_SET_HC_TO_H_FLOW_CONTROL_ARG_LEN   ((INT8U) 1)
#define HCI_HOST_BUFFER_SIZE_ARG_LEN           ((INT8U) 1)
#define HCI_HOST_NUM_COMPLETED_PKTS_ARG_LEN    ((INT8U) 1)
#define HCI_READ_LINK_SUPERV_TIMEOUT_ARG_LEN   ((INT8U) 5)
#define HCI_WRITE_LINK_SUPERV_TIMEOUT_ARG_LEN  ((INT8U) 3)
#define HCI_READ_NUM_SUPPORTED_IAC_ARG_LEN     ((INT8U) 2)
#define HCI_READ_CURRENT_IAC_LAP_ARG_LEN       ((INT8U) 5) /* Variable */
#define HCI_WRITE_CURRENT_IAC_LAP_ARG_LEN      ((INT8U) 1)
#define HCI_READ_PAGESCAN_PERIOD_MODE_ARG_LEN  ((INT8U) 2)
#define HCI_WRITE_PAGESCAN_PERIOD_MODE_ARG_LEN ((INT8U) 1)
#define HCI_READ_PAGESCAN_MODE_ARG_LEN         ((INT8U) 2)
#define HCI_WRITE_PAGESCAN_MODE_ARG_LEN        ((INT8U) 1)
#define HCI_READ_INQUIRYSCAN_TYPE_ARG_LEN	   ((INT8U) 2) 	/* Add for spec 1.2 */
#define HCI_WRITE_INQUIRYSCAN_TYPE_ARG_LEN	   ((INT8U) 1)	/* Add for spec 1.2 */
#define HCI_READ_INQUIRY_MODE_ARG_LEN	       ((INT8U) 2) 	/* Add for spec 1.2 */
#define HCI_WRITE_INQUIRY_MODE_ARG_LEN	       ((INT8U) 1)	/* Add for spec 1.2 */


#define HCI_READ_LOCAL_VER_INFO_ARG_LEN        ((INT8U) 9)
#define HCI_READ_LOCAL_FEATURES_LEN            ((INT8U) 9)
#define HCI_READ_BUFFER_SIZE_ARG_LEN           ((INT8U) 8)
#define HCI_READ_COUNTRY_CODE_ARG_LEN          ((INT8U) 2)
#define HCI_READ_BD_ADDR_ARG_LEN               ((INT8U) 7)
#define HCI_READ_FAILED_CONTACT_COUNT_ARG_LEN  ((INT8U) 5)
#define HCI_RESET_FAILED_CONTACT_COUNT_ARG_LEN ((INT8U) 3)
#define HCI_GET_LINK_QUALITY_ARG_LEN           ((INT8U) 4)
#define HCI_READ_RSSI_ARG_LEN                  ((INT8U) 4)
#define HCI_READ_LOOPBACK_MODE_ARG_LEN         ((INT8U) 2)
#define HCI_WRITE_LOOPBACK_MODE_ARG_LEN        ((INT8U) 1)
#define HCI_ENABLE_DUT_ARG_LEN                 ((INT8U) 1)
#define HCI_MNFR_EXTENSION_ARG_LEN             ((INT8U) 1)
 

/*HCI_CMD_PACKET: 0x01 + opcode(2 bytes)+length(1 byte)+data[255]*/


__packed struct hci_cmd_packet {
//	uint8 misc_data; /* to adjust with lmp_handle_msg */
	uint8 cmd_flag;  /*must be 0x01*/
	uint16 opcode;
	uint8 length;
	uint8 data[1];
};

__packed struct hci_acl_packet {
	uint8 acl_flag; //must be 0x02
    INT16U conn_handle;
    INT16U length;
    INT8U *data;
};

__packed struct hci_sco_packet {
    INT16U conn_handle;
    INT8U length;
    INT8U *data;
};


__packed struct hci_evt_packet {
	uint8 evt_flag; /*must be 0x04*/
    uint8 evt_code;
    uint8 length;
    //uint8 data[1];
	INT8U *data;
};


#define HCI_INQUIRY                     	          0x0401 

__packed struct hci_cp_inquiry {
	uint8     lap[3];
	uint8     length;
	uint8     num_rsp;
};

//#define HCI_INQUIRY_CANCEL              	          0x0402

#define HCI_REMOTE_NAME_REQ  			      0x0419
__packed struct hci_cp_remote_name_request {
    struct bdaddr_t bdaddr;
    uint8 page_scan_repetition_mode;
    uint8 reserved;
    uint16 clk_off;
};

#define HCI_REMOTE_NAME_CANCEL  			  0x041A
__packed struct hci_cp_remote_name_cancel {
    struct bdaddr_t bdaddr;
};

#define HCI_CREATE_CONNECTION          		  0x0405
__packed struct hci_cp_create_conn {
    struct bdaddr_t bdaddr;
    uint16 pkt_type;
    uint8 page_scan_repetition_mode;
    uint8 reserved;
    uint16 clk_off;
    uint8 allow_role_switch;
};

#define HCI_DISCONNECT              	  0x0406
__packed struct hci_cp_disconnect {
	uint16   handle;
	uint8     reason;
};


#define HCI_ACCEPT_CONNECTION_REQ   		  0x0409
__packed struct hci_cp_accept_conn_req {
	struct bdaddr_t bdaddr;

	uint8     role;
};

#define HCI_REJECT_CONNECTION_REQ 		  0x040A
__packed struct hci_cp_reject_conn_req {
	struct bdaddr_t bdaddr;
	uint8     reason;
};
#define HCI_LINK_KEY_REQ_REPLY    		  0x040B
__packed struct hci_cp_link_key_reply {
	struct bdaddr_t bdaddr;
	uint8     link_key[16];
};

#define HCI_LINK_KEY_REQ_NEG_REPLY  		  0x040C
__packed struct hci_cp_link_key_neg_reply {
	struct bdaddr_t bdaddr;
};

#define HCI_PIN_CODE_REQ_REPLY        		  0x040D
__packed struct hci_cp_pin_code_reply {
	struct bdaddr_t  bdaddr;
	uint8     pin_len;
	uint8     pin_code[16];
};

#define HCI_PIN_CODE_REQ_NEG_REPLY  		  0x040E
__packed struct hci_cp_pin_code_neg_reply {
	struct bdaddr_t  bdaddr;
};
#define HCI_CHANGE_CONN_PKT_TYPE 		      0x040F
__packed struct hci_cp_change_conn_pkt_type {
	uint16 conn_handle;
	uint16 pkt_type;
};


#define HCI_AUTH_REQ        			  0x0411
__packed struct hci_cp_auth_req {
	uint16 conn_handle;
};

#define HCI_SET_CONN_ENCRYPTION        			  0x0413
__packed struct hci_cp_set_conn_encryption {
	uint16 conn_handle;
	uint8 encryption_enable;
};

#define HCI_SETUP_SYNC_CONN			  0x0428
__packed struct hci_cp_setup_sync_conn {
	uint16 conn_handle;
	uint32 tx_bandwidth;
	uint32 rx_bandwidth;
	uint16 max_latency;
	uint16 voice_setting;
	uint8 retx_effort;
	uint16 pkt_type;
};

#define HCI_ADDSCO_CONN  			          0x0407
__packed struct hci_cp_addsco_conn {
	uint16 conn_handle;
	uint16 pkt_type;
};



#define HCI_ACCEPT_SYNC_CONN			  0x0429
__packed struct hci_cp_accept_sync_conn {
	struct bdaddr_t bdaddr;
	uint32 tx_bandwidth;
	uint32 rx_bandwidth;
	uint16 max_latency;
	uint16 voice_setting;
	uint8 retx_effort;
	uint16 pkt_type;
};


#define HCI_REJECT_SYNC_CONN			  0x042A
__packed struct hci_cp_reject_sync_conn {
	struct bdaddr_t bdaddr;
	uint8     reason;
};

#define HCI_IO_CAPABILITY_REQ_REPLY 0x042B
__packed struct hci_cp_io_capability_req_reply {
	struct bdaddr_t bdaddr;
	uint8     io_capability;
	uint8     oob_data_present;
	uint8   auth_requirements;
};


#define HCI_IO_CAPABILITY_REQ_NEGA_REPLY 0x0434
__packed struct hci_cp_io_capability_req_nega_reply {
	struct bdaddr_t bdaddr;
	uint8     reason;
};

#define HCI_SET_EVENT_MASK            		  0x0C01
__packed struct hci_cp_set_event_mask {
	uint8 event_mask[8];
};
#define HCI_USER_CONFIRM_REQ_REPLY 0x042C
__packed struct hci_cp_user_confirm_req_reply {
	struct bdaddr_t bdaddr;
};


#define HCI_WRITE_SCAN_ENABLE          	 	  0x0C1A
__packed struct hci_cp_write_scan_enable {
	uint8     scan_enable;
};

#define HCI_WRITE_CLASS_OF_DEVICE     	 	  0x0C24
__packed struct hci_cp_write_class_of_device {
	uint8    class_dev[3];
};

#define HCI_WRITE_LOCAL_NAME                  0x0C13
#define BTM_NAME_MAX_LEN			100
__packed struct hci_cp_write_local_name {
	uint8    local_name[BTM_NAME_MAX_LEN];
};

#define HCI_SNIFF_MODE                            0x0803
__packed struct hci_cp_sniff_mode {
        uint16 conn_handle;
        uint16 sniff_max_interval;
        uint16 sniff_min_interval; 
        uint16 sniff_attempt; 
        uint16 sniff_timeout;
};

#define HCI_EXIT_SNIFF_MODE                       0x0804
__packed struct hci_cp_exit_sniff_mode {
        uint16 conn_handle;
};

#define HCI_WRITE_LP_SETTINGS                     0x080D  
__packed struct hci_cp_write_link_policy {
        uint16 conn_handle;
        uint16 link_policy_settings;
};


#define HCI_READ_STORED_LINK_KEY        	  0x0C0D
__packed struct hci_cp_read_stored_linkkey {
    struct bdaddr_t bdaddr;
    uint8 read_all_flag;
};

#define HCI_WRITE_STORED_LINK_KEY      		  0x0C11
#define HCI_HS_STORED_LINK_KEY      		  0x201C //
__packed struct hci_cp_write_stored_linkkey {
    uint8 num_keys_to_write;
    struct bdaddr_t bdaddr;
    uint8    link_key[16];
};

#define HCI_DELETE_STORED_LINK_KEY   		  0x0C12
__packed struct hci_cp_delete_stored_linkkey {
    struct bdaddr_t bdaddr;
    uint8  delete_all_flag;
};

#define HCI_WRITE_AUTH_ENABLE         		  0x0C20
__packed struct hci_cp_write_auth_enable{
    uint8 enable_flag;
};

#define HCI_WRITE_LINK_SUPERV_TIMEOUT           0x0c37
__packed struct hci_cp_write_super_timeout {
	uint16 conn_handle;
	uint16 timeout;
};

#define HCI_WRITE_SSP_MODE        		  0x0C56
__packed struct hci_cp_write_ssp_mode{
    uint8 ssp_mode;
};

#define HCI_READ_SSP_MODE        		  0x0C55



#define HCI_SET_BD_ADDR					 0xFC1A
__packed struct hci_cp_set_bdaddr {
    struct bdaddr_t bdaddr;
};

#define HCI_WRITE_MEMORY                  0xFC90
__packed struct hci_cp_write_memory {
    uint32 address;
    uint32 value;
    uint8  bytelen;
};

__packed struct hci_cp_set_uart {
    uint8 uart_baud_rate;
	uint8 uart_cfg;
};

__packed struct hci_cp_flow_cntl{
  	uint8 flow_cntl;
};

__packed struct hci_cp_host_buffer_size{
  	uint16 host_acl_length;
	uint8 host_sco_length;
	uint16 host_num_acl_pkts;
	uint16 host_num_sco_pkts;
};

__packed struct hci_cp_host_num_completed_pkts{
  	uint8 num_of_handle;
	uint16 conn_handle;
	uint16 host_num_completed_pkts;
};

#define HCI_READ_BUFFER_SIZE  0x1005
//#define HCI_WRITE_PAGE_TIMEOUT            0x0C18
__packed struct hci_cp_write_page_timeout {
uint16 page_timeout;
};

#define HCI_WRITE_PAGESCAN_ACTIVITY       	  0x0C1C
__packed struct hci_cp_write_pagescan_activity {
    uint16 pagescan_interval;
    uint16 pagescan_window;
};
#define HCI_WRITE_INQUIRYSCAN_ACTIVITY   	  0x0C1E
__packed struct hci_cp_write_inquiryscan_activity {
    uint16 inquiryscan_interval;
    uint16 inquiryscan_window;
};

struct hci_evt_packet *hcievt_packet_create (uint8 evtcode, uint8 evt_param_len);
void hci_post_inq_complete_evt(uint8 status);
void hci_post_remote_name_req_complete_evt(uint8 status, struct bdaddr_t *bdaddr, uint8 *name, uint8 namelen);

void hci_post_connpkt_type_change_evt(uint8 status, uint16 handle, uint16 pkt_type);
void hci_post_pin_code_req_evt(struct bdaddr_t *bdaddr );
void hci_post_read_remote_extfeatures_evt(uint8 status, uint16 handle, 
												uint8 page_num,uint8 max_page_num,uint8 *ext_features  );
void hci_post_read_clkoff_evt(uint8 status, uint16 handle, uint16 clkoff);
void hci_post_role_change_evt(uint8 status, struct bdaddr_t *bdaddr, uint8 new_role );

INT8U LMP_AclDataTx(INT8U* DataPtr, INT16U length, INT16U ConHdl);

int8 btlib_hcicmd_accep_conn_req(struct bdaddr_t *bdaddr, uint8 role);
int8 btlib_hcicmd_pincode_reply(struct bdaddr_t *bdaddr, uint8 *pin, int8 pinlen);
int8 btlib_hcicmd_linkkey_neg_reply(struct bdaddr_t *bdaddr);
int8 btlib_hcicmd_linkkey_reply(struct bdaddr_t *bdaddr, uint8 *linkkey);
int8 btlib_hcicmd_disconnect (uint16 conn_handle, uint8 reason);
int8 btlib_hcicmd_set_conn_encryption (uint16 conn_handle, uint8 encry_enable);
int8 btlib_hcicmd_user_confirm_req_reply(struct bdaddr_t *bdaddr);
int8 btlib_hcicmd_set_localbdaddr(uint8 *bdaddr);
int8 btlib_hcicmd_write_classofdevice (uint8 *class_dev);
int8 btlib_hcicmd_write_localname (uint8 *local_name);
int8 btlib_hcicmd_write_page_timeout(uint16 page_timeout);
int8 btlib_hcicmd_acl_connect(struct bdaddr_t *bdaddr, uint16 pkt_type, uint8 page_scan_repetition_mode, uint16 clk_off, uint8 allow_role_switch);
int8 btlib_hcicmd_write_scan_enable(int8 scan_enable);
int8 btlib_hcicmd_write_memory(uint32 addr, uint32 value, uint8 bytelen);
int8 btlib_hcicmd_authentication_req (uint16 conn_handle);
int8 btlib_hcicmd_write_supervisiontimeout(uint16 conn_handle, uint16 timeout);
int8 btlib_hcicmd_write_page_scan_activity(uint16 interval, uint16 window);
int8 btlib_hcicmd_write_inquiry_scan_activity(uint16 interval, uint16 window);
int8 hcicmd_msg_send(struct hci_cmd_packet *param);

#endif

