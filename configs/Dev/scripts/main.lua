-- Initialization script
-- CalX Lua helper library
local calxLib = require 'calxLib'

-- Initialization procedure
function init()
	-- Default plane configuration center
	local configure_x = 0.5
	local configure_y = 0.5

	-- Retrieve plane configuration center from settings
	local settings = calx.settings
	if settings:has('relative_pos') then
		configure_x = settings:get('relative_pos'):float('x', configure_x)
		configure_y = settings:get('relative_pos'):float('y', configure_y)
	end

	-- Connect two motors and one instrument by COM port
	local m1 = calx.motors:connect(1, 9600, calx.serial.parity.No)
	local m2 = calx.motors:connect(2, 9600, calx.serial.parity.No)
	local i1= calx.instruments:connect(4, 19200, calx.serial.parity.No)
	-- Create coordinate plane
	local p1 = calx.planes:create(m1, m2, i1)
	-- Configure it
	p1:configure(calx.point.new(configure_x, configure_y), 1.25)
	-- Open watcher dialog
	p1:openWatcher()
	-- Start REPL if necessary
	if calx.config:get('script'):bool('repl', false) then
		calxLib.repl()
	end
end
