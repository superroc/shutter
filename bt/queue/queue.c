#include "queue.h"
#include "stm32f10x_type.h"

uint8 current_input_index = 0;
uint8 current_dealing_input_index = 0;
uint8 *input_message[INPUT_TYPE_DEEPTH];

uint8 current_output_index = 0;
uint8 current_dealing_output_index = 0;
uint8 *output_message[OUTPUT_TYPE_DEEPTH];

uint8 queue_event(uint8 type, uint8 *message)
{
	uint8 ret = FALSE;
	
	switch(type) {
		case INPUT_TYPE:
			if(input_message[current_input_index] == NULL) {
				input_message[current_input_index++] = message;
				if(current_input_index == INPUT_TYPE_DEEPTH)
					current_input_index = 0;

				ret = TRUE;
			}
			break;
		case OUTPUT_TYPE:
			if(output_message[current_output_index] == NULL) {
				output_message[current_output_index++] = message;
				if(current_output_index == OUTPUT_TYPE_DEEPTH)
					current_output_index = 0;

				ret = TRUE;
			}
			break;
		default:
			break;
	}

	return ret;
}

uint8 *dequeue_event(uint8 type)
{
	uint8 *message = NULL;
	
	switch(type) {
		case INPUT_TYPE:
			if(current_dealing_input_index != current_input_index) {
				message = input_message[current_dealing_input_index];
				input_message[current_dealing_input_index++] = NULL;
				if(current_dealing_input_index == INPUT_TYPE_DEEPTH)
					current_dealing_input_index = 0;
			}
			break;
		case OUTPUT_TYPE:
			if(current_dealing_output_index != current_output_index) {
				message = output_message[current_dealing_output_index];
				output_message[current_dealing_output_index++] = NULL;
				if(current_dealing_output_index == OUTPUT_TYPE_DEEPTH)
					current_dealing_output_index = 0;
			}
			break;
		default:
			break;
	}

	return message;
}

void queue_init(void)
{
	uint8 i;

	for(i=0; i<INPUT_TYPE_DEEPTH; i++) {
		input_message[i] = NULL;
	}

	for(i=0; i<OUTPUT_TYPE_DEEPTH; i++) {
		output_message[i] = NULL;
	}
}

