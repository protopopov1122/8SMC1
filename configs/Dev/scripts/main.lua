function init()
	local configure_x = 0.5
	local configure_y = 0.5

	local settings = calx.settings
	if settings:has('relative_pos') then
		configure_x = settings:get('relative_pos'):float('x', configure_x)
		configure_y = settings:get('relative_pos'):float('y', configure_y)
	end

	local m1 = calx.motors:connect(1, 9600, calx.serial.parity.No)
	local m2 = calx.motors:connect(2, 9600, calx.serial.parity.No)
  local i1= calx.instruments:connect(4, 19200, calx.serial.parity.No)
	local p1 = calx.planes:create(m1, m2, i1)
	p1:configure(calx.point.new(configure_x, configure_y), 1.25)
	p1:openWatcher()
	if calx.config:get('script'):bool('repl', false) then
		repl()
	end
end
