## Ctrl-Lib
This directory contains higher level library. It controls complex device moves, synchronization, and other abstractions. Some of functions are not implemented, some may be refractored. Library files located in several subdirectories. See their README.

Control library(Ctrl-lib) depends on Device API implementation, so it is platform-independent and can be used with any compatible device classes.

Abstractions:
* Device controller - uses device API to create simpliest abstractions(rolling, position reset, state tracking, moving).
* Coordinate plane - abstractions to perform some actions on coordinate plane(log actions, validation, coordinate mapping and actual movement). These abstractions may be stored in stack(the bottom stack element is coordinate controller).
* Coordinate controller - uses two devices to create coordinate plane abstraction(used as base for any other coordinate plane implementations).
* Coordniate task - includes program to execute on coordinate plane(this is interface with different concrete versions).
* Coordinate translator - used to translate coordinate plane(supported coordinate planes: Decart(linear), logarithmic and polar) coordinates(floating point) to real motor positions(integer).
* Useful tools - classes that perform some routine work(circle generator - Bresenhams circle algorithm implementation, g-code parser - load g-code from stream and transfer to coordinate task, graph builder - facade to graph build API - calculates function graph points, translates them to motor positions, controls motors).
* System manager - bring all system functions together in one interface.