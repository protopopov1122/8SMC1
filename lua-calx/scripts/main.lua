function repl()
	local function print_results(...)
		if select('#', ...) > 1 then
			print(select(2, ...))
		end
	end

	local function ends_with(str, ending)
		return ending == '' or str:sub(-#ending) == ending
	end

	local function read_code()
		local code = ''
		local reading = true
		while reading do
			reading = false
			local line = io.read('*line')
			if #line > 0 and ends_with(line, '\\') then
				reading = true
				code = code .. line:sub(1, #line - 1)
			else
				code = code .. line
			end
		end
		return code
	end
	
	print('Starting Lua REPL')
	environment = {
		PROMPT = '> ',
		EXPR_MODE = true,
		_REPL_WORK = true,
		exit = function () environment._REPL_WORK = false end
	}
	setmetatable(environment, {
		__index = _ENV
	})
	while environment._REPL_WORK do
		io.write(environment.PROMPT)
		io.stdout:flush()
		local code = read_code()
	
		local f, err = load(code, 'stdin', nil, environment)
		if err and environment.EXPR_MODE then
			f = load('return (' .. code .. ')', 'stdin', nil, environment)
		end
	
		if f then
			print_results(pcall(f))
		else
			print(err)
		end
	end
	print('Exiting Lua REPL')
end

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
