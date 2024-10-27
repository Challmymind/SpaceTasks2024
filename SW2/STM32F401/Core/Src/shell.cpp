#include "shell.hpp"

// Must be inside source file to prevent ODR violation
const char* HELP_OUTPUT = "print -> Prints some text on the screen\n\r\
showtime -> Does something funny\n\r\
help -> Prints all commands and their descriptions\n\r\
";

/**
 * Tokenize input.
 *
 * Input as one long string with white spaces isn't that helpful.
 * This funtions splits input into smaller _tokens. Max _tokens count is defined by SHELL_MAX_ARGS
 * Max token size is defined by SHELL_MAX_TOKEN_LEN
 *
 * @param ptr Pointer to string to be tokenized, can be any input ex. string from UART buffer.
 * @param size Size of the ptr, prevents out of bounds errors.
 * @return number of read _tokens, -1 if some error occured (like invalid syntax)
 */
int Shell::tokenize(const char* ptr, int size){
    if(size == 0) return -1;
    // Tracks last accessed index
    int current = 0;
    // Search for command name
    int x = 0;
    // Needs to deduct 1 from length in order to make sure we can terminate string
    while(true){
        // Abort if no proper syntax has been found to the end of the string
        if(current >= size) return -1;
        if(x >= SHELL_MAX_TOKEN_LEN-1) return -1;
        char c = ptr[current];
        // Attempt to clear input from artifacts if any has been added
        if((c >= 127) || (c<=31)) {current++;continue;}
        // Command name ends with "[=" characters so we jump ahead 2 index to the start of args
        if(c == '[') {
            current += 2;
            _tokens[0][x] = '\0'; // Indicate end of the string
            break;
        }
        else {
            _tokens[0][x] = c;
            current++;
            x++;
        }
    }
    
    // Tokenizing arguments
    for(int i = 1; i<SHELL_MAX_ARGS; i++){
        // Overshadowing last x variable
        int x = 0;
        // Needs to deduct 1 from length in order to make sure we can terminate string
        while(true){
            // Abort if no proper syntax has been found to the end of the string
            if(current >= size) return -1;
            if(x >= SHELL_MAX_TOKEN_LEN-1) return -1;

            char c = ptr[current];
            // Special case
            if(c == ']'){
                current++;
                _tokens[i][x] = '\0'; // Indicate end of the string
                return (i+1);
            }
            // Attempt to clear input from artifacts if any has been added
            if((c >= 127) || (c<=31)) {current++;continue;}
            // Command arg ends with ';' character
            if(c == ';') {
                current++;
                _tokens[i][x] = '\0'; // Indicate end of the string
                break;
            }
            else {
                _tokens[i][x] = c;
                current++;
                x++;
            }
        }
        
    }
    return SHELL_MAX_ARGS-1;
}

/**
 * Sets internal string comparator.
 *
 * Comparator should return 0 if strings are equal, all others outputs will be treated as failure.
 * It must be set explicit before executing Shell::execute to prevent undefined behaviour.
 *
 * @param ff Pointer to comparator function.
 */
void Shell::setStringCompare(int (*ff)(const char *, const char *)){
    __string_compare = ff;
}

/**
 * Execute shell's main logic.
 *
 * Checks for commands and executes callback if any has been found.
 *
 * @param ptr Pointer to string that must be parsed, can be any input ex. string from UART buffer.
 * @param size Size of the ptr, prevents out of bounds errors.
 * @return -1 if there wasn't any token inside ptr, 0 if no command has been executed, 1 if some command has been executed.
 */
int Shell::execute(const char *ptr, int size){
    int nargs = tokenize(ptr, size);
    if(nargs == 0) return -1;
    if(__string_compare(_tokens[0],"print")==0) {
        if (nargs < 1) return -2;
        __command_print_callback(_tokens[1]);
        return 1;
    }
	if(__string_compare(_tokens[0],"showtime")==0) {
        if (nargs < 2) return -2;
        __command_showtime_callback(_tokens[1], _tokens[2]);
        return 1;
    }
	if(__string_compare(_tokens[0],"help")==0) {
        if (nargs < 0) return -2;
        __command_help_callback();
        return 1;
    }
	
    return 0;
 
}

// Sets callback for the print command
void Shell::setCommand_print_Callback(void(*ff)(const char* text)){
    __command_print_callback = ff;
}
// Sets callback for the showtime command
void Shell::setCommand_showtime_Callback(void(*ff)(const char* random ,const char* another)){
    __command_showtime_callback = ff;
}
// Sets callback for the help command
void Shell::setCommand_help_Callback(void(*ff)()){
    __command_help_callback = ff;
}
