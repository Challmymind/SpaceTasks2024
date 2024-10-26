#include "shell.hpp"

const char* Shell::getBuffer(){
    return buffer;
}

int Shell::tokenize(char* ptr, int size){
    if(size == 0) return 0;
    int current = 0;
    for(int x = 0; x<SHELL_MAX_ARGS; x++){
        int token_current = 0;
        while ((ptr[current] > 32) && (ptr[current] < 127)) {
            if(token_current >= SHELL_MAX_TOKEN_LEN-1) {
                tokens[x][token_current] = '0';
                continue;
            }
            tokens[x][token_current] = ptr[current];
            token_current++;
            current++;
        }
        if(ptr[current] == 32) {
            tokens[x][token_current] = '0';
            continue;
        }
        else return x;
    }
    return SHELL_MAX_ARGS-1;
}

int Shell::execute(char *ptr, int size){
    int nargs = tokenize(ptr, size);
    if(nargs == 0) return -1;
    if(string_compare(tokens[0],"test")) {
        __command_test_callback(tokens[0],nargs);
        return 1;
    }

    return 0;
 
}