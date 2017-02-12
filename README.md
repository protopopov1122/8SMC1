8SMC1 API wrappers
===================
This project contains library and command-line interface for 8SMC1 stepper motor controllers.
Library consists of USMC API wrappers for C++ and use MicroSMC controller driver. By now, existing API is low level and directly mapped to USMC API.
Command line inteface is used to test main library, controllers and stepper motors, as well as replace default MicroSMC test program, that is quite poor.
Project also contains 'Stub.cxx' file that contains 'USMCDLL.h' stub implementaion.
For details see README in appropriate subdirectories.

Current goals
===================
* cover all USMC API with C++ wrappers
* write high-level API to use with this wrappers
* make command-line inteface usable with reasonable command aliases, parameters etc.
* implement subset of G-Code and function graphs to execute on top of this API
* integrate this project with useful tools like AutoCAD, Inkscape G-Code plugin
For more detailed info see TODO.

Motivation
===================
This project started with goal to create two-axis moving laser using existing stepper motor and controller for physics lab. Basically we create simple cnc for educational purposes.

Known issues
===================
* Current MicroSMC driver works only on Windows 2000/XP/Vista - controller may be correctly used only on this platforms. Possible solution - reimplement all low level wrappers on top of another controller(like Arduino). Other project parts will depend only on these wrappers so will not be affected, but porting will be quite hard.
* Project is on ultra-early development stage and test are performed two times a week, so new functions may be unstable or not work at all. See NEWS to track current project state.
* There are a lot of synchronization options and other parameters for our controller - we have not figured how to use them yet.

Disclaimer
===================
Code is not guaranted to work, project also may be discontinued. There are standart tools in SMCView package that allow using AutoCAD with 8SMC1 contorller, but they require additional licanses. We are just trying to implement same functionality.

Builing
===================
Project is built using MinGW cross compiler(build tested on top of Linux and Windows Subsystem for Linux).
Create 'res' directory in root of project and copy 'USMCDLL.h' and 'USMCDLL.dll' to it. You must install MiscoSMC driver.
Project has two build systems(simply two different Makefile). One is hand-written(old) and is convinient for development in terminal, other(new) is auto-generated and is useful for incremental build(used in IDEs). This repo contains new Makefile.
You can generate them(NOTE: You should regenerate incremental Makefile if new file is added to project):
```bash
./misc/gen-makefile # For 'new' Makefile
./misc/gen-makefile old # For 'old' one
```
Build commands for both are the same:
```bash
make clean	# Clean all
make all	# Build command-line interface
make stub	# Build stub to test CLI without controller
make gcode	# Build testing G-Code parser
```

Authors and Contributions
===================
Code author: Eugene Protopopov <protopopov1122@yandex.ru>.

Work with controllers is performed in command.

By now code have no license, but feel free to use project, contribute code and ideas.
