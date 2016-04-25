#include "stm32f10x_type.h"
#include "buffer.h"

uint32 receive_buffer_pool_map = 0;
uint8 *receive_buffer_pool[RECEIVE_BUFFER_COUNT];
uint8 BUFFER[MAX_BUFFER_LENGTH];
uint16 buffer_size[RECEIVE_BUFFER_COUNT] = 
{
	12, 12, 12,
	25, 25, 25,
	55, 55, 55,
	100, 100, 100
};

uint32 READ_INT32(int offset,void *addr)
{
	uint8 *p;
	p=(uint8 *)(((uint8 *)addr) + offset);

	return ((*p)+
		((*(p+1))<<8)+
		((*(p+2))<<16)+
		((*(p+3))<<24));
}

void WRITE_INT32(int offset,void *addr,uint32 data)
{
	uint8 *p;
	p=(uint8 *)(((uint8 *)addr) + offset);

	*p++ = data & 0xFF;
	*p++ = (data>>8) & 0xFF;
	*p++ = (data>>16) & 0xFF;
	*p = (data>>24) & 0xFF;

}

uint8 *malloc_buffer(uint16 size)
{
	uint8 i;

	for(i=0; i<RECEIVE_BUFFER_COUNT; i++) {
		if(size <= buffer_size[i])
			break;
	}

	if(i == RECEIVE_BUFFER_COUNT)
		return NULL;

	//enter critical
	for(; i<RECEIVE_BUFFER_COUNT; i++) {
		if((receive_buffer_pool_map & (1<<i)) != 0) {
			receive_buffer_pool_map &= (~(1<<i));
			break;
		}
	}
	//exit critical

	if(i == RECEIVE_BUFFER_COUNT)
		return NULL;

	return receive_buffer_pool[i];
}

static uint8 search_buffer_index(uint8 *buff, uint8 begin, uint8 end)
{
	uint8 middle;

	if(begin == end) {
		if(receive_buffer_pool[begin] == buff) {
			return begin;
		} else {
			return 0;
		}
	}

	middle = (begin+end)/2;
	if(receive_buffer_pool[middle] == buff) {
		return middle;
	} else if(receive_buffer_pool[middle] < buff) {
		return search_buffer_index(buff, middle+1, end);
	} else {
		return search_buffer_index(buff, begin, middle-1);
	}
}

void free_buffer(uint8 *buff)
{
	uint8 i;

	i = search_buffer_index(buff, 0, RECEIVE_BUFFER_COUNT-1);

	if(i != 0) {
		receive_buffer_pool_map |= (1<<i);
	} else {
		if(receive_buffer_pool[0] == buff)
			receive_buffer_pool_map |= 1;
	}
}

void buffer_init(void)
{
	uint8 i, count=0;
	uint8 *buffer = BUFFER;
	
#if RECEIVE_BUFFER_COUNT == 32
	receive_buffer_pool_map = 0xFFFFFFFF;
#else
	receive_buffer_pool_map = (1<<RECEIVE_BUFFER_COUNT) - 1;
#endif

	for(i=0; i<BUFFER1_COUNT; i++) {
		buffer_size[count] = BUFFER1_LENGTH;
		receive_buffer_pool[count++] = buffer;
		buffer += BUFFER1_SPACE_LENGTH;
	}

	for(i=0; i<BUFFER2_COUNT; i++) {
		buffer_size[count] = BUFFER2_LENGTH;
		receive_buffer_pool[count++] = buffer;
		buffer += BUFFER2_SPACE_LENGTH;
	}

	for(i=0; i<BUFFER3_COUNT; i++) {
		buffer_size[count] = BUFFER3_LENGTH;
		receive_buffer_pool[count++] = buffer;
		buffer += BUFFER3_SPACE_LENGTH;
	}

	for(i=0; i<BUFFER4_COUNT; i++) {
		buffer_size[count] = BUFFER4_LENGTH;
		receive_buffer_pool[count++] = buffer;
		buffer += BUFFER4_SPACE_LENGTH;
	}
}

