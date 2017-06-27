## General CalX description
This section explains some general information about the project.

#### Motivation and Goals
Project started on February 2017 in university physics lab. Main goal was connecting existing stepper motors and laser. Physics laboratory owns a set of Standa 8SMC1 stepper motors and controllers as well as an EKSPLA NL300 laser. This hardware and specific requirements for it strongly affected system properties, however on later programming stages project code mostly was refactored to be portable and universal.

#### Project structure
Project is split into several components that provide different interfaces. They will be discussed later, however there is a list of available usages:
* CalX UI - graphical user interface. It is a main goal of development, because end user interacts only with it, so it covers most of system functions. Interface tends to save system core flexibility so it may seem a bit complicated. [UI manual](gui.md)
* CalX CLI - command-line interface. It was used during early development stages. Currently it doesn't cover all system functions, however in the perspective it is more powerful comparing to GUI. [CLI manual](cli.md)
* CalX Ctrl-lib - system functional core. It is dynamic library that may be used to control system, both GUI and CLI use it. Main goal of Ctrl-lib is offering set of different level abstractions that could be easily combined in a different ways. [CalX system concepts](concepts.md)
* Device drivers - device drivers are completely separated from Ctrl-lib, and library itself could be portable. [Device API manual](devapi.md)

#### Portability
CalX UI, CLI and Ctrl-lib are created with portability in mind. By now, they can be compiled and used on both Windows and Linux. However system requires a valid device driver to work properly which obviously is not portable across different platforms. Project includes device driver stubs(called 'emulated') to test system and port it on other platforms and drivers for Standa 8SMC1 and EKSPLA NL300 devices. See [compatibility](compat.md) and [installation](install.md) manuals.

#### Disclaimer
Project started as an educational and may be discontinued. Currently it contains fully functional and tested universal mini-CNC implementation with library, command line and graphical user interface, as well as driver implementation for Standa 8SMC1 stepper motors and EKSPLA NL300 laser. Further work on project not guaranteed. Summing up:
```
CalX is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

#### Authors and Contributions
Author: Jevgēnijs Protopopovs <jprotopopov1122@gmail.com>.

Testing, consultations and support: Mihails Andžans

All contributions are welcome.

Project code is distributed under the terms of GNU LGPLv3. See COPYING and COPYING.LESSER
