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

def generate_help_output(name : str, definition : str):
    help : str = """{name} -> {definition}\\n\\\n""".format(name=name, definition=definition)
    return help

def generate_callbacks_symbol(name : str, args):
    packaging : str = " ,".join(args)
    symbol : str = """// Sets callback for the {name} command
        void setCommand_{name}_Callback(void(*ff)({packaging}));\n\t\t""".format(name=name,packaging=packaging)
    return symbol

def generate_callback_variable(name : str, args):
    packaging : str = " ,".join(args)
    variable : str = """void (*__command_{name}_callback)({packaging});\n\t\t""".format(name=name,packaging=packaging)
    return variable

# GENERATING HEADER FILE
def generate_header() -> str:
    variables["CALLBACKS_FUNCTIONS"] = ""
    variables["CALLBACKS_DECLARATIONS"] = ""
    variables["HELP_OUTPUT"] = ""
    for i,y,z in variables["CALLBACKS"]:
        variables["CALLBACKS_FUNCTIONS"] += generate_callbacks_symbol(i,z)
        variables["CALLBACKS_DECLARATIONS"] += generate_callback_variable(i,z)
        variables["HELP_OUTPUT"] += generate_help_output(i,y)
    with open("header.template", "r") as f:
        header = f.read()
    header = header.format(**variables)
    return header

def generate_callback_execution(name : str, args):
    size: str = len(args)
    parameters = list()
    for x,y in enumerate(args):
        if "int" in args[x]:
            parameters.append("__to_int(_tokens[{x}])".format(x=(x+1)))
        else:
            parameters.append("_tokens[{x}]".format(x=(x+1)))
    parameters = ", ".join(parameters)
    variable : str = """if(__string_compare(_tokens[0],"{name}")==0) {{
        if (nargs < {size}) return -2;
        __command_{name}_callback({parameters});
        return 1;
    }}\n\t""".format(name=name, size=size,parameters=parameters)
    return variable

def generate_callback_setter(name :str, args):
    packaging : str = " ,".join(args)
    setter : str = """// Sets callback for the {name} command
void Shell::setCommand_{name}_Callback(void(*ff)({packaging})){{
    __command_{name}_callback = ff;
}}\n""".format(name=name,packaging=packaging)
    return setter

# GENERATING SOURCE FILE
def generate_source() -> str:
    variables["LIB_NAME_LOWER"] = variables["LIB_NAME"].lower();
    variables["CALLBACKS_EXECUTION"] = ""
    variables["CALLBACKS_SETTERS"] = ""
    for i,_,z in variables["CALLBACKS"]:
        variables["CALLBACKS_EXECUTION"] += generate_callback_execution(i,z)
        variables["CALLBACKS_SETTERS"] += generate_callback_setter(i,z)
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