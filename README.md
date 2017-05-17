CalX Stepper motor tools.
===================

CalX is project of creating high level control application and library for stepper motors and instruments for them. Project codebase is device and platform independent, so may be adopted to any kind of motor controllers, only requirement is implementation of some interfaces for them(called Device API). During development and testing are used Standa 8SMC1 stepper motor controllers and EKSPLA NL300 lasers, so so CalX already contains Device API implementation for them. By 17/05/2017 several integral tests were performed and system is working fully, however some enhancements are planned.

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
Project uses CMake as a build system, it also includes CPack build commands so installer can be generated easialy(althrough you need NSIS installed). Main system dependencies are Visual C++ library and wxWidgets library, so you should download, unpack and compile(unicode shared monolithic library) it in some top-level directory(project uses standart FindwxWidgets module, see it documentation). Project builds on both Windows and Linux, however Linux installator is not configured yet. General instructions:
```bash
mkdir build
cd build	# Build out-of-source
cmake ..	# Generate build files
msbuild CalX.sln /p:Configuration=Release /m	# On Windows
make	# On Linux
cpack
```
Project also contains MinGW build files, that should be used for Windows XP:
```bash
./misc/old-build/old-build.sh build "-jN" [WX] [WX_LIB] # Where N - build process count, WX - path to wxWidgets, WX_LIB - path to wxWidgets unicode release monolithic dynamic library relatively to WX.
```
It produces full system build in build/old-build directory, however system installer not available. Resulting executables are linked statically to runtime.

Authors and Contributions
===================
Author: Jevgēnijs Protopopovs <protopopov1122@yandex.ru>.

Testing is performed in command.

Project code is distributed under the terms of GNU LGPLv3. See COPYING and COPYING.LESSER
