# What's the idea?
I wanted to create flexible library for creating proto shell with microcontrollers and i wanted to stick to 3 rules:
* Avoid at all cost dynamic allocation
* Don't force user to mess with library code
* Don't set boundries on avaible space <br>

Well this aproach sets some heavy boundries because normally one can pick only 2 points from this list: 

1+2 -> you limit user with some predefined types/arrays limits (which may lead to wasting space) <br>
1+3 -> in order to incorporate new funtions user must change library code by himself <br>
2+3 -> now we are in the world of dynamic allocation which isn't that bad not good either <br>

but there is solution... but you cannot learn such power from jedi

## To cheat the game
In order to pass all my 3 objectives i decided to cheat a little bit by modifing library code by a script!
That's right, my library code do not need any by-hand modifications in order to add new command, just add definition to the config file
and my script will re-generate library with proper callback pointer, callback setters and check in main execution function. User can even add description which 
is shown when help command is invoked. Library will handle tokenization and casting by itself (mostly). Now only what user must do is to set new callback using
generated setter.
## Preview on PC
![Preview!](https://github.com/Challmymind/SpaceTasks2024/blob/main/SW2/working_on_pc.png)

## How to use
1. Create config file
2. Generate code
   Tip for CubeIde! You can set pre-build step in order to make it automatic (as intended)
   ![Auto Auto!](https://github.com/Challmymind/SpaceTasks2024/blob/main/SW2/pre_build_tip.png)
4. Set obligatory functions setters in code
5. Set defined callbacks
6. Compile

## Example json config 
```
{
    "LIB_NAME"              :"EXAMPLE",
    "SHELL_MAX_ARGS"        : 20,
    "SHELL_MAX_TOKEN_LEN"   : 100,
    "CALLBACKS"             : [
        ["print", "Prints some text on the screen", ["char* text"]], 
        ["showtime", "Does something funny", ["int random", "int another"]]
    ],
    "INCLUDE_PATH"          : "./Inc",
    "SOURCE_PATH"           : "./Src"
}
```

## Integration with STM32G474RET3 WORKING
### How?
My librarary needs to read arbitrary amount of data from any source but HAL UART doesn't provide such function.
In order to make it simple i created additional uart_universial.hpp and uart_universial.cpp to provide solution.
Provided function is blocking meaning that it blocks thread but it's more like small delay because it saves state and returns whenever possible.
Only when transaction is completed function will report about results via return value.
### Testing
I was lacking UART-USB converted so i decided to use my rp pico h with [this library](https://github.com/Noltari/pico-uart-bridge). Worked perfectly.
Also i just tested one function because i supplied only one callback, others before initializing will result in Fault_Handler at best.
![Recaived UART!](https://github.com/Challmymind/SpaceTasks2024/blob/main/SW2/working_via_uart_STM32F401.png)
