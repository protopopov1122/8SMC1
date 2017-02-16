This file contains detailed information about project goals.
------------------------------------
All work on the project may be splitted into few steps:
* Write more tests for different parts of system.
* Writing wrapper API to cover all existing USMC API functions. Reason: create abstraction layer on top of existing API(this may prevent compability problems, see 'Other' section)
* Implement high-level API on top of wrappers. Reason: perform error processing, speed control, some more abstract but useful functions.
* Create command-line interface(CLI) to test existing API and integrate all system. Reason: testing is easier, system may become useful earlier.
* Write G-Code interpreter for system API, integrate interpreter into CLI. Reason: G-Code allows working with a lot of different existing applications, it is standart way for all CNCs.
* Write function graph generator that will allow plotting on plane.
* Integrate system into CAD systems. Main project goal.

Start goals:
* Understand how all existing API options work.
* Cover all MicroSMC driver API with C++ wrappers.
* Extend command-line interface(CLI) to make it really usable to fully control 8SMC1 and test other project parts.
* Test all wrapper functions on real controller.
* Implement high level API on top of existing wrappers.

Higher level API goals:
* Use only C++ wrappers to interact with controller(NO direct USMC API).
* Provide reasonable error handling.
* Observe controller modes to prevent errors(e.g. control maximal motor speed).
* Make CLI mostly use high level API.

G-Code interpreter:
* Write G-Code parser that will parse any G-Code written by standart(may also use existing open-source tools).
* Write interpreter for small subset of G-Code commands(seems we only need positioning commands).
* Integrate interpreter with CLI or write standalone tool(for testing) and create GUI program.

Function plotter:
* Write simple math expression parser(req. constants, variables, functions and operators).
* Write math expression evaluator based on parsed expression.
* Integrate evaluator with other API and CLI.

Integration:
There are two ways of integration: write standalone apps for local computer(pros: faster development, do not require many technology use; cons: local computer is slow and obviously can't handle all used software), writing client-server app(pros: can use more powerful computer for integration; cons: require different teachnology integration and network connection).
First way requires:
* Integrate G-Code interpreter with AutoCAD.
* Write extension for Inkscape to use G-Code interpreter with Inkscape G-Code extension.
* Write standalone application for function plotter.
Second way requires:
* Write server for computer with controllers that will represent system API for network applications
* Write client: it may be Web-application, local client app, many local scripts. It will use server network API to control devices.

Other:
* Current MicroSMC driver that is used by wrapper is available only for Windows 2000/XP/Vista. The simpliest solution is using old hardware or virtual machine, but then intergration with existing software may be hard. Another option is using different controller with same stepper motors. This will require a lot of work with writing new driver and C++ wrappers but benefits are significant(e.g. using with newer Windows versions and Linux, simplier integration). This is the main reason of separating low-level wrapper API and higher-level API and other tools(if we will change controller we will only rewrite wrappers, other project parts will be the same). By now development is continued on top of 8SMC1 controller using old Windows XP PC, but this problem should be solved anyway.
