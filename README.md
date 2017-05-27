CalX Stepper motor tools.
===================

CalX is project of creating high level control application and library for stepper motors and instruments for them. Project codebase is device and platform independent, so may be adopted to any kind of motor controllers, only requirement is implementation of some interfaces for them(called Device API). During development and testing are used Standa 8SMC1 stepper motor controllers and EKSPLA NL300 lasers, so so CalX already contains Device API implementation for them. By 25/05/2017 system is fully functional and ready to production.

To see info about building and user manual view [documentation](https://github.com/protopopov1122/CalX/tree/master/docs)


Motivation
===================
This project started with goal to create two-axis moving laser using existing stepper motor and controller for physics lab for educational purposes. Currently it is more universal and has more features. After first version release, there also may be added other device types and functions.

Known issues
===================
Project started as simple Standa 8SMC1 motor control interface, however during development it extended with features that wasn't necessary at start, so there is some consequences:
* Not all text files that describe project is up-to-date. Development is quite fast and focused on new function implementation, so not all textual descriptions are actual.
* Currently CalX has possibility to use any stepper motors, however the only hardware controller we have is Standa 8SMC1 - all functionality is tested with them.
* Project is portable and has versions for Windows and Linux. Main development version is Windows version, Linux version currently may be unstable.

Disclaimer
===================
Project is not fully tested, it may contain some bugs.

The only Device API implementation is written for Standa controllers and EKSPLA laser, other device support may be added later. 8SMC1 devices have own standart packages for similar purposes, but they require additional licenses and are discontinued by manufacter.


Authors and Contributions
===================
Author: Jevgēnijs Protopopovs <protopopov1122@yandex.ru>.

Testing is performed in command.

Project code is distributed under the terms of GNU LGPLv3. See COPYING and COPYING.LESSER
