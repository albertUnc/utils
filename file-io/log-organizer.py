"""
log-organizer.py
A python script that organizes logs by message type
eg. Info, Error, Warn, Fatal

Can read settings from a json file
"""

import json
import os
import sys
from conversions import stob, is_bool

settings = {
    "info": "[INFO]",
    "warn": "[WARN]",
    "error": "[ERROR]",
    "fatal": "[FATAL]",

    "anywhere_in_line": True,
    "delete_file": False,

    "input_file": "log.txt",

    "info_output": "info.log",
    "error_output": "error.log",
    "warn_output": "warn.log",
    "fatal_output": "fatal.log",
}

valid_args = [
    "--info",
    "--warn",
    "--error",
    "--fatal",
    "--anywhere_in_line",
    "--delete_file",
    "--input_file",    
    "--info_output",   
    "--error_output",  
    "--warn_output", 
    "--fatal_output",
]

def load_settings(path: str) -> None:
    global settings
    if not os.path.exists(path):
        sys.exit(f"Invalid path for settings file: {path}")
    with open(path, "r") as file:
        try:
            data = json.load(file)
        except json.JSONDecodeError as e:
            sys.exit(f"Error parsing JSON settings file: {e}")
            
        for elem in data:
            if elem not in settings:
                sys.exit(f"Invalid name in settings file: {elem}")
        settings.update(data)

def organize() -> None:
    if not os.path.exists(settings["input_file"]):
        sys.exit(f"Input file non-existent: {settings['input_file']}")

    open_mode = "w" if settings["delete_file"] else "a"

    try:
        with open(settings["input_file"], "r") as src, \
             open(settings["info_output"], open_mode) as info_out, \
             open(settings["error_output"], open_mode) as error_out, \
             open(settings["warn_output"], open_mode) as warn_out, \
             open(settings["fatal_output"], open_mode) as fatal_out:

            for line in src:
                if not settings["anywhere_in_line"]:
                    if   line.startswith(settings["info"]):  info_out.write(line)
                    elif line.startswith(settings["warn"]):  warn_out.write(line)
                    elif line.startswith(settings["error"]): error_out.write(line)
                    elif line.startswith(settings["fatal"]): fatal_out.write(line)
                    else: print(f"Line has no valid token at start: {line.strip()}")
                else:
                    if   settings["info"]  in line: info_out.write(line)
                    elif settings["warn"]  in line: warn_out.write(line)
                    elif settings["error"] in line: error_out.write(line)
                    elif settings["fatal"] in line: fatal_out.write(line)
                    else: print(f"Line has no valid token: {line.strip()}")

    except OSError as e:
        sys.exit(f"File I/O error: {e}")

def effect_args() -> None:
    args = sys.argv[1:] 
    
    if len(args) == 0:
        return

    if args[0] == "--config":
        if len(args) < 2:
            sys.exit("Missing path for --config argument")
        load_settings(args[1])
        return

    skip = False
    for i, arg in enumerate(args):
        if skip:
            skip = False
            continue
        
        if arg not in valid_args:
            sys.exit(f"Invalid argument: {arg}")
            
        if i + 1 >= len(args):
            sys.exit(f"Missing value for argument: {arg}")

        val = args[i+1]

        if arg == "--info":
            settings["info"] = val
            skip = True
        elif arg == "--warn":
            settings["warn"] = val
            skip = True
        elif arg == "--error":
            settings["error"] = val
            skip = True
        elif arg == "--fatal":
            settings["fatal"] = val
            skip = True
        elif arg == "--anywhere_in_line":
            if not is_bool(val): sys.exit(f"Value for {arg} must be True or False.")
            settings["anywhere_in_line"] = stob(val)
            skip = True
        elif arg == "--delete_file":
            if not is_bool(val): sys.exit(f"Value for {arg} must be True or False.")
            settings["delete_file"] = stob(val)
            skip = True
        elif arg == "--input_file":
            settings["input_file"] = val
            skip = True
        elif arg == "--info_output":
            settings["info_output"] = val
            skip = True
        elif arg == "--error_output":
            settings["error_output"] = val
            skip = True
        elif arg == "--warn_output":
            settings["warn_output"] = val
            skip = True
        elif arg == "--fatal_output":
            settings["fatal_output"] = val
            skip = True

if __name__ == "__main__":
    effect_args()
    organize()
    print("Success!")
    sys.exit(0)