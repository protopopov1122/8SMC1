## Device API wrappers
This directory contains Device API controller-specific C++ bindings. Other project code use these bindings as proxy to controllers, so only this API is platform-dependent.
Currently, there is several wrappers:
* Standa 8SMC1 stepper motor and EKSPLA NL300 drivers - main production and testing drivers.
* Emulated drivers - Device API stub implementation that is used to port and test CalX on different systems.

## NOTE
Device API was designed to make other project parts controller-independent, however the only implentation of this interfaces project contains is Standa 8SMC1 stepper motor controller API wrappers and EKSPLA NL300 laser driver, so Device API interfaces are influenced by them. Device API implentation for Standa controllers also contains lot of extra methods that are required by API.
