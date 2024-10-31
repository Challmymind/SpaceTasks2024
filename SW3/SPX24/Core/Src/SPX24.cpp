/*
 * SPX24.cpp
 *
 *  Created on: Oct 31, 2024
 *      Author: czupu
 */

#include "SPX24.hpp"
#include <cmath>

static SPX24::PARAMETER _last_requested;

void SPX24::readSensor(SPX24::PARAMETER x,
		GPIO_TypeDef *CTRL0_PORT, uint16_t CTRL0_PIN,
		GPIO_TypeDef *CTRL1_PORT, uint16_t CTRL1_PIN,
		GPIO_TypeDef *TR_PORT, uint16_t TR_PIN)

{
	switch(x){
	case SPX24::PARAMETER::MOISTURE:
		HAL_GPIO_WritePin(CTRL0_PORT, CTRL0_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CTRL1_PORT, CTRL1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(TR_PORT, TR_PIN, GPIO_PIN_SET);
		_last_requested = x;
		break;
	case SPX24::PARAMETER::PH:
		HAL_GPIO_WritePin(CTRL0_PORT, CTRL0_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CTRL1_PORT, CTRL1_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(TR_PORT, TR_PIN, GPIO_PIN_SET);
		_last_requested = x;
		break;
	case SPX24::PARAMETER::TEMPERATURE:
		HAL_GPIO_WritePin(CTRL0_PORT, CTRL0_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CTRL1_PORT, CTRL1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(TR_PORT, TR_PIN, GPIO_PIN_SET);
		_last_requested = x;
		break;
	}

}

SPX24::PARAMETER SPX24::getCurrentSetSensor(){
	return _last_requested;
}


float SPX24::phConversion(uint32_t raw){
	return 7 + std::log10(float(raw)/2048.f);
}


float SPX24::tempConversion(uint32_t raw){
	return 273*std::exp(0.25f*float(raw)/3.3);
}


float SPX24::moistConversion(uint32_t raw){
	return float(raw)/4096.f;
}




