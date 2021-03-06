## CalX system concepts
This section contains main system concept description. System is projected to be flexible and portable, so main idea is combining different level concepts and abstractions. Graphical user interface and core library both are trying to represent this philosophy.
##### Devices
The lowest platform-independent part is the device abstraction. It offers standard ways of interaction with different models of stepper motors and cutting instruments. All devices can be controlled separately and connected in different order. It is not convenient to control devices this way, however it is possible.
##### Coordinate planes
Coordinate plane is the next level of abstraction. It is a combination of the two motors and an instrument (optional) that synchronizes them and offers the coordinate plane interface. Most movement strategies are implemented on this level. Although coordinate plane is managing devices, it is an abstraction, so system includes different coordinate plane implementations that work as filters - they process given arguments and commands then pass it to the next plane, using composition principles. This naturally leads to the stack abstraction: you work with a top plane, but motors are controlled by the bottom one.
##### Tasks
Then comes task conception. Task keeps movement and other plane operation sequence and can perform it on a given coordinate plane and with given speed. Tasks are used to represent complex figures and paths. GCode, which is a well-known CNC programming standard, is directly translated into tasks, some tasks are generated on the fly with given parameters.
##### Coordinate translators
Coordinate planes operate with integer values, because motor steps have no fraction part. However, it may be useful and simpler to end-user to operate with floating point values just like a real coordinate plane. Coordinate translator is used to scale floating point coordinates into plane physical steps. Translators also are an abstract conception and may be stacked in the same way as coordinate planes to represent complex coordinate systems.
##### Other subsystems
CalX system contains a few subsystems that can't be included in the hierarchy however they are necessary for correct functioning:
* Math engine - CalX allows building mathematical function graphs, so it includes a simple mathematical expression parser and an evaluator with some helper classes. See [math engine](math.md) description.
* GCode parser - parses CNC programming language called GCode(which is an industry standart) into tasks.
* Configuration manager - project contains .ini file parser and virtual ini-like structure manager to keep and modify configuration on the fly. See [configuration section](other.md).
* Bresenham algorithm implementation for circles.
* Journaling subsystem - CalX has own logging library which is also used for high-level system journaling.

### Concept combining
CalX architecture heavily relies on these abstractions and interfaces, most system parts may be combined in a different ways. This flexibility is hard to reflex in the user interface, so Ctrl-lib is much more flexible and abstract than UI, so in some specific occasions may be simplier to write own interface for it.
