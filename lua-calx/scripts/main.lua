--[[
	API reference
	    calx.serial.pairty                                Holds possible serial port parity constants
		    .No, .Odd, Even,
		    .Mark, .Space
		calx.motor                                        Motor controller bindings
		    .connectSerial(int, int, int)                 Connect motor by serial port(pamaeters: port, baudrate, pairty)
            .getCount()                                   Return current connected motor count
		    .getPower(int)                                Return specified motor power state
		    .enablePower(int, bool)                       Change motor power state
			.move(int, int, float)                        Start motor move
			.relativeMove(int, int, float)                Start motor move relatively to current position
			.stop(int)                                    Stop motor movement
			.getPosition(int)                             Return current motor position
			.waitWhileRunning(int)                        Pause execution while motor is moving
			.moveToTrailer(int, int)                      Move motor to specified trailer
			.checkTrailers(int)                           Check if motor reached one of trailers
		
		calx.motor.power                                  Holds power state constants
		    .No, .Half, .Full
		calx.motor.trailer
            .Top, .Bottom
					
		calx.instrument                                   Instrument controller bindings
		    .connectSerial(int, int, int)                 Connect instrument by serial port(pamaeters: port, baudrate, pairty)
		    .getCount()                                   Return current connected instrument count
			.openSession(int)                             Open selected instrument session
			.closeSession(int)                            Close selected instrument session
			.setRunnable(int, bool)                       Make selected instrument runnable
			.isRunnable(int)                              Check if instrument is runnable
			.enable(int, bool)                            Enable selected instrument
			.isEnabled(int)                               Check if selected instrument is enabled
			.getMode(int)                                 Get selected instrument mode
			.setMode(int, int)                            Set selected instrument mode
			.isSessionOpened(int)                         Check if instrument session opened
			.getInfo(int)                                 Get instrument text info
		
		calx.instrument.mode                              Possible instrument working modes
		    .Off, .Prepare, .Full
			
		calx.plane.create(int, int, int)                  Create plane using 2 motors and 1 instrument(optional)
		calx.plane.move(int, double x, double y,          Move plane to position (x;y) with speed S, synchroniously(sync), relatively(rel)
		                double S, bool sync, bool rel)
		calx.plane.arc(int, double x, double y,           Arc movement to point (x;y) with center on (cx; cy), splitter spl,
		               double cx, double cy,              speed S, clockwise, relative
					   int spl, double S,
					   bool clockwise,
					   bool rel)
--]]


function init()
	m1 = calx.motor.connectSerial(1, 9600, calx.serial.parity.No)
	m2 = calx.motor.connectSerial(2, 9600, calx.serial.parity.No)
    i1= calx.instrument.connectSerial(4, 19200, calx.serial.parity.No)
	p1 = calx.plane.create(m1, m2, i1)
	calx.plane.configure(p1, 0.5, 0.5, 1.25)
	calx.plane.newWatcher(p1)
end
