#ifndef __EXAMPLE_HPP__
#define __EXAMPLE_HPP__

#ifndef SHELL_MAX_ARGS
#define SHELL_MAX_ARGS 20
#endif

#ifndef SHELL_MAX_TOKEN_LEN
#define SHELL_MAX_TOKEN_LEN 100
#endif


/**
* Help output.
*
* String compiled from definitions provided in config file for each callback.
* Can be used to implement descriptions printing.
*/
extern const char* HELP_OUTPUT;


/**
* Shell class for implementing callbacks.
*
* Provides a simple way to call user defined callbacks without manually modifing source code.
* Makes it easier new maintainers to add commands on the fly using config file without knowning internal structure.
* Shell goal is to become envoirment agnostic but free of source code modifications in order to port to desire end device.
*/
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
        int execute(const char *ptr, int size);

        /**
        * Sets internal string comparator.
        *
        * Comparator should return 0 if strings are equal, all others outputs will be treated as failure.
        * It must be set explicit before executing Shell::execute to prevent undefined behaviour.
        *
        * @param ff Pointer to comparator function.
        */
        void setStringCompare(int (*ff)(const char* x, const char* y));

        // Sets callback for the print command
        void setCommand_print_Callback(void(*ff)(const char* text));
		// Sets callback for the showtime command
        void setCommand_showtime_Callback(void(*ff)(const char* random ,const char* another));
		// Sets callback for the help command
        void setCommand_help_Callback(void(*ff)());
		
        
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
        int tokenize(const char *ptr, int size);

        /**
        * Converts string to int.
        *
        * Example: Converts "1234" to 1234
        * Detects end of the string using \0
        *
        * @param str Pointer to the string to be converted
        * @return Converted number.
        */
        //int __to_int(const char* str);

        int (*__string_compare)(const char* x, const char* y);
        char _tokens[20][100];
        void (*__command_print_callback)(const char* text);
		void (*__command_showtime_callback)(const char* random ,const char* another);
		void (*__command_help_callback)();
		
};

#endif