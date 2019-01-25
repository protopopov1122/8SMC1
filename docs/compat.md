## CalX system compatibility
In this manual section system compatibility to modern standarts and portability across different operating systems are discussed. CalX is positioned as platform and hardware-independent mini-CNC implementation so it will be compared to the industry standarts.
#### G-Code
As a standart task representation CalX uses GCode which is well-known CNC programming language. CalX GCode support is strictly limited to the minimal command set because of high abstraction level above hardware, however popular GCode generation tools are able to generate only these supported commands. Supported command list with implementation description.
```
G00 - asynchronous movement
G01 - synchronous movement
G02 - clockwise arc movement
G03 - counter-clockwise arc movement
```
All other commands are ignored because they can't be completely implemented on minimal abstraction that offers CalX Device API. Supported GCode generation tested on Inkscape GCode plugin, dxf2code and hand-written code listings.
#### Portability
CalX system consists of several components and each portability has to be discussed separately. Generally, CalX is almost completely written on C++11 however some C++14 and C++17 features are used. Compilation is tested on MSVC 17 and MinGW compilers, Coverity free tests also were passed. According to them there may be two existing portability problems: integer type casts with possible overflow and exception ignoring. Despite this fact, currently CalX works and tested on Windows XP/7/10 and modern Linux versions. Platform specific code is strictly isolated so portability is quite high.
* CalX Ctrl-lib - system functional core. It has no third-party dependencies and depends only on C++ standart library and STL. All platform and hardware dependent code was moved out of Ctrl-lib and interfaces are used to interact with it.
* CalX CLI - uses only C++ standart library and STL, so generally requires only modern C++ compiler on the platform.
* CalX GUI - used GUI framework is wxWidgets and all system specific functions are called using wxWidgets library. Portability is limited only by this library.
* CalX Device API implementations - Device API is meant to be platform oriented and use OS specific functions and drivers. Device API itself is an interface and common code base for Ctrl-lib - device interaction and theoretically may be as portable as Ctrl-lib.
#### Built-in device support
By now CalX system has drivers for Standa 8SMC1 stepper motors and EKSPLA NL300 laser so this device set will work out-of-box. All other device combinations will require own Device API drivers to work.
