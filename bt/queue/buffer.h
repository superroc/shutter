#ifndef _BUFFER_H
#define _BUFFER_H

#include "stm32f10x_type.h"

#define BUFFER1_LENGTH		12
#define BUFFER1_SPACE_LENGTH	((((BUFFER1_LENGTH-1)>>2)+1)<<2)
#define BUFFER1_COUNT		10
#define BUFFER2_LENGTH		25
#define BUFFER2_SPACE_LENGTH	((((BUFFER2_LENGTH-1)>>2)+1)<<2)
#define BUFFER2_COUNT		10
#define BUFFER3_LENGTH		55
#define BUFFER3_SPACE_LENGTH	((((BUFFER3_LENGTH-1)>>2)+1)<<2)
#define BUFFER3_COUNT		7
#define BUFFER4_LENGTH		300
#define BUFFER4_SPACE_LENGTH	((((BUFFER4_LENGTH-1)>>2)+1)<<2)
#define BUFFER4_COUNT		5

#define RECEIVE_BUFFER_COUNT	(BUFFER1_COUNT+BUFFER2_COUNT+BUFFER3_COUNT+BUFFER4_COUNT)
#define MAX_BUFFER_LENGTH		((BUFFER1_SPACE_LENGTH*BUFFER1_COUNT	\
									+BUFFER2_SPACE_LENGTH*BUFFER2_COUNT	\
									+BUFFER3_SPACE_LENGTH*BUFFER3_COUNT	\
									+BUFFER4_SPACE_LENGTH*BUFFER4_COUNT))

void WRITE_INT32(int offset,void *addr,uint32 data);
uint32 READ_INT32(int offset,void *addr);
uint8 *malloc_buffer(uint16 size);
void free_buffer(uint8 *buff);
void buffer_init(void);

#endif	//_BUFFER_H

