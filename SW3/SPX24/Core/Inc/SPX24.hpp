/*
 * SPX24.hpp
 *
 *  Created on: Oct 31, 2024
 *      Author: czupu
 */
#ifndef INC_SPX24_HPP_
#define INC_SPX24_HPP_

#include <stm32g4xx_hal.h>

/**
* SPX24 sensor related functions.
*/
namespace SPX24{

enum PARAMETER{
	MOISTURE,
	PH,
	TEMPERATURE
};

/**
* Prepares the SPX24 to read the selected soil parameter.
*
* @param x Selected parameter to be read.
* @param CTRL0_PORT port of the pin GPIO used as CLK0.
* @param CTRL0_PIN  pin used as CLK0.
* @param CTRL1_PORT port of the pin GPIO used as CLK1.
* @param CTRL1_PIN  pin used as CLK1.
* @param TR_PORT port of pin used as a trigger to indicate to the sensor to start the measurement.
* @param TR_PORT pin used as a trigger to indicate to the sensor to start the measurement.
*/
void readSensor(PARAMETER x,
		GPIO_TypeDef *CTRL0_PORT, uint16_t CTRL0_PIN,
		GPIO_TypeDef *CTRL1_PORT, uint16_t CTRL1_PIN,
		GPIO_TypeDef *TR_PORT, uint16_t TR_PIN);

/**
* Provides information about the last parameter requested.
*
* @return last requested parameter.
*
*/
PARAMETER getCurrentSetSensor();

/**
* Converts raw ADC values (assuming 12 bit resolution) to float.
*
* @param raw read from ADC.
* @return Logarithmic float value representing soil pH.
*/
float phConversion(uint32_t raw);

/**
* Converts raw ADC values (assuming 12 bit resolution) to float.
*
* @param raw read from ADC.
* @return return Exponential value representing the temperature
*/
float tempConversion(uint32_t raw);

/**
* Converts raw ADC values (assuming 12 bit resolution) to float.
*
* @param raw read from ADC.
* @return return a float value in the range 0-1 representing the humidity percentage.
*/
float moistConversion(uint32_t raw);


}




#endif /* INC_SPX24_HPP_ */
