## CalX system programming
This section discusses Ctrl-lib architecture and use in third-party software.
#### Abstract
CalX Ctrl-lib is functional core of whole system and it offers many possibilities that can't be accessed from the general user interface. Obviously sometimes may be easier writing own interface for Ctrl-lib. Ctrl-lib license is GNU LGLPv3 so it can be used in any other projects if you don't change Ctrl-lib code. This manual written to help discovering Ctrl-lib code so without constant code reading it may seem incomplete. Currently this section is not finished.
#### Ctrl-lib Architecture
Firstly you need to read [used concepts](https://github.com/protopopov1122/CalX/blob/master/docs/concepts.md) section, because it just locates abstractions that are used while projecting Ctrl-lib in the file tree.
* Top level abstractions and facades are located directly in the Ctrl-lib directory
* Device API common code and abstract classes are in 'ctrl-lib/devices'
* Coordinate plane filters and interfaces - 'ctrl-lib/plane'
* Different task implementations - 'ctrl-lib/task'
* Coordinate translators and interface - 'ctrl-lib/translator'
* Math engine and parser - 'ctrl-lib/graph'
* Helper classes and algorithms - 'ctrl-lib/misc'

#### Programming
All CalX Ctrl-lib functions can be accesed by including single 'ctrl-lib/SystemManager.h' which is facade to all system functions.
CalX Ctrl-lib designed to be used according to this pattern:
* Device API implementation - you should have existing Device API driver to start working with the library. You may use or extend existing drivers to fit your needs, however Ctrl-lib heavily relies on Device API driver
* Device API, configuration and extensions preloading - your code should prepare and initialize valid Device API exemplar(currently external library is used for this, however it is not obligatory). Configuration and extensions are not necesarry, it's your own decision
* System manager instantiation - you should create SystemManager object, which is central Ctrl-lib abstraction and pass to it arguments prepared in previous step. All further work should be done through SystemManager.
* Connect devices according to your Device API implementation, attach planes, load tasks, etc. - work with Ctrl-lib API
* Destroy SystemManager and Device API instance through default C++ destructors.

#### Extensions
CalX system have basic extension support. You can attach to SystemManager special extension object(ctrl-lib/ExtEngine.h), that will receive high-level system events and have full access to Ctrl-lib API. You can also overload special class, that have full access to GUI(however GUI has no special API and has quite messy code).
