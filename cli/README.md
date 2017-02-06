## Command Line Interface(CLI)
This directory contains implementation of CLI. It is used to control system core, test it and link all API functions together. CLI implementation consists of:
* CLI class - hold list of available commands, read line from console, parse and try to execute. 
* DevCLI.cpp - contains definitons of commands that operate with Device wrapper API
* main.cpp - using Device wrapper API initialize DeviceManager, create CLI and add all defined in DevCLI.cpp commands.


## CLI syntax
By now, CLI command aliases and parameters are directly binded on Device wrapper API, but it supposed to be more high-level, so all current commands are temporary and used for testing.
