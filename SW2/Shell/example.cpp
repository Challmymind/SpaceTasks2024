#include "example.hpp"

const char* Shell::getBuffer(){
    return buffer;
}

/**
 * Tokenize input.
 *
 * Input as one long string with white spaces isn't that helpful.
 * This funtions splits input into smaller tokens. Max tokens count is defined by SHELL_MAX_ARGS
 * Max token size is defined by SHELL_MAX_TOKEN_LEN
 *
 * @param ptr Pointer to string to be tokenized, can be any input ex. string from UART buffer.
 * @param size Size of the ptr, prevents out of bounds errors.
 * @return number of read tokens.
 */
int Shell::tokenize(char* ptr, int size){
    if(size == 0) return 0;
    int current = 0;
    for(int x = 0; x<SHELL_MAX_ARGS; x++){
        int token_current = 0;
        while ((ptr[current] > 32) && (ptr[current] < 127)) {
            if(token_current >= SHELL_MAX_TOKEN_LEN-1) {
                tokens[x][token_current] = '\0';
                continue;
            }
            tokens[x][token_current] = ptr[current];
            token_current++;
            current++;
        }
        if(ptr[current] == 32) {
            tokens[x][token_current] = '\0';
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
    if(__string_compare(tokens[0],"print")) {
        __command_print_callback(tokens[0],nargs);
        return 1;
    }
	if(__string_compare(tokens[0],"showtime")) {
        __command_showtime_callback(tokens[0],nargs);
        return 1;
    }
	
    return 0;
 
}

// Sets callback for the print command
void Shell::setCommand_print_Callback(void(*ff)(char* arg, int nargs)){
    __command_print_callback = ff;
}
// Sets callback for the showtime command
void Shell::setCommand_showtime_Callback(void(*ff)(char* arg, int nargs)){
    __command_showtime_callback = ff;
}
