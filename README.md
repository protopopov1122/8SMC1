CalX Stepper motor library.
===================

CalX is project of creating high level control application and library for stepper motors. Project codebase is device and platform independent, so may be adopted to any kind of motor controllers, only requirement is implementation of some interfaces for them(called Device API). During development and testing is used Standa 8SMC1 stepper motor controllers, so so CalX already contains Device API implementation for them.

Project consists of several parts:
* Device API - the only code which directly interacts with motor controller/drivers and is platform dependent. Its components are stored in device subdirectory. Device API defines abstract intefaces so that other code uses only them and is device-independent. To add new device class support to system, you must implement these interfaces(they are tiny) and patch some files(two lines per cli/main.cpp and ui/CalxApp.cpp) to use them. Reference Device API implementation is wrappers for Standa 8SMC1 controllers(located in device/8smc1 directory), but they have a lot of extra code that isn't necessary.
* Ctrl-lib - main and central component of project. Is offers different actions and abstractions that are used for complex stepper motor control. It uses only Device API created abstractions, so Ctrl-lib is platform-independent and may be used with any compatible controller/motor. Basically it is project functional core, on top of it works GUI and command line interface.
* CLI(command line interface) - interface that was used during Ctrl-lib development and testing. It doesn't cover all Ctrl-lib possibilities(however most of them) and has complex commands and parameters, however it can be useful.
* UI(graphical user interface) - wxWidgets based graphical application that uses Ctrl-lib to provide convinient device control and cover its functions. Ctrl-lib has big variety of features, so UI itself may be not very user-friendly. Ctrl-lib is extended to needs of UI, because UI is development target.

Track NEWS.md to view recent changes.
For more info see READMEs in subdirectories(read this README before).


Motivation
===================
This project started with goal to create two-axis moving laser using existing stepper motor and controller for physics lab for educational purposes. Currently it is more universal and has more features.

Known issues
===================
Project started as simple Standa 8SMC1 motor control interface, however during development it extended with features that wasn't necessary at start, so there is some consequences:
* Not all text files that describe project is up-to-date. Development is quite fast and focused on new function implementation, so not all textual descriptions are actual.
* Currently CalX has possibility to use any stepper motors, however the only hardware controller we have is Standa 8SMC1 - all functionality is tested with them.
* Currently project should be able to build/run on different OS, however main platform is Windows XP.

Disclaimer
===================
Project is on development stage so it may be unstable.

The only Device API implementation is written for Standa controllers, other device support may be added later. 8SMC1 devices have own standart packages for similar purposes, but they require additional licenses and are discontinued by manufacter.

Builing
===================
Project is built using MSVC and MinGW cross compiler(MinGW build tested on top of Linux and Windows Subsystem for Linux).

To use 8SMC1 controller:
You must install MiscoSMC driver. Create 'res' directory in root of project and copy 'USMCDLL.h', 'USMCDLL.lib' and 'USMCDLL.dll' to it(it required to proper build, these files located in MicroSMC folder in Program files). You should also copy 'USMCDLL.lib' to root of project, if you are building by MSVC without use of stub.
If you build GUI using MinGW, wxWidgets(version 3.1.0) must be compiled with additional link flags "-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic" as monolithic unicode shared library.

Currently MSVC build for GUI is broken, but it will be fixed few days later.
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
make calx.exe # Build CLI
make calx.dll # Build library; required for GUI
make calxui.exe WX=$(WX) # Build GUI(currently under development); $(WX) - wxWidgets-3.1.0 directory; requires calx.dll build first.
```

Authors and Contributions
===================
Author: Jevgenijs Protopopovs <protopopov1122@yandex.ru>.

Testing is performed in command.

Project code is distributed under the terms of GNU LGPLv3. See COPYING and COPYING.LESSER
