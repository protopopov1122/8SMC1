--[[
	API reference
		calx                                              Object exported from application. It contains all available CalX API
		    .connectSerialMotor(int, int, int)            Connect motor by serial port(pamaeters: port, baudrate, pairty)
		    .connectSerialInstrument(int, int, int)       Connect instrument by serial port(pamaeters: port, baudrate, pairty)
            .motorCount()                                 Return current connected motor count
		    .instrumentCount()                            Return current connected instrument count
		    .motorPower(int)                              Return specified motor power state
		    .enableMotorPower(int, bool)                  Change motor power state
			.motorMove(int, int, float)                   Start motor move
			.motorRelativeMove(int, int, float)           Start motor move relatively to current position
			.motorStop(int)                               Stop motor movement
		
		calx.power                                        Holds power state constants
		          ["No"], ["Half"], ["Full"]
		calx.pairty                                       Holds possible serial port parity constants
		          ["No"], ["Odd"], ["Even"],
		          ["Mark"], ["Space"]
		calx.trailer
		            ["Top"], ["Bottom"]
--]]


function init()
    calx.connectSerialInstrument(4, 19200, calx.parity["No"])
	print("Connected " .. calx.motorCount() .. " motors and " .. calx.instrumentCount() .. " instruments")
	calx.enableMotorPower(0, true)
	calx.enableMotorPower(1, true)
	print(calx.motorPower(0) ~= calx.power["No"])
	calx.motorMove(0, 0, 4000)
	calx.motorMove(1, 0, 4000)
	calx.motorMoveToTrailer(0, calx.trailer["Top"])
	calx.motorMoveToTrailer(1, calx.trailer["Bottom"])
	print(calx.motorPosition(0) .. "x" .. calx.motorPosition(1))
end
