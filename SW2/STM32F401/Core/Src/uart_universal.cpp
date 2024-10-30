/*
 * uart_universal_read.c
 *
 *  Created on: Oct 27, 2024
 *      Author: Michał Lasoń
 */
#include "uart_universal.hpp"

uint8_t current = 0;
uint8_t end = 0;

/**
* Read UART variable length.
*
* BLOCKING FUNCTION
* HAL implementations must declare how many bytes are expected, forcing this workaround.
* A better way would be to use DMA, but for a simple shell this should be sufficient.
* UART_UNIVERSAL_READ reads all input until it finds a special ASCII symbol such as \r or \n.
* 
* @param buffer_size Maximum size of read message that can be read.
* @param huart UART handler to be used.
* @param buffer Buffer for storing the read message.
* @return Size of the message that was read.
*/
int UART_UNIVERSAL_READ(UART_HandleTypeDef *huart, char* buffer, int buffer_size){


	uint8_t sample;
	while(1){
		if(end == 1){
			current = 0;
			end = 0;
		}
		// Prevents buffer limits from being exceeded
		if(current >buffer_size-1) return current;
		// Reads one byte at a time.
		if(HAL_UART_Receive(huart, &sample, 1, 2000) != HAL_OK) return 0;
		// Checks for special characters and increases current
		if((sample>31) && (sample<127)) {buffer[current] = (char)sample; current++;}
		if(sample == '\r'){
			end = 1;
			return current;
		}
		// Makes it less blocking
		return 0;
	}
	// Something went wrong, shouldn't be here
	return 0;
}

