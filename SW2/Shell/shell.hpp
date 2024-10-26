#ifndef __SHELL_HPP__
#define __SHELL_HPP__

#ifndef SHELL_BUFFER_SIZE
#define SHELL_BUFFER_SIZE 99
#endif

#ifndef SHELL_MAX_ARGS
#define SHELL_MAX_ARGS 101
#endif

#ifndef SHELL_MAX_TOKEN_LEN
#define SHELL_MAX_TOKEN_LEN 69
#endif

class Shell{
    public:
        int execute(char *ptr, int size);
        const char* getBuffer();
        void command_test_callback(void(*ff)(char* arg, int nargs));

        
    private:
        int tokenize(char *ptr, int size);
        int string_compare(const char* x, const char* y);
        char tokens[101][69];
        char buffer[99];
        void (*__command_test_callback)(char* arg, int nargs);

};

#endif