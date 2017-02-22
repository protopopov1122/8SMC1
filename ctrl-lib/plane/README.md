## Ctrl-Lib plane

This directory contains different implementations of Coordinate Plane abstraction. Most of them work as filters - take actions preprocess it(e.g. log action, map/validate coordinates) and pass it to next. All abstractions are stacked. Top abstraction is called by code, but bottom executes instruction on hardware(or it may be hardware emulator).