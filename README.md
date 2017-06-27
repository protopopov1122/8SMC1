# CalX Stepper motor and mini-CNC toolset

CalX is project of creating high level control application and library for stepper motors and cutting instruments for them. Project codebase is device and platform independent, so may be adopted to any kind of motor controllers, only requirement is implementation of some interfaces for them(called Device API). During development and testing are used Standa 8SMC1 stepper motor controllers and EKSPLA NL300 lasers, so so CalX already contains Device API implementation for them. By 25/05/2017 system is fully functional and ready for production.

**CalX system [documentation](docs/README.md) for detailed information**


### Motivation
This project started with goal to create two-axis moving laser using existing stepper motor and controller for physics lab for educational purposes. Currently it is more universal and has more features. After first version release, there also may be added other device types and functions.

### Disclaimer
Project started as an educational and may be discontinued. Currently it contains fully functional and tested universal mini-CNC implementation with library, command line and graphical user interface, as well as driver implementation for Standa 8SMC1 stepper motors and EKSPLA NL300 laser. Further work on project not guaranteed. Summing up:
```
CalX is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```


### Authors and Contributions
Author: Jevgēnijs Protopopovs <jprotopopov1122@gmail.com>.

Testing is performed in command, all contributions are welcome.

Project code is distributed under the terms of GNU LGPLv3. See COPYING and COPYING.LESSER
