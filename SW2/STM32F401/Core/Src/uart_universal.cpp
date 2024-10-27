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
* UART variable length read.
*
* BLOCKING FUNCTION
* HAL implementations requires to declare how much bytes are expected.
* UART_UNIVERSAL_READ reads all input until finds special ASCII symbol like \r or \n

* @param buffer_size Max size of read message.
* @param huart UART handler to be used.
* @param buffer Buffer to store read message.
* @return Size of the read message.
*/
int UART_UNIVERSAL_READ(UART_HandleTypeDef *huart, char* buffer, int buffer_size){


	uint8_t sample;
	while(1){
		if(end == 1){
			current = 0;
			end = 0;
		}
		// Prevent overflowing of buffer limits
		if(current >buffer_size-1) return current;
		// Read one byte at the time.
		if(HAL_UART_Receive(huart, &sample, 1, 2000) != HAL_OK) return 0;
		// Check for special characters and increment current
		if((sample>31) && (sample<127)) {buffer[current] = (char)sample; current++;}
		if(sample == '\r'){
			end = 1;
			return current;
		}
		// Makes it less blocking
		return 0;
	}
	// Should be here, something went wrong
	return 0;
}

