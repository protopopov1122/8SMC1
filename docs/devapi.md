Device API Manual
===========================
### Abstract
Device API is an abstraction that is used to represent any type of motors and instruments and give clear interface to other CalX system components. These interfaces are projected to be as abstract as possible, however they are influenced by Standa 8SMC1 stepper motor drivers. This is short manual of creating third-party Device API implementation; the main goal of the manual is decribing API methods and requirements for them. It meant to be read along with Device API source(especially headers).

### Device API concepts
Device API implementation defines several concepts:
* Device - most abstract class. Device in most aspects is common to all API implementations, so only few methods should be reimplemented. Device API common part:
	* Device type - either Motor or Instrument, **should be passed in Device constructor**. See DeviceType enum at 'Device.h'. Connected methods: Device::getType()
	* Device ID - integer that identifies each device. Must be unique among devices of the same type, different type devices may have same ID's. **Device ID should be set up in every Device class implementation by setting Device::dev field**. Connected methods: Device::getID().
	* Error management - each device stores dynamic array(std::vector) of strings that describe errors. Once error happened, error description should be pushed to Device::errors field, then DeviceManager should be called(see below). Connected methods: Device::hasErrors(), Device::pollError().
	* Mutex - each device has mutex that is used to prevent races and data corruptions. You can use it by calling: Device::lock(), Device::unlock(), Device::tryLock()
	* Configuration - each device can use CalX configuration system to store any variadic parameters and allow system to control them. Connected methods: Device::getConfiguration()
	* Logging - each device is able to log events using Device::log method that redirects logging to one of CalX logging system sinks.
And implementation specific part:
	* Device Manager - each Device API implementation has own DeviceManager(see below) and Device should store pointer to it. You should override Device::getDeviceManager
	* Device and runtime info - device may provide additional info to CalX system. This info is not used by system itself, however user may view it. You should override Device::getDeviceInfo() to provide info about device hardware and Device::getRuntimeInfo() to provide info about current state. They both may return fake values.
* Device connection - system may support different device connection ways. Device manager defines how each type of device may be connected to the system. Some devices are initialized at the start by device managers and they cannot be connected in any other way, some may be attached later. Currently system provides only serial port connection classes, but adding new ones is easy. See 'Device.h' DeviceConnectionType, DeviceConnectionPrms and below.


Now about each device type separately:
* Motor - representation of any stepper motors. It is subclass of Device and also it should implement folowing methods:
   * motor_coord_t getPosition() - returns current motor position it steps. motor_coord_t - integer value.
   * bool isTrailedPressed(int) - indicates specified trailer state(valid arguments are 1 and 2).
   * bool isRunning() - tells, if motor is moving at this time.
   * Power getPowerState() - tells, if motor currently is enabled/disabled. See Power enum in 'Device.h'. Power may have values: NoPower, HalfPower, FullPower. If your devices doesn't have half power mode, just ignore it; if they are enabled always, just return FullPower.
   * bool enablePower(bool) - enables/disables motor. Should return false if error occured.
   * bool start(motor_coord_t, float, unsigned char, bool) - move motor to the position(1 argument) with given speed(2 argument). Third(divisor) and fourth arguments may be ignored(moreover fourth argument is currently reserved for synchronization). This method must be asynchronious and finish as fast as possible. Should return false if error occured.
   * bool stop() - stop device if running. Must be asynchronious and not block control-flow. Should return false if error occured.
* Instrument - representation of of any cutting instruments. It is subclass of Device and also it should implement folowing methods:
	* bool enable(bool) - enable/disable instrument depending on argument. Should return false if error occured.
	* bool enabled() - indicates instrument state.
	* bool open_session() - open instrument session(prepare for further work).
	* bool close_session() - close instrument session(shutdown it);
	* InstrumentMode getMode() - returns current instrument mode. Currently system defines such modes: Off, Prepare, Full. See InstrumentMode enum in 'Device.h'
	* bool setMode(InstrumentMode) - change instrument mode.  Should return false if error occured.


And Device Manager which brings all the things together. Most DeviceManager operations are semi-common for all Device APIs - public interface is already implemented, but inner part is implementation-dependent:
* Motor and Instrument holding - DeviceManager already is able to hold Motor and Instrument objects and provide it to users by getters DeviceManager::getMotor(device_id_t), DeviceManager::getMotorCount(), DeviceManager::getInstrument(device_id_t) and DeviceManager::getInstrumentCount(). However **each implementation must take care of:** adding devices to DeviceManager::dev and DeviceManager::instr arrays(std::vector) depending on their connection type, refreshing(implement DeviceManager::refresh method) and resource destroying on exit.
* Error handling - DeviceManager contains array of errors and methods similar to Device, but **filling this array and redirecting Device errors is implementation specific.**
* Mutex - DeviceManager contains mutex that can be used it calling: DeviceManager::lock(), DeviceManager::unlock(), DeviceManager::tryLock()
* Configuration - DeviceManager has no any configuration itself, but provides standart way to load configuration from external files. See DeviceManager::loadConfiguration(std::string, ConfigManager*). First argument is relative path to file and second is configuration manager. Each device contains own ConfigManager, so you should only pass pointer to it.
* Device connection - DeviceManager stores allowed conntection types for all device types and provies appropriate getters: DeviceManager::getConnectionTypes(std::vector<DeviceConnectionType>&, std::vector<DeviceConnectionType>&), DeviceManager::canMotorConnect(DeviceConnectionType), DeviceManager::canIntrumentConnect(DeviceConnectionType). System uses them to display possible connection controllers, however **methods DeviceManager::connectMotor and DeviceManager::connectInstrument of course must be implemented**

To allow loading your module you should define function:
```C++
extern "C" LIBEXPORT DeviceManager *getDeviceManager(); // LIBEXPORT - Ctrl-lib defined macro. You may replace it with __declspec(dllexport) on Windows
```

### Connection with CalX system
Device API implementation should be represented as a single atomic module and must be compiled into dynamic linking library. Device API depends on CalX Ctrl-lib functions and definitions, so version of CalX Ctrl-lib library and its headers are necessarry for development. Then you should create own configuration file(see 'misc/main.conf.ini') with extension '.conf.ini' or edit existing(most significant optionas are library name and step-unit conversion parameters). Whole CalX system is built using CMake build system and Makefile-based custom system so it's recommended to include your implementation in system source tree and edit build files.

### Examples
By 26/05/2017 project contains two Device API implementations:
* Standa 8SMC1 motor and EKSPLA NL300 laser drivers - production code. May be used as a reference, but implementations are quite complex and messy.
* Emulation Device API - clean and straightforward code, may be used as a skeleton for your library.

### Licensing
Despite the fact that other CalX code is licensed under the terms of GNU LGPLv3, your Device API drivers and implementation may freely copy code from 'device/emulated'(Emulation Device API) and have no licensing restrictions.
