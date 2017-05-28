## CalX system concepts
This section contains main system concept description. System is projected to be flexible and portable, so main idea is combining different level concepts and abstractions. Graphical user interface and core library both are trying to represent this philosophy.
##### Devices
The lowest platform-independent part is device abstraction. It offers standart ways of interaction with different models of stepper motors and cutting instruments. All devices can be controlled separately and connected in different orders. It is not convenient to control devices this way, however it is possible.
##### Coordinate planes
Coordinate plane is next level of abstraction. It is combination of few devices that synchronizes them and offers coordinate interface. Most movement strategies are implemented at this level. Although coordinate plane is managing devices, it is an abstraction, so system includes different coordinate plane implementations that work as filters - process given arguments and commands and pass it to the next plane. This naturally leads to stack abstraction: you work with top plane, but motors are controlled by bottom one.
##### Tasks
Then comes tasks. Task keeps movement and other plane operation sequence and can perform it on given coordinate plane and speed. Tasks are used to represent complex figures and paths. GCode, which is well-known CNC programming standart, is directly translated into tasks.
##### Coordinate translators
Coordinate planes operate with integer values, because motor steps have no fraction part. However, it may be useful to operate with floating point values just like real coordinate plane. Coordinate translator is used to scale floating point coordinates into plane steps. Translators also are abstract conception and may be stacked in the same way as coordinate planes to represent complex coordinate systems.
##### Other subsystems
CalX system contains few subsystems that can't be included in the hierarchy however they are necessary for correct functioning:
* Math engine - CalX allows building mathematical function graphs, so it includes simple mathematical expression parser and evaluator and some helper classes.
* GCode parser - parser industry standart CNC programming language GCode into tasks.
* Configuration manager - project contains .ini file parser and virtual ini-like structure manager to keep and modify configuration on the fly.
* Bresenham algorithm implementation for circles

### Concept combining
CalX architecture heavily relies on abstractions and interfaces, most system parts may be combined in different ways. This flexibility is hard to reflex in user interface, so Ctrl-lib is much more flexible and abstract than UI, so in some specific usages may be simpler to write own interface for it.