## CalX User Interface
Graphical User Interface is the main goal of development, so currently it represents Ctrl-lib offered functions better that CLI. On the other hand, GUI may seem quite complex because it tends to cover most library functionality.

##### Interface organization
Main program window is split into 6 tabs to mirror Ctrl-lib subsystem structure. There are also other dialogs: initial configuration selector and debug-console.

##### Devices tab
First main application tab represents separate devices. It includes device connection panel and currently connected device list.

Device connection panel shows device connection ways, that are supported by Device API driver. It may be hidden if driver doesn't support dynamic connection. Device connection is based on connection dialogs that specify connection parameters and mode.

Device list contains devices sorted by connection time. Each device is stored in its own frame and different device types have different frame configurations. Left frame side shows current device state, but right contains buttons and controls. Frames may slightly blink because of constant state updates. Each device has dynamic configuration parameters, however their usage depends on Device API driver.
* Motor frame - left frame part shows current motor position, power, trailer state and other parameters. Motor position is showed in the hardware-dependent steps. Motor controls are split into position control(in the center) and discrete state controls(in the right). Position control allows changing motor position with given speed. Position may be changed absolutely(moving to the coordinate) and relatively(moving to the N steps from the current position). Discrete state controls include power control, movement to trailer(moving to minimal/maximal coordinate of motor with given speed), motor movement stop and configuration. Usually you need to ensure that motors are powered on before starting, otherwise you will get errors. See [error description](error.md).
* Instrument frame - instrument parameters, hardware and runtime info are shown on the top, left frame part tells current instrument state(enabled/disabled), in the center is placed state and configuration controls, but at the right side is located discrete instrument mode changer.

Device frames may be locked if any higher-level abstractions involve these devices in any actions.

##### Coordinate plane tab
Coordinate plane tab is the second application tab. It includes two parts: coordinate plane list and current coordinate plane. Both part sizes may be adjusted. At the beginning no coordinate planes are defined, so you need to create one by pressing 'Add' button and selecting appropriate devices. Now selected devices will be attached to this plane and any action with the plane will lock devices(and otherwise - device activation will lock plane).
All positions and coordinates are shown and calculated in configuration specified units.

Coordinate plane frame is split into different movement type controls(all of them are hideable). Current plane position may be called plane pointer. Information about the plane is located on the frame top. Movement stop button and watcher caller button are also there. Watcher is a non-modal dialog that shows coordinate plane state and movement history(plane must be measured before, see above). Then in column comes different movement type controllers:
* linear movement - coordinate plane pointer can be moved to the given coordinate. Movement can be synchrone(and with enabled instrument if it is attached) and asynchrone(instrument is disabled). Asynchrone movement is usually faster, but absolutely not accurate. Movement can be absolute(move to the given coordinates) and relative(add given coordinates to the current position).
* arc movement - plane pointer can be moved along the part of circle. You should provide circle center coordinates and description coordinates. If given coordinates do not approximately match circle, you will receive an error message. Then arc is split into chords; splitter represents chord count in the full circle(thus bigger splitter means shorter chords). Speed control has been described in the linear movement section. Arc movement may be clockwise and counter-clockwise. Relative movement means that description and center coordinates are calculated using current plane position.
* function graph builder - CalX allows arbitrary function graph building. On the left is graph parameter controls. Firstly, you should define math function(read [math engine manual](math.md) for more info). Then X and Y axis ranges. If X axis range is reversed, graph will be built from the maximal X to minimal. X axis step represents graph building precision. Build speed is the same as the linear movement speed. On the right is localed coordinate system control. By default, Decart coordinates are used, but you may put logarithmic or polar coordinates on top of them. You may preview graph(plane must be measured before) and build it.

On the right frame side is located coordinate plane settings. There you may temporarily disable instrument usage on this plane. Below are placed calibration and measurement buttons. Calibration means moving both motors to the start/end of the coordinate plane. Measurement is essential and recommended before plane usage(for this purpose position change control may be easier), plane calibrates motors to the both sides to calculate available coordinate range. Then comes position setup(by default hidden). There you may move pointer to the part of plane specified in the range from 0 to 1(plane needs to be measured) or configure plane to measure, place pointer to position and set it as a start point. **Plane configuration is recommended before usage**. On the filters tab you can change some plane parameters, however it usually is not needed and you need to known what exactly you changed. There you can setup logging, coordinate plane center offset and coordinate scale(WARNING, both axis scale must be changed simultaneously otherwise arc movement will break) and coordinate or speed validation.

##### Task tab
Third application tab is a task manager. It consists of task list and loader, task building plane and area for specific task controlling(its content depends on task type).

On the right tab side you can see opened task list, add or remove them, selected task parameters will appear in the central area. On the top are located common task functions:
* build - you can build selected task on any available coordinate plane with base speed. Base speed is maximal speed that motors will reach during task execution(however they may be faster during jumps if it specified in config).
* preview - you can view, how selected task will look on any available coordinate plane. This coordinate plane should be measured first. Preview building is not momental and may take some time.
* linearization - each task may be transformed into simple GCode that consists only of linear movements and jumps. After linearization GCode is saved as new task and may be exported to the file system.

Currently, three task types are accessible:
* GCode tasks - main task type. You can load any valid GCode(that uses only supported operations) and execute it on any plane. On the central pane you will see loaded GCode and can change coordinate translators to adjust code position and scale on plane. You should change translation scale carefully and simultaneously, also preview before building is recommended.
* Linear task - you are able to create vertical or horizontal lines on given rectangle and with given space between them. On the central area you can change rectangle coordinates, space between lines and their direction.
* Programmed task - you can program any elementary movement(jump, move, arc) sequence. You will see action sequence as a list in central area, change action sequence and add/remove them by pressing buttons below. Each action type has its own parameters that will be displayed by selecting an action.

##### Configuration
CalX system has its own configuration system - read the description. UI contains component that manages given configuration and gives possibility to add/remove/change parameters, add/remove entries. Application contains different configurations for different subsystems, so the same component will appear in many UI parts. Main application configuration is located in the fourth tab. Component is quite intuitive: on the left you will see entry list, add or remove them, on the right you will see selected entry key-value pair list. You can add new key-value pair, remove and change selected pair. [Read mode](other.md) about configuration system.

##### Scripting tab
CalX system has a support of external scripting. Abstract scripting interface is offered to the external libraries, but user is able to make calls to the external scripting engine. There are two possible interactions with scripting engine:
* loading an external script from the filesystem - script is executed directly and then user is able to invoke functions and execute arbitrary code in the scope of executed script.
* executing scripts directly - user is able to execute scripts in the separate scope.
CalX scripting bindings cover the most essential parts of system, however they lack the flexibility of system core. Scripting bindings are also synchronized with user interface.

##### Math engine
User is able to evaluate predefined math formulas, defining custom variable values. Currently math formulas can be precompiled or defined in the initialization script. Using configuration file is impossible (due the lack of list support in system configuration format), embedding formula input dialog into UI is inconvenient for the end-user.

##### Debug-mode console
If debug-mode is enabled in the configuration, on Windows you will see second application window with CLI console and logging(if logging is enabled). On Linux logging and CLI will be on the main console. You can work with CLI in debug-mode, however GUI and CLI are not synchronized and you should be careful.

##### Configuration-selection dialog
Main application configuration is stored in the app root directory and have extension \*.conf.ini. If no configuration is provided, application will tell you about it and use built-in settings and constants. If multiple configuration are present, application will show dialog with existing configuration paths, names and descriptions(if they are defined) and you will be able to select preferred configuration.
