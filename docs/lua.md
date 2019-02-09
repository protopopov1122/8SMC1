## Lua APIs
CalX includes Lua engine and exposes the most notable system APIs to Lua. This section documents existing APIs.

All existing CalX procedures and objects are included into the scope of `calx` global table.

### Data transfer objects and enumerations
There are several objects that are used for enumerations and data strcture transfer:
1. `calx.serial.parity` - COM port parity constants
```lua
calx.serial.parity.No
calx.serial.parity.Odd
calx.serial.parity.Even
calx.serial.parity.Mark
calx.serial.parity.Space
```
2. `calx.motors.power` - motor power state
```lua
calx.motors.power.No
calx.motors.power.Half
calx.motors.power.Full
```
3. `calx.motors.trailer` - motor trailer identifier
```lua
calx.motor.trailer.Top
calx.motor.trailer.Bottom
```
4. `calx.instruments.mode` - instrument modes
```lua
calx.instruments.mode.Off
calx.instruments.mode.Prepare
calx.instruments.mode.Full
```
5. `calx.point` - 2D point data structure
```lua
local point = calx.point.new(10, 20)
point.x
point.y
```
6. `calx.rect` - 2D rectangle
```lua
local rect = calx.rect.new(1, 2, 3, 4)
rect.x
rect.y
rect.w
rect.h
```

### Device APIs
CalX exposes several objects to work with separate devices
1. `calx.motors` - motors
```lua
calx.motors:count()                            -- total motor count
calx.motors:get(motor_id)                      -- get motor controller
calx.motors:connect(port_id, baudrate, parity) -- connect new motor by serial port
```
2. Motor controller returned by `calx.motors.get`
```lua
local motor = calx.motors:get(0)  -- getting the motor
motor:id()                        -- get motor identifier
motor:power()                     -- get motor power state
motor:enable(true)                -- enable/disable the motor
motor:move(destination, speed)    -- move motor to the absolute position
motor:relMove(destination, speed) -- move motor relatively to currrent positon
motor:stop()                      -- stop the motor
motor:position()                  -- get the position of motor
motor:toTrailer(trailer_id)       -- move motor to the specified trailer
motor:checkTrailers()             -- check if any trailer is pressed
motor:wait()                      -- wait while motor is running
```
3. `calx.instruments` - instruments
```lua
calx.instruments:count()                            -- get instrument count
calx.instruments:get(instrument_id)                 -- get instrument controller
calx.instruments:connect(port_id, baudrate, parity) -- connect new instrument by serial port
```
4. Instrument controller returned by `calx.instruments.get`
```lua
local instr = calx.instruments:get(0)     -- getting the instrument
instr:id()                                -- get instrument identifier
instr:open()                              -- open instrument session
instr:close()                             -- close instrument session
instr:opened()                            -- is session opened
instr:enable(true)                        -- enable/disable the instrument
instr:enabled()                           -- is instrument enabled
instr:runnable()                          -- is instrument runnable
instr:setRunnable(true)                   -- make instrument runnable/not runnable
instr:mode()                              -- get instrument mode
instr:setMode(calx.instruments.mode.Off)  -- set instrument mode
instr:info()                              -- get instrument info
```

### Configuration
Lua bindings expose read-only access to system configuration and settings.
1. `calx.config` - access to configuration entries
```lua
calx.config:has('entry_name')    -- check if entry exists
calx.config:get('entry_name')    -- get configuration entry
```
2. `calx.settings` - access to system settings
```lua
calx.settings:exists()        -- check if settings are attached to the system
calx.settings:has('entry')    -- check if settings entry exists
calx.settings:get('entry')    -- get settings entry
```
3. Configuration entries
```lua
local entry = calx.config:get('entry')    -- getting configuration entry
entry:has('key')                          -- check if entry has key 'key'
entry:int('key', 0)                       -- get 'key' integer value or 0
entry:float('key', 0.0)                   -- get 'key' floaring-point value or 0.0
entry:string('key', '')                   -- get 'key' string value or ''
entry:bool('key', true)                   -- get 'key' boolean value or true
```

### Planes
Lua bindings include plane-related functions
1. `calx.planes` - create and obtain plane objects
```lua
local plane = calx.planes:create(motor1, motor2, instrument) -- Create new coordinate plane providing appropriate device objects
calx.planes:count()                                          -- Count of currently registered planes
local plane2 = calx.planes:get(0)                            -- Get the 0th plane
```
2. Plane controller returned by `calx.planes.get`
```lua
local plane = calx.planes:get(0)                       -- Get the plane controller
plane:id()                                             -- Get plane identifier
plane:move(calx.point.new(x, y), speed, true, true)    -- Move coordinate plane pointer to the (x;y) with given speed synchroniously/asinchroniously in absolute/relative coordinates
plane:arc(calx.point.new(dx, dy), calx.point.new(cx, cy), splitter, speed, true, true) -- Perform arc movement to the point (dx; dy) with center in (cx; cy) splitting in into N hordes with given speed, clockwise/counter-clockwise in absolute/relative coordinates
plane:calibrate(calx.motors.trailer.Top)               -- Calibrate coordinate plane to the given trailer
plane:measure(calx.motors.trailer.Top)                 -- Measure coordinate plane starting at the given trailer
plane:fmove(calx.point.new(0.5, 0.5), speed)           -- Move coordinate plane pointer to the point (x;y) where x and y are in [0; 1] with given speed
plane:configure(calx.point.new(0.5, 0.5), speed)       -- Combines measure and fmove operations
plane:position()                                       -- Obtain coordinate plane pointer position
plane:size()                                           -- Obtain coordinate plane size
plane:measured()                                       -- Check if coordinate plane is measured
plane:asCenter()                                       -- Use current position as the center
plane:openWatcher()                                    -- Open watcher dialog for this plane
```

### Tasks
Lua bindings include basic facilities for task management using `calx.tasks` object.
```lua
calx.tasks:count()                      -- Count of currently registered tasks
calx.tasks:remove(0)                    -- Remove Nth task
calx.tasks:gcodeFile(title, filePath)   -- Load GCode from file, giving the title to new task
calx.tasks:gcode(title, code)           -- Load GCode from string, giving the title to new task
calx.tasks:linear(title, calx.tasks.linearTask.new(calx.rect.new(x, y, w, h), speed, true)) -- Create linear grid task for the rectangle [x; y; width; height] with given speed and vertical/horizontal lines
```

### Math
Lua bindings provide basic control math formula UI using `calx.math` object.
```lua
calx.math:count()                             -- Registered math formula count
calx.math:add(title, formula, {               -- Register a new formula with the given title
  { name='x', description='Variable x' }      -- and a table of variable descriptions
})
calx.math:remove(0)                           -- Remove Nth formula
```

### Journaling
Lua bindings provide access to the system journal and logs, using `calx.logger` and `calx.journal` objects. Both objects implement the same interface - multiple methods that log the message with given severity.
```lua
calx.journal:debug('Message')
calx.journal:info('Message')
calx.journal:warning('Message')
calx.journal:error('Message')
calx.journal:critical('Message')
```