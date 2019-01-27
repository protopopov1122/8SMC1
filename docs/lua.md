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

### TODO