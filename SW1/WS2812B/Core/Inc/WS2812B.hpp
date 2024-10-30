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
 * About timings
 * Ideally the signal period should be 1250ns (800kHz). The PWM clock frequency is set to 16Mhz (62.5ns),
 * so to get the perfect period you have to divide the frequency by 20. Instead I will use period counter and set it to
 * (8+13-1)=20. Now 13 ticks is 812ns and 8 ticks is 500n which is close enough.
 *
 */
#define HIGH_PWM_DUTY 	13
#define LOW_PWM_DUTY 	7
#define RES_PWM_DUTY	0 	// Sends reset, informing LEDs that a new cycle is starting
#define PWM_DUTY_SIZE 	8 	// In the DMA settings, the size of the data is set to one byte, so that after each byte sent, the PWM updates it's duty.


/**
 * LED structure,
 * A 3 element array could be used instead, but I think it's more readable and minimises the risk of messing up the order of the colours.
 */
struct LED {
	uint8_t green;
    uint8_t red;
    uint8_t blue;
} __attribute__((packed));

/**
 * LED strip class.
 *
 * Manages strips of LEDs. Class uses callback to set leds on the fly. Each callback function is called with the led structure
 * representing the current led, the led index to help build patterns, and feedback from the last callback call.
 */
template<int N>
class Strip{

public:

	/**
	 * Initializes class.
	 * Class assumes that no new LEDs are added to a given strip after the initial strip run.
	 *
	 * @param N Number of LEDs in the controlled strip.
	 * @param tim Handler for using PWM with DMA.
	 * @ channel DMA to PWM channel.
	 */
	Strip(TIM_HandleTypeDef* tim, uint32_t channel){
		_tim = tim;
		_channel = channel;
	}

	/**
	 * Sets color modulation callback.
	 *
	 * Each callback function is called with the led structure
	 * representing the current led, the led index to help build patterns, and feedback from the last callback call.
	 * The function returns an int that is passed to the next LED via feedback. It is a support mechanism for building patterns.
	 *
	 * @param led Structure Pointer representing an LED. Apply colours to this object.
	 * @param i Current index of the LED in the strip. Can be used for patterns.
	 * @param feedback Feedback from last LED. Can be used for patterns. Exception: always 0 for first led.
	 *
	 * @return feedback that is passed to the next LED in the strip.
	 *
	 */
	void setLedCallback(int (*modulation)(LED* led,  const int i, const int feedback)){
		_modulation = modulation;
	}

	/**
	 * Runs main logic.
	 *
	 * Each run() call class modulates the whole strip according to the provided callback and sends data to the chosen PWM pin via DMA.
	 * Suitable for main loop execution. Callback function can be swapped between calls.
	 */
	void run(){
		if(_modulation == nullptr) return;

		LED led;
		int feedback;
		for(int i = 0; i < N; i++){
			feedback = _modulation(&led, i, feedback);
			_convert(led, i);
		}
		// Set last duty as reset code.
		_strip[_size] = RES_PWM_DUTY;
		HAL_TIM_PWM_Start_DMA(_tim, _channel, (uint32_t *)_strip, _size);
	}



private:

	/**
	 * Converts LED into WS2812B format.
	 *
	 * @param led LED to be converted.
	 * @param i index of the led in the _strip.
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
	 * Buffer representing physical strip via PWM duty array
	 *
	 * Size = (amount of LEDs * colors * bits for one color) + 1 res code.
	 */
	uint8_t _strip[(N*3*8)+1];
	uint32_t _size = (N*3*8)+1;

	/**
	 * Modulation function callback.
	 */
	int (*_modulation)(LED* led, const int i, const int feedback) = nullptr;

	/**
	* Handlers for transferring buffers via DMA to PWM.
	*/
	TIM_HandleTypeDef* _tim;
	uint32_t _channel;

};




#endif /* INC_WS2812B_HPP_ */
