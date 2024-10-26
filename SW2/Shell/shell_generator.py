import json
import sys
import os

# Script variables (defaults)
variables = {
"LIB_NAME"              :"EXAMPLE",
"SHELL_BUFFER_SIZE"     : "99",
"SHELL_MAX_ARGS"        :"101",
"SHELL_MAX_TOKEN_LEN"   : "69",
"CALLBACKS" : ["print", "showtime"]
}

variables["INCLUDE_PATH"] = os.getcwd();
variables["SOURCE_PATH"] = os.getcwd();

def read_config(name :str):
    conf = dict()
    with open(name, "r") as f:
        conf = json.load(f)
    
    for x in variables.keys():
        if x in conf:
            variables[x] = conf[x]


def generate_callbacks_symbol(name : str):
    symbol : str = """// Sets callback for the {name} command
        void setCommand_{name}_Callback(void(*ff)(char* arg, int nargs));\n\t\t""".format(name=name)
    return symbol

def generate_calback_variable(name : str):
    variable : str = """void (*__command_{name}_callback)(char* arg, int nargs);\n\t\t""".format(name=name)
    return variable

# GENERATING HEADER FILE
def generate_header() -> str:
    variables["CALLBACKS_FUNCTIONS"] = ""
    variables["CALLBACKS_DECLARATIONS"] = ""
    for i,_ in variables["CALLBACKS"]:
        variables["CALLBACKS_FUNCTIONS"] += generate_callbacks_symbol(i)
        variables["CALLBACKS_DECLARATIONS"] += generate_calback_variable(i)
    with open("header.template", "r") as f:
        header = f.read()
    header = header.format(**variables)
    return header

def generate_callback_execution(name : str):
    variable : str = """if(__string_compare(tokens[0],"{name}")) {{
        __command_{name}_callback(tokens[0],nargs);
        return 1;
    }}\n\t""".format(name=name)
    return variable

def generate_callback_setter(name :str):
    setter : str = """// Sets callback for the {name} command
void Shell::setCommand_{name}_Callback(void(*ff)(char* arg, int nargs)){{
    __command_{name}_callback = ff;
}}\n""".format(name=name)
    return setter

# GENERATING SOURCE FILE
def generate_source() -> str:
    variables["LIB_NAME_LOWER"] = variables["LIB_NAME"].lower();
    variables["CALLBACKS_EXECUTION"] = ""
    variables["CALLBACKS_SETTERS"] = ""
    for i,_ in variables["CALLBACKS"]:
        variables["CALLBACKS_EXECUTION"] += generate_callback_execution(i)
        variables["CALLBACKS_SETTERS"] += generate_callback_setter(i)
    with open("source.template", "r") as f:
        source = f.read()
    source = source.format(**variables)
    return source

def main():
    if (len(sys.argv) > 1):
        read_config(sys.argv[1])
    header = open(variables["INCLUDE_PATH"] + "/" + variables["LIB_NAME"].lower() + ".hpp", "w")
    header.write(generate_header())
    header.close()
    source = open(variables["SOURCE_PATH"] + "/" + variables["LIB_NAME"].lower() + ".cpp", "w")
    source.write(generate_source())
    source.close()


if __name__ == "__main__":
    main()