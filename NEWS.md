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

