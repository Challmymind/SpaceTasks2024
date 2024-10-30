#include "shell.hpp"

// Must be inside source file to prevent ODR violation
const char* HELP_OUTPUT = "print -> Prints some text on the screen\n\r\
showtime -> Does something funny\n\r\
help -> Prints all commands and their descriptions\n\r\
";

/**
* Tokenise input.
*
* This function splits the input string into smaller tokens. 
* The maximum number of tokens is defined by SHELL_MAX_ARGS. 
* The maximum size of the token is defined by SHELL_MAX_TOKEN_LEN.
*
* @param ptr Pointer to string to tokenise, can be any input e.g. string from UART buffer.
* @param size Size of the ptr, prevents out of bounds errors.
* @return The number of tokens that were read.
*/
int Shell::tokenize(const char* ptr, int size){
    if(size == 0) return -1;
    // Last access track index
    int current = 0;
    // Search for command name
    int x = 0;
    // Must subtract 1 from length to ensure string can be terminated
    while(true){
        // Abort if no correct syntax was found in the string
        if(current >= size) return -1;
        if(x >= SHELL_MAX_TOKEN_LEN-1) return -1;
        char c = ptr[current];
        // Attempt to remove artefacts from the input if any have been added. 
        // Assuming clean input might be better, forcing the end user to think about what they are doing.
        if((c >= 127) || (c<=31)) {current++;continue;}
        // Command name ends with "[=" characters, so we jump 2 index to the beginning of the arguments.
        // Can lead to undefined behaviour if the syntax is incorrect, critical part, must be fixed.
        #warning unfixed possible source of error.
        if(c == '[') {
            current += 2;
            _tokens[0][x] = '\0'; // Terminate string
            break;
        }
        else {
            _tokens[0][x] = c;
            current++;
            x++;
        }
    }
    
    // Tokenising arguments
    for(int i = 1; i<SHELL_MAX_ARGS; i++){
        // Overshadowing last x variable
        int x = 0;
        while(true){
            // Abort if no correct syntax was found in the string
            if(current >= size) return -1;
            if(x >= SHELL_MAX_TOKEN_LEN-1) return -1;

            char c = ptr[current];
            // Special case
            if(c == ']'){
                current++;
                _tokens[i][x] = '\0'; // Terminate string
                return (i+1);
            }
            // Attempt to remove artefacts from the input if any have been added. 
            // Assuming clean input might be better, forcing the end user to think about what they are doing.
            if((c >= 127) || (c<=31)) {current++;continue;}
            // Command arguments end with ';' character
            if(c == ';') {
                current++;
                _tokens[i][x] = '\0'; // Terminate string
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
* Sets the internal string comparator.
*
* The comparator should return 0 if the strings are equal, all other outputs are treated as not equal. 
* It must be set explicitly before calling Shell::execute to avoid undefined behaviour.
*
* @param ff Pointer to the comparator function.
*/
void Shell::setStringCompare(int (*ff)(const char *, const char *)){
    __string_compare = ff;
}

/**
* Execute the main logic of the shell.
*
* Checks for commands and performs a callback if one is found.
*
* @param ptr Pointer to string to parse, can be any input e.g. string from UART buffer.
* @param size Size of the ptr, prevents out of bounds errors.
* @return -1 if there's no token in ptr, 0 if no command was executed, 1 if any command was executed.
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
