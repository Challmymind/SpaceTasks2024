import json
import sys

# Script variables (defaults)
variables = {
"LIB_NAME"              :"SHELL",
"SHELL_BUFFER_SIZE"     : "99",
"SHELL_MAX_ARGS"        :"101",
"SHELL_MAX_TOKEN_LEN"   : "69",
"CALLBACKS" : ["test"]
}

def read_config(name :str):
    conf = dict()
    with open(name, "r") as f:
        conf = json.load(f)
    
    for x in variables.keys():
        if x in conf:
            variables[x] = conf[x]


def generate_callbacks_symbol(name : str):
    symbol : str = """void command_{name}_callback(void(*ff)(char* arg, int nargs));\n""".format(name=name)
    return symbol

def generate_calback_variable(name : str):
    variable : str = """void (*__command_{name}_callback)(char* arg, int nargs);\n""".format(name=name)
    return variable

# GENERATING HEADER FILE
def generate_header() -> str:
    variables["CALLBACKS_FUNCTIONS"] = ""
    variables["CALLBACKS_DECLARATIONS"] = ""
    for i in variables["CALLBACKS"]:
        variables["CALLBACKS_FUNCTIONS"] += generate_callbacks_symbol(i)
        variables["CALLBACKS_DECLARATIONS"] += generate_calback_variable(i)
    header : str ="""#ifndef __{LIB_NAME}_HPP__
#define __{LIB_NAME}_HPP__

#ifndef SHELL_BUFFER_SIZE
#define SHELL_BUFFER_SIZE {SHELL_BUFFER_SIZE}
#endif

#ifndef SHELL_MAX_ARGS
#define SHELL_MAX_ARGS {SHELL_MAX_ARGS}
#endif

#ifndef SHELL_MAX_TOKEN_LEN
#define SHELL_MAX_TOKEN_LEN {SHELL_MAX_TOKEN_LEN}
#endif

class Shell{{
    public:
        int execute(char *ptr, int size);
        const char* getBuffer();
        {CALLBACKS_FUNCTIONS}
        
    private:
        int tokenize(char *ptr, int size);
        int string_compare(const char* x, const char* y);
        char tokens[{SHELL_MAX_ARGS}][{SHELL_MAX_TOKEN_LEN}];
        char buffer[{SHELL_BUFFER_SIZE}];
        {CALLBACKS_DECLARATIONS}
}};

#endif""".format(**variables)
    return header

def generate_calback_execution(name : str):
    variable : str = """if(string_compare(tokens[0],"{name}")) {{
        __command_{name}_callback(tokens[0],nargs);
        return 1;
    }}\n""".format(name=name)
    return variable

# GENERATING SOURCE FILE
def generate_source() -> str:
    variables["LIB_NAME_LOWER"] = variables["LIB_NAME"].lower();
    variables["CALLBACKS_EXECUTION"] = ""
    for i in variables["CALLBACKS"]:
        variables["CALLBACKS_EXECUTION"] += generate_calback_execution(i)
    source : str ="""#include "{LIB_NAME_LOWER}.hpp"

const char* Shell::getBuffer(){{
    return buffer;
}}

int Shell::tokenize(char* ptr, int size){{
    if(size == 0) return 0;
    int current = 0;
    for(int x = 0; x<SHELL_MAX_ARGS; x++){{
        int token_current = 0;
        while ((ptr[current] > 32) && (ptr[current] < 127)) {{
            if(token_current >= SHELL_MAX_TOKEN_LEN-1) {{
                tokens[x][token_current] = \'0\';
                continue;
            }}
            tokens[x][token_current] = ptr[current];
            token_current++;
            current++;
        }}
        if(ptr[current] == 32) {{
            tokens[x][token_current] = \'0\';
            continue;
        }}
        else return x;
    }}
    return SHELL_MAX_ARGS-1;
}}

int Shell::execute(char *ptr, int size){{
    int nargs = tokenize(ptr, size);
    if(nargs == 0) return -1;
    {CALLBACKS_EXECUTION}
    return 0;
 
}}""".format(**variables)
    return source

def main():
    if (len(sys.argv) > 1):
        read_config(sys.argv[1])
    header = open(variables["LIB_NAME"].lower() + ".hpp", "w")
    header.write(generate_header())
    header.close()
    source = open(variables["LIB_NAME"].lower() + ".cpp", "w")
    source.write(generate_source())
    source.close()


if __name__ == "__main__":
    main()