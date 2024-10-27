/*
 * uart_universal.hpp
 *
 *  Created on: Oct 27, 2024
 *      Author: czupu
 */

#ifndef INC_UART_UNIVERSAL_HPP_
#define INC_UART_UNIVERSAL_HPP_
#include <stm32g4xx_hal.h>

/**
* UART variable length read.
*
* BLOCKING FUNCTION
* HAL implementations requires to declare how much bytes are expected.
* UART_UNIVERSAL_READ reads all input until finds special ASCII symbol like \r or \n
* Max size of read message is the size of the buffer, too small buffer can lead to undefined  of dependent functions.
*
* @param buffer_size Max size of read message.
* @param huart UART handler to be used.
* @param buffer Buffer to store read message.
* @return Size of the read message.
*/
int UART_UNIVERSAL_READ(UART_HandleTypeDef *huart, char* buffer, int buffer_size);


#endif /* INC_UART_UNIVERSAL_HPP_ */
