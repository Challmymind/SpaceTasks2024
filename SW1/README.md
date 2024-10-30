# Overview
WS2812B LED library. I've decided to do it differently than most common libraries do: <br>
[FastLED](https://github.com/FastLED/FastLED) - uses SPI. <br>
[Adafruit_NeoPixel ](https://github.com/adafruit/Adafruit_NeoPixel) - looks like it's using GPIO bit bang. <br><br>
Both designs have some pros and cons:
SPI is fast and reliable, but it wastes a whole interface just to control a strip of LEDs. <br>
Bit banging is much easier to port to other controllers and even families, but it can be unreliable and wastes a lot of CPU. <br>
CPU wasting in both designs can be adressed by DMA but then adds complexity to porting. <br>

I decided to use PWM + DMA as for me it seems like a good middle ground. Uses only one pin and does not compromise accuracy. Porting to other stm32 devices is also easy.
I also decided to make it compile time.

## Table of contents
* [Overview](#overview)
* [Library](#library)
  * [Hot to use](#how-to-use)
  * [Defines](#defines)
  * [Strip](#strip)
* [STM32G474RET App](#stm32g474ret-app)
* [Possible improvements](#possible-improvements)

# Library
The library is header only and lacks most of the features of the fully developed libraries available on the Internet. [Header can be found here](https://github.com/Challmymind/SpaceTasks2024/blob/main/SW1/WS2812B/Core/Inc/WS2812B.hpp).
I couldn't test if library in fact works because i lacked 1) LED strip, 2) Oscilloscope, 3) Logic analyzer (only one is needed). Nevertheless, I checked with STM32 DMA AN and PWM.

# How to use
Short tutorial on how to integrate my library.
1. Download header.
2. Include header in project.
3. Adjust defines for microcontroller if needed. [More here](#defines).
4. Create Strip object. [More here](#strip).
5. Supply Strip object with the modulation callback. [Also more here](#strip).
6. Add Strip::run() in main loop.
7. Enjoy

## Defines
Defaults assume 16Hz clock of STM32G474RET. To adapt to other MCUs, make sure that new timings are set with [WS2812B specification](https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf).
The header file uses ifndef guard for each define, allowing compile-time overrides. List of defines:
* HIGH_PWM_DUTY -> Needs to comply with timings for logical 1 WS2812B protocol. ount when PWM drop signal goes to low. Basically the width of the pulse. 
* LOW_PWM_DUTY  -> Needs to comply with timings for logical 0 WS2812B protocol. Count when PWM drop signal goes to low. Basically the width of the pulse.
* RES_PWM_DUTY  -> Width od the pulse for reset signal. Can be left at 0 always.

## Strip
Strip class requires information about the length of the strip at compile time. More importantly, how to modulate diods and create light patterns when the function calls callback.
```cpp
int (*_modulation)(LED* led, const int i, const int feedback)
```
The clever design that makes more complex patterns possible and easy is the use of feedback from other devices in the strip. Each run loop provides a callback function with LED abstraction, allowing the user to modulate colours, index in the strip to adjust for length-related patterns and, most importantly, returned value of the previous LED to use as feedback.<br>
Strip is a template class that requires compile time length to create an array.

# STM32G474RET APP
App implements WS2812B library and set's colors to desired patterns.
* Red is a square wave with a pulse width of 50%.
* Green is a sawtooth signal.
* Blue blinks 3 times when choosen gpio will be shorted.
```cpp
void update_click(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)) clicked = 1;
	else clicked = 0;
}
```

# Possible improvements
If it were to become a real library, a real application, it should at least implement things like brightness adjustment independent of modulation. GPIO reads should also be swapped to interrupts to ensure that nothing is missed.
