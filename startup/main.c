#include "sys.h"
#include "uart.h"		
#include "delay.h"	
#include "led.h" 
#include "beep.h"	 	 
#include "key.h"

#include "buffer.h"
#include "queue.h"
#include "btm.h"

const uint8 local_bdaddr[6] = {0x9e, 0x12, 0x11, 0x08, 0x12, 0x01};
const uint8 class_of_device[3] = {0x64, 0x05, 0x00};

int main(void)
{			
	u16 t;
	u16 len;	
	u16 times=0;
	uint32 delay;
	uint8 *message;
	uint8 data;
	uint8 buff[1];
    uint32 count = 0;
    uint8 flash_label = 0;

    //reset 5615
    RCC->APB2ENR|=1<<7;
    GPIOF->CRH&=0XFFF0FFFF;
	GPIOF->CRH|=0X00030000;//PF.12推挽输出
	GPIOF->ODR&=(~(0x1<<12));    //PE.012输出高
	
	
	Stm32_Clock_Init(6);	//系统时钟设置
	uart_init(72,115200);	 	//串口初始化为9600
	delay_init(72);	   	 	//延时初始化 
	//reset 5615
    RCC->APB2ENR|=1<<7;
    GPIOF->CRH&=0XFF00FFFF;
	GPIOF->CRH|=0X00830000;//PF.12推挽输出  PF.13 input
	GPIOF->ODR|=0x1<<13;    //PF13 上拉
	GPIOF->ODR&=(~(0x1<<12));    //PE.12输出low
	delay_ms(50);
    GPIOF->ODR|=(0x01<<12);

    RCC->APB2ENR|=1<<6;     //使能PORTE时钟
    GPIOE->CRL&=0X0FFFFFFF;	//PE7设置成输入	  
	GPIOE->CRL|=0X80000000; 				   
	GPIOE->ODR|=1<<7;	   	//PE7 上拉
    
	uart_init(72,115200);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
	BEEP_Init();         	//初始化蜂鸣器端口
	KEY_Init();         	//初始化与按键连接的硬件接口

	buffer_init();
	queue_init();

	btm_init();
	l2cap_init();
	sdp_init();
	hid_init();

	#define DELAY_COUNT		80000

	btlib_hcicmd_write_memory(0x10000000, 0x1f, 0x01); //opccr_reg (0x0800009c) to 0x1f 
	for(delay = 0; delay<DELAY_COUNT; delay++); //delay
		
	btlib_hcicmd_write_memory(0x0800009c, 0x1f, 0x01); //opccr_reg (0x0800009c) to 0x1f 
	for(delay = 0; delay<DELAY_COUNT; delay++); //delay
	
	btlib_hcicmd_write_memory(0x080000c2, 0x0, 0x01); //reserved_byte1(0x080000c2) to 0 
	for(delay = 0; delay<DELAY_COUNT; delay++); //delay
	
	btlib_hcicmd_write_memory(0x080001bc, 0xfb, 0x01); //Device_Features0 0x080001bc
	for(delay = 0; delay<DELAY_COUNT; delay++); //delay
	
	btlib_hcicmd_write_memory(0x080001bf, 0x70, 0x01); //Device_Features3 0x080001bf
	for(delay = 0; delay<DELAY_COUNT; delay++); //delay
	
	btlib_hcicmd_write_memory(0x080001c0, 0x80, 0x01); //Device_Features4 0x080001c0
	for(delay = 0; delay<DELAY_COUNT; delay++); //delay
	
	btlib_hcicmd_write_memory(0x080001c1, 0x01, 0x01); //Device_Features5 0x080001c1
	for(delay = 0; delay<DELAY_COUNT; delay++); //delay

	btlib_hcicmd_set_localbdaddr((uint8 *)local_bdaddr); //write local bdaddr
	for(delay = 0; delay<DELAY_COUNT; delay++); //delay

	btlib_hcicmd_write_classofdevice((uint8 *)class_of_device);	//write class of device
	for(delay = 0; delay<DELAY_COUNT; delay++); //delay  

	btlib_hcicmd_write_localname("FRQ HID2"); //write localname
	for(delay = 0; delay<DELAY_COUNT; delay++); //delay

    btlib_hcicmd_write_page_scan_activity(0x200, 0x20);
    for(delay = 0; delay<DELAY_COUNT; delay++); //delay

    btlib_hcicmd_write_inquiry_scan_activity(0x200, 0x20);
    for(delay = 0; delay<DELAY_COUNT; delay++); //delay
	//btlib_hcicmd_write_page_timeout(0x10*256);
	//for(delay = 0; delay<20000; delay++);

	btm_device_mode_set(DEVICE_MODE_DISCOVERABLE, DEVICE_MODE_CONNECTABLE);

    printf("hello world.\r\n");
	
 	while(1)
	{
		message = dequeue_event(INPUT_TYPE);
		if(message != NULL)
			hci_app_callback(message);

		data = KEY_Scan(0);
		switch(data) {
			case 1:
				buff[0] = 0x58;//enter
				hid_send_interrupt_data(buff, 1);
				buff[0] = 0x00;//vol up
				hid_send_interrupt_data(buff, 1);
				break;
			case 2:
				buff[0] = 0x4;//enter
				hid_send_interrupt_data(buff, 1);
				buff[0] = 0x00;//vol up
				hid_send_interrupt_data(buff, 1);
				break;
			case 3:
				buff[0] = 0x5;//enter
				hid_send_interrupt_data(buff, 1);
				buff[0] = 0x00;//vol up
				hid_send_interrupt_data(buff, 1);
				break;
			case 4:
				buff[0] = 0x04;//
				hid_send_interrupt_data2(buff, 1);
				buff[0] = 0x00;//vol up
				hid_send_interrupt_data2(buff, 1);
				break;
			default:
				break;
		}
	}
}


