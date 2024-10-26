#ifndef __EXAMPLE_HPP__
#define __EXAMPLE_HPP__

#ifndef SHELL_BUFFER_SIZE
#define SHELL_BUFFER_SIZE 255
#endif

#ifndef SHELL_MAX_ARGS
#define SHELL_MAX_ARGS 20
#endif

#ifndef SHELL_MAX_TOKEN_LEN
#define SHELL_MAX_TOKEN_LEN 100
#endif

class Shell{
    public:

        /**
        * Execute shell's main logic.
        *
        * Checks for commands and executes callback if any has been found.
        *
        * @param ptr Pointer to string that must be parsed, can be any input ex. string from UART buffer.
        * @param size Size of the ptr, prevents out of bounds errors.
        * @return -1 if there wasn't any token inside ptr, 0 if no command has been executed, 1 if some command has been executed.
        */
        int execute(char *ptr, int size);

        /**
        * Sets internal string comparator.
        *
        * Comparator should return 0 if strings are equal, all others outputs will be treated as failure.
        * It must be set explicit before executing Shell::execute to prevent undefined behaviour.
        *
        * @param ff Pointer to comparator function.
        */
        void setStringCompare(int (*ff)(const char* x, const char* y));

        const char* getBuffer();

        // Sets callback for the print command
        void setCommand_print_Callback(void(*ff)(char* arg, int nargs));
		// Sets callback for the showtime command
        void setCommand_showtime_Callback(void(*ff)(char* arg, int nargs));
		
        
    private:

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
        int tokenize(char *ptr, int size);
        
        int (*__string_compare)(const char* x, const char* y);
        char tokens[20][100];
        char buffer[255];
        void (*__command_print_callback)(char* arg, int nargs);
		void (*__command_showtime_callback)(char* arg, int nargs);
		
};

#endif