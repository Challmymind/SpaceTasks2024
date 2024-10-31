# Overview
Reading soil parameters using SPX24 sensor. To make it non-blocking I used interrupts.

## Table of contents
* [Overview](#overview)
* [How it works](#how-it-works)
* [Parsing output](#parsing-output)
* [STM32G474RET](#stm32g474ret)
* [Possible improvements](#possible-improvements)

# How it works
In order to read data from the sensor as quickly as possible, I had to use interrupts. A potential danger would be if interrupts were called too often, 
starving other processes, but 10ms timing to register a sensor trigger is enough time to prevent this. I used two interrupts for two tasks:
* to detect if data is ready using external interrupt (EXTI)
* to detect the end of ADC reading
After the first initialisation of the sensor read, the program continues until the sensor fires an external interrupt handler.
Then the handler will start the ADC read using the interrupt (the ADC will fire the interrupt to indicate the end of the read).
On completion, the ADC handler will read the data, store it to the appropriate variable and start another sensor read with a different parameter set.
Essentially, a loop is created after the first call to read the sensor.

# Parsing output
To parse the raw ADC output to units defined by SPX25, the user must use conversion functions within the SPX24 namespace.
```cpp
float phConversion(uint32_t raw);
float tempConversion(uint32_t raw);
float moistConversion(uint32_t raw);
```
[All helping functions are defined here.](https://github.com/Challmymind/SpaceTasks2024/blob/main/SW3/SPX24/Core/Inc/SPX24.hpp)

# STM32G474RET
To make it easier to read the code, I have used pin labels with names that match the pin names on the SPX24 datasheet.

# Possible improvements
To make it better I could try to make it into a class and encapsulate all the variables, but it's harder to make it modular because of the use of interrupts.
Also, the ADC interrupt could be changed to a DMA interrupt that fires new sensor reads after registering that new data has been read.
