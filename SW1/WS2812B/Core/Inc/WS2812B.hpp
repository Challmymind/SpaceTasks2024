/*
 * WS2812B.hpp
 *
 *  Created on: Oct 29, 2024
 *      Author: czupu
 */

#ifndef INC_WS2812B_HPP_
#define INC_WS2812B_HPP_

#include "stm32g4xx_hal.h"

/**
 * Timings
 * Ideally signal period should be 1250ns (800kHz). PWM clock frequency is set to 16Mhz (62.5ns) so in order to achieve perfect period frequency must be
 * divided by 20. It's not necessary because 13 ticks is 812ns and 8 ticks is 500ns which is close enough. (Period counter=(8+13-1)=20)
 *
 */
#define HIGH_PWM_DUTY 	13
#define LOW_PWM_DUTY 	7
#define RES_PWM_DUTY	0 	// Sends reset, informing leds that new cycle is starting
#define PWM_DUTY_SIZE 	8 	// In DMA settings size of data is set to one byte so after each sent byte PWM updates it's duty


/**
 * LED structure,
 * Could be used 3 element array instead but i thinks it's more readable and minimizes risks
 * of messing up color order.
 */
struct LED {
	uint8_t green;
    uint8_t red;
    uint8_t blue;
} __attribute__((packed));

/**
 * LED strip class.
 *
 * Manages strips of led. Class uses callback for settings leds on the fly.
 * Each callback function is called with led structure representing current led,
 * index of the led to aid build patterns, and feedback from last callback call.
 */
template<int N>
class Strip{

public:

	/**
	 * Initializes class.
	 *
	 * @param N Amount of leds in the controlled strip.
	 * @param tim Handler to use PWM with DMA.
	 * @ channel Channel for DMA to PWM.
	 */
	Strip(TIM_HandleTypeDef* tim, uint32_t channel){
		_tim = tim;
		_channel = channel;
	}

	/**
	 * Set up  color modulation.
	 *
	 * Each callback function is called with led structure representing current led,
	 * index of the led to aid build patterns, and feedback from last callback call.
	 * Function return int that will be passed to the next led via feedback. It is supportive
	 * mechanism for building patterns.
	 *
	 * @param led Structure pointer representing led. Apply colors to this object.
	 * @param i Current index of the led. Can be used for patterns.
	 * @param feedback Feedback from last led. Can be used for patterns. Exception: always 0 for first led;
	 *
	 * @return Returns feedback that will be passed to next led.
	 *
	 */
	void setLedCallback(int (*modulation)(LED* led,  const int i, const int feedback)){
		_modulation = modulation;
	}

	void run(){
		if(_modulation == nullptr) return;

		LED led;
		int feedback;
		for(int i = 0; i < N; i++){
			feedback = _modulation(&led, i, feedback);
			_convert(led, i);
		}
		_strip[_size] = RES_PWM_DUTY;
		HAL_TIM_PWM_Start_DMA(_tim, _channel, (uint32_t *)_strip, _size);
	}



private:

	/**
	 * Converts LED into WS2812B format.
	 *
	 * @param led Led to be converted
	 * @param i index of the led in the _strip
	 */
	void _convert(LED led, int i){

		for(uint8_t x=0; x<3;x++){

			for(uint8_t y=0; y<PWM_DUTY_SIZE; y++)
			{
				if(( ( ( (uint8_t*)(&led) )[x] >> y) & 0x01) == 1)
				{
					// Move to correct led and change its duty values
					_strip[(i*3*8)+(x*8)+(7-y)] = HIGH_PWM_DUTY;
				}
				else
				{
					_strip[(i*3*8)+(x*8)+(7-y)] = LOW_PWM_DUTY;
				}
			}

		}
	}

	/**
	 * Represents physical strip via duty buffer.
	 *
	 * Size = (amount of leds * colors * bits for one color) + 1 res code.
	 */
	uint8_t _strip[(N*3*8)+1];
	uint32_t _size = (N*3*8)+1;

	/**
	 * Modulation callback.
	 */
	int (*_modulation)(LED* led, const int i, const int feedback) = nullptr;

	/**
	* Data for transferring buffers via DMA to PWM.
	*/
	TIM_HandleTypeDef* _tim;
	uint32_t _channel;

};




#endif /* INC_WS2812B_HPP_ */
