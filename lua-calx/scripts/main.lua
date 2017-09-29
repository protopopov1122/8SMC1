--[[
	API reference
		calx                                              Object exported from application. It contains all available CalX API
		calx.connectSerialMotor(int, int, int)            Connect motor by serial port(pamaeters: port, baudrate, pairty)
		calx.connectSerialInstrument(int, int, int)       Connect instrument by serial port(pamaeters: port, baudrate, pairty)
--]]


function init()
    calx.connectSerialMotor(4, 19200, 0)
    calx.connectSerialMotor(4, 19200, 0)
    calx.connectSerialInstrument(4, 19200, 0)
end
