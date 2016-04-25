#ifndef _QUEUE_H
#define _QUEUE_H

#include "stm32f10x_type.h"

enum event_type{
	INPUT_TYPE = 1,
	OUTPUT_TYPE
};
#define OUTPUT_TYPE_DEEPTH	4
#define INPUT_TYPE_DEEPTH	10

uint8 queue_event(uint8 type, uint8 *message);
uint8 *dequeue_event(uint8 type);
void queue_init(void);

#endif	//_QUEUE_H

