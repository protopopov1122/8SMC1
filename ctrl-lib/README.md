## Ctrl-Lib
This directory contains higher level library. It controls complex device moves, synchronization, and other abstractions. Some of functions are not implemented, some may be refractored. Library files located in several subdirectories. See their README.

Abstractions:
* Device controller - uses device API to create simpliest abstraction(rolling, position reset, state tracking, moving).
* Coordinate controller - uses two devices to create coordinate plane abstraction.
* Coordinate plane - abstractions to perform some actions on coordinate plane(log actions, validation, coordinate mapping and actual movement). These abstractions are stored in stack(the bottom stack element is coordinate controller).
* Coordniate task - includes program to execute on coordinate plane.

Components(list is not full):
* DeviceController.
* CoordController.
* CoordTask.
* CoordTranslator - translates virtual g-code coordinates to motor steps.
* SystemManager -  central system component, holds DeviceManager, device controllers and coord planes, task list.
* CoordPlane - different abstractions to use as coordinate plane.
* GCodeParser - parse G-Code file and translate it to coordinate task.
* CircleGenerator - Bresenhams algorithm implementation for arc movement.