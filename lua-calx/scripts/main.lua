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
--]]


function init()
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
end
