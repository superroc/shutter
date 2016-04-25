#include "stm32f10x_type.h"
#include "queue.h"
#include "hci.h"
#include "buffer.h"
#include "btm_handle_hcievent.h"

/* for uart */
char recvhcidata_uart_begin_flag = 0;
char recvhcidata_uart_type = 0;
unsigned short recvhcidata_uart_pkt_length = 0;
int recvhcidata_uart_rx_offset = 0;
unsigned char *recvhcidata_uart_head_ptr = NULL;
unsigned char *recvhcidata_uart_data_ptr = NULL;

int app_allow_sleep_flag = 0;

/*-------------------------------------------------------------------------
    Function    :  hciuart_handle_rx_char             xxx
    Description:
        HCI_CMD_PACKET: 0x01 + opcode(2 bytes)+length(1 byte)+data 
        HCI_ACL_PACKET: 0x02 + conn handle(2 bytes) + length(2 bytes) + data
        HCI_SCO_PACKET: 0x03 + conn handle(2 bytes) + length(1byte) + data
-------------------------------------------------------------------------*/
void recvhcidata_uart_rx(unsigned char c)
{
	uint16 conn_handle;
	
	if(recvhcidata_uart_begin_flag == 2){ /*recv data*/			
		recvhcidata_uart_data_ptr[recvhcidata_uart_rx_offset++] = c;
		if(recvhcidata_uart_rx_offset==recvhcidata_uart_pkt_length) {
			if(queue_event(INPUT_TYPE, recvhcidata_uart_head_ptr)!=TRUE){
				goto RX_RESET;  //ERROR
			}
			recvhcidata_uart_head_ptr = NULL;
			recvhcidata_uart_data_ptr = NULL;
			goto RX_RESET;
		}
		return;
	}

	if(recvhcidata_uart_begin_flag == 1) { /* recv header */
		recvhcidata_uart_head_ptr[recvhcidata_uart_rx_offset++] = c;			
		/*  get a acl packet  */
		if((recvhcidata_uart_rx_offset == 5) && ((recvhcidata_uart_type == HCI_ACL_PACKET))){
			recvhcidata_uart_pkt_length = (recvhcidata_uart_head_ptr[4]<<8) + recvhcidata_uart_head_ptr[3];
			conn_handle = (recvhcidata_uart_head_ptr[2]<<8) + recvhcidata_uart_head_ptr[1];

			if(recvhcidata_uart_pkt_length == 0) {
				goto RX_RESET;
			}
			if(recvhcidata_uart_pkt_length > BUFFER4_LENGTH){
				goto RX_RESET;
			}

			recvhcidata_uart_data_ptr = malloc_buffer(recvhcidata_uart_pkt_length);
			if(recvhcidata_uart_data_ptr == NULL) {
				goto RX_RESET;
			}
			
			WRITE_INT32(5, recvhcidata_uart_head_ptr, (int)recvhcidata_uart_data_ptr);
			recvhcidata_uart_begin_flag = 2;
			recvhcidata_uart_rx_offset = 0;
			return ;
		}

		/*  get a evt packet  */
		if((recvhcidata_uart_rx_offset == 3) && (recvhcidata_uart_type == HCI_EVT_PACKET)){
			recvhcidata_uart_pkt_length = c;
			
			if((recvhcidata_uart_pkt_length == 0) || (recvhcidata_uart_pkt_length > 256)) {
				goto RX_RESET;
			}
			
			recvhcidata_uart_data_ptr = malloc_buffer(recvhcidata_uart_pkt_length);
			if(recvhcidata_uart_data_ptr == NULL){
				goto RX_RESET;
			}
			
			WRITE_INT32( 3, recvhcidata_uart_head_ptr, (int)recvhcidata_uart_data_ptr);
			recvhcidata_uart_begin_flag = 2;
			recvhcidata_uart_rx_offset = 0;
			return ;
		}

		if(recvhcidata_uart_rx_offset >= 5){
			goto RX_RESET;
		}
		return;
	}

	/*  check whether hci packet begins  */
	if(recvhcidata_uart_begin_flag == 0){	
		recvhcidata_uart_type = c;	
		if ((recvhcidata_uart_type != HCI_EVT_PACKET) && (recvhcidata_uart_type != HCI_ACL_PACKET) ) {
			return;
		}
		
		recvhcidata_uart_head_ptr = malloc_buffer(5+4);  /* hcihead + data_ptr */
		if(recvhcidata_uart_head_ptr == NULL){
			return;
		}
		
		recvhcidata_uart_head_ptr[0] = c;
		recvhcidata_uart_rx_offset = 1;
		recvhcidata_uart_begin_flag = 1;
		return;
	}

RX_RESET:
	recvhcidata_uart_rx_offset = 0;

	if (recvhcidata_uart_head_ptr!=NULL) {
		free_buffer(recvhcidata_uart_head_ptr);
		recvhcidata_uart_head_ptr = NULL;
	}
	if (recvhcidata_uart_data_ptr!=NULL) {
		free_buffer(recvhcidata_uart_data_ptr);
		recvhcidata_uart_data_ptr = NULL;
	}    

	recvhcidata_uart_begin_flag = 0;
	recvhcidata_uart_type = 0;
	recvhcidata_uart_pkt_length = 0;
}

void hci_app_callback( unsigned char *param)
{
	unsigned char type;

	type = *param;

	switch (type) {
		case HCI_ACL_PACKET:            
			btm_acl_handle_nocopy( ((struct hci_acl_packet *)(param))->conn_handle,
				                ((struct hci_acl_packet *)(param))->length,
				                ((struct hci_acl_packet *)(param))->data);

			free_buffer(param);
			param = NULL;	
			break;

		case HCI_EVT_PACKET:
			btm_event_handle(param);
				
			if((((struct hci_evt_packet *)param)->length) != 0) {
			 	free_buffer(((struct hci_evt_packet *)param)->data);
				((struct hci_evt_packet *)param)->data = NULL;
			}
			free_buffer(param);
			param = NULL;
			break;
				
		default:
			free_buffer(param);
			param = NULL;
			break;
	}
}

