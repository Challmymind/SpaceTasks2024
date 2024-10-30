# Overview
WS2812B LED library. I've decided to do it differently than most common libraries do: <br>
[FastLED](https://github.com/FastLED/FastLED) - uses SPI. <br>
[Adafruit_NeoPixel ](https://github.com/adafruit/Adafruit_NeoPixel) - looks like it's using GPIO bit bang. <br><br>
Both disigns have some pros and cons:
SPI is fast and reliable, but it wastes a whole interface just to control a strip of LEDs. <br>
Bit banging is much easier to port to other controllers and even families, but it can be unreliable and waste a lot of CPU. <br>
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
TODO
## Strip
TODO

# STM32G474RET APP
TODO

# Possible improvements
TODO
