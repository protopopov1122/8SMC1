## Device API wrappers
This directory contains Device API controller-specific C++ bindings. Other project code use these bindings as proxy to controllers, so only this API is platform-dependent. Device API offers couple of interfaces that device wrappers should implement:
* Motor - represents single stepper motor, contains minimal functions that it should implement(moreover few of them are purely informative and may return any random/fake value). 
* Instrument - represents single instrument. In this context instrument means something able to cut, plot etc.
* DeviceManager - holds list of connected Devices of some class. At one run system may use devices on one class, so it use device manager to access them. Device manager defines couple of initialization, low level error management and informative methods(they also may return fake values).
Instruments and Motors may be connected dynamically, but currently the only implemented dynamic connection mechanism is serial port connection(however other connection possiblities may added easily).

## Used abstractions
System operates with devices. Each device is either instrument or motor. Short description of used abstractions, for more details see Device.h.

Instrument must implement:
* enable/disable functions.
* 3 modes - off/prepare/max. Some of them may be emulated, system doesn't control them directly.
* info getting method.
* all other properties may be defined as configurable values. See ctrl-lib/ConfigManager, each Instrument contains own config.

Motor must implement:
* start/stop functions.
* state and position getting functions.
* informative functions.

## NOTE
Device API was designed to make other project parts controller-independent, however the only implentation of this interfaces project contains is Standa 8SMC1 stepper motor controller API wrappers and EKSPLA NL300 laser driver, so Device API interfaces are influenced by them. Device API implentation for Standa controllers also contains lot of extra methods that are required by API.
