## General CalX description
This section explains some general information about the project.

#### Motivation and Goals
Project started on February 2017 in university physics lab. Main goal was connection existing stepper motors and laser. Physics laboratory owns set of Standa 8SMC1 stepper motors and controllers as well as EKSPLA NL300 laser. This hardware and specific requirements for it strongly affected system properties, however at later programming stages project code mostly was refactored to be portable and universal.

#### Project structure
Project is split into several components that provide different level of interface. They will be discussed later, however there are list of available usages:
* CalX UI - graphical user interface. It is main goal of development, because end user interacts only with it, so it covers most of system functions. Interface tends to save system core flexibility so it may seem a bit complicated. [UI manual](gui.md)
* CalX CLI - command-line interface. It was used during early development stages. Currently it doesn't cover all system functions, however in perspective it is more powerful comparing to GUI. [CLI manual](cli.md)
* CalX Ctrl-lib - system functional core. It is dynamic library that may be used to control system, both GUI and CLI use it. Main goal of Ctrl-lib is offering set of different level abstractions that could be easily combined in different ways. [CalX system concepts](concepts.md)
* Device drivers - device drivers are completely separated from Ctrl-lib, so library itself could be portable. [Device API manual](devapi.md)

#### Portability
CalX UI, CLI and Ctrl-lib are created with portability in mind. By now, they can be compiled and used on both Windows and Linux. However system requires valid device drivers to work properly which are obviously not portable across different platforms. Project includes device driver stubs(called 'emulated') to test system and port it on other platforms. See [compatibility](compat.md) and [installation](install.md) manuals.

#### Disclaimer
Project started as an educational and may be discontinued. Currently it contains fully functional and tested universal mini-CNC implementation with library, command line and graphical user interface, as well as driver implementation for Standa 8SMC1 stepper motors and EKSPLA NL300 laser. Further work on project not guaranteed. Summing up:
```
CalX is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

#### Authors and Contributions
Author: Jevgēnijs Protopopovs <protopopov1122@yandex.ru>.

Testing is performed in command, all contributions are welcome.

Project code is distributed under the terms of GNU LGPLv3. See COPYING and COPYING.LESSER
