/*
 * uart_universal.hpp
 *
 *  Created on: Oct 27, 2024
 *      Author: czupu
 */

#ifndef INC_UART_UNIVERSAL_HPP_
#define INC_UART_UNIVERSAL_HPP_
#include <stm32g4xx_hal.h> // swapped from generic "main.h

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
int UART_UNIVERSAL_READ(UART_HandleTypeDef *huart, char* buffer, int buffer_size);


#endif /* INC_UART_UNIVERSAL_HPP_ */
