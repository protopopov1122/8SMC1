CalX Stepper motor tools.
===================

CalX is project of creating high level control application and library for stepper motors and instruments for them. Project codebase is device and platform independent, so may be adopted to any kind of motor controllers, only requirement is implementation of some interfaces for them(called Device API). During development and testing are used Standa 8SMC1 stepper motor controllers and EKSPLA NL300 lasers, so so CalX already contains Device API implementation for them. By 13/04/2017 program 1.0 version is finished and final tests will take place on 20/04/2017, then release will be published on GitHub. However, system is already usable and completed.

Project consists of several parts:
* Device API - the only code which directly interacts with motor controller/drivers and instruments, it is platform dependent. Its components are stored in device subdirectory. Device API defines abstract intefaces so that other code uses only them and is device-independent. To add new device class support to system, you must implement these interfaces(they are tiny) and patch some files(two lines per cli/main.cpp and ui/CalxApp.cpp) to use them. Reference Device API implementation is wrappers for Standa 8SMC1 controllers(located in device/8smc1 directory), but they have a lot of extra code that isn't necessary.
* Ctrl-lib - main and central component of project. Is offers different actions and abstractions that are used for complex stepper motor control. It uses only Device API created abstractions, so Ctrl-lib is platform-independent and may be used with any compatible controller/motor. Basically it is project functional core, on top of it works GUI and command line interface.
* CLI(command line interface) - interface that was used during Ctrl-lib development and testing. It doesn't cover all Ctrl-lib possibilities(however most of them) and has complex commands and parameters, however it can be useful.
* UI(graphical user interface) - wxWidgets based graphical application that uses Ctrl-lib to provide convinient device control and cover its functions. Ctrl-lib has big variety of features, so UI itself may be not very user-friendly. Ctrl-lib is extended to needs of UI.

Track NEWS.md to view recent changes.
For more info see READMEs in subdirectories(read this README before).


Motivation
===================
This project started with goal to create two-axis moving laser using existing stepper motor and controller for physics lab for educational purposes. Currently it is more universal and has more features. After first version release, there may added also other device types and functions.

Known issues
===================
Project started as simple Standa 8SMC1 motor control interface, however during development it extended with features that wasn't necessary at start, so there is some consequences:
* Not all text files that describe project is up-to-date. Development is quite fast and focused on new function implementation, so not all textual descriptions are actual.
* Currently CalX has possibility to use any stepper motors, however the only hardware controller we have is Standa 8SMC1 - all functionality is tested with them.
* Project is portable and has versions for Windows and Linux. Main development version is Windows version, Linux version currently is unstable.

Disclaimer
===================
Project is not fully tested, it may contain some bugs.

The only Device API implementation is written for Standa controllers and EKSPLA laser, other device support may be added later. 8SMC1 devices have own standart packages for similar purposes, but they require additional licenses and are discontinued by manufacter.

Building
===================
Project contains two build systems:
* CMake build files. Recently integrated into project, however CMake files are quite simple. Recomended to use. Currently it is main priority, successfully builds Device API drivers, Ctrl-lib and Calx UI. Build steps:
For Windows: generate MSVC soultion and build it by Visual Studio.

For Linux:
```bash
mkdir -p build
cd build	# Create and enter build directory
cmake ..	# Generate build files
make		# Build project
make install	# Install software. Not tested
```
* Custom shell-script based build system. Quite complex, will be removed after full CMake integration.

Project is built using MSVC and MinGW cross compiler(MinGW build tested on top of Linux and Windows Subsystem for Linux). Project UI depends on wxWidgets and links to it dynamically. Before using you should make sure that 'autoconf' key in config.ini is commented/disable, because it contains configuration parameters specific to used equipment and will not work on other systems.

To use 8SMC1 controller:
You must install MiscoSMC driver. Create 'res' directory in root of project and copy 'USMCDLL.h', 'USMCDLL.lib' and 'USMCDLL.dll' to it(it required to proper build, these files located in MicroSMC folder in Program files). You should also copy 'USMCDLL.lib' to root of project, if you are building by MSVC without use of stub.
If you build GUI using MinGW, wxWidgets(version 3.1.0) must be compiled with additional link flags "-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic" as monolithic unicode shared library.

For MinGW:
Project has custom build system based on Makefile. Makefile script supports incemental build and is generated by other scripts. Current Makefile supports parallel build. Project contains Makefile that is able to compile all components, however if you add new source files, you should regenerate it:
```bash
./misc/gen-makefile > Makefile
```
Build commands:
```bash
make all WX=$(WX) [WXLIB=...] # Build all project parts. Recomended to use. $(WX) - wxWidgets directory. WXLIB - target library to copy(something like 'lib/wxmsw310u_gcc_custom.dll'), not necessary during build(however useful to run from build directory).
make clean	# Clean all
make stub	# Build stub to test CLI without controller
make calx.exe # Build CLI
make calx.dll # Build library; required for GUI
make calxui.exe WX=$(WX) [WXLIB=...] # Build GUI; $(WX) - wxWidgets directory; requires calx.dll build first. WXLIB - target library to copy(something like 'lib/wxmsw310u_gcc_custom.dll'), not necessary during build(however useful to run from build directory).
```
Build all command also performs some other steps, prepares build directory, so after that build directory contains fully functional and complete system. Program requires 'config.ini' in working directory, it contains some system settings. If ImageMagick is installed Makefile builds .ico(from misc/icon.svg) file and result executable will have icon.

To build on Linux:
Project supports build on Linux and is fully working, however functionality was not tested and this version is unstable. 
```bash
make clean	# Clean all
make libcalx.so	# Build functional core
make devapi_emulated # Build Device API implementation
make calxui	# Build GUI; WX=$(WX) [WXLIB=...] # Build GUI; $(WX) - wxWidgets directory; requires calx.dll build first. WXLIB - target library to copy(something like 'lib/wxmsw310u_gcc_custom.dll'), not necessary during build(however useful to run from build directory).

```

For MSVC:
Build by MSVC is possible, however it's not main compiler for development.
See winbuild/README.md. You may execute one of winbuild/build.bat [WX] files from project root to build full project(WX - path to WxWidgets 3.1.0 directory).
You can regenerate build files by command:
```bash
misc/gen-winbuild
```

Authors and Contributions
===================
Author: Jevgēnijs Protopopovs <protopopov1122@yandex.ru>.

Testing is performed in command.

Project code is distributed under the terms of GNU LGPLv3. See COPYING and COPYING.LESSER
