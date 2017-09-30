--[[
	API reference
		calx                                              Object exported from application. It contains all available CalX API
		calx.connectSerialMotor(int, int, int)            Connect motor by serial port(pamaeters: port, baudrate, pairty)
		calx.connectSerialInstrument(int, int, int)       Connect instrument by serial port(pamaeters: port, baudrate, pairty)
		calx.motorCount()                                 Return current connected motor count
		calx.instrumentCount()                            Return current connected instrument count
--]]


function init()
    calx.connectSerialInstrument(4, 19200, 0)
	print("Connected " .. calx.motorCount() .. " motors and " .. calx.instrumentCount() .. " instruments")
end
