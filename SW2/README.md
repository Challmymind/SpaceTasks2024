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

## How to use
1. Create config file
2. Generate code
3. Set obligatory functions setter in code
4. Set defined callbacks
5. Compile

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

## Integration with STM32G474RET3 incoming
