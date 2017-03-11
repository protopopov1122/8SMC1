## Device API wrappers
This directory contains Device API controller-specific C++ bindings. Other project code use these bindings as proxy to controllers, so only this API is platform-dependent. Device API offers couple of interfaces that device wrappers should implement:
* Device - represent single stepper motor, contains minimal functions that it should implement(moreover few of them are purely informative and may return any random/fake value). 
* DeviceManager - holds list of connected Devices of some class. At one run system may use devices on one class, so it use device manager to access them. Device manager defines couple of initialization, low level error management and informative methods(they also may return fake values).

## NOTE
Device API was designed to make other project parts controller-independent, however the only implentation of this interfaces project contains is Standa 8SMC1 controller API wrappers, so Device API interfaces are influenced by 8SMC1 API. Device API implentation for Standa controllers also contains lot of extra methods that are required by API.
