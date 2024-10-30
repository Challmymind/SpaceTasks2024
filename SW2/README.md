# Overview
I wanted to create a flexible library for creating proto-shells with microcontrollers and I wanted to stick to 3 constraints:
1. Avoid dynamic allocation at all costs.
2. Don't force users to deal with library code (provide the abstraction to interact with it).
3. Don't set limits on the space available (e.g. maximum 30 commands defined). <br>

Well, this approach sets some strict limits, because normally you can only choose 2 items from this list.: 

1+2 -> limits users with some predefined types/array sizes (which can lead to wasted/unused space) <br>
1+3 -> to add new functionality, users must modify the library code themselves. <br>
2+3 -> dynamic allocation :) <br>

There is a solution... But such power cannot be learned from Jedi. Generate code to compile based on config.
That's what my library does, the user defines the command, parameters and description in the config file and my script provides him with code based on the callbacks he needs to set.

## Table of contents
* [Overview](#overview)
* [Example](#example)
  * [PC preview](#pc-preview)
* [Hot to use](#how-to-use)
  * [Shell config](#shell-config)
  * [Callbacks](#callbacks)
  * [Trick for STM32CubeIDE](#trick-for-stm32cubeide)
  * [Input](#input)
    * [UARTUniversal](#uartuniversal)
* [STM32 Blackpill](#stm32-blackpill)
* [STM32G474RET Library](#stm32g474ret-library)
* [Possible improvements](#possible-improvements)

# Example
My library code does not need any by hand modifications to add new command, just add definition to config file
and my script will regenerate the library with the correct callback pointers, callback setters and checks in the main execution function.
Library takes care of tokenisation of string. Now all the user has to do is set a new callback using the generated setter and provide the shell with input from the user.
This is helped by the creation of UARTUniversal for STM32 HAL-based applications that require UART input.
## PC preview
Shell has been generated using [example config](https://github.com/Challmymind/SpaceTasks2024/blob/main/SW2/Shell/shell_config.json).
![Preview!](https://github.com/Challmymind/SpaceTasks2024/blob/main/SW2/working_on_pc.png)

# How to use
Short tutorial on how to integrate my library.

1. Download Shell library (and UARTUniversal if you need).
2. Create your project and place shell code in it.
3. Adjust config to your needs. [More about config](#shell-config).
4. Generate library code. If using STM32CubeIDE use [this](#trick-for-stm32cubeide) trick.
5. Create a shell object in your code and set all [generated callbacks](#callbacks).
6. Place Shell::execute() in the main loop to be called with the [user's input](#input).
7. Enjoy. After config change regenerate the shell code and adjust the callbacks.

## Shell config
```json
{
    "LIB_NAME"              :"EXAMPLE",
    "SHELL_MAX_ARGS"        : 20,
    "SHELL_MAX_TOKEN_LEN"   : 100,
    "CALLBACKS"             : [
        ["print", "Prints some text on the screen", ["const char* text"]], 
        ["showtime", "Does something funny", ["const char* random", "const char* another"]],
        ["help", "Prints all commands and their descriptions", []]
    ],
    "INCLUDE_PATH"          : "./example",
    "SOURCE_PATH"           : "./example",
    "_COMMENT": ""
}
```
LIB_NAME - names of the generated files (.cpp, .hpp), please provide them in upper case for a better parsing. <br>
SHELL_MAX_ARGS - maximum number of parsed arguments during tokenisation, upper limit of arguments for any defined command. <br>
SHELL_MAX_TOKEN_LEN - maximum number of characters in a given token, upper limit for argument length. <br>
CALLBACKS - list of commands to be declared with strcuture: COMMAND_NAME,DESCRIPTION,ARGUMENTS. The script will only generate callback setters for defined commands. [More about generation](#callbacks). <br>
INCLUDE_PATH - directory where generated .hpp files will be stored. The directory must exist. <br>
SOURCE_PATH - directory where generated .cpp files will be stored. The directory must exist. <br>
All other fields will be omitted. <br>

## Callbacks
Script generates a almost ready-to-use library that requires callbacks to be set using callback setters. [Example of generated code](https://github.com/Challmymind/SpaceTasks2024/blob/main/SW2/Shell/example/example.hpp).
library always requires a string comparator to be set using ```Shell::setStringCompare(int (*ff)(const char* x, const char* y))``` and all defined commands callbacks. Generated setter looks like that:
```hpp
/** Sets a callback for the command {name).
* Config description: {desc}
*/
void setCommand_{name}_Callback(void(*ff)({args}));
```
name - name provided to the script via config using the callback list. <br>
desc - description assigned to the command in config. <br>
args - arguments assigned to the command in config. <br>
<br>
Example of setting callback:
```cpp
void print(const char* string){
	HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 2000);
}
int main(){
	Shell shell;
	shell.setCommand_print_Callback(print);
}
```

## Trick for STM32CubeIDE
To make building more automatic, call a Python shell script like this as a pre-build step.
![trick!](https://github.com/Challmymind/SpaceTasks2024/blob/main/SW2/pre_build_tip.png)

## Input
User input must be provided as a string with it's size to the ```Shell::execute``` function. Example:
```cpp
	// Read message from the UART
	int bytes = UART_UNIVERSAL_READ(&huart1, (char*)rx_buff, 255);
	// Perform main shell function
	if(bytes > 0) shell.execute((char*)rx_buff, bytes);
```
### UARTUniversal
This useful library has only one function ```int UART_UNIVERSAL_READ(UART_HandleTypeDef *huart, char* buffer, int buffer_size)``` which when called
will periodically read the available input in the UART buffer and will not report the end of the message until the \r symbol has been found or the maximum size of the provided buffer has been reached.
Suggested for use when integrating shell with stm32 uart hal. Example can be found [here](https://github.com/Challmymind/SpaceTasks2024/blob/main/SW2/STM32F401/Core/Src/main.cpp).

# STM32 Blackpill
Library was tested on stm32 blackpill. I lacked nucleo to test with UART2 or usb-uart converter so I used rpi pi h with this [library](https://github.com/Noltari/pico-uart-bridge).
Code for this example avaible [here](https://github.com/Challmymind/SpaceTasks2024/tree/main/SW2/STM32F401).
![Recaived UART!](https://github.com/Challmymind/SpaceTasks2024/blob/main/SW2/working_via_uart_STM32F401.png)
![Tweak!](https://github.com/Challmymind/SpaceTasks2024/blob/main/SW2/final_help_tweaks.png)
![Console!](https://github.com/Challmymind/SpaceTasks2024/blob/main/SW2/uart_screen.png)

# STM32G474RET Library
It's the encapsulation of all components (shell + auto-regenerate + UARTUniversal). <br>
To use:
1. Build project
2. Link builded *.a file to your project.
3. Include all includes
4. Connect all elements like in example in your desired function.
   * Set [callbacks](#callbacks)
   * Integrate [UARTUniversal](#uartuniversal) for UART input.
   * Place Shell::execute in main loop.
5. Build your project.
6. To update library change config and rebuild and go to step 2.

# Possible improvements
For example, I have highlighted one particular function that I know can be a source of errors. If I had more time, I would focus on critical parts like this, then I would implement dynamic casting from strings as callbacks to allow for 
arguments with diffrent types. I even started doing it, which left artefacts that can be seen in [Python script](https://github.com/Challmymind/SpaceTasks2024/blob/main/SW2/Shell/shell_generator.py), but scraped because I had to do other tasks.
