--[[
	API reference
	    calx.serial.pairty                                Holds possible serial port parity constants
		    ["No"], ["Odd"], ["Even"],
		    ["Mark"], ["Space"]
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
		    ["No"], ["Half"], ["Full"]
		calx.motor.trailer
            ["Top"], ["Bottom"]
					
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
		    ["Off"], ["Prepare"], ["Full"]
--]]


function init()
	calx.motor.connectSerial(1, 9600, calx.serial.parity["No"])
	calx.motor.connectSerial(2, 9600, calx.serial.parity["No"])
    calx.instrument.connectSerial(4, 19200, calx.serial.parity["No"])
	print("Connected " .. calx.motor.getCount() .. " motors and " .. calx.instrument.getCount() .. " instruments")
	calx.motor.enablePower(0, true)
	calx.motor.enablePower(1, true)
	print(calx.motor.getPower(0) ~= calx.motor.power["No"])
	calx.motor.move(0, 0, 4000)
	calx.motor.move(1, 0, 4000)
	calx.motor.moveToTrailer(0, calx.motor.trailer["Top"])
	calx.motor.moveToTrailer(1, calx.motor.trailer["Bottom"])
	print(calx.motor.getPosition(0) .. "x" .. calx.motor.getPosition(1))
	calx.instrument.setRunnable(0, true)
	calx.instrument.setMode(0, calx.instrument.mode["Full"])
	calx.instrument.openSession(0)
	calx.instrument.enable(0, true)
	print(calx.instrument.getInfo(0))
	print(calx.instrument.isSessionOpened(10))
end
