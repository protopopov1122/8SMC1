8SMC1 API wrappers
===================

This project aims to create library to perform high-level control of 8SMC1 controllers and UI to use it. Currently project contains library core(still in development but already has most useful functions, however not all are tested) and command-line interface to test it(SEE cli/README.md to view command-line manual).

Project has few levels: low-level Device wrapper API(wraps actual 8SMC1 driver API), high-level library(used to more complex stuff: different movement, logging, error checking, g-code and tasks), command-line interface(for testing purposes), device driver stubs(to test without hardware).

Currently project is in development stage, so API may be unstable and contain bugs. Althrough project functional core is almost finished UI is not written so not all functions are tested. Function testing on real hardware takes place two times a week(Monday and Thursday) so functionality that is added between this days may not work(track NEWS.md to view current state). All bugfixes added during tests firstly will be uploaded to testing branch and then refractored, but development code pushed in master(during the week testing and master will not be synchronized).

For more info see READMEs in subdirectories.

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
* Project is on development stage and tests are performed two times a week, so new functions may be unstable or not work at all. See NEWS to track current project state.
* There are a lot of synchronization options and other parameters for our controller - we have not figured how to use them yet. Probably not all controller options will be used.

Disclaimer
===================
Project is on development stage so it may be unstable. There are standart tools in SMCView package that allow using AutoCAD with 8SMC1 contorller, but they require additional licanses, our controllers are discontinued as well. We are just trying to implement same functionality with some additions.

Builing
===================
Project is built using MSVC and MinGW cross compiler(MinGW build tested on top of Linux and Windows Subsystem for Linux).
You must install MiscoSMC driver. Create 'res' directory in root of project and copy 'USMCDLL.h', 'USMCDLL.lib' and 'USMCDLL.dll' to it(it required to proper build, these files located in MicroSMC folder in Program files). You should also copy 'USMCDLL.lib' to root of project, if you are building by MSVC without use of stub.
If you build GUI using MinGW, wxWidgets(version 3.1.0) must be compiled with additional link flags "-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic" as monolithic unicode shared library.

For MSVC:
See winbuild/README.md. You may execute one of winbuild/*.bat files from project root to build one of targets.
You can regenerate build files by command:
```bash
misc/gen-winbuild
```

For MinGW:
Project has two build systems(simply two different Makefile). One is hand-written(old) and is convinient for development in terminal, other(new) is auto-generated and is useful for incremental build(used in IDEs). This repo contains new Makefile.
You can generate them(NOTE: You should regenerate incremental Makefile if new file is added to project):
```bash
./misc/gen-makefile > Makefile # For 'new' Makefile
./misc/gen-makefile old > Makefile # For 'old' one
```
Build commands for both are the same:
```bash
make clean	# Clean all
make stub	# Build stub to test CLI without controller
make 8smc1.exe # Build CLI
make 8smc1.dll # Build library; required for GUI
make ui.exe WX=$(WX) # Build GUI(currently not implemented); $(WX) - wxWidgets-3.1.0 directory; requires 8smc1.dll build first.
```

Authors and Contributions
===================
Code author: Eugene Protopopov <protopopov1122@yandex.ru>.

Work with controllers is performed in command.

By now code have no license(most likely project will have some kind of GNU GPL in the end), but feel free to use project, contribute code and ideas.
