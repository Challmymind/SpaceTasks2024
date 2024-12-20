#ifndef __SHELLGEN_HPP__
#define __SHELLGEN_HPP__

#ifndef SHELL_MAX_ARGS
#define SHELL_MAX_ARGS 20
#endif

#ifndef SHELL_MAX_TOKEN_LEN
#define SHELL_MAX_TOKEN_LEN 100
#endif


/**
* String compiled from definitions provided in the config file for each callback. 
* Can be used to implement printing of descriptions.
*/
extern const char* HELP_OUTPUT;


/**
* Shell class based on the implementation of callbacks.
*
* Provides an easy way to invoke custom callbacks without manually modifying the source code. 
* Makes it easier for new maintainers to add commands on the fly using the config file without knowing the internal structure. 
* Shell goal is to become envoirment agnostic, so no need for additional porting of the class itself.
*/
class Shell{
    public:

        /**
        * Execute the main logic of the shell.
        *
        * Checks for commands and performs a callback if one is found.
        *
        * @param ptr Pointer to string to parse, can be any input e.g. string from UART buffer.
        * @param size Size of the ptr, prevents out of bounds errors.
        * @return -1 if there's no token in ptr, 0 if no command was executed, 1 if any command was executed.
        */
        int execute(const char *ptr, int size);

        /**
        * Sets the internal string comparator.
        *
        * The comparator should return 0 if the strings are equal, all other outputs are treated as not equal. 
        * It must be set explicitly before calling Shell::execute to avoid undefined behaviour.
        *
        * @param ff Pointer to the comparator function.
        */
        void setStringCompare(int (*ff)(const char* x, const char* y));

        /** START OF THE AUTO-GENERATED CALLBACK SETTERS. */

        /** Sets a callback for the command print.
        * Config description: Prints some text on the screen.
        */
        void setCommand_print_Callback(void(*ff)(const char* text));

		/** Sets a callback for the command showtime.
        * Config description: Does something funny.
        */
        void setCommand_showtime_Callback(void(*ff)(const char* random ,const char* another));

		/** Sets a callback for the command help.
        * Config description: Prints all commands and their descriptions.
        */
        void setCommand_help_Callback(void(*ff)());

		
        /** END OF THE AUTO-GENERATED CALLBACK SETTERS. */
        
    private:

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
        int tokenize(const char *ptr, int size);

        /**
        * Pointer to the compare function. Make sure it's set before Shell::execute.
        */
        int (*__string_compare)(const char* x, const char* y);

        /**
        * Buffer for tokens.
        */
        char _tokens[20][100];

        /** START OF THE AUTO-GENERATED CALLBACK POINTERS. */

        void (*__command_print_callback)(const char* text);
		void (*__command_showtime_callback)(const char* random ,const char* another);
		void (*__command_help_callback)();
		
        /** END OF THE AUTO-GENERATED CALLBACK FUNCTIONS. */
};

#endif