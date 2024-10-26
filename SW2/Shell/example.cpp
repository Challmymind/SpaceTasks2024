#include "example.hpp"

/**
 * Tokenize input.
 *
 * Input as one long string with white spaces isn't that helpful.
 * This funtions splits input into smaller _tokens. Max _tokens count is defined by SHELL_MAX_ARGS
 * Max token size is defined by SHELL_MAX_TOKEN_LEN
 *
 * @param ptr Pointer to string to be tokenized, can be any input ex. string from UART buffer.
 * @param size Size of the ptr, prevents out of bounds errors.
 * @return number of read _tokens.
 */
int Shell::tokenize(char* ptr, int size){
    if(size == 0) return 0;
    int current = 0;
    for(int x = 0; x<SHELL_MAX_ARGS; x++){
        int token_current = 0;
        while ((ptr[current] > 32) && (ptr[current] < 127)) {
            if(token_current >= SHELL_MAX_TOKEN_LEN-1) {
                _tokens[x][token_current] = '\0';
                continue;
            }
            _tokens[x][token_current] = ptr[current];
            token_current++;
            current++;
        }
        if(ptr[current] == 32) {
            _tokens[x][token_current] = '\0';
            continue;
        }
        else return x;
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
* Sets internal write operation.
*
* This command is specified by end user of the library, it can do almost everything.
* The idea is that caller can implement writing directly by peripheral or just store output to buffer.
*
* @param ff Pointer to the write function.
* @param str Pointer to the string to be written.
* @param len Length of the string.
*/
void Shell::setWrite(int (*ff)(const char* str, int len)){
    __write = ff;
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
int Shell::execute(char *ptr, int size){
    int nargs = tokenize(ptr, size);
    if(nargs == 0) return -1;
    if(__string_compare(_tokens[0],"help")==0) {
        __write(_help_output, sizeof(_help_output));
        return 1;
    }
    if(__string_compare(_tokens[0],"print")==0) {
        if (nargs < 1) return -2;
        __command_print_callback(_tokens[0]);
        return 1;
    }
	if(__string_compare(_tokens[0],"showtime")==0) {
        if (nargs < 2) return -2;
        __command_showtime_callback(__to_int(_tokens[0]), __to_int(_tokens[1]));
        return 1;
    }
	
    return 0;
 
}

// Sets callback for the print command
void Shell::setCommand_print_Callback(void(*ff)(char* text)){
    __command_print_callback = ff;
}
// Sets callback for the showtime command
void Shell::setCommand_showtime_Callback(void(*ff)(int random ,int another)){
    __command_showtime_callback = ff;
}
