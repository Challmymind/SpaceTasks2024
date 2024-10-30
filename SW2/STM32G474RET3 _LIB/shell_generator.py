import json
import sys
import os

# Script variables (default values).
# Can be replaced with json config.
variables = {
"LIB_NAME"              :"EXAMPLE",     # Generated files names (.hpp, .cpp), shell is too generic not to allow this.
"SHELL_MAX_ARGS"        :"101",
"SHELL_MAX_TOKEN_LEN"   : "69",
"CALLBACKS" : ["print", "showtime"]
}

# Default directory for storing generated files, can be overridden in config using INCLUDE_PATH and SOURCE_PATH values.
variables["INCLUDE_PATH"] =     os.getcwd();
variables["SOURCE_PATH"] =      os.getcwd();

# Overrides values with config.
# Does not do syntax check.
def read_config(name :str):
    conf = dict()
    with open(name, "r") as f:
        conf = json.load(f)
    
    for x in variables.keys():
        if x in conf:
            variables[x] = conf[x]

# Compiles a string using the descriptions of defined (via shell) commands.
# It's useful for "help commands" etc.
#
# Possible improvement:
# Instead of generating one string, generate a separate string for each command, making it more universal.
# Commands could use it to have a [command -help] signature.
def generate_help_output(name : str, definition : str):
    help : str = """{name} -> {definition}\\n\\r\\\n""".format(name=name, definition=definition)
    return help

# Creates a callback setter in the header file.
def generate_callbacks_symbol(name : str, args, desc):
    packaging : str = " ,".join(args)
    symbol : str = """/** Sets a callback for the command {name}.
        * Config description: {desc}.
        */
        void setCommand_{name}_Callback(void(*ff)({packaging}));\n\n\t\t""".format(name=name,packaging=packaging,desc=desc)
    return symbol

# Create a variable to store the command callback.
def generate_callback_variable(name : str, args):
    packaging : str = " ,".join(args)
    variable : str = """void (*__command_{name}_callback)({packaging});\n\t\t""".format(name=name,packaging=packaging)
    return variable

# GENERATING HEADER FILE
# Generates full .hpp file.
def generate_header() -> str:
    variables["CALLBACKS_FUNCTIONS"] = ""
    variables["CALLBACKS_DECLARATIONS"] = ""
    variables["HELP_OUTPUT"] = ""
    for i,y,z in variables["CALLBACKS"]:
        variables["CALLBACKS_FUNCTIONS"] += generate_callbacks_symbol(i,z,y)
        variables["CALLBACKS_DECLARATIONS"] += generate_callback_variable(i,z)
        variables["HELP_OUTPUT"] += generate_help_output(i,y)
    with open("header.template", "r") as f:
        header = f.read()
    header = header.format(**variables)
    return header

# Creates a command branch in the shell loop function.
def generate_callback_execution(name : str, args):
    size: str = len(args)
    parameters = list()
    for x,y in enumerate(args):
        # Possible improvement:
        # Currently dynamic conversations based on config do not work, only char* parameters work.
        # It can be fixed by the implementation of dynamic converters, as below, to ANY type that can be 
        # implemented by the end user via callback.
        if "int" in args[x]:
            parameters.append("__to_int(_tokens[{x}])".format(x=(x+1))) # converter (disabled feature)
        else:
            parameters.append("_tokens[{x}]".format(x=(x+1)))
    parameters = ", ".join(parameters)
    variable : str = """if(__string_compare(_tokens[0],"{name}")==0) {{
        if (nargs < {size}) return -2;
        __command_{name}_callback({parameters});
        return 1;
    }}\n\t""".format(name=name, size=size,parameters=parameters)
    return variable

# Creates the body of the callback setter.
def generate_callback_setter(name :str, args):
    packaging : str = " ,".join(args)
    setter : str = """// Sets callback for the {name} command
void Shell::setCommand_{name}_Callback(void(*ff)({packaging})){{
    __command_{name}_callback = ff;
}}\n""".format(name=name,packaging=packaging)
    return setter

# GENERATING SOURCE FILE
# Generates full .cpp file.
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
    # Checks if config exists.
    if (len(sys.argv) > 1):
        read_config(sys.argv[1])

    # Create .hpp file. May overwrite existing or fail if directory does not exist.
    # It's better to fail in this case to make sure the user notices.
    header = open(variables["INCLUDE_PATH"] + "/" + variables["LIB_NAME"].lower() + ".hpp", "w")
    header.write(generate_header())
    header.close()

    # Create .cpp file. May overwrite existing or fail if directory does not exist.
    source = open(variables["SOURCE_PATH"] + "/" + variables["LIB_NAME"].lower() + ".cpp", "w")
    source.write(generate_source())
    source.close()


if __name__ == "__main__":
    main()