## NEWS
This file contains log of development process(testing results, achieved goals, current state)

##### 06/02/2017
First program test on real controllers. System contains simple CLI, most significant USMC API wrappers, USMC API stub implementaion.
What tested:
* Start on Windows 7 - failed(driver not working), works only on Windows XP
* CLI test - command line works fine, however some functions do not perform NULL pointer check and cause crash on invalid data.
* Power up/down - failed, I used manufacter software to power up controller. Possible reason: wrong power up parameters.
* Positioning commands(Start, Stop, Fast Stop) - works. All parameters are properly used.
* Controller state checking - works. Most useful state info is printed(power, voltage, temperature, position, trailer states etc.)
* Controller mode modifying commands - failed. Reason: wrong parameters.

Conclusion: need to fix power up/down commands, controller mode modifying and find out about all controller parameters.

##### 07/02/2017
* Added G-Code parser and test to it.
* Maybe fixed power up bug.

TODO: Test power up/down on real controller.

##### 09/02/2017
System test on real controllers.
Tested:
* Power up - bug fixed, controller power ups motor by command.
* Initialisation - failed. System can't initialise itself. TODO: Fix.
* Movement - tested separate device movement(start/stop/x commands). Works well.
* Roll to trailer - works. Trailer numbers were confused but this bug is fixed.
* Coordinate plane - works, however speed check must be performed.
* Calibration - basic calibration works(Basically it is both motor rolls).

Other:
* Detected maximal motor speed(approx. 6000 steps/s).
* Detected required motor comeback after roll(approx. 800 steps).


TODO:
* Motor synchronization during movement.
* Virtual coordinates mapping to real.
* Maximal speed checks.
* Refractoring.

Conclusion: development goes well, most useful system core functions being sucessfully developed, however a lot of work should be done.

##### 10/02/2017
Refractored code. TODO:
* Retest all high level functions: roll, calibrate, coord move
* Test synchronization(barely impossible but still)

##### 11/02/2017
Heavy refractoring. Must recheck all functions on the next testing.

##### 12/02/2017
More refractoring. Before tests proper work not guaranteed. TODO: Test all cli functions one more time on real motors.

##### 14/02/2017
After refractoring and code rewrite not all functions were tested, some bugs also were found, system will be more stable after few next test, but now not all functions work as expected.

##### 16/02/2017
Test on real controllers. Found bug: speed projections were not calculated as expected(should have fixed it but have not tested fix). This bug affected arc and synchronized movement. All other functions work. TODO: Retest all functions next time.

Other:
Now development will continue in two main branches(and some temporary ones): master(for the main development process) and testing(to commit bugfixes during tests). Some decisions were made about system integration(however they are not final) - see TODO.

##### 18/02/2017
A lot of refractoring and new features. Most bugs after last test should be fixed, althrought because of new features some functions might not work. Also integrated G-Code parser into system. Looking forward to the next test on motors(most likely will be on Monday 20/02/2017). If there will be no bugs then core of the system will be almost implemented(there are some other system functions to implement, however most of the work is done). The next task is writing GUI and integration on top of project core.

##### 20/02/2017
Test on real motors. All tested functions work:
* Clockwise and counter-clockwise arc movement.
* G-Code - tested linear G-Code patterns(performs well), however all other also should work.
* Function graphs - tested simple function graphs(e.g x, x^2), they work correctly.
* All other functions(calibration, logging, mapping) - work.

Conclusion: next time(probably 23/02/2017) more tests must be performed. The next test pack should include more G-Code patterns and graph types(including different arcs and so on), also some minor fixes and features will be included till the next test.

TODO:
* Implement facade pattern for all system functions to make further work simplier.
* Create GUI for the system: GUI supposed to have client-server architecture(currently, we don't know which technologies will be used). Server will interact with facade to control system and provide network protocol. Client will connect to server and send commands/receive data from it(client most likely will be native Windows or .NET application, but server will be implemented in C++). However on the next test time more detailed info about UI will be available.

##### 23/02/2017
Test on real motors. All system functions work, however GCode parser should be rewritten to be able using most crappy GCode(currently only good formatted one is working). Other system functions work fine.

Tested(all subsystems works fine):
* Complex GCode patterns.
* Function drawing.
* Coordinate plane filters.

It was decided that client-server API is not necessarry, so GUI will be written for local computer(most likely C++ and Qt will be used).

TODO:
* Rewrite GCode parser.
* Add laser related code(It will be integrated in different system parts).
* Add some other components(polar coordinates, get/set options to coordinate plane filters, new filters - history filter, coord plane emulator).
* Implement facade and GUI.

Conclusion: project functional core is mostly implemented, however laser support must be integrated, so the next thing is GUI.

##### 25/02/2017
Added MSVC compiler support. TODO: test its executables on real controllers.

##### 26/02/2017
Started implementing UI based on WxWidgets(require version 3.10), currently added only compilation support and stub. 

##### 03/03/2017
Latest tests were performed on 02/03/2017, all CLI functions work, wxWidgets compiles and runs on WinXP properly. Next task is writing GUI. By now part of GUI to control separate devices is almost implemented.

##### 04/03/2017
UI is under heavy development, must be tested, however all implemented functions should work.

##### 06/03/2017
UI test on real motors. All implemented functions work, however log into wxTextCtrl is quite slow and should be disabled/rewritten.

##### 09/03/2017
UI tested on real motors. All covered functions work properly, program is slow at first start, however further work is fast enought. Memory leaks were not detected, but their existence must be checked more carefully, because application is supposed to work long period of time without restart(minimum 12 hours).

##### 10/03/2017
Device API was refractored to offer abstract interfaces. Concrete implementations were moved to separate subdirectory. Now project may use any controllers, but some extra work on this should be done. Project was renamed to CalX.

##### 17/03/2017
Most of project is finished. GUI is almost functional for many purposes(however it does not represent all API functions). Device API fully separated from other project components and used as a plugin(so many Device API handlers may be supported). Added Instrument support to Ctrl-lib and UI(however 8SMC1 Device API implementation can't handle them now and use stubs).

##### 30/03/2017
GUI and system itself is almost fully functional. Most of the functions were implemented and added. The last part is connection to laser commands for out laser(Eksma NL300) and some small improvements.

##### 08/04/2017
Project is almost completely finished. Last part is laser integration and system final installation, after that project alpha version will be added on GitHub. Then I'm planning to add some features, that are not necessary now(e.g. compilation and work on stub drivers on Linux, console interface extension, some other stuff), and release it as final version.
There are also some further work planned: integrate other device types in system structure and interface.