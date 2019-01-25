# CalX Stepper motor and mini-CNC toolset

CalX is a high-level application and shared library aimed to control and synchronize stepper motors and cutting instruments. The most of project codebase is platform- and device-independent, and can be adopted to different types of motor controllers and many kinds of instruments, the only requirement is the implementation of wrapper interfaces (so called Device API). Project already contains Device API implementation for Standa 8SMC1 stepper motor controllers and EKSPLA NL300 laser, which were used during development. Currently system is fully-functional and mostly tested on these devices, however any help is appreciated.

**Project provides detailed [documentation](docs/README.md), however recent aspects of the system (journaling, scripting, math formula engine) are not covered**


### Motivation
This project started with a goal to create two-axis moving laser using existing stepper motor and controller in the university physics lab (mostly with educational purposes). Currently it is quite universal and has more features than required at the start. After first version release, there also may be added other device types (e.g. measurement instruments) and functions.

### Disclaimer
Project started as an educational and may be discontinued. Currently it contains fully functional and mostly tested mini-CNC implementation with a library, command line and graphical user interface, as well as driver implementation for Standa 8SMC1 stepper motors and EKSPLA NL300 laser. Further work on the project is not guaranteed. Summing up:
```
CalX is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```


### Authors and Contributions
Author: Jevgēnijs Protopopovs <jprotopopov1122@gmail.com>.

Testing, consultatons and support: Mihails Andžans

All contributions are welcome.

Project code is distributed under the terms of GNU LGPLv3. See COPYING and COPYING.LESSER
