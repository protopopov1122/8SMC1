## Other notes about CalX
This section contains notes about CalX system that can't be included in any other manual part.
### CalX Configuration system
CalX Ctrl-lib contains simple configuration system that is widely used across the application. This configuration system is similar to INI files but has stronger typing. Configuration may be attached to any system component and it is controlled by configuration manager. Configuration manager holds single configuration and consists of named entries. Each entry may include key-value pairs with following types: 64-bit signed integer, 64-bit floating point decimal, boolean and string. All this structure is purely virtual and may be hold only in RAM. However, configuration import and export is also possible. Import/export format is INI file with some special enhancements(almost all of them are backward-compatible):
* Comments may start with '#' from any position in line
* All numbers are threated as integers or reals
* Booleans are 'true' and 'false'
* String may be in double-quotation symbols "...", however all other values are also threated as strings
Configuration validation mechanism is also included in Ctrl-lib but is not used by the application.
### CalX extensions
CalX system contains simple extension engine. It is interface between Ctrl-lib and some dynamic link libraries. Main system configuration defines which library(called engine) may be loaded, but it works only in UI mode. Currently extension interface is poor and by default includes initialization and destroy functions as well as some system-wide events, all other functionality must be implemented in library itself. You could also write extension for UI application by overriding special class. Library has full access to CalX system manager and whole Ctrl-lib(and also UI if loaded appropriate plugin). In perspective extension engine could be expanded but it already has possibility to create scripting language bindings to the CalX.
### Testing
By now CalX system include very limited count of unit tests, project also contains sample GCode for testing. In the future testing may be added only to Ctrl-lib.
