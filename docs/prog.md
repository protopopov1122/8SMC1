## CalX system programming
This section discusses Ctrl-lib architecture and use in third-party software.
#### Abstract
CalX Ctrl-lib is functional core of whole system and it offers many possibilities that can't be accessed from the general user interface. Obviously sometimes may be easier writing own interface for Ctrl-lib. Ctrl-lib license is GNU LGLPv3 so it can be used in any other projects if you don't change Ctrl-lib code. This manual written to help discovering Ctrl-lib code so without constant code reading it may seem incomplete. Currently this section is not finished.
#### Ctrl-lib Architecture
Firstly you need to read [used concepts](https://github.com/protopopov1122/CalX/blob/master/docs/concepts.md) section, because it just locates abstractions that are used while projecting Ctrl-lib in the file tree.
* Top level abstractions and facades are located directly in the Ctrl-lib directory:
  * SystemManager - main library component that controls and manages all used resources. Many operations require system manager to be passed as an argument.
  * Motor and instrument controllers - high level abstractions that are used to control separate devices. They provide most used Device API functions and new hardware independent operations.
  *
  * Configuration - configuration manager is widely used across application. There also is configuration validator but is is not currently used.
  * Request subsystem - CalX contains special configuration subsystem that allows calling different library components in the identical way
  *  Miscellaneous - logging subsystem, event listener definitions, extension engine interface, common constant and enumeration definitions.
