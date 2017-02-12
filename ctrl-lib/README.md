## Ctrl-Lib
This directory contains higher level library. It controls complex device moves, synchronization, and other abstractions. Some of functions are not implemented, some may be refractored.

Abstractions:
* Device controller - uses device API to create simpliest abstraction(rolling, position reset, state tracking, moving).
* Coordinate controller - uses two devices to create coordinate plane abstraction.
* Coordniate task - includes program to execute on coordinate controller.

Components:
* DeviceController - performs each device control functions(moving, rolling, trailer checking).
* CoordController - stores two device controllers and creates coordinate plane abstraction on top of them(including calibration, trailer and error control, move synchronization).
* CoordTask - implement task abstraction for coordinate plane. Each task contains list of separate steps to do(e.g. calibrate, move, jump) and may be performed on top of any coordinate plane.
* CoordTranslator - not implemented yet. Purpose: translate virtual coordinates to real motor steps.
* SystemManager -  central system component, holds DeviceManager, device and coord controllers, task list.
